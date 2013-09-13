//---------------------------------------------------------------------------
// app.cpp - Implementation of the application.
//
// To change the behavior of this application, comment and uncomment lines at
// TApp::Init() and TApp::Query().
//
// Author: Fabio Jun Takada Chino (chino@icmc.sc.usp.br)
// Copyright (c) 2002 GBDI-ICMC-USP
//---------------------------------------------------------------------------
#include <iostream>
#pragma hdrstop
#include "app.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
// Class TApp
//---------------------------------------------------------------------------
/**
* Initializes the application.
*/
void TApp::Init(){

   // Uncoment one of the page manager creation procedures. Do not uncoment both
   // of them.
   //CreateDiskPageManager(); // To create it in disk
   CreateMemoryPageManager(); // To create it in memory

   // Creates the tree
   CreateTree();
}//end TApp::Init

/**
* Runs the application.
*/
void TApp::Run(){


   //Choose the nearest method
   ((myMMTree *)MetricTree)->SetNearestMethod(1);

   //Choose the insertion method
   ((myMMTree *)MetricTree)->SetInsertionMethod(1);

   //Reset the Distance Counter
   MetricTree->GetMetricEvaluator()->ResetStatistics();

   // Load the tree with a few values.
   LoadTree();

   //Show many distances used to create the tree
   printf("Calculos de Distancia para a criacao da arvore = %i",MetricTree->GetMetricEvaluator()->GetDistanceCount());

   cin.get();

   int min, max;
   ((myMMTree *)MetricTree)->GetHeights(min,max);
   printf("Altura Minima = %d e altura Maxima = %d\n", min, max);
   printf("Numero de Nos = %d\n\n", ((myMMTree *)MetricTree)->GetNumberOfNodes());



   MetricTree->GetMetricEvaluator()->ResetStatistics();

   // Query the tree
   printf("Performing queries..\n");

   PerformQuery();

   printf("\nCalculos de Distancia para a consulta = %i\n",MetricTree->GetMetricEvaluator()->GetDistanceCount());
   cin.get();

}//end TApp::Run

/**
* Deinitialize the application.
*/
void TApp::Done(){

   if (this->MetricTree != NULL){
      delete this->MetricTree;
   }//end if

   if (this->PageManager != NULL){
      delete this->PageManager;
   }//end if
}//end TApp::Done

/**
* Creates a memory page manager. It must be called before CreateTree().
*/
void TApp::CreateMemoryPageManager(){

   // 1024 bytes per page
   PageManager = new stMemoryPageManager(2048);
}//end TApp::CreateMemoryPageManager()

/**
* Creates a disk page manager. It must be called before CreateTree().
*/
void TApp::CreateDiskPageManager(){
   stDiskPageManager * dman;

   dman = new stDiskPageManager();

   // Setup
   // Filename is mytree.dat
   // 1024 bytes per page
   // HeaderSize is 16 bytes (a value large enough to hold the reader -
   // This parameter will fall in the future)
   dman->Create("mytree.dat", 1024, 16);

   PageManager = dman;
}//end TApp::CreateDiskPageManager

/**
* Creates a tree using the current Page Manager. Call
* CreateMemoryPageManager() or CreateDiskPageManager() before it to create
* a PageManager.
*/
void TApp::CreateTree(){

   MetricTree = new myMMTree(PageManager);
}//end TApp::CreateTree

/**
* Loads the tree with a set of cities.
*/
void TApp::LoadTree(){
   AddToTree("Guatapará", -48.03, -21.49);
   AddToTree("Luís Antônio", -47.7, -21.55);
   AddToTree("Motuca", -48.15, -21.5);
   AddToTree("Rincão", -48.07, -21.58);
   AddToTree("Matão", -48.36, -21.6);
   AddToTree("Araraquara", -48.17, -21.79);
   AddToTree("Santa Rita do Passa Quatro", -47.47, -21.71);
   AddToTree("São Carlos", -47.89, -22.01);
   AddToTree("Santa Lúcia", -48.08, -21.68);
   AddToTree("Américo Brasiliense", -48.1, -21.72);
   AddToTree("Gavião Peixoto", -48.49, -21.83);
   AddToTree("Descalvado", -47.61, -21.9);
   AddToTree("Ibaté", -47.99, -21.95);
   AddToTree("Boa Esperança do Sul", -48.39, -21.99);
   AddToTree("Ribeirão Bonito", -48.17, -22.06);
   AddToTree("Bocaina", -48.51, -22.13);
   AddToTree("Trabiju", -48.33, -22.04);
   AddToTree("Analândia", -47.66, -22.12);
   AddToTree("Dourado", -48.31, -22.1);
   AddToTree("Brotas", -48.12, -22.28);
   AddToTree("Jaú", -48.55, -22.29);
   AddToTree("Itirapina", -47.82, -22.25);
   AddToTree("Jarinu", -46.72, -23.1);
   AddToTree("Atibaia", -46.55, -23.11);
   AddToTree("Itupeva", -47.05, -23.15);
   AddToTree("Jundiaí", -46.88, -23.18);
   AddToTree("Nazaré Paulista", -46.39, -23.18);
   AddToTree("Bom Jesus dos Perdões", -46.46, -23.13);
   AddToTree("Campo Limpo Paulista", -46.78, -23.2);
   AddToTree("Várzea Paulista", -46.82, -23.21);
   AddToTree("Santa Isabel", -46.22, -23.31);
   AddToTree("Cabreúva", -47.13, -23.3);
   AddToTree("Mairiporã", -46.58, -23.31);
   AddToTree("Francisco Morato", -46.74, -23.28);
   AddToTree("Franco da Rocha", -46.72, -23.32);
   AddToTree("Guarulhos", -46.53, -23.46);
   AddToTree("Cajamar", -46.87, -23.35);
   AddToTree("Pirapora do Bom Jesus", -47, -23.39);
   AddToTree("Arujá", -46.32, -23.39);
   AddToTree("Caieiras", -46.74, -23.36);
   AddToTree("Araçariguama", -47.06, -23.43);
   AddToTree("São Paulo", -46.63, -23.54);
   AddToTree("Santana de Parnaíba", -46.91, -23.44);
   AddToTree("São Roque", -47.13, -23.52);
   AddToTree("Itaquaquecetuba", -46.34, -23.48);
   AddToTree("Osasco", -46.79, -23.53);
   AddToTree("Barueri", -46.87, -23.51);
   AddToTree("Guaratinguetá", -45.19, -22.81);
   AddToTree("Pindamonhangaba", -45.46, -22.92);
   AddToTree("Cunha", -44.96, -23.07);
   AddToTree("Aparecida", -45.23, -22.84);
   AddToTree("São José dos Campos", -45.88, -23.17);
   AddToTree("Monteiro Lobato", -45.84, -22.95);
   AddToTree("Roseira", -45.3, -22.89);
   AddToTree("Joanópolis", -46.27, -22.93);
   AddToTree("Tremembé", -45.54, -22.95);
   AddToTree("Taubaté", -45.55, -23.02);
   AddToTree("Caçapava", -45.7, -23.1);
   AddToTree("Lagoinha", -45.19, -23.09);
   AddToTree("São Luís do Paraitinga", -45.31, -23.22);
   AddToTree("Redenção da Serra", -45.53, -23.26);
   AddToTree("Jacareí", -45.96, -23.3);
   AddToTree("Jambeiro", -45.68, -23.25);
   AddToTree("Ubatuba", -45.07, -23.43);
   AddToTree("Natividade da Serra", -45.44, -23.37);
   AddToTree("Paraibuna", -45.66, -23.38);

}//end TApp::LoadTree

/**
* Performs a query and outputs its results.
*/
void TApp::PerformQuery(){
   myResult * result;
   TCity * sample;

   // Choose a sample by uncomenting one of then
// sample = new TCity("Motuca", -48.15, -21.5);
// sample = new TCity("Araraquara", -48.17, -21.79);
   sample = new TCity("São Paulo", -46.63, -23.54);
//  sample = new TCity("São Carlos", -47.89, -22.01);
// sample = new TCity("Unknown City", -48, -22);
// sample = new TCity("Dourado", -48.31, -22.1);
//  sample = new TCity("São José do Rio Preto", -20.81, -49.37);
   // Perform a query...
//   result = MetricTree->RangeQuery(sample, 0.34);
//   result = MetricTree->RangeQuery(sample, 0.5, 5);
   result = MetricTree->NearestQuery(sample, 5, false);
// result = MetricTree->KandRangeQuery(sample, 0.3, 5, true);
// result = MetricTree->KorRangeQuery(sample, 0.5, 60);


   // Print query result.
   PrintResults(result);

   // Clean home
   delete result;
   delete sample;

}//end TApp::PerformQuery

/**
* Prints the result set to the standard output.
*
* @param result The result to be printed.
*/
void TApp::PrintResults(myResult * result){
   int w;
   TCity * tmp;

   switch (result->GetQueryType()){
      case myResult::UNKNOWN:
         cout << "Unknown type of query!\n";
         break;
      case myResult::RANGEQUERY:
         cout << "Range query with radius " << result->GetRadius() <<
               ".\n";
         cout << "The sample object was " << *(result->GetSample()) << ".\n";
         break;
      case myResult::KANDRANGEQUERY:
         cout << "K and range query with radius " << result->GetRadius() <<
               " with limit of " << result->GetK() << " objects. Tie list : "
              << (result->GetTie()? "Yes": "No") << ".\n";
         cout << "The sample object was " << *(result->GetSample()) << ".\n";
         break;
      case myResult::KORRANGEQUERY:
         cout << "K or range query with radius " << result->GetRadius() <<
               " with limit of " << result->GetK() << " objects. Tie list : "
               << (result->GetTie()? "Yes": "No") << ".\n";
         cout << "The sample object was " << *(result->GetSample()) << ".\n";
         break;
      case myResult::KNEARESTQUERY:
         cout << "K nearest neighbour query with limit of " <<
               result->GetK() << " objects. Tie list : " << (result->GetTie()? "Yes": "No") << ".\n";
         cout << "The sample object was " << *(result->GetSample()) << ".\n";
         break;
   }//end switch

   if (result->GetNumOfEntries() == 0){
      cout << "No results!\n";
   }else{
      cout << "   " << result->GetNumOfEntries() << " results:\n";
      for (w = 0; w < result->GetNumOfEntries(); w++){
         tmp = (TCity *)(*result)[w].GetObject();
         cout << "      Distance = " << (*result)[w].GetDistance() <<
               " - Object = ";
         cout << *tmp << "\n";
      }//end for
   }//end if
}//end TApp::PrintResults
