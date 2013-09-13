/**********************************************************************
 * GBDI Arboretum - Copyright (c) 2002-2004 GBDI-ICMC-USP
 *
 *                           Homepage: http://gbdi.icmc.usp.br/arboretum
 **********************************************************************/

#include "stBTree.h"

/* ====================================================================
 * The GBDI-ICMC-USP Software License Version 1.0
 *
 * Copyright (c) 2004 Grupo de Bases de Dados e Imagens, Instituto de
 * Ci???ncias Matem???ticas e de Computa??????o, University of S???o Paulo -
 * Brazil (the Databases and Image Group - Intitute of Matematical and 
 * Computer Sciences).  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by Grupo de Bases
 *        de Dados e Imagens, Instituto de Ci???ncias Matem???ticas e de
 *        Computa??????o, University of S???o Paulo - Brazil (the Databases 
 *        and Image Group - Intitute of Matematical and Computer 
 *        Sciences)"
 *
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names of the research group, institute, university, authors
 *    and collaborators must not be used to endorse or promote products
 *    derived from this software without prior written permission.
 *
 * 5. The names of products derived from this software may not contain
 *    the name of research group, institute or university, without prior
 *    written permission of the authors of this software.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OF THIS SOFTWARE OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * ====================================================================
 *                                            http://gbdi.icmc.usp.br/
 */
/**
 * @file
 *
 * This file is the implementation of stBPlusTree methods.
 *
 * @version 1.0
 * @author Daniel dos Santos Kaster (dskaster@uel.br)
 */

//==============================================================================
// Class stBPlusTree
//------------------------------------------------------------------------------

template < class KeyType, class ObjectType, class Comparator >
stBPlusTree<KeyType, ObjectType, Comparator>::stBPlusTree(stPageManager * pageManager, bool duplicationAllowed) {

    PageManager = pageManager;
    
    this->duplicationAllowed = duplicationAllowed;
    
    this->comparationFunctionCount = 0;

    // Load the header page
    HeaderPage = PageManager->GetHeaderPage();
    if (HeaderPage->GetPageSize() < sizeof (stBPlusTreeHeader)) {
#ifdef __stDEBUG__
        cout << "The page size is too small to store the tree header." << endl;
        throw page_size_error("The page size is too small to store the tree header.");
#endif //__stDEBUG__
    }//end if

    // Set header
    Header = (stBPlusTreeHeader *) HeaderPage->GetData();

    // Is the tree empty?
    if (PageManager->IsEmpty()) {
        // Yes, create a default header
        HeaderPage->Clear();
        Header->Magic[0] = 'B';
        Header->Magic[1] = 'P';
        Header->Magic[2] = 'L';
        Header->Magic[3] = 'S';
        Header->RootPageID = 0;
        Header->LeftmostLeafPageID = 0;
        Header->RightmostLeafPageID = 0;
        Header->Height = 0;
        Header->ObjectCount = 0;
        Header->IndexNodeCount = 0;
        Header->LeafNodeCount = 0;

        // Schedule header page for update
        HeaderUpdate = true;
    }//end if
    else {
        HeaderUpdate = false;
    }

}//end stBPlusTree::stBPlusTree

//------------------------------------------------------------------------------

template < class KeyType, class ObjectType, class Comparator >
stBPlusTree<KeyType, ObjectType, Comparator>::~stBPlusTree() {

    if (HeaderUpdate) {
        PageManager->WriteHeaderPage(HeaderPage);
    }//end if
    PageManager->ReleasePage(HeaderPage);
}//end stBPlusTree::~stBPlusTree()

//------------------------------------------------------------------------------

template < class KeyType, class ObjectType, class Comparator >
bool stBPlusTree<KeyType, ObjectType, Comparator>::Insert(KeyType key, ObjectType * obj) {

    // Is not there a root ?
    if (Header->RootPageID == 0) {
        // No! We shall create the new LEAF root node.
        stPage * page;
        tBLeafNode * leafNode;
        int insertIdx;

        page = PageManager->GetNewPage();
        leafNode = new tBLeafNode(PageManager, page, true);
        
        // Insert the new object.
        unsigned int insertResult = leafNode->Insert(key, obj->GetSerializedSize(), obj->Serialize(), duplicationAllowed);

        if (insertResult != tBNode::SUCCESS && insertResult != tBNode::DUPLICATION) {
            // Oops. There is an error during the insertion.
            // free resources
            delete leafNode;
            PageManager->DisposePage(page);

#ifdef __stDEBUG__
            cout << "The page size is too small to store the first object. Increase it!" << endl;
            throw page_size_error("The page size is too small to store the first object. Increase it!");
#endif //__stDEBUG__

            return false;
        }//end if

        // The new object was inserted.
        // Update the root pointer
        Header->RootPageID = page->GetPageID();

        // Update the leftmost and rightmost leaf pointers
        Header->LeftmostLeafPageID = Header->RightmostLeafPageID = Header->RootPageID;

        // Update the Height
        Header->Height++;

        // Update the leaf node count
        Header->LeafNodeCount++;

//        cout << endl << "root:";
//        for (int i=0; i<leafNode->GetNumberOfEntries(); i++) {
//            cout << " " << leafNode->GetKeyAt(i);
//        }
//        cout << endl;

        // free resources
        delete leafNode;
        PageManager->ReleasePage(page);
    }//end if
    else {
        // Variables passed by reference to InsertRecursive to manage splits
        stPageID newNodeRepRightPageID = 0;
        KeyType newNodeRepKey;

        // Let's search
        if (InsertRecursive(Header->RootPageID, key, obj, newNodeRepKey, newNodeRepRightPageID) == PROMOTION) {
            // Split occurred! We must create a new INDEX root node
            stPage * page;
            tBIndexNode * indexNode;
            typename tBIndexNode::stBIndexNodeEntry entry;

            page = PageManager->GetNewPage();
            indexNode = new tBIndexNode(PageManager, page, true);

            // Set the new representative entry
            entry.Key = newNodeRepKey;
            entry.RightPageID = newNodeRepRightPageID;

            //cout << endl << "up: " << entry.Key << " at 0 pageId: " << entry.RightPageID << endl;

            // Insert entry in the first position
            if (!indexNode->InsertEntryAt(0, entry)) {
                // Oops, an error occurred!
                // free resources
                delete indexNode;
                PageManager->DisposePage(page);

#ifdef __stDEBUG__
                cout << "Error inserting index entry." << endl;
                throw logic_error("Error inserting index entry.");
#endif //__stDEBUG__

                return false;
            }//end if

            // Set the old root as entry's left son
            indexNode->SetLeftPageIDAt(0, Header->RootPageID);

            // Update the root pointer
            Header->RootPageID = page->GetPageID();

            // Update the height
            Header->Height++;

            // Update the index node count
            Header->IndexNodeCount++;

//            cout << endl << "root: ";
//            for (int i=0; i<indexNode->GetNumberOfEntries(); i++) {
//                cout << indexNode->GetEntryAt(i).Key;
//            }
//            cout << endl;

            // free resources
            delete indexNode;
            PageManager->ReleasePage(page);
        }//end if
    }//end else

    // Update object count.
    Header->ObjectCount++;

    // Schedule writing the header page
    HeaderUpdate = true;

    return true;
}//end stBPlusTree::Insert()

//------------------------------------------------------------------------------

template < class KeyType, class ObjectType, class Comparator >
int stBPlusTree<KeyType, ObjectType, Comparator>::InsertRecursive(
    stPageID currPageID, KeyType newKey, ObjectType * newObj, KeyType & newNodeRepKey,
    stPageID & newNodeRepRightPageID) {

    stPage * currPage; // Current page
    stPage * newPage; // New page
    tBNode * currNode; // Current node
    tBIndexNode * indexNode; // Current index node.
    tBIndexNode * newIndexNode; // New index node for splits
    tBLeafNode * leafNode; // Current leaf node.
    tBLeafNode * newLeafNode; // New leaf node.
    stPageID lowerLevelPageID;

    int insertIdx; // Insert index.
    stInsertAction result; // Returning value.

    currPage = PageManager->GetPage(currPageID);
    currNode = tBNode::CreateNode(PageManager, currPage);

    // What shall I do ?
    if (currNode->GetNodeType() == tBNode::INDEX) {
        KeyType subNodeRepKey;
        stPageID subNodeRepRightPageID;

        // Index Node cast.
        indexNode = (tBIndexNode *) currNode;

//        cout << endl << "index before:";
//        for (int i=0; i<indexNode->GetNumberOfEntries(); i++) {
//            cout << " " << indexNode->GetEntryAt(i).Key;
//        }
//        cout << endl;

        // Which subtree must be traversed?
        insertIdx = indexNode->Find(newKey);
//        cout << "subtree: " << insertIdx << endl;

        // Which lower level page should be traversed?
        if (insertIdx < indexNode->GetNumberOfEntries()) {
            lowerLevelPageID = indexNode->GetLeftPageIDAt(insertIdx);
        }
        else {
            lowerLevelPageID = indexNode->GetRightPageIDAt(insertIdx - 1);
        }

        // Try to insert
        if (InsertRecursive(lowerLevelPageID, newKey,
                newObj, subNodeRepKey, subNodeRepRightPageID) == PROMOTION) {
            // A split occurred in the lower level
            // Set the new representative entry
            typename tBIndexNode::stBIndexNodeEntry newEntry;
            newEntry.Key = subNodeRepKey;
            newEntry.RightPageID = subNodeRepRightPageID;

            //cout << endl << "index up: " << newEntry.Key << " at " << insertIdx << " pageId: " << newEntry.RightPageID << endl;

            // Try to insert newEntry in this index node
            if (indexNode->InsertEntryAt(insertIdx, newEntry)) {
                // Success, nothing to do in the upper levels.
                result = NO_ACT;
            }//end if
            else {
                // It is necessary to split this index node
                newPage = PageManager->GetNewPage();
                newIndexNode = new tBIndexNode(PageManager, newPage, true);

                // Split it
                SplitIndex(indexNode, newIndexNode, newEntry, newNodeRepKey);
                newNodeRepRightPageID = newPage->GetPageID();

                // Update the index node count
                Header->IndexNodeCount++;

                // Free resources
                delete newIndexNode;
                PageManager->ReleasePage(newPage);

                // Report split
                result = PROMOTION;
            }//end else
        }//end if
        else {
            // Nothing to do, just return.
            result = NO_ACT;
        }//end else

//        cout << endl << "index after:";
//        for (int i=0; i<indexNode->GetNumberOfEntries(); i++) {
//            cout << " " << indexNode->GetEntryAt(i).Key;
//        }
//        cout << endl << "Free: " << indexNode->GetFree() << endl;
    }//end if
    else {
        // currNode is a leaf node
        leafNode = (tBLeafNode *) currNode;

//        cout << endl << "leaf before:";
//        for (int i=0; i<leafNode->GetNumberOfEntries(); i++) {
//            cout << " " << leafNode->GetKeyAt(i);
//        }
//        cout << endl << "leaf Free: " << leafNode->GetFree() << endl;


        // Try to insert the element
        unsigned int status = leafNode->Insert(newKey, newObj->GetSerializedSize(), newObj->Serialize(), duplicationAllowed);

        if (status == tBNode::ERROR) {
            // Unrecoverable error when inserting element. Abandon the insertion.
            result = NO_ACT;
        }//end if
        else if (status == tBNode::SUCCESS) {
            // Success, nothing to do in the upper levels.
            result = NO_ACT;
        }//end if
        else if (status == tBNode::DUPLICATION) {
            // Duplication in a tree that doesn't allow it, nothing to do in the upper levels.
            result = NO_ACT;
        }//end if
        else if (status == tBNode::SUCCESS_NEWOVERFLOWNODE) {
            // Update node count
            Header->LeafNodeCount++;

            // Success, nothing to do in the upper levels.
            result = NO_ACT;
        }//end if
        else {
            // A split is required
            newPage = PageManager->GetNewPage();
            newLeafNode = new tBLeafNode(PageManager, newPage, true);

            // Split leafNode
            SplitLeaf(leafNode, newLeafNode, newKey, newObj, newNodeRepKey);
            newNodeRepRightPageID = newPage->GetPageID();

            // If leafNode was the rightmost leaf, the RightmostLeaf pointer
            // needs to be updated.
            if (Header->RightmostLeafPageID == leafNode->GetPageID()) {
                Header->RightmostLeafPageID = newLeafNode->GetPageID();
            }

            // Update the leaf node count
            Header->LeafNodeCount++;

            // Free resources
            delete newLeafNode;
            PageManager->ReleasePage(newPage);

            // Report split
            result = PROMOTION;
        }//end else

//        cout << endl << "leaf after:";
//        for (int i=0; i<leafNode->GetNumberOfEntries(); i++) {
//            cout << " " << leafNode->GetKeyAt(i);
//        }
//        cout << endl << "leaf Free: " << leafNode->GetFree() << endl;
    }//end else

    // Clean home
    delete currNode;
    PageManager->ReleasePage(currPage);

    return result;
}//end stBPlusTree::InsertRecursive()

//------------------------------------------------------------------------------

template < class KeyType, class ObjectType, class Comparator >
void stBPlusTree<KeyType, ObjectType, Comparator>::SplitIndex(
    tBIndexNode * leftNode, tBIndexNode * rightNode,
    typename tBIndexNode::stBIndexNodeEntry newEntry, KeyType & newNodeRepKey) {

    stCount leftMedianIdx;
    stCount leftNumEntries;
    stCount idx;

    // Get the median element and the number of entries of the left node
    leftMedianIdx = leftNode->FindMedian();
    leftNumEntries = leftNode->GetNumberOfEntries();

    // Copy left entries greater than the median to the right node
    for (idx = leftMedianIdx; idx < leftNumEntries; idx++) {
        rightNode->InsertEntryAt((idx - leftMedianIdx), leftNode->GetEntryAt(idx));
    }//end for

    // Delete such entries from the left node
    // They are deleted from the last to the median to avoid idx OutOfBounds exceptions.
    for (idx = leftNumEntries - 1; idx >= leftMedianIdx; idx--) {
        leftNode->DeleteEntryAt(idx);
    }//end for

    // In which node should newEntry be inserted?
    if (newEntry.Key <= leftNode->GetEntryAt(leftMedianIdx - 1).Key) {
        // Insert into the left one
        idx = leftNode->Find(newEntry.Key);
        leftNode->InsertEntryAt(idx, newEntry);

        // Set the last element of the left node as the median element to come up
        leftNumEntries = leftNode->GetNumberOfEntries();
        newNodeRepKey = leftNode->GetEntryAt(leftNumEntries - 1).Key;

        // Set the leftmost pointer of the right node as the entry's (right) pointer
        rightNode->SetLeftPageIDAt(0, leftNode->GetEntryAt(leftNumEntries - 1).RightPageID);

        // Delete entry
        leftNode->DeleteEntryAt(leftNumEntries - 1);
    }//end if
    else {
        // Insert into the right node
        idx = rightNode->Find(newEntry.Key);
        rightNode->InsertEntryAt(idx, newEntry);

        // Set the first element of the right node as the median element to come up
        newNodeRepKey = rightNode->GetEntryAt(0).Key;

        // Set the leftmost pointer of the right node as the entry's (right) pointer
        rightNode->SetLeftPageIDAt(0, rightNode->GetEntryAt(0).RightPageID);

        // Delete entry
        rightNode->DeleteEntryAt(0);
    }//end else

//    cout << endl << "index left:";
//    for (int i=0; i<leftNode->GetNumberOfEntries(); i++) {
//        cout << " " << leftNode->GetEntryAt(i).Key;
//    }
//    cout << endl << "left Free: " << leftNode->GetFree() << endl;
//
//    cout << endl << "index right:";
//    for (int i=0; i<rightNode->GetNumberOfEntries(); i++) {
//        cout << " " << rightNode->GetEntryAt(i).Key;
//    }
//    cout << endl << "left Free: " << rightNode->GetFree() << endl;

}//end stBPlusTree::SplitIndex()

//------------------------------------------------------------------------------

template < class KeyType, class ObjectType, class Comparator >
void stBPlusTree<KeyType, ObjectType, Comparator>::SplitLeaf(
    tBLeafNode * leftNode, tBLeafNode * rightNode, KeyType newKey, ObjectType * newObj,
    KeyType & newNodeRepKey) {

    stCount numEntries;
    stCount medianIdx;
    long idx;

//    if (newKey > 36.39 && newKey < 36.41) {
//        cout << "AQUI: " << fixed << newKey << endl;
//    }

    numEntries = leftNode->GetNumberOfEntries();
    medianIdx = leftNode->FindMedian();

    // Is node full of elements with the same key?
    if (medianIdx == 0) {
        // Yes. Check if newKey should be inserted at right
        if (newKey > leftNode->GetKeyAt(medianIdx)) {
            unsigned int insertResult = rightNode->Insert(newKey, newObj->GetSerializedSize(), newObj->Serialize(), duplicationAllowed);

            if (insertResult != tBNode::SUCCESS && insertResult != tBNode::DUPLICATION) {
                throw page_size_error("The page size is too small to store the element.");
            }//end if

//            cout << endl << "left: ";
//            for (int i = 0; i < leftNode->GetNumberOfEntries(); i++)
//                cout << leftNode->GetKeyAt(i) << " ";
//            cout << endl << "right: ";
//            for (int i = 0; i < rightNode->GetNumberOfEntries(); i++)
//                cout << rightNode->GetKeyAt(i) << " ";

            return;
        }
    }//end if

    // Copy entries greater than or equal to the median to the right node. We perform this
    // from the median to the end to avoid unnecessary object data memory
    // movements when inserting them in the right node.
    for (idx = medianIdx; idx < numEntries; idx++) {
        unsigned int insertResult = rightNode->Insert(leftNode->GetKeyAt(idx), leftNode->GetSerializedObjectSizeAt(idx), leftNode->GetSerializedObjectAt(idx), duplicationAllowed);

        if (insertResult != tBNode::SUCCESS && insertResult != tBNode::DUPLICATION) {
            throw page_size_error("The page size is too small to store the element.");
        };
    }//end for

    // Delete such entries from the left node. We perform this from the end to
    // the median to avoid unnecessary object data memory movements when deleting
    // them from the left node.
    for (idx = numEntries; idx > medianIdx; idx--) {
        leftNode->DeleteElementAt(idx - 1);
    }//end for

    // Insert the new element.
    // Must the new element be inserted in the left node?
    if ((leftNode->GetNumberOfEntries() == 0) || (newKey <= leftNode->GetKeyAt(leftNode->GetNumberOfEntries() - 1))) {
        unsigned int insertResult = leftNode->Insert(newKey, newObj->GetSerializedSize(), newObj->Serialize(), duplicationAllowed);
        
        if (insertResult != tBNode::SUCCESS && insertResult != tBNode::DUPLICATION) {
            throw page_size_error("The page size is too small to store the element.");
        };
    }//end if
        // Must it be inserted in the right node?
    else if (newKey >= rightNode->GetKeyAt(0)) {
        unsigned int insertResult = rightNode->Insert(newKey, newObj->GetSerializedSize(), newObj->Serialize(), duplicationAllowed);
        
        if (insertResult != tBNode::SUCCESS && insertResult != tBNode::DUPLICATION) {
            throw page_size_error("The page size is too small to store the element.");
        };
    }// So, insert it in the node that is more free.
    else if (leftNode->GetFree() <= rightNode->GetFree()) {
        unsigned int insertResult = leftNode->Insert(newKey, newObj->GetSerializedSize(), newObj->Serialize(), duplicationAllowed);
        
        if (insertResult != tBNode::SUCCESS && insertResult != tBNode::DUPLICATION) {
            throw page_size_error("The page size is too small to store the element.");
        };
    } else {
        unsigned int insertResult = rightNode->Insert(newKey, newObj->GetSerializedSize(), newObj->Serialize(), duplicationAllowed);
        
        if (insertResult != tBNode::SUCCESS && insertResult != tBNode::DUPLICATION) {
            throw page_size_error("The page size is too small to store the element.");
        };
    }//end else

//    cout << "left: ";
//    for (int i = 0; i < leftNode->GetNumberOfEntries(); i++)
//        cout << leftNode->GetKeyAt(i) << " ";
//    cout << "\nright: ";
//    for (int i = 0; i < rightNode->GetNumberOfEntries(); i++)
//        cout << rightNode->GetKeyAt(i) << " ";

    // Adjust the LeafNode links
    rightNode->SetPreviousPageID(leftNode->GetPageID());
    if (leftNode->GetNextPageID() != 0) {
        rightNode->SetNextPageID(leftNode->GetNextPageID());
        stPage * nextPage = PageManager->GetPage(rightNode->GetNextPageID());
        tBLeafNode * nextNode = new tBLeafNode(PageManager, nextPage, false);
        nextNode->SetPreviousPageID(rightNode->GetPageID());
        delete nextNode;
    }
    leftNode->SetNextPageID(rightNode->GetPageID());

    // Set the representative key of these two nodes as the last element
    // of leftNode
    newNodeRepKey = leftNode->GetKeyAt(leftNode->GetNumberOfEntries() - 1);

}//end stBPlusTree::SplitLeaf()

//------------------------------------------------------------------------------

template < class KeyType, class ObjectType, class Comparator >
stTOResult <ObjectType, KeyType> * stBPlusTree<KeyType, ObjectType, Comparator>::QueryEqual(KeyType key) {

    this->resetComparationFunctionCount();
    
    stPageID currPageID;
    stPage * currPage; // Current page
    tBNode * currNode; // Current node
    tBIndexNode * indexNode; // Current index node.
    tBLeafNode * leafNode; // Current leaf node.

    tResult * result;
    ObjectType * obj;

    stCount idx;
    
    this->resetComparationFunctionCount();

    // Create result
    result = new tResult();
    result->SetQueryInfo(TO_EQUALQUERY, key, key);

    currPageID = Header->RootPageID;

    // Let's search
    while (currPageID != 0) {
        currPage = PageManager->GetPage(currPageID);
        currNode = tBNode::CreateNode(PageManager, currPage);

        // What shall I do ?
        if (currNode->GetNodeType() == tBNode::INDEX) {

            // Index Node cast.
            indexNode = (tBIndexNode *) currNode;

            // Which subtree must be traversed?
            idx = indexNode->Find(key);
//            cout << "subtree: " << idx << endl;

            // Which lower level page should be traversed?
            if (idx < indexNode->GetNumberOfEntries()) {
                currPageID = indexNode->GetLeftPageIDAt(idx);
            } else {
                currPageID = indexNode->GetRightPageIDAt(idx - 1);
            }
        } else {
            // currNode is a leaf node
            // Leaf Node cast.
            leafNode = (tBLeafNode *) currNode;

            if (leafNode->FindFirst(key, idx)) {
                // The pair insertions will be O(1), because of the iterator position
                while ((idx < leafNode->GetNumberOfEntries()) && (equal(key, leafNode->GetKeyAt(idx)))) {
                    obj = new ObjectType();
                    obj->Unserialize(leafNode->GetSerializedObjectAt(idx), leafNode->GetSerializedObjectSizeAt(idx));
                    result->AddPair(obj, leafNode->GetKeyAt(idx), result->endPairs());
                    idx++;
                }
            }
            currPageID = 0;
        }

        // Clean home
        delete currNode;
        PageManager->ReleasePage(currPage);
    }//end while

    return result;
}// end stBPlusTree::QueryEqual()

//------------------------------------------------------------------------------

template < class KeyType, class ObjectType, class Comparator >
stTOResult <ObjectType, KeyType> * stBPlusTree<KeyType, ObjectType, Comparator>::QueryBetween(KeyType lowerBound, KeyType upperBound) {

    this->resetComparationFunctionCount();
    
    stPageID currPageID;
    stPageID nextLeafPageID;
    stPage * currPage; // Current page
    stPage * nextLeafPage; // Next leaf page
    tBNode * currNode; // Current node
    tBIndexNode * indexNode; // Current index node.
    tBLeafNode * leafNode; // Current leaf node.
    tBLeafNode * nextLeafNode; // Next leaf node

    tResult * result;
    ObjectType * obj;

    stCount idx;
    
    this->resetComparationFunctionCount();

    // Create result
    result = new tResult();
    result->SetQueryInfo(TO_BETWEENQUERY, lowerBound, upperBound);

    currPageID = Header->RootPageID;

    // Let's search
    while (currPageID != 0) {
        currPage = PageManager->GetPage(currPageID);
        currNode = tBNode::CreateNode(PageManager, currPage);

        // What shall I do ?
        if (currNode->GetNodeType() == tBNode::INDEX) {

            // Index Node cast.
            indexNode = (tBIndexNode *) currNode;

            // Which subtree must be traversed?
            idx = indexNode->Find(lowerBound);
//            cout << "subtree: " << idx << endl;

            // Which lower level page should be traversed?
            if (idx < indexNode->GetNumberOfEntries()) {
                currPageID = indexNode->GetLeftPageIDAt(idx);
            } else {
                currPageID = indexNode->GetRightPageIDAt(idx - 1);
            }
        } else {
            // currNode is a leaf node
            // Leaf Node cast.
            leafNode = (tBLeafNode *) currNode;

            leafNode->FindFirst(lowerBound, idx);

            // Get entries in the current node.
            while ((idx < leafNode->GetNumberOfEntries()) && (!less(upperBound, leafNode->GetKeyAt(idx)))) {
                obj = new ObjectType();
                obj->Unserialize(leafNode->GetSerializedObjectAt(idx), leafNode->GetSerializedObjectSizeAt(idx));
                result->AddPair(obj, leafNode->GetKeyAt(idx), result->endPairs());
                idx++;
            }

            // Get entries in the following leaf nodes.
            nextLeafPageID = leafNode->GetNextPageID();

            while (nextLeafPageID != 0) {
                nextLeafPage = PageManager->GetPage(nextLeafPageID);
                nextLeafNode = new tBLeafNode(PageManager, nextLeafPage, false);

                idx = 0;
                // Get entries in the current node.
                while ((idx < nextLeafNode->GetNumberOfEntries()) && (!less(upperBound, nextLeafNode->GetKeyAt(idx)))) {
                    obj = new ObjectType();
                    obj->Unserialize(nextLeafNode->GetSerializedObjectAt(idx), nextLeafNode->GetSerializedObjectSizeAt(idx));
                    result->AddPair(obj, nextLeafNode->GetKeyAt(idx), result->endPairs());
                    idx++;
                }

                // Schedule visiting the next leaf node.
                nextLeafPageID = nextLeafNode->GetNextPageID();

                // Free leaf node resources
                delete nextLeafNode;
                PageManager->ReleasePage(nextLeafPage);
            }

            currPageID = 0;
        }

        // Clean home
        delete currNode;
        PageManager->ReleasePage(currPage);
    }//end while

    return result;
}// end stBPlusTree::QueryBetween()

//------------------------------------------------------------------------------

template < class KeyType, class ObjectType, class Comparator >
stTOResult <ObjectType, KeyType> * stBPlusTree<KeyType, ObjectType, Comparator>::QueryLessThanOrEqual(KeyType key) {

    this->resetComparationFunctionCount();
    
    stPageID currPageID;
    stPage * currPage; // Current page
    tBLeafNode * leafNode; // Current leaf node.

    tResult * result;
    ObjectType * obj;

    stCount idx;
    bool stop;
    
    this->resetComparationFunctionCount();

    // Create result
    result = new tResult();
    result->SetQueryInfo(TO_LESSTHANOREQUALQUERY, 0, key);

    // Get entries from the leftmost leaf node.
    currPageID = Header->LeftmostLeafPageID;
    stop = false;

    while (currPageID != 0) {
        currPage = PageManager->GetPage(currPageID);
        leafNode = new tBLeafNode(PageManager, currPage, false);

        // Get entries
        for (idx = 0; idx < leafNode->GetNumberOfEntries(); idx++) {
            if (less(key, leafNode->GetKeyAt(idx))) {
                stop = true;
                break;
            }
            obj = new ObjectType();
            obj->Unserialize(leafNode->GetSerializedObjectAt(idx), leafNode->GetSerializedObjectSizeAt(idx));
            result->AddPair(obj, leafNode->GetKeyAt(idx), result->endPairs());
        }//end for

        // Schedule visiting the next leaf node.
        if (!stop) {
            currPageID = leafNode->GetNextPageID();
        }
        else {
            currPageID = 0;
        }

        // Free leaf node resources
        delete leafNode;
        PageManager->ReleasePage(currPage);
    }//end while

    return result;
}// end stBPlusTree::QueryLessThanOrEqual()

//------------------------------------------------------------------------------

template < class KeyType, class ObjectType, class Comparator >
stTOResult <ObjectType, KeyType> * stBPlusTree<KeyType, ObjectType, Comparator>::QueryLessThan(KeyType key) {

    this->resetComparationFunctionCount();
    
    stPageID currPageID;
    stPage * currPage; // Current page
    tBLeafNode * leafNode; // Current leaf node.

    tResult * result;
    ObjectType * obj;

    stCount idx;
    bool stop;
    
    this->resetComparationFunctionCount();

    // Create result
    result = new tResult();
    result->SetQueryInfo(TO_LESSTHANQUERY, 0, key);

    // Get entries from the leftmost leaf node.
    currPageID = Header->LeftmostLeafPageID;
    stop = false;

    while (currPageID != 0) {
        currPage = PageManager->GetPage(currPageID);
        leafNode = new tBLeafNode(PageManager, currPage, false);

        // Get entries
        for (idx = 0; idx < leafNode->GetNumberOfEntries(); idx++) {
            if (!less(leafNode->GetKeyAt(idx), key)) {
                stop = true;
                break;
            }
            obj = new ObjectType();
            obj->Unserialize(leafNode->GetSerializedObjectAt(idx), leafNode->GetSerializedObjectSizeAt(idx));
            result->AddPair(obj, leafNode->GetKeyAt(idx), result->endPairs());
        }//end for

        // Schedule visiting the next leaf node.
        if (!stop) {
            currPageID = leafNode->GetNextPageID();
        }
        else {
            currPageID = 0;
        }

        // Free leaf node resources
        delete leafNode;
        PageManager->ReleasePage(currPage);
    }//end while

    return result;
}// end stBPlusTree::QueryLessThan()

//------------------------------------------------------------------------------

template < class KeyType, class ObjectType, class Comparator >
stTOResult <ObjectType, KeyType> * stBPlusTree<KeyType, ObjectType, Comparator>::QueryGreaterThanOrEqual(KeyType key) {

    this->resetComparationFunctionCount();
    
    stPageID currPageID;
    stPage * currPage; // Current page
    tBLeafNode * leafNode; // Current leaf node.

    tResult * result;
    ObjectType * obj;

    stCount idx;
    bool stop;
    
    this->resetComparationFunctionCount();

    // Create result
    result = new tResult();
    result->SetQueryInfo(TO_GREATERTHANOREQUALQUERY, key, 0);

    // Get entries from the rightmost leaf node.
    currPageID = Header->RightmostLeafPageID;
    stop = false;

    while (currPageID != 0) {
        currPage = PageManager->GetPage(currPageID);
        leafNode = new tBLeafNode(PageManager, currPage, false);

        // Get entries
        for (idx = leafNode->GetNumberOfEntries(); idx > 0; idx--) { // I will use (idx - 1) to avoid negative values in stCount
            if (less(leafNode->GetKeyAt(idx - 1), key)) {
                stop = true;
                break;
            }
            obj = new ObjectType();
            obj->Unserialize(leafNode->GetSerializedObjectAt(idx - 1), leafNode->GetSerializedObjectSizeAt(idx - 1));
            result->AddPair(obj, leafNode->GetKeyAt(idx - 1), result->beginPairs());
        }//end for

        // Schedule visiting the next leaf node.
        if (!stop) {
            currPageID = leafNode->GetPreviousPageID();
        }
        else {
            currPageID = 0;
        }

        // Free leaf node resources
        delete leafNode;
        PageManager->ReleasePage(currPage);
    }//end while

    return result;
}// end stBPlusTree::QueryGreaterThanOrEqual()

//------------------------------------------------------------------------------

template < class KeyType, class ObjectType, class Comparator >
stTOResult <ObjectType, KeyType> * stBPlusTree<KeyType, ObjectType, Comparator>::QueryGreaterThan(KeyType key) {

    this->resetComparationFunctionCount();
    
    stPageID currPageID;
    stPage * currPage; // Current page
    tBLeafNode * leafNode; // Current leaf node.

    tResult * result;
    ObjectType * obj;

    stCount idx;
    bool stop;
    
    this->resetComparationFunctionCount();

    // Create result
    result = new tResult();
    result->SetQueryInfo(TO_GREATERTHANQUERY, key, 0);

    // Get entries from the rightmost leaf node.
    currPageID = Header->RightmostLeafPageID;
    stop = false;

    while (currPageID != 0) {
        currPage = PageManager->GetPage(currPageID);
        leafNode = new tBLeafNode(PageManager, currPage, false);

        // Get entries
        for (idx = leafNode->GetNumberOfEntries(); idx > 0; idx--) { // I will use (idx - 1) to avoid negative values in stCount
            if (!less(key, leafNode->GetKeyAt(idx - 1))) {
                stop = true;
                break;
            }
            obj = new ObjectType();
            obj->Unserialize(leafNode->GetSerializedObjectAt(idx - 1), leafNode->GetSerializedObjectSizeAt(idx - 1));
            result->AddPair(obj, leafNode->GetKeyAt(idx - 1), result->beginPairs());
        }//end for

        // Schedule visiting the next leaf node.
        if (!stop) {
            currPageID = leafNode->GetPreviousPageID();
        }
        else {
            currPageID = 0;
        }

        // Free leaf node resources
        delete leafNode;
        PageManager->ReleasePage(currPage);
    }//end while

    return result;
}// end stBPlusTree::QueryGreaterThan()

//------------------------------------------------------------------------------
