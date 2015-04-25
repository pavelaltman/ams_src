//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_SEL_CPS_DATE.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmSelCpsDate *FrmSelCpsDate;
//---------------------------------------------------------------------------
__fastcall TFrmSelCpsDate::TFrmSelCpsDate(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFrmSelCpsDate::BBCancelClick(TObject *Sender)
{
    ModalResult=-1;
}
//---------------------------------------------------------------------------
void __fastcall TFrmSelCpsDate::BBOkClick(TObject *Sender)
{
    ModalResult = 1;
}
//-----------------------------------------------------------------------
int TFrmSelCpsDate::SelCpsDate(TDateTime& BegDate)
{
    AmsQuery AQ;
    AQ.SetSqlTextReady(
        "select POINT_DATE from "+BaseUserName+".CPS");
    AQ.Exec("Все ровные даты");
    TStrings* Str= LBDates->Items;
    Str->Clear();
    int CurrSel = -1;
    while (AQ.NotEof())
    {
        TDateTime CD = AQ.GetFldDate(0);

        if ( BegDate == CD)
        {
            CurrSel = Str->Count;
        }
        Str->Add( DateToStr(CD));
        AQ.Next();
    }
    int Res =0;
    if (CurrSel == -1) CurrSel = 0;
    LBDates->ItemIndex= CurrSel;
    if (ShowModal()>0)
    {
        Res = 1;
        BegDate = StrToDate(Str->Strings[LBDates->ItemIndex]);
    }
    return Res;
}
//-----------------------------------------------------------------------
