//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_CHOOSE_OPER.h"
#include "FRM_BROWS_AN.h"
#include "amsclnt.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AmsDBGrid"
#pragma link "AmsFooter"
#pragma link "FRM_BRWS_ACC_WINDOW"
#pragma link "Oracle"
#pragma link "OracleData"
#pragma link "RXDBCtrl"
#pragma link "RXSplit"
#pragma link "FRM_BRWS_ACC_WINDOW"
#pragma link "FRM_OPER_LIST_ROOT"
#pragma resource "*.dfm"
TFrmChooseOper *FrmChooseOper;
//---------------------------------------------------------------------------
__fastcall TFrmChooseOper::TFrmChooseOper(TComponent* Owner)
    : TFrmOperListRoot(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmChooseOper::BBOkClick(TObject *Sender)
{
//
        TFrmBrowsAn* Frm = ParentF;
        if (Frm)
        {
            Frm->OperSelectedOk(DSet->FieldByName("ITEM_SUM_ID")->AsInteger);
        }
        Close();
}
//---------------------------------------------------------------------------
void TFrmChooseOper::Begin_(TFrmBrowsAn* ParentF_, int AccCode)
{
    ParentF=ParentF_;
    Begin (AccCode,"Выбор операции для копирования аналитической суммы");
}
//---------------------------------------------------------------------------
void __fastcall TFrmChooseOper::BBCancelClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void TFrmChooseOper::Begin(int AccCode_, const AnsiString& Caption_)
{
    Grid1->Width=ClientWidth-4 ;
    Grid2->Width=ClientWidth-4 ;
    Grid1->Height=ClientHeight-150 ;
    AccCode=AccCode_;

    DSet->Active=false ;
    DSetItems->Active=false ;

    Beg=G.ur.beg_date.val.T() ;
    End=G.ur.end_date.val.T() ;
    DSet->SetVariable("BEG_DATE",Beg) ;
    DSet->SetVariable("END_DATE",End) ;
    DSet->SetVariable("POST",post) ;
    if (bound)
    {
        DSet->DeclareVariable("OP_ID",otInteger) ;
        DSet->SetVariable("OP_ID",bound) ;
    }

    HelpContext= bound ? 14 : (del ? 97 : (post ? 98 : (allowed ? 101 :6))) ;

    del=0;
    AnsiString tname= "opers" ,
               tiname= "op_items" ,
               delstr= "" ,
               boundstr= "",
               allow_tabstr= allowed ? (","+BaseUserName+".privtest ") : AnsiString("") ,
               allow_condstr= allowed ? " AND UNAME=USER AND cond_id=POID AND PR0>0 " : "" ;

    DS_Q Q(DSet);
    Q.DelVars();
    AnsiString Str="";
    BuildFullSql(Str,Q);

    Q.SetSqlText(Str);

    DSetItems->SQL->Text="select "+BaseUserName+".GetDCLetter(side) AS ASIDE,"+
            BaseUserName+".User_Acc(a.name) AS ANAME,"+
            BaseUserName+".AnalInfo(op_id,op_subid,i.code) AS ANALINFO, VALUE "
            "from "+BaseUserName+"."+tiname+" i,"+BaseUserName+".accs a, "+BaseUserName+".sums "
            "where op_id = :op_id and op_subid = :op_subid AND i.code=a.code "
            "AND a.node=0 AND sum_id=item_sum_id "
            "ORDER BY ASIDE,A.NAME" ;
    Caption=Caption;
    ExecODS(DSet);

    //DSet->Active=true ;
    DSetItems->Active=true ;
}
//---------------------------------------------------------------------------
void TFrmChooseOper::BuildFullSql   (AnsiString& Sql, DS_Q& Q)
{
    Q.DeclVar("BEG_DATE",Beg) ;
    Q.DeclVar("END_DATE",End) ;
    Q.DeclVar("CODE", AccCode);
    del=0;

    Sql="select O.rowid, O.* , *.GetOperSum(o.op_id,o.op_subid) as SUMMA,"
                    "*.GetOperAcc('D',o.op_id,o.op_subid) as DACCS,"
                    "*.GetOperAcc('K',o.op_id,o.op_subid) as CACCS, OI.ITEM_SUM_ID "
                      "from *.op_items OI, *.opers O, *.privtest P "
                      "where O.status=1 AND UNAME=USER AND O.cond_id=POID AND PR0>0 "
                      "and O.OP_ID=OI.OP_ID and O.OP_SUBID=OI.OP_SUBID and "
                      "OI.CODE=:CODE "
                      "AND OI.op_date>=:beg_date AND OI.op_date<:end_date "
                      "ORDER by O.OP_DATE, O.OP_ID, O.OP_SUBID" ;

};
//---------------------------------------------------------------------------

void __fastcall TFrmChooseOper::Grid1DblClick(TObject *Sender)
{
    //
    BBOkClick(this);
}
//---------------------------------------------------------------------------
void TFrmChooseOper::KeyEvent (WORD &Key, TShiftState Shift)
{
    if (Key==13)
    {
        Key=0;
        BBOkClick(this);
    }
    else
    {
        TFrmOperListRoot::KeyEvent (Key,Shift);
    }
};
//---------------------------------------------------------------------------
AnsiString TFrmChooseOper::FormFindSql(DS_Q& Q)
{
    Q.DeclVar("BEG_DATE", Beg);
    Q.DeclVar("END_DATE", End);
    Q.DeclVar("CODE", AccCode);
    return "select O.OP_ID, O.OP_SUBID from *.op_items OI, *.opers O, *.privtest P "
                      "where O.status=1 AND UNAME=USER AND O.cond_id=POID AND PR0>0 "
                      "and O.OP_ID=OI.OP_ID and O.OP_SUBID=OI.OP_SUBID and "
                      "OI.CODE=:CODE "
                      "AND OI.op_date>=:beg_date AND OI.op_date<:end_date ";
}
//---------------------------------------------------------------------------
void __fastcall TFrmChooseOper::ActEditExecute(TObject *Sender)
{
    BBOkClick(this);
}
//---------------------------------------------------------------------------

