//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "AmsLogon.h"
#include "PRINT_ERROR.h"
#include "OracleLogon.hpp"

#pragma link "Oracle"
#pragma link "OracleLogon"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAmsLogon *)
{
    new TAmsLogon(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAmsLogon::TAmsLogon(TComponent* Owner)
    : TOracleLogon(Owner)
{
}
//---------------------------------------------------------------------------
namespace Amslogon
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TAmsLogon)};
         RegisterComponents("Samples", classes, 0);
    }
}
//---------------------------------------------------------------------------
bool __fastcall TAmsLogon::AmsExecute(void)
{
  bool Result = false;
  if (Session)
  {
    if (Options.Contains(ldDatabaseList))
        BuildOracleAliasList;
    if (FBuchName=="")
    {
        AskString("Введите название бухгалтерии", 30, FBuchName);
    }
    Caption ="Подключение к бухгалтерии "+FBuchName;
    Result = Execute();
  }
  return Result;
};
