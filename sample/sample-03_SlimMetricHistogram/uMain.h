//---------------------------------------------------------------------------

#ifndef uMainH
#define uMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include <arboretum/stMemoryPageManager.h>
#include <arboretum/stPlainDiskPageManager.h>
#include <arboretum/stSlimTree.h>
#include <arboretum/stDBMTree.h>
#include <arboretum/stDummyTree.h>
#include <arboretum/stBasicObjects.h>
#include <arboretum/stBasicMetricEvaluators.h>
#include <arboretum/stMAMView.h>

#include "metrichisto.h"

#include <fstream>
#include <string.h>

typedef stMetricTree < tMetricHistogram, tMetricHistogramEvaluator > myMetricTree;
typedef stSlimTree   < tMetricHistogram, tMetricHistogramEvaluator > mySlimTree;
typedef stDummyTree  < tMetricHistogram, tMetricHistogramEvaluator > myDummyTree;
typedef stResult     < tMetricHistogram > myResult;

//---------------------------------------------------------------------------
class TfMain : public TForm
{
__published:	// IDE-managed Components
    TMemo *MM;
    TButton *Run;
    void __fastcall RunClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfMain(TComponent* Owner);
    void BuildTree(myMetricTree *tree, AnsiString fileName);
};
//---------------------------------------------------------------------------
extern PACKAGE TfMain *fMain;
//---------------------------------------------------------------------------
#endif
