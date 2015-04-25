//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_MSG.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmMsg *FrmMsg;
//---------------------------------------------------------------------------
__fastcall TFrmMsg::TFrmMsg(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFrmMsg::Panel1Resize(TObject *Sender)
{
    BBOk->Left=(Width-BBOk->Width)>>1;
}
//---------------------------------------------------------------------------
