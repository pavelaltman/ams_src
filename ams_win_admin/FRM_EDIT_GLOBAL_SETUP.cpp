//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_EDIT_GLOBAL_SETUP.h"
#include "MAIN_FRM.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "OracleData"
#pragma link "OracleNavigator"
#pragma resource "*.dfm"
TFrmEditGlobalSetup *FrmEditGlobalSetup;
//---------------------------------------------------------------------------
__fastcall TFrmEditGlobalSetup::TFrmEditGlobalSetup(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmEditGlobalSetup::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Action=caFree;    
}
//---------------------------------------------------------------------------
void __fastcall TFrmEditGlobalSetup::FormCreate(TObject *Sender)
{
    ODS->Active=true;    
}
//---------------------------------------------------------------------------
