/**
* @file
*
* This file defines the Rtree nodes.
*
* @todo Everything.
*/
// Copyright (c) 2002-2003 GBDI-ICMC-USP

#ifndef __STRNODE_H
#define __STRNODE_H

#include <gbdi/arboretum/storage/stPage.h>
#include <gbdi/datatype/stTypes.h>
#include <gbdi/util/stException.h>

//-----------------------------------------------------------------------------
// Class stRNode
//-----------------------------------------------------------------------------
/**
* This abstract class is the basic RTree node. All classes that implement
* RTree nodes must extend this class.
*
* <p>The main function of this class is to provide a way to identify a disk node
* and create the required node instance to manipulate the node.
*
* <p>The structure of Index Node follows:
* @image html Rnode.png "R node structure"
*
* <p>The <b>Header</b> holds the information about the node itself.
*     - Type: Type of this node. It may be stRNode::INDEX or stRNode::LEAF.
*     - Occupation: Number of entries in this node.
*
* <p>The <b>Node Data</b> is the segment of the node which holds the particular information
* of each type of the node. This class does not know how this information is organized.
*
* @version 1.0
* @todo Documentation review.
* @see stRIndexNode
* @see stRLeafNode
* @ingroup slim
*/
class stRNode{
   public:

      /**
      * Node type.
      */
      enum stRNodeType{
         /**
         * ID of an index node.
         */
         INDEX = 0x4449, // In little endian "ID"

         /**
         * ID of a leaf node.
         */
         LEAF = 0x464C // In little endian "LF"
      };//end stNodeType

      /**
      * This method will dispose this instance and all associated resources.
      */
      virtual ~stRNode(){
      }//end ~stRNode()

      /**
      * Returns the type of this RTree node (Leaf or Index).
      *
      * @return the type of node.
      * @see stNodeType
      */
      stNodeType GetNodeType(){
         return Header->Type;
      }//end GetNodeType

      /**
      * Returns the associated page.
      *
      * @return The associated page.
      */
      stPage * GetPage(){
         return Page;
      }//end GetPage

      /**
      * Returns the ID of the associated page.
      *
      * @return The ID of the associated page.
      */
      stPageID GetPageID(){
         return Page->GetPageID();
      }//end GetPage

      /**
      * This is a virtual method that defines a interface for the instantiate
      * the correct specialization of this class.
      *
      * @param page The instance of stPage.
      */
      static stRNode * CreateNode(stPage * page);

      /**
      * Returns the number of entries in this node.
      *
      * @return the number of entries.
      * @see GetEntry()
      * @see GetObject()
      * @see GetObjectSize()
      */
      stCount GetNumberOfEntries(){
         return this->Header->Occupation;
      }//end GetNumberOfEntries

      /**
      * This is a virtual method that defines a interface in the insertion of a
      * new Object in a Node.
      *
      * @param size The size of the object in bytes.
      * @param object The object data.
      * @warning The parameter size is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @return The position in the vector Entries.
      * @see RemoveObject()
      */
      virtual int AddEntry(stSize size, const stByte * object) = 0;

      /**
      * Gets the serialized object. Use GetObjectSize to determine the size of
      * the object.
      *
      * @param idx The idx of the entry.
      * @warning The parameter idx is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @return A pointer to the serialized object.
      * @see GetObjectSize()
      */
      virtual const stByte * GetObject(stCount idx) = 0;

      /**
      * Returns the size of the object in bytes. Use GetObject() to get the
      * object data.
      *
      * @param idx The idx of the entry.
      * @warning The parameter idx is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @return The size of the serialized object.
      * @see GetObject()
      */
      virtual stSize GetObjectSize(stCount idx) = 0;

      /**
      * Remove All entries.
      */
      void RemoveAll(){
         #ifdef __stDEBUG__
         stNodeType type;
         type = Header->Type;
         Page->Clear();
         Header->Type = type;
         #else
         this->Header->Occupation = 0;
         #endif //__stDEBUG__
      }//end RemoveAll

      /**
      * Returns the total number of objects in the subtree.
      */
      virtual stCount GetTotalObjectCount() = 0;

      /**
      * Returns the global overhead of a R node (header size) in bytes.
      */
      static stSize GetGlobalOverhead(){
         return sizeof(stRNodeHeader);
      }//end GetGlobalOverhead()

   protected:
      /**
      * This is the structure of the Header of a RTree node.
      */
      #pragma pack(1)
      typedef struct stRNodeHeader{
         /**
         * Node type.
         */
         stNodeType Type;

         /**
         * Number of entries.
         */
         stCount Occupation;
      } stRNodeHeader; //end stHeader
      #pragma pack()

      /**
      * Header of this page.
      */
      stRNodeHeader * Header;

      /**
      * The page related with this class.
      */
      stPage * Page;

      /**
      * Creates a new instance of this class.
      *
      * @param page An instance of stPage.
      */
      stRNode(stPage * page){
         this->Page = page;
         Header = (stRNodeHeader *)(this->Page->GetData());
      }//end stRNode
};//end stRNode

//-----------------------------------------------------------------------------
// Class stRIndexNode
//-----------------------------------------------------------------------------
/**
* This class implements the index node of the RTree.
*
* <P>The RTree index node...
*
* <P>The structure of Index Node follows:
* @image html indexnode.png "Index node structure"
*
* <P>The <b>Header</b> holds the information about the node itself.
*     - Type: Type of this node. It is always stRNode::INDEX (0x4449).
*     - Occupation: Number of entries in this node.
*
* <P>The <b>Entry</b> holds the information of the link to the other node.
*  - PageID: The identifier of the page which holds the root of the sub tree.
*       - NEntries: Number of objects in the sub tree.
*       - Offset: The offset of the object in the page. DO NOT MODIFY ITS VALUE.
*
* <P>The <b>Object</b> is an array of bytes that holds the information required to rebuild
* the original object.
*
* @version 1.0
* @todo Documentation review.
* @see stRNode
* @see stRLeafNode
* @ingroup R
*/
// +---------------------------------------------------------------------------------------------------------------------------------------------------+
// | Type | Occupation | PgID0 | NEnt0 | SonIsData0 | Dimension0 | Bounces0 | OffSet0 |...| PgIDn | NEntn | SonIsDatan | Dimensionn | Bouncesn | OffSetn | <-- blankspace --> |Objn |...|Obj0 |
// +---------------------------------------------------------------------------------------------------------------------------------------------------+
class stRIndexNode: public stRNode {
   public:
      /**
      * This type represents a R tree index node entry.
      */
      #pragma pack(1)
      typedef struct stRIndexEntry{
         /**
         * ID of the page.
         */
         stPageID PageID;

         /**
         * Number of entries in the sub-tree.
         */
         stCount NEntries;

         /**
         * True if index entry points to a leaf node or false if not
         */
         bool sonIsLeaf;

         /**
         * Offset of the object.
         * @warning NEVER MODIFY THIS FIELD. YOU MAY DAMAGE THE STRUCTURE OF THIS NODE.
         */
         stSize Offset;
      } stRIndexEntry; //end stIndexEntry
      #pragma pack()

      /**
      * Creates a new instance of this class. The parameter <i>page</i> is an
      * instance of stPage that hold the node data.
      *
      * <P>The parameter <i>create</i> tells to stIndexPage what operation will
      * be performed. True means that the page will be initialized and false
      * means that the page will be used as it is. The default value is false.
      *
      * @param page The page that hold the data of this node.
      * @param create The operation to be performed.
      */
      stRIndexNode(stPage * page, bool create = false);

      /**
      * Returns the reference of the desired entry. You may use this method to
      * read and modify the entry information.
      *
      * @param idx The idx of the entry.
      * @warning The parameter idx is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @return stIndexEntry the reference of the desired entry.
      * @see stLeafEntry
      * @see GetNumberOfEntries()
      * @see GetObject()
      * @see GetObjectSize()
      */
      stRIndexEntry & GetIndexEntry(stCount idx){
         #ifdef __stDEBUG__
         if (idx >= GetNumberOfEntries()){
            throw invalid_argument("idx value is out of range.");
         }//end if
         #endif //__stDEBUG__

         return Entries[idx];
      }//end GetIndexEntry

      /**
      * Adds a new entry to this node. This method will return the idx of the new
      * node or a negative value for failure.
      *
      * <P>This method will fail if there is not enough space to hold the
      * new object.
      *
      * <P>If you have added a new entry successfully, you may edit the entry
      * fields using the method GetEntry().
      *
      * @param size The size of the object in bytes.
      * @param object The object data.
      * @warning The parameter size is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @return The position in the vector Entries.
      * @see RemoveEntry()
      * @see GetEntry()
      */
      virtual int AddEntry(stSize size, const stByte * object);

      /**
      * Gets the serialized object. Use GetObjectSize to determine the size of
      * the object.
      *
      * @param idx The idx of the entry.
      * @warning The parameter idx is not verified by this implementation.
      * unless __stDEBUG__ is defined at compile time.
      * @return A pointer to the serialized object.
      * @see GetObjectSize()
      */
      const stByte * GetObject(stCount idx);

      /**
      * Returns the size of the object. Use GetObject() to get the object data.
      *
      * @param idx The idx of the entry.
      * @warning The parameter idx is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @return The size of the serialized object.
      * @see GetObject()
      */
      stSize GetObjectSize(stCount idx);

      /**
      * Removes an entry from this object.
      *
      * @param idx The idx of the entry.
      * @warning The parameter idx is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @see GetObjectSize()
      */
      void RemoveEntry(stCount idx);

      /**
      * Returns the total number of objects in the subtree.
      */
      virtual stCount GetTotalObjectCount();

      /**
      * Returns the overhead of each index node entry in bytes.
      */
      static stSize GetIndexEntryOverhead(){
         return sizeof(stRIndexEntry);
      }//end GetIndexEntryOverhead()

      /**
      * Returns the amount of the free space in this node.
      */
      stSize GetFree();

   private:
      /**
      * Entry pointer
      */
      stRIndexEntry * Entries;

};//end stRIndexPage

//-----------------------------------------------------------------------------
// Class stRLeafNode
//-----------------------------------------------------------------------------
/**
* This class implements the Leaf node of the RTree.
*
* <P>The RTree leaf node...
* The structure of Leaf Node follows:
* @image html leafnode.png "Leaf node structure"
*
* <P>The <b>Header</b> holds the information about the node itself.
*     - Type: Type of this node. It is always stRNode::LEAF (0x464C).
*     - Occupation: Number of entries in this node.
*
* <P>The <b>Entry</b> holds the information of the link to the other node.
*       - Offset: The offset of the object in the page. DO NOT MODIFY ITS VALUE.
*
* <P>The <b>Object</b> is an array of bytes that holds the information required
* to rebuild the original object.
*
* @version 1.0
* @todo Documentation review.
* @see stRNode
* @see stRIndexNode
* @see stRMemLeafNode
* @ingroup R
*/
// +--------------------------------------------------------------------------------------------+
// | Type | Occupation | OffSet0 | OffSetn | <-- blankspace --> |Objn |...|Obj0 |
// +--------------------------------------------------------------------------------------------+
class stRLeafNode: public stRNode {
   public:
      /**
      * This type represents a R tree leaf node entry.
      */
      #pragma pack(1)
      typedef struct stRLeafEntry{
         /**
         * Offset of the object.
         * @warning NEVER MODIFY THIS FIELD. YOU MAY DAMAGE THE STRUCTURE OF THIS NODE.
         */
         stSize Offset;
      } stRLeafEntry; //end stLeafEntry
      #pragma pack()

      /**
      * Creates a new instance of this class. The paramenter <i>page</i> is an
      * instance of stPage that hold the node data.
      *
      * <P>The parameter <i>create</i> tells to stLeafPage what operation will
      * be performed. True means that the page will be initialized and false
      * means that the page will be used as it is. The default value is false.
      *
      * @param page The page that hold the data of this node.
      * @param create The operation to be performed.
      */
      stRLeafNode(stPage * page, bool create = false);

      /**
      * Returns the reference of the desired leaf entry. You may use this method to
      * read and modify the leaf entry information.
      *
      * @param idx The idx of the leaf entry.
      * @warning The parameter idx is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @return stLeafEntry the reference of the desired entry.
      * @see stLeafEntry
      * @see GetNumberOfEntries()
      * @see GetObject()
      * @see GetObjectSize()
      */
      stRLeafEntry & GetLeafEntry(stCount idx){
         #ifdef __stDEBUG__
         if (idx >= GetNumberOfEntries()){
            throw invalid_argument("idx value is out of range.");
         }//end if
         #endif //__stDEBUG__

         return Entries[idx];
      }//end GetLeafEntry

      /**
      * Adds a new entry to this node. This method will return the idx of the new
      * node or a negative value for failure.
      *
      * <P>This method will fail if there is not enough space to hold the
      * new object.
      *
      * <P>If you have added a new entry successfully, you may edit the entry
      * fields using the method GetEntry().
      *
      * @param size The size of the object in bytes.
      * @param object The object data.
      * @warning The parameter size is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @return The position in the vector Entries.
      * @see RemoveEntry()
      * @see GetEntry()
      */
      virtual int AddEntry(stSize size, const stByte * object);

      /**
      * Gets the serialized object. Use GetObjectSize() to determine the size of
      * the object.
      *
      * @param idx The idx of the entry.
      * @warning The parameter idx is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @return A pointer to the serialized object.
      * @see GetObjectSize()
      */
      const stByte * GetObject(stCount idx);

      /**
      * Returns the size of the object. Use GetObject() to get the object data.
      *
      * @param idx The idx of the entry.
      * @warning The parameter idx is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @return The size of the serialized object.
      * @see GetObject()
      */
      stSize GetObjectSize(stCount idx);

      /**
      * Removes an entry from this object.
      *
      * @param idx The idx of the entry.
      * @warning The parameter idx is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @return A pointer to the serialized object.
      * @see GetObjectSize()
      * @todo This method is not implemented yet.
      */
      void RemoveEntry(stCount idx);

      /**
      * Returns the total number of objects in the subtree.
      */
      virtual stCount GetTotalObjectCount(){
         return (stCount)GetNumberOfEntries();
      }//end GetTotalObjectCount()

      /**
      * Returns the overhead of each leaf node entry in bytes.
      */
      static stSize GetLeafEntryOverhead(){
         return sizeof(stRLeafEntry);
      }//end GetLeafEntryOverhead()

      /**
      * Returns the amount of the free space in this node.
      */
      stSize GetFree();

   private:
      /**
      * Entry pointer
      */
      stRLeafEntry * Entries;

};//end stRLeafNode

// Include implementation
#include "stRNode.cc"

#endif //__STRNODE_H
