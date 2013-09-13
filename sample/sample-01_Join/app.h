//---------------------------------------------------------------------------
// app.h - Implementation of the application.
//
// Author: Author: Enzo Seraphim (seraphim@icmc.sc.usp.br)
// Copyright (c) 2004 GBDI-ICMC-USP
//---------------------------------------------------------------------------
#ifndef appH
#define appH

// Metric Tree includes
#include <arboretum/stMetricTree.h>
#include <arboretum/stMemoryPageManager.h>
#include <arboretum/stDiskPageManager.h>
#include <arboretum/stDummyTree.h>
#include <arboretum/stSeqTree.h>
#include <arboretum/stSlimTree.h>
#include <arboretum/stPartitionHash.h>

// My object
#include "myobject.h"

//---------------------------------------------------------------------------
// class TApp
//---------------------------------------------------------------------------
class TApp{
   public:
      /**
      * This is the type used by the result.
      */
      typedef stResult < TCity > myResult;

      /**
      * This is the type of the Dummy Tree defined by TCity and
      * TCityDistanceEvaluator.
      */
      typedef stDummyTree < TCity, TCityDistanceEvaluator > myDummyTree;

      /**
      * This is the type of the Seq Tree defined by TCity and
      * TCityDistanceEvaluator.
      */
      typedef stSeqTree < TCity, TCityDistanceEvaluator > mySeqTree;

      /**
      * This is the type of the Slim Tree defined by TCity and
      * TCityDistanceEvaluator.
      */
      typedef stSlimTree < TCity, TCityDistanceEvaluator > mySlimTree;

      /**
      * This is the type of the Partition defined by TCity and
      * TCityDistanceEvaluator.
      */
      typedef stPartitionHash < TCity, TCityDistanceEvaluator > myPartitionHash;

      /**
      * Creates a new instance of this class.
      */
      TApp(){
         PageManagerDummy = NULL;
         DummyTree = NULL;
         PageManagerDummyJoin = NULL;
         DummyTreeJoin = NULL;
         PageManagerSlim = NULL;
         PageManagerSeq = NULL;
         SeqTree = NULL;
         PageManagerSeqJoin = NULL;
         SeqTreeJoin = NULL;
         PageManagerSlim = NULL;
         SlimTree = NULL;
         PageManagerSlimJoin = NULL;
         SlimTreeJoin = NULL;
         PageManagerPartition = NULL;
         Partition = NULL;
         PageManagerPartitionJoin = NULL;
         PartitionJoin = NULL;
      }//end TApp

      /**
      * Initializes the application.
      */
      void Init();

      /**
      * Runs the application.
      */
      void Run();

      /**
      * Deinitialize the application.
      */
      void Done();
   private:
      /**
      * The Page Manager.
      */
      stDiskPageManager * PageManagerDummy;
      stDiskPageManager * PageManagerDummyJoin;
      stDiskPageManager * PageManagerSeq;
      stDiskPageManager * PageManagerSeqJoin;
      stDiskPageManager * PageManagerSlim;
      stDiskPageManager * PageManagerSlimJoin;
      stDiskPageManager * PageManagerPartition;
      stDiskPageManager * PageManagerPartitionJoin;

      /**
      * The Tree e Hash.
      */
      myDummyTree * DummyTree;
      myDummyTree * DummyTreeJoin;
      mySeqTree * SeqTree;
      mySeqTree * SeqTreeJoin;
      mySlimTree * SlimTree;
      mySlimTree * SlimTreeJoin;
      myPartitionHash * Partition;
      myPartitionHash * PartitionJoin;

      /**
      * Creates a tree using the current Page Manager. Call
      * CreateMemoryPageManager() or CreateDiskPageManager() before it to create
      * a PageManager.
      */
      void CreateTree();

      /**
      * Adds a city to tree.
      *
      * @param name City's name.
      * @param lat City's latitude.
      * @param lon City's longitude.
      */
      void AddToTree(char * name, double lat, double lon){
         TCity * city = new TCity(name, lat, lon);
         DummyTree->Add(city);
         SeqTree->Add(city);
         SlimTree->Add(city);
//         Partition->Add(city);
         delete city;
      }//end AddToTree

      void AddToTreeJoin(char * name, double lat, double lon){
         TCity * city = new TCity(name, lat, lon);
         DummyTreeJoin->Add(city);
         SeqTreeJoin->Add(city);
         SlimTreeJoin->Add(city);
//         PartitionJoin->Add(city);
         delete city;
      }//end AddToTreeJoin

      /**
      * Loads the tree from file with a set of cities.
      */
      void LoadTree(char *fileNameIndex, char *fileNameJoin);

      /**
      * Performs a query and outputs its results.
      */
      void PerformRangeJoinDummy();
      void PerformRangeJoinSeq();
      void PerformRangeJoinSlim();
      void PerformRangeJoinPartition();
      void PerformNearestJoinDummy();
      void PerformNearestJoinSeq();
      void PerformNearestJoinSlim();
      void PerformNearestJoinPartition();

      /**
      * Prints the heuristic values.
      */
      void PrintHeuristics(bool printJoin);

      /**
      * Reset Heuristiscs
      */
      void ResetHeuristics();

      /**
      * Prints the result set to the standard output.
      *
      * @param result The result to be printed.
      */
      void PrintResults(myResult * result);

};//end TApp

#endif //end appH
