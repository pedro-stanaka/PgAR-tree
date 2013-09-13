//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("mh.res");
USEFORM("uMain.cpp", fMain);
USELIB("..\..\lib\arboretum.lib");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
         Application->Initialize();
         Application->CreateForm(__classid(TfMain), &fMain);
         Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------
