//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "amsclnt.h"
#include "FRM_OPER_LIST_ROOT.h"
#include "FRM_OPER_HIST.h"
#include "FRM_INPUT_OPER.h"
#include "FRM_PERIOD.h"
#include "FRM_OPER_LIST.h"
#include "ams_win_c.h"
#include "REFRESH_ACCS.h"
#include "FRM_SHOW_TIED_DB.h"
#include "FRM_OPER_LIST_TIED_OPERS.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "OracleData"
#pragma link "RXDBCtrl"
#pragma link "OracleNavigator"
#pragma link "RXSplit"
#pragma link "AmsDBGrid"
#pragma link "Oracle"
#pragma resource "*.dfm"
extern void ClrAction(TAction*);
//---------------------------------------------------------------------------
__fastcall TFrmOperListRoot::TFrmOperListRoot(TComponent* Owner)
    : TForm(Owner)
{
    ActPrint->ShortCut=TextToShortCut("Alt+G");
    ActEdit->ShortCut=TextToShortCut("Enter");
    ActRead->ShortCut=TextToShortCut("Space");
    ActS->ShortCut=TextToShortCut("Alt+C");
    ActHist->ShortCut=TextToShortCut("Alt+B");
    ActShowTiedBase->ShortCut=TextToShortCut("Alt+,");

    if (GC::ReadOnly)
    {
        ClrAction(ActDel);
    }

    FormName="OPERVIEW";
    SortTxt=" order by O.OP_DATE, O.OP_ID, O.OP_SUBID ";
    DSet->UpdatingTable=BaseUserName+".OPERS ";
    ShowTiedBase=false;
}
//---------------------------------------------------------------------------
void TFrmOperListRoot::Init(int _post,int _del,int _bound,AnsiString _text,int _allowed)
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
    DSet->SetVariable("BEG_DATE",Beg) ;
    DSet->SetVariable("END_DATE",End) ;
    DSet->SetVariable("POST",post) ;
    if (bound)
    {
        DSet->DeclareVariable("OP_ID",otInteger) ;
        DSet->SetVariable("OP_ID",bound) ;
    }

    HelpContext= bound ? 14 : (del ? 97 : (post ? 98 : (allowed ? 101 :6))) ;


    AnsiString tname= del ? "del_opers" : "opers" ,
               tiname= del ? "del_op_items" : "op_items" ,
               delstr= del ? "Del" : "" ,
               boundstr= bound ? " AND op_id=:op_id " : "",
               allow_tabstr= allowed ? (","+BaseUserName+".privtest ") : AnsiString("") ,
               allow_condstr= allowed ? " AND UNAME=USER AND O.cond_id=POID AND PR0>0 " : "" ;

    AnsiString OrderBy = bound ? AnsiString("OP_DATE, CI.ORD") :
    (G.ur.sort_num ? AnsiString("OP_DATE, DOC_TYPE, "+BaseUserName+".NormChar(DOC_NUM, 30) ") :
    AnsiString("OP_DATE, OP_ID, OP_SUBID"));
    AnsiString AddConds1;
    AnsiString AddConds2;
    if (bound)
    {
        AddConds1=", "+BaseUserName+".CONDS CI ";
        AddConds2=" and O.COND_ID=CI.COND_ID (+) ";
    }
    else
    {
        AddConds1 = " ";
        AddConds2 = " ";
    }

    DSet->SQL->Text="select O.rowid, O.* , "+BaseUserName+".Get"+delstr+"OperSum(op_id,op_subid) as SUMMA,"
                      +BaseUserName+".Get"+delstr+"OperAcc('D',op_id,op_subid) as DACCS,"
                      +BaseUserName+".Get"+delstr+"OperAcc('K',op_id,op_subid) as CACCS "
                      "from "+BaseUserName+"."+tname+" O "+allow_tabstr+AddConds1+
                      "where status=1 AND post=:post "
                      "AND op_date>=:beg_date AND op_date<:end_date "+boundstr+
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
    if (G.ur.set_to_end)
      DSet->Last();

}
//---------------------------------------------------------------------------
void TFrmOperListRoot::FormTitle()
{
 AnsiString Cap ;
 if (bound)
 {
  Cap= "Группа операций к операции "+bound_parent ;
 }
 else
 {
  Cap= allowed ? "Разрешенные операции" : (del ? (post ? "Удаленные отложенные операции" : "Удаленные операции") : (post ? "Отложенные операции" : "Журнал операций")) ;
  Cap+=" ["+Beg.DateString()+" - "+End.DateString()+"]";
 }
 Caption=Cap ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperListRoot::FormCreate(TObject *Sender)
{
 //
    Top=2;
    Height=AmsWinC->ClientBottom_()-Top-50;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperListRoot::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    AmsWinC->FindActiveMDIChild();
    AccsToWin.Del(this);
    Action = caFree;
    LastForm=NULL;
}
//---------------------------------------------------------------------------
//extern void OutSQL (TOracleDataSet* OQ, const AnsiString& Prompt,
//            const char* FileName=NULL);
//---------------------------------------------------------------------------
void __fastcall TFrmOperListRoot::ActDelExecute(TObject *Sender)
{
    if (Grid1->SelectedRows->Count>1)
    {
        if (AskMessage("Удалить "+IntToStr(Grid1->SelectedRows->Count)+
            " выделенных операций")==ID_NO) return;
        {
            for (int i = 0; i< Grid1->SelectedRows->Count; i++)
            {
                DSet->GotoBookmark((void *)Grid1->SelectedRows->Items[i].c_str());
                try {
                    try {
                        DSet->Delete();
                    }
                    catch (Exception& E){
                       // ErrorMessage(E.Message);
                    }
                }
                catch (TAmsException &e)
                {
                    if (e.IconType!=-1)
                        ErrorMessage(e.Message) ;
                }

            }
            Grid1->SelectedRows->Clear();
            AccsToWin.Refresh();
        }
    }
    else
    {
        if (AskMessage("Удалить операцию")==ID_NO) return  ;

        try {
            try {
                DSet->Delete();
            }
            catch (Exception& E)
            {
                OutSQL(DSet, "\r\n"+ E.Message+"\r\n------------\r\n");
                throw;
            }
            AccsToWin.Refresh();
        }
        catch (TAmsException &e)
        {
         if (e.IconType!=-1)
           ErrorMessage(e.Message) ;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperListRoot::DSetApplyRecord(TOracleDataSet *Sender,
      char Action, bool &Applied, AnsiString &NewRowId)
{
    if (Action == 'D')
    {
       DelOper(DSet->FieldByName("OP_ID")->AsInteger,
               DSet->FieldByName("OP_SUBID")->AsInteger);
       Applied=true ;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperListRoot::ActHistExecute(TObject *Sender)
{
    TFrmOperHist* F;
    Application->CreateForm(__classid(TFrmOperHist), &F);
    F->ShowHist(DSet->FieldByName("OP_ID")->AsInteger,
                DSet->FieldByName("OP_SUBID")->AsInteger);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperListRoot::ActPerExecute(TObject *Sender)
{
 TFrmPeriod* F ;
 Application->CreateForm(__classid(TFrmPeriod), &F);
 F->SetInitPeriod(Beg,End) ;

 int res=F->ShowModal();
 if (res==1)
 {
  Beg=F->DSet->FieldByName("POINT_DATE")->AsDateTime ;
  End=F->DSet1->FieldByName("END_DATE")->AsDateTime ;
  DSet->SetVariable("BEG_DATE",Beg) ;
  DSet->SetVariable("END_DATE",End) ;

  FormTitle() ;
  DSet->Refresh() ;
 }
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperListRoot::ActSExecute(TObject *Sender)
{
 TFrmOperListTiedOpers* F;
 Application->CreateForm(__classid(TFrmOperListTiedOpers), &F);

 AnsiString Inf=DSet->FieldByName("DOC_TYPE")->AsString+" "+DSet->FieldByName("DOC_NUM")->AsString ;
 int OpId =DSet->FieldByName("OP_ID")->AsInteger;
 F->Init(0,0, OpId,Inf);
 F->Show();
}
//---------------------------------------------------------------------------


void EditOper(int op_id,int op_subid,int del,int readonly)
{
 if (!readonly)
 {
  AmsQuery Q ;
  Q.SetPLText("*.PREPARE_EDIT_OPER(:OP_ID,:OP_SUBID,:RES,:UNAME);") ;
  Q.DeclVar("OP_ID",op_id) ;
  Q.DeclVar("OP_SUBID",op_subid) ;
  Q.DeclVarInt("RES") ;
  Q.DeclVarStr("UNAME") ;
  Q.Exec("Prepare to edit") ;

  if (Q.GetVarInt("RES")==2)
  {
   if (AskMessage("Операция редактируется пользователем "+Q.GetVarStr("UNAME")+". Просмотреть?")==ID_NO)
     return ;
   readonly=1 ;
  }
 }

 TFrmInputOper* F;
 Application->CreateForm(__classid(TFrmInputOper), &F);

 F->Init(op_id,op_subid,del,readonly);
 F->Show();
}

void NewOper()
{
 TFrmInputOper* F;
 Application->CreateForm(__classid(TFrmInputOper), &F);

 F->Init(0,0,0,0);
 F->Show();
}


void __fastcall TFrmOperListRoot::ActEditExecute(TObject *Sender)
{
 int op_id=DSet->FieldByName("OP_ID")->AsInteger ,
     op_subid=DSet->FieldByName("OP_SUBID")->AsInteger ;

 EditOper(op_id,op_subid,del,GC::ReadOnly);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperListRoot::ActReadExecute(TObject *Sender)
{
 int op_id=DSet->FieldByName("OP_ID")->AsInteger ,
     op_subid=DSet->FieldByName("OP_SUBID")->AsInteger ;

 EditOper(op_id,op_subid,del,1);
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperListRoot::ActNewExecute(TObject *Sender)
{
    AmsWinC->ActInpOper->Execute();
}
//---------------------------------------------------------------------------
void TFrmOperListRoot::KeyEvent (WORD &Key, TShiftState Shift)
{
    if (Key==13)
    {
        Key=0;
        ActEdit->Execute();
    }
    else
    if  (Key==32)
    {
        Key=0;
        ActRead->Execute();
    }
    else
    if (Key==67)    // Alt+C
    {
        if (Shift.Contains(ssAlt))
        {
            Key=0;
            ActS->Execute();
        }
    }
    else
    if (Key==66)    // Alt + И
    {
        if (Shift.Contains(ssAlt))
        {
            Key=0;
            ActHist->Execute();
        }
    }
};
//---------------------------------------------------------------------------
void __fastcall TFrmOperListRoot::Grid3KeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    KeyEvent(Key,Shift);
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperListRoot::Grid1DblClick(TObject *Sender)
{
    ActEdit->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperListRoot::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if (ActiveControl==Grid1)
    {
        KeyEvent(Key,Shift);
    }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
AnsiString TFrmOperListRoot::FormFindSql(DS_Q& Q)
{
    Q.DeclVar("BEG_DATE", Beg);
    Q.DeclVar("END_DATE", End);
    return "select O.OP_ID, O.OP_SUBID from *.OPERS O "
        "where O.OP_DATE>=:BEG_DATE and O.OP_DATE<:END_DATE and O.STATUS=1 ";
}
//---------------------------------------------------------------------------
void TFrmOperListRoot::FindOk()
{
    DS_Q Q(FindQ);
    Q.DelVars();
    AnsiString WhereAdd="";
    FindSetted=false;
    if (FilterSetted)
        FrmFilter->FormWhereCond(WhereAdd,Q,true);
    if (FrmFind->FormWhereCond(WhereAdd, Q, true))
    {
        Q.SetSqlText(FormFindSql(Q)+WhereAdd+SortTxt);
        ExecSQL(FindQ, "Пользовательский поиск");
        FindSetted=true;
        FindAgain();
    }
};
//---------------------------------------------------------------------------
void TFrmOperListRoot::ChangedFilter()
{
    DSet->Active=false;
    DS_Q Q(DSet);
    Q.DelVars();
    AnsiString Sql="";
    BuildFullSql(Sql, Q);
    ExecODS( DSet );
}
//---------------------------------------------------------------------------
void TFrmOperListRoot::UpdFldsDescr()
{
    for (int i =0; i< FldsDescr.Count(); i++)
    {
        SFldOpt & FO = FldsDescr[i];
        if (FO.RealName=="SIDE")
            FO.FullName="OI."+FO.RealName;
        else
        if (FO.RealName=="VALUE")
            FO.FullName="S."+FO.RealName;
        else
            FO.FullName="O."+FO.RealName;
    }
};
//---------------------------------------------------------------------------
void __fastcall TFrmOperListRoot::ActFindExecute(TObject *Sender)
{
//
    if (!FrmFind)
    {
        Application->CreateForm(__classid(TFrmFindDbPos), &FrmFind);
        AmsQuery AQ;
        AQ.SetSqlText ("begin select max(OID) into :OID from *.OBJS where NAME=:NAME; end;");
        AQ.DeclVar("NAME", FormName);
        AQ.DeclVar("OID", 0);
        AQ.Exec("Код ACCVIEW");
        FldsDescr.SetDbDescr(FormName, AQ.GetVarInt("OID"), " ");
        FldsDescr.ReadFldsDescr();
        UpdFldsDescr();
        FrmFind->Prepare(&FldsDescr, FormName);
    }

    if (FrmFind->FindDbPos())
    {
        FindOk();
    }
}
//---------------------------------------------------------------------------
void TFrmOperListRoot::FindAgain()
{
    AmsQuery AQ(FindQ);
    if (AQ.NotEof())
    {
        Variant Vals[2] ;
        Vals[0]=AQ.GetFldInt(0);
        Vals[1]=AQ.GetFldInt(1);
        Variant VecVals  = VarArrayOf(Vals, 1);
        TSearchRecordOptions SetSR;
        SetSR<<srFromBeginning;
        DSet->SearchRecord("OP_ID;OP_SUBID", VecVals, SetSR);
        AQ.Next();
    }
    else
    {
        Message("Больше записей не обнаружено");
    }
};
//------------------------------------------------------------------------
void __fastcall TFrmOperListRoot::ActFindAgExecute(TObject *Sender)
{
    FindAgain();
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperListRoot::ActSelExecute(TObject *Sender)
{
//
    if (!FrmFilter)
    {
        Application->CreateForm(__classid(TFrmFindDbPos), &FrmFilter);
        AmsQuery AQ;
        AQ.SetSqlText ("begin select max(OID) into :OID from *.OBJS where NAME=:NAME; end;");
        AQ.DeclVar("NAME", FormName);
        AQ.DeclVar("OID", 0);
        AQ.Exec("Код ACCVIEW");
        FldsDescr.SetDbDescr(FormName, AQ.GetVarInt("OID"), " ");
        FldsDescr.ReadFldsDescr();
        UpdFldsDescr();
        FrmFilter->Prepare(&FldsDescr, FormName);
    }

    if (FrmFilter->FindDbPos())
    {
        FilterSetted=true;
        ChangedFilter();
    }
}
//---------------------------------------------------------------------------
void TFrmOperListRoot::BuildFullSql(AnsiString& Sql, DS_Q& Q)
{
    AnsiString tname= del ? "del_opers" : "opers" ,
               tiname= del ? "del_op_items" : "op_items" ,
               delstr= del ? "Del" : "" ,
               boundstr= bound ? " AND op_id=:op_id " : "";

    Sql ="select O.rowid, O.* , "+BaseUserName+
                        ".Get"+delstr+"OperSum(op_id,op_subid) as SUMMA,"
                      +BaseUserName+".Get"+delstr+"OperAcc('D',op_id,op_subid) as DACCS,"
                      +BaseUserName+".Get"+delstr+"OperAcc('K',op_id,op_subid) as CACCS ";


    GetFromAndWhere(Sql, Q);
    GetOrderBy(Sql);
    Q.SetSqlText(Sql);
}
//---------------------------------------------------------------------------
void TFrmOperListRoot::GetFromAndWhere(AnsiString& Sql, DS_Q& Q)
{
    Q.DeclVar("POST", post);
    Q.DeclVar("BEG_DATE", Beg);
    Q.DeclVar("END_DATE", End);

    AnsiString  tname= del ? "del_opers O " : "opers O " ,
                allow_tabstr= allowed ? (","+BaseUserName+".privtest ") : AnsiString("") ,
                allow_condstr= allowed ? " AND UNAME=USER AND O.cond_id=POID AND PR0>0 " : "",
                boundstr= bound ? " AND O.op_id=:op_id " : "" ;

    Sql+="from "+BaseUserName+"."+tname+" "+allow_tabstr+
      "where O.status=1 AND O.post=:post "
      "AND O.op_date>=:beg_date AND O.op_date<:end_date "+boundstr+allow_condstr;
    if (FilterSetted)
    {
        FrmFilter->FormWhereCond(Sql, Q, true);
    }
}
//---------------------------------------------------------------------------
void TFrmOperListRoot::GetOrderBy(AnsiString& Sql)
{
    Sql+=SortTxt;
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperListRoot::ActSortExecute(TObject *Sender)
{
    if (!FrmSortOrder)
    {
        Application->CreateForm(__classid(TFrmSortOrder), &FrmSortOrder);
        if (!FldsDescr.Count())
        {
            AmsQuery AQ;
            AQ.SetSqlText ("begin select max(OID) into :OID from *.OBJS where NAME=:NAME; end;");
            AQ.DeclVar("NAME", FormName);
            AQ.DeclVar("OID", 0);
            AQ.Exec("Код ACCVIEW");
            FldsDescr.SetDbDescr(FormName, AQ.GetVarInt("OID"), " ");
            FldsDescr.ReadFldsDescr();
            UpdFldsDescr();
        }
        for (int i=0; i< FldsDescr.Count();i++)
        {
            SFldOpt& FO=FldsDescr[i];

            FrmSortOrder->AddStrs(FO.Alias, FO.FullName);
        }
    }
    AnsiString Res="";
    FrmSortOrder->GetCommaText(Res, false);
    if (Res!="")
    {
        SortTxt=" order by "+Res;
    }
    else
    {
        SortTxt=" order by O.OP_DATE, O.OP_ID, O.OP_SUBID ";
    }
    ChangedOrder();
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperListRoot::ActAnSortExecute(TObject *Sender)
{
    SortTxt=" order by O.OP_DATE, O.OP_ID, O.OP_SUBID ";
    ChangedOrder();
}
//---------------------------------------------------------------------------
void TFrmOperListRoot::ChangedOrder()
{
    ChangedFilter();
};
//---------------------------------------------------------------------------
void __fastcall TFrmOperListRoot::ActAllExecute(TObject *Sender)
{
    FilterSetted=false;
    ChangedFilter();
}
//---------------------------------------------------------------------------
void TFrmOperListRoot::RefreshAfterOper(bool otlozh)
{
    bool h=false;
    if (otlozh) {
      DSet->Next();
      if (DSet->Eof) {DSet->Prior(); h=true;}
    }

        int CurrOpId = DSet->FieldByName("OP_ID")->AsInteger;
        int CurrOpSubId = DSet->FieldByName("OP_SUBID")->AsInteger;
        DSet->DisableControls();
        DSet->Refresh();
        Variant Vals[2] ;
        Vals[0]=CurrOpId;
        Vals[1]=CurrOpSubId;
        Variant VecVals  = VarArrayOf(Vals, 1);
        TSearchRecordOptions SetSR;
        SetSR<<srFromBeginning;
        DSet->SearchRecord("OP_ID;OP_SUBID", VecVals, SetSR);

    if (otlozh) { if (h) DSet->Last(); else DSet->Prior();}
        DSet->EnableControls();
  //if (G.ur.set_to_end)
  // DSet->Last();
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperListRoot::ActShowTiedBaseExecute(TObject *Sender)
{
    ShowTiedBase=true;//!ShowTiedBase;
    if (ShowTiedBase)
    {
        //ActShowTiedBase->Caption="Спрятать связанную БД";
        if (!FrmShowTiedDb)
        {
            Application->CreateForm(__classid(TFrmShowTiedDb), &FrmShowTiedDb);
        }
        FrmShowTiedDb->ShowTiedDb(DSet);
    }
    else
    {
        ActShowTiedBase->Caption="Показывать связанную БД";
        if (FrmShowTiedDb)
        {
            FrmShowTiedDb->Close();
        }
    }
    BringToFront();
}
//---------------------------------------------------------------------------
void TFrmOperListRoot::ChildFormClosed(TForm* Frm)
{
    if (Frm==FrmShowTiedDb)
    {
        FrmShowTiedDb= NULL;
    }
};
//---------------------------------------------------------------------------

void __fastcall TFrmOperListRoot::Button1Click(TObject *Sender)
{
    //
    TMAScroll->Enabled=false;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFrmOperListRoot::TMAScrollTimer(TObject *Sender)
{
//
    if (TMAScroll->Enabled)
    {
        TMAScroll->Enabled=false;
        TMAScroll->Interval=750;
        AmsQuery AQ;
        if (DSet->Active)
        {
            AQ.SetSqlText("select *.GetDbLine( :OP_ID, :OP_SUBID) from DUAL");
            AQ.DeclVarInt("OP_ID", DSet->FieldByName("OP_ID")->AsInteger);
            AQ.DeclVarInt("OP_SUBID", DSet->FieldByName("OP_SUBID")->AsInteger);
            AQ.Exec("Выбор данных о связанной БД");
            if (AQ.NotEof())
            {
                Label1->Caption="Информация о текущей проводке: "+AQ.GetFldStr(0);
            }
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperListRoot::DSetAfterScroll(TDataSet *DataSet)
{
//
    TMAScroll->Enabled=false;
    TMAScroll->Interval=1000;
    TMAScroll->Enabled=true;
    Label1->Caption="Информация о текущей проводке";
}
//---------------------------------------------------------------------------


void __fastcall TFrmOperListRoot::FormActivate(TObject *Sender)
{
    LastForm=this;
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperListRoot::ActPrintExecute(TObject *Sender)
{
     BeginExecReport("Журнал");
}
//---------------------------------------------------------------------------

void __fastcall TFrmOperListRoot::Grid1GetCellParams(TObject *Sender,
      TField *Field, TFont *AFont, TColor &Background, bool Highlight)
{
//-----------------------------CH Storno 
    if (DSet->FieldByName("STORNO")->AsInteger)
    {
       AFont->Color=Highlight? clLime : clRed;
       AFont->Style<<fsBold;
       if (!Highlight) Background=clYellow;
    }
//-----------------------------EO CH
}
//---------------------------------------------------------------------------


