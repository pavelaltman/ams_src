//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "AmsInputStrML.h"
#pragma link "AmsInput"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAmsInputStrML *)
{
    new TAmsInputStrML(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAmsInputStrML::TAmsInputStrML(TComponent* Owner)
    : TAmsInput(Owner)
{
    FMem = new TMemo (this);
    FMem->WordWrap= false;
    FMem ->Parent = this;
    FMem->OnKeyDown = MultiCopyKeyDown;
    FMem->OnChange=AmsChanged;

    FirstAct = FMem;
    FPrompt->Alignment=taLeftJustify;
    Width = 50;
    AmsLineCount= 2;    
}
//---------------------------------------------------------------------------
namespace Amsinputstrml
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TAmsInputStrML)};
         RegisterComponents("Samples", classes, 0);
    }
}
//---------------------------------------------------------------------------
void __fastcall TAmsInputStrML::Resize(void)
{
    if (FMem)
    {
        FPrompt ->Left = 3;
        FPrompt -> Top = 2;
        FPrompt->Width = Width - 6;
        FMem->Left = FPrompt->Left;
        FMem->Top = FPrompt->Top + FPrompt->Height+3;
        FMem->Width= Width - (FMem->Left <<1);
        Height = FMem->Top+ FMem->Height + 3;  
    }        
}
//---------------------------------------------------------------------------
void __fastcall TAmsInputStrML::SetLineCount(int Val)
{
    FLinesCount = Val;
    FHeight= abs(FMem->Font->Height);
    FHeight= FHeight + (FHeight>>1);
    FHeight= FHeight * Val;
    FMem->Height = FHeight;
    Resize();
};
//---------------------------------------------------------------------------
