//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_FILTER_FORM.h"
#include "REFRESH_ACCS.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AmsDBGrid"
#pragma link "AmsFooter"
#pragma link "FRM_BRWS_ACC_WINDOW"
#pragma link "Oracle"
#pragma link "OracleData"
#pragma link "RXDBCtrl"
#pragma link "RXSplit"
#pragma resource "*.dfm"
TFrmFilterForm *FrmFilterForm;
//---------------------------------------------------------------------------
__fastcall TFrmFilterForm::TFrmFilterForm(TComponent* Owner)
    : TFrmBrwsAccWindow(Owner)
{
}
//---------------------------------------------------------------------------
void TFrmFilterForm::SetNewArticle(const AnsiString& Art, int Ver, bool WithoutVer)
{
    CurrOpt.WithoutVer=WithoutVer;
    CurrOpt.FilterArticle=Art;
    CurrOpt.FilterVer=Ver;
    RefreshOpt();
}
//---------------------------------------------------------------------------
void TFrmFilterForm::BeginFilter(int AccCode_, const AnsiString& Art, int Ver, bool WithoutVer)
{
    CurrOpt.WithoutVer=WithoutVer;
    Begin(AccCode_, "Фильтр ", Art, Ver);
}
//---------------------------------------------------------------------------
void __fastcall TFrmFilterForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    PC_FilterPos.ChildClose(this);
    Action=caFree;
}
//---------------------------------------------------------------------------
void __fastcall TFrmFilterForm::EscButClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
