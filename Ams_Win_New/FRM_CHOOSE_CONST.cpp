//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_CHOOSE_CONST.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AmsStringGrid"
#pragma link "FRM_CHOOSE_OBJ"
#pragma link "Oracle"
#pragma resource "*.dfm"
TFrmChooseConst *FrmChooseConst;
//---------------------------------------------------------------------------
__fastcall TFrmChooseConst::TFrmChooseConst(TComponent* Owner)
    : TFrmChooseObj(Owner)
{
}
//---------------------------------------------------------------------------
