//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "AMS_READ_CNF_UTIL.h"
#include "AMS_READ_CNF.h"
#include "AMS_UTILS.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
void InitCnf (LPSTR CmdLine_)
{
  CConfigWin Cnf (CmdLine_);
  AnsiString L = Cnf.Next();
  while (L != "")
  {
       if (L=="READONLY")
            GC::ReadOnly= true;
       else if (L=="TRACE")
            GC::TraceOn=true ;
       else if (L=="RULE")
            GC::RuleOn=true ;
       else if (L.Pos("USER=")==1)
            GC::UserName = L.SubString(6, L.Length()-4) ;
       else if (L.Pos("PASS=")==1)
            GC::UserPwd = L.SubString(6, L.Length()-4) ;
       else if (L.Pos("CONN=")==1)
            GC::ConnStr = L.SubString(6, L.Length()-4) ;
       else if (L.Pos("BOOK=")==1)
       {
            GC::BaseUserName = L.SubString(6, L.Length()-4);
            BaseUserName =GC::BaseUserName;
       }
       else if (L.Pos("DOCPATH=")==1)
            GC::DocPath = L.SubString(9, L.Length()-7) ;
       else if (L.Pos("DRVFILE=")==1)
            GC::DrvFile = L.SubString(9, L.Length()-7) ;
       else if (L.Pos("PRN=")==1)
            GC::Prn = L.SubString(5, L.Length()-3) ;
       else if (L.Pos("AUTORUN=")==1)
            GC::AutoRun = L.SubString(9, L.Length()-7) ;
       else if (L.Pos("AUTOEXIT=")==1)
            GC::AutoExit = true ;
       else if (L.Pos("NEW")==1)
            GC::NewPlan = true ;
       else if (L.Pos("DIRECT")==1)
            GC::Direct = true ;
       else if (L.Pos("EXCEL=")==1)
            GC::ExcelType = L.SubString(7, L.Length()-5) ;
       else if (L.Pos("VIEWDEBUG")==1)
            GC::ViewDebug = true ;
       else if (L.Pos("SHOWCORRACCS")==1)
            GC::ShowCorrAccs = true ;
       else if (L.Pos("ACCSFULLEXPAND")==1)
            GC::AccsFullExpand = true ;
       else if (L.Pos("GUI")==1)
            GC::GUI = true ;
       else if (L.Pos("STATUS")==1)
            GC::Status = true;
       else if (L.Pos("TOOLBAR")==1)
            GC::ToolBar = true;
       else if (L.Pos("MAINMENU")==1)
            GC::MainMenu = true;
       else if (L.Pos("WORDPAD")==1)
            GC::WordPad = true;     
       else if (L.Pos("ANNEXTPOS=")==1)
            GC::AnNextPos = L.SubString(11, L.Length()-9) ;
       else if (L.Pos("INIFILE=")==1)
       {
        Cnf.BeginFile(L.SubString(9, L.Length()-7)) ;
       }
       L = Cnf.Next();
  }// Пока L не NULL
}
//---------------------------------------------------------------------------
 