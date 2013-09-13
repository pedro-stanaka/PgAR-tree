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
#include <arboretum/stSlimTree.h>
#include <arboretum/stPartitionHash.h>
//#include <arboretum/stDBMTree.h>

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
      typedef stSlimTree < TCity, TCityDistanceEvaluator > mySlimTree;

      /**
      * This is the type of the Dummy Tree defined by TCity and
      * TCityDistanceEvaluator.
      */
      typedef stPartitionHash < TCity, TCityDistanceEvaluator > myPartitionHash;

      /**
      * This is the type of the Dummy Tree defined by TCity and
      * TCityDistanceEvaluator.
      */
//      typedef stDBMTree < TCity, TCityDistanceEvaluator > myDBMTree;

      /**
      * Creates a new instance of this class.
      */
      TApp(){
         PageManagerSlim = NULL;
         SlimTree = NULL;
         PageManagerPartition = NULL;
         Partition = NULL;
//         PageManagerDBM = NULL;
//         DBMTree = NULL;
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
      stDiskPageManager * PageManagerSlim;
      stDiskPageManager * PageManagerPartition;
//      stDiskPageManager * PageManagerDBM;


      /**
      * The Tree e Hash.
      */
      mySlimTree * SlimTree;
      myPartitionHash * Partition;
//      myDBMTree * DBMTree;

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
         SlimTree->Add(city);
         Partition->Add(city);
//         DBMTree->Add(city);
         delete city;
      }//end AddToTree

      /**
      * Loads the tree from file with a set of cities.
      */
      void LoadTree(char *filename);

      /**
      * Performs a query and outputs its results.
      */
      void PerformQuery();

      /**
      * Prints the result set to the standard output.
      *
      * @param result The result to be printed.
      */
      void PrintResults(myResult * result);

};//end TApp

#endif //end appH
