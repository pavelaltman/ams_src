//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_ACCS_TIES.h"
#include "Oracle.hpp"
#include "OracleData.hpp"
#include "MAIN_FRM.h"
#include "FRM_ACCS_STR.h"
#include "print_error.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Oracle"
#pragma link "AMS_TREE_VIEW"
#pragma link "AmsAccsView"
#pragma resource "*.dfm"
TFrmAccsTies *FrmAccsTies;
//---------------------------------------------------------------------------
__fastcall TFrmAccsTies::TFrmAccsTies(TComponent* Owner)
    : TForm(Owner), UserClose (false), Accs(NULL)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmAccsTies::BBCancelClick(TObject *Sender)
{
    ModalResult=-1;    
    if (Accs) 
        Accs->Release();    
}
//---------------------------------------------------------------------------
void __fastcall TFrmAccsTies::BBOkClick(TObject *Sender)
{
    OQ->SQL->Text ="begin insert into "+CurrSchemaName+".ACCS_TIES "
       " values (:WHERE_COPY, :MY_ACC, :WHERE_ACC); "
       " commit; end; ";
    OQ->DeleteVariables();
    OQ->DeclareVariable("WHERE_COPY",otString);   
    OQ->DeclareVariable("MY_ACC",otString);   
    OQ->DeclareVariable("WHERE_ACC",otString);   
    OQ->SetVariable("WHERE_COPY", WhereSchemaName);
    for (int i =1; i< StrGrid->RowCount;i++)
    {
        if (StrGrid->Cells[2][i]!="")
        {
            OQ->SetVariable("MY_ACC",StrGrid->Cells[0][i]);
            OQ->SetVariable("WHERE_ACC",StrGrid->Cells[2][i]);
            ExecSQL (OQ, "Соответсвие счетов для "+StrGrid->Cells[1][i]);
        };         
    };
    ModalResult = 1;
    if (Accs) 
        Accs->Release();    
};
//-------------------------------------------------------------------------
void __fastcall TFrmAccsTies::StrGridDblClick(TObject *Sender)
{
//
    if (!Accs)
    {
        Application->CreateForm(__classid(TFrmAccsStr), &Accs);
        Accs->Begin(WhereSchemaName, "",ChooseAcc);         
    }
    Accs->Show();                    
}
//---------------------------------------------------------------------------
void __fastcall TFrmAccsTies::ChooseAcc (TFrmAccsStr* Sender, const TAccName& NewAccs)
{
    if (
         (StrGrid->Cells[StrGrid->Col][StrGrid->Row] == "") ||
         AskMessage("Заменить содержимое ячейки?")==ID_YES
    )
        StrGrid->Cells[StrGrid->Col][StrGrid->Row]=NewAccs.GetSysName();
};
//---------------------------------------------------------------------------
void TFrmAccsTies::Clear(void)
{
    //
    StrGrid->RowCount=1;
    StrGrid->RowCount=2;
    StrGrid->FixedRows=1;
    //FormResize(this);
    StrGrid->Cells[0][0]="Системное назв";
    StrGrid->Cells[1][0]="Пользоват назв";
    StrGrid->Cells[2][0]="В "+WhereSchemaName+" сист назв";
}
//---------------------------------------------------------------------------
int TFrmAccsTies::TieAccs(const AnsiString& CurrSchema,
                          const AnsiString& WhereSchema,
                          int COND_ID, Oracle::TOracleQuery* OQ)
{
    OQ->SQL->Text =
    "select A.NAME, A1.NAME as NAME1, A.DESCR "
    "from "+CurrSchema+".ACCS A, "+WhereSchema+".ACCS A1 "
    "where "
	"A.CODE in (select distinct code from "+CurrSchema+".COND_ITEMS where "
    "COND_ID = :COND_ID) and "
      "not exists(select 1 from "+CurrSchema+".ACCS_TIES "
               " where "
	           " BASEUSERNAME = '"+WhereSchema+"' and ACC_NAME = A.NAME)  AND "
      " A.NAME = A1.NAME(+) order by A.NAME ";

    OQ->DeleteVariables();
    OQ->DeclareVariable("COND_ID", otInteger);
    OQ->SetVariable("COND_ID", COND_ID);
    ExecSQL(OQ, "Проверка все ли связаны счета");
    if (!OQ ->Eof)
    {
        TFrmAccsTies* F ;
        Application->CreateForm(__classid(TFrmAccsTies), &F);
        int HaveAllAccs = 0;
        do
        {
            F->Begin(CurrSchema, WhereSchema);
            Message("Необходимо, чтобы все счета\r\n"
                "имели соответсвие со счетами бухгалтерии "+WhereSchema);
            while (!OQ->Eof)
            {
                F->AddRow(OQ);
                OQ->Next();
            };
            F->ShowModal();
            ExecSQL(OQ, "Проверка все ли связаны счета");
            if (OQ->Eof)
                HaveAllAccs = 1;
        }
        while (HaveAllAccs==0);
        F->Release();
    };
    return 0;
};
//---------------------------------------------------------------------------
int TFrmAccsTies::TieAccsAn(const AnsiString& CurrSchema,
                          const AnsiString& WhereSchema,
                          int DbId, Oracle::TOracleQuery* OQ)
{
    AmsQuery AQ(OQ);
    AQ.Clear();
    AQ.SetSqlTextReady(
    "select A.NAME, A1.NAME as NAME1, A.DESCR "
    "from "+CurrSchema+".ACCS A, "+WhereSchema+".ACCS A1 "
    "where "
	"A.CODE in (select distinct code from "+CurrSchema+".DBADDINFO where "
    "DBID = :DBID) and "
      "not exists(select 1 from "+CurrSchema+".ACCS_TIES "
               " where "
	           " BASEUSERNAME = '"+WhereSchema+"' and ACC_NAME = A.NAME)  AND "
      " A.NAME = A1.NAME(+) order by A.NAME ");

    AQ.DeclVar("DBID", DbId);
    AQ.Exec("Проверка все ли связаны счета");
    if (AQ.NotEof())
    {
        TFrmAccsTies* F ;
        Application->CreateForm(__classid(TFrmAccsTies), &F);
        int HaveAllAccs = 0;
        do
        {
            F->Begin(CurrSchema, WhereSchema);
            Message("Необходимо, чтобы все счета\r\n"
                "имели соответсвие со счетами бухгалтерии "+WhereSchema);
            while (!OQ->Eof)
            {
                F->AddRow(OQ);
                OQ->Next();
            };
            F->ShowModal();
            ExecSQL(OQ, "Проверка все ли связаны счета");
            if (OQ->Eof)
                HaveAllAccs = 1;
        }
        while (HaveAllAccs==0);
        F->Release();
    };
    return 0;
};
//---------------------------------------------------------------------------
void TFrmAccsTies::AddRow(Oracle::TOracleQuery* OQ)
{
    TAccName AName;
    AName.SetSysAccName (OQ->FieldAsString(0));
    StrGrid->RowCount++;
    int CRow = StrGrid->RowCount-2;
    StrGrid->Cells[0][CRow]=AName.GetSysName();
    StrGrid->Cells[1][CRow]=AName.GetUserName()+" "+OQ->FieldAsString(2);
    if (OQ->FieldIsNull(1))
        StrGrid->Cells[2][CRow]="";
    else
        StrGrid->Cells[2][CRow]=OQ->FieldAsString(1);
};
//---------------------------------------------------------------------------
void TFrmAccsTies::Begin(const AnsiString& CurrSchema, 
                         const AnsiString& WhereSchema)
{
    CurrSchemaName = CurrSchema;
    WhereSchemaName = WhereSchema;
    AccView->Begin(WhereSchema, "");
    Clear();        
}
//---------------------------------------------------------------------------
void __fastcall TFrmAccsTies::FormResize(TObject *Sender)
{
    int CurrWidth = (StrGrid->Width-8) >> 3;
    StrGrid->ColWidths[0]=1;
    StrGrid->ColWidths[1]=(CurrWidth<< 2)+CurrWidth;
    StrGrid->ColWidths[2]=(CurrWidth<<1) + CurrWidth;    
}
//---------------------------------------------------------------------------
void __fastcall TFrmAccsTies::Splitter1CanResize(TObject *Sender,
      int &NewSize, bool &Accept)
{
//
   Accept = true;
   FormResize(this);    
}
//---------------------------------------------------------------------------

void __fastcall TFrmAccsTies::AccViewChange(TObject *Sender,
      TTreeNode *Node)
{
    StrGrid->Cells[StrGrid->Col][StrGrid->Row]=TAccName::ToSysNameDescr(Node->Text);
}
//---------------------------------------------------------------------------


