//---------------------------------------------------------------------------
#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_POST_OPERS.h"
#include "amsclnt.h"
#include "REFRESH_ACCS.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AmsDBGrid"
#pragma link "FRM_OPER_LIST"
#pragma link "Oracle"
#pragma link "OracleData"
#pragma link "RXDBCtrl"
#pragma link "RXSplit"
#pragma resource "*.dfm"
TFrmPostOpers *FrmPostOpers;
//---------------------------------------------------------------------------
__fastcall TFrmPostOpers::TFrmPostOpers(TComponent* Owner)
    : TFrmOperList(Owner)
{
}
//---------------------------------------------------------------------------
void TFrmPostOpers::Begin(int AccCode_, const AnsiString& CurrTitle_)
{
    AccCode=AccCode_;
    CurrTitle=CurrTitle;
    Grid1->Width=ClientWidth-4 ;
    Grid2->Width=ClientWidth-4 ;
    Grid1->Height=ClientHeight-150 ;

    DSet->Active=false ;
    DSetItems->Active=false ;

    Beg=G.ur.beg_date.val.T() ;
    End=G.ur.end_date.val.T() ;
    FormTitle() ;
    DS_Q Q(DSet);
    AnsiString Sql="";
    BuildFullSql(Sql, Q);
    Q.SetSqlText(Sql);
    ExecODS(DSet);

    DSetItems->SQL->Text="select "+BaseUserName+".GetDCLetter(side) AS ASIDE,"+
            BaseUserName+".User_Acc(a.name) AS ANAME,"+
            BaseUserName+".AnalInfo(op_id,op_subid,i.code) AS ANALINFO, VALUE "
            "from "+BaseUserName+".op_items i,"+BaseUserName+".accs a, "+BaseUserName+".sums "
            "where op_id = :op_id and op_subid = :op_subid AND i.code=a.code "
            "AND a.node=0 AND sum_id=item_sum_id "
            "ORDER BY ASIDE,A.NAME" ;
    ExecODS(DSetItems);
    AccsToWin.Add(this);
}
//---------------------------------------------------------------------------
AnsiString TFrmPostOpers::FormFindSql(DS_Q& Q)
{
    Q.DeclVar("POST", 1);
    Q.DeclVar("BEG_DATE", Beg);
    Q.DeclVar("END_DATE", End);
    Q.DeclVar("CODE", AccCode);
    AnsiString Sql ="select O.OP_ID, O.OP_SUBID "
    "from *.OPERS O, *.OP_ITEMS OI, *.privtest P "
    "where O.OP_DATE>=:BEG_DATE and O.OP_DATE<:END_DATE and O.POST=:POST and "
        " STATUS=1 and "
        "  OI.OP_ID=O.OP_ID and OI.OP_SUBID=O.OP_SUBID and OI.CODE=:CODE AND "
        "  P.UNAME=USER AND O.cond_id=P.POID AND PR0>0 ";
    GetOrderBy(Sql);
    return Sql;

};
//---------------------------------------------------------------------------
void TFrmPostOpers::BuildFullSql   (AnsiString& Sql, DS_Q& Q)
{
    Sql ="select O.rowid, O.* , "+BaseUserName+
                        ".GetOperSum(O.op_id, O.op_subid) as SUMMA,"
                      "*.GetOperAcc('D', O.op_id, O.op_subid) as DACCS,"
                      "*.GetOperAcc('K', O.op_id, O.op_subid) as CACCS "
    "from *.OPERS O, *.OP_ITEMS OI, *.privtest P "
    "where O.OP_DATE>=:BEG_DATE and O.OP_DATE<:END_DATE and O.POST=:POST and "
        " STATUS=1 and "
        "  OI.OP_ID=O.OP_ID and OI.OP_SUBID=O.OP_SUBID and OI.CODE=:CODE AND "
        "  P.UNAME=USER AND O.cond_id=P.POID AND PR0>0 ";
    Q.DeclVar("POST", 1);
    Q.DeclVar("BEG_DATE", Beg);
    Q.DeclVar("END_DATE", End);
    Q.DeclVar("CODE", AccCode);

    if (FilterSetted)
    {
        FrmFilter->FormWhereCond(Sql, Q, true);
    }

    GetOrderBy(Sql);
    Q.SetSqlText(Sql);
};
//---------------------------------------------------------------------------
void TFrmPostOpers::FormTitle()
{
  Caption=CurrTitle+" ["+Beg.DateString()+" - "+End.DateString()+"]";
}
//---------------------------------------------------------------------------
