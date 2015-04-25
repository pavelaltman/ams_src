//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "AMS_HDR.h"
#pragma hdrstop
#include "FRM_COL_GUI.h"
#include "amsclnt.h"

#include "ams_win_c.h"
#include "DAT_MOD.h"
#include "FRM_CHOOSE_OBJ.h"
#include "FRM_OPER_LIST.h"
#include "FRM_PERIOD.h"
#include "FRM_WINDOWS_LIST.h"
#include "FRM_CHOOSE_DB_POS.h"
#include "ADD_MODULE.h"
#include "FRM_ACC_LIST.h"
#include "FRM_CHOOSE_STD_OPER.h"
#include "FRM_CALC.h"
#include "Impoper.h"
#include "FRM_USER_OPTS.h"
#include "FRM_MAP.h"
#include "FRM_EXIT.h"
#include "FRM_INPUT_OPER.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFrmColGUI *FrmColGUI;
//---------------------------------------------------------------------------
__fastcall TFrmColGUI::TFrmColGUI(TComponent* Owner)
    : TForm(Owner)
{
   Begin();
}
//---------------------------------------------------------------------------

void __fastcall TFrmColGUI::Im4Click(TObject *Sender)
{
    AmsWinC->ActBDExecute(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFrmColGUI::FormActivate(TObject *Sender)
{
    WindowState=wsMaximized;
}
//---------------------------------------------------------------------------

void __fastcall TFrmColGUI::Im7Click(TObject *Sender)
{
//    WindowState=wsMinimized;
        TFrmInputOper* Frm;
        Application->CreateForm(__classid (TFrmInputOper), &Frm);
        Frm->Init(0,0,0,0,37);
}
//---------------------------------------------------------------------------

void __fastcall TFrmColGUI::Im1Click(TObject *Sender)
{
//    WindowState=wsMinimized;
    AmsWinC->ActExitExecute(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFrmColGUI::Im2Click(TObject *Sender)
{
//    WindowState=wsMinimized;
   AmsWinC->ActShowPlanAccExecute(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFrmColGUI::Begin()
{
   AmsWinC->ToolBar1->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall TFrmColGUI::Im3Click(TObject *Sender)
{
//   AmsWinC->ActListWindExecute(Sender);
//    Visible=false;
//    WindowState=wsMinimized;
    AmsWinC->ActBDExecute(Sender);
    AmsWinC->FrmChooseDb->EditDB(24);
//    AmsWinC->FrmChooseDb->Hide();

}
//---------------------------------------------------------------------------
void __fastcall TFrmColGUI::Im5Click(TObject *Sender)
{
//    WindowState=wsMinimized;
//   AmsWinC->ActShowPlanAccExecute(Sender);
    ShowMessage("Im5");
}
//---------------------------------------------------------------------------
void __fastcall TFrmColGUI::Im6Click(TObject *Sender)
{
//    WindowState=wsMinimized;
//   AmsWinC->ActShowPlanAccExecute(Sender);
      ShowMessage("Im6");
}
//---------------------------------------------------------------------------
