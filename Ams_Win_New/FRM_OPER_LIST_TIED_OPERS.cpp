//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "amsclnt.h"
#include "REFRESH_ACCS.h"
#include "FRM_OPER_LIST_TIED_OPERS.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AmsDBGrid"
#pragma link "FRM_OPER_LIST_ROOT"
#pragma link "Oracle"
#pragma link "OracleData"
#pragma link "RXDBCtrl"
#pragma link "RXSplit"
#pragma resource "*.dfm"
TFrmOperListTiedOpers *FrmOperListTiedOpers;
//---------------------------------------------------------------------------
__fastcall TFrmOperListTiedOpers::TFrmOperListTiedOpers(TComponent* Owner)
    : TFrmOperListRoot(Owner)
{
}
//---------------------------------------------------------------------------
void TFrmOperListTiedOpers::Init(int _post,int _del,int _bound,AnsiString _text,int _allowed)
{
    post=_post ;
    del=_del ;
    bound=_bound ;
    bound_parent=_text ;
    allowed=_allowed ;

    Grid1->Width=ClientWidth-4 ;
    Grid2->Width=ClientWidth-4 ;
    Grid1->Height=ClientHeight-150 ;

    DSet->Active=false ;
    DSetItems->Active=false ;

    Beg=G.ur.beg_date.val.T() ;
    End=G.ur.end_date.val.T() ;
    //DSet->SetVariable("BEG_DATE",Beg) ;
    //DSet->SetVariable("END_DATE",End) ;
    //DSet->SetVariable("POST",post) ;
    if (bound)
    {
        DSet->DeclareVariable("OP_ID",otInteger) ;
        DSet->SetVariable("OP_ID",bound) ;
    }

    HelpContext= bound ? 14 : (del ? 97 : (post ? 98 : (allowed ? 101 :6))) ;


    AnsiString tname= del ? "del_opers" : "opers" ,
               tiname= del ? "del_op_items" : "op_items" ,
               delstr= del ? "Del" : "" ,
               boundstr= bound ? " AND op_id=:op_id " : " AND post=:post "
                      "AND op_date>=:beg_date AND op_date<:end_date ",
               allow_tabstr= allowed ? (","+BaseUserName+".privtest ") : AnsiString("") ,
               allow_condstr= allowed ? " AND UNAME=USER AND O.cond_id=POID AND PR0>0 " : "" ;

    AnsiString OrderBy;

    //AnsiString OrderBy = bound ? AnsiString("OP_DATE, CI.ORD") :
    //(G.ur.sort_num ? AnsiString("OP_DATE, DOC_TYPE, "+BaseUserName+".NormChar(DOC_NUM, 30) ") :
    //AnsiString("OP_DATE, OP_ID, OP_SUBID"));
    AnsiString AddConds1;
    AnsiString AddConds2;
    if (bound)
    {
        OrderBy ="OP_DATE, CI.ORD";
        AddConds1=", "+BaseUserName+".CONDS CI ";
        AddConds2=" and O.COND_ID=CI.COND_ID(+) AND O.OP_SUBID=CI.COND_SUBID(+) ";
    }
    else
    {
        AddConds1 = " ";
        AddConds2 = " ";
        if (G.ur.sort_num)
            OrderBy="OP_DATE, DOC_TYPE, "+BaseUserName+".NormChar(DOC_NUM, 30) ";
        else
            OrderBy="OP_DATE, OP_ID, OP_SUBID";
    }

    DSet->SQL->Text="select O.rowid, O.* , "+BaseUserName+".Get"+delstr+"OperSum(op_id,op_subid) as SUMMA,"
                      +BaseUserName+".Get"+delstr+"OperAcc('D',op_id,op_subid) as DACCS,"
                      +BaseUserName+".Get"+delstr+"OperAcc('K',op_id,op_subid) as CACCS "
                      "from "+BaseUserName+"."+tname+" O "+allow_tabstr+AddConds1+
                      "where status=1 "+boundstr+
                      allow_condstr+AddConds2+
                      "ORDER by "+OrderBy;

    DSetItems->SQL->Text="select "+BaseUserName+".GetDCLetter(side) AS ASIDE,"+
            BaseUserName+".User_Acc(a.name) AS ANAME,"+
            BaseUserName+".AnalInfo(op_id,op_subid,i.code) AS ANALINFO, VALUE "
            "from "+BaseUserName+"."+tiname+" i,"+BaseUserName+".accs a, "+BaseUserName+".sums "
            "where op_id = :op_id and op_subid = :op_subid AND i.code=a.code "
            "AND a.node=0 AND sum_id=item_sum_id "
            "ORDER BY ASIDE,A.NAME" ;
    FormTitle() ;
    ExecODS(DSet);
    TNumericField* SumFld=dynamic_cast<TNumericField *>(DSet->FieldByName("SUMMA"));
    SumFld->DisplayFormat=",0.00###";
    //DSet->Active=true ;
    DSetItems->Active=true ;
    AccsToWin.Add(this);
}
//------------------------------------------------------------------------------
