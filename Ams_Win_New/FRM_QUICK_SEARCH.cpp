//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_QUICK_SEARCH.h"
#include "FRM_BROWS_DB.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmQuickSearch *FrmQuickSearch;
//---------------------------------------------------------------------------
__fastcall TFrmQuickSearch::TFrmQuickSearch(TComponent* Owner)
    : TForm(Owner), Setted(false)
{
}
//---------------------------------------------------------------------------
void TFrmQuickSearch::SetSeaching (TFrmBrowsDb* Frm)
{
    FrmBrowsDb=Frm;
    EdInp->Text = "";
    Setted= true;
    Show();
}
//---------------------------------------------------------------------------
void __fastcall TFrmQuickSearch::EdInpKeyPress(TObject *Sender, char &Key)
{
    if (Setted && (Key==13||Key==27))
    {
        Hide();
        FrmBrowsDb->EndQuickSearch();
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmQuickSearch::EdInpChange(TObject *Sender)
{
    if (Setted)
    {
        FrmBrowsDb->QuickSearchNewKey(EdInp->Text);
    }
}
//---------------------------------------------------------------------------
