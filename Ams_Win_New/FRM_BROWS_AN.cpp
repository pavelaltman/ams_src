//---------------------------------------------------------------------------
#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_BROWS_AN.h"
#include "AMS_QUERY.h"
#include "FRM_FOOTER_VISIBLE.h"
#include "FRM_INPUT_OPER.h"
#include "FRM_ASK_ACC_ANAL.h"
#include "FRM_ASK_AN_SUM_TYPE.h"
#include "FRM_PERIOD.h"
#include "amsclnt.h"
#include "FRM_SEL_CPS_DATE.h"
#include "FRM_CHOOSE_OBJ_MODAL.h"
#include "ams_win_c.h"
#include "FRM_CHOOSE_OPER.h"
#include "REFRESH_ACCS.h"
#include "FRM_FILTER_FORM.h"
#include "FRM_ACC_LIST.h"
#include "FRM_ASK_DB_AN_FLD.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AMS_TREE_VIEW"
#pragma link "AmsDbCatView"
#pragma link "AmsDBGrid"
#pragma link "FRM_BROWS_DB"
#pragma link "Oracle"
#pragma link "OracleData"
#pragma link "RXDBCtrl"
#pragma link "AmsFooter"
#pragma resource "*.dfm"
TFrmBrowsAn *FrmBrowsAn;
bool first=true;
//---------------------------------------------------------------------------
void DistrLine (TFldsDescr* FldsDescr,
                int AnSumEd, int AnSumOst,
                TOracleDataSet* ODS_, // DS без версий (на все версии 1 строка)
                const AnsiString& SpisOrder)  // Указатель на FSM, который будем использовать для пересчета каждой строки
{
    FieldSetVerDistr DistrVer(FldsDescr, ODS_);
    DistrVer.SetCurrArt(ODS_->FieldByName("ARTICLE")->AsString);
    DistrVer.SetParams(AnSumEd, AnSumOst, SpisOrder);
    DistrVer.Build();
    AmsQuery AQOst;
    AQOst.SetSqlTextReady("delete from "+BaseUserName+"."+FldsDescr->GetAnName()+
        " where SUMID=:SUMID and AARTICLE=:AARTICLE");
    AQOst.DeclVar("SUMID", AnSumEd);
    AQOst.DeclVar("AARTICLE", DistrVer.GetCurrArt());
    AQOst.Exec("Удаляем то что уже есть в аналит сумме");
    AQOst.Clear();
    AmsQuery AQIns;
    DistrVer.PrepareInsertQuery(AQIns);
    AQIns.DeclVarStr("AARTICLE", DistrVer.GetCurrArt());
    if (DistrVer.PrepareOstQuery(AQOst))
    {
        AQOst.DeclVar("AARTICLE", DistrVer.GetCurrArt());
        AQOst.Exec("Остаток по позиции");
        int OstKol=DistrVer.GetVersQPlus(DistrVer.GetCurrArt());
        bool More=true;
        if (OstKol>0){
          OstKol=DistrVer.GetVersQ(DistrVer.GetCurrArt());
          while (More && AQOst.NotEof())
          {
              if (OstKol<=1)
              {
                  AQIns.SetVar("AVER", AQOst.GetFldInt(1));
                  AQIns.SetVar("AARTICLE", DistrVer.GetCurrArt());
                  DistrVer.PutOst(AQIns, false);
                  More = false;
              }
              else
              {
                  More=DistrVer.HaveOst(AQOst, AQIns);
                  AQOst.Next();
                  OstKol--;
              }
          }
        }
        if (More)
        {
            DistrVer.PutOst(AQIns, true);
        }
    }
    else
    {
        DistrVer.PutOst(AQIns, true);
    }
    // Пересчет по каждой линии
}
//---------------------------------------------------------------------------
void TFrmBrowsAn::DistrLine2()
{
}
//---------------------------------------------------------------------------
OpVars* TFrmBrowsAn::GetOpVars(){return Vars->VARS();};
//---------------------------------------------------------------------------
__fastcall TFrmBrowsAn::TFrmBrowsAn(TComponent* Owner)
    : TFrmBrowsDb(Owner),
        TotalAll(&FldsDescr, ODS),
        TotalShowLines(&FldsDescr, ODS),
        TotalCurrCat(&FldsDescr, ODS),
        TotalCatTree(&FldsDescr, ODS),
        TotalSelected(&FldsDescr, ODS),
        SetTotalNow(true),FreeExecuted(false), FilterForm(0), Dirty(false)
{
    ActVisibilityMode->ShortCut=TextToShortCut("Alt+H");
    ActOperVars->ShortCut=TextToShortCut("Alt+U");
    ActFilter->ShortCut=TextToShortCut("Alt+A");
    ActDocGen->ShortCut=TextToShortCut("Alt+J");
    ActFormGen->ShortCut=TextToShortCut("Alt+V");
    ShtrixCodeCounter=0;
    /*
    ->ShortCut=TextToShortCut("Alt+");
    ->ShortCut=TextToShortCut("Alt+");
    */

}
//---------------------------------------------------------------------------
void TFrmBrowsAn::Begin(TForm* Parent_,int AnCode, const AnsiString& AnName,
        const AnsiString& SetName, const AnsiString& Descr,
        int AnSumEd_, int AnSumOst_, const BegMode& BM )
{
    PrepareForm(Parent_,AnCode,AnName,SetName,Descr,AnSumEd_,AnSumOst_,BM );
}
//---------------------------------------------------------------------------
void TFrmBrowsAn::PrepareForm(TForm* Parent_,int AnCode, const AnsiString& AnName,
        const AnsiString& SetName, const AnsiString& Descr,
        int AnSumEd_, int AnSumOst_, const BegMode& BM )
{
    jkl=0;//EO CH
    BM1=BM;
    ODS1 = new TOracleDataSet (this);
    ODS1->Session= DatMod->OSession;
    ODS1->OnApplyRecord=ODSApplyRecord;

    TFrmInputOper* Frm=dynamic_cast<TFrmInputOper*> (Parent_);
    if (Frm)
    {
        ActOperVars->Visible=true;
    }
    else
    {
        ActOperVars->Visible=false;
    }

    ExistFooterChecked=false;
    ParentF= Parent_;
    AnSumEd=AnSumEd_;
    AnSumOst=AnSumOst_;
    AccCode = BM.AccCode;
    BalFld=BM.BalFld;
    if (BM.NoVerOrd=="")
        NoVerOrd=" D.VER ";
    else
        NoVerOrd=BM.NoVerOrd;

    CheckNeg=BM.CheckNeg;       // Проверять на отрицательность остатка?
    if (CheckNeg)
    {
        AddToRem=BM.AddToRem;       // По дебету или кредиту операция
        PredSumId=BM.PredSumId;       // Сумма которая была ранее
    }


    ZeroVer=BM.ZeroVer;
    EdMode = BM.EdMode;
    WithoutVer= !BM.ShowVer;
    WithoutCats= BM.WithoutCat;
    NeedDistr=BM.NeedDistr;
    FooterCntrl.SetDistr(NeedDistr);

    Vars=BM.Vars;
    BalMask=",0.00";

    ReadOnly=0;
    if (BM.ReadOnly)
    {
        ReadOnly = eoReadOnly;
        ActDel->Enabled=false;
        ActDel->Visible=false;
        ActNew->Enabled=false;
        ActNew->Visible=false;
        ActCopy->Enabled=false;
        ActCopy->Visible=false;
        ActCalc->Enabled=false;
        ActCalc->Visible=false;
        ActDelPos->Enabled=false;
        ActDelPos->Visible=false;
        ActClearAn->Enabled=false;
        ActClearAn->Visible=false;
        ActTakeAnSum->Enabled=false;
        ActTakeAnSum->Visible=false;
        ActDestr->Enabled=false;
        ActDestr->Visible=false;
        ActCalcWithClear->Enabled=false;
        ActCalcWithClear->Visible=false;
        ActCalcVisible->Enabled=false;
        ActCalcVisible->Visible=false;
        ActClaclVisibleWithClear->Enabled=false;
        ActClaclVisibleWithClear->Visible=false;
        BBOk->Visible=false;
        BBOk->Enabled=false;
    }

    if (SetName == "" || SetName == " ")
        Caption = Descr;
    else
        Caption = Descr+" ["+SetName+"]";
    CurrSetName =SetName;

    AmsQuery AQ (OQ);

    // Узнаем код и название базы данных и аналитики
    AQ.Clear();
    if (AnName!="")
    {
        AQ.SetSqlText ("select OID, NAME from *.OBJS "
                "start with NAME = :NAME and OTYPE = 5 connect by prior PARENT = OID");
        AQ.DeclVar("NAME", AnName);
    }
    else
    {
        AQ.SetSqlText( "select OID, NAME from *.OBJS "
                "start with OID = :OID and OTYPE = 5 connect by prior PARENT = OID");
        AQ.DeclVar("OID", AnCode);
    }
    AQ.Exec("");
    if (AQ.NotEof())
    {
        CurrAnId = AQ.GetFldInt(0);
        CurrAnName = AQ.GetFldStr(1);
        AQ.Next();
        CurrDBId = AQ.GetFldInt(0);
        CurrDBName = AQ.GetFldStr(1);
    }
    //---------------------------------------------------------------
    // Проверяем был ли указан начальный артикул+ версия
    ArtVerSel=false;
    if (BM.BegArt!="")
    {
        AQ.Clear();
        AQ.SetSqlTextReady(
            "select CATID from "+BaseUserName+"."+CurrDBName+
            " where ARTICLE=:ARTICLE and VER=:VER");
        AQ.DeclVar("ARTICLE", BM.BegArt);
        AQ.DeclVar("VER", BM.BegVer);
        AQ.Exec("Находим категорию для выбранного артикула и версии");
        if (AQ.NotEof())
        {
            ArtSel=BM.BegArt;
            VerSel=BM.BegVer;
            CatIdSel=AQ.GetFldInt(0);
            ArtVerSel=true;
        }
    }
    //--------------------------------------------------------------
    // Определяем к-во позиций в аналитике
    AQ.Clear();
    AQ.SetSqlTextReady("select count(*) from "+BaseUserName+"."+CurrAnName+
            " where SUMID=:SUMID");
    AQ.DeclVar("SUMID", AnSumEd);
    AQ.Exec("Количество позиций в аналитике");
    TotalPos = AQ.GetFldInt(0);
    if (TotalPos)
    {
        TotalPos--;
        if (TotalPos)
        {
            EdMode = asmSelPos;
            WithoutCats= true;
            try { if (Frm->CM_Var>=0) WithoutCats=Frm->CM_Var; } catch (...) {;}
            if (Visible)
                DBG->SetFocus();
            else
                Panel1->TabOrder=0;
        }
    }
    if (BM.ShtrixFld!="")
    {
        ShtrixCodeMode=true; // Включен ли режим ввода данных по штрих-коду
        ShtrixFld=BM.ShtrixFld;

        ShtrixOrderFld=BM.ShtrixOrderFld;
        EdShtrixCode->Enabled= true;
        EdShtrixCode->Visible= true;
        AQ.SetSqlText("select max("+ShtrixOrderFld+") from *."+CurrAnName+
            " where SUMID=:SUMID ");
        AQ.Exec("Выбираем максимальное число для порядка при штрих-коде");
        ShtrixCodeCounter=AQ.GetFldInt(0)+1;


    }
    else
    {
        ShtrixCodeMode=false; // Включен ли режим ввода данных по штрих-коду
        ShtrixFld="";
        EdShtrixCode->Enabled= false;
        EdShtrixCode->Visible= false;
    }

    if (EdMode==-1)
        EdMode=asmAllBase;

    if(ShtrixCodeMode)
    {
        SxSaveEdMode=EdMode;
        SxSaveWithoutCats=WithoutCats;
        EdMode = asmSelPos;
        WithoutCats= true;
    }
    ChangeShtrixMode=ShtrixCodeMode;
    //--------------------------------------------------------------
    CurrCatId = -1;
    ODS->DisableControls();
    RefreshCats();
    BuildCols();
    //
    TTreeNode* TN  = CV->Items->GetFirstNode();
    CV->Selected=TN;
    if (CV->Items->Count==1)
    {
        CV->Hide();
        if (!CV->Visible && Visible)
            DBG->SetFocus();
    }
    ShowMode();
    ODS->EnableControls();
    if (ArtVerSel)
    {
        SetPos(ArtSel, VerSel, CatIdSel);
        Panel1->TabOrder=0;
        ArtVerSel=false;
    }
    ViewUpdated();
    /*
    if (BM.Recalc==1)
        ActCalc->Execute();
    if (BM.Recalc==2)
        ActCalcWithClear->Execute();
    if (BM.Distr)
        ActDestr->Execute();
    */
    if (GC::ViewDebug)
    {
        Caption = Caption + " ["+CurrDBName+"/"+CurrAnName+" "+SetName+"]";
    }
    ShowBalSum();
    if (Visible && ShtrixCodeMode)
    {
        EdShtrixCode->SetFocus();
    }
}
//---------------------------------------------------------------------------
// Заполняет AllAddAnSum (Коды аналит сумм для всех доп полей)
void TFrmBrowsAn::GetAllAddAnSum ()
{
    AddAnSums.clear();
    FldsDescr.FormAddAnSums(&AddAnSums);
}
//---------------------------------------------------------------------------
void TFrmBrowsAn::BuildCols(void)
{
    ColsReady = false;

    DBG->Columns->Clear();
    int FrozenTotWidth =0;
    //int TotWidth = 0;
    int W= DBG->Canvas->TextWidth("0");
    FldsDescr.SetAnDescr(CurrDBName, CurrDBId, CurrAnName, CurrAnId,CurrSetName);
    FldsDescr.ReadFldsDescr();
    GetAllAddAnSum ();

    TField* Fld;

    BuildSqlQuery(ODS);
    PrepareODS1();

    ODS->DisableControls();
    ExecODS(ODS);

    TDBGridColumns* Cols;
    int FixedCols = 0;
    ColsIndx.clear();
    ZestFlds.clear();
    for (int i = 0 ; i< FldsDescr.Count(); i++)
    {
        ColsIndx.push_back(-1);
        if ( (FldsDescr[i].FldAddInfo==aiAn)&&(FldsDescr[i].DefOpt & ZestFld))
        {
            ZestFlds.push_back(i);
        }
    }

    // Строим замороженные колонки
    for (int i = 0 ; i< FldsDescr.Count(); i++)
    {
        SFldOpt& XX = FldsDescr[i];
        if (XX.FldAddInfo!=aiAnR || AnSumOst)
        {
            Fld = ODS->FieldByName(XX.SqlAlias);
            XX.FldNo = ODS->Fields->IndexOf(Fld);
            if (!((XX.DefOpt & Hidden) || (XX.DispOpt & Hidden)))
            {
                int AddW = XX.DiplWidth*W;
                if ((XX.DefOpt & Frozen) || (XX.DispOpt & Frozen))
                {
                    Cols=DBG->Columns;
                    FrozenTotWidth += AddW+6;

                    if (NeedDistr && (XX.DispOpt&DebetCredit) && (XX.DefOpt&DebetCredit))
                    {
                        TColumn * Col = Cols->Add();
                        Fld = ODS->FieldByName(XX.SqlAlias+"dt");
                        Col->Field= Fld;
                        Col->Title->Caption = XX.Alias+"дебет";
                        Col->Width = AddW+5;
                        FixedCols++;
                        ColsIndx[i] = FixedCols;

                        Col = Cols->Add();
                        Fld = ODS->FieldByName(XX.SqlAlias+"ct");
                        Col->Field= Fld;
                        Col->Title->Caption = XX.Alias+"кредит";
                        Col->Width = AddW+5;
                        FixedCols++;
                        ColsIndx[i] = FixedCols;
                    }
                    else
                    {
                        TColumn * Col = Cols->Add();
                        Col->Field= Fld;
                        Col->Title->Caption = XX.Alias;
                        Col->Width = AddW+5;
                        FixedCols++;
                        ColsIndx[i] = FixedCols;
                    }
                }
            }
        }
    }
    int CurrCol = FixedCols;
    for (int i = 0 ; i< FldsDescr.Count(); i++)
    {
        SFldOpt& XX = FldsDescr[i];
        if (XX.FldAddInfo!=aiAnR || AnSumOst)
        {
            Fld = ODS->FieldByName(XX.SqlAlias);
            XX.FldNo = ODS->Fields->IndexOf(Fld);
            if (!((XX.DefOpt & Hidden) || (XX.DispOpt & Hidden)))
            {
                int AddW = XX.DiplWidth*W;
                if (! ((XX.DefOpt & Frozen) || (XX.DispOpt & Frozen)))
                {
                    Cols=DBG->Columns;
                    FrozenTotWidth += AddW+6;
                    if (NeedDistr && (XX.DispOpt&DebetCredit) && (XX.DefOpt&DebetCredit))
                    {
                        TColumn * Col = Cols->Add();
                        Fld = ODS->FieldByName(XX.SqlAlias+"dt");
                        Col->Field= Fld;
                        Col->Title->Caption = XX.Alias+"дебет";
                        Col->Width = AddW+5;
                        ColsIndx[i]=++CurrCol;

                        Col = Cols->Add();
                        Fld = ODS->FieldByName(XX.SqlAlias+"ct");
                        Col->Field= Fld;
                        Col->Title->Caption = XX.Alias+"кредит";
                        Col->Width = AddW+5;
                        ColsIndx[i]=++CurrCol;
                        FrozenTotWidth += AddW+6;
                    }
                    else
                    {
                        TColumn * Col = Cols->Add();
                        Col->Field= Fld;
                        Col->Title->Caption = XX.Alias;
                        //-----------CHLIGA
                          Col->Visible=AddW;
                        //EO CHLIGA
                        Col->Width = AddW+5;
                        ColsIndx[i]=++CurrCol;
                    }
                }
            }
        }
    }

//-------------------------CH

  int CalcColumnsWidth=0;
  for (int j=0; j<jkl; j++) {
      TColumn * Col = DBG->Columns->Add();
      Col->Field= ODS->FieldByName(CALCUNIQUE+IntToStr(j));
      Col->Title->Caption = CalcFO[j].Alias;
      Col->Width = (CalcFO[j].DiplWidth*DBG->Canvas->TextWidth("0"))*3+5;
      CalcColumnsWidth+=Col->Width;
}

//-------------------------EO CH

    DBG->FixedCols=FixedCols;
    ODS->EnableControls();
    if (CV->Items->Count<2)
    {
        CV->Width=8;
    }

    FrozenTotWidth+=CV->Width+44;
    if (FrozenTotWidth<AmsWinC->Width-24)
    {
        if (AmsWinC->Width < Left+FrozenTotWidth)
        {
            Left =AmsWinC->Width-FrozenTotWidth-16;
        }
        Width = FrozenTotWidth;
    }
    else
    {
        Width=AmsWinC->Width-24;
        Left=8;
    }
    //---------CHLIGA
    Width+=CalcColumnsWidth;
    //---------EO CHLIGA

    int BegTop = AmsWinC->ToolBar1->Top+AmsWinC->ToolBar1->Height;
    int BegHeight= AmsWinC->StatusBar1->Top - BegTop-24;

    if (BegHeight< (Top+Height))
    {
        if (BegHeight<Height)
        {
            Top= BegTop;
            Height=BegHeight-Top;
        }
        else
        {
            Top=BegHeight-Height;
        }
    }

    FormatSums();
    ColsReady = true;
    PanGridResize(this);
    if (BalFld!="")
    {
        int i=0;
        int BalFldNo=-1;
        while (i<FldsDescr.Count() && BalFldNo==-1)
        {
            SFldOpt& FO= FldsDescr[i];
            if (FO.FldAddInfo==aiAn && FO.RealName==BalFld)
                BalFldNo=FO.FldNo;
            else
                i++;
        }
        TNumericField* NFld = dynamic_cast<TNumericField*>(ODS->Fields->Fields[BalFldNo]);
        BalMask=NFld->DisplayFormat;
    }

    //----------------------------------------------
    // Работа с Footerами
    //----------------------------------------------
    FooterCntrl.SetFooter(Footer, &FldsDescr);
    TotalAll.Build();
    TotalShowLines.Build();
    TotalCurrCat.Build();
    TotalCatTree.Build();
    TotalSelected.Build();
    FooterCntrl.FI.Add(&TotalAll, true);
    FooterCntrl.FI.Add(&TotalShowLines, false);
    FooterCntrl.FI.Add(&TotalCurrCat, false);
    FooterCntrl.FI.Add(&TotalCatTree, false);
    FooterCntrl.FI.Add(&TotalSelected, false);

    //-------------------------------------------
    // Получаем тотал строку для всей аналит суммы
    //--------------------------------------------
    FooterCntrl.Tie( DBG, ColsIndx);
    RecalcFooter();
    FooterCntrl.Draw();
}
//---------------------------------------------------------------------------
void TFrmBrowsAn::GetFromAndWhere (AnsiString& SelBeg, DS_Q D)
{
    SelBeg +=" from "+BaseUserName+"."+CurrDBName+" D, ";
    if (HaveCatsSort && WithoutCats)
    {
        SelBeg+=BaseUserName+".DBCATTREE CT, ";
    }
    SelBeg+=BaseUserName+"."+CurrAnName+" AN ";
    if (AnSumOst)
    {
        SelBeg+=", "+BaseUserName+"."+CurrAnName+" RAN";
    }

    // Для доп полей
    std::vector<SAddAnSumDescr>& DescrAnSum= FldsDescr.GetAddFldsDescr();
    for (unsigned int i =0; i< AddAnSums.size(); i++)
    {
        SAddAnSumDescr& D = DescrAnSum[i];
        SelBeg+=", "+BaseUserName+"."+CurrAnName+" "+D.TurnType+
                IntToStr(D.SubAccCode)+" ";
    }


    SelBeg+=" where ";
    if (HaveCatsSort && WithoutCats )
    {
        SelBeg+="CT.DBID = :DBID and CT.OWNCAT = D.CATID and ";
        D.DeclVar("DBID", CurrDBId);
    }
    if (! WithoutCats)
    {
        SelBeg+="D.CATID=:CATID and ";
        D.DeclVar("CATID", CurrCatId);
    }

    if (ArtVerSel)
    {
        SelBeg+="D.ARTICLE=:ART_SEL and D.VER=:VER_SEL and ";
        D.DeclVar("ART_SEL", ArtSel);
        D.DeclVar("VER_SEL", VerSel);
    }

    SelBeg+="D.ARTICLE!=' ' "+ GetFilterCond(D)+ " and ";
    switch (EdMode)
    {
        case asmAllBase:
            SelBeg+=" AN.SUMID(+)=:ANSUMID and D.ARTICLE=AN.AARTICLE(+) and D.VER=AN.AVER(+)";
            if (AnSumOst)
            {
                SelBeg+=" and "
                " RAN.SUMID(+)=:RANSUMID and D.ARTICLE=RAN.AARTICLE(+) and D.VER=RAN.AVER(+) ";
                D.DeclVar("RANSUMID", AnSumOst);
            }
        break;
        case asmSelPos:
            SelBeg+=" AN.SUMID=:ANSUMID and D.ARTICLE=AN.AARTICLE and D.VER=AN.AVER";
            if (AnSumOst)
            {
                SelBeg+=" and "
                " RAN.SUMID(+)=:RANSUMID and D.ARTICLE=RAN.AARTICLE(+) and D.VER=RAN.AVER(+) ";
                D.DeclVar("RANSUMID", AnSumOst);
            }
        break;
        case asmRems:
            SelBeg+=" AN.SUMID(+)=:ANSUMID and D.ARTICLE=AN.AARTICLE(+) and D.VER=AN.AVER(+)"
            " and RAN.SUMID(+)=:RANSUMID and D.ARTICLE=RAN.AARTICLE(+) and D.VER=RAN.AVER(+) and "
            "(AN.AVER is not null or RAN.AVER is not null) ";
            D.DeclVar("RANSUMID", AnSumOst);
        break;
    }
    // Для доп полей
    for (unsigned int i =0; i< DescrAnSum.size(); i++)
    {
        SAddAnSumDescr& DI = DescrAnSum[i];
        AnsiString A =DI.TurnType+IntToStr(DI.SubAccCode);

        SelBeg+=" and "+A+".SUMID(+)=:"+A+"SUMID and D.ARTICLE="+A+
            ".AARTICLE(+) and D.VER="+A+".AVER(+) ";
        D.DeclVar(A+"SUMID", AddAnSums[i]);
    }
    D.DeclVar("ANSUMID", AnSumEd);
};
//---------------------------------------------------------------------------
void TFrmBrowsAn::BuildSqlQuery(TOracleDataSet* ODS_)
{
    ODS_->Active = false;
    ODS_->DeleteVariables();
    if (!DefaultSort && HaveCatsSort )
    {
        FrmSortOrder->SetSelection(0, false);
        OrderByClause="";
        FrmSortOrder->GetCommaText(OrderByClause, true);
        if (OrderByClause == "")
            OrderByClause = "D.ARTICLE, D.VER";
        HaveCatsSort = false;
    }
    AnsiString SIGN=BM1.AddToRem?"-":"+";
    AnsiString Hs;

    if ( !WithoutVer)
    {
        AnsiString SelBeg = "select D.ROWID, AN.ROWID ANALITIC_ROWID";
        for ( int i = 0; i< FldsDescr.Count(); i++)
        {
            SFldOpt& FO = FldsDescr[i];
            if((!BM1.newop)&&(FldsDescr[i].FldAddInfo== aiAnR)) {
                  Hs=AnsiUpperCase(FldsDescr[i].FullName);
                  Hs=Hs.SubString(Hs.Pos("RAN.")+4, Hs.Length());
                  Hs="nvl(RAN."+Hs+", 0)"+SIGN+"nvl(AN."+Hs+", 0)";
                  //Hs=nvl(RAN."+Hs+SIGH+"AN."+Hs;
                  SelBeg +=", "+Hs+" "+FldsDescr[i].SqlAlias;
                } else {
              if (FO.FldAddInfo!=aiAnR || AnSumOst)
              {
                  if (NeedDistr && (FO.DispOpt&DebetCredit) && (FO.DefOpt&DebetCredit))
                  {
                      SelBeg +=", "+BaseUserName+".GetDKValue('D', "+FldsDescr[i].FullName+") "+FldsDescr[i].SqlAlias+"dt";
                      SelBeg +=", "+BaseUserName+".GetDKValue('K', "+FldsDescr[i].FullName+") "+FldsDescr[i].SqlAlias+"ct";
                  }
  //---------------------CHLIGA

                  if ((FldsDescr[i].FldAddInfo==aiAn)&&(FldsDescr[i].Type==3) && (FldsDescr[i].DBId>0))
                    if ((FldsDescr[i].DispOpt & Editable)&&(FldsDescr[i].DefOpt&Editable)){
                     SelBeg +=", "+BaseUserName+".GetAnalInfo( "+FldsDescr[i].FullName+" , "+IntToStr(FldsDescr[i].DBId)+")  "+CALCUNIQUE+IntToStr(jkl);
                     CalcFO[jkl]=FldsDescr[i];
                     FldsDescr[i].DiplWidth=0;
                     jkl++;
                  }
  //---------------------EO CHLIGA
                  SelBeg +=", "+FldsDescr[i].FullName+" "+FldsDescr[i].SqlAlias;
              }
            }
        }
        GetFromAndWhere (SelBeg, DS_Q(ODS_));
        if (ZeroVer) SelBeg+=" AND D.VER=0 ";
        //--------------------------------------------------------
        if (RecalcArticle!="")
        {
            SelBeg+=" AND AN.AARTICLE=:ARTICLE ";
            ODS_->DeclareVariable("ARTICLE", otString);
            ODS_->SetVariable("ARTICLE", RecalcArticle);
        }
        //--------------------------------------------------------
        SelBeg+=" order by " + GetOrderBy();
        ODS_->SQL->Text = SelBeg;
    }
    else
    {   // Без версий
        // Формируем строку с макс поз
        AnsiString SelMax = "select max (1) RID, max(AN.ROWID) ANALITIC_ROWID ";
        for ( int i = 0; i< FldsDescr.Count(); i++)
        {
            SFldOpt& FO = FldsDescr[i];
            if((!BM1.newop)&&(FldsDescr[i].FldAddInfo== aiAnR)) {
                  Hs=AnsiUpperCase(FldsDescr[i].FullName);
                  Hs=Hs.SubString(Hs.Pos("RAN.")+4, Hs.Length());
                  Hs="nvl(RAN."+Hs+", 0)"+SIGN+"nvl(AN."+Hs+", 0)";
                  //Hs="RAN."+Hs+SIGH+"AN."+Hs;
                  SelMax +=", sum("+Hs+") as "+FldsDescr[i].SqlAlias;
                } else {
                 if (FldsDescr[i].FldAddInfo!=aiAnR || AnSumOst)
                 {

                  if ( FldsDescr[i].RealName.UpperCase() != "ARTICLE")
                  {
                      if ((FldsDescr[i].DefOpt & Addable) && (FldsDescr[i].FldAddInfo!= aiDb))
                          SelMax +=", sum("+FldsDescr[i].FullName+") as "+FldsDescr[i].SqlAlias;
                      else
                          SelMax +=", max("+FldsDescr[i].FullName+") as "+FldsDescr[i].SqlAlias;
                  }
                  else
                      SelMax+=", ARTICLE";
                }
            }
        }

        if (WithoutCats && HaveCatsSort)
        {
            SelMax+=", max(CT.TORD) as TORD ";
        }
        GetFromAndWhere(SelMax, ODS_);
        if (ZeroVer) SelMax+=" AND D.VER=0 ";

        SelMax+=" group by D.ARTICLE order by " + GetOrderBy();
        ODS_->SQL->Text = SelMax;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsAn::ActEditAnExecute(TObject *Sender)
{
    int Flags = eoAnFlds | ReadOnly;
    CurrFlags=Flags;
    DeletePosAn=false;
    ActEdDbPosExec (Flags);
    if (DeletePosAn && EdMode==asmSelPos)
    {
        ODS->Next();
        RefreshView();
    }
    if (NeedRecalcArt)
    {
        AnsiString Art=ODS->FieldByName("ARTICLE")->AsString;
        RecalcOneArticle(Art);   
        AmsQuery AQ;
        AQ.SetSqlText("select count(*) from *."+
                CurrAnName+" where SUMID=:SUM_ID and AARTICLE!=' '");
        AQ.DeclVar("SUM_ID", AnSumEd);
        AQ.Exec("Получаем тотал значения");
        TotalPos=AQ.GetFldInt(0);
        NeedRecalcArt=false;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsAn::ActEdDbPosExecute(TObject *Sender)
{
    int Flags = eoAnFlds | eoDbFlds | ReadOnly;
    if (!Sender)
        Flags|= eoNewPos;
    CurrFlags=Flags;
    ActEdDbPosExec (Flags);

}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsAn::ActVisibilityModeExecute(TObject *Sender)
{
//
    if (ShtrixCodeMode)
    {
        Message("Переход в режим ввода по штрих-коду F11");
        ActShtrixModeOnOff->Execute();
        return;
    }
    bool Ok = false;
    while (!Ok)
    {
        EdMode++;
        if (EdMode > asmRems) EdMode = asmSelPos;
        Ok = true;
        if (EdMode == asmRems && !AnSumOst)
            Ok = false;
    }

    RefreshView();
    ShowMode();
}
//---------------------------------------------------------------------------
void TFrmBrowsAn::ShowMode()
{
    TFrmBrowsDb::ShowMode();
    AnsiString S;
    if (ShtrixCodeMode)
    {
        S="Штрих-код";
    }
    else
    {
        switch (EdMode)
        {
            case asmAllBase:
                S="Вся база";
            break;
            case asmSelPos:
                S="Выбранные позиции";
            break;
            case asmRems:
                S="Остаток";
            break;
        }
    }
    SBar->Panels->Items[5]->Text = S;

    if (ChangeShtrixMode) {
      ChangeShtrixMode=false;
      //CV->Visible=!ShtrixCodeMode;
      this->Width+=ShtrixCodeMode ? CV->Width : (-CV->Width);
      }


};
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsAn::ODSApplyRecord(TOracleDataSet *Sender,
      char Action, bool &Applied, AnsiString &NewRowId)
{
    AnsiString Sjkl;
    if (Action=='C')
    {
        Applied=true;
        return;
    }

    AmsQuery AQ;
    TOracleDataSet* ODS=Sender;
    bool FooterChanged = false;

    if (Action == 'I')
    {
        /*
        if (CurrFlags & eoDbFlds)
        {
            AnsiString S= "begin insert into "+BaseUserName+"."+CurrDBName+"(";
            if (BuildInsQuery(S, AQ, aiDb))
            {
                S+="; select ROWID into :CURR_ROWID from "+BaseUserName+"."+
                    CurrDBName+" where ARTICLE=:ARTICLE and VER=:VER; end;";
                AQ.DeclVar("CURR_ROWID", "");
                AQ.SetSqlTextReady(S);
                AQ.Exec("Вставка позиции БД");
                NewRowId= AQ.GetVarStr("CURR_ROWID");
                AQ.Clear();
            }
        }
        if (CurrFlags & eoAnFlds)
        {
            if (IncludeInSum())
            {
                AQ.Clear();
                AnsiString S= "insert into "+BaseUserName+"."+CurrAnName+"(";
                if (BuildInsQuery(S, AQ, aiAn))
                {
                    AQ.SetSqlTextReady(S);
                    AQ.Exec("Вставка позиции в аналитику");
                    TotalPos++;
                    FooterCntrl.AddLine(ODS);
                    FooterChanged = true;
                }
            }
        }
        */
        NewRowId= NewRowId_;
        Applied = true;
        Dirty=true;
    }
    else
    if (Action == 'U')
    {
        Dirty=true;
        if ((!WithoutVer ) || (NeedRecalcArt))
        {
            // (!WithoutVer) {
            if (CurrFlags & eoDbFlds)
            if (!WithoutVer) {

                    AnsiString S= "update "+BaseUserName+"."+CurrDBName +" set ";
                    if (BuildUpdQuery(S, AQ, aiDb,Sender))
                    {
                      Sjkl=ODS->RowId();
                      AQ.SetSqlTextReady(S);
                      AQ.DeclVar("CURR_ROWID", Sjkl);
                      AQ.Exec("Update позиции БД");
                    }


            } else {
                    TOracleQuery* QJ= DatMod->GetOQ();
                   try{
                      QJ->DeleteVariables();
                      Sjkl=ODS->RowId();
                      QJ->SQL->Text="select "+CurrDBName+".rowid from "+BaseUserName+"."+CurrDBName+", "+
                                     BaseUserName+"."+CurrAnName+" "+" where "+CurrAnName+".rowid = '"+
                                     Sjkl+"' and aarticle=article";
                      QJ->Execute();
                      while (!QJ->Eof) {
                        AnsiString S= "update "+BaseUserName+"."+CurrDBName +" set ";
                        if (BuildUpdQuery(S, AQ, aiDb,Sender))
                        {

                          AQ.SetSqlTextReady(S);
                          AQ.DeclVar("CURR_ROWID", QJ->FieldAsString(0));
                          AQ.Exec("Update позиции БД");
                        }
                        QJ->Next();
                      }
                   } __finally {
                      DatMod->ReleaseOQ(QJ);
                   }

            }
            AQ.Clear();
            if (CurrFlags & eoAnFlds)
            {
                // CHLIGA

                try {
                  TField* Fld = NULL;
                  bool reak=false;
                  for (int j=0;j<ODS->Fields->Count;j++) {
                    if (UpperCase(ODS->Fields->Fields[j]->FieldName)=="ANALITIC_ROWID") {
                      reak=true; break;
                    }
                  }
                  AnsiString tempory;
                  if (reak) { Fld = ODS->FieldByName("ANALITIC_ROWID"); tempory=Fld->AsString;}
                  else { Sjkl=ODS->RowId(); AQ.DeclVar("CURR_ROWID", Sjkl);
                         if (WithoutVer) goto Q14;
                       }


                // EO CHLIGA
                if (SetTotalNow)
                    FooterCntrl.SubLine(ODS, true);
                FooterChanged = true;
                if (IncludeInSum())
                {
                  if (Fld) {
                    if (Fld->IsNull)
                    {
                        ODS->FieldByName("AN_AARTICLE")->AsString = ODS->FieldByName("ARTICLE")->AsString;
                        ODS->FieldByName("AN_AVER")->AsInteger = ODS->FieldByName("VER")->AsInteger;
                        ODS->FieldByName("AN_SUMID")->AsInteger = AnSumEd;
                        AnsiString S= "insert into "+BaseUserName+"."+CurrAnName+"(";
                        BuildInsQuery(S, AQ, aiAn);
                        AQ.SetSqlTextReady(S);
                        AQ.Exec("Вставка позиции в аналитику");
                        TotalPos++;
                    }
                    else
                    {
                        AnsiString S= "update "+BaseUserName+"."+CurrAnName+" set ";
                        if (BuildUpdQuery(S, AQ, aiAn, Sender))
                        {
                            AQ.SetSqlTextReady(S);
                            AQ.DeclVar("CURR_ROWID", Fld->AsString);
                            AQ.Exec("Update позиции аналитики");
                        }
                    }
                    if (SetTotalNow)
                        FooterCntrl.AddLine(ODS);
                    FooterChanged = true;
                  }
                  else {
                    /*if (Sjkl!="") {
                      AnsiString S= "update "+BaseUserName+"."+CurrAnName+" set ";
                        if (BuildUpdQuery(S, AQ, aiAn, Sender))
                        {
                            AQ.SetSqlTextReady(S);
                            AQ.DeclVar("CURR_ROWID", Sjkl);
                            AQ.Exec("Update позиции аналитики");
                        }
                        if (SetTotalNow)
                        FooterCntrl.AddLine(ODS);
                        FooterChanged = true;
                    }*/

                  }

                }
                else // Все поля включать в сумму == 0 => Ничего не должно быть в аналитике
                {
                    AQ.Clear();
                    if (!Fld->IsNull)
                    {
                        AQ.SetSqlTextReady("delete from "+BaseUserName+
                            "."+CurrAnName+" where ROWID=:CURR_ROWID");
                        AQ.DeclVar("CURR_ROWID",Fld->AsString);
                        AQ.Exec("Удаляем запись из аналитики");
                        DeletePosAn=true;
                    }
                }
                //CH LIGA
                } catch (...) {
                  return;
                }
                //EO CH LIGA
            }
        }
        else  //Без версий
        {
          if (CurrFlags & eoDbFlds)
            {
                AnsiString S= "update "+BaseUserName+"."+CurrDBName+" set ";
                if (BuildUpdQuery(S, AQ, aiDb,Sender))
                {
                    AQ.SetSqlTextReady(S);
                    AQ.DeclVar("ART", ODS->FieldByName("ARTICLE")->AsString);
                    AQ.Exec("Update позиции БД");
                }
            }
            Q14:
            FooterCntrl.SubLine(ODS, true);
            FooterChanged = true;
            if (IncludeInSum())
            {
                DistrLine(&FldsDescr, AnSumEd, AnSumOst, ODS, NoVerOrd);
                FooterCntrl.AddLine(ODS);//CHLIGA EO CHLIGA
                NeedRecalcArt=true;
            }
            else
            {
                AQ.Clear();
                AQ.SetSqlTextReady("delete from "+BaseUserName+
                   "."+CurrAnName+" where SUMID=:SUMID and AARTICLE=:AARTICLE");
                AQ.DeclVar("SUMID",AnSumEd);
                AQ.DeclVar("AARTICLE",ODS->FieldByName("ARTICLE")->AsString);
                AQ.Exec("");
                AnsiString S=ODS->RowId();
            }
        }
        Applied = true;

    }
    else
    if (Action == 'D')
    {
        Dirty=true;
        // Если что-то есть в аналитике сначала удаляем ее
        bool reak=false;
                  for (int j=0;j<ODS->Fields->Count;j++) {
                    if (UpperCase(ODS->Fields->Fields[j]->FieldName)=="ANALITIC_ROWID") {
                      reak=true; break;
                    }
        }
      if (reak) {
        if (!ODS->FieldByName("ANALITIC_ROWID")->IsNull)
        {
            if (WithoutVer)
            {
                AQ.SetSqlTextReady("begin delete from "+BaseUserName+"."+
                    CurrAnName+" where SUMID=:SUMID and AARTICLE=:AARTICLE;"
                    "select count(*) into :RES from "+BaseUserName+"."+
                    CurrAnName+" where SUMID=:SUMID; end;");
                AQ.DeclVar("AARTICLE", ODS->FieldByName("ARTICLE")->AsString);
                AQ.DeclVar("SUMID", AnSumEd);
                AQ.DeclVar("RES", 0);
                AQ.Exec("Удаляем записи для безверсионной ан суммы");
                TotalPos=AQ.GetVarInt("RES")-1;
            }
            else
            {
                AQ.SetSqlTextReady("delete from "+BaseUserName+"."+
                    CurrAnName+" where ROWID=:CURR_ROWID");
                AQ.DeclVar("CURR_ROWID", ODS->FieldByName("ANALITIC_ROWID")->AsString);
                AQ.Exec("");
                TotalPos--;
            }
        }
      } else {
         if (WithoutVer)
            {
                AQ.SetSqlTextReady("begin delete from "+BaseUserName+"."+
                    CurrAnName+" where SUMID=:SUMID and AARTICLE=:AARTICLE;"
                    "select count(*) into :RES from "+BaseUserName+"."+
                    CurrAnName+" where SUMID=:SUMID; end;");
                AQ.DeclVar("AARTICLE", ODS->FieldByName("ARTICLE")->AsString);
                AQ.DeclVar("SUMID", AnSumEd);
                AQ.DeclVar("RES", 0);
                AQ.Exec("Удаляем записи для безверсионной ан суммы");
                TotalPos=AQ.GetVarInt("RES")-1;
            }
      }
        AQ.Clear();
        // Теперь пробуем удалить позицию БД
        if (!WithoutVer)
        {
            // Проверка на нулевые версии
            if (!ODS->FieldByName("VER")->AsInteger)
            {
                AQ.SetSqlTextReady(
                "select 1 from "+BaseUserName+"."+FldsDescr.GetDbName()+" where "
                "ARTICLE=:ARTICLE and VER!=0");
                AQ.DeclVarStr("ARTICLE", ODS->FieldByName("ARTICLE")->AsString);
                AQ.Exec("Есть ли нулевые версии");
                if (AQ.NotEof())
                {
                    THROW_AMS_EXC("Существуют не нулевые версии. Сначала надо удалить их.");
                }
                AQ.Clear();
            }

            AQ.SetPLText(":RES:=*."+CurrDBName+"_DELPOS(:ART,:VER,:RES_TXT, :RES1);");
            AQ.DeclVar("RES",0);
            AQ.DeclVar("RES1",0);
            AQ.DeclVar("RES_TXT","");
            AQ.DeclVar("ART", ODS->FieldByName("ARTICLE")->AsString);
            AQ.DeclVar("VER", ODS->FieldByName("VER")->AsInteger);
        }
        else
        {
            AQ.SetPLText(":RES:=*."+CurrDBName+"_DELPOS(:ART,NULL,:RES_TXT,:RES1);");
            AQ.DeclVar("RES1",0);
            AQ.DeclVar("RES",0);
            AQ.DeclVar("RES_TXT","");
            AQ.DeclVar("ART", ODS->FieldByName("ARTICLE")->AsString);
        }
        AQ.Exec("Удаление позиции");
        if (AQ.GetVarInt("RES")==0)
        {
            THROW_AMS_EXC("Немогу удалить."+AQ.GetVarStr("RES_TXT"));
        }
        FooterCntrl.SubLine(ODS);
        FooterChanged = true;
        Applied = true;
    }

    if (FooterChanged && SetTotalNow)
    {
        if (!ExistFooterChecked)
        {
            AQ.Clear();
            AQ.SetSqlTextReady(
            "begin declare vTmp number(1);"
            "begin "
             " select max(1) into vTmp from "+BaseUserName+"."+CurrAnName+
             " where SUMID=:SUMID and AARTICLE=' ' and AVER=0;"
             " if vTmp is NULL then"
             "  insert into "+BaseUserName+"."+CurrAnName+" (SUMID,AARTICLE, AVER) values (:SUMID,' ',0);"
             "  commit;"
             " end if;"
            "end;"
            "end;");
            AQ.DeclVar("SUMID", AnSumEd);
            AQ.Exec("Вставляем итого строку, если это необходимо");
            ExistFooterChecked=true;
        }
        TotalAll.MakeTotalUpdate(AnSumEd);
        FooterCntrl.Draw();
    }
    if (WithoutVer) Applied = true;
    ShowBalSum();
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsAn::ActHistExecute(TObject *Sender)
{
//
    AnsiString S;
    if (GC::ViewDebug)
        S="Код редактируемой аналитической суммы: ["+CurrDBName+"/"+CurrAnName+
            "/"+CurrSetName+"] "+IntToStr(AnSumEd);
    else
        S="Код редактируемой аналитической суммы: "+IntToStr(AnSumEd);
    Message(S);
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsAn::DBGAmsFooterSyncho(TAmsDBGrid *Sender,
      SSynchoCause &Cause)
{
    Footer->SynChro (Sender, Cause);
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsAn::ActFooterExecute(TObject *Sender)
{
//
    TFrmFooterVisible* Frm;
    Application->CreateForm(__classid(TFrmFooterVisible), &Frm);
    if (Frm->FooterVisible(&FooterCntrl))
    {
        RecalcFooter();
        FooterCntrl.Draw();
    }
    Frm->Release();
}
//---------------------------------------------------------------------------
void TFrmBrowsAn::RecalcFooter(int Indx, bool Redraw)
{
    AnsiString Sql;
    int Beg = 0;
    int End = FooterCntrl.FI.Count();
    if (Indx !=-1)
    {
        Beg = Indx;
        End = Beg+1;
    }
    for (int i = Beg ; i< End; i++)
    {
        if ( FooterCntrl.FI.Visible(i))
        {
            AmsQuery AQ;
            switch (i)
            {
                // Полная итого строка
                case 0:
                    Sql = "select "+FooterCntrl.GetSql()+" from "+BaseUserName+
                    "."+CurrAnName+" AN where AN.SUMID=:SUMID and AN.AARTICLE=' ' and "
                    "AN.AVER=0";
                    AQ.DeclVar("SUMID", AnSumEd);
                break;
                // Итого строка для выбранных позиций
                case 1:
                    Sql = "select "+FooterCntrl.GetSql();
                    GetFromAndWhere (Sql, DS_Q(AQ()));
                break;
                // Итого строка для Текущей категории
                case 2:
                    Sql = "select "+FooterCntrl.GetSql()+" from "+BaseUserName+
                    "."+CurrAnName+" AN, "+BaseUserName+"."+CurrDBName+" D where "
                    "AN.SUMID=:SUMID and AN.AARTICLE= D.ARTICLE and AN.AVER=D.VER and "
                    "D.CATID=:CATID and D.ARTICLE!=' '";
                    AQ.DeclVar("SUMID", AnSumEd);
                    AQ.DeclVar("CATID", CurrCatId);
                break;
                // Итого строка для поддерева, начиная с тек категории
                case 3:
                    Sql = "select "+FooterCntrl.GetSql()+" from "+BaseUserName+
                    "."+CurrAnName+" AN, "+BaseUserName+"."+CurrDBName+" D where "
                    "AN.SUMID=:SUMID and AN.AARTICLE= D.ARTICLE and AN.AVER=D.VER and D.ARTICLE!=' ' and "
                    "D.CATID in (select OWNCAT from "+BaseUserName+".DBCAT "
                    "start with OWNCAT=:OWNCAT connect by prior OWNCAT=CATID)";
                    AQ.DeclVar("SUMID", AnSumEd);
                    AQ.DeclVar("OWNCAT", CurrCatId);
                break;
                // Итого строка для выделенных пользователем строк
                case 4:
                    if (DBG->SelectedRows->Count>1)
                    {
                        ODS->DisableControls();
                        AnsiString CurrPos = ODS->Bookmark;
                        FieldSetAnTotal& AnTotal = FooterCntrl.FI[4];
                        AnTotal.SetZero();
                        for (int j = 0 ; j< DBG->SelectedRows->Count; j++)
                        {
                            ODS->GotoBookmark((void *)DBG->SelectedRows->Items[j].c_str());
                            AnTotal.AddLine(ODS);
                        }
                        ODS->GotoBookmark((void*) CurrPos.c_str());
                        ODS->EnableControls();
                    }
                break;
            }
            if (i!=4)
            {
                AQ.SetSqlTextReady(Sql);
                FieldSetAnTotal& AnTotal = FooterCntrl.FI[i];
                AQ.Exec("Находим итого строку");
                AnTotal.CopyAnalSum(AQ);
            }
        }
    }
    if (Redraw) FooterCntrl.Draw();
}
//---------------------------------------------------------------------------
void TFrmBrowsAn::CatChanged ()
{
    RecalcFooter(2, false);
    RecalcFooter(3, true );
}
//---------------------------------------------------------------------------
void TFrmBrowsAn::BeginSelection()
{
    TimSelected->Interval = 1000;
    TimSelected->Enabled = true;
}
//---------------------------------------------------------------------------
void TFrmBrowsAn::EndSelection()
{
    TimSelected->Enabled = false;
    FooterCntrl.FI.Hide(4);
    FooterCntrl.Draw();
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsAn::TimSelectedTimer(TObject *Sender)
{
    TimSelected->Enabled= false;
    FooterCntrl.FI.Show(4);
    RecalcFooter(4, true);

}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TFrmBrowsAn::SetFilterOn()
{
    FooterCntrl.FI.Show(1);
    RecalcFooter(1, true);
};
//---------------------------------------------------------------------------
void TFrmBrowsAn::SetFilterOff()
{
    FooterCntrl.FI.Hide(1);
    FooterCntrl.Draw();
};
//---------------------------------------------------------------------------
void TFrmBrowsAn::ChangeFooter (int Sign)
{
    if (Sign ==1)
        FooterCntrl.AddLine(ODS);
    else
        FooterCntrl.SubLine(ODS);
}
//---------------------------------------------------------------------------
bool TFrmBrowsAn::IncludeInSum()
{
    bool Res = false;
    unsigned int i = 0;
    while (!Res && i<ZestFlds.size())
    {
        SFldOpt& FO=FldsDescr[ZestFlds[i]];
        TField* Fld = ODS->Fields->Fields[FO.FldNo];
        if (!Fld->IsNull && (Fld->AsFloat!=0))
            Res=true;
        else
            i++;
    }
    return Res;
};
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsAn::ActDelExecute(TObject *Sender)
{
    // Удаляем позицию аналитики (для всех полей включать в сумму ставим 0)
    if (DBG->SelectedRows->Count>0)
    {
        if( AskMessage("Удалить "+ IntToStr(DBG->SelectedRows->Count)+
            " выделеннных записей?") == ID_YES)
        {
            for (int i = 0; i< DBG->SelectedRows->Count; i++)
            {
                ODS->GotoBookmark((void *)DBG->SelectedRows->Items[i].c_str());
                try
                {
                    DeletePosAn=false;
                    ODS->Edit();
                    CurrFlags=eoAnFlds;
                    for (unsigned int i = 0; i< ZestFlds.size(); i++)
                    {
                        SFldOpt& FO = FldsDescr[ZestFlds[i]];
                        TField* Fld = ODS->Fields->Fields[FO.FldNo];
                        Fld->AsFloat=0;
                    }
                    ODS->Post();
                      AmsQuery AQ;
                     AQ.SetSqlText("select count(*) from *."+
                             CurrAnName+" where SUMID=:SUM_ID and AARTICLE!=' '");
                     AQ.DeclVar("SUM_ID", AnSumEd);
                     AQ.Exec("Получаем тотал значения");
                     TotalPos=AQ.GetFldInt(0);
                }
                catch (...)
                {
                    ErrorMessage("Позиция занята");
                }
            }
            FooterCntrl.Draw();
            DBG->SelectedRows->Clear();
            if (DeletePosAn && EdMode==asmSelPos)
            {
                ODS->Next();
                RefreshView();
            }
        }
    }
    else
    if (AskMessage("Убрать позицию из аналитики?", false)== ID_YES)
    {
        DeletePosAn=false;
        ODS->Edit();
        CurrFlags=eoAnFlds;
        for (unsigned int i = 0; i< ZestFlds.size(); i++)
        {
            SFldOpt& FO = FldsDescr[ZestFlds[i]];
            TField* Fld = ODS->Fields->Fields[FO.FldNo];
            Fld->AsFloat=0;
        }
        ODS->Post();
         AmsQuery AQ;
        AQ.SetSqlText("select count(*) from *."+
                CurrAnName+" where SUMID=:SUM_ID and AARTICLE!=' '");
        AQ.DeclVar("SUM_ID", AnSumEd);
        AQ.Exec("Получаем тотал значения");
        TotalPos=AQ.GetFldInt(0);

        FooterCntrl.Draw();
        if (DeletePosAn && EdMode==asmSelPos)
        {
            ODS->Next();
            RefreshView();
        }
    }
    ShowBalSum();
    RefreshView();
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsAn::ActClearAnExecute(TObject *Sender)
{
    // Очистить аналитическую сумму
    if(AskMessage("Очистить аналитическую сумму?")==ID_YES)
    {
        AmsQuery AQ;
        AQ.SetPLTextReady("delete from "+BaseUserName+"."+CurrAnName+" where "
            "SUMID=:SUMID; insert into "+BaseUserName+"."+CurrAnName+
                "(SUMID, AARTICLE, AVER) values (:SUMID, ' ', 0);commit;");
        AQ.DeclVar("SUMID", AnSumEd);
        AQ.Exec("Очистка аналит суммы");
        TotalPos=0;
        FooterCntrl.FI[0].SetZero();
        RefreshView();
        FooterCntrl.Draw();
        ShowBalSum();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsAn::ActTakeAnSumExecute(TObject *Sender)
{
    //
    int AnSumId = -1;
    if (AskMessage("Взять аналитическую сумму из позиции БД?")==ID_YES)
    {
        TFrmAskDbAnFld* Frm;
        Application->CreateForm(__classid(TFrmAskDbAnFld), &Frm);
        Frm->AskDbAnFld(AnSumId, CurrAnId);
        Frm->Release();
    }
    else
    {
        TFrmAskAccAnal* Frm;
        Application->CreateForm(__classid(TFrmAskAccAnal), &Frm);
        TAccName Acc=Frm->AskAccAnal(CurrAnId);
        Frm->Release();
        if (Acc.GetAccCode(NULL)!=-1)
        {
            TFrmAskAnalSumType* F;
            Application->CreateForm(__classid(TFrmAskAnalSumType), &F);
            int SumType=F->AskAnalSumType();
            int AccCode = Acc.GetAccCode(NULL);
            AmsQuery AQ;
            switch (SumType)
            {
                case 0:
                {
                    TFrmSelCpsDate* Frm;
                    Application->CreateForm(__classid(TFrmSelCpsDate), &Frm);
                    TDateTime BegP =G.ur.beg_date.val.T();

                    int XX = Frm->SelCpsDate(BegP);
                    Frm->Release();
                    if (XX)
                    {
                        AmsQuery AQ;
                        AQ.SetSqlTextReady("begin :RES:="+BaseUserName+
                                ".RI(:CODE, :BEG_DATE); end;");
                        AQ.DeclVar("RES", 0);
                        AQ.DeclVar("CODE", AccCode);
                        AQ.DeclVar("BEG_DATE", BegP);
                        AQ.Exec("Находим аналит сумму остатка");
                        AnSumId = AQ.GetVarInt("RES");
                    }

                }
                break;
                case 1:
                case 2:
                {
                    TFrmPeriod* Frm;
                    Application->CreateForm(__classid(TFrmPeriod), &Frm);

                    TDateTime BegP =G.ur.beg_date.val.T();
                    TDateTime EndP =G.ur.end_date.val.T();
                    int XX = Frm->Period(BegP,EndP);
                    Frm->Release();
                    if (XX)
                    {
                        if (SumType == 1)
                        {
                            AQ.SetSqlTextReady("begin :RES:="+BaseUserName+
                                ".CTI(:CODE, :BEG_DATE, :END_DATE); end;");
                        }
                        else
                        {
                            AQ.SetSqlTextReady(" begin :RES:="+BaseUserName+
                                ".DTI(:CODE, :BEG_DATE, :END_DATE); end;");
                        }
                        AQ.DeclVar("RES", 0);
                        AQ.DeclVar("CODE", AccCode);
                        AQ.DeclVar("BEG_DATE", BegP);
                        AQ.DeclVar("END_DATE", EndP);
                        AQ.Exec("Нах обороты");
                        AnSumId = AQ.GetVarInt("RES");
                    }

                }
                break;
                case 3:
                {
                    TFrmChooseOper* Frm;
                    Application->CreateForm(__classid(TFrmChooseOper), &Frm);
                    Frm->Begin_(this, AccCode);
                }
                break;
            }
        }
    }
    if (AnSumId>0)
    {
        AmsQuery AQ;
        AnsiString Str =CurrSetName==" "?(AnsiString)"":CurrSetName;
        AQ.SetSqlText(
        "begin "
          "*.AddAnalSum(:ANID, :RES_SUMID,:SUM_ID, :SIGN);"
          "commit;"
        " end;");
        AQ.DeclVar("ANID", CurrAnId);
        AQ.DeclVar("RES_SUMID", AnSumEd);
        AQ.DeclVar("SUM_ID", AnSumId);
        AQ.DeclVar("SIGN", 1);
        AQ.Exec("");
        RefreshView();
        OkMessage("Аналитика добавлена");
        MakeNewTotal();
    }
    return;
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsAn::BBCancelClick(TObject *Sender)
{
    //
    if ( !Dirty || AskMessage("Выйти без сохранения?", false) ==ID_YES)
    {
        TFrmInputOper* Frm = dynamic_cast<TFrmInputOper*>(ParentF);
        if (Frm)
        {
            Frm->AnalCancel();
        }
        else
        {
            TFrmEditDbPos*  Frm = dynamic_cast<TFrmEditDbPos*>(ParentF);
            if (Frm)
            {
                Frm->AnalCancel();
            }
            else
            {
                TFrmAccList* Frm = dynamic_cast<TFrmAccList*>(ParentF);
                if (Frm)
                {
                    Frm->AnRemInputBad(this);
                }
            }
        }
        EdNewAnal=false;
        FreeExecuted= true;
        Close() ;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsAn::ActDelPosExecute(TObject *Sender)
{
    if (AskMessage("Удалить позицию базы данных?")==ID_YES)
    {
        ODS->Delete();
        FooterCntrl.Draw();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsAn::ActDestrExecute(TObject *Sender)
{
    //  !!!!! Для многопоточного необходимо сделать CriticalSection
    //
    TOracleDataSet * ODS1 = new TOracleDataSet (this);
    ODS1->Session= DatMod->OSession;
    // сохраняем значения
    int EdModeP=EdMode;
    bool WithoutCatsP= WithoutCats;           // Просмотр в режиме без категорий ?
    bool DefaultSortP=DefaultSort;           // Сортировка по умолчанию - Артикул версия
    bool WithoutVerP= WithoutVer;            // Показ без версий
    bool FilterOnP= FilterOn;


    EdMode=asmSelPos;
    WithoutCats= true;           // Просмотр в режиме без категорий ?
    DefaultSort=true;           // Сортировка по умолчанию - Артикул версия
    WithoutVer= true;            // Показ без версий
    FilterOn= false;

    BuildSqlQuery(ODS1);


    EdMode=EdModeP;
    WithoutCats= WithoutCatsP;           // Просмотр в режиме без категорий ?
    DefaultSort=DefaultSortP;           // Сортировка по умолчанию - Артикул версия
    WithoutVer= WithoutVerP;            // Показ без версий
    FilterOn= FilterOnP;

    // Конец критикал секшн

    try
    {
        ODS1->Active= true;
        ODS1->First();
        while (!ODS1->Eof)
        {
            DistrLine (&FldsDescr, AnSumEd, AnSumOst, ODS1, NoVerOrd);
            NeedRecalcArt=true;
            AnsiString Art=ODS->FieldByName("ARTICLE")->AsString;
            RecalcOneArticle(Art);
            ODS1->Next();
        }

        AmsQuery AQ;
        AQ.SetSqlText("select count(*) from *."+
                CurrAnName+" where SUMID=:SUM_ID and AARTICLE!=' '");
        AQ.DeclVar("SUM_ID", AnSumEd);
        AQ.Exec("Получаем тотал значения");
        TotalPos=AQ.GetFldInt(0);
        NeedRecalcArt=false;
        RefreshView();
        delete ODS1;
    }
    catch(...)
    {
        delete ODS1;
        throw;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsAn::BBOkClick(TObject *Sender)
{
        if (!ExistFooterChecked)
        {
            AmsQuery AQ;
            AQ.SetSqlTextReady(
            "begin declare vTmp number(1);"
            "begin "
             " select max(1) into vTmp from "+BaseUserName+"."+CurrAnName+
             " where SUMID=:SUMID and AARTICLE=' ' and AVER=0;"
             " if vTmp is NULL then"
             "  insert into "+BaseUserName+"."+CurrAnName+" (SUMID,AARTICLE, AVER) values (:SUMID,' ',0);"
             "  commit;"
             " end if;"
            "end;"
            "end;");
            AQ.DeclVar("SUMID", AnSumEd);
            AQ.Exec("Вставляем итого строку, если это необходимо");
            ExistFooterChecked=true;
        }
        TotalAll.MakeTotalUpdate(AnSumEd);


    if (CheckNeg)
    {
        // Находим все поля дистр
        std::vector<int> DistrFlds;
        for (int i = 0; i< FldsDescr.Count(); i++)
        {
            SFldOpt& FO=FldsDescr[i];
            if (    (FO.DefOpt & DistrFld)&&
                    (FO.FldAddInfo==aiAn))
            {
                DistrFlds.push_back(i);
            }
        }
        if (!DistrFlds.size())
        {
            ErrorMessage("Аналитика должна иметь поле РАСПРЕДЕЛЯТЬ.");
            return;
        }
        AnsiString S="select T.ARTICLE, T.VER, T.CATID" ;
        AnsiString WhereAdd;
        AnsiString AD="+";
        AnsiString AP="-";
        if (!AddToRem)
        {
            AD="-";
            AP="+";
        }
        for (unsigned i =0; i< DistrFlds.size(); i++)
        {
            SFldOpt& FO= FldsDescr[DistrFlds[i]];
            AnsiString AddStr= "nvl(AO."+FO.RealName+", 0)"+AP+
                        "nvl(AW."+FO.RealName+",0)"+AD+
                        "nvl(AE."+FO.RealName+",0)";
            WhereAdd+=AddStr+"<0 and ";
        }
        AmsQuery AQ;

        AQ.SetSqlText(S+"  from *."+FldsDescr.GetDbName()+" T, *."+
                    FldsDescr.GetAnName()+" AO, *."+
                    FldsDescr.GetAnName()+" AW, *."+
                    FldsDescr.GetAnName()+" AE "+
                    "where "
                        "T.ARTICLE	= AO.AARTICLE(+) and "
                        "T.VER		= AO.AVER(+) and "
                        "T.ARTICLE 	= AW.AARTICLE(+) and "
                        "T.VER		= AW.AVER (+) and "
                        "AO.SUMID(+)	=:SUMID_O and "
                        "T.ARTICLE 	= AE.AARTICLE(+) and "
                        "T.VER		= AE.AVER (+) and "
                        "AE.SUMID(+)	= :SUMID_E and "
                        "AW.SUMID(+)	= :SUMID_W and "+WhereAdd+
                        "(AW.AARTICLE is not NULL or AE.AARTICLE is not NULL)"
                        " and AE.AARTICLE!=' ' "
                        " and ROWNUM<2");

        AQ.DeclVar("SUMID_O", AnSumOst);
        AQ.DeclVar("SUMID_E", AnSumEd);
        AQ.DeclVar("SUMID_W", PredSumId);

        AQ.Exec("Ищу отрицательные остатки");
        if (AQ.NotEof())
        {
            if (AskMessage("Могут быть отрицательные остатки.\r\n"
                    "Catid="+AQ.GetFldStr(2)+" Article='"+AQ.GetFldStr(0)+"' Ver="+IntToStr(AQ.GetFldInt(1))+
                 "\r\nПродолжить?", false)!=ID_YES)
            {
                SetPos(AQ.GetFldStr(0), AQ.GetFldInt(1), AQ.GetFldInt(2));
                return ;
            }
        }

    }
    if (BalFld!="")
    {
        AmsQuery AQ;
        AQ.SetSqlText(
            "begin "
            "   declare vBalSum number;"
            "begin "
            "   select max("+BalFld+") into vBalSum "
            "   from *."+CurrAnName+" where SUMID=:SUMID and "
            "   AARTICLE=' ' and AVER=0;"
            "   if vBalSum is not NULL then "
            "       update *.SUMS set VALUE=vBalSum "
            "       where SUM_ID=:SUMID; "
            "   end if; "
            "end; commit;"
            "end;");
        AQ.DeclVar("SUMID", AnSumEd);
        AQ.Exec("Вставляем балансовую сумму");
    }
    TFrmInputOper* Frm = dynamic_cast<TFrmInputOper*>(ParentF);
    if (Frm)
    {
        Frm->AnalOk();
    }
    else
    {
        TFrmEditDbPos* Frm = dynamic_cast<TFrmEditDbPos*>(ParentF);
        if (Frm)
        {
            Frm->AnalOk();
        }
        else
        {
            TFrmAccList* Frm = dynamic_cast<TFrmAccList*>(ParentF);
            if (Frm)
            {
                Frm->AnRemInputOk(this);
            }
        }
    }
    FreeExecuted= true;
    EdNewAnal=false;
    Close() ;
}
//---------------------------------------------------------------------------
void TFrmBrowsAn::Recalc (bool ClearFlds, TOracleDataSet* ODS_)
{
    FSM->SetDS(ODS_);
    CurrFlags = eoAnFlds;
    try
    {
        ExecODS(ODS_);
        ODS_->First();
        SetTotalNow= false;
        while (!ODS_->Eof)
        {
            ODS_->Edit();
            RunProg(0, 1, 0, ClearFlds);
            RunProg(1, 1, 0, false);
            for (int i = 0; i< FldsDescr.Count(); i++)
            {
                SFldOpt& FO = FldsDescr[i];
                if ( !(FO.DefOpt&SystemFld) &&
                        (FO.FldAddInfo == aiDb || FO.FldAddInfo == aiAn)
                )
                {
                    TField* Fld = ODS_->Fields->Fields[FO.FldNo];
                    FSM->At(i)->SetVal(Fld);
                }
            }
            ODS_->Post();
            ODS_->Next();
        }
        SetTotalNow=true;

        //----------------------------------------------------
        // Пересчитываем итого строку ??
        AmsQuery AQ;
        AQ.SetSqlTextReady(
            "select "+FooterCntrl.GetSql()+" from "+
            BaseUserName+"."+CurrAnName+" AN where AN.SUMID=:SUMID and AN.AARTICLE !=' '"
        );
        AQ.DeclVar("SUMID", AnSumEd);
        AQ.Exec("Находим тотал строку");
        TotalAll.CopyAnalSum(AQ);
        TotalAll.MakeTotalUpdate(AnSumEd);
        //----------------------------------------------------
        FSM->SetDS(ODS);
    }
    catch(...)
    {
        FSM->SetDS(ODS);
        SetTotalNow= true;
        TotalAll.MakeTotalUpdate(AnSumEd);
        throw;
    }



    RefreshView();
    MakeNewTotal();
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsAn::ActCalcExecute(TObject *Sender)
{
    if (AskMessage("Пересчитать аналитическую сумму?")!=ID_YES)
    {
        return;
    }
    // Пересчет по Контрл-Ф2
    TOracleDataSet * ODS1 = new TOracleDataSet (this);
    ODS1->Session= DatMod->OSession;
    ODS1->OnApplyRecord=ODSApplyRecord;
    // сохраняем значения
    int EdModeP=EdMode;
    bool WithoutCatsP= WithoutCats;           // Просмотр в режиме без категорий ?
    bool DefaultSortP=DefaultSort;           // Сортировка по умолчанию - Артикул версия
    bool WithoutVerP= WithoutVer;            // Показ без версий
    bool FilterOnP= FilterOn;

    EdMode=asmSelPos;
    WithoutCats= true;           // Просмотр в режиме без категорий ?
    DefaultSort=true;           // Сортировка по умолчанию - Артикул версия
    WithoutVer=false;            // Показ без версий
    FilterOn= false;

    BuildSqlQuery(ODS1);

    EdMode=EdModeP;
    WithoutCats= WithoutCatsP;           // Просмотр в режиме без категорий ?
    DefaultSort=DefaultSortP;           // Сортировка по умолчанию - Артикул версия
    WithoutVer= WithoutVerP;            // Показ без версий
    FilterOn= FilterOnP;

    try
    {
        Recalc(false, ODS1);
        delete ODS1;
        EdMode=EdModeP;
        WithoutCats= WithoutCatsP;           // Просмотр в режиме без категорий ?
        DefaultSort=DefaultSortP;           // Сортировка по умолчанию - Артикул версия
        WithoutVer= WithoutVerP;            // Показ без версий
        FilterOn= FilterOnP;
    }
    catch(...)
    {
        delete ODS1;
        EdMode=EdModeP;
        WithoutCats= WithoutCatsP;           // Просмотр в режиме без категорий ?
        DefaultSort=DefaultSortP;           // Сортировка по умолчанию - Артикул версия
        WithoutVer= WithoutVerP;            // Показ без версий
        FilterOn= FilterOnP;
        throw;
    }
    ShowBalSum();
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsAn::ActCalcWithClearExecute(TObject *Sender)
{
    if (AskMessage("Пересчитать аналитическую сумму и вводимые поля?")!=ID_YES)
    {
        return;
    }
    // Пересчет по Контрл-Ф4
    TOracleDataSet * ODS1 = new TOracleDataSet (this);
    ODS1->Session= DatMod->OSession;
    ODS1->OnApplyRecord=ODSApplyRecord;
    // сохраняем значения
    int EdModeP=EdMode;
    bool WithoutCatsP= WithoutCats;           // Просмотр в режиме без категорий ?
    bool DefaultSortP=DefaultSort;           // Сортировка по умолчанию - Артикул версия
    bool WithoutVerP= WithoutVer;            // Показ без версий
    bool FilterOnP= FilterOn;


    EdMode=asmSelPos;
    WithoutCats= true;           // Просмотр в режиме без категорий ?
    DefaultSort=true;           // Сортировка по умолчанию - Артикул версия
    WithoutVer=false;            // Показ без версий
    FilterOn= false;

    BuildSqlQuery(ODS1);


    try
    {
        Recalc(true, ODS1);
        delete ODS1;
        EdMode=EdModeP;
        WithoutCats= WithoutCatsP;           // Просмотр в режиме без категорий ?
        DefaultSort=DefaultSortP;           // Сортировка по умолчанию - Артикул версия
        WithoutVer= WithoutVerP;            // Показ без версий
        FilterOn= FilterOnP;
    }
    catch(...)
    {
        delete ODS1;
        EdMode=EdModeP;
        WithoutCats= WithoutCatsP;           // Просмотр в режиме без категорий ?
        DefaultSort=DefaultSortP;           // Сортировка по умолчанию - Артикул версия
        WithoutVer= WithoutVerP;            // Показ без версий
        FilterOn= FilterOnP;
        throw;
    }
    ShowBalSum();
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsAn::ActCalcVisibleExecute(TObject *Sender)
{
// Shift+F2
    Recalc(false, ODS);
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsAn::ActClaclVisibleWithClearExecute(
      TObject *Sender)
{
// Shift+ F4
    Recalc(true, ODS);
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsAn::DBGFrosenDblClick(TObject *Sender)
{
    //
/*    AmsQuery AQ;
    AQ.SetSqlText("select * from *."+FldsDescr->GetDbName()+
        " where "+ShFld_+"=:SH_FLD");
    AQ.DeclVar("SH_FLD", ShtrixVal_);
    AQ.Exec("Находим позицию БД с таким штрихкодом");
    if (!AQ.NotEof())
    {
        ErrorMessage("Такой штрих-код не обнаружен в базе");
        return 1;
    }

    OpVars* OP=FrmBrowsDb->GetOpVars();
    OP->AddVars(AQ);*/

    ActEditAnExecute(this);
}
//---------------------------------------------------------------------------
void EditAnalSum(TForm* Parent_,int AnId,AnsiString AnName,AnsiString SetName,
                AnsiString Descr,int ASEd,int ASOst)
{
    TFrmBrowsAn* F;
    Application->CreateForm (__classid(TFrmBrowsAn), &F);
    F->Begin(Parent_,AnId,AnName,SetName,Descr,ASEd,ASOst);
}
//---------------------------------------------------------------------------
void TFrmBrowsAn::TakeAnSum()
{
    ActTakeAnSum->Execute();
};
//---------------------------------------------------------------------------
void TFrmBrowsAn::BeginEdit(int Key)
{
    int Flags = eoAnFlds | ReadOnly;
    CurrFlags=Flags;
    HaveFirstDigit = true;
    FirstDigit = Key;
    ActEdDbPosExec (Flags);
    if (NeedRecalcArt)
    {
        AnsiString Art=ODS->FieldByName("ARTICLE")->AsString;
        RecalcOneArticle(Art);
        AmsQuery AQ;
        AQ.SetSqlText("select count(*) from *."+
                CurrAnName+" where SUMID=:SUM_ID and AARTICLE!=' '");
        AQ.DeclVar("SUM_ID", AnSumEd);
        AQ.Exec("Получаем тотал значения");

        TotalPos=AQ.GetFldInt(0);
        NeedRecalcArt=false;
    }
};
//---------------------------------------------------------------------------


void __fastcall TFrmBrowsAn::ActOk_Execute(TObject *Sender)
{
//
    BBOkClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsAn::ActDocGenExecute(TObject *Sender)
{
//
    GenerateDoc(otDoc);
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsAn::ActFormGenExecute(TObject *Sender)
{
//
    GenerateDoc(otRepForm);
}
//---------------------------------------------------------------------------
void TFrmBrowsAn::GenerateDoc(int ObjType)
{
    //
    SCodeAndName S;
    S.Name="";
    TFrmChooseObjModal* Frm;
    Application->CreateForm(__classid(TFrmChooseObjModal), &Frm);
    if (!Frm->ChooseObjModal(ObjType, S))
    {
        return;
    }

    TGenLocalVars LV;
    OpVars* OV =LV.VARS();
    OV->AddVarInt("SUMID", AnSumEd);
    OV->AddVarInt("REMSUMID", AnSumOst);
    OV->AddVarInt("CATMODE", !WithoutCats);
    OV->AddVarInt("OUTMODE", EdMode);
    OV->AddVarInt("EXTART", WithoutVer);

    if (AccCode)
    {
        do {
            AmsQuery AQ;
            AQ.SetSqlTextReady(
            "select * from "+BaseUserName+".ACCS where CODE=:CODE");
            AQ.DeclVarInt("CODE", AccCode);
            AQ.Exec("Выбираем данные осчете");
            OV->AddVars(AQ);
            OV=LV.ACCV();
            OV->AddVars(AQ);
        }while(0);
    }
    BeginExecReport(S.Name, &LV);
};
//---------------------------------------------------------------------------
void TFrmBrowsAn::ViewUpdated()
{
    SSynchoCause S;
    S.Cause=scSizeChange;
    Footer->SynChro( DBG, S);
};
//---------------------------------------------------------------------------
void TFrmBrowsAn::ShowBalSum()
{
    LabTotal->Caption=
        "Наименований:" + IntToStr(TotalPos)+
        "  Балансовая сумма: "+ FormatFloat(BalMask, TotalAll.GetResult(BalFld));
};
//---------------------------------------------------------------------------
void TFrmBrowsAn::MakeNewTotal()
{
    do {
        AmsQuery AQ;
        AQ.SetSqlTextReady (
            "begin "
            "   select count(*) into :TOTAL_POS "
            "   from "+BaseUserName+"."+CurrAnName+
            "   where SUMID=:SUMID and AARTICLE!=' ';"
           // "   select max("+BalFld+") into :BAL_FLD "
           // "   from "+BaseUserName+"."+CurrAnName+
           // "   where SUMID=:SUMID and ARTICLE=' ' and VER=0;"
            "end;");
        AQ.DeclVar("TOTAL_POS",0);
        AQ.DeclVar("SUMID", AnSumEd);
        //AQ.DeclVar("BAL_FLD", 0.0);
        AQ.Exec("Находим тотал-строку");
        TotalPos= AQ.GetVarInt("TOTAL_POS");
    }while(0);
    RecalcFooter();
    ShowBalSum();
};
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsAn::ActOperVarsExecute(TObject *Sender)
{
//
    TFrmInputOper* Frm=dynamic_cast<TFrmInputOper*> (ParentF);
    if (Frm)
    {
        Frm->BeginTiedDbInput(false);
    }
}
//---------------------------------------------------------------------------
void TFrmBrowsAn::EditOperVars()
{
    ActOperVars->Execute();
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsAn::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    AmsWinC->FindActiveMDIChild();
    if (!FreeExecuted)
    {
        Action=caNone;
        BBCancelClick(this);
        return;
    }

    if (FrmEditDbPos)
    {
        FrmEditDbPos->Release();
        FrmEditDbPos=NULL;
    }

    if (FrmFindDbPos)
    {
        FrmFindDbPos->Release();
        FrmFindDbPos=NULL;
    }

    if (FrmFilter)
    {
        FrmFilter->Release();
        FrmFilter=NULL;
    }

    if (FrmSortOrder)
    {
        FrmSortOrder->Release();
        FrmSortOrder=NULL;
    }

    if (FrmQuickSearch)
    {
        FrmQuickSearch->Release();
        FrmQuickSearch=NULL;
    }

    delete ODS1;
    delete FSM;
    Action = caFree;

}
//---------------------------------------------------------------------------
void TFrmBrowsAn::SetNewSumId(int NewSumId)
{
    AnSumEd=NewSumId;
}
//---------------------------------------------------------------------------
void TFrmBrowsAn::OperSelectedOk(int SumId)
{
    //TODO: Add your source code here
    if (SumId>0)
    {
        AmsQuery AQ;
        AnsiString Str =CurrSetName==" "?(AnsiString)"":CurrSetName;
        AQ.SetSqlText(
        "begin "
          "*.AddAnalSum(:ANSUM, :RES_SUMID,:SUM_ID, :SIGN);"
          "commit;"
        " end;");
        AQ.DeclVar("ANSUM", CurrAnId);
        AQ.DeclVar("RES_SUMID", AnSumEd);
        AQ.DeclVar("SUM_ID", SumId);
        AQ.DeclVar("SIGN", 1);
        AQ.Exec("");
        RefreshView();
        OkMessage("Аналитика добавлена");
        MakeNewTotal();
    }
}
//---------------------------------------------------------------------------
void TFrmBrowsAn::OperSelectedFalse()
{
    //TODO: Add your source code here
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsAn::ActFilterExecute(TObject *Sender)
{
    if (!FilterForm || !PC_FilterPos.IsHaveChild(this, FilterForm))
    {
        Application->CreateForm(__classid(TFrmFilterForm), &FilterForm);
        PC_FilterPos.SetChild(this, FilterForm);
    }

    FilterForm->BeginFilter(AccCode, ODS->FieldByName("ARTICLE")->AsString,
                                    ODS->FieldByName("VER")->AsInteger, WithoutVer);
}
//---------------------------------------------------------------------------
void TFrmBrowsAn::SetFilter()
{
    ActFilter->Execute();
};
//---------------------------------------------------------------------------
void TFrmBrowsAn::VirtKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
    if (Shift.Contains(ssAlt))
    {
        switch(Key)
        {
            case 74: // Cntrl+О
                Key=0;
                ActDocGen->Execute();
            break;
            case 86: // Cntrl+М
                Key=0;
                ActFormGen->Execute();
            break;
        }
    }
}
//---------------------------------------------------------------------------
void TFrmBrowsAn::MakeInsert(TOracleDataSet* ODS)
{
    AmsQuery AQ;
    bool FooterChanged = false;
    char Action = 'I';
    bool Applied=false;
    AnsiString NewRowId;
    if (Action == 'I')
    {
        if (CurrFlags & eoDbFlds)
        {
            AnsiString S= "begin insert into "+BaseUserName+"."+CurrDBName+"(";
            if (BuildInsQuery(S, AQ, aiDb))
            {
                S+="; select ROWID into :CURR_ROWID from "+BaseUserName+"."+
                    CurrDBName+" where ARTICLE=:ARTICLE and VER=:VER; end;";
                AQ.DeclVar("CURR_ROWID", "");
                AQ.SetSqlTextReady(S);
                AQ.Exec("Вставка позиции БД");
                NewRowId= AQ.GetVarStr("CURR_ROWID");
                AQ.Clear();
            }
        }
        if (CurrFlags & eoAnFlds)
        {
            if (IncludeInSum())
            {
                AQ.Clear();
                AnsiString S= "insert into "+BaseUserName+"."+CurrAnName+"(";
                if (BuildInsQuery(S, AQ, aiAn))
                {
                    AQ.SetSqlTextReady(S);
                    AQ.Exec("Вставка позиции в аналитику");
                    TotalPos++;
                    FooterCntrl.AddLine(ODS);
                    FooterChanged = true;
                }
            }
        }
        Applied = true;
    }
    if (FooterChanged && SetTotalNow)
    {
        if (!ExistFooterChecked)
        {
            AQ.Clear();
            AQ.SetSqlTextReady(
            "begin declare vTmp number(1);"
            "begin "
             " select max(1) into vTmp from "+BaseUserName+"."+CurrAnName+
             " where SUMID=:SUMID and AARTICLE=' ' and AVER=0;"
             " if vTmp is NULL then"
             "  insert into "+BaseUserName+"."+CurrAnName+" (SUMID,AARTICLE, AVER) values (:SUMID,' ',0);"
             "  commit;"
             " end if;"
            "end;"
            "end;");
            AQ.DeclVar("SUMID", AnSumEd);
            AQ.Exec("Вставляем итого строку, если это необходимо");
            ExistFooterChecked=true;
        }
        TotalAll.MakeTotalUpdate(AnSumEd);
    }
    if (WithoutVer) Applied = true;
    ShowBalSum();
    NewRowId_=NewRowId;
    if (EdMode==asmAllBase)
    {
        ODS->Post();
    }
    else
    {
        ODS->Cancel();
        Message("Вставлено, перейдите в режим ВСЯ БАЗА");
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsAn::ActShtrixModeOnOffExecute(TObject *Sender)
{
//
    ShtrixCodeMode=!ShtrixCodeMode;
    if (ShtrixCodeMode)
    {
        ActShtrixModeOnOff->Caption="Выключить штрих-код ввод";


    }
    else
    {
        ActShtrixModeOnOff->Caption="Включить штрих-код ввод";

    }
    ChangeShtrixMode=true;

    EdShtrixCode->Enabled=ShtrixCodeMode;
    EdShtrixCode->Visible=ShtrixCodeMode;
    if (ShtrixCodeMode)
    {
        EdShtrixCode->SelectAll();
        SxSaveEdMode=EdMode;
        SxSaveWithoutCats=WithoutCats;
        EdMode = asmSelPos;
        WithoutCats= true;
    }
    else
    {
        EdMode = SxSaveEdMode;
        WithoutCats=SxSaveWithoutCats;
    }
    RefreshView();
    ShowMode();
    if (ShtrixCodeMode)
        EdShtrixCode->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsAn::ButEnterClick(TObject *Sender)
{
    if (EdShtrixCode->Focused())
    {
        if (!FrmEditDbPos)
        {
            Application->CreateForm (__classid (TFrmEditDbPos), &FrmEditDbPos);
            FrmEditDbPos->Top= 50;
            FrmEditDbPos->Left= 50;
            FrmEditDbPos->WithoutCats=WithoutCats;
            FrmEditDbPos->FrmBrowsDb=this;
            FrmEditDbPos->Position = poMainFormCenter;
            FrmEditDbPos->SetFldsDescr(&FldsDescr);
        }
        if (ShtrixCodeCounter < TotalPos)
        {
            ShtrixCodeCounter=TotalPos;
        }
        ShtrixCodeCounter++;

        CurrFlags=eoAnFlds;
        // Идет ввод по шрих кодам
        // Находим такую позицию в уже введенном
        TSearchRecordOptions SetSR;
        SetSR<<srFromBeginning;
        int Res=-1;
        int NewPos=1;
        if (ODS->SearchRecord(ShtrixFld, EdShtrixCode->Text, SetSR))
        {
            // Уже такая позиция была введена
            FSM->LoadData(*GetOpVars(), 1, 0 , 0);
            Res=FrmEditDbPos->ShtrixAddOne(ODS, ShtrixFld,
                    ShtrixOrderFld, ShtrixCodeCounter);
            AmsQuery AQ;
            AQ.SetSqlText("select 1 from *."+CurrAnName+
                " where SUMID=:SUMID and AARTICLE=:ARTICLE and AVER=:VER");
            AQ.DeclVar("SUMID", AnSumEd);
            AQ.DeclVar("ARTICLE", ODS->FieldByName("ARTICLE")->AsString);
            AQ.DeclVar("VER", ODS->FieldByName("VER")->AsInteger);
            AQ.Exec("Есть ли такая запись в аналитике");
            if (AQ.NotEof())
            {
                NewPos=0;
                FooterCntrl.SubLine(ODS, true);
            }
        }
        else
        {
            // Проставить 1 на к-л позиции
            Res=FrmEditDbPos->ShtrixMakeNewPos(ODS, ShtrixFld,
                    ShtrixOrderFld, EdShtrixCode->Text, ShtrixCodeCounter);
        }
        if (!Res)
        {
            Dirty=true;
            TotalPos+=NewPos;
            AmsQuery AQ;
            AnsiString Ins=
                NewPos?"insert into *."+ FldsDescr.GetAnName() +" (SUMID":
                    "update *."+CurrAnName+" set SUMID=:SUMID ";
            AnsiString Val=":SUMID";
            AQ.DeclVarInt("SUMID", AnSumEd);
            Struct* S = GetOpVars()->Stru();
            for (int i= 0; i< FldsDescr.Count(); i++)
            {
                SFldOpt& FO=FldsDescr.At(i);
                if (FO.FldAddInfo==aiAn)
                {
                    if (FO.RealName.UpperCase()!="SUMID")
                    {
                        Node* N =S->FindVar(FO.RealName.c_str());
                        if (N)
                        {
                            Ins+=", "+FO.RealName;
                            if (NewPos)
                                Val+=", :"+FO.RealName;
                            else
                                Ins+="=:"+FO.RealName;
                            switch (FO.Type)
                            {
                                case aftSumma:
                                    AQ.DeclVarSum(FO.RealName, N->Number());
                                break;
                                case aftInt:
                                case aftBool:
                                    AQ.DeclVarInt(FO.RealName, N->Number());
                                break;
                                case aftStr:
                                    AQ.DeclVarStr(FO.RealName, N->String().data());
                                break;
                                case aftDate:
                                    AQ.DeclVarSum(FO.RealName, N->GetDate().T());
                                break;
                            }
                            S->DelVar(FO.RealName.c_str());
                        }
                    }
                }
            }// Прошли по всем полям
            if (NewPos)
                AQ.SetPLText( Ins+") values ("+Val+"); commit;");
            else
                AQ.SetPLText( Ins+" where SUMID=:SUMID and AARTICLE=:AARTICLE and AVER=:AVER; commit;");
            AQ.Exec("Вставляем в аналитику ");

            ODS->DisableControls();
            ODS->Active=false;
            ODS->Active=true;
            //ODS->Last();
            ODS->EnableControls();
            FooterCntrl.AddLine(ODS, true);
            FooterCntrl.Draw();
            ShowBalSum();
            EdShtrixCode->SelectAll();
        }
    }
    else
        TFrmBrowsDb::ButEnterClick(Sender);
}
//---------------------------------------------------------------------------
AnsiString TFrmBrowsAn::GetOrderBy(void)
{
    if (ShtrixCodeMode)
    {
        return " AN."+ShtrixOrderFld+" desc ";
    }
    else
        return TFrmBrowsDb::GetOrderBy();
};
//---------------------------------------------------------------------------
Prog* TFrmBrowsAn::GetAnProg()
{
    if (!an_prog_comp)
    {
        ProgFile pf;
        char fname[256];
        sprintf(fname,"%s\\%s.prg",GC::DocPath.c_str(),CurrAnName.c_str());
        pf.Init(fname);
        try {
            an_prog.Compile(&pf);
            an_prog.Link(GC::DocPath.c_str());
        } catch(...) {}
    }
    an_prog_comp = true;
    return &an_prog;
};
//---------------------------------------------------------------------------
void TFrmBrowsAn::RecalcOneArticle(const AnsiString& Art)
{
    // Пересчет по Контрл-Ф2
    TOracleDataSet * ODS1 = new TOracleDataSet (this);
    ODS1->Session= DatMod->OSession;
    ODS1->OnApplyRecord=ODSApplyRecord;
    // сохраняем значения
    int EdModeP=EdMode;
    bool WithoutCatsP= WithoutCats;           // Просмотр в режиме без категорий ?
    bool DefaultSortP=DefaultSort;           // Сортировка по умолчанию - Артикул версия
    bool WithoutVerP= WithoutVer;            // Показ без версий
    bool FilterOnP= FilterOn;

    EdMode=asmSelPos;
    WithoutCats= true;           // Просмотр в режиме без категорий ?
    DefaultSort=true;           // Сортировка по умолчанию - Артикул версия
    WithoutVer=false;            // Показ без версий
    FilterOn= false;

    RecalcArticle=Art;
    BuildSqlQuery(ODS1);
    RecalcArticle="";

    EdMode=EdModeP;
    WithoutCats= WithoutCatsP;           // Просмотр в режиме без категорий ?
    DefaultSort=DefaultSortP;           // Сортировка по умолчанию - Артикул версия
    WithoutVer= WithoutVerP;            // Показ без версий
    FilterOn= FilterOnP;

    try
    {
        Recalc(false, ODS1);
        delete ODS1;
        EdMode=EdModeP;
        WithoutCats= WithoutCatsP;           // Просмотр в режиме без категорий ?
        DefaultSort=DefaultSortP;           // Сортировка по умолчанию - Артикул версия
        WithoutVer= WithoutVerP;            // Показ без версий
        FilterOn= FilterOnP;
    }
    catch(...)
    {
        delete ODS1;
        EdMode=EdModeP;
        WithoutCats= WithoutCatsP;           // Просмотр в режиме без категорий ?
        DefaultSort=DefaultSortP;           // Сортировка по умолчанию - Артикул версия
        WithoutVer= WithoutVerP;            // Показ без версий
        FilterOn= FilterOnP;
        throw;
    }
    ShowBalSum();
};

//------------------------------------------------------------

void TFrmBrowsAn::RefreshView(void)
{
    jkl=0;
    TFrmBrowsDb::RefreshView();
//     ODS->DisableControls();
    int CatId = CurrCatId;
    int TL = DBG->LeftCol;
    int Col = DBG->Col;
    AnsiString Art = ODS->FieldByName("ARTICLE")->AsString;
    int Ver = ODS->FieldByName("VER")->AsInteger;

    BuildSqlQuery(ODS);
    PrepareODS1();

    ExecODS(ODS);
    DBG->LeftCol = TL;
    DBG->Col = Col;
    FormatSums();
    //ODS->Active = true;
    SetPos(Art, Ver, CatId);

//    ODS->EnableControls();
    ViewUpdated();
}

void TFrmBrowsAn::CalcAnSumWithoutVer(AnsiString art_)
{
    int CatId = CurrCatId;
    AnsiString Art = ODS->FieldByName("ARTICLE")->AsString;
    int Ver = ODS->FieldByName("VER")->AsInteger;

    AnsiString Strx=ODS1->SQL->Text;
    AnsiString BeforeGroup=Strx.SubString(1, Strx.Pos("order by")-1);
    AnsiString AfterGroup=Strx.SubString(Strx.Pos("order by"), Strx.Length());
    Strx=BeforeGroup+" and D.ARTICLE= '"+art_+"' "+AfterGroup;
    ODS1->SQL->Text=Strx;
    //ODS1->Variables->Assign(((TVariables*)ODS->Variables));
    try
    {
        FSM->SetDS(ODS1);
        ODS1->Active=true;
        //ExecODS(ODS1);
        ODS1->First();
        SetTotalNow= false;
        while (!ODS1->Eof)
        {
            ODS1->Edit();
            RunProg(0, 1, 0, false);
            RunProg(1, 1, 0, false);
            for (int i = 0; i< FldsDescr.Count(); i++)
            {
                SFldOpt& FO = FldsDescr[i];
                if ( !(FO.DefOpt&SystemFld) &&
                        (FO.FldAddInfo == aiDb || FO.FldAddInfo == aiAn)
                )
                {
                    TField* Fld = ODS1->Fields->Fields[FO.FldNo];
                    FSM->At(i)->SetVal(Fld);
                }
            }
            ODS1->Post();
            ODS1->Next();
        }
        ODS1->Active=false;
        FSM->SetDS(ODS);
    }
    catch(...)
    {
       ODS1->Active=false;
       FSM->SetDS(ODS);
       throw;
    }
    ShowBalSum();
    MakeNewTotal();
    SetPos(Art, Ver, CatId);

}

void TFrmBrowsAn::PrepareODS1()
{
    int EdModeP=EdMode;
    bool WithoutCatsP= WithoutCats;
    bool DefaultSortP=DefaultSort;
    bool WithoutVerP= WithoutVer;
    bool FilterOnP= FilterOn;

    EdMode=asmSelPos;
    WithoutCats= true;
    DefaultSort=true;
    WithoutVer=false;
    FilterOn= false;

    BuildSqlQuery(ODS1);

    EdMode=EdModeP;
    WithoutCats= WithoutCatsP;
    DefaultSort=DefaultSortP;
    WithoutVer= WithoutVerP;
    FilterOn= FilterOnP;

}
//---------------------------------------------------------------------------







