//---------------------------------------------------------------------------
// uMain.cpp - Implementation of an application that inserts metric
// histograms on a slim-tree
//
// Author: Humberto Razente (hlr@icmc.usp.br)
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfMain *fMain;
//---------------------------------------------------------------------------
__fastcall TfMain::TfMain(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void TfMain::BuildTree(myMetricTree *tree, AnsiString fileName) {
   ifstream in(fileName.c_str());
   char orFile[255];
   int size;
   double ValueX, ValueY;
   tMetricHistogramBin * bins;
   tMetricHistogram *obj;
   int objCount = 0;

   if (tree != NULL){
      if (!in.is_open()){
          MM->Lines->Add("Cannot open input file!");
      }
      else {
          while (in >> size){
             bins = new tMetricHistogramBin[size];
             for (int idx = 0; idx < size; idx++){
                in >> ValueX;
                in >> ValueY;
                bins[idx].Set(ValueX, ValueY);
             }//end for
             in.getline(orFile, 255, '\n');
             MM->Lines->Add(IntToStr(objCount+1) + " " + orFile);
             obj = new tMetricHistogram(size, bins);
             obj->SetTag(objCount);
             tree->Add(obj);
             delete[] bins;
             delete obj;
             objCount++;
          }//end do

         MM->Lines->Add("Added " + IntToStr(objCount) + " objects to tree.");
         in.close();
      }
  }
  else {
      MM->Lines->Add("Zero object added.");
  }
}

void __fastcall TfMain::FormCreate(TObject *Sender) {
    DecimalSeparator = '.';
}
//---------------------------------------------------------------------------

void __fastcall TfMain::RunClick(TObject *Sender) {
   // create slim tree
   DeleteFile("slim.dat");
   stPlainDiskPageManager *PageManager = PageManager = new stPlainDiskPageManager("slim.dat", 8192);
   mySlimTree *SlimTree = new mySlimTree(PageManager);
   BuildTree(SlimTree, "metrichistograms.txt");

   delete SlimTree;
   delete PageManager;
}
//---------------------------------------------------------------------------

