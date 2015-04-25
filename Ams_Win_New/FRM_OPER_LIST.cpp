//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "amsclnt.h"
#include "FRM_OPER_LIST.h"
#include "REFRESH_ACCS.h"
 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AmsDBGrid"
#pragma link "FRM_OPER_LIST_ROOT"
#pragma link "OracleData"
#pragma link "RXDBCtrl"
#pragma link "RXSplit"
#pragma link "Oracle"
#pragma resource "*.dfm"
TFrmOperList *FrmOperList;
//---------------------------------------------------------------------------
__fastcall TFrmOperList::TFrmOperList(TComponent* Owner)
    : TFrmOperListRoot(Owner)
{
}
//---------------------------------------------------------------------------



