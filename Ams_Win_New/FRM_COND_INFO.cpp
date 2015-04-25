//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_COND_INFO.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AmsStringGrid"
#pragma resource "*.dfm"
TFrmCondInfo *FrmCondInfo;
//---------------------------------------------------------------------------
__fastcall TFrmCondInfo::TFrmCondInfo(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmCondInfo::FormClose(TObject *Sender, TCloseAction &Action)
{
    Action=caFree;
}
//---------------------------------------------------------------------------
void TFrmCondInfo::CondInfo(int CondId_, const AnsiString& CondText)
{
    Caption=CondText;
    CondId=CondId_;

    SG->Cells[0][0]="Сторно";
    SG->Cells[1][0]="Дебет";
    SG->Cells[2][0]="Кредит";
    SG->Cells[3][0]="Документ";
    SG->Cells[4][0]="Текст";


    AmsQuery AQCond;
    AQCond.SetSqlText(
    //              0          1        2     3
        "select COND_SUBID, DOC_TYPE, TEXT, STORNO "
        "from *.CONDS "
        "where COND_ID=:COND_ID");
    AQCond.DeclVarInt("COND_ID", CondId);

    AmsQuery AQItems;
    AQItems.SetSqlText(
        "select A.NAME, side from *.COND_ITEMS CI, *.ACCS A "
        "where COND_ID=:COND_ID and COND_SUBID=:COND_SUBID and A.CODE=CI.CODE "
        "order by ITEM_NUM ");
    AQItems.DeclVarInt("COND_ID", CondId);
    AQItems.DeclVarInt("COND_SUBID", 0);

    AQCond.Exec("Находим все части станд операции");
    int CurrLine=1;
    while (AQCond.NotEof())
    {
        AnsiString Dt, Ct;
        AQItems.SetVar("COND_SUBID", AQCond.GetFldInt(0));
        AQItems.Exec("");
        AnsiString* Curr = NULL;
        while (AQItems.NotEof())
        {
            AnsiString Side = AQItems.GetFldStr(1);
            if ( Side=="D")
                Curr=&Dt;
            else
                Curr=&Ct;
            if ((*Curr)!="") (*Curr)+=", ";
            (*Curr)+=TAccName::ToUserName(AQItems.GetFldStr(0));
            AQItems.Next();
        }

        if (CurrLine==SG->RowCount)
            SG->RowCount= CurrLine+1;

        if (AQCond.GetFldInt(3))
            SG->Cells[0][CurrLine]=">";

        SG->Cells[1][CurrLine]=Dt;
        SG->Cells[2][CurrLine]=Ct;
        SG->Cells[3][CurrLine]=AQCond.GetFldStr(1);
        SG->Cells[4][CurrLine]=AQCond.GetFldStr(2);

        CurrLine++;
        AQCond.Next();
    }
    int HT = SG->RowCount* (SG->DefaultRowHeight+1) +16;
    Height= HT+65;
    ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TFrmCondInfo::BBOkClick(TObject *Sender)
{
    ModalResult=1;  
}
//---------------------------------------------------------------------------
