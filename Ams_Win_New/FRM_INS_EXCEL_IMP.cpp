//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_INS_EXCEL_IMP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "OracleData"
#pragma resource "*.dfm"
TFrmInsExcelImp *FrmInsExcelImp;
//---------------------------------------------------------------------------
__fastcall TFrmInsExcelImp::TFrmInsExcelImp(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmInsExcelImp::BBOkClick(TObject *Sender)
{
    //
    if (EdName->Text!="")
    {
        AmsQuery AQ;
        AQ.SetSqlText("select 1 from *.EXCEL_IMP where DB_ID=:DB_ID "
            "and SHORT_NAME=:SHORT_NAME");
        AQ.DeclVar("DB_ID", DbId);
        AQ.DeclVar("SHORT_NAME", EdName->Text);
        AQ.Exec("Проверка существует ли");
        if (AQ.NotEof())
        {
            if (AskMessage("Отчет c таким именем уже существует. Перезаписать?", false)!=ID_YES)
            {
                return;
            }
        }
        AQ.SetPLText("delete from *.EXCEL_IMP_ITEMS where DB_ID=:DB_ID and SHORT_NAME=:SHORT_NAME;"
        "delete from *.EXCEL_IMP where DB_ID=:DB_ID and SHORT_NAME=:SHORT_NAME;"
        "insert into *.EXCEL_IMP (DB_ID, SHORT_NAME, DESCR) values "
        " (:DB_ID, :SHORT_NAME, :DESCR); commit;");
        AQ.DeclVar("DESCR", MemDescr->Lines->Text);
        AQ.Exec("Вставка описания");
        AQ.Clear();
        AQ.SetPLText("insert into *.EXCEL_IMP_ITEMS (DB_ID, SHORT_NAME, ORD, REALNAME) "
        "values(:DB_ID, :SHORT_NAME, :ORD, :REALNAME);commit;");
        AQ.DeclVar("DB_ID", DbId);
        AQ.DeclVar("SHORT_NAME", EdName->Text);
        AQ.DeclVar("ORD", 0);
        AQ.DeclVar("REALNAME", "");
        for (unsigned int i = 0; i < Res->size();i++)
        {
            AQ.SetVar("ORD", (int)i);
            SFldOpt& FO = FldsDescr->At(Res->at(i));
            AQ.SetVar("REALNAME", FO.RealName);
            AQ.Exec("");
        }
        Message("Записано");
    }
    else
    {
        ErrorMessage("Укажите краткое имя для импорта");
        return;
    }
    ModalResult=1;    
}
//---------------------------------------------------------------------------

void __fastcall TFrmInsExcelImp::BitBtn1Click(TObject *Sender)
{
    ModalResult=-1;    
}
//---------------------------------------------------------------------------
void TFrmInsExcelImp::InsExcelImp(int DbId_, TFldsDescr* FD, std::vector<int> * Res_)
{
    DbId=DbId_;
    Res=Res_;
    FldsDescr=FD;

    DS_Q Q(ODS);
    Q.SetSqlText("select * from *.EXCEL_IMP where DB_ID=:DB_ID order by SHORT_NAME");
    Q.DeclVar("DB_ID", DbId);
    ExecODS(ODS);
    ShowModal(); 
}
//---------------------------------------------------------------------------

void __fastcall TFrmInsExcelImp::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Action=caFree;    
}
//---------------------------------------------------------------------------

