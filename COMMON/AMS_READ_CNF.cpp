//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "AMS_READ_CNF.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

extern char doc_path[];
extern int To1251;
 
//----------------------------------------------------------------
void CConfigWin::BeginFile(const AnsiString& FileName)
{
     char Buf[500]; 
     if (ReadFile) return; // Не можем из файла конфигурации это делать
     strcpy (Buf, FileName.c_str());
     F.open(FileName.c_str());
     if (F.good()) 
        ReadFile=true;
};
//----------------------------------------------------------------
AnsiString CConfigWin::Next()
{
   if (!ReadFile)
   {
    if (LastPos>CnfLine.Length())
    {
        return "";
    }
    else
    {
        int j = LastPos;
        char Res [100];
        char* ChR = Res;
        int i = 0;
        while (j <=CnfLine.Length() && CnfLine[j] != ' '&& CnfLine[j] != '\t')
        {
            Res [i++] = toupper(CnfLine[j]);
            j++;
        }
        while (j<=CnfLine.Length() && (CnfLine[j] == ' ' || CnfLine[j] == '\t'))
            j++;
        Res[i]=0;
        LastPos = j;
        return ChR;
    }
   }
   else
   {
    while (!F.eof())
    {
      memset(Buf,0, 400);
      F.getline(Buf, 399);
      char* L = Buf; 
      if (L && (L[0]!='#')&&(L[0]!='\n'))
      {
       int Len = strlen(L);
       char* Back = &L[Len-1];
       // Trunc left
       while ((
               (*L==' ')||(*L=='\t')
               ) && (*L!=0)
             )
       {
            L++ ;
       }
       // Trunc Right
       while ( (
                (*Back=='\n')||(*Back==' ')||(*Back=='\t')
               ) && (*Back!=0)
             ) Back--;
       Back[1]=0;
       char* R = L;
       while (*R)
       {
            *R = toupper(*R);
            R++;
       }
       if (strlen(L)>0) return L;
      }
    } // Пока не конец файла
    F.close();
    ReadFile= false;
    return Next();
   } // Если конфигурация считывается из файла
}
//----------------------------------------------------------------
bool         GC::ReadOnly = false;
bool         GC::TraceOn  = false;
bool         GC::RuleOn   = false;
AnsiString   GC::UserName = "AMS";
AnsiString   GC::UserPwd  = "";
AnsiString   GC::BaseUserName="AMS";
AnsiString   GC::ConnStr ="S";
AnsiString   GC::DocPath ="AMS";
AnsiString   GC::DrvFile ;
AnsiString   GC::Prn;
AnsiString   GC::AutoRun;
bool         GC::AutoExit = false;
bool         GC::NewPlan  = false;
bool         GC::Direct   = false;
bool         GC::ViewDebug= false;
bool         GC::SqlDebug = false;
bool         GC::EnglI    = false;
AnsiString   GC::ExcelType;
bool         GC::GUI = false;
bool         GC::ShowCorrAccs   = false;
bool         GC::AccsFullExpand = false;
bool         GC::Status = false;
bool         GC::ToolBar = false;
bool         GC::MainMenu = false;
bool         GC::AccList = false;
int          GC::SizePic = 200;
AnsiString   GC::AnNextPos="";
bool         GC::WordPad = false;
//----------------------------------------------------------------
extern void InitCType();
extern int new_plan ;
void InitCnf (LPSTR CmdLine_)
{
  InitCType();
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
       else if (L.Pos("TO1251=")==1)
            To1251= L.SubString(8, L.Length()-4).ToIntDef(0) ;
       else if (L.Pos("CONN=")==1)
            GC::ConnStr = L.SubString(6, L.Length()-4) ;
       else if (L.Pos("BOOK=")==1)
       {
            GC::BaseUserName = L.SubString(6, L.Length()-4);
            BaseUserName =GC::BaseUserName;
       }
       else if (L.Pos("DOCPATH=")==1)
       {
            GC::DocPath = L.SubString(9, L.Length()-7) ;
            strcpy (doc_path, GC::DocPath.c_str());
       }
       else if (L.Pos("DRVFILE=")==1)
            GC::DrvFile = L.SubString(9, L.Length()-7) ;
       else if (L.Pos("PRN=")==1)
            GC::Prn = L.SubString(5, L.Length()-3) ;
       else if (L.Pos("AUTORUN=")==1)
       {
            GC::AutoRun = L.SubString(9, L.Length()-7);
            GC::AutoRun.TrimRight();
       }
       else if (L.Pos("AUTOEXIT")==1)
            GC::AutoExit = true ;
       else if (L.Pos("NEW")==1)
       {
            GC::NewPlan = true ;
            new_plan=1;
       }
       else if (L.Pos("DIRECT")==1)
            GC::Direct = true ;
       else if (L.Pos("EXCEL=")==1)
            GC::ExcelType = L.SubString(7, L.Length()-5) ;
       else if (L.Pos("VIEWDEBUG")==1)
       {
            GC::ViewDebug=true;
            AmsViewDebug=true;
       }
       else if (L.Pos("ENGI")==1)
       {
            GC::EnglI=true;
            ToEnglI=true;
       }

       else if (L.Pos("SHOWCORRACCS")==1)
            GC::ShowCorrAccs = true ;
       else if (L.Pos("ACCSFULLEXPAND")==1)
            GC::AccsFullExpand = true ;

       else if (L.Pos("INIFILE=")==1)
       {
        Cnf.BeginFile(L.SubString(9, L.Length()-7)) ;
       }
       else if (L.Pos("ANNEXTPOS=")==1)
            GC::AnNextPos = L.SubString(11, L.Length()-9) ;
       else if (L.Pos("GUI")==1)
            GC::GUI = true;
       else if (L.Pos("STATUS")==1)
            GC::Status = true;
       else if (L.Pos("TOOLBAR")==1)
            GC::ToolBar = true;
       else if (L.Pos("MAINMENU")==1)
            GC::MainMenu = true;
       else if (L.Pos("ACCLIST")==1)
            GC::AccList = true;
       else if (L.Pos("WORDPAD")==1)
            GC::WordPad = true;
       L = Cnf.Next();
  }// Пока L не NULL
}
//----------------------------------------------------------------


