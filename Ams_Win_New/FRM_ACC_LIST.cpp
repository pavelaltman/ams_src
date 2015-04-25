//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_ACC_LIST.h"
#include "DAT_MOD.h"
#include "FRM_CHOOSE_FORM.h"
#include "FRM_BROWS_AN.h"
#include "FRM_BRWS_ACC_WINDOW.h"
#include "FRM_POST_OPERS.h"
#include "FRM_ANAL_SETUP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AMS_TREE_VIEW"
#pragma link "AmsAccsView"
#pragma link "Oracle"
#pragma resource "*.dfm"
TFrmAccList *FrmAccList;

extern void ClrAction( TAction* );
extern void SetAction( TAction* );
extern bool HaveRole(const AnsiString& RoleName, TOracleQuery* OQ);


//---------------------------------------------------------------------------
__fastcall TFrmAccList::TFrmAccList(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFrmAccList::FormCreate(TObject *Sender)
{
    if (DatMod->OSession->Connected)
    {
        AccsView->Begin(BaseUserName,-1);
        EdAccCode->Width=Width-7 ;
        Width=250;

        ClrAction(ActBegRems);
        if (!GC::ReadOnly)
        {
            AmsQuery AQ;
            if(
                HaveRole(BaseUserName+"SUPER", AQ())||
                HaveRole(BaseUserName+"_INPREMS", AQ())
            )
            {
                extern void SetAction( TAction* );
                SetAction (ActBegRems);
            }
        }
        //----------------CH
        if (GC::AccsFullExpand) {
          WindowState=wsMaximized;
          AccsView->FullExpand();
          AccsView->Selected=AccsView->Items->GetFirstNode();
        }        //EO CH
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmAccList::EdAccCodeChange(TObject *Sender)
{
    int i = 0;
    while (i< AccsView->Items->Count &&
        AccsView->Items->Item[i]->Text.Pos(EdAccCode->Text)!=1) i++;
    if (i< AccsView->Items->Count)
    {
        AccsView->Selected = AccsView->Items->Item[i];
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmAccList::ActExecFormExecute(TObject *Sender)
{
//
    TFrmChooseForm* ChooseForm;
    Application->CreateForm(__classid(TFrmChooseForm), &ChooseForm);
    int AccCode = AccsView->GetCurrCode();
    ChooseForm->BeginForm(AccCode);
}
//---------------------------------------------------------------------------

void __fastcall TFrmAccList::ActShowRemsExecute(TObject *Sender)
{
//
    TFrmBrowsAn::BegMode BM;
    int Rem = 0;
    int AnalId=0;
    BM.AccCode= AccsView-> GetCurrCode();
    BM.ReadOnly=true;
    do{
        AmsQuery AQ;
        AQ.SetSqlText(
            "begin "
            "select max(ANAL_ID) into :ANAL_ID "
            "from *.ACCS where CODE=:CODE; :RI:=*.RI(:CODE);commit;end;");
        AQ.DeclVarInt("CODE", BM.AccCode);
        AQ.DeclVarInt("RI", 0);
        AQ.DeclVarInt("ANAL_ID", 0);
        AQ.Exec("Находим кол остатка");
        Rem= AQ.GetVarInt("RI");
        AnalId= AQ.GetVarInt("ANAL_ID");
    }while(0);

    TFrmBrowsAn* Frm;
    Application->CreateForm(__classid(TFrmBrowsAn), &Frm);
    Frm->Begin(NULL, AnalId, ""," ","Остаток счета", Rem, 0, BM );
}
//---------------------------------------------------------------------------
void __fastcall TFrmAccList::ActAccViewExecute(TObject *Sender)
{
//
    if (!AccsView->Selected)
    {
        return;
    }
    if (AccsView->GetCurrCode())
    {
        TFrmBrwsAccWindow * Frm;
        Application->CreateForm(__classid(TFrmBrwsAccWindow), &Frm);
        Frm->Begin(AccsView->GetCurrCode(), AccsView->Selected->Text);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmAccList::AccsViewDblClick(TObject *Sender)
{
    ActAccView->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TFrmAccList::Button1Click(TObject *Sender)
{
    ActAccView->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TFrmAccList::ActOtlOperExecute(TObject *Sender)
{
    if (AccsView->GetCurrCode())
    {
        TFrmPostOpers* Frm;
        Application->CreateForm(__classid(TFrmPostOpers), &Frm);
        Frm->Begin(AccsView->GetCurrCode(), AccsView->Selected->Text);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmAccList::ActUstAnalExecute(TObject *Sender)
{
//
    int AccCode= AccsView->GetCurrCode();
    if (AccCode)
    {
        int AnalId=0;
        do {
        AmsQuery AQ;
        AQ.SetSqlTextReady("select ANAL_ID from "+BaseUserName+".ACCS where CODE=:CODE");
        AQ.DeclVar("CODE", AccCode);
        AQ.Exec("");
        if (AQ.NotEof())
        {
            AnalId=AQ.GetFldInt(0);
        }
        }
        while (0);
        if (AnalId)
        {
            TFrmAnalSetup* Frm;
            Application->CreateForm(__classid(TFrmAnalSetup), &Frm);
            Frm->AnalSetup (AccCode,AnalId);
        }
        else
        {
            ErrorMessage("По счету не ведется аналитический учет");
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmAccList::ActBegRemsExecute(TObject *Sender)
{
    AmsQuery AQ;
    SBrwsAccWindowOptions CurrOpt;
    AQ.SetSqlText(
        //        0         1       2         3         4         5           6
        "select A.NAME, A.DESCR, O.NAME, A.ANAL_ID, O.PARENT, A.BAL_FLD, "
        //     6          7
            "A.SETNAME, A.NODE from *.ACCS A, *.OBJS O "
        "where A.CODE=:CODE and A.ANAL_ID = O.OID (+)" );
    AQ.DeclVar("CODE", AccsView->GetCurrCode());
    AQ.Exec("Узнаем информацию о счете");
    AnsiString InpCaption= "Ввод остатка на начало по счету "+
    AccsView->Selected->Text;
    CurrOpt.AccCode=AccsView->GetCurrCode();
    int Node =0;
    if (AQ.NotEof())
    {
        CurrOpt.AnName= AQ.GetFldStr(2);
        CurrOpt.AnCode= AQ.GetFldInt(3);
        CurrOpt.DbCode= AQ.GetFldInt(4);
        CurrOpt.BalFld= AQ.GetFldStr(5);
        CurrOpt.SetName= AQ.GetFldStr(6);
        Node= AQ.GetFldInt(7);
    }
    if (Node)
    {
        Message(InpCaption+" невозможен\r\n Счет имеет субсчета");
        return;
    }
    AQ.Clear();
    AQ.SetSqlText("begin SELECT end_val_id INTO :sum_id FROM *.rems WHERE code=:sum_id"
            " AND rem_date=(SELECT MIN(point_date) FROM *.cps); end;");
    AQ.DeclVar("SUM_ID", CurrOpt.AccCode);
    AQ.Exec("Узнаем код старого остатка по счету");
    int OldId = AQ.GetVarInt("SUM_ID");

    if (CurrOpt.AnName!="")  // Вводим аналитический остаток
    {
        AQ.Clear();
        AQ.SetSqlText(
            "begin "
                "*.CreateSum(:NEW_ID, 0);"
                "*.CopyAnalSum(:ANAL_ID,:NEW_ID,:OLD_ID);"
                "commit;"
            "end;");
        AQ.DeclVarInt("NEW_ID");
        AQ.DeclVar("OLD_ID", OldId);
        AQ.DeclVar("ANAL_ID", CurrOpt.AnCode);
        AQ.Exec("Копируем существующий остаток");
        int EdSumId=AQ.GetVarInt("NEW_ID");
        TFrmBrowsAn* Frm;
        TFrmBrowsAn::BegMode BM;

        BM.AccCode=CurrOpt.AccCode;
        BM.BalFld=CurrOpt.BalFld;
        BM.NeedDistr=true;

        Application->CreateForm(__classid(TFrmBrowsAn), &Frm);
        SApplyEditRem ER;
        ER.AccCode=CurrOpt.AccCode;
        ER.SumId=EdSumId;
        WaitForApply[Frm]=ER;
        Frm->Begin(this,CurrOpt.AnCode, CurrOpt.AnName, CurrOpt.SetName,InpCaption, EdSumId,0,BM);//EO CH
    }
    else // Вводим синтетический остаток
    {
        AQ.Clear();
        AQ.SetPLText("select VALUE into :VALUE from *.SUMS where SUM_ID=:SUMID;");
        AQ.DeclVarSum("VALUE");
        AQ.DeclVar("SUMID", OldId);
        AQ.Exec("Выбираем значение старого синт остатка");
        Sum OldOstS= AQ.GetVarSum("VALUE");
        double OldOst=OldOstS;
        if (AskSum(InpCaption, OldOst))
        {
            AQ.Clear();
            AQ.SetSqlText(
                "begin "
                    "*.CreateSum(:NEW_ID, :VALUE); "
                    "*.bookkern.ChangeBegVal(:ACC_CODE,:NEW_ID);"
                    "commit;"
                "end;");
            AQ.DeclVarSum("VALUE", OldOst);
            AQ.DeclVarInt("NEW_ID");
            AQ.DeclVar("ACC_CODE", CurrOpt.AccCode);
            AQ.Exec("Вводим синтетический остаток на начало");
        }
    }
}
//---------------------------------------------------------------------------
void TFrmAccList::AnRemInputOk(TFrmBrowsAn* Frm)
{
    SApplyEditRem& S= WaitForApply[Frm];
    if (S.AccCode)
    {
        AmsQuery AQ;
        AQ.SetSqlText(
            "begin "
                "*.bookkern.ChangeBegVal(:ACC_CODE,:NEW_ID);"
                "commit;"
            "end;");
        AQ.DeclVar("NEW_ID",    S.SumId);
        AQ.DeclVar("ACC_CODE",  S.AccCode);
        AQ.Exec("Вводим аналитический остаток на начало");
    }
    WaitForApply.erase(Frm);
}
//---------------------------------------------------------------------------
void TFrmAccList::AnRemInputBad(TFrmBrowsAn* Frm)
{
    SApplyEditRem& S= WaitForApply[Frm];
    if (S.AccCode)
    {
        AmsQuery AQ;
        AQ.SetSqlText("begin delete from *.SUMS where SUM_ID=:SUMID;commit;end;");
        AQ.DeclVar("SUMID", S.SumId);
        AQ.Exec("");
    }    
    WaitForApply.erase(Frm);
}
//---------------------------------------------------------------------------

void __fastcall TFrmAccList::ActRecalcLevExecute(TObject *Sender)
{
    AmsQuery AQ;
    AQ.SetSqlText("begin *.RefreshAccs; commit; end;");
    AQ.Exec("Пересчитываем уровни счетов");
    Message("Уровни счетов пересчитаны");
}
//---------------------------------------------------------------------------

void __fastcall TFrmAccList::AccsViewKeyPress(TObject *Sender, char &Key)
{
    if ( Key >= '0' && Key<='9')
    {
        EdAccCode->SetFocus();
        char Arr[2];
        Arr[0]=Key;
        Arr[1]=0;
        EdAccCode->Text=Arr;
        EdAccCode->SelStart=2;
        EdAccCode->SelLength=0;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmAccList::Button2Click(TObject *Sender)
{
//
        EdAccCode->SetFocus();
        EdAccCode->Text="";
}
//---------------------------------------------------------------------------
void __fastcall TFrmAccList::ActRefreshExecute(TObject *Sender)
{
//
    AccsView->Begin(BaseUserName,-1);
}
//---------------------------------------------------------------------------

void __fastcall TFrmAccList::ActExpandAllExecute(TObject *Sender)
{
   AccsView->FullExpand();
}
//---------------------------------------------------------------------------

void __fastcall TFrmAccList::ActCollapseAllExecute(TObject *Sender)
{
  AccsView->FullCollapse();
}
//---------------------------------------------------------------------------

