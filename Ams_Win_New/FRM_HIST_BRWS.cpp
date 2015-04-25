//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_HIST_BRWS.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AMS_TREE_VIEW"
#pragma link "AmsDbCatView"
#pragma link "FRM_BROWS_DB"
#pragma link "Oracle"
#pragma link "OracleData"
#pragma link "RXDBCtrl"
#pragma link "AmsDBGrid"
#pragma resource "*.dfm"
TFrmHistBrws *FrmHistBrws;
//---------------------------------------------------------------------------
__fastcall TFrmHistBrws::TFrmHistBrws(TComponent* Owner)
    : TFrmBrowsDb(Owner)
{
}
//---------------------------------------------------------------------------
void TFrmHistBrws::Begin(int DBID, 
                        const AnsiString& DBName, 
                        const AnsiString& DBDescr,
                        const AnsiString& SetName)
{
    CurrDBId = DBID;
    if (DBName=="")
    {
        OQ->SQL->Text= "select NAME from "+BaseUserName+".OBJS where OID=:OID and HIST = 1";
        OQ->DeleteVariables();
        OQ->DeclareVariable("OID", otInteger);
        OQ->SetVariable("OID", DBID);
        ExecSQL(OQ, "Находим название базы данных");
        if (! OQ->Eof)           
        {
            CurrDBName = OQ->FieldAsString(0);
        }
        else
        {
            THROW_AMS_EXC ("Нет базы данных с таким кодом "+IntToStr(DBID));
        }
    }
    else    
        CurrDBName = DBName;
    CurrSetName = SetName;
    CurrCatId = -1;
    Caption = "Удаленные позиции ["+CurrDBName+"] "+DBDescr;
    //RefreshCats();
    BuildCols();
    //BuildSqlQuery(TOracleDataSet* ODS);    
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TFrmHistBrws::BuildSqlQuery(TOracleDataSet* ODS_)
{
    ODS_->Active = false;
    ODS_->DeleteVariables();

    AnsiString SelBeg = "select D.ROWID";
    for ( int i = 0; i< FldsDescr.Count(); i++)
    {
        SelBeg +=", "+FldsDescr[i].FullName;
    }
    SelBeg +=" from "+BaseUserName+"."+CurrDBName+"_H D "
    "where " 
	"not exists (select 1 from "+BaseUserName+"."+CurrDBName+" where " 
				"ARTICLE = D.ARTICLE and " 
				"VER = D.VER) "
    "order by ARTICLE, VER, CATID";
    ODS->SQL->Text= SelBeg;
}
//---------------------------------------------------------------------------
