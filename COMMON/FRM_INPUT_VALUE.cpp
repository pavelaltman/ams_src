//---------------------------------------------------------------------------
 
#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_INPUT_VALUE.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmInputValue *FrmInputValue=0;
//---------------------------------------------------------------------------
__fastcall TFrmInputValue::TFrmInputValue(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmInputValue::BBCancelClick(TObject *Sender)
{
    ModalResult=-1;
}
//---------------------------------------------------------------------------
void __fastcall TFrmInputValue::BBOkClick(TObject *Sender)
{
    ModalResult = 1;
}
//-------------------------------------------------------------------------

void __fastcall TFrmInputValue::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Action = caHide;
}
//---------------------------------------------------------------------------

void __fastcall TFrmInputValue::FormShow(TObject *Sender)
{
    BBOk->SetFocus();
    EdValue->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TFrmInputValue::FormDestroy(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------

