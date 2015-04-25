//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FRM_EXIT.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmExit *FrmExit;
//---------------------------------------------------------------------------
__fastcall TFrmExit::TFrmExit(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmExit::mbYesClick(TObject *Sender)
{
  ModalResult=mrOk;    
}
//---------------------------------------------------------------------------

int __fastcall TFrmExit::ShowForm(AnsiString QueryStr)
{
    Label1->Caption=QueryStr;
    return ShowModal();
}