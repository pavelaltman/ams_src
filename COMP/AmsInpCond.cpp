//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "AmsInpCond.h"
#include "enums.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
const int TAmsInpCond::VPos = 1;

static inline void ValidCtrCheck(TAmsInpCond *)
{
    new TAmsInpCond(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAmsInpCond::TAmsInpCond(TComponent* Owner)
    : TWinControl(Owner), Pred(NULL), Post(NULL)
{

    EdTxt= new TEdit(this);
    EdTxt->Parent = this;
    EdTxt->Top=VPos;
    EdTxt->Anchors <<akLeft<<akTop<<akRight<<akBottom;
    EdTxt->OnKeyDown = InpKeyDown;

    CBSign= new TComboBox(this);
    CBSign->Width= 45;
    CBSign->Parent = this;
    CBSign->Top = VPos;
    CBSign->OnKeyPress = CBKeyPress;
    CBSign->Style = Stdctrls::csDropDownList;

    Lab = new TLabel (this);
    Lab->Parent = this;
    Lab->Top = VPos+2;

    Height = CBSign ->Height+ ( VPos <<1);
    Width = 100;
}
//---------------------------------------------------------------------------
namespace Amsinpcond
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TAmsInpCond)};
         RegisterComponents("Samples", classes, 0);
    }
}
//---------------------------------------------------------------------------
void TAmsInpCond::RecalcOutput()
{
    Lab->Left = 2;
    CBSign->Left = Lab->Left+Lab->Width+2;
    EdTxt->Left=CBSign->Left +CBSign->Width+2;
    EdTxt->Width = Width - 2- EdTxt->Left;
};
//---------------------------------------------------------------------------
void TAmsInpCond::SetFld(const AnsiString& Prompt, int FldType,
    const AnsiString& HintVal)
{
    TStrings* Str = CBSign->Items;
    Str->Clear();
    switch (FldType)
    {
        case aftDate:
        case aftSumma:
        case aftInt:
            Str->Add("=");
            Str->Add("!=");
            Str->Add(">=");
            Str->Add(">");
            Str->Add("<=");
            Str->Add("<");
            CBSign->Text = "=";
            CBSign->ItemIndex= 0;
        break;
        case aftStr:
            Str->Add("=");        // 0
            Str->Add("!=");       // 1
            Str->Add(">=");       // 2
            Str->Add(">");        // 3
            Str->Add("<=");       // 4
            Str->Add("<");        // 5
            Str->Add("~");        // 6
            CBSign->Text = "~";
            CBSign->ItemIndex= 6;
        break;
        case aftBool:
            Str->Add("=");
            Str->Add("!=");
            CBSign->Text = "=";
            CBSign->ItemIndex= 0;
        break;
    }
    if (HintVal!="")
    {
        Hint = HintVal;
        ShowHint = true;
    }
    AmsPrompt= Prompt;
    EdTxt->Text = "";
}
//---------------------------------------------------------------------------
void TAmsInpCond::ClrCond()
{
    EdTxt ->Text = "";
}
//---------------------------------------------------------------------------
AnsiString TAmsInpCond::GetStr()
{
    return EdTxt->Text.Trim();
}
//---------------------------------------------------------------------------
AnsiString TAmsInpCond::GetSign()
{
    return CBSign->Text;
}
//---------------------------------------------------------------------------
int TAmsInpCond::GetInt()
{
    int i = GetStr().ToIntDef(0);
    return i;
}
//---------------------------------------------------------------------------
double TAmsInpCond::GetSum()
{
    return ::GetSum (GetStr());
}
//---------------------------------------------------------------------------
TDateTime TAmsInpCond::GetDate()
{
    return ::GetDate (GetStr());
}
//---------------------------------------------------------------------------
int TAmsInpCond::GetPromptLen()
{
    return Lab->Width;
}
//---------------------------------------------------------------------------
void TAmsInpCond::SetPromptLen(int PLen)
{
    Lab->AutoSize = false;
    Lab->Alignment = taRightJustify;
    Lab->Width = PLen;
    RecalcOutput();
}
//---------------------------------------------------------------------------
void __fastcall TAmsInpCond::CBKeyPress(TObject *Sender, char &Key)
{
    if (Key==32 || Key ==13)
    {
        CBSign->DroppedDown= true;
    }
};
//---------------------------------------------------------------------------
void __fastcall TAmsInpCond::InpKeyDown(TObject *Sender,
    WORD &Key, TShiftState Shift)
{
    if (Key == 38 || Key == 40 || Key == 13 )
    {
        if ((Key ==38)&& Pred)
            Pred->SetFocus();
        if ((Key ==40 || Key ==13) && Post)
            Post->SetFocus();
    }
};
//---------------------------------------------------------------------------
void __fastcall TAmsInpCond::DoEnter(void)
{
    EdTxt->SetFocus();
};

