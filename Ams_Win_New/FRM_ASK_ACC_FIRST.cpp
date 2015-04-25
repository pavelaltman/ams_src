//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_ASK_ACC_FIRST.h"
#include "FRM_ASK_ACC_CODE.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmAskAccFirst *FrmAskAccFirst;
//---------------------------------------------------------------------------
__fastcall TFrmAskAccFirst::TFrmAskAccFirst(TComponent* Owner)
    : TForm(Owner), FrmAskAccCode(0)
{
}
//---------------------------------------------------------------------------
AnsiString TFrmAskAccFirst::GetAccName()
{
    int q=ShowModal();
    if (q==-1) return "";
    else return EdAcc->Text;
};
//---------------------------------------------------------------------------
void __fastcall TFrmAskAccFirst::ButChooseAccClick(TObject *Sender)
{
    //
    if (!FrmAskAccCode)
    {
        Application->CreateForm(__classid(TFrmAskAccCode), &FrmAskAccCode);
    }

    FrmAskAccCode->EdAccCode->Text="";
    int Acc=FrmAskAccCode->AskAccCode(BaseUserName, "");
    if (!Acc)
    {
        return;
    }
    AnsiString S= FrmAskAccCode->AccsView->Selected->Text;
    if (int i = S.Pos(" "))
    {
        S=S.SubString(1, i-1);
        EdAcc->Text=S;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmAskAccFirst::BBOkClick(TObject *Sender)
{
    ModalResult=1;
}
//---------------------------------------------------------------------------
void __fastcall TFrmAskAccFirst::FormDestroy(TObject *Sender)
{
    //if (FrmAskAccCode)
    //    FrmAskAccCode->Release();
}
//---------------------------------------------------------------------------
void __fastcall TFrmAskAccFirst::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if (Shift.Contains(ssAlt))
    {
        if (Key==67)
        {
            Key=0;
            ButChooseAccClick(this);
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmAskAccFirst::BBCancelClick(TObject *Sender)
{
    ModalResult=-1;
}
//---------------------------------------------------------------------------

void __fastcall TFrmAskAccFirst::FormShow(TObject *Sender)
{
  EdAcc->Text="";
  EdAcc->SetFocus();
}
//---------------------------------------------------------------------------

