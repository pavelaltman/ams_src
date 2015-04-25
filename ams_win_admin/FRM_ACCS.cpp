//---------------------------------------------------------------------------
#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_ACCS.h"
#include "MAIN_FRM.h"
#include "FRM_ACCS_STR.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "OracleData"
#pragma resource "*.dfm"
//TFrmAccs *FrmAccs;
//---------------------------------------------------------------------------
__fastcall TFrmAccs::TFrmAccs(TComponent* Owner)
  : TForm(Owner)
{           
}
//---------------------------------------------------------------------------
void __fastcall TFrmAccs::ToolButton1Click(TObject *Sender)
{
 //
 Q->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TFrmAccs::FormClose(TObject *Sender, TCloseAction &Action)
{
  Action = caHide;  
}
//---------------------------------------------------------------------------
void __fastcall TFrmAccs::ToolButton3Click(TObject *Sender)
{
  //FrmAccsStr->Show();  
}
//---------------------------------------------------------------------------

