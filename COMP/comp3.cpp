//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("comp3.res");
USEPACKAGE("vcl50.bpi");
USEUNIT("AmsInputPoint.cpp");
USEPACKAGE("AMS_COMP2.bpi");
USEPACKAGE("VCLDB50.bpi");
USEPACKAGE("VCLX50.bpi");
USEUNIT("..\COMMON\AMS_READ_CNF_UTIL.cpp");
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Package source.
//---------------------------------------------------------------------------

#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
    return 1;
}
//---------------------------------------------------------------------------
