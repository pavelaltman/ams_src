//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_READ_EXCEL_IMP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "OracleData"
#pragma resource "*.dfm"
TFrmReadExcelImp *FrmReadExcelImp;
//---------------------------------------------------------------------------
__fastcall TFrmReadExcelImp::TFrmReadExcelImp(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmReadExcelImp::BBOkClick(TObject *Sender)
{
    //
    if (ODS->Eof)
        return ;
    AmsQuery AQ;
    AQ.SetSqlText("select REALNAME from *.EXCEL_IMP_ITEMS where DB_ID=:DB_ID and "
        "SHORT_NAME=:SHORT_NAME order by ORD");
    AQ.DeclVar("DB_ID", DbId);
    AQ.DeclVar("SHORT_NAME", ODS->FieldByName("SHORT_NAME")->AsString);
    AQ.Exec("");
    Res->clear();
    while (AQ.NotEof())
    {
        AnsiString FName= AQ.GetFldStr(0);
        int i = 0;
        while (i< FD->Count() && FD->At(i).RealName!=FName) i++;
        if (i< FD->Count())
        {
            Res->push_back(i);
        }
        AQ.Next();
    }
    Message("—читано "+IntToStr(Res->size())+" полей");
    ModalResult=1;
}
//---------------------------------------------------------------------------
void __fastcall TFrmReadExcelImp::BitBtn1Click(TObject *Sender)
{
    ModalResult=-1;    
}
//---------------------------------------------------------------------------
int TFrmReadExcelImp::ReadExcelImp(int DbId_, TFldsDescr* FD_, std::vector<int> * Res_)
{
    //TODO: Add your source code here
    DbId=DbId_;
    FD=FD_;
    Res=Res_;

    DS_Q Q(ODS);
    Q.SetSqlText("select * from *.EXCEL_IMP where DB_ID=:DB_ID order by SHORT_NAME");
    Q.DeclVar("DB_ID", DbId);
    ExecODS(ODS);
    return ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TFrmReadExcelImp::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Action=caFree;    
}
//---------------------------------------------------------------------------
