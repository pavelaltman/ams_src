//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "AmsInput.h"
#include "Clipbrd.hpp"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAmsInput *)
{
    //new TAmsInput(NULL);
}
//---------------------------------------------------------------------------
#define BegTop 5
//---------------------------------------------------------------------------
__fastcall TAmsInput::TAmsInput(TComponent* Owner)
    : TWinControl(Owner),Pred(NULL), Post(NULL), FirstAct(NULL)
{
    Parent = dynamic_cast<TWinControl*>(Owner);
    FPrompt=new TLabel(this);
    FPrompt->Parent=this;
    FPrompt->Top=BegTop;
    FPrompt->AutoSize = false;
    FPrompt->WordWrap =false;
    FPrompt->Left= 1;
    FPrompt->Alignment= taRightJustify;
    FPrompt->Layout=tlCenter;
    FPrompt->Width = 40; 
    FPromptLen = 40;
    //Align = alTop;
}
//---------------------------------------------------------------------------
void __fastcall TAmsInput::Loaded(void)
{
    TWinControl::Loaded();
    SetPromptLen(FPromptLen);    
};
//---------------------------------------------------------------------------
namespace Amsinput
{
    void __fastcall PACKAGE Register()
    {
         //TComponentClass classes[1] = {__classid(TAmsInput)};
         //RegisterComponents("Samples", classes, 0);
    }
}
//---------------------------------------------------------------------------
void __fastcall TAmsInput::MultiCopyKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
    if (Shift.Contains(ssCtrl))
    {
        if (Key==86)
        {
            if (Clipboard()->HasFormat(CF_TEXT))
            {
                AnsiString S = Clipboard()->AsText;
                if (S.Pos("\t") && FOnMultiCopy)
                {
                    Key = 0;
                    int i = S.Pos("\t");
                    Clipboard()->AsText= S.SubString(1, i-1);
                    Clear();
                    FOnMultiCopy(this, S);
                }
            }
        }
    }
};
//---------------------------------------------------------------------------
void __fastcall TAmsInput::KeyDown(Word &Key, Classes::TShiftState Shift)
{
    if (Key==13)
    {
        if (FOnPressEnter)
        {
            FOnPressEnter(this);
        }
        Key=0;
    }
};
//---------------------------------------------------------------------------
void __fastcall TAmsInput::DoEnter(void)
{
    if (FirstAct)
        FirstAct->SetFocus();
};
//---------------------------------------------------------------------------
void TAmsInput::GoBack()
{
};
//---------------------------------------------------------------------------
void __fastcall TAmsInput::AmsChanged(System::TObject* Sender)
{
    if (FOnChanged)
        FOnChanged(this);
}
//---------------------------------------------------------------------------

