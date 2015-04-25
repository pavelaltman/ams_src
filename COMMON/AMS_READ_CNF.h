//---------------------------------------------------------------------------

#ifndef AMS_READ_CNFH
#define AMS_READ_CNFH
//----------------------------------------------------------------
#include <fstream>
//----------------------------------------------------------------

class GC // Global Config Variables
{
public:
    static bool         ReadOnly;
    static bool         TraceOn ;
    static bool         RuleOn;
    static AnsiString   UserName;
    static AnsiString   UserPwd;
    static AnsiString   BaseUserName;
    static AnsiString   ConnStr;
    static AnsiString   DocPath;
    static AnsiString   DrvFile;
    static AnsiString   Prn;
    static AnsiString   AutoRun;
    static bool         AutoExit;
    static bool         NewPlan;
    static bool         Direct;
    static bool         ViewDebug;
    static bool         SqlDebug;
    static bool         EnglI;
    static AnsiString   ExcelType;
    static bool         GUI;
    static bool         ShowCorrAccs;
    static bool         AccsFullExpand;
    static bool         MainMenu;
    static bool         Status;
    static bool         ToolBar;
    static bool         AccList;
    static int          SizePic;
    static AnsiString   AnNextPos;
    static bool         WordPad;
};
//----------------------------------------------------------------
// Для того чтобы параметры конфигурации читались единообразно
//----------------------------------------------------------------
class CConfigWin
{
   AnsiString CnfLine;
   int LastPos;
   bool ReadFile;
   char Buf [400];
   std::ifstream F;
public:
   CConfigWin(const AnsiString& CmdLine)
     {CnfLine=CmdLine; LastPos = 1; ReadFile=false;};
   AnsiString Next();

   void BeginFile(const AnsiString& FileName);

   ~CConfigWin() {F.close();};   
};
//----------------------------------------------------------------
extern void InitCnf (LPSTR CL);
//----------------------------------------------------------------
#endif
