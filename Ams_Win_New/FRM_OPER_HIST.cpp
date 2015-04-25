//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "FRM_OPER_HIST.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "OracleData"
#pragma link "RXDBCtrl"
#pragma resource "*.dfm"
TFrmOperHist *FrmOperHist;
//---------------------------------------------------------------------------
__fastcall TFrmOperHist::TFrmOperHist(TComponent* Owner)
    : TForm(Owner)
{
    ODS->SQL->Text =
    "select H.REVISION, H.REV_TIME, H.USERNAME, U.FULL_NAME "
    "from "+BaseUserName+".history H, "+BaseUserName+".USER_OPTIONS U "
    "where "
	"H.OP_ID = :OP_ID and "
	"H.OP_SUBID = :OP_SUBID and "
	"H.USERNAME = U.USERNAME ";
}
//---------------------------------------------------------------------------

void TFrmOperHist::ShowHist (int _op_id, int _op_subid)
{
    ODS->SetVariable("OP_ID", _op_id);
    ODS->SetVariable("OP_SUBID", _op_subid);
    ODS->Active=false;
    ODS->Active = true;
    if (GC::ViewDebug)
    {
        Caption= "История операции [ "+IntToStr(_op_id)+" "+IntToStr(_op_subid)+"]";
    }
    ShowModal();
}
