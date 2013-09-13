//---------------------------------------------------------------------------
// Implementation of the application.
//
// Author: Author: Enzo Seraphim (seraphim@icmc.sc.usp.br)
// Copyright (c) 2004 GBDI-ICMC-USP
//---------------------------------------------------------------------------
#include <iostream>
#include <cstdio>
#include "app.h"

using namespace std;

//---------------------------------------------------------------------------
// Class TApp
//---------------------------------------------------------------------------
// Initializes the application.
void TApp::Init(){
        // Creates the tree
   CreateTree();
}//end TApp::Init

//---------------------------------------------------------------------------
// Runs the application.
void TApp::Run(){
  // Initiating counters
  SlimTree->GetMetricEvaluator()->ResetStatistics();
  Partition->GetMetricEvaluator()->ResetStatistics();

  // Lets load the tree with a lot values.
  LoadTree("cidades.txt");//manual (para criar o arquivo)

  cout << "[SLIM] Number of distance-calculators:" << SlimTree->GetMetricEvaluator()->GetDistanceCount()<< endl;
  cout << "[SLIM] Height = " << SlimTree->GetHeight() << endl;
  cout << "[SLIM] Node = " << SlimTree->GetNodeCount() << endl;

  cout << "[PART] Number of distance-calculators:" << Partition->GetMetricEvaluator()->GetDistanceCount()<< endl;
  cout << "[PART] Height = " << Partition->GetHeight() << endl;
  cout << "[PART] IndexBucket = " << Partition->GetIndexBucketCount() << endl;
  cout << "[PART] LeafBucket = " << Partition->GetLeafBucketCount() << endl;
  cout << "[PART] RepNode = " << Partition->GetRepNodeCount() << endl;
  cout << "[PART] NumberRep = " << Partition->GetGlobalNumberRep() << endl;
  
  TCity * tmpObj;
  cout << "object Representatives: " << endl;
  for (int i = 0; i < Partition->GetGlobalNumberRep(); i++){
     tmpObj = Partition->GetGlobalRepObject(i);
     cout << tmpObj->GetName() << endl;
  }//end for
  
  TCity * sample = new TCity("Dourado", -22.1, -48.31);
//  TCity * sample = new TCity("Zortéa - SC", -27.45, -51.55);
  
  stPartitionRegionDesc reg;
  Partition->ObjectHashFunction(sample, reg);
  
  cout << "ObjectHash = [" << (int)reg.GetNumberRep() << "] " << reg.GetRegion()  << endl;
  
  myResult * res = Partition->SequencialPointQuery(sample);
  cout << "Point Query: " << endl;
  for (int i = 0; i < res->GetNumOfEntries(); i++){
    tmpObj = (TCity *)(*res)[i].GetObject();
    cout << "Dist= " << (*res)[i].GetDistance() <<" ";
    cout << sample->GetName() << endl;
  }//end for
  delete sample;
   
/*  
  sample = new TCity("Dourado", -22.1, -48.31);
  stPartitionRegionDesc reg;
  Partition->ObjectHashFunction(sample, reg);
  cout << "ObjectHash = [" << (int)reg.GetNumberRep() << "] " << reg.GetRegion()  << endl;
  
  stPartitionListRegionDesc list;
  Partition->RadiusHashFunction(sample, 60, list);
  cout << "RadiusHash = " << endl;
  
/*  for (int i = 0; i < list.GetNumberItem(); i++){
     cout << " [" << (int)list.GetRegionDesc(i).GetNumberRep() << "]   " << list.GetRegionDesc(i).GetRegion() << " | ";
  }//end for
  cout << endl;
*/
//  cout << "[DBM] Number of distance-calculators:" << DBMTree->GetMetricEvaluator()->GetDistanceCount()<< endl;
//  cout << "[DBM] Height = " << DBMTree->GetHeight() << endl;
//  cout << "[DBM] Node = " << DBMTree->GetNodeCount() << endl;

  //Queries
  //PerformQuery();

}//end TApp::Run

//---------------------------------------------------------------------------
// Deinitialize the application.
void TApp::Done(){

   if (SlimTree != NULL)
      delete SlimTree;
   if (PageManagerSlim != NULL)
      delete PageManagerSlim;

   if (Partition != NULL)
      delete Partition;
   if (PageManagerPartition != NULL)
      delete PageManagerPartition;

//   if (DBMTree != NULL)
//      delete DBMTree;
//   if (PageManagerDBM != NULL)
//      delete PageManagerDBM;

}//end TApp::Done

//---------------------------------------------------------------------------
// Creates a tree using the current Page Manager.
void TApp::CreateTree(){
   // Load tree
   //PageManagerSlim->Open("mytreeSlim.dat");
   //PageManagerPartition->Open("myPartition.dat");

   //slim-tree
   PageManagerSlim = new stDiskPageManager();
   PageManagerSlim->Create("mytreeSlim.dat", 1024);
   SlimTree = new mySlimTree(PageManagerSlim);
   SlimTree->SetSplitMethod(mySlimTree::smSPANNINGTREE);
   SlimTree->SetChooseMethod(mySlimTree::cmMINDIST);

   //partition-hash
   PageManagerPartition = new stDiskPageManager();
   PageManagerPartition->Create("myPartition.dat", 1024);
   Partition = new myPartitionHash(PageManagerPartition);
   Partition->SetChooseMethod(myPartitionHash::cmRANDOM);
//   Partition->SetChooseMethod(myPartitionHash::cmDIFFER);
//   Partition->SetChooseMethod(myPartitionHash::cmVARIANCE);

//   PageManagerDBM = new stDiskPageManager();
//   PageManagerDBM->Create("mytreeDBM.dat", 1024);
//   DBMTree = new myDBMTree(PageManagerDBM);
//   DBMTree->SetSplitMethod(myDBMTree::smMIN_MAX);
//   DBMTree->SetChooseMethod(myDBMTree::cmMINDISTWITHCOVER);
//   DBMTree->SetMinOccupation(0.5);

}//end TApp::CreateTree

//---------------------------------------------------------------------------
// Loads the tree from file with a set of cities.
void TApp::LoadTree(char *filename){
  FILE *f;
  int cont, contObj;
  char buffer[100], city[100];
  double dlat, dlong;
  char *ptr;

  if ((f = fopen(filename, "r")) == NULL){
     fprintf(stderr, "Cannot open input file.\n");
  }
  else{
     contObj = 0;
     cont = 0;
     fgets(buffer, 100, f);
     do{
         switch(cont){

           case 0: //city = new char[strlen(buffer)];
                   strcpy(city, buffer);
                   ptr = strchr(city, '\n');
                   *ptr = '\0';
                   cont++;
                   break;

           case 1: dlat = atof(buffer);
                   cont++;
                   break;

           case 2:
                  cout << "Adding " << contObj << " | ";
                   dlong = atof(buffer);
                   AddToTree(city, dlat, dlong);
                   contObj++;
                   cont = 0;
         } // end switch

         fgets(buffer, 100, f);
     }while (!feof(f)); // end while

     fclose(f);

  }// end else
}//end TApp::LoadTree(char *filename)

//---------------------------------------------------------------------------
// Performs a query and outputs its results.
void TApp::PerformQuery(){
//   double distance_count;
   myResult * resultSlim;
   TCity * sample;

   char ref_cidade[40];
   double ref_lat, ref_lon;
   int ref_nearest = 15;
   cout << endl << "City id: ";
   cin >> ref_cidade;


   if(ref_cidade[0]=='/')
             sample = new TCity("Dourado", -22.1, -48.31);
   else
        {
        cout << "City Latitude: ";
        cin >> ref_lat;
        cout << "City Longitude: ";
        cin >> ref_lon;
        cout << "Nearest k: ";
        cin >> ref_nearest;
        sample = new TCity(ref_cidade, ref_lat, ref_lon);
        }

   // Perform a query...
   SlimTree->GetMetricEvaluator()->ResetStatistics();

   resultSlim = SlimTree->NearestQuery(sample, ref_nearest, true);

   // Print query result.
   PrintResults(resultSlim);
   cout << "Number of distance-calculators:" << SlimTree->GetMetricEvaluator()->GetDistanceCount()<< "\n";
   cout << "Height = " << SlimTree->GetHeight();

   // Clean home
   delete resultSlim;
   delete sample;
}//end TApp::PerformQuery

//---------------------------------------------------------------------------
// Prints the result set to the standard output.
void TApp::PrintResults(myResult * result){
   int w;
   TCity * tmp;

   cout << "Range query with radius " << result->GetRadius() << ".\n";
   cout << "The sample object was " << *(result->GetSample()) << ".\n";

   if (result->GetNumOfEntries() == 0){
      cout << "No results!\n";
   }else{
      cout << "  " << result->GetNumOfEntries() << " results:\n";
      for (w = 0; w < result->GetNumOfEntries(); w++){
         tmp = (TCity *)(*result)[w].GetObject();
         cout << "Dist= " << (*result)[w].GetDistance() <<" ";
         cout << *tmp << "\n";
      }//end for
   }//end if

}//end TApp::PrintResults
