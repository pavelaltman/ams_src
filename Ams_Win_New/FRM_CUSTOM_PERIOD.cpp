//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FRM_CUSTOM_PERIOD.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ToolEdit"
#pragma link "Placemnt"
#pragma resource "*.dfm"
TFrmCustomPeriod *FrmCustomPeriod;
//---------------------------------------------------------------------------
__fastcall TFrmCustomPeriod::TFrmCustomPeriod(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmCustomPeriod::ButCloseClick(TObject *Sender)
{
    Hide();
}
//---------------------------------------------------------------------------
