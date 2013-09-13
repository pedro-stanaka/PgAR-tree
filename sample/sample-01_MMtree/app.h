//---------------------------------------------------------------------------
// app.h - Implementation of the application.
//
// Author: Fabio Jun Takada Chino (chino@icmc.sc.usp.br)
// Copyright (c) 2002 GBDI-ICMC-USP
//---------------------------------------------------------------------------
#ifndef appH
#define appH

// Metric Tree includes
#include <arboretum/stMetricTree.h>
#include <arboretum/stMMTree.h>
#include <arboretum/stMemoryPageManager.h>
#include <arboretum/stDiskPageManager.h>

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
      * This is the type of the Metric Tree defined by TCity and
      * TCityDistanceEvaluator.
      */
      typedef stMetricTree < TCity, TCityDistanceEvaluator > myMetricTree;

      /**
      * This is the type of the Dummy Tree defined by TCity and
      * TCityDistanceEvaluator.
      */
      typedef stMMTree < TCity, TCityDistanceEvaluator > myMMTree;

      /**
      * Creates a new instance of this class.
      */
      TApp(){
         PageManager = NULL;
         MetricTree = NULL;
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
      stPageManager * PageManager;

      /**
      * The Metric Tree.
      */
      myMetricTree * MetricTree;

      /**
      * Creates a memory page manager. It must be called before CreateTree().
      */
      void CreateMemoryPageManager();

      /**
      * Creates a disk page manager. It must be called before CreateTree().
      */
      void CreateDiskPageManager();

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
         TCity * city;

         city = new TCity(name, lat, lon);
         MetricTree->Add(city);
         delete city;
      }//end AddToTree

      /**
      * Loads the tree with a set of cities.
      */
      void LoadTree();

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
