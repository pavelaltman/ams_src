//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "AmsInputStr.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAmsInputStr *)
{
    new TAmsInputStr(NULL);
}
//---------------------------------------------------------------------------
#define BegTop  2
#define BegLeft 1
#define Prom    10
#define ButWidth 24
//---------------------------------------------------------------------------
__fastcall TAmsInputStr::TAmsInputStr(TComponent* Owner)
    : TAmsInput(Owner)
{
    FBut=new TButton(this);
    FBut->Parent=this;
    FBut->Caption="i";
    FBut->Width = ButWidth;
    FBut->TabStop = false;
    FBut->Visible= false;
    FHistButVis = false;

    FClrBut=new TButton(this);
    FClrBut->Parent=this;
    FClrBut->Caption="Del";
    FClrBut->Width = ButWidth;
    FClrBut->TabStop = false;
    FClrBut->Visible= false;

    FBut->Top = BegTop;
    FBut->Hint="История вводов";
    FBut->ShowHint=true;
    FBut->OnClick= HistButPressed;

    FClrBut->Top = BegTop;
    FClrBut->Hint="Очистить";
    FClrBut->ShowHint=true;
    FClrBut->OnClick= ClrButPressed;

    FInpVal=new TEdit(this);
    FInpVal->Parent=this;
    FInpVal->Top=  BegTop;
    FInpVal->OnKeyDown = InpEditKeyDown;

    FBut->OnKeyDown=InpEditKeyDown;
    FBut->Height = FInpVal->Height;

    FClrBut->OnKeyDown=InpEditKeyDown;
    FClrBut->Height = FInpVal->Height;

    FInpVal->Left= FPrompt->Width+3;
    FInpVal->OnChange = ChangeEdit;
    FInpVal->OnExit = EditExit;
    FirstAct = FInpVal;
    Width= 100;
    Height =
        (FInpVal->Height > FPrompt->Height?FInpVal->Height:FPrompt->Height)
        + BegTop+1;
}
//---------------------------------------------------------------------------
namespace Amsinputstr
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TAmsInputStr)};
         RegisterComponents("Samples", classes, 0);
    }
}
//---------------------------------------------------------------------------
void __fastcall TAmsInputStr::Loaded(void)
{
    TAmsInput::Loaded();
    Resize();
};
//---------------------------------------------------------------------------
void __fastcall TAmsInputStr::Resize(void)
{
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
};
//---------------------------------------------------------------------------
void TAmsInputStr::ChangeEdit2()
{
};
//---------------------------------------------------------------------------
void __fastcall TAmsInputStr::ChangeEdit(System::TObject* Sender)
{
    ChangeEdit2();
    if (ToEnglI)
    {
        AnsiString S = FInpVal->Text;
        bool Changed=false;
        for (int i=1; i<=S.Length(); i++)
        {
            if (S[i]=='і')
            {
                S[i]='i';
                Changed=true;
            }
            else
            if (S[i]=='І')
            {
                S[i]='I';
                Changed=true;
            }
        }
        if (Changed)
        {
            int CPos = FInpVal->SelStart;
            int SelLength=FInpVal->SelLength;
            FInpVal->Text=S;
            FInpVal->SelStart=CPos;
            FInpVal->SelLength=SelLength;
        }
    }
    AmsChanged(this);
    if (FOnChangeEdit)
        FOnChangeEdit(Sender);
};
//---------------------------------------------------------------------------
void __fastcall TAmsInputStr::HistButPressed(System::TObject* Sender)
{
    if (FInputHistory)
        FInpVal->Text = FInputHistory->GetHistVal(FInpVal);
};
//---------------------------------------------------------------------------
void __fastcall TAmsInputStr::ClrButPressed(System::TObject* Sender)
{
};
//---------------------------------------------------------------------------
void __fastcall TAmsInputStr::InpEditKeyDown(TObject *Sender,
    WORD &Key, TShiftState Shift)
{
    MultiCopyKeyDown(Sender, Key,Shift);
    if (Key == 38 || Key == 40 || Key == 13 )
    {
        if ((Key ==38)&& FOnPredEl)
            FOnPredEl(this);
        if ((Key ==40) && FOnNextEl)
            FOnNextEl(this);
        if ((Key==13) && FOnPressEnter)
            FOnPressEnter(this);
        Key =0;
    }
};
//---------------------------------------------------------------------------
void __fastcall TAmsInputStr::SetAmsEnabled(bool Val_)
{
    TAmsInput::SetAmsEnabled(Val_);
    bool VV = Val_;
    FInpVal->Enabled= VV;
};
//---------------------------------------------------------------------------
// Позиционирует курсор за последий символ ввода
void TAmsInputStr::GoBack()
{
    int Pos = FInpVal->Text.Length();
    FInpVal->SelStart = Pos;
};
//---------------------------------------------------------------------------

