//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "AmsQuery.h"
#pragma link "Oracle"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAmsQuery *)
{
    new TAmsQuery(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAmsQuery::TAmsQuery(TComponent* Owner)
    : TOracleQuery(Owner)
{
    FAmsSQL = new TStringList();
}
//---------------------------------------------------------------------------
namespace Amsquery
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TAmsQuery)};
         RegisterComponents("Samples", classes, 0);
    }
}
//---------------------------------------------------------------------------
void TAmsQuery::AmsExec()
{
    if (ComponentState.Contains(csDesigning))
    {
        if (InitUser =="")
            BaseUserName = "";
        else
            BaseUserName = InitUser;
    }
    SQL->Text = ReplaceBaseUser(FAmsSQL->Text);
    ExecSQL(this,"");
}
//---------------------------------------------------------------------------
void TAmsQuery::Decl_Set(const AnsiString& VarName, const AnsiString& VarVal)
{
    DeclareVariable(VarName, otString);
    SetVariable(VarName, VarVal);
}
//---------------------------------------------------------------------------
void TAmsQuery::Decl_Set(const AnsiString& VarName, int VarVal)
{
    DeclareVariable(VarName, otInteger);
    SetVariable(VarName, VarVal);
}
//---------------------------------------------------------------------------
void TAmsQuery::Decl_Set(const AnsiString& VarName, double VarVal)
{
    DeclareVariable(VarName, otFloat);
    SetVariable(VarName, VarVal);
}
//---------------------------------------------------------------------------
void TAmsQuery::Decl_Set(const AnsiString& VarName, TDateTime VarVal)
{
    DeclareVariable(VarName, otDate);
    SetVariable(VarName, VarVal);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TStrings* __fastcall TAmsQuery::GetAmsSQL_ ()
{
    return FAmsSQL;
};
//---------------------------------------------------------------------------
void __fastcall TAmsQuery::SetAmsSQL_(TStrings* SL)
{
    FAmsSQL->Clear();
    FAmsSQL->AddStrings(SL);
};
//---------------------------------------------------------------------------
__fastcall TAmsQuery::~TAmsQuery()
{
    delete FAmsSQL;
};
