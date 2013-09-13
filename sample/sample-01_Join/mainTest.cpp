//---------------------------------------------------------------------------

#pragma hdrstop
#include "app.h"

//---------------------------------------------------------------------------

#pragma argsused
int main(int argc, char* argv[])
{
   TApp app;

   // Init application.
   app.Init();

   // Run it.
   app.Run();

   // Release resources.
   app.Done();

   return 0;
}
//---------------------------------------------------------------------------


