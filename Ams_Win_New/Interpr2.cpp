//---------------------------------------------------------------------------

#include "AMS_HDR.h" 
#pragma hdrstop
USERES("Interpr2.res");
USEFORM("MAIN_FRM.cpp", MainFrm);
USEUNIT("AMS_QUERY.cpp");
USEUNIT("Analytic.cpp");
USEUNIT("Anprog.cpp");
USEUNIT("Bookrus1.cpp");
USEUNIT("Intpost.cpp");
USEUNIT("Progcomp.cpp");
USEUNIT("Progfunc.cpp");
USEUNIT("Proglex.cpp");
USEUNIT("Progmisc.cpp");
USEUNIT("Progora.cpp");
USEUNIT("Progpipe.cpp");
USEUNIT("Progrun.cpp");
USEUNIT("Sum.cpp");
USEUNIT("Sumtext.cpp");
USEUNIT("Useprog.cpp");
USE("CURR_PROG_HDR.h", File);
USEUNIT("Strlist.cpp");
USEUNIT("Rctype.cpp");
USEUNIT("orcpp.cpp");
USEUNIT("Orcflds.cpp");
USEUNIT("Date.cpp");
USEUNIT("..\COMMON\AMS_EXC.cpp");
USEUNIT("..\COMMON\AMS_UTILS.cpp");
USELIB("..\Dll_PrintError\PrintError.lib");
USEUNIT("Amsoraru.cpp");
USEUNIT("..\COMMON\UTILS.cpp");
USEUNIT("Syntbl.cpp");
USELIB("D:\Program Files\Borland\CBuilder5\Projects\Lib\doa.lib");
USEUNIT("Scrneng.cpp");
USEFORM("DAT_MOD_INTERPR.cpp", DatMod); /* TDataModule: File Type */
USEUNIT("ADD_MODULE.cpp");
USEFORM("..\COMMON\FRM_SEL_LIST.cpp", FrmSelList);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
         Application->Initialize();
         Application->CreateForm(__classid(TMainFrm), &MainFrm);
         Application->CreateForm(__classid(TDatMod), &DatMod);
         Application->CreateForm(__classid(TFrmSelList), &FrmSelList);
         Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------
