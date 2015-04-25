//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_GEN_SETUP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmGenSetup *FrmGenSetup;
//---------------------------------------------------------------------------
__fastcall TFrmGenSetup::TFrmGenSetup(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmGenSetup::Button1Click(TObject *Sender)
{
    Hide();    
}
//---------------------------------------------------------------------------
