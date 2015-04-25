//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AmsInputAnal.h"
#pragma link "AmsInput"
#pragma link "AmsInputDbPos"
#pragma link "AmsInputStr"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAmsInputAnal *)
{
    new TAmsInputAnal(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAmsInputAnal::TAmsInputAnal(TComponent* Owner)
    : TAmsInputDbPos(Owner), GetAnalInfo_(0), FAnalVal(0)
{
    FBut->Caption = "Ан";
    FBut->Hint = "Ввод/просмотр аналитики";

    FAnalVal=new TEdit(this);
    FAnalVal->Parent=this;
    FAnalVal->Top=FInpVal->Top;
    FAnalVal->Left=FInpVal->Left;
    FAnalVal->Width=FInpVal->Width;
    FAnalVal->Height=FInpVal->Height;
    FAnalVal->OnKeyDown = InpEditKeyDown;
    FAnalVal->Enabled=false;
    FInpVal->Visible=false;
}
//---------------------------------------------------------------------------
namespace Amsinputanal
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TAmsInputAnal)};
         RegisterComponents("Samples", classes, 0);
    }
}
//---------------------------------------------------------------------------
void TAmsInputAnal::ViewChanged()
{
    if (!ComponentState.Contains(csDesigning))
    {
        if (GetAnalInfo_)
        {
            GetAnalInfo_(this);

        }
        else
        {
            FAnalVal->Text=FInpVal->Text;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TAmsInputAnal::Resize(void)
{
    TAmsInputStr::Resize();
    if (FAnalVal)
    {
        FAnalVal->Top=FInpVal->Top;
        FAnalVal->Width=FInpVal->Width;
        FAnalVal->Height=FInpVal->Height;
        FAnalVal->Left=FInpVal->Left;
    }
    /*
    if (FInpVal)
    {
        int BW = FBut->Visible?(2*ButWidth+2):0;
        FInpVal->Width = Width - BW- FPrompt->Width - Prom ;
        FInpVal->Left =  FPrompt->Width+3;
        if (BW)
        {
            FBut->Left = FInpVal->Left + FInpVal->Width + 1;
            FClrBut->Left = FInpVal->Left + FInpVal->Width + 3+FBut->Width;
        }
        Height =
            (FInpVal->Height > FPrompt->Height?FInpVal->Height:FPrompt->Height)
            + BegTop+1;
    }
    */
};
//---------------------------------------------------------------------------

