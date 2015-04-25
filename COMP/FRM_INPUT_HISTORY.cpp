//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "FRM_INPUT_HISTORY.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmInputHistory *FrmInputHistory;
//---------------------------------------------------------------------------
__fastcall TFrmInputHistory::TFrmInputHistory(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TFrmInputHistory::CBInpHistKeyPress(TObject *Sender,
      char &Key)
{
    if (Key==13)
        ModalResult =1;
    else
    if (Key == 27)
        ModalResult =-1;
           
}
//---------------------------------------------------------------------------

void __fastcall TFrmInputHistory::CBInpHistChange(TObject *Sender)
{
    ModalResult=1;    
}
//---------------------------------------------------------------------------

