//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "AmsInputDbPos.h"
#pragma link "AmsInputStr"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAmsInputDbPos *)
{
    new TAmsInputDbPos(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAmsInputDbPos::TAmsInputDbPos(TComponent* Owner)
    : TAmsInputStr(Owner)
{
    FInpVal->Enabled= false;
    FInpVal->TabStop =  false;

    HBut= new TButton(this);
    HBut ->Top = -50;
    HBut->Parent= this;
    HBut->OnEnter =ChangeActControl;

    FBut->Caption = "БД";
    FBut->Hint = "Выбор позиции БД";
    FBut->TabStop = true;
    FBut->Visible = true;
    FBut->OnKeyDown = InpEditKeyDown;
    FBut->OnClick = HistButPressed;

    FClrBut->Visible = true;
    FClrBut->OnKeyDown = InpEditKeyDown;
    FClrBut->OnClick = ClrButPressed;

    FirstAct = FBut;
}
//---------------------------------------------------------------------------
namespace Amsinputdbpos
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TAmsInputDbPos)};
         RegisterComponents("Samples", classes, 0);
    }
}
//---------------------------------------------------------------------------
void __fastcall TAmsInputDbPos::ChangeEdit(System::TObject* Sender)
{};
//---------------------------------------------------------------------------
void __fastcall TAmsInputDbPos::HistButPressed(System::TObject* Sender)
{
    if (FAmsButPressed)
        FAmsButPressed(this);
};
//---------------------------------------------------------------------------
void __fastcall TAmsInputDbPos::ClrButPressed(System::TObject* Sender)
{
    if (FAmsClrButPressed)
        FAmsClrButPressed(this);
};
//---------------------------------------------------------------------------
void __fastcall TAmsInputDbPos::SetAmsEnabled(bool Val)
{
    TAmsInput::SetAmsEnabled(Val);
    FBut->Enabled = Val;
}
//---------------------------------------------------------------------------
void __fastcall TAmsInputDbPos::ChangeActControl(System::TObject* Sender)
{
    int Pred = 0;
    int Next = 0;
    GetAsyncKeyState(VK_UP);
    GetAsyncKeyState(VK_DOWN);
    GetAsyncKeyState(VK_TAB);
    if (GetAsyncKeyState(VK_UP))
        Pred=1;
    else
    if (GetAsyncKeyState(VK_DOWN))
        Next=1;
    else
    if (GetAsyncKeyState(VK_TAB))
    {
        GetAsyncKeyState(VK_SHIFT);
        if (!GetAsyncKeyState(VK_SHIFT))
            Next=1;
    }
    if (Pred && FOnPredEl)
        FOnPredEl (this);
    else
    if (Next && FOnNextEl)
        FOnNextEl(this);
    else
        FBut->SetFocus();
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

