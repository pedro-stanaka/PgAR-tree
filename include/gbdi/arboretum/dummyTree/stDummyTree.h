/**********************************************************************
* GBDI Arboretum - Copyright (c) 2002-2004 GBDI-ICMC-USP
*
*                           Homepage: http://gbdi.icmc.usp.br/arboretum
**********************************************************************/
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
#ifndef __STDUMMYTREE_H
#define __STDUMMYTREE_H

#include <math.h>
#include <exception>
#include <iostream>
using namespace std;

#include <gbdi/util/stCommon.h>
#include <gbdi/datatype/stTypes.h>
#include <gbdi/arboretum/stMetricTree.h>
#include <gbdi/arboretum/dummyTree/stDummyNode.h>


/**
* This class template implements a Dummy Tree. The Dummy Tree has the same
* external interface and behavior of a Metric Tree but is implemented as
* sequential list.
*
* <P>It can perform both range and k-nearest neighbout queries but without
* the performance associated with other structures. Its algorithms always
* have O(N) (except for insertion which is O(1)). In other words, it will
* take a lot of time to give answers but it will assure their correctness.
*
* <P>This class was developed to generate perfect answers to queries. It
* allows the build of automated test programs for other metric trees
* implemented by this library.
*
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @todo Finish the implementation.
* @todo Compute statistics.
* @version 1.0
* @ingroup dummy
*/
template <class ObjectType, class EvaluatorType>
class stDummyTree: public stMetricTree<ObjectType, EvaluatorType>{

   public:

      /**
      * This is the class that abstracts the object used by this metric tree.
      */
      typedef ObjectType tObject;

      /**
      * This is the class that abstracts the metric evaluator used by this metric
      * tree.
      */
      typedef EvaluatorType tMetricEvaluator;

      /**
      * This is the class that abstracs an result set for simple queries.
      */
      typedef stResult <ObjectType> tResult;

      /**
      * This is the class that abstracs an result set for simple queries.
      */
      typedef stConstrainedResult <ObjectType> tConstrainedResult;

      /**
      * This is the class that abstracs an result set for joined queries.
      */
      typedef stJoinedResult <ObjectType> tJoinedResult;



     class Iterator{
     public:
       Iterator(){
         pageID_ = 0;
         entryOnPage_ = 0;
         pageManager_ = NULL;
         currPage = NULL;
         currNode = NULL;
         numberOfEntries_ = 0;
         obj_ = new tObject();
       }

       Iterator(stPageID pageID, int entryOnPage, stPageManager* pageManager){
         currPage = NULL;
         currNode = NULL;
         numberOfEntries_ = 0;
         obj_ = new tObject();

         pageID_ = pageID;
         entryOnPage_ = entryOnPage;
         pageManager_ = pageManager;

         load();
       }

       virtual ~Iterator(){
         if(currNode)
           delete currNode;
         if(currPage)
           pageManager_->ReleasePage(currPage);
         if(obj_)
           delete obj_;
       }


       stPageID getPageID(){
         return pageID_;
       }

       int entryOnPage(){
         return entryOnPage_;
       }

       tObject* getObject(){
         return obj_;
       }

       tObject* operator*(){
         return getObject();
       }


       Iterator& operator++(int){
         entryOnPage_++;
         if(entryOnPage_ < numberOfEntries_){
           loadObject();
         }else{

           pageID_ = currNode->GetNextNode();
           entryOnPage_ = 0;

           delete currNode;
           pageManager_->ReleasePage(currPage);
           currNode = NULL;
           currPage = NULL;

           load();
         }
       }

       bool operator==(const Iterator& it){
         if(pageID_ == it.pageID_ && entryOnPage_ == it.entryOnPage_)
           return true;
         else
           return false;
       }

       bool operator!=(const Iterator& it){
         if(pageID_ == it.pageID_ && entryOnPage_ == it.entryOnPage_)
           return false;
         else
           return true;
       }


       Iterator& operator=(const Iterator& it){
         if(currNode)
           delete currNode;
         if(currPage)
           pageManager_->ReleasePage(currPage);

         currPage = NULL;
         currNode = NULL;
         numberOfEntries_ = 0;

         pageID_ = it.pageID_;
         entryOnPage_ = it.entryOnPage_;
         pageManager_ = it.pageManager_;

         load();
       }
     private:
       bool load(){
         if(pageID_ != 0){

           currPage = pageManager_->GetPage(pageID_);
           currNode = new stDummyNode(currPage);

           numberOfEntries_ = currNode->GetNumberOfEntries();

           return loadObject();
         }
         return false;
       }


       bool loadObject(){
         if(entryOnPage_ < numberOfEntries_){
           obj_->Unserialize(currNode->GetObject(entryOnPage_), currNode->GetObjectSize(entryOnPage_));
           return true;
         }else
           throw;
         return false;
       }

       stPageID pageID_;
       int entryOnPage_;
       stPageManager* pageManager_;
       tObject* obj_;


       stPage* currPage;
       stDummyNode* currNode;
       int numberOfEntries_;
     };

     Iterator begin(){
       stPageID pageID;
       int entryOnPage;

       pageID = this->GetRoot();
       entryOnPage = 0;
       return Iterator(pageID, entryOnPage, this->myPageManager);
     }

     Iterator end(){
       return Iterator(0, 0, this->myPageManager);
     }





      /**
      * Creates a new instance of the Dummy Tree.
      *
      * @param pageman The Page Manager to be used.
      */
      stDummyTree(stPageManager * pageman);


      stDummyTree(stPageManager * pageman, EvaluatorType* metricEvaluator);

      /**
      * Disposes this tree and releases all associated resources.
      */
      virtual ~stDummyTree(){

         if (HeaderPage != NULL){
            // Release the header page.
            this->myPageManager->ReleasePage(HeaderPage);
         }//end if
      }//end ~stDummyTree

      /**
      * This method adds an object to the metric tree. This method may fail it the object size
      * exceeds the page size - 16.
      *
      * @param obj The object to be added.
      * @return True for success or false otherwise.
      */
      virtual bool Add(tObject * obj);

      /**
      * Returns the number of objetcs of this sequencial scan.
      */
      virtual long GetNumberOfObjects(){
         return Header->ObjectCount;
      }//end GetNumberOfObjects

      /**
      * Returns the number of nodes of this tree.
      */
      virtual long GetNodeCount(){
         return Header->NodeCount;
      }//end GetNodeCount

      /**
      * Returns the MaxOccupation of the nodes.
      */
      virtual stCount GetMaxOccupation(){
         return Header->MaxOccupation;
      }//end GetMaxOccupation

      /**
      * Returns the MaxOccupation of the nodes.
      */
      void SetMaxOccupation(stCount newValue){
         if (newValue > Header->MaxOccupation){
            Header->MaxOccupation = newValue;
         }//end if
      }//end SetMaxOccupation

      /**
      * Returns the root pageID.
      */
      stPageID GetRoot(){
         return Header->Root;
      }//end GetRoot

      /**
      * This method will perform a range query. The result will be a set of
      * pairs object/distance.
      *
      * @param sample The sample object.
      * @param range The range of the results.
      * @return The result or NULL if this method is not implemented.
      * @warning The instance of tResult returned must be destroied by user.
      */
      virtual tResult * RangeQuery(tObject * sample, stDistance range);

      /**
      * This method will perform a reverse range query.
      * The result will be a set of pairs object/distance.
      *
      * @param sample The sample object.
      * @param range The range of the results. All object that are
      *  greater than the range distance will be included in the result set.
      * @return The result or NULL if this method is not implemented.
      * @warning The instance of tResult returned must be destroied by user.
      */
      virtual tResult * ReversedRangeQuery(tObject * sample, stDistance range);

      /**
      * This method will perform a k nearest neighbor query.
      *
      * @param sample The sample object.
      * @param k The number of neighbours.
      * @param tie The tie list. Default false.
      * @return The result or NULL if this method is not implemented.
      * @warning The instance of tResult returned must be destroied by user.
      */
      virtual tResult * NearestQuery(tObject * sample, stCount k, bool tie = false);

      /**
      * This method will perform a K-Farthest neighbor query.
      *
      * @param sample The sample object.
      * @param k The number of neighbours.
      * @param tie The tie list. Default false.
      * @return The result or NULL if this method is not implemented.
      * @warning The instance of tResult returned must be destroied by user.
      */
      virtual tResult * FarthestQuery(tObject * sample, stCount k, bool tie = false);

      /**
      * This method will perform a range query with a limited number of results.
      *
      * <P>This query is a combination of the standard range query and the standard
      * k-nearest neighbour query. All objects which matches both conditions
      * will be included in the result.
      *
      * @param sample The sample object.
      * @param range The range of the results.
      * @param k The maximum number of results.
      * @param tie The tie list. This parameter is optional. Default false;
      * @return The result or NULL if this method is not implemented.
      * @warning The instance of tResult returned must be destroied by user.
      */
      virtual tResult * KAndRangeQuery(tObject * sample, stDistance range,
            stCount k, bool tie=false);

      /**
      * This method will perform range query with a limited number of results.
      *
      * <P>This query is a combination of the standard range query and the
      * standard k-nearest neighbour query. All objects which matches with
      * one of two conditions will be included in the result.
      *
      * @param sample The sample object.
      * @param range The range of the results.
      * @param k The maximum number of results.
      * @param tie The tie list. This parameter is optional. Default false;
      * @return The result or NULL if this method is not implemented.
      * @warning The instance of tResult returned must be destroied by user.
      */
      virtual tResult * KOrRangeQuery(tObject * sample, stDistance range,
            stCount k, bool tie=false);

      /**
      * This method will perform a ring query. The result will be a set of
      * pairs object/distance.
      *
      * <P>The object pointed by <b>sample</b> will not be destroyed by this
      * method.
      *
      * @param sample The sample object.
      * @param inRange The inner range of the results.
      * @param outRange The outter range of the results.
      * @return The result.
      * @warning The instance of tResult returned must be destroied by user.
      * @warning The inRange must be less than the outRange.
      * @exception unsupported_method_error If this method is not supported by this tree.
      */
      virtual tResult * RingQuery(tObject * sample, stDistance inRange,
            stDistance outRange);

      /**
      * This method will perform a grouping SUM range query. The result will be a set of
      * pairs object/distance.
      *
      * @param sampleList The list of sample objects.
      * @param sampleSize The number of samples of the param sampleList.
      * @param range The range of the results.
      * @return The result or NULL if this method is not implemented.
      * @warning The instance of tResult returned must be destroied by user.
      * @autor Implemented by Humberto Razente
      */
      tResult * SumRangeQuery(tObject ** sampleList, stSize sampleSize,
                              stDistance range);

      /**
      * This method will perform a grouping SUM k nearest neighbor query.
      *
      * @param sampleList The list of sample objects.
      * @param sampleSize The number of samples of the param sampleList.
      * @param k The number of neighbours.
      * @param tie The tie list. Default false.
      * @return The result or NULL if this method is not implemented.
      * @warning The instance of tResult returned must be destroied by user.
      */
      tResult * SumNearestQuery(tObject ** sampleList, stSize sampleSize,
                                stCount k, bool tie = false);

      /**
      * This method will perform a grouping MAX range query. The result will be a set of
      * pairs object/distance.
      *
      * @param sampleList The list of sample objects.
      * @param sampleSize The number of samples of the param sampleList.
      * @param range The range of the results.
      * @return The result or NULL if this method is not implemented.
      * @warning The instance of tResult returned must be destroied by user.
      * @autor Implemented by Humberto Razente
      */
      tResult * MaxRangeQuery(tObject ** sampleList, stSize sampleSize,
                              stDistance range);

      /**
      * This method will perform a grouping MAX k nearest neighbor query.
      *
      * @param sampleList The list of sample objects.
      * @param sampleSize The number of samples of the param sampleList.
      * @param k The number of neighbours.
      * @param tie The tie list. Default false.
      * @return The result or NULL if this method is not implemented.
      * @warning The instance of tResult returned must be destroied by user.
      */
      tResult* MaxNearestQuery(tObject ** sampleList, stSize sampleSize,
                               stCount k, bool tie = false);

      /**
      * This method will perform a grouping ALL range query. The result will be a set of
      * pairs object/distance.
      *
      * @param sampleList The list of sample objects.
      * @param sampleSize The number of samples of the param sampleList.
      * @param range The range of the results.
      * @return The result or NULL if this method is not implemented.
      * @warning The instance of tResult returned must be destroied by user.
      * @autor Implemented by Humberto Razente
      */
      tResult * AllRangeQuery(tObject ** sampleList, stSize sampleSize,
                              stDistance range);

      /**
      * This method will perform a grouping ALL k nearest neighbor query.
      *
      * @param sampleList The list of sample objects.
      * @param sampleSize The number of samples of the param sampleList.
      * @param k The number of neighbours.
      * @param tie The tie list. Default false.
      * @return The result or NULL if this method is not implemented.
      * @warning The instance of tResult returned must be destroied by user.
      */
      tResult* AllNearestQuery(tObject ** sampleList, stSize sampleSize,
                               stCount k, bool tie = false);

      /**
      * This method will perform a k-nearest neighbor joined query.
      *
      * @param dummyTree The Dummy tree to be joined.
      * @param k The number of neighbours.
      * @param tie The tie list. Default false.
      * @return The result or NULL if this method is not implemented.
      * @warning The instance of tJoinedResult returned must be destroied by user.
      * @autor Implemented by Humberto Razente
      */
      tJoinedResult * NearestJoinQuery(stDummyTree * dummyTree, stCount k,
                                       bool tie = false);

      /**
      * This method will perform a range joined query.
      *
      * @param dummyTree The dummyTree to be joined.
      * @param range The range of the results.
      * @return The result or NULL if this method is not implemented.
      * @warning The instance of tJoinedResult returned must be destroied by user.
      * @autor Implemented by Humberto Razente
      */
      tJoinedResult * RangeJoinQuery(stDummyTree * dummyTree, stDistance range);

      /**
      * This method will perform a k-closest neighbor joined query.
      *
      * @param dummyTree The Dummy tree to be joined.
      * @param k The number of neighbours.
      * @param tie The tie list. Default false.
      * @return The result or NULL if this method is not implemented.
      * @warning The instance of tJoinedResult returned must be destroied by user.
      * @autor Implemented by Humberto Razente
      */
      tJoinedResult * ClosestJoinQuery(stDummyTree * dummyTree, stCount k,
                                       bool tie = false);

      /**
      * This method will perform a PRE-CONDITION CONSTRAINED k nearest neighbor query.
      *
      * @param sample The sample object.
      * @param k The number of neighbours.
      * @param tie The tie list. Default false.
      * @return The result or NULL if this method is not implemented.
      * @warning The instance of tResult returned must be destroied by user.
      */
      template <class TupleType>
      tResult * preConstrainedNearestQuery(tObject * sample, stCount k, stCount idx,
            bool (*compare)(const void *, const void *), const void * value);

      template <class TupleType>
      tResult * intraConstrainedNearestQueryCountGreaterThanOrEqual(tObject * sample,
            stCount k, stCount idx, bool (*compare)(const void *, const void *), const void * value,
            stCount aggValue);

      template <class TupleType>
      tResult * intraConstrainedNearestQueryCountLessThanOrEqual(tObject * sample,
            stCount k, stCount idx, bool (*compare)(const void *, const void *), const void * value,
            stCount aggValue);

      template <class TupleType>
      tResult * intraConstrainedNearestQueryCountDistinctGreaterThanOrEqual(tObject * sample,
            stCount k, stCount idx, bool (*compare)(const void *, const void *), const void * value,
            stCount aggIdx, bool (*aggCompare)(const void *, const void *), stCount aggValue);

      template <class TupleType>
      tResult * intraConstrainedNearestQueryCountDistinctLessThanOrEqual(tObject * sample,
            stCount k, stCount idx, bool (*compare)(const void *, const void *), const void * value,
            stCount aggIdx, bool (*aggCompare)(const void *, const void *), stCount aggValue);

   private:

      /**
      * This type defines the header of the Dummy Tree.
      */
      typedef struct DummyTreeHeader{
         /**
         * The root.
         */
         stPageID Root;

         /**
         * The number of the objects in this page.
         */
         stSize ObjectCount;

         /**
         * Total number of nodes.
         */
         stSize NodeCount;

         /**
         * Maximum number of objects in a node.
         */
         stCount MaxOccupation;
      } stDummyTreeHeader;

      /**
      * The header page. It will be kept in memory all the time to avoid
      * reads.
      */
      stPage * HeaderPage;

      /**
      * The header of the "tree".
      */
      stDummyTreeHeader * Header;

      /**
      * If true, the header must be written to the page manager.
      */
      bool HeaderUpdate;

      /**
      * Creates the header for an empty tree.
      */
      void Create(){

         LoadHeader();
         Header->Root = 0;
         Header->ObjectCount = 0;
         Header->NodeCount = 0;
         Header->MaxOccupation = 0;
         HeaderUpdate = true;
         WriteHeader();
      }//end Create

      /**
      * Loads the header from the page manager.
      */
      void LoadHeader(){
         HeaderPage = this->myPageManager->GetHeaderPage();
         Header = (stDummyTreeHeader *) HeaderPage->GetData();
      }//end LoadHeader

      /**
      * Writes the header into the Page Manager.
      */
      void WriteHeader(){
         if (HeaderUpdate){
            this->myPageManager->WriteHeaderPage(HeaderPage);
            HeaderUpdate = false;
         }//end if
      }//end WriteHeader

      /**
      * Set the new root pageID.
      */
      void SetRoot(stPageID newRootPageID){
         Header->Root = newRootPageID;
      }//end SetRoot

      /**
      * Updates the object counter.
      */
      void UpdateObjectCounter(int inc){
         Header->ObjectCount += inc;
         HeaderUpdate = true;
      }//end UpdateObjectCounter

};//end stDummyTree

#include "stDummyTree.cc"

#endif //__STDUMMYTREE_H
