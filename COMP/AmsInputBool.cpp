//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "AmsInputBool.h"
#pragma link "AmsInput"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAmsInputBool *)
{
    new TAmsInputBool(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAmsInputBool::TAmsInputBool(TComponent* Owner)
    : TAmsInput(Owner)
{
       
    FChB    =new TCheckBox(this);
    FChB->Parent=this;
    FChB->Top=  FPrompt->Top; 
    FChB->Left= FPrompt->Width+3; 
    FChB->Caption = "";
    FChB->AllowGrayed =true;
    FChB->OnClick=AmsChanged;
    FirstAct = FChB;
    Width= 100;
    Height =FPrompt->Height +FPrompt->Top +1;
    Resize();            
}
//---------------------------------------------------------------------------
namespace Amsinputbool
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TAmsInputBool)};
         RegisterComponents("Samples", classes, 0);
    }
}
//---------------------------------------------------------------------------
void __fastcall TAmsInputBool::Loaded(void)
{
    TWinControl::Loaded();
    Resize();
};
//---------------------------------------------------------------------------
void __fastcall TAmsInputBool::Resize(void)
{
    FChB->Left= FPrompt->Width+3; 
};
//---------------------------------------------------------------------------

