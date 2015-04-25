//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_USER_OPTS.h"
#include "amsclnt.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RXSpin"
#pragma link "ToolEdit"
#pragma resource "*.dfm"
TFrmUserOpts *FrmUserOpts;
//---------------------------------------------------------------------------
__fastcall TFrmUserOpts::TFrmUserOpts(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
int TFrmUserOpts::UserOpts()
{
    //TODO: Add your source code here
    int Res = 0;
    do {
        AmsQuery AQ;
        AQ.SetSqlText("select * from *.USER_OPTIONS "
                        "where USERNAME=USER");
        AQ.Exec("Находим установки пользователя");
        if (AQ.NotEof())
        {
            SESum->AsInteger    =AQ.GetFldInt("SUM_LEN");
            SEKop->AsInteger    =AQ.GetFldInt("SUM_DIG");
            DatInp->Date        =AQ.GetFldDate("INP_DATE");
            ChBCurrDate->Checked=AQ.GetFldInt("CUR_DATES");
            ChBApps->Checked=AQ.GetFldInt("APPS");
            ChBGotoEnd->Checked= AQ.GetFldInt("SET_TO_END");
            ChBSortByDocNum->Checked=AQ.GetFldInt("SORT_NUM");
        }
        if (ShowModal()==1)
        {
            AQ.SetSqlText ( "begin update *.user_options "
                    "set SUM_LEN=:SUM_LEN, "
                    "SUM_DIG=:SUM_DIG, INP_DATE=:INP_DATE, "
                    "CUR_DATES=:CUR_DATES, SORT_NUM=:SORT_NUM,"
                    "APPS=:APPS, SET_TO_END=:SET_TO_END "
                    " where USERNAME=USER; commit; end;");
            AQ.DeclVar("SUM_LEN",   SESum->AsInteger);
            AQ.DeclVar("SUM_DIG",   SEKop->AsInteger);
            AQ.DeclVar("INP_DATE",  DatInp->Date);
            AQ.DeclVar("CUR_DATES", (int)ChBCurrDate->Checked);
            AQ.DeclVar("SORT_NUM",  (int)ChBSortByDocNum->Checked);
            AQ.DeclVar("APPS", (int)ChBApps->Checked);
            AQ.DeclVar("SET_TO_END", (int)ChBGotoEnd->Checked);
            AQ.Exec("Вставляем установки пользователя");
            G.ur.sum_len    =SESum->AsInteger;
            G.ur.sum_dig    =SEKop->AsInteger;
            G.ur.apps   = ChBApps->Checked;
            G.ur.inp_date.SetNull();
            G.ur.cur_dates=ChBCurrDate->Checked;
            G.ur.sort_num=ChBSortByDocNum->Checked;
            G.ur.set_to_end=ChBGotoEnd->Checked;
            Res=1;

        }

    }while (0);
    return Res;
}
//---------------------------------------------------------------------------

void __fastcall TFrmUserOpts::BBCancelClick(TObject *Sender)
{
    ModalResult=-1;
}
//---------------------------------------------------------------------------
void __fastcall TFrmUserOpts::BBOkClick(TObject *Sender)
{
    ModalResult=1;
}
//-------------------------------------------------------------------------

