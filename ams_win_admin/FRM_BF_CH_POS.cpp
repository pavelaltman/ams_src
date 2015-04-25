//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FRM_BF_CH_POS.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "FRM_BUILD_AN_FUNC"
#pragma link "Oracle"
#pragma resource "*.dfm"
TFrmBFChPos *FrmBFChPos;
//---------------------------------------------------------------------------
__fastcall TFrmBFChPos::TFrmBFChPos(TComponent* Owner)
    : TFrmBuildAnFunc(Owner)
{
}
//---------------------------------------------------------------------------
int TFrmBFChPos::BuildThis(int BuildWhat)
{
};
