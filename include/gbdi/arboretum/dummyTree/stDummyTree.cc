/**********************************************************************
 * GBDI Arboretum - Copyright (c) 2002-2004 GBDI-ICMC-USP
 *
 *                           Homepage: http://gbdi.icmc.usp.br/arboretum
 **********************************************************************/

#include <gbdi/arboretum/stMetricAccessMethod.h>

#include "stDummyTree.h"

/* ====================================================================
 * The GBDI-ICMC-USP Software License Version 1.0
 *
 * Copyright (c) 2004 Grupo de Bases de Dados e Imagens, Instituto de
 * Ci�ncias Matem�ticas e de Computa��o, University of S�o Paulo -
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
 *        de Dados e Imagens, Instituto de Ci�ncias Matem�ticas e de
 *        Computa��o, University of S�o Paulo - Brazil (the Databases 
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
//Implementation of stDummyTree.h

//------------------------------------------------------------------------------

template <class ObjectType, class EvaluatorType>
stDummyTree<ObjectType, EvaluatorType>::stDummyTree(stPageManager * pageman) :
stMetricTree<ObjectType, EvaluatorType>(pageman) {

  // Will I create or read it
  if (this->myPageManager->IsEmpty()) {
    // Create it
    this->Create();
    // Default values
    Header->ObjectCount = 0;
    Header->NodeCount = 0;
    Header->MaxOccupation = 0;
  }
  else {
    // Use it
    this->LoadHeader();
  }//end if
}//end stDummyTree<ObjectType><EvaluatorType>::stDummyTree

//------------------------------------------------------------------------------

template <class ObjectType, class EvaluatorType>
stDummyTree<ObjectType, EvaluatorType>::stDummyTree(stPageManager * pageman, EvaluatorType* metricEvaluator) :
stMetricTree<ObjectType, EvaluatorType>(pageman, metricEvaluator) {

  // Will I create or read it
  if (this->myPageManager->IsEmpty()) {
    // Create it
    this->Create();
    // Default values
    Header->ObjectCount = 0;
    Header->NodeCount = 0;
    Header->MaxOccupation = 0;
  }
  else {
    // Use it
    this->LoadHeader();
  }//end if
}

//------------------------------------------------------------------------------

template <class ObjectType, class EvaluatorType>
bool stDummyTree<ObjectType, EvaluatorType>::Add(tObject * obj) {
  // The object will be added in the first page.
  // When it is full, it will create a new page and link
  // it in the begining of the list.
  stPage * currPage;
  stDummyNode * currNode;
  bool overflow;
  stPageID nextPageID;
  int id;

  // Does it fit ?
  if (obj->GetSerializedSize() > this->myPageManager->GetMinimumPageSize() - 16) {
    return false;
  }//end if

  // Adding object
  if (this->GetRoot() == 0) {
    overflow = true;
    nextPageID = 0;
  }
  else {
    // Get node
    currPage = this->myPageManager->GetPage(this->GetRoot());
    currNode = new stDummyNode(currPage);

    // Try to add
    id = currNode->AddEntry(obj->GetSerializedSize(), obj->Serialize());
    if (id >= 0) {
      // I was able to add.
      this->myPageManager->WritePage(currPage);
      overflow = false;

      // update the maximum number of entries.
      this->SetMaxOccupation(currNode->GetNumberOfEntries());
    }
    else {
      // Oops! Overflow!
      overflow = true;
      nextPageID = currPage->GetPageID();
    }//end if

    // Clear the mess
    delete currNode;
    this->myPageManager->ReleasePage(currPage);
  }//end if

  // Manage overflows
  if (overflow) {
    // Oops! New root required !
    currPage = this->myPageManager->GetNewPage();
    currNode = new stDummyNode(currPage, true);
    currNode->SetNextNode(nextPageID);
    // Update the number of nodes.
    Header->NodeCount++;

    // I'll add it here
    id = currNode->AddEntry(obj->GetSerializedSize(), obj->Serialize());

    // Write the new node
    this->myPageManager->WritePage(currPage);

    // Update "tree" state
    this->SetRoot(currPage->GetPageID());
    WriteHeader();

    // Clear the mess again
    delete currNode;
    this->myPageManager->ReleasePage(currPage);
  }//end overflow
  //Update the number of objects.
  UpdateObjectCounter(1);
  // Write Header!
  WriteHeader();

  return true;
}//end stDummyTree<ObjectType><EvaluatorType>::Add

//------------------------------------------------------------------------------

template <class ObjectType, class EvaluatorType>
stResult<ObjectType> * stDummyTree<ObjectType, EvaluatorType>::RangeQuery(
                                                                          tObject * sample, stDistance range) {
  stPage * currPage;
  stDummyNode * currNode;
  tResult * result;
  tObject tmp;
  stDistance distance;
  stCount i;
  stPageID nextPageID;

  // Create result
  result = new tResult();
  result->SetQueryInfo(sample->Clone(), RANGEQUERY, -1, range, false);

  // First node
  nextPageID = this->GetRoot();

  // Let's search
  while (nextPageID != 0) {
    // Get node
    currPage = this->myPageManager->GetPage(nextPageID);
    currNode = new stDummyNode(currPage);

    // Lets check all objects in this node
    for (i = 0; i < currNode->GetNumberOfEntries(); i++) {
      // Rebuild the object
      tmp.Unserialize(currNode->GetObject(i), currNode->GetObjectSize(i));

      // Evaluate distance
      distance = this->myMetricEvaluator->GetDistance(&tmp, sample);

      // Is it qualified ?
      if (distance <= range) {
        // Yes! I'm qualified !
        result->AddPair(tmp.Clone(), distance);
      }//end if
    }//end for

    // Next PageID...
    nextPageID = currNode->GetNextNode();

    // Free it all
    delete currNode;
    this->myPageManager->ReleasePage(currPage);
  }//end while

  // Return the result.
  return result;
}//end stDummyTree<ObjectType><EvaluatorType>::RangeQuery

//------------------------------------------------------------------------------

template <class ObjectType, class EvaluatorType>
stResult<ObjectType> * stDummyTree<ObjectType, EvaluatorType>::ReversedRangeQuery(
                                                                                  tObject * sample, stDistance range) {
  stPage * currPage;
  stDummyNode * currNode;
  tResult * result;
  tObject tmp;
  stDistance distance;
  stCount i;
  stPageID nextPageID;

  // Create result
  result = new tResult();
  result->SetQueryInfo(sample->Clone(), REVERSEDRANGEQUERY, -1, range, false);

  // First node
  nextPageID = this->GetRoot();

  // Let's search
  while (nextPageID != 0) {
    // Get node
    currPage = this->myPageManager->GetPage(nextPageID);
    currNode = new stDummyNode(currPage);

    // Lets check all objects in this node
    for (i = 0; i < currNode->GetNumberOfEntries(); i++) {
      // Rebuild the object
      tmp.Unserialize(currNode->GetObject(i), currNode->GetObjectSize(i));

      // Evaluate distance
      distance = this->myMetricEvaluator->GetDistance(&tmp, sample);

      // Is it qualified ?
      if (distance >= range) {
        // Yes! I'm qualified !
        result->AddPair(tmp.Clone(), distance);
      }//end if
    }//end for

    // Next PageID...
    nextPageID = currNode->GetNextNode();

    // Free it all
    delete currNode;
    this->myPageManager->ReleasePage(currPage);
  }//end while

  // Return the result.
  return result;
}//end stDummyTree<ObjectType><EvaluatorType>::ReversedRangeQuery

//------------------------------------------------------------------------------

template <class ObjectType, class EvaluatorType>
stResult<ObjectType> * stDummyTree<ObjectType, EvaluatorType>::NearestQuery(
                                                                            tObject * sample, stCount k, bool tie) {
  stPage * currPage;
  stDummyNode * currNode;
  tResult * result;
  tObject tmp;
  stDistance distance;
  stCount i;
  stPageID nextPageID;

  // Create result
  result = new tResult(k);
  result->SetQueryInfo(sample->Clone(), KNEARESTQUERY, k, -1.0, tie);

  // First node
  nextPageID = this->GetRoot();

  // Let's search
  while (nextPageID != 0) {
    // Get node
    currPage = this->myPageManager->GetPage(nextPageID);
    currNode = new stDummyNode(currPage);

    // Lets check all objects in this node
    for (i = 0; i < currNode->GetNumberOfEntries(); i++) {
      // Rebuild the object
      tmp.Unserialize(currNode->GetObject(i), currNode->GetObjectSize(i));

      // Evaluate distance
      distance = this->myMetricEvaluator->GetDistance(&tmp, sample);

      // Is it qualified ?
      if (result->GetNumOfEntries() < k) {
        // Unnecessary to check. Just add.
        result->AddPair(tmp.Clone(), distance);
      }
      else {
        // Will I add ?
        if (distance <= result->GetMaximumDistance()) {
          // Yes! I'll.
          result->AddPair(tmp.Clone(), distance);
          result->Cut(k);
        }//end if
      }//end if
    }//end for

    // Next Node...
    nextPageID = currNode->GetNextNode();

    // Free it all
    delete currNode;
    this->myPageManager->ReleasePage(currPage);
  }//end while

  // Return the result.
  return result;
}//end NearestQuery

//------------------------------------------------------------------------------

template <class ObjectType, class EvaluatorType>
stResult<ObjectType> * stDummyTree<ObjectType, EvaluatorType>::FarthestQuery(
                                                                             tObject * sample, stCount k, bool tie) {
  stPage * currPage;
  stDummyNode * currNode;
  tResult * result;
  tObject tmp;
  stDistance distance;
  stCount i;
  stPageID nextPageID;

  // Create result
  result = new tResult(k);
  result->SetQueryInfo(sample->Clone(), KFARTHESTQUERY, k, -1.0, tie);

  // First node
  nextPageID = this->GetRoot();

  // Let's search
  while (nextPageID != 0) {
    // Get node
    currPage = this->myPageManager->GetPage(nextPageID);
    currNode = new stDummyNode(currPage);

    // Lets check all objects in this node
    for (i = 0; i < currNode->GetNumberOfEntries(); i++) {
      // Rebuild the object
      tmp.Unserialize(currNode->GetObject(i), currNode->GetObjectSize(i));

      // Evaluate distance
      distance = this->myMetricEvaluator->GetDistance(&tmp, sample);

      // Is it qualified ?
      if (result->GetNumOfEntries() < k) {
        // Unnecessary to check. Just add.
        result->AddPair(tmp.Clone(), distance);
      }
      else {
        // Will I add ?
        if (distance >= result->GetMinimumDistance()) {
          // Yes! I'll.
          result->AddPair(tmp.Clone(), distance);
          result->CutFirst(k);
        }//end if
      }//end if
    }//end for

    // Next Node...
    nextPageID = currNode->GetNextNode();

    // Free it all
    delete currNode;
    this->myPageManager->ReleasePage(currPage);
  }//end while

  // Return the result.
  return result;
}//end FarthestQuery

//------------------------------------------------------------------------------

template <class ObjectType, class EvaluatorType>
stResult<ObjectType> * stDummyTree<ObjectType, EvaluatorType>::KAndRangeQuery(
                                                                              tObject * sample, stDistance range, stCount k, bool tie) {
  stPage * currPage;
  stDummyNode * currNode;
  tResult * result;
  tObject tmp;
  stDistance distance;
  stCount i;
  stPageID nextPageID;

  // Create result
  result = new tResult(k);
  result->SetQueryInfo(sample->Clone(), KANDRANGEQUERY, k, range, tie);

  // First node
  nextPageID = this->GetRoot();

  // Let's search
  while (nextPageID != 0) {
    // Get node
    currPage = this->myPageManager->GetPage(nextPageID);
    currNode = new stDummyNode(currPage);

    // Lets check all objects in this node
    for (i = 0; i < currNode->GetNumberOfEntries(); i++) {
      // Rebuild the object
      tmp.Unserialize(currNode->GetObject(i), currNode->GetObjectSize(i));

      // Evaluate distance
      distance = this->myMetricEvaluator->GetDistance(&tmp, sample);

      // Is it qualified ?
      if (distance <= range) {
        // Yes! I'm qualified !
        if (result->GetNumOfEntries() < k) {
          // Has less than k.
          result->AddPair(tmp.Clone(), distance);
        }
        else {
          // May I add ?
          if (distance <= result->GetMaximumDistance()) {
            // Yes! I'll add it and cut the results if necessary
            result->AddPair(tmp.Clone(), distance);
            result->Cut(k);
          }//end if
        }//end if
      }//end if
    }//end for

    // Next Node...
    nextPageID = currNode->GetNextNode();

    // Free it all
    delete currNode;
    this->myPageManager->ReleasePage(currPage);
  }//end while

  // Return the result.
  return result;
}//end KAndRangeQuery

//------------------------------------------------------------------------------

template <class ObjectType, class EvaluatorType>
stResult<ObjectType> * stDummyTree<ObjectType, EvaluatorType>::KOrRangeQuery(
                                                                             tObject * sample, stDistance range, stCount k, bool tie) {
  stPage * currPage;
  stDummyNode * currNode;
  tResult * result;
  tObject tmp;
  stDistance distance, dk = MAXDOUBLE;
  stCount i;
  stPageID nextPageID;

  // Create result
  result = new tResult();
  result->SetQueryInfo(sample->Clone(), KORRANGEQUERY, k, range, tie);

  // First node
  nextPageID = this->GetRoot();

  // Let's search
  while (nextPageID != 0) {
    // Get node
    currPage = this->myPageManager->GetPage(nextPageID);
    currNode = new stDummyNode(currPage);

    // Lets check all objects in this node
    for (i = 0; i < currNode->GetNumberOfEntries(); i++) {
      // Rebuild the object
      tmp.Unserialize(currNode->GetObject(i), currNode->GetObjectSize(i));

      // Evaluate distance
      distance = this->myMetricEvaluator->GetDistance(&tmp, sample);

      // KorRange part
      if (distance <= dk) {
        // Add in the result
        result->AddPair(tmp.Clone(), distance);
        // Test if Nearest > Range.
        if (dk > range) {
          // Cut the result if it is possible.
          if (result->GetNumOfEntries() >= k) {
            // This depends on tie list too
            result->Cut(k);
            // Range > Nearest
            if (result->GetMaximumDistance() <= range) {
              // Query radius is range.
              dk = range;
            }
            else {
              // Query radius is the farthest object (last nearest).
              dk = result->GetMaximumDistance();
            }//end if
          }//end if
        }//end if
      }//end if
    }//end for

    // Next Node...
    nextPageID = currNode->GetNextNode();

    // Free it all
    delete currNode;
    this->myPageManager->ReleasePage(currPage);
  }//end while

  // Return the result.
  return result;
}//end KOrRangeQuery

//------------------------------------------------------------------------------

template <class ObjectType, class EvaluatorType>
stResult<ObjectType> * stDummyTree<ObjectType, EvaluatorType>::RingQuery(
                                                                         tObject * sample, stDistance inRange, stDistance outRange) {
  stPage * currPage;
  stDummyNode * currNode;
  tResult * result;
  tObject tmp;
  stDistance distance;
  stCount i;
  stPageID nextPageID;

  // Create result
  result = new tResult();
  result->SetQueryInfo(sample->Clone(), RINGQUERY, -1, outRange, inRange);

  // First node
  nextPageID = this->GetRoot();

  // Let's search
  while (nextPageID != 0) {
    // Get node
    currPage = this->myPageManager->GetPage(nextPageID);
    currNode = new stDummyNode(currPage);

    // Lets check all objects in this node
    for (i = 0; i < currNode->GetNumberOfEntries(); i++) {
      // Rebuild the object
      tmp.Unserialize(currNode->GetObject(i), currNode->GetObjectSize(i));

      // Evaluate distance
      distance = this->myMetricEvaluator->GetDistance(&tmp, sample);

      // Is it qualified ?
      if ((distance <= outRange) && (distance > inRange)) {
        // Yes! I'm qualified !
        result->AddPair(tmp.Clone(), distance);
      }//end if
    }//end for

    // Next Node...
    nextPageID = currNode->GetNextNode();

    // Free it all
    delete currNode;
    this->myPageManager->ReleasePage(currPage);
  }//end while

  // Return the result.
  return result;
}//end RingQuery

//------------------------------------------------------------------------------

template <class ObjectType, class EvaluatorType>
stResult< ObjectType> * stDummyTree<ObjectType, EvaluatorType>::SumNearestQuery(
                                                                                tObject ** sampleList, stSize sampleSize, stCount k, bool tie) {

  stPage * currPage;
  stDummyNode * currNode;
  tObject tmp;
  stDistance distance;
  stCount i;
  stSize idx;
  stPageID nextPageID;

  // Create result
  tResult * result = new tResult();
  result->SetQueryInfo(sampleList[0]->Clone(), SUMKNEARESTQUERY, k, -1.0, tie);

  // First node
  nextPageID = this->GetRoot();

  // Let's search
  while (nextPageID != 0) {
    // Get node
    currPage = this->myPageManager->GetPage(nextPageID);
    currNode = new stDummyNode(currPage);

    // Lets check all objects in this node
    for (i = 0; i < currNode->GetNumberOfEntries(); i++) {
      // Rebuild the object
      tmp.Unserialize(currNode->GetObject(i), currNode->GetObjectSize(i));
      // Reset the distance.
      distance = 0.0;
      // For each sample object
      for (idx = 0; idx < sampleSize; idx++) {
        // Evaluate distance
        distance += this->myMetricEvaluator->GetDistance(&tmp, sampleList[idx]);
      }//end for

      // Is it qualified?
      if (result->GetNumOfEntries() < k) {
        // Unnecessary to check. Just add.
        result->AddPair(tmp.Clone(), distance);
      }
      else {
        // Will I add ?
        if (distance <= result->GetMaximumDistance()) {
          // Yes! I'll.
          result->AddPair(tmp.Clone(), distance);
          result->Cut(k);
        }//end if
      }//end if
    }//end for
    // Next Node...
    nextPageID = currNode->GetNextNode();

    // Free it all
    delete currNode;
    this->myPageManager->ReleasePage(currPage);
  }//end while

  // Return the result.
  return result;
}//end SumNearestQuery

//------------------------------------------------------------------------------

template <class ObjectType, class EvaluatorType>
stResult<ObjectType> * stDummyTree<ObjectType, EvaluatorType>::SumRangeQuery(
                                                                             tObject ** sampleList, stSize sampleSize, stDistance range) {

  stPage * currPage;
  stDummyNode * currNode;
  tObject tmp;
  stDistance distance;
  stCount i;
  stSize idx;
  stPageID nextPageID;

  // Create result
  tResult * result = new tResult();
  result->SetQueryInfo(sampleList[0]->Clone(), SUMRANGEQUERY, -1, range, false);

  // First node
  nextPageID = this->GetRoot();

  // Let's search
  while (nextPageID != 0) {
    // Get node
    currPage = this->myPageManager->GetPage(nextPageID);
    currNode = new stDummyNode(currPage);

    // Lets check all objects in this node
    for (i = 0; i < currNode->GetNumberOfEntries(); i++) {
      // Rebuild the object
      tmp.Unserialize(currNode->GetObject(i), currNode->GetObjectSize(i));
      // Reset the distance.
      distance = 0.0;
      // For each sample object
      for (idx = 0; idx < sampleSize; idx++) {
        // Evaluate distance
        distance += this->myMetricEvaluator->GetDistance(&tmp, sampleList[idx]);
      }//end for

      // Is it qualified?
      if (distance <= range) {
        // Yes! I'm qualified !
        result->AddPair(tmp.Clone(), distance);
      }//end if
    }//end for

    // Next Node...
    nextPageID = currNode->GetNextNode();

    // Free it all
    delete currNode;
    this->myPageManager->ReleasePage(currPage);
  }//end while

  // Return the result.
  return result;
}//end SumRangeQuery


//------------------------------------------------------------------------------

template <class ObjectType, class EvaluatorType>
stResult< ObjectType> * stDummyTree<ObjectType, EvaluatorType>::MaxNearestQuery(
                                                                                tObject ** sampleList, stSize sampleSize, stCount k, bool tie) {

  stPage * currPage;
  stDummyNode * currNode;
  tObject tmp;
  stDistance distance, tmpdistance;
  stCount i;
  stSize idx;
  stPageID nextPageID;

  // Create result
  tResult * result = new tResult();
  result->SetQueryInfo(sampleList[0]->Clone(), SUMKNEARESTQUERY, k, -1.0, tie);

  // First node
  nextPageID = this->GetRoot();

  // Let's search
  while (nextPageID != 0) {
    // Get node
    currPage = this->myPageManager->GetPage(nextPageID);
    currNode = new stDummyNode(currPage);

    // Lets check all objects in this node
    for (i = 0; i < currNode->GetNumberOfEntries(); i++) {
      // Rebuild the object
      tmp.Unserialize(currNode->GetObject(i), currNode->GetObjectSize(i));
      // Reset the distance
      distance = 0.0;
      // For each sample object
      for (idx = 0; idx < sampleSize; idx++) {
        // Evaluate distance
        tmpdistance = this->myMetricEvaluator->GetDistance(&tmp, sampleList[idx]);
        if (tmpdistance > distance) {
          distance = tmpdistance;
        }
      }//end for

      // Is it qualified?
      if (result->GetNumOfEntries() < k) {
        // Unnecessary to check. Just add.
        result->AddPair(tmp.Clone(), distance);
      }
      else {
        // Will I add ?
        if (distance <= result->GetMaximumDistance()) {
          // Yes! I'll.
          result->AddPair(tmp.Clone(), distance);
          result->Cut(k);
        }//end if
      }//end if
    }//end for
    // Next Node...
    nextPageID = currNode->GetNextNode();

    // Free it all
    delete currNode;
    this->myPageManager->ReleasePage(currPage);
  }//end while

  // Return the result.
  return result;
}//end MaxNearestQuery

//------------------------------------------------------------------------------

template <class ObjectType, class EvaluatorType>
stResult<ObjectType> * stDummyTree<ObjectType, EvaluatorType>::MaxRangeQuery(
                                                                             tObject ** sampleList, stSize sampleSize, stDistance range) {

  stPage * currPage;
  stDummyNode * currNode;
  tObject tmp;
  stDistance distance, tmpdistance;
  stCount i;
  stSize idx;
  stPageID nextPageID;

  // Create result
  tResult * result = new tResult();
  result->SetQueryInfo(sampleList[0]->Clone(), SUMRANGEQUERY, -1, range, false);

  // First node
  nextPageID = this->GetRoot();

  // Let's search
  while (nextPageID != 0) {
    // Get node
    currPage = this->myPageManager->GetPage(nextPageID);
    currNode = new stDummyNode(currPage);

    // Lets check all objects in this node
    for (i = 0; i < currNode->GetNumberOfEntries(); i++) {
      // Rebuild the object
      tmp.Unserialize(currNode->GetObject(i), currNode->GetObjectSize(i));
      // Reset the distance.
      distance = 0.0;
      // For each sample object
      for (idx = 0; idx < sampleSize; idx++) {
        // Evaluate distance
        tmpdistance = this->myMetricEvaluator->GetDistance(&tmp, sampleList[idx]);
        if (tmpdistance > distance) {
          distance = tmpdistance;
        }
      }//end for

      // Is it qualified?
      if (distance <= range) {
        // Yes! I'm qualified !
        result->AddPair(tmp.Clone(), distance);
      }//end if
    }//end for

    // Next Node...
    nextPageID = currNode->GetNextNode();

    // Free it all
    delete currNode;
    this->myPageManager->ReleasePage(currPage);
  }//end while

  // Return the result.
  return result;
}//end MaxRangeQuery

//------------------------------------------------------------------------------

template <class ObjectType, class EvaluatorType>
stResult< ObjectType> * stDummyTree<ObjectType, EvaluatorType>::AllNearestQuery(
                                                                                tObject ** sampleList, stSize sampleSize, stCount k, bool tie) {

  stPage * currPage;
  stDummyNode * currNode;
  tObject tmp;
  stDistance distance, tmpdistance;
  stCount i;
  stSize idx;
  stPageID nextPageID;

  // Create result
  tResult * result = new tResult();
  result->SetQueryInfo(sampleList[0]->Clone(), SUMKNEARESTQUERY, k, -1.0, tie);

  // First node
  nextPageID = this->GetRoot();

  // Let's search
  while (nextPageID != 0) {
    // Get node
    currPage = this->myPageManager->GetPage(nextPageID);
    currNode = new stDummyNode(currPage);

    // Lets check all objects in this node
    for (i = 0; i < currNode->GetNumberOfEntries(); i++) {
      // Rebuild the object
      tmp.Unserialize(currNode->GetObject(i), currNode->GetObjectSize(i));
      // Reset the distance.
      distance = 0.0;
      // For each sample object
      for (idx = 0; idx < sampleSize; idx++) {
        // Evaluate distance
        tmpdistance = this->myMetricEvaluator->GetDistance(&tmp, sampleList[idx]);
        distance += tmpdistance * tmpdistance;
      }//end for
      distance = sqrt(distance);

      // Is it qualified?
      if (result->GetNumOfEntries() < k) {
        // Unnecessary to check. Just add.
        result->AddPair(tmp.Clone(), distance);
      }
      else {
        // Will I add ?
        if (distance <= result->GetMaximumDistance()) {
          // Yes! I'll.
          result->AddPair(tmp.Clone(), distance);
          result->Cut(k);
        }//end if
      }//end if
    }//end for
    // Next Node...
    nextPageID = currNode->GetNextNode();

    // Free it all
    delete currNode;
    this->myPageManager->ReleasePage(currPage);
  }//end while

  // Return the result.
  return result;
}//end AllNearestQuery

//------------------------------------------------------------------------------

template <class ObjectType, class EvaluatorType>
stResult<ObjectType> * stDummyTree<ObjectType, EvaluatorType>::AllRangeQuery(
                                                                             tObject ** sampleList, stSize sampleSize, stDistance range) {

  stPage * currPage;
  stDummyNode * currNode;
  tObject tmp;
  stDistance distance, tmpdistance;
  stCount i;
  stSize idx;
  stPageID nextPageID;

  // Create result
  tResult * result = new tResult();
  result->SetQueryInfo(sampleList[0]->Clone(), SUMRANGEQUERY, -1, range, false);

  // First node
  nextPageID = this->GetRoot();

  // Let's search
  while (nextPageID != 0) {
    // Get node
    currPage = this->myPageManager->GetPage(nextPageID);
    currNode = new stDummyNode(currPage);

    // Lets check all objects in this node
    for (i = 0; i < currNode->GetNumberOfEntries(); i++) {
      // Rebuild the object
      tmp.Unserialize(currNode->GetObject(i), currNode->GetObjectSize(i));
      // Reset the distance.
      distance = 0.0;
      // For each sample object
      for (idx = 0; idx < sampleSize; idx++) {
        // Evaluate distance
        tmpdistance = this->myMetricEvaluator->GetDistance(&tmp, sampleList[idx]);
        distance += tmpdistance * tmpdistance;
      }//end for
      distance = sqrt(distance);

      // Is it qualified?
      if (distance <= range) {
        // Yes! I'm qualified !
        result->AddPair(tmp.Clone(), distance);
      }//end if
    }//end for

    // Next Node...
    nextPageID = currNode->GetNextNode();

    // Free it all
    delete currNode;
    this->myPageManager->ReleasePage(currPage);
  }//end while

  // Return the result.
  return result;
}//end AllRangeQuery

//------------------------------------------------------------------------------

template <class ObjectType, class EvaluatorType>
stJoinedResult<ObjectType> * stDummyTree<ObjectType, EvaluatorType>::NearestJoinQuery(
                                                                                      stDummyTree * dummyTree, stCount k, bool tie) {

  // Yes, there is.
  stPage * currPage;
  stPage * joinedPage;
  stDummyNode * currNode;
  stDummyNode * joinedNode;
  stPageID nextPageID;
  stPageID nextJoinedPageID;
  tJoinedResult ** tmpResult;
  tObject tmp;
  tObject tmpJoin;
  stDistance distance;
  stCount i, j, idx;
  stCount numberOfEntries, joinedNumberOfEntries;

  // Create result
  tJoinedResult * result = new tJoinedResult();
  result->SetQueryInfo(KNEARESTJOINQUERY, k, -1.0, tie);

  // Get the first node
  nextPageID = this->GetRoot();

  // Let's search
  while (nextPageID != 0) {
    // Get node
    currPage = this->myPageManager->GetPage(nextPageID);
    currNode = new stDummyNode(currPage);
    // Get the number of entries.
    numberOfEntries = currNode->GetNumberOfEntries();

    // Allocate a temporary result.
    tmpResult = new tJoinedResult * [numberOfEntries];

    for (i = 0; i < numberOfEntries; i++) {
      tmpResult[i] = new tJoinedResult();
      tmpResult[i]->SetQueryInfo(KNEARESTJOINQUERY, k, -1.0, tie);
    }//end for

    // Get the root of the joined tree.
    nextJoinedPageID = dummyTree->GetRoot();
    // Let's search the joined tree
    while (nextJoinedPageID != 0) {
      // Get node
      joinedPage = dummyTree->myPageManager->GetPage(nextJoinedPageID);
      joinedNode = new stDummyNode(joinedPage);
      // Get the number of entries.
      joinedNumberOfEntries = joinedNode->GetNumberOfEntries();

      // Lets check all objects in this node
      for (i = 0; i < numberOfEntries; i++) {
        // Rebuild the object.
        tmp.Unserialize(currNode->GetObject(i),
                        currNode->GetObjectSize(i));
        // For each entry in the joined node.
        for (j = 0; j < joinedNumberOfEntries; j++) {
          // Rebuild the object.
          tmpJoin.Unserialize(joinedNode->GetObject(j),
                              joinedNode->GetObjectSize(j));
          // Evaluate distance.
          distance = this->myMetricEvaluator->GetDistance(&tmp, &tmpJoin);
          // Is it a qualified object?
          if (tmpResult[i]->GetNumOfEntries() < k) {
            // Unnecessary to check. Just add.
            tmpResult[i]->AddJoinedTriple(tmp.Clone(), tmpJoin.Clone(), distance);
          }
          else {
            // Will I add ?
            if (distance <= tmpResult[i]->GetMaximumDistance()) {
              // Yes! I'll.
              tmpResult[i]->AddJoinedTriple(tmp.Clone(), tmpJoin.Clone(), distance);
              tmpResult[i]->Cut(k);
            }//end if
          }//end if
        }//end for
      }//end for

      // Next joined node...
      nextJoinedPageID = joinedNode->GetNextNode();

      // Free it all
      delete joinedNode;
      dummyTree->myPageManager->ReleasePage(joinedPage);
    }//end while

    // Copy the tmpResult objects to the final result.
    for (i = 0; i < numberOfEntries; i++) {
      for (idx = 0; idx < tmpResult[i]->GetNumOfEntries(); idx++) {
        result->AddJoinedTriple((* tmpResult[i])[idx].GetObject()->Clone(),
                                (* tmpResult[i])[idx].GetJoinedObject()->Clone(),
                                (* tmpResult[i])[idx].GetDistance());
      }//end for
      // Cleanning
      delete tmpResult[i];
    }//end for
    // Free it all.
    delete[] tmpResult;

    // Next Node...
    nextPageID = currNode->GetNextNode();

    // Free it all
    delete currNode;
    this->myPageManager->ReleasePage(currPage);
  }//end while

  // Return the result.
  return result;
}//end NearestJoinQuery

//------------------------------------------------------------------------------

template <class ObjectType, class EvaluatorType>
stJoinedResult<ObjectType> * stDummyTree<ObjectType, EvaluatorType>::RangeJoinQuery(
                                                                                    stDummyTree * dummyTree, stDistance range) {

  stPage * currPage;
  stPage * joinedPage;
  stDummyNode * currNode;
  stDummyNode * joinedNode;
  stPageID nextPageID;
  stPageID nextJoinedPageID;
  tObject tmp;
  tObject tmpJoin;
  stDistance distance;
  stCount i, j, idx;
  stCount numberOfEntries, joinedNumberOfEntries;

  // Create result
  tJoinedResult * result = new tJoinedResult();
  result->SetQueryInfo(RANGEJOINQUERY, -1, range, false);

  // Get the first node
  nextPageID = this->GetRoot();

  // Let's search
  while (nextPageID != 0) {
    // Get node
    currPage = this->myPageManager->GetPage(nextPageID);
    currNode = new stDummyNode(currPage);
    // Get the number of entries.
    numberOfEntries = currNode->GetNumberOfEntries();

    // Get the root of the joined tree.
    nextJoinedPageID = dummyTree->GetRoot();
    // Let's search the joined tree
    while (nextJoinedPageID != 0) {
      // Get node
      joinedPage = dummyTree->myPageManager->GetPage(nextJoinedPageID);
      joinedNode = new stDummyNode(joinedPage);
      // Get the number of entries.
      joinedNumberOfEntries = joinedNode->GetNumberOfEntries();

      // Lets check all objects in this node
      for (i = 0; i < numberOfEntries; i++) {
        // Rebuild the object
        tmp.Unserialize(currNode->GetObject(i), currNode->GetObjectSize(i));

        for (j = 0; j < joinedNumberOfEntries; j++) {
          // Rebuild the object.
          tmpJoin.Unserialize(joinedNode->GetObject(j), joinedNode->GetObjectSize(j));

          // Evaluate distance
          distance = this->myMetricEvaluator->GetDistance(&tmp, &tmpJoin);

          // Is it qualified ?
          if (distance <= range) {
            // Yes! I'm qualified !
            result->AddJoinedTriple(tmp.Clone(), tmpJoin.Clone(), distance);
          }//end if
        }//end for
      }//end for

      // Next Node...
      nextJoinedPageID = joinedNode->GetNextNode();

      // Free it all
      delete joinedNode;
      dummyTree->myPageManager->ReleasePage(joinedPage);
    }//end while

    // Next Node...
    nextPageID = currNode->GetNextNode();

    // Free it all
    delete currNode;
    this->myPageManager->ReleasePage(currPage);
  }//end while

  // Return the result.
  return result;
}//end RangeJoinQuery

//------------------------------------------------------------------------------

template <class ObjectType, class EvaluatorType>
stJoinedResult<ObjectType> * stDummyTree<ObjectType, EvaluatorType>::ClosestJoinQuery(
                                                                                      stDummyTree * dummyTree, stCount k, bool tie) {

  // Yes, there is.
  stPage * currPage;
  stPage * joinedPage;
  stDummyNode * currNode;
  stDummyNode * joinedNode;
  stPageID nextPageID;
  stPageID nextJoinedPageID;
  tObject tmp;
  tObject tmpJoin;
  stDistance distance;
  stCount i, j, idx;
  stCount numberOfEntries, joinedNumberOfEntries;

  // Create result
  tJoinedResult * result = new tJoinedResult();
  result->SetQueryInfo(KCLOSESTJOINQUERY, k, -1.0, tie);

  // Get the first node
  nextPageID = this->GetRoot();

  // Let's search
  while (nextPageID != 0) {
    // Get node
    currPage = this->myPageManager->GetPage(nextPageID);
    currNode = new stDummyNode(currPage);
    // Get the number of entries.
    numberOfEntries = currNode->GetNumberOfEntries();

    // Get the root of the joined tree.
    nextJoinedPageID = dummyTree->GetRoot();
    // Let's search the joined tree
    while (nextJoinedPageID != 0) {
      // Get node
      joinedPage = dummyTree->myPageManager->GetPage(nextJoinedPageID);
      joinedNode = new stDummyNode(joinedPage);
      // Get the number of entries.
      joinedNumberOfEntries = joinedNode->GetNumberOfEntries();

      // Lets check all objects in this node
      for (i = 0; i < numberOfEntries; i++) {
        // Rebuild the object.
        tmp.Unserialize(currNode->GetObject(i),
                        currNode->GetObjectSize(i));
        // For each entry in the joined node.
        for (j = 0; j < joinedNumberOfEntries; j++) {
          // Rebuild the object.
          tmpJoin.Unserialize(joinedNode->GetObject(j),
                              joinedNode->GetObjectSize(j));
          // Evaluate distance.
          distance = this->myMetricEvaluator->GetDistance(&tmp, &tmpJoin);
          //if insert localresult
          if (result->GetNumOfEntries() < k) {
            // Unnecessary to check. Just add.
            result->AddJoinedTriple(tmp.Clone(), tmpJoin.Clone(), distance);
          }
          else {
            // Will I add ?
            if (distance <= result->GetMaximumDistance()) {
              result->AddJoinedTriple(tmp.Clone(), tmpJoin.Clone(), distance);
              //cut if there is more than k elements
              result->Cut(k);
            }//end if
          }//end if
        }//end for
      }//end for

      // Next joined node...
      nextJoinedPageID = joinedNode->GetNextNode();

      // Free it all
      delete joinedNode;
      dummyTree->myPageManager->ReleasePage(joinedPage);
    }//end while

    // Next Node...
    nextPageID = currNode->GetNextNode();

    // Free it all
    delete currNode;
    this->myPageManager->ReleasePage(currPage);
  }//end while

  // Return the result.
  return result;
}//end ClosestJoinQuery

//------------------------------------------------------------------------------

template <class ObjectType, class EvaluatorType>
template <class TupleType>
stResult<ObjectType> * stDummyTree<ObjectType, EvaluatorType>::preConstrainedNearestQuery(
                                                                                          tObject * sample, stCount k, stCount idx, bool (*compare)(const void *, const void *), const void * value) {

  stPage * currPage;
  stDummyNode * currNode;
  //tResult * result;
  tConstrainedResult * result;
  tObject tmp;
  stDistance distance;
  stCount i;
  stPageID nextPageID;

  TupleType tuple;

  // Create result
  //result = new tResult(k);
  result = new tConstrainedResult(k);
  result->SetQueryInfo(sample->Clone(), KNEARESTQUERY, k, -1.0, false);

  // First node
  nextPageID = this->GetRoot();

  // Let's search
  while (nextPageID != 0) {
    // Get node
    currPage = this->myPageManager->GetPage(nextPageID);
    currNode = new stDummyNode(currPage);

    // Lets check all objects in this node
    for (i = 0; i < currNode->GetNumberOfEntries(); i++) {
      // Rebuild the object
      tmp.Unserialize(currNode->GetObject(i), currNode->GetObjectSize(i));

      // Evaluate distance
      distance = this->myMetricEvaluator->GetDistance(&tmp, sample);

      // Is it qualified regarding distance?
      // Obs: first test the distance because the constraint information may not be available in the index, requiring accessing data blocks
      if ((result->GetNumOfEntries() < k) || (distance <= result->GetMaximumDistance())) {

        // Get the tuple
        tuple.Unserialize((stByte *) tmp.GetStrOID(), tmp.GetStrOIDSize());

        // Is it qualified regarding the constraint?
        if (compare(tuple.get(idx), value)) {
          result->AddSatisfyPair(tmp.Clone(), distance);

          // Is it necessary to cut?
          if (result->GetNumOfEntries() > k) {
            result->Cut(k);
          }//end if
        }//end if
      } //end if
    }//end for

    // Next Node...
    nextPageID = currNode->GetNextNode();

    // Free it all
    delete currNode;
    this->myPageManager->ReleasePage(currPage);
  }//end while

  // Return the result.
  return result;
}//end preConstrainedNearestQuery

//------------------------------------------------------------------------------

template <class ObjectType, class EvaluatorType>
template <class TupleType>
stResult<ObjectType> * stDummyTree<ObjectType, EvaluatorType>::intraConstrainedNearestQueryCountGreaterThanOrEqual(//Obs: CountGreaterThan(5) can be CountGreaterThanOrEqual(6)
                                                                                                                   tObject * sample, stCount k, stCount idx, bool (*compare)(const void *, const void *), const void * value,
                                                                                                                   stCount aggValue) {

  stPage * currPage;
  stDummyNode * currNode;
  tConstrainedResult * result;
  tObject tmp;
  stDistance distance;
  stCount i;
  stPageID nextPageID;

  TupleType tuple;

  // Create result
  result = new tConstrainedResult(k);
  result->SetQueryInfo(sample->Clone(), KNEARESTQUERY, k, -1.0, false);

  // First node
  nextPageID = this->GetRoot();

  // Let's search
  while (nextPageID != 0) {
    // Get node
    currPage = this->myPageManager->GetPage(nextPageID);
    currNode = new stDummyNode(currPage);

    // Lets check all objects in this node
    for (i = 0; i < currNode->GetNumberOfEntries(); i++) {
      // Rebuild the object
      tmp.Unserialize(currNode->GetObject(i), currNode->GetObjectSize(i));

      // Evaluate distance
      distance = this->myMetricEvaluator->GetDistance(&tmp, sample);

      // Is it qualified regarding distance?
      // Obs: first test the distance because the constraint information may not be available in the index, requiring accessing data blocks
      if ((result->GetNumOfEntries() < k) || (distance <= result->GetMaximumDistance())) {

        // Get the tuple
        tuple.Unserialize((stByte *) tmp.GetStrOID(), tmp.GetStrOIDSize());

        // Is it qualified regarding the constraint?
        if (compare(tuple.get(idx), value)) {

          // Unnecessary to check. Just add.
          result->AddSatisfyPair(tmp.Clone(), distance);

          // Is it necessary to cut? If it is, there are more than aggValue satisfying elements, so the last can be safely cut.
          if (result->GetNumOfEntries() > k) {
            result->Cut(k);
          }//end if
        }//end if
        else {
          // Is there room for elements not satisfying the constraint?
          if (result->GetNumOfEntriesNotSatisfyPairs() < (k - aggValue)) {
            result->AddNotSatisfyPair(tmp.Clone(), distance);

            // Is it necessary to cut? If it is, there are more than aggValue satisfying elements, so the last can be safely cut.
            if (result->GetNumOfEntries() > k) {
              result->Cut(k);
            }//end if
          }//end if
          else {
            // Is it closer than the farthest element that does not satisfy the constraint?
            if ((result->GetNumOfEntriesNotSatisfyPairs() > 0) && (distance < result->GetLastNotSatisfyPair()->GetDistance())) {

              // Exchange them
              result->AddNotSatisfyPair(tmp.Clone(), distance);
              result->CutNotSatisfyPairs(k - aggValue);
            }//end if
          }//end else
        }//end else
      } //end if
    }//end for

    // Next Node...
    nextPageID = currNode->GetNextNode();

    // Free it all
    delete currNode;
    this->myPageManager->ReleasePage(currPage);
  }//end while

  // Return the result.
  return result;
}//end intraConstrainedNearestQueryCountGreaterThanOrEqual

//------------------------------------------------------------------------------

template <class ObjectType, class EvaluatorType>
template <class TupleType>
stResult<ObjectType> * stDummyTree<ObjectType, EvaluatorType>::intraConstrainedNearestQueryCountLessThanOrEqual(//Obs: CountLessThan(5) can be CountLessThanOrEqual(4)
                                                                                                                tObject * sample, stCount k, stCount idx, bool (*compare)(const void *, const void *), const void * value,
                                                                                                                stCount aggValue) {

  stPage * currPage;
  stDummyNode * currNode;
  tConstrainedResult * result;
  tObject tmp;
  stDistance distance;
  stCount i;
  stPageID nextPageID;

  TupleType tuple;

  // Create result
  result = new tConstrainedResult(k);
  result->SetQueryInfo(sample->Clone(), KNEARESTQUERY, k, -1.0, false);

  // First node
  nextPageID = this->GetRoot();

  // Let's search
  while (nextPageID != 0) {
    // Get node
    currPage = this->myPageManager->GetPage(nextPageID);
    currNode = new stDummyNode(currPage);

    // Lets check all objects in this node
    for (i = 0; i < currNode->GetNumberOfEntries(); i++) {
      // Rebuild the object
      tmp.Unserialize(currNode->GetObject(i), currNode->GetObjectSize(i));

      // Evaluate distance
      distance = this->myMetricEvaluator->GetDistance(&tmp, sample);

      // Is it qualified regarding distance?
      // Obs: first test the distance because the constraint information may not be available in the index, requiring accessing data blocks
      if ((result->GetNumOfEntries() < k) || (distance <= result->GetMaximumDistance())) {

        // Get the tuple
        tuple.Unserialize((stByte *) tmp.GetStrOID(), tmp.GetStrOIDSize());

        // Is it qualified regarding the constraint?
        if (compare(tuple.get(idx), value)) {

          // Is there room for elements satisfying the constraint?
          if (result->GetNumOfEntriesSatisfyPairs() < (aggValue)) {
            result->AddSatisfyPair(tmp.Clone(), distance);

            // Is it necessary to cut? If it is, the number of satisfying elements will not increase, so the last can be safely cut.
            if (result->GetNumOfEntries() > k) {
              result->Cut(k);
            }//end if
          }//end if
          else {
            // Is it closer than the farthest element that satisfies the constraint?
            if ((result->GetNumOfEntriesSatisfyPairs() > 0) && (distance < result->GetLastSatisfyPair()->GetDistance())) {

              // Exchange them
              result->AddSatisfyPair(tmp.Clone(), distance);
              result->CutSatisfyPairs(aggValue);
            }//end if
          }//end else
        }//end if
        else {
          // Unnecessary to check. Just add.
          result->AddNotSatisfyPair(tmp.Clone(), distance);

          // Is it necessary to cut? If it is, the number of satisfying elements will not increase, so the last can be safely cut.
          if (result->GetNumOfEntries() > k) {
            result->Cut(k);
          }//end if
        }//end else
      } //end if
    }//end for

    // Next Node...
    nextPageID = currNode->GetNextNode();

    // Free it all
    delete currNode;
    this->myPageManager->ReleasePage(currPage);
  }//end while

  // Return the result.
  return result;
}//end intraConstrainedNearestQueryCountLessThanOrEqual

//------------------------------------------------------------------------------

template <class ObjectType, class EvaluatorType>
template <class TupleType>
stResult<ObjectType> * stDummyTree<ObjectType, EvaluatorType>::intraConstrainedNearestQueryCountDistinctGreaterThanOrEqual(
                                                                                                                           tObject * sample, stCount k, stCount idx, bool (*compare)(const void *, const void *), const void * value,
                                                                                                                           stCount aggIdx, bool (*aggCompare)(const void *, const void *), stCount aggValue) {

  stPage * currPage;
  stDummyNode * currNode;
  tConstrainedResult * result;
  tObject tmp;
  stDistance distance;
  stCount i;
  stPageID nextPageID;

  TupleType tuple;

  // Create result
  result = new tConstrainedResult(k);
  result->SetQueryInfo(sample->Clone(), KNEARESTQUERY, k, -1.0, false);

  // First node
  nextPageID = this->GetRoot();

  // Let's search
  while (nextPageID != 0) {
    // Get node
    currPage = this->myPageManager->GetPage(nextPageID);
    currNode = new stDummyNode(currPage);

    // Lets check all objects in this node
    for (i = 0; i < currNode->GetNumberOfEntries(); i++) {
      // Rebuild the object
      tmp.Unserialize(currNode->GetObject(i), currNode->GetObjectSize(i));

      // Evaluate distance
      distance = this->myMetricEvaluator->GetDistance(&tmp, sample);

      // Is it qualified regarding distance?
      // Obs: first test the distance because the constraint information may not be available in the index, requiring accessing data blocks
      if ((result->GetNumOfEntries() < k) || (distance <= result->GetMaximumDistance())) {

        // Get the tuple
        tuple.Unserialize((stByte *) tmp.GetStrOID(), tmp.GetStrOIDSize());

        bool duplicateFound = false;

        // Is it qualified regarding the constraint?
        if (compare(tuple.get(idx), value)) {

          // Look for a duplicate
          for (typename tConstrainedResult::tIteSPairs it = result->beginSatisfyPairs(); it != result->endSatisfyPairs(); it++) {
            // Get the object
            tObject * tmpSatisfy;
            tmpSatisfy = (tObject *) (*(*it))->GetObject();

            // Get the tuple
            TupleType tupleSatisfy;
            tupleSatisfy.Unserialize((stByte *) tmpSatisfy->GetStrOID(), tmpSatisfy->GetStrOIDSize());

            // Is it a duplicate?
            if (aggCompare(tuple.get(aggIdx), tupleSatisfy.get(aggIdx))) {

              duplicateFound = true;
              // Is it closer than the duplicate?
              if (distance < (*(*it))->GetDistance()) {

                // Add the closer duplicate to the satisfying list.
                result->AddSatisfyPair(tmp.Clone(), distance);

                // The farther duplicate must now be considered as a non-satisfying element.
                // So, is there room for elements not satisfying the constraint?
                if (result->GetNumOfEntriesNotSatisfyPairs() < (k - aggValue)) {
                  // Yes, move it to the not satisfying list.
                  result->ToNotSatisfyPairs(it);

                  // If it is necessary to cut, there are more than aggValue satisfying elements, so the last can be safely cut.
                  result->Cut(k);
                }//end if
                else {
                  // No. Is it closer than the farthest element that does not satisfy the constraint?
                  if ((result->GetNumOfEntriesNotSatisfyPairs() > 0) && ((*(*it))->GetDistance() < result->GetLastNotSatisfyPair()->GetDistance())) {

                    // Yes, exchange them.
                    result->ToNotSatisfyPairs(it);
                    result->CutNotSatisfyPairs(k - aggValue);
                  }//end if
                  else {
                    // No, remove it.
                    result->RemoveSatisfyPairs(it);
                  }
                }//end else

              }//end if
              else {
                // As tmp will not be considered in the counting constraint, it can be treated as a non-satisfying element.
                // So, is there room for elements not satisfying the constraint?
                if (result->GetNumOfEntriesNotSatisfyPairs() < (k - aggValue)) {
                  // Yes, add it to the not satisfying list.
                  result->AddNotSatisfyPair(tmp.Clone(), distance);

                  // If it is necessary to cut, there are more than aggValue satisfying elements, so the last can be safely cut.
                  result->Cut(k);

                }//end if
                else {
                  // Is it closer than the farthest element that does not satisfy the constraint?
                  if ((result->GetNumOfEntriesNotSatisfyPairs() > 0) && (distance < result->GetLastNotSatisfyPair()->GetDistance())) {

                    // Exchange them
                    result->AddNotSatisfyPair(tmp.Clone(), distance);
                    result->CutNotSatisfyPairs(k - aggValue);
                  }//end if
                }//end else
              }//end else
              break; // OBS: it is necessary to break the loop to avoid testing the loop's end condition, as the next iterator position could be deleted, yielding a segmentation fault.
            }//end if
          }//end for

          if (!duplicateFound) {
            result->AddSatisfyPair(tmp.Clone(), distance);

            // Is it necessary to cut?
            if (result->GetNumOfEntries() > k) {
              // Is there more than aggValue satisfying elements?
              if (result->GetNumOfEntriesSatisfyPairs() > aggValue) {
                // Yes, so the last can be safely cut.
                result->Cut(k);
              }
              else {
                // No, cut the last non-satisfying element.
                result->CutNotSatisfyPairs(k - aggValue);
              }
            }//end if
          }//end if
        }//end if
        else {
          // Is there room for elements not satisfying the constraint?
          if (result->GetNumOfEntriesNotSatisfyPairs() < (k - aggValue)) {
            result->AddNotSatisfyPair(tmp.Clone(), distance);

            // If it is necessary to cut, there are more than aggValue satisfying elements, so the last can be safely cut.
            result->Cut(k);
          }//end if
          else {
            // Is it closer than the farthest element that does not satisfy the constraint?
            if ((result->GetNumOfEntriesNotSatisfyPairs() > 0) && (distance < result->GetLastNotSatisfyPair()->GetDistance())) {
              // Exchange them
              result->AddNotSatisfyPair(tmp.Clone(), distance);
              result->CutNotSatisfyPairs(k - aggValue);
            }//end if
          }//end else
        }//end else
      } //end if
    }//end for

    // Next Node...
    nextPageID = currNode->GetNextNode();

    // Free it all
    delete currNode;
    this->myPageManager->ReleasePage(currPage);
  }//end while

  // Return the result.
  return result;
}//end intraConstrainedNearestQueryCountDistinctGreaterThanOrEqual

//------------------------------------------------------------------------------

template <class ObjectType, class EvaluatorType>
template <class TupleType>
stResult<ObjectType> * stDummyTree<ObjectType, EvaluatorType>::intraConstrainedNearestQueryCountDistinctLessThanOrEqual(
                                                                                                                        tObject * sample, stCount k, stCount idx, bool (*compare)(const void *, const void *), const void * value,
                                                                                                                        stCount aggIdx, bool (*aggCompare)(const void *, const void *), stCount aggValue) {

  stPage * currPage;
  stDummyNode * currNode;
  tConstrainedResult * result;
  tObject tmp;
  stDistance distance;
  stCount i;
  stPageID nextPageID;

  TupleType tuple;

  // Create result
  result = new tConstrainedResult(k);
  result->SetQueryInfo(sample->Clone(), KNEARESTQUERY, k, -1.0, false);

  // First node
  nextPageID = this->GetRoot();

  // Let's search
  while (nextPageID != 0) {
    // Get node
    currPage = this->myPageManager->GetPage(nextPageID);
    currNode = new stDummyNode(currPage);

    // Lets check all objects in this node
    for (i = 0; i < currNode->GetNumberOfEntries(); i++) {
      // Rebuild the object
      tmp.Unserialize(currNode->GetObject(i), currNode->GetObjectSize(i));

      // Evaluate distance
      distance = this->myMetricEvaluator->GetDistance(&tmp, sample);

      // Is it qualified regarding distance?
      // Obs: first test the distance because the constraint information may not be available in the index, requiring accessing data blocks
      if ((result->GetNumOfEntries() < k) || (distance <= result->GetMaximumDistance())) {

        // Get the tuple
        tuple.Unserialize((stByte *) tmp.GetStrOID(), tmp.GetStrOIDSize());

        // Is it qualified regarding the constraint?
        if (compare(tuple.get(idx), value)) {

          bool duplicateFound = false;

          for (typename tConstrainedResult::tIteSPairs it = result->beginSatisfyPairs(); it != result->endSatisfyPairs(); it++) {
            // Get the object
            tObject * tmpSatisfy;
            tmpSatisfy = (tObject *) (*(*it))->GetObject();

            // Get the tuple
            TupleType tupleSatisfy;
            tupleSatisfy.Unserialize((stByte *) tmpSatisfy->GetStrOID(), tmpSatisfy->GetStrOIDSize());

            // Is it a duplicate?
            if (aggCompare(tuple.get(aggIdx), tupleSatisfy.get(aggIdx))) {

              duplicateFound = true;
              // Is it closer than the duplicate?
              if (distance < (*(*it))->GetDistance()) {
                // Exchange them (the current element with its duplicate)
                result->AddSatisfyPair(tmp.Clone(), distance);
                result->RemoveSatisfyPairs(it);// the duplicate
                break;
              }//end if
            }//end if
          }//end for

          if (!duplicateFound) {
            // Is there room for elements satisfying the constraint?
            if (result->GetNumOfEntriesSatisfyPairs() < (aggValue)) {
              result->AddSatisfyPair(tmp.Clone(), distance);

              // If it is necessary to cut, the number of satisfying elements will not become greater than aggValue, so the last can be safely cut.
              result->Cut(k);
            }//end if
            else {
              // Is it closer than the farthest element that satisfies the constraint?
              if ((result->GetNumOfEntriesSatisfyPairs() > 0) && (distance < result->GetLastSatisfyPair()->GetDistance())) {

                // Exchange them (the current element with the last satisfying one)
                result->AddSatisfyPair(tmp.Clone(), distance);
                result->CutSatisfyPairs(aggValue);// the last satisfying element
              }//end if
            }//end else
          }//end if
        }//end if
        else {
          // Unnecessary to check. Just add.
          result->AddNotSatisfyPair(tmp.Clone(), distance);

          // If it is necessary to cut, the number of satisfying elements will not increase, so the last can be safely cut.
          result->Cut(k);
        }//end else
      } //end if
    }//end for

    // Next Node...
    nextPageID = currNode->GetNextNode();

    // Free it all
    delete currNode;
    this->myPageManager->ReleasePage(currPage);
  }//end while

  // Return the result.
  return result;
}//end intraConstrainedNearestQueryCountDistinctLessThanOrEqual
