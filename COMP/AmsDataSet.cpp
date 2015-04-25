//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "AmsDataSet.h"
#pragma link "OracleData"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAmsDataSet *)
{
    new TAmsDataSet(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAmsDataSet::TAmsDataSet(TComponent* Owner)
    : TOracleDataSet(Owner)
{
    FAmsSQL = new TStringList();
}
//---------------------------------------------------------------------------
namespace Amsdataset
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TAmsDataSet)};
         RegisterComponents("Samples", classes, 0);
    }
}
//---------------------------------------------------------------------------
TStrings* __fastcall TAmsDataSet::GetAmsSQL_ ()
{
    return FAmsSQL;
};
//---------------------------------------------------------------------------
void __fastcall TAmsDataSet::SetAmsSQL_(TStrings* SL)
{
    FAmsSQL->Clear();
    FAmsSQL->AddStrings(SL);
};
//---------------------------------------------------------------------------
bool __fastcall TAmsDataSet::GetAmsActive_ ()
{
    return Active;
};
//---------------------------------------------------------------------------
void __fastcall TAmsDataSet::SetAmsActive_(bool Val)
{
    if (Val == Active) return;
    if (ComponentState.Contains(csDesigning))
    {
        if (InitUser =="")
            BaseUserName = "";
        else
            BaseUserName = InitUser;
    }
    if (Val)
        SQL->Text = ReplaceBaseUser(FAmsSQL->Text);
    Active = Val;
};
//---------------------------------------------------------------------------
__fastcall TAmsDataSet::~TAmsDataSet()
{
    delete FAmsSQL;
};
