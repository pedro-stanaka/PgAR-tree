//---------------------------------------------------------------------------
// Implementation of the application.
//
// Author: Author: Enzo Seraphim (seraphim@icmc.sc.usp.br)
// Copyright (c) 2004 GBDI-ICMC-USP
//---------------------------------------------------------------------------
#include <iostream>
#include <fstream>
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
   // Lets load the tree with a lot values.
   LoadTree("city.txt", "cityJoin.txt");//manual (para criar o arquivo)

   //Queries
   PerformRangeJoinDummy();
   PerformRangeJoinSeq();
   PerformRangeJoinSlim();
//   PerformRangeJoinPartition();
   PerformNearestJoinDummy();
   PerformNearestJoinSeq();
   PerformNearestJoinSlim();
//   PerformNearestJoinPartition();
}//end TApp::Run

//---------------------------------------------------------------------------
void TApp::ResetHeuristics(){
   // Initiating counters
   DummyTree->GetMetricEvaluator()->ResetStatistics();
   DummyTreeJoin->GetMetricEvaluator()->ResetStatistics();
   SeqTree->GetMetricEvaluator()->ResetStatistics();
   SeqTreeJoin->GetMetricEvaluator()->ResetStatistics();
   SlimTree->GetMetricEvaluator()->ResetStatistics();
   SlimTreeJoin->GetMetricEvaluator()->ResetStatistics();
   Partition->GetMetricEvaluator()->ResetStatistics();
   PartitionJoin->GetMetricEvaluator()->ResetStatistics();
}//end TApp::ResetHeuristic

//---------------------------------------------------------------------------
void TApp::PrintHeuristics(bool printJoin){
   if(printJoin){
      cout << endl;
      cout << "[DUMMY] Number of distance-calculators:" <<
         DummyTree->GetMetricEvaluator()->GetDistanceCount()<< endl;
      cout << "[DUMMY] Height = " << DummyTree->GetHeight() << endl;
      cout << "[DUMMY] Node = " << DummyTree->GetNodeCount() << endl;
   }//end if
   cout << "[DUMMYJOIN] Number of distance-calculators:" <<
      DummyTreeJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[DUMMYJOIN] Height = " << DummyTreeJoin->GetHeight() << endl;
   cout << "[DUMMYJOIN] Node = " << DummyTreeJoin->GetNodeCount() << endl;

   if(printJoin){
      cout << "[SEQ] Number of distance-calculators:" <<
         SeqTree->GetMetricEvaluator()->GetDistanceCount()<< endl;
      cout << "[SEQ] Height = " << SeqTree->GetHeight() << endl;
      cout << "[SEQ] Node = " << SeqTree->GetNodeCount() << endl;
   }//end if
   cout << "[SEQJOIN] Number of distance-calculators:" <<
      SeqTreeJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[SEQJOIN] Height = " << SeqTreeJoin->GetHeight() << endl;
   cout << "[SEQJOIN] Node = " << SeqTreeJoin->GetNodeCount() << endl;

   if(printJoin){
      cout << "[SLIM] Number of distance-calculators:" <<
         SlimTree->GetMetricEvaluator()->GetDistanceCount()<< endl;
      cout << "[SLIM] Height = " << SlimTree->GetHeight() << endl;
      cout << "[SLIM] Node = " << SlimTree->GetNodeCount() << endl;
   }//end if
   cout << "[SLIMJOIN] Number of distance-calculators:" <<
      SlimTreeJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[SLIMJOIN] Height = " << SlimTreeJoin->GetHeight() << endl;
   cout << "[SLIMJOIN] Node = " << SlimTreeJoin->GetNodeCount() << endl;
/*
   if(printJoin){
      cout << "[PART] Number of distance-calculators:" <<
         Partition->GetMetricEvaluator()->GetDistanceCount()<< endl;
      cout << "[PART] Height = " << Partition->GetHeight() << endl;
      cout << "[PART] IndexBucket = " << Partition->GetIndexBucketCount() << endl;
      cout << "[PART] LeafBucket = " << Partition->GetLeafBucketCount() << endl;
   }//end if
   cout << "[PARTJOIN] Number of distance-calculators:" <<
      PartitionJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[PARTJOIN] Height = " << PartitionJoin->GetHeight() << endl;
   cout << "[PARTJOIN] IndexBucket = " << PartitionJoin->GetIndexBucketCount() << endl;
   cout << "[PARTJOIN] LeafBucket = " << PartitionJoin->GetLeafBucketCount() << endl;
*/
}//end TApp::PrintHeuristic

//---------------------------------------------------------------------------
// Deinitialize the application.
void TApp::Done(){
   if (DummyTree != NULL)
      delete DummyTree;
   if (PageManagerDummy != NULL)
      delete PageManagerDummy;
   if (DummyTreeJoin != NULL)
      delete DummyTreeJoin;
   if (PageManagerDummyJoin != NULL)
      delete PageManagerDummyJoin;

   if (SeqTree != NULL)
      delete SeqTree;
   if (PageManagerSeq != NULL)
      delete PageManagerSeq;
   if (SeqTreeJoin != NULL)
      delete SeqTreeJoin;
   if (PageManagerSeqJoin != NULL)
      delete PageManagerSeqJoin;

   if (SlimTree != NULL)
      delete SlimTree;
   if (PageManagerSlim != NULL)
      delete PageManagerSlim;
   if (SlimTreeJoin != NULL)
      delete SlimTreeJoin;
   if (PageManagerSlimJoin != NULL)
      delete PageManagerSlimJoin;

   if (Partition != NULL)
      delete Partition;
   if (PageManagerPartition != NULL)
      delete PageManagerPartition;
   if (PartitionJoin != NULL)
      delete PartitionJoin;
   if (PageManagerPartitionJoin != NULL)
      delete PageManagerPartitionJoin;
}//end TApp::Done

//---------------------------------------------------------------------------
// Creates a tree using the current Page Manager.
void TApp::CreateTree(){
   // Load tree
   //PageManagerSlim->Open("mytreeSlim.dat");
   //PageManagerPartition->Open("myPartition.dat");

   //dummy-tree
   PageManagerDummy = new stDiskPageManager();
   PageManagerDummy->Create("Dummy.dat", 1024);
   DummyTree = new myDummyTree(PageManagerDummy);

   //dummy-tree Join
   PageManagerDummyJoin = new stDiskPageManager();
   PageManagerDummyJoin->Create("DummyJoin.dat", 1024);
   DummyTreeJoin = new myDummyTree(PageManagerDummyJoin);

   //seq-tree
   PageManagerSeq = new stDiskPageManager();
   PageManagerSeq->Create("Seq.dat", 1024);
   SeqTree = new mySeqTree(PageManagerSeq);
   SeqTree->SetSplitMethod(mySeqTree::smSPANNINGTREE);
   SeqTree->SetChooseMethod(mySeqTree::cmMINDIST);

   //seq-tree Join
   PageManagerSeqJoin = new stDiskPageManager();
   PageManagerSeqJoin->Create("SeqJoin.dat", 1024);
   SeqTreeJoin = new mySeqTree(PageManagerSeqJoin);
   SeqTreeJoin->SetSplitMethod(mySeqTree::smSPANNINGTREE);
   SeqTreeJoin->SetChooseMethod(mySeqTree::cmMINDIST);

   //slim-tree
   PageManagerSlim = new stDiskPageManager();
   PageManagerSlim->Create("Slim.dat", 1024);
   SlimTree = new mySlimTree(PageManagerSlim);
   SlimTree->SetSplitMethod(mySlimTree::smSPANNINGTREE);
   SlimTree->SetChooseMethod(mySlimTree::cmMINDIST);

   //slim-tree Join
   PageManagerSlimJoin = new stDiskPageManager();
   PageManagerSlimJoin->Create("SlimJoin.dat", 1024);
   SlimTreeJoin = new mySlimTree(PageManagerSlimJoin);
   SlimTreeJoin->SetSplitMethod(mySlimTree::smSPANNINGTREE);
   SlimTreeJoin->SetChooseMethod(mySlimTree::cmMINDIST);

   //partition-hash
   PageManagerPartition = new stDiskPageManager();
   PageManagerPartition->Create("Part.dat", 1024);
   Partition = new myPartitionHash(PageManagerPartition);
   Partition->SetChooseMethod(myPartitionHash::cmVARIANCE);

   //partition-hash join
   PageManagerPartitionJoin = new stDiskPageManager();
   PageManagerPartitionJoin->Create("PartJoin.dat", 1024);
   PartitionJoin = new myPartitionHash(PageManagerPartitionJoin);
   PartitionJoin->SetChooseMethod(myPartitionHash::cmVARIANCE);

}//end TApp::CreateTree

//---------------------------------------------------------------------------
// Loads the tree from file with a set of cities.
void TApp::LoadTree(char *fileNameIndex, char *fileNameJoin){
   //reset heuristics
   ResetHeuristics();
   //input files
   ifstream fIndex(fileNameIndex);
   ifstream fJoin(fileNameJoin);
   if((fIndex.is_open()) && (fJoin.is_open())){
      char id[500];
      char cLat[10];
      char cLon[10];
      double lat;
      double lon;
      while (!fIndex.eof()){
         //read
         fIndex.getline(id, 500);
         if (!fIndex.eof()){
            fIndex.getline(cLat, 10);
            lat = atof(cLat);
            if (!fIndex.eof()){
               fIndex.getline(cLon, 10);
               lon = atof(cLon);
               AddToTree(id, lat, lon);
            }//end if
         }//end if
      }//end while
      fIndex.close();
      while (!fJoin.eof()){
         //read
         fJoin.getline(id, 500);
         if (!fJoin.eof()){
            fJoin.getline(cLat, 10);
            lat = atof(cLat);
            if (!fJoin.eof()){
               fJoin.getline(cLon, 10);
               lon = atof(cLon);
               AddToTreeJoin(id, lat, lon);
            }//end if
         }//end if
      }//end while
      fJoin.close();
   }//end if
   //Print Heuristic
   PrintHeuristics(true);
}//end TApp::LoadTree

//---------------------------------------------------------------------------
void TApp::PerformRangeJoinDummy(){
   //result
   myResult * res;
   //Dummy Range Join
   PageManagerDummy->ResetStatistics();
   PageManagerDummyJoin->ResetStatistics();
   DummyTree->GetMetricEvaluator()->ResetStatistics();
   DummyTreeJoin->GetMetricEvaluator()->ResetStatistics();
   res = DummyTree->RangeJoinQuery(DummyTreeJoin,10);
   delete res;
   cout << "[RJ:DUMMYxDUMMY] Number of distance-calculators:" <<
      DummyTree->GetMetricEvaluator()->GetDistanceCount() +
      DummyTreeJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[RJ:DUMMYxDUMMY] Number of Disk Accesses: " <<
      PageManagerDummy->GetReadCount() + PageManagerDummyJoin->GetReadCount() << endl;
   //Seq Range Join
   PageManagerSeq->ResetStatistics();
   PageManagerDummyJoin->ResetStatistics();
   SeqTree->GetMetricEvaluator()->ResetStatistics();
   DummyTreeJoin->GetMetricEvaluator()->ResetStatistics();
   res = SeqTree->RangeJoinQuery(DummyTreeJoin,10);
   delete res;
   cout << "[RJ:SEQxDUMMY] Number of distance-calculators:" <<
      SeqTree->GetMetricEvaluator()->GetDistanceCount() +
      DummyTreeJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[RJ:SEQxDUMMY] Number of Disk Accesses: " <<
      PageManagerSeq->GetReadCount() + PageManagerDummyJoin->GetReadCount() << endl;
   //Slim Range Join
   PageManagerSlim->ResetStatistics();
   PageManagerDummyJoin->ResetStatistics();
   SlimTree->GetMetricEvaluator()->ResetStatistics();
   DummyTreeJoin->GetMetricEvaluator()->ResetStatistics();
   res = SlimTree->RangeJoinQuery(DummyTreeJoin,10);
   delete res;
   cout << "[RJ:SLIMxDUMMY] Number of distance-calculators:" <<
      SlimTree->GetMetricEvaluator()->GetDistanceCount() +
      DummyTreeJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[RJ:SLIMxDUMMY] Number of Disk Accesses: " <<
      PageManagerSlim->GetReadCount() + PageManagerDummyJoin->GetReadCount() << endl;
/*
   //Partition Range Join
   PageManagerPartition->ResetStatistics();
   PageManagerDummyJoin->ResetStatistics();
   Partition->GetMetricEvaluator()->ResetStatistics();
   DummyTreeJoin->GetMetricEvaluator()->ResetStatistics();
   res = Partition->RangeJoinQuery(DummyTreeJoin,10);
   delete res;
   cout << "[RJ:PARTxDUMMY] Number of distance-calculators:" <<
      Partition->GetMetricEvaluator()->GetDistanceCount() +
      DummyTreeJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[RJ:SEQxDUMMY] Number of Disk Accesses: " <<
      PageManagerPartition->GetReadCount() + PageManagerDummyJoin->GetReadCount() << endl;
*/
}//end TApp::PerformRangeJoin

//---------------------------------------------------------------------------
void TApp::PerformRangeJoinSeq(){
   //result
   myResult * res;
   //Dummy Range Join
   PageManagerDummy->ResetStatistics();
   PageManagerSeqJoin->ResetStatistics();
   DummyTree->GetMetricEvaluator()->ResetStatistics();
   SeqTreeJoin->GetMetricEvaluator()->ResetStatistics();
   res = DummyTree->RangeJoinQuery(SeqTreeJoin,10);
   delete res;
   cout << "[RJ:DUMMYxSEQ] Number of distance-calculators:" <<
      DummyTree->GetMetricEvaluator()->GetDistanceCount() +
      SeqTreeJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[RJ:DUMMYxSEQ] Number of Disk Accesses: " <<
      PageManagerDummy->GetReadCount() + PageManagerSeqJoin->GetReadCount() << endl;
   //Seq Range Join
   PageManagerSeq->ResetStatistics();
   PageManagerSeqJoin->ResetStatistics();
   SeqTree->GetMetricEvaluator()->ResetStatistics();
   SeqTreeJoin->GetMetricEvaluator()->ResetStatistics();
   res = SeqTree->RangeJoinQuery(SeqTreeJoin,10);
   delete res;
   cout << "[RJ:SEQxSEQ] Number of distance-calculators:" <<
      SeqTree->GetMetricEvaluator()->GetDistanceCount() +
      SeqTreeJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[RJ:SEQxSEQ] Number of Disk Accesses: " <<
      PageManagerSeq->GetReadCount() + PageManagerSeqJoin->GetReadCount() << endl;
   //Slim Range Join
   PageManagerSlim->ResetStatistics();
   PageManagerSeqJoin->ResetStatistics();
   SlimTree->GetMetricEvaluator()->ResetStatistics();
   SeqTreeJoin->GetMetricEvaluator()->ResetStatistics();
   res = SlimTree->RangeJoinQuery(SeqTreeJoin,10);
   delete res;
   cout << "[RJ:SLIMxSEQ] Number of distance-calculators:" <<
      SlimTree->GetMetricEvaluator()->GetDistanceCount() +
      SeqTreeJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[RJ:SLIMxSEQ] Number of Disk Accesses: " <<
      PageManagerSlim->GetReadCount() + PageManagerSeqJoin->GetReadCount() << endl;
/*
   //Partition Range Join
   PageManagerPartition->ResetStatistics();
   PageManagerSeqJoin->ResetStatistics();
   Partition->GetMetricEvaluator()->ResetStatistics();
   SeqTreeJoin->GetMetricEvaluator()->ResetStatistics();
   res = Partition->RangeJoinQuery(SeqTreeJoin,10);
   delete res;
   cout << "[RJ:PARTxSEQ] Number of distance-calculators:" <<
      Partition->GetMetricEvaluator()->GetDistanceCount() +
      SeqTreeJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[RJ:PARTxSEQ] Number of Disk Accesses: " <<
      PageManagerPartition->GetReadCount() + PageManagerSeqJoin->GetReadCount() << endl;
*/
}//end TApp::PerformRangeJoin

//---------------------------------------------------------------------------
void TApp::PerformRangeJoinSlim(){
   //result
   myResult * res;
   //Dummy Range Join
   PageManagerDummy->ResetStatistics();
   PageManagerSlimJoin->ResetStatistics();
   DummyTree->GetMetricEvaluator()->ResetStatistics();
   SlimTreeJoin->GetMetricEvaluator()->ResetStatistics();
   res = DummyTree->RangeJoinQuery(SlimTreeJoin,10);
   delete res;
   cout << "[RJ:DUMMYxSLIM] Number of distance-calculators:" <<
      DummyTree->GetMetricEvaluator()->GetDistanceCount() +
      SlimTreeJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[RJ:DUMMYxSLIM] Number of Disk Accesses: " <<
      PageManagerDummy->GetReadCount() + PageManagerSlimJoin->GetReadCount() << endl;
   //Seq Range Join
   PageManagerSeq->ResetStatistics();
   PageManagerSlimJoin->ResetStatistics();
   SeqTree->GetMetricEvaluator()->ResetStatistics();
   SlimTreeJoin->GetMetricEvaluator()->ResetStatistics();
   res = SeqTree->RangeJoinQuery(SlimTreeJoin,10);
   delete res;
   cout << "[RJ:SEQxSLIM] Number of distance-calculators:" <<
      SeqTree->GetMetricEvaluator()->GetDistanceCount() +
      SlimTreeJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[RJ:SEQxSLIM] Number of Disk Accesses: " <<
      PageManagerSeq->GetReadCount() + PageManagerSlimJoin->GetReadCount() << endl;
   //Slim Range Join
   PageManagerSlim->ResetStatistics();
   PageManagerSlimJoin->ResetStatistics();
   SlimTree->GetMetricEvaluator()->ResetStatistics();
   SlimTreeJoin->GetMetricEvaluator()->ResetStatistics();
   res = SlimTree->RangeJoinQuery(SlimTreeJoin,10);
   delete res;
   cout << "[RJ:SLIMxSLIM] Number of distance-calculators:" <<
      SlimTree->GetMetricEvaluator()->GetDistanceCount() +
      SlimTreeJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[RJ:SLIMSLIM] Number of Disk Accesses: " <<
      PageManagerSlim->GetReadCount() + PageManagerSlimJoin->GetReadCount() << endl;
/*
   //Partition Range Join
   PageManagerPartition->ResetStatistics();
   PageManagerSlimJoin->ResetStatistics();
   Partition->GetMetricEvaluator()->ResetStatistics();
   SlimTreeJoin->GetMetricEvaluator()->ResetStatistics();
   res = Partition->RangeJoinQuery(SlimTreeJoin,10);
   delete res;
   cout << "[RJ:PARTxSLIM] Number of distance-calculators:" <<
      Partition->GetMetricEvaluator()->GetDistanceCount() +
      SlimTreeJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[RJ:PARTxSLIM] Number of Disk Accesses: " <<
      PageManagerPartition->GetReadCount() + PageManagerSlimJoin->GetReadCount() << endl;
*/
}//end TApp::PerformRangeJoin

//---------------------------------------------------------------------------
void TApp::PerformRangeJoinPartition(){
   //result
   myResult * res;
   //Dummy Range Join
   PageManagerDummy->ResetStatistics();
   PageManagerPartitionJoin->ResetStatistics();
   DummyTree->GetMetricEvaluator()->ResetStatistics();
   PartitionJoin->GetMetricEvaluator()->ResetStatistics();
   res = DummyTree->RangeJoinQuery(PartitionJoin,10);
   delete res;
   cout << "[RJ:DUMMYxPART] Number of distance-calculators:" <<
      DummyTree->GetMetricEvaluator()->GetDistanceCount() +
      PartitionJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[RJ:DUMMYxPART] Number of Disk Accesses: " <<
      PageManagerDummy->GetReadCount() + PageManagerPartitionJoin->GetReadCount() << endl;
   //Seq Range Join
   PageManagerSeq->ResetStatistics();
   PageManagerPartitionJoin->ResetStatistics();
   SeqTree->GetMetricEvaluator()->ResetStatistics();
   PartitionJoin->GetMetricEvaluator()->ResetStatistics();
   res = SeqTree->RangeJoinQuery(PartitionJoin,10);
   delete res;
   cout << "[RJ:SEQxPART] Number of distance-calculators:" <<
      SeqTree->GetMetricEvaluator()->GetDistanceCount() +
      PartitionJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[RJ:SEQxPART] Number of Disk Accesses: " <<
      PageManagerSeq->GetReadCount() + PageManagerPartitionJoin->GetReadCount() << endl;
   //Slim Range Join
   PageManagerSlim->ResetStatistics();
   PageManagerPartitionJoin->ResetStatistics();
   SlimTree->GetMetricEvaluator()->ResetStatistics();
   PartitionJoin->GetMetricEvaluator()->ResetStatistics();
   res = SlimTree->RangeJoinQuery(PartitionJoin,10);
   delete res;
   cout << "[RJ:SLIMxPART] Number of distance-calculators:" <<
      SlimTree->GetMetricEvaluator()->GetDistanceCount() +
      PartitionJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[RJ:SLIMxPART] Number of Disk Accesses: " <<
      PageManagerSlim->GetReadCount() + PageManagerPartitionJoin->GetReadCount() << endl;
   //Partition Range Join
   PageManagerPartition->ResetStatistics();
   PageManagerPartitionJoin->ResetStatistics();
   Partition->GetMetricEvaluator()->ResetStatistics();
   PartitionJoin->GetMetricEvaluator()->ResetStatistics();
   res = Partition->RangeJoinQuery(PartitionJoin,10);
   delete res;
   cout << "[RJ:PARTxPART] Number of distance-calculators:" <<
      PartitionJoin->GetMetricEvaluator()->GetDistanceCount() +
      PartitionJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[RJ:PARTxPART] Number of Disk Accesses: " <<
      PageManagerPartition->GetReadCount() + PageManagerPartitionJoin->GetReadCount() << endl;
}//end TApp::PerformRangeJoin

//---------------------------------------------------------------------------
void TApp::PerformNearestJoinDummy(){
   //result
   myResult * res;
   //Dummy Nearest Join
   PageManagerDummy->ResetStatistics();
   PageManagerDummyJoin->ResetStatistics();
   DummyTree->GetMetricEvaluator()->ResetStatistics();
   DummyTreeJoin->GetMetricEvaluator()->ResetStatistics();
   res = DummyTree->NearestJoinQuery(DummyTreeJoin,10);
   delete res;
   cout << "[NJ:DUMMYxDUMMY] Number of distance-calculators:" <<
      DummyTree->GetMetricEvaluator()->GetDistanceCount() +
      DummyTreeJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[NJ:DUMMYxDUMMY] Number of Disk Accesses: " <<
      PageManagerDummy->GetReadCount() + PageManagerDummyJoin->GetReadCount() << endl;
   //Seq Nearest Join
   PageManagerSeq->ResetStatistics();
   PageManagerDummyJoin->ResetStatistics();
   SeqTree->GetMetricEvaluator()->ResetStatistics();
   DummyTreeJoin->GetMetricEvaluator()->ResetStatistics();
   res = SeqTree->NearestJoinQuery(DummyTreeJoin,10);
   delete res;
   cout << "[NJ:SEQxDUMMY] Number of distance-calculators:" <<
      SeqTree->GetMetricEvaluator()->GetDistanceCount() +
      DummyTreeJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[NJ:SEQxDUMMY] Number of Disk Accesses: " <<
      PageManagerSeq->GetReadCount() + PageManagerDummyJoin->GetReadCount() << endl;
   //Slim Nearest Join
   PageManagerSlim->ResetStatistics();
   PageManagerDummyJoin->ResetStatistics();
   SlimTree->GetMetricEvaluator()->ResetStatistics();
   DummyTreeJoin->GetMetricEvaluator()->ResetStatistics();
   res = SlimTree->NearestJoinQuery(DummyTreeJoin,10);
   delete res;
   cout << "[NJ:SLIMxDUMMY] Number of distance-calculators:" <<
      SlimTree->GetMetricEvaluator()->GetDistanceCount() +
      DummyTreeJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[NJ:SLIMxDUMMY] Number of Disk Accesses: " <<
      PageManagerSlim->GetReadCount() + PageManagerDummyJoin->GetReadCount() << endl;
/*
   //Partition Nearest Join
   PageManagerPartition->ResetStatistics();
   PageManagerDummyJoin->ResetStatistics();
   Partition->GetMetricEvaluator()->ResetStatistics();
   DummyTreeJoin->GetMetricEvaluator()->ResetStatistics();
   res = Partition->NearestJoinQuery(DummyTreeJoin,10);
   delete res;
   cout << "[NJ:PARTxDUMMY] Number of distance-calculators:" <<
      Partition->GetMetricEvaluator()->GetDistanceCount() +
      DummyTreeJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[NJ:SEQxDUMMY] Number of Disk Accesses: " <<
      PageManagerPartition->GetReadCount() + PageManagerDummyJoin->GetReadCount() << endl;
*/
}//end TApp::PerformNearestJoin

//---------------------------------------------------------------------------
void TApp::PerformNearestJoinSeq(){
   //result
   myResult * res;
   //Dummy Nearest Join
   PageManagerDummy->ResetStatistics();
   PageManagerSeqJoin->ResetStatistics();
   DummyTree->GetMetricEvaluator()->ResetStatistics();
   SeqTreeJoin->GetMetricEvaluator()->ResetStatistics();
   res = DummyTree->NearestJoinQuery(SeqTreeJoin,10);
   delete res;
   cout << "[NJ:DUMMYxSEQ] Number of distance-calculators:" <<
      DummyTree->GetMetricEvaluator()->GetDistanceCount() +
      SeqTreeJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[NJ:DUMMYxSEQ] Number of Disk Accesses: " <<
      PageManagerDummy->GetReadCount() + PageManagerSeqJoin->GetReadCount() << endl;
   //Seq Nearest Join
   PageManagerSeq->ResetStatistics();
   PageManagerSeqJoin->ResetStatistics();
   SeqTree->GetMetricEvaluator()->ResetStatistics();
   SeqTreeJoin->GetMetricEvaluator()->ResetStatistics();
   res = SeqTree->NearestJoinQuery(SeqTreeJoin,10);
   delete res;
   cout << "[NJ:SEQxSEQ] Number of distance-calculators:" <<
      SeqTree->GetMetricEvaluator()->GetDistanceCount() +
      SeqTreeJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[NJ:SEQxSEQ] Number of Disk Accesses: " <<
      PageManagerSeq->GetReadCount() + PageManagerSeqJoin->GetReadCount() << endl;
   //Slim Nearest Join
   PageManagerSlim->ResetStatistics();
   PageManagerSeqJoin->ResetStatistics();
   SlimTree->GetMetricEvaluator()->ResetStatistics();
   SeqTreeJoin->GetMetricEvaluator()->ResetStatistics();
   res = SlimTree->NearestJoinQuery(SeqTreeJoin,10);
   delete res;
   cout << "[NJ:SLIMxSEQ] Number of distance-calculators:" <<
      SlimTree->GetMetricEvaluator()->GetDistanceCount() +
      SeqTreeJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[NJ:SLIMxSEQ] Number of Disk Accesses: " <<
      PageManagerSlim->GetReadCount() + PageManagerSeqJoin->GetReadCount() << endl;
/*
   //Partition Nearest Join
   PageManagerPartition->ResetStatistics();
   PageManagerSeqJoin->ResetStatistics();
   Partition->GetMetricEvaluator()->ResetStatistics();
   SeqTreeJoin->GetMetricEvaluator()->ResetStatistics();
   res = Partition->NearestJoinQuery(SeqTreeJoin,10);
   delete res;
   cout << "[NJ:PARTxSEQ] Number of distance-calculators:" <<
      Partition->GetMetricEvaluator()->GetDistanceCount() +
      SeqTreeJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[NJ:PARTxSEQ] Number of Disk Accesses: " <<
      PageManagerPartition->GetReadCount() + PageManagerSeqJoin->GetReadCount() << endl;
*/
}//end TApp::PerformNearestJoin

//---------------------------------------------------------------------------
void TApp::PerformNearestJoinSlim(){
   //result
   myResult * res;
   //Dummy Nearest Join
   PageManagerDummy->ResetStatistics();
   PageManagerSlimJoin->ResetStatistics();
   DummyTree->GetMetricEvaluator()->ResetStatistics();
   SlimTreeJoin->GetMetricEvaluator()->ResetStatistics();
   res = DummyTree->NearestJoinQuery(SlimTreeJoin,10);
   delete res;
   cout << "[NJ:DUMMYxSLIM] Number of distance-calculators:" <<
      DummyTree->GetMetricEvaluator()->GetDistanceCount() +
      SlimTreeJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[NJ:DUMMYxSLIM] Number of Disk Accesses: " <<
      PageManagerDummy->GetReadCount() + PageManagerSlimJoin->GetReadCount() << endl;
   //Seq Nearest Join
   PageManagerSeq->ResetStatistics();
   PageManagerSlimJoin->ResetStatistics();
   SeqTree->GetMetricEvaluator()->ResetStatistics();
   SlimTreeJoin->GetMetricEvaluator()->ResetStatistics();
   res = SeqTree->NearestJoinQuery(SlimTreeJoin,10);
   delete res;
   cout << "[NJ:SEQxSLIM] Number of distance-calculators:" <<
      SeqTree->GetMetricEvaluator()->GetDistanceCount() +
      SlimTreeJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[NJ:SEQxSLIM] Number of Disk Accesses: " <<
      PageManagerSeq->GetReadCount() + PageManagerSlimJoin->GetReadCount() << endl;
   //Slim Nearest Join
   PageManagerSlim->ResetStatistics();
   PageManagerSlimJoin->ResetStatistics();
   SlimTree->GetMetricEvaluator()->ResetStatistics();
   SlimTreeJoin->GetMetricEvaluator()->ResetStatistics();
   res = SlimTree->NearestJoinQuery(SlimTreeJoin,10);
   delete res;
   cout << "[NJ:SLIMxSLIM] Number of distance-calculators:" <<
      SlimTree->GetMetricEvaluator()->GetDistanceCount() +
      SlimTreeJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[NJ:SLIMSLIM] Number of Disk Accesses: " <<
      PageManagerSlim->GetReadCount() + PageManagerSlimJoin->GetReadCount() << endl;
/*
   //Partition Nearest Join
   PageManagerPartition->ResetStatistics();
   PageManagerSlimJoin->ResetStatistics();
   Partition->GetMetricEvaluator()->ResetStatistics();
   SlimTreeJoin->GetMetricEvaluator()->ResetStatistics();
   res = Partition->NearestJoinQuery(SlimTreeJoin,10);
   delete res;
   cout << "[NJ:PARTxSLIM] Number of distance-calculators:" <<
      Partition->GetMetricEvaluator()->GetDistanceCount() +
      SlimTreeJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[NJ:PARTxSLIM] Number of Disk Accesses: " <<
      PageManagerPartition->GetReadCount() + PageManagerSlimJoin->GetReadCount() << endl;
*/
}//end TApp::PerformNearestJoin

//---------------------------------------------------------------------------
void TApp::PerformNearestJoinPartition(){
   //result
   myResult * res;
   //Dummy Nearest Join
   PageManagerDummy->ResetStatistics();
   PageManagerPartitionJoin->ResetStatistics();
   DummyTree->GetMetricEvaluator()->ResetStatistics();
   PartitionJoin->GetMetricEvaluator()->ResetStatistics();
   res = DummyTree->NearestJoinQuery(PartitionJoin,10);
   delete res;
   cout << "[NJ:DUMMYxPART] Number of distance-calculators:" <<
      DummyTree->GetMetricEvaluator()->GetDistanceCount() +
      PartitionJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[NJ:DUMMYxPART] Number of Disk Accesses: " <<
      PageManagerDummy->GetReadCount() + PageManagerPartitionJoin->GetReadCount() << endl;
   //Seq Nearest Join
   PageManagerSeq->ResetStatistics();
   PageManagerPartitionJoin->ResetStatistics();
   SeqTree->GetMetricEvaluator()->ResetStatistics();
   PartitionJoin->GetMetricEvaluator()->ResetStatistics();
   res = SeqTree->NearestJoinQuery(PartitionJoin,10);
   delete res;
   cout << "[NJ:SEQxPART] Number of distance-calculators:" <<
      SeqTree->GetMetricEvaluator()->GetDistanceCount() +
      PartitionJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[NJ:SEQxPART] Number of Disk Accesses: " <<
      PageManagerSeq->GetReadCount() + PageManagerPartitionJoin->GetReadCount() << endl;
   //Slim Nearest Join
   PageManagerSlim->ResetStatistics();
   PageManagerPartitionJoin->ResetStatistics();
   SlimTree->GetMetricEvaluator()->ResetStatistics();
   PartitionJoin->GetMetricEvaluator()->ResetStatistics();
   res = SlimTree->NearestJoinQuery(PartitionJoin,10);
   delete res;
   cout << "[NJ:SLIMxPART] Number of distance-calculators:" <<
      SlimTree->GetMetricEvaluator()->GetDistanceCount() +
      PartitionJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[NJ:SLIMxPART] Number of Disk Accesses: " <<
      PageManagerSlim->GetReadCount() + PageManagerPartitionJoin->GetReadCount() << endl;
   //Partition Nearest Join
   PageManagerPartition->ResetStatistics();
   PageManagerPartitionJoin->ResetStatistics();
   Partition->GetMetricEvaluator()->ResetStatistics();
   PartitionJoin->GetMetricEvaluator()->ResetStatistics();
   res = Partition->NearestJoinQuery(PartitionJoin,10);
   delete res;
   cout << "[NJ:PARTxPART] Number of distance-calculators:" <<
      PartitionJoin->GetMetricEvaluator()->GetDistanceCount() +
      PartitionJoin->GetMetricEvaluator()->GetDistanceCount()<< endl;
   cout << "[NJ:PARTxPART] Number of Disk Accesses: " <<
      PageManagerPartition->GetReadCount() + PageManagerPartitionJoin->GetReadCount() << endl;
}//end TApp::PerformNearestJoin

//---------------------------------------------------------------------------
// Prints the result set to the standard output.
void TApp::PrintResults(myResult * result){
   unsigned w;
   TCity * tmp;

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

