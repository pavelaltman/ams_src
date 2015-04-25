//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_ASK_DB_AN_FLD.h"
#include "GET_DB_POS.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "OracleData"
#pragma link "AmsDBGrid"
#pragma link "RXDBCtrl"
#pragma resource "*.dfm"
TFrmAskDbAnFld *FrmAskDbAnFld;
//---------------------------------------------------------------------------
__fastcall TFrmAskDbAnFld::TFrmAskDbAnFld(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmAskDbAnFld::BBOkClick(TObject *Sender)
{
    AnsiString Art = "";
    int Ver = 0;
    int DbId = ODSDBID->Value;
    if (GetDbPos(DbId, Art, Ver, "" ))
    {
        AmsQuery AQ;
        AQ.SetSqlText("select "+ODSREALNAME->Value+" from  *."+
            ODSNAME->Value+" where ARTICLE=:ARTICLE AND VER=:VER");
        AQ.DeclVar("ARTICLE", Art);
        AQ.DeclVar("VER", Ver);
        AQ.Exec("Находим код копируемой аналитической суммы");
        Res=0;
        if (AQ.NotEof())
        {
            Res=AQ.GetFldInt(0);
        }
    }
    ModalResult=1;
}
//---------------------------------------------------------------------------
void __fastcall TFrmAskDbAnFld::BBCancelClick(TObject *Sender)
{
    ModalResult=-1;
}
//---------------------------------------------------------------------------
int TFrmAskDbAnFld::AskDbAnFld(int& Val, int AnId)
{
    ODS->Active=false;
    DS_Q Q(ODS);
    Q.SetSqlText("select O.NAME,O.DESCR, D.REALNAME, D.ALIAS, D.DBID "
        "from *.DBSTR D, *.OBJS O "
        "where VTYPE=3 and RDBID=:DBID and "
	    "*.HavePriv(DBID)=1 and O.OID = D.DBID");
    ODS->SetVariable("DBID", AnId);
    ODS->Active=true;
    int Result=0;
    if (ShowModal()==1)
    {
        Val=Res;
        Result=1;
    }
    return Result;
}
//---------------------------------------------------------------------------
