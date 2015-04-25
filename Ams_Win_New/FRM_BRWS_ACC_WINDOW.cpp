//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_BRWS_ACC_WINDOW.h"
#include "amsclnt.h"
#include "FRM_VIEW_OPTIONS_AW.h"
#include "ams_win_c.h"
#include "FRM_SEL_LIST.h"
#include "GET_DB_POS.h"
#include "FRM_PERIOD.h"
#include "FRM_AN_VIEW.h"
#include "REFRESH_ACCS.h"
#include "FRM_ASK_ANAL_SUM_TYPE2.h"
#include "FRM_CHOOSE_FORM.h"
#include "FRM_POST_OPERS.h"
#include "FRM_CHOOSE_NOT_PLAIN_PERIOD.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "FRM_OPER_LIST"
#pragma link "OracleData"
#pragma link "RXDBCtrl"
#pragma link "RXSplit"
#pragma link "FRM_OPER_LIST"
#pragma link "AmsFooter"
#pragma link "AmsDBGrid"
#pragma link "RxCombos"
#pragma link "Oracle"
#pragma resource "*.dfm"
TFrmBrwsAccWindow *FrmBrwsAccWindow;
//---------------------------------------------------------------------------
__fastcall TFrmBrwsAccWindow::TFrmBrwsAccWindow(TComponent* Owner)
    : TFrmOperListRoot(Owner)
{
        ActViewOpt->ShortCut=TextToShortCut("Alt+H"); // Алт+р
        ActAnaliticShow->ShortCut=TextToShortCut("Alt+N");
        ActFieldName->ShortCut=TextToShortCut("Alt+D");
        ActAnalEndRem->ShortCut=TextToShortCut("Alt+J");
        ActAnalChoose->ShortCut=TextToShortCut("Alt+F");

        AFEnd->RowHeights[eefEmpty]=-1;
        AFEnd->RowHeights[eefSelected]=-1;
        AFEnd->RowHeights[eefF5]=0;
        AFEnd->RowCount=eefEnd;

        /*
        ->ShortCut=TextToShortCut("Alt+");
        ->ShortCut=TextToShortCut("Alt+");
        ->ShortCut=TextToShortCut("Alt+");
        ->ShortCut=TextToShortCut("Alt+");
        ->ShortCut=TextToShortCut("Alt+");
        */

    CurrOpt.DK=SBrwsAccWindowOptions::dkBoth;
    CurrOpt.ByDate=false;
    CurrOpt.Analitic=false;
    CurrOpt.SortByModifDate=false;
    CurrOpt.DetailedCorresp=false;
    CurrOpt.AccsTreeDepth=-1;

    // для неровного периода
    CurrOpt.PerNotPlain=false;
    CurrOpt.SyntOnly=true;
    AS_BegR=AS_EndR=AS_DTurn=AS_CTurn=0;

    FormName="ACCVIEW";
    DSetItems->SQL->Text=
    ReplaceBaseUser(
    "select *.GetDCLetter(side) AS ASIDE, "
         "*.User_Acc(a.name) AS ANAME, "
         "*.AnalInfo(op_id,op_subid,i.code) AS ANALINFO, VALUE "
         "from *.op_items i,*.accs a, *.sums "
         "where op_id = :op_id and op_subid = :op_subid AND i.code=a.code "
         "AND a.node=0 AND sum_id=item_sum_id "
         "ORDER BY ASIDE,A.NAME ");

    // Цвет каким закрашивать
    Dk=TColor(16049806);
}
//---------------------------------------------------------------------------
void TFrmBrwsAccWindow::Begin(int AccCode_, const AnsiString& Title_,
                                const AnsiString& Art, int Ver)
{
    BegTitle=Title_;
    CurrOpt.AccCode = AccCode_;

    AccsToWin.Add(this);

    CurrOpt.FldName="";
    CurrOpt.FilterArticle=Art;
    CurrOpt.FilterVer=Ver;
    CurrOpt.DateBeg=G.BegDate().T();
    CurrOpt.DateEnd=G.EndDate().T();
    int BAL;
    // Узнаем установки счета
    AmsQuery AQ;
    AQ.SetSqlText(
        //        0         1       2         3         4         5           6
        "select A.NAME, A.DESCR, O.NAME, A.ANAL_ID, O.PARENT, A.BAL_FLD, A.SETNAME, A.BAL from *.ACCS A, *.OBJS O "
        "where A.CODE=:CODE and A.ANAL_ID = O.OID (+)" );
    AQ.DeclVar("CODE", AccCode_);
    AQ.Exec("Узнаем информацию о счете");
    if (AQ.NotEof())
    {
        CurrOpt.AnName= AQ.GetFldStr(2);
        CurrOpt.AnCode= AQ.GetFldInt(3);
        CurrOpt.DbCode= AQ.GetFldInt(4);
        CurrOpt.BalFld= AQ.GetFldStr(5);
        CurrOpt.SetName= AQ.GetFldStr(6);

        BAL=AQ.GetFldInt(7);
        if (CurrOpt.FilterArticle!="")
        {
            BegTitle+=" "+TAccName::ToUserName(AQ.GetFldStr(0))+" "+AQ.GetFldStr(1);
            CurrOpt.FldName=GetFirstFieldName();
        }
    }
    // Для синтетических счетов прячем
    if (CurrOpt.AnName=="")
    {
        ActAnalEndRem   ->Visible=false;
        ActAnalEndRem   ->Enabled=false;
        ActFiltr        ->Visible=false;
        ActFiltr        ->Enabled=false;
        ActFieldName    ->Visible=false;
        ActFieldName    ->Enabled=false;
        ActAnaliticShow ->Visible=false;
        ActAnaliticShow ->Enabled=false;
        ActAnalChoose   ->Visible=false;
        ActAnalChoose   ->Enabled=false;
    }
    CurrOpt.OrderBy=SortTxt;
    //------------------------CH
    AmsQuery NQ;

    NQ.SetSqlText("select count(*) from "+BaseUserName+".op_items where code=:ACC_CODE and op_date>=:BEG_DATE and op_date<=:END_DATE");
    NQ.DeclVar("ACC_CODE", IntToStr(CurrOpt.AccCode));
    NQ.DeclVar("BEG_DATE", CurrOpt.DateBeg);
    NQ.DeclVar("END_DATE", CurrOpt.DateEnd);
    NQ.Exec("Получаем колво операций по счету");
    if (NQ.GetFldInt(0) && BAL)
      CurrOpt.DetailedCorresp=GC::ShowCorrAccs;//EO CH
    RefreshOpt();
    if (Top> 64)
        Top=64;
    WindowState=wsNormal;
    AccsToWin.Add(this);
}
//---------------------------------------------------------------------------
void TFrmBrwsAccWindow::MakeTitle(void)
{
    Caption=BegTitle+" ["+DateToStr(CurrOpt.DateBeg)+"-"+
        DateToStr(CurrOpt.DateEnd)+"] ";
    if (CurrOpt.FilterArticle!="")
    {
        //CH Filter WithoutVer
        if (CurrOpt.WithoutVer)
          Caption=Caption+" ["+CurrOpt.FilterArticle+" "+
            "Без Версий] ";
        else
          Caption=Caption+" ["+CurrOpt.FilterArticle+" "+
            IntToStr(CurrOpt.FilterVer)+"] ";//EO CH
    }
    if (CurrOpt.FldName!="")
    {
        Caption=Caption+" ["+CurrOpt.FldName+"]";
    }

    if (FilterSetted)
        StatusBar->Panels->Items[0]->Text="Выборка: "+FrmFilter->GetDescr();
    else
        StatusBar->Panels->Items[0]->Text="";

    if (GC::ViewDebug)
    {
        Caption=Caption + "["+IntToStr(CurrOpt.AccCode)+"] ";
    }

    if (HaveWhereTotal)
    {
        AFEnd->RowHeights[eefF5]=AFEnd->DefaultRowHeight;
        AFEnd->SetVal("Оборот выборки", eefF5, 1);
//--- CHANGED AS ANSWER ON QUESTION #3 MADE 13.03.02     OutTurns(AFEnd, WTDTurn, DtCol, eefF5);
//--- CHANGED AS ANSWER ON QUESTION #3 MADE 13.03.02     OutTurns(AFEnd, WTCTurn, CtCol, eefF5);
        AFEnd->Height= 3* (AFEnd->DefaultRowHeight+1);
    }
    else
    {
        AFEnd->RowHeights[eefF5]=0;
        AFEnd->Height= 2* (AFEnd->DefaultRowHeight+1);
    }



    AFBeg->SetVal("Остаток на "+DateToStr(CurrOpt.DateBeg), 0, 1);
    AFEnd->SetVal("Оборот с "+DateToStr(CurrOpt.DateBeg)+" по "+
                DateToStr(CurrOpt.DateEnd), eefTurns, 1);
    AFEnd->SetVal("Остаток на "+DateToStr(CurrOpt.DateEnd), eefRems, 1);
    // Выводим остатки и обороты
    OutDK(AFBeg, BegR, DtCol, CtCol, 0);
    OutDK(AFEnd, EndR, DtCol, CtCol, eefRems);
//--- CHANGED AS ANSWER ON QUESTION #3 MADE 13.03.02     OutTurns(AFEnd, DTurn, DtCol, eefTurns);
//--- CHANGED AS ANSWER ON QUESTION #3 MADE 13.03.02     OutTurns(AFEnd, CTurn, CtCol, eefTurns);
}

//---------------------------------------------------------------------------
void __fastcall TFrmBrwsAccWindow::ActViewOptExecute(TObject *Sender)
{
    // Изменяем режимы отображения
    TFrmViewOptionsAW* Frm;
    Application->CreateForm(__classid(TFrmViewOptionsAW), &Frm);
    if (Frm->ViewOptions(CurrOpt))
    {
        for (int j=0;j<AFEnd->ColCount;j++){AFEnd->SetVal("",3,j);}
        RefreshOpt();
    }
}
//---------------------------------------------------------------------------
void TFrmBrwsAccWindow::KeyEvent (WORD &Key, TShiftState Shift)
{
    if ( (Key==13 || Key==32) && !HaveOpId)
    {
        Key=0;
    }
    else
    {
        TFrmOperListRoot::KeyEvent(Key,Shift);
        if (Key)
        {
            if (Shift.Contains(ssAlt))
            {
                switch (Key)
                {
                    case 71:        // Alt+П печать
                        Key=0;
                        ActPrint->Execute();
                    break;
                    case 72:        // Режим видимости
                        Key=0;
                        ActViewOpt->Execute();
                    break;
                    case 65:        // Фильтр
                        ActFiltr->Execute();
                        Key=0;
                    break;
                    case 68:        // Выбор поля
                        Key=0;
                        ActFieldName->Execute();
                    break;
                    case 78:        // Alt+T Просмотр аналитики
                        Key=0;
                        ActAnaliticShow->Execute();
                    break;
                    case 70:        //Alt+А выбор типа аналитики
                        Key=0;
                        ActAnalChoose->Execute();
                    break;
                    case 74:        //Alt+0 остаток на конец
                        Key=0;
                        ActAnalEndRem->Execute();
                    break;
                    case 86:        //Alt+M генерация формы
                        Key=0;
                        ActRunForm->Execute();
                    break;
                }
            }
        }
    }
};
//---------------------------------------------------------------------------
AnsiString GetFldName (AnsiString FldName)
{
    int i = 0;
    while (i=FldName.Pos("/"))
    {
        FldName[i]='_';
    }
    return FldName;
}
//---------------------------------------------------------------------------
void TFrmBrwsAccWindow::BuildCols(AnsiString Simple)
{
    StornoFld=NULL;
    int W= Grid1->Canvas->TextWidth("0");
    int TotWidth =0;
    //---------------------------------------------------
    // Константы для ширины окна
    //---------------------------------------------------
    int WAnal=20;
    int WTextShort=10;
    int WText=28;
    int WOpDate=10;
    int WDocNum=10;
    int WDocType=8;
    int WSum=12;
    //---------------------------------------------------
    int AddW;
    TField* Fld;
    TColumn * Col;
    TDBGridColumns* Cols= Grid1->Columns;
    Cols->Clear();
    Grid1->AmsSynchoEnable=false;
    DSet->Active=false;
//-------------------------- CHANGED AS ANSWER ON QUESTION #3 MADE 13.03.02    ClrFooter();
    //TOracleDataSet* ODS=DSet;
    HaveDayRem=false;
    HaveOpId=true;
    DayIndex.clear();
    DayRemV.clear();
    DayRemV.push_back(0);


    DSet->Active= false;
    for (unsigned int i=0; i< Vec.size(); i++)
    {
        TField* N = Vec[i];
        delete N;
    }
    Vec.clear();

    DSetItems->Active=false;
    DSetItems->MasterFields="";
    DSetItems->Master=NULL;


    DSet->FieldDefs->Clear();
    DSet->Fields->Clear();

    // Выбираем результирующий остаток
    GetRem();

    int BegCol=0;
    int DtAcc = 0;
    int CtAcc = 0;
    DtCol=-1;
    CtCol=-1;

    ClrFooter();
    //Строим колонки для отображения нового запроса
    AmsQuery AQ;

    if (CurrOpt.DetailedCorresp)
    {
      AmsQuery NQ;

      NQ.SetSqlText("select count(*) from "+BaseUserName+".op_items where code=:ACC_CODE and op_date>=:BEG_DATE and op_date<=:END_DATE");
      NQ.DeclVar("ACC_CODE", IntToStr(CurrOpt.AccCode));
      NQ.DeclVar("BEG_DATE", CurrOpt.DateBeg);
      NQ.DeclVar("END_DATE", CurrOpt.DateEnd);
      NQ.Exec("Получаем колво операций по счету");
      if (NQ.GetFldInt(0)) {
        AnsiString Str=" ";
        DS_Q Q(AQ());
        AW.BuildRemQuery(Q, Str, DtAcc, CtAcc);
        Q.SetSqlText(Str);
        AQ.Exec("Итого строка для развернутой корреспонденции счетов");
      } 
    };



    if (CurrOpt.ByDate)
    {   // Группируем по датам
        BegCol=2;
        //------------------------------------------------------------
        Grid2->Visible=false;
        Label1->Visible=false;
        DSetItems->Active=false;
        HaveOpId= false;
        HaveDayRem= true;
        //------------------------------------------------------------
        OpDateFld = CreateDateField("OP_DATE");
        TotWidth += CreateDateCol(OpDateFld, "Дата", WOpDate*W+50);
        //--------------------------------------------------
        int j = 0;
        if (CurrOpt.DK & SBrwsAccWindowOptions::dkDebet)
        {
            DSum = CreateSumField("DSUM");
            TotWidth+=CreateSumCol(DSum,"Дебет",WSum*W);
            if (CurrOpt.DetailedCorresp)
            {
                for (int i=0; i<DtAcc; i++, j++)
                {
                    TFloatField* T = CreateSumField(AQ()->FieldName(j));

                    TotWidth += CreateSumCol(T, AQ()->FieldName(j),WSum*W);
                }
            }
            DtCol=BegCol;
            BegCol=DtCol+DtAcc+1;
        }
        if (CurrOpt.DK & SBrwsAccWindowOptions::dkCredit)
        {
            CSum = CreateSumField("CSUM");
            TotWidth+=CreateSumCol(CSum,"Кредит",WSum*W);
            if (CurrOpt.DetailedCorresp)
            {
                for (int i=0; i<CtAcc; i++, j++)
                {
                    TFloatField* T = CreateSumField(AQ()->FieldName(j));

                    TotWidth += CreateSumCol(T, AQ()->FieldName(j),WSum*W);
                }
            }
            CtCol=BegCol;
        }
        //--------------------------------------------------
        if (CurrOpt.DK == SBrwsAccWindowOptions::dkBoth)
        {
            DayRemD=CreateSumField("DAY_REM_D");
            DayRemD->Calculated=true;
            TotWidth+=CreateSumCol(DayRemD,"Дт ост конец дня",WSum*W);

            DayRemC=CreateSumField("DAY_REM_C");
            DayRemC->Calculated=true;
            TotWidth+=CreateSumCol(DayRemC,"Кт ост конец дня",WSum*W);
            CurrRem=BegR;
            HaveDayRem=true;
        }


    }
    else
    {   //   Не группируем по датам
        //------------------------------------------------------------
        Grid2->Visible=true;
        Label1->Visible=true;
        HaveOpId= true;
        HaveDayRem= false;
        //------------------------------------------------------------
        BegCol=5;
        Fld=CreateSumField("OP_ID");
        Fld=CreateSumField("OP_SUBID");
        Fld=CreateSumField("ITEM_SUM_ID");
        Fld=CreateTxtField("SETNAME",30);
        if (CurrOpt.Analitic)
        {
            BegCol++;
            Fld = CreateTxtField("ANAL");
            TotWidth += CreateTxtCol(Fld, "Аналитика", WAnal*W);
        }
        Fld = CreateTxtField("TEXT");
        TotWidth += CreateTxtCol(Fld,"Текст",CurrOpt.Analitic? WTextShort*W :WText*W);
        Fld = CreateTxtField("DOC_TYPE");
        TotWidth += CreateTxtCol(Fld,"Документ", WDocType*W);
        Fld = CreateTxtField("DOC_NUM");
        TotWidth += CreateTxtCol(Fld,"Номер",WDocNum*W);
        Fld = CreateDateField("OP_DATE");
        TotWidth += CreateDateCol(Fld, "Дата", WOpDate*W);
        int j = 0;
        if (CurrOpt.DK & SBrwsAccWindowOptions::dkDebet)
        {
            DtCol=BegCol++;
            BegCol+=DtAcc;
            DSum=CreateSumField("DSUM");
            TotWidth +=CreateSumCol(DSum,"Дебет",WSum*W);
            if (CurrOpt.DetailedCorresp)
            {
                for (int i=0; i<DtAcc; i++, j++)
                {
                    TFloatField* T = CreateSumField(AQ()->FieldName(j));

                    TotWidth +=CreateSumCol(T,AQ()->FieldName(j),WSum*W);
                }
            }
        }
        j=DtAcc;
        if (CurrOpt.DK & SBrwsAccWindowOptions::dkCredit)
        {
            CtCol=BegCol;
            CSum =CreateSumField("CSUM");
            TotWidth +=CreateSumCol(CSum,"Кредит",WSum*W);
            if (CurrOpt.DetailedCorresp)
            {
                for (int i=0; i<CtAcc; i++, j++)
                {
                    TFloatField* T = CreateSumField(AQ()->FieldName(j));

                    TotWidth +=CreateSumCol(T,AQ()->FieldName(j),WSum*W);
                }
            }
        } // Have credit

        if (CurrOpt.DetailedCorresp || CurrOpt.WithoutVer)
            StornoFld=CreateIntField("STORNO");
        else
            StornoFld=CreateInt_Field("STORNO");

    } // Date no Group
    AnsiString SAVE = DSet->SQL->Text;
    AnsiString SIMPLE=Simple+" from "+BaseUserName+".ADD_OPITEMS";
    for (int i= 0; i< DSet->VariableCount(); i++)
    {
        if (!i) SIMPLE+=" where ";
        else    SIMPLE+=" and ";
        SIMPLE+= DSet->VariableName(i)+"="+ DSet->VariableName(i);
    };

    DSet->SQL->Text=SIMPLE;
    ExecODS(DSet);
    DSet->Active=false;
    DSet->FieldDefs->Update();
    Grid1->AmsSynchoEnable=true;
    DSet->SQL->Text=SAVE;
    ExecODS(DSet);


/*-------------------------- CHANGED AS ANSWER ON QUESTION #3 MADE 13.03.02


    int j=0;
    if ((CurrOpt.DK & SBrwsAccWindowOptions::dkDebet)&&CurrOpt.DetailedCorresp)
    {
        int CurrCol = DtCol+1;
        for (int i=0; i<DtAcc;i++)
        {
            AFEnd->SetVal(AQ.GetFldSum(j++), eefTurns, CurrCol++);
        }
    }
    j=DtAcc;                                                                                        `
    if ((CurrOpt.DK & SBrwsAccWindowOptions::dkCredit)&&CurrOpt.DetailedCorresp)
    {
        int CurrCol = CtCol+1;
        for (int i=0; i<CtAcc;i++)
        {
            AFEnd->SetVal(AQ.GetFldSum(j++), eefTurns, CurrCol++);
        }
    }

--------------------------EO CHANGED AS ANSWER ON QUESTION #3 MADE 13.03.02*/

    if (HaveOpId)
    {
        DSetItems->Master=DSet;
        DSetItems->MasterFields="OP_ID;OP_SUBID";
        DSetItems->Active=true;
    }
    if (CurrOpt.DetailedCorresp)
    {
        AFEnd->SetColor(CtCol, Dk);
        AFEnd->SetColor(DtCol, Dk);
        AFBeg->SetColor(CtCol, Dk);
        AFBeg->SetColor(DtCol, Dk);
    }

    CalcSize(TotWidth);
    MakeTitle();
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrwsAccWindow::Grid1AmsFooterSyncho(TAmsDBGrid *Sender,
      SSynchoCause &Cause)
{
    //
    AFBeg->SynChro (Sender, Cause);
    AFEnd->SynChro (Sender, Cause);
}
//---------------------------------------------------------------------------
void TFrmBrwsAccWindow::OutDK(TAmsFooter* F, double Val, int DtCol, int CtCol, int Row)
{
    if (Val>0)
    {
        if (DtCol>0)
            F->SetVal(Val, Row, DtCol);
        if (CtCol>0)
            F->SetVal("", Row, CtCol);
    }
    else
    if (Val<0)
    {
        if (CtCol>0)
            F->SetVal(-Val, Row, CtCol);
        if (DtCol>0)
            F->SetVal("", Row, DtCol);
    }
    else
    {
        if (DtCol>0)
            F->SetVal("", Row, DtCol);
        if (CtCol>0)
            F->SetVal("", Row, CtCol);
    }
}
//---------------------------------------------------------------------------
TFrmBrwsAccWindow::OutTurns(TAmsFooter* F, double Val, int Col, int Row)
{
    if (Val)
    {
        if (Col>0)
            F->SetVal(Val, Row, Col);
    }
    else
    {
        if (Col>0)
            F->SetVal("", Row, Col);
    }
    return 0;
}
//---------------------------------------------------------------------------
void TFrmBrwsAccWindow::ClrFooter()
{
   for (int j=0;j<AFBeg->ColCount;j++){
          AFBeg->SetVal("",0,j);
      }
    for (int j=0;j<AFEnd->ColCount;j++){
          AFEnd->SetVal("",4,j);
      }
    /*for (int i=0; i<AFBeg->RowCount;i++)
        for (int j=0; j < AFBeg->ColCount; j++)
             AFBeg->SetVal("", i, j);
    for (int i=0; i<AFEnd->RowCount;i++)
        for (int j=0; j < AFEnd->ColCount; j++)
             AFEnd->SetVal("", i, j);
             */
}
//---------------------------------------------------------------------------
void TFrmBrwsAccWindow::CalcSize(int GridWidth)
{
    int MaxWidth=AmsWinC->Width;
    int Add= 40;

    int GrWidth = 0;
    TDBGridColumns* Cols=Grid2->Columns;
    for (int i= 0; i< (Cols->Count-1); i++)
    {
        GrWidth+= Cols->Items[i]->Width+1;
    }
    TColumn* Col =Cols->Items[Cols->Count-1];

    if (MaxWidth< (Left+GridWidth+Add))
    {
        Left=8;
        if (MaxWidth< (Left+GridWidth+Add))
        {
            Width=MaxWidth-Left-8;
        }
        else
        {
            Width=GridWidth+Add;
        }
    }
    else
    {
            Width=GridWidth+Add;
    }
    Col->Width= Width-GrWidth-Add+11;
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrwsAccWindow::Grid1GetCellParams(TObject *Sender,
      TField *Field, TFont *AFont, TColor &Background, bool Highlight)
{
    if (StornoFld)
    {
        if (StornoFld->AsInteger!=0)
        {
            AFont->Color=Highlight? clLime : clRed;
            AFont->Style<<fsBold;
            if (!Highlight) Background=clYellow;
        }
    }
    if (!Highlight && CurrOpt.DetailedCorresp && Background!=clYellow)
    {
        if (Field==DSum || Field==CSum)
        {
            Background=Dk;//clTeal;
        }
    }

}
//---------------------------------------------------------------------------
void __fastcall TFrmBrwsAccWindow::DSetCalcFields(TDataSet *DataSet)
{
    if(HaveDayRem)
    {
        int Indx= DayIndex[OpDateFld->Value];
        if (! Indx)
        {
            double Delta= DSum->Value - CSum->Value;
            CurrRem= CurrRem + Delta;
            Indx=DayRemV.size();
            DayIndex[OpDateFld->Value]=Indx;
            DayRemV.push_back(CurrRem);
        }
        if ( ((double)DayRemV[Indx]) > 0)
        {
            DayRemD->Value=DayRemV[Indx];
        }
        else
        if ( ((double)DayRemV[Indx])<0)
        {
            DayRemC->Value=-DayRemV[Indx];
        }

    }
}
//---------------------------------------------------------------------------
TFloatField* TFrmBrwsAccWindow::CreateSumField(const AnsiString& FldName)
{
    TFloatField* T = new TFloatField(NULL);
    Vec.push_back(T);
    T->FieldName = FldName;
    T->Name = GetFldName(DSet->Name + T->FieldName);
    T->Index = DSet->FieldCount;
    T->DataSet = DSet;
    T->DisplayFormat=",0.00###";
    return T;
};
//---------------------------------------------------------------------------
int TFrmBrwsAccWindow::CreateSumCol (TFloatField* Fld, const AnsiString& ColTitle, int Width)
{
    int AddW = Width+6;
    TDBGridColumns*Cols= Grid1->Columns;
    TColumn* Col = Cols->Add();
    Col->Field= Fld;
    Col->Title->Caption = ColTitle;
    Col->Width = AddW-1;
    return AddW;
};
//---------------------------------------------------------------------------
TDateTimeField* TFrmBrwsAccWindow::CreateDateField(const AnsiString& FldName)
{
    TDateTimeField* DT = new TDateTimeField(NULL);
    Vec.push_back(DT);
    DT->FieldName = FldName;
    DT->Name = DSet->Name + DT->FieldName;
    DT->Index = DSet->FieldCount;
    DT->DataSet = DSet;
    return DT;
};
//---------------------------------------------------------------------------
int TFrmBrwsAccWindow::CreateDateCol (TField* Fld, const AnsiString& ColTitle, int Width)
{
    int AddW = Width;
    TDBGridColumns*Cols= Grid1->Columns;
    TColumn* Col = Cols->Add();
    Col->Field= Fld;
    Col->Title->Caption = ColTitle;
    Col->Width = AddW+5;
    return AddW+6;
};
//---------------------------------------------------------------------------
TField* TFrmBrwsAccWindow::CreateTxtField(const AnsiString& FldName, int MaxSize)
{
    TStringField* T = new TStringField(NULL);
    Vec.push_back(T);
    T->FieldName = FldName;
    T->Name = DSet->Name + T->FieldName;
    T->Index = DSet->FieldCount;
    T->DataSet = DSet;
    T->Size= MaxSize;
    return T;
};
//---------------------------------------------------------------------------
int TFrmBrwsAccWindow::CreateTxtCol (TField* Fld, const AnsiString& ColTitle, int Width)
{
    int AddW = Width;
    TDBGridColumns*Cols= Grid1->Columns;
    TColumn* Col = Cols->Add();
    Col->Field= Fld;
    Col->Title->Caption = ColTitle;
    Col->Width = AddW+5;
    return AddW+6;
};
//---------------------------------------------------------------------------
TField* TFrmBrwsAccWindow::CreateIntField(const AnsiString& FldName)
{
    TFloatField* DT = new TFloatField(NULL);
    Vec.push_back(DT);
    DT->FieldName = FldName;
    DT->Name = DSet->Name + DT->FieldName;
    DT->Index = DSet->FieldCount;
    DT->DataSet = DSet;
    return DT;
};
//---------------------------------------------------------------------------
int TFrmBrwsAccWindow::CreateIntCol (TField* Fld, const AnsiString& ColTitle, int Width)
{
    int AddW = Width;
    TDBGridColumns*Cols= Grid1->Columns;
    TColumn* Col = Cols->Add();
    Col->Field= Fld;
    Col->Title->Caption = Width;
    Col->Width = AddW+5;
    return AddW+6;
};
//---------------------------------------------------------------------------
TIntegerField* TFrmBrwsAccWindow::CreateInt_Field(const AnsiString& FldName)
{
    TIntegerField* DT = new TIntegerField(NULL);
    Vec.push_back(DT);
    DT->FieldName = FldName;
    DT->Name = DSet->Name + DT->FieldName;
    DT->Index = DSet->FieldCount;
    DT->DataSet = DSet;
    return DT;
};
//---------------------------------------------------------------------------
int TFrmBrwsAccWindow::CreateInt_Col (TField* Fld, const AnsiString& ColTitle, int Width)
{
    int AddW = Width;
    TDBGridColumns*Cols= Grid1->Columns;
    TColumn* Col = Cols->Add();
    Col->Field= Fld;
    Col->Title->Caption = Width;
    Col->Width = AddW+5;
    return AddW+6;
};
//---------------------------------------------------------------------------

void __fastcall TFrmBrwsAccWindow::ActFiltrExecute(TObject *Sender)
{
    // Устанавливаем фильтр по позиции
    if (CurrOpt.AnName=="")
    {
        ActFieldName->Visible= false;
        ActFiltr->Visible= false;
        return;
    }
    int Vers=CurrOpt.WithoutVer;//EO CH Filter WithoutVer
    if (GetDbPos(CurrOpt.DbCode,Vers, CurrOpt.FilterArticle, CurrOpt.FilterVer, ""))
    {

        /*----------  CHANGED AS ANSWER ON QUESTION #3 MADE 13.03.02
        CurrOpt.DetailedCorresp=false;

        */
        FilterSetted=false;
        //----------EO CHANGED AS ANSWER ON QUESTION #3 MADE 13.03.02


        CurrOpt.WithoutVer=Vers;
        if (CurrOpt.FldName=="")
            CurrOpt.FldName=GetFirstFieldName();
        RefreshOpt();
    }
    else
    {
        CurrOpt.WithoutVer=0;
        CurrOpt.FilterArticle="";
        RefreshOpt();
    }
}
//---------------------------------------------------------------------------
bool IsNotSystem (TOracleQuery* OQ)
{
    AmsQuery AQ(OQ);
    AnsiString Res=AQ.GetFldStr(1).UpperCase();
    return ! ((Res=="AARTICLE")||(Res=="AVER")||(Res=="SUMID"));
}

void __fastcall TFrmBrwsAccWindow::ActFieldNameExecute(TObject *Sender)
{
//
    if (CurrOpt.AnName=="")
    {
        ActFieldName->Visible= false;
        ActFiltr->Visible= false;
        return;
    }
    do {
        AmsQuery AQ;
        AQ.SetSqlText("select S.ALIAS, S.REALNAME, nvl(O.OPT,0) OPT  "
         "from *.DBSTR S, *.DBOPTOV O "
         "where "
         "S.DBID = (select OID from *.OBJS where NAME = :NAME) and "
         "S.DBID = O.DBID (+) and "
         "S.DELETED = 0 and "
         "O.SETNAME (+) is NULL and "
         "S.REALNAME = O.REALNAME (+) order by S.ORD");
        AQ.DeclVar("NAME", CurrOpt.AnName);
        AQ.Exec("Поля базы данных или аналитики");
        TFrmSelList* Frm ;
        Application->CreateForm( __classid(TFrmSelList), &Frm);
        Frm->Caption="";
        AnsiString Res = Frm->AskFieldStr2(AQ(), "Поле для показа", IsNotSystem);
        Frm->Release();
        if (Res=="")
            return;
        CurrOpt.FldName=Res;
        CurrOpt.DetailedCorresp=false;
    } while (0);
    RefreshOpt();
}
//---------------------------------------------------------------------------
void TFrmBrwsAccWindow::RefreshOpt()
{
    AnsiString Str=" ";
    AnsiString Simple=" ";
    DS_Q Q(DSet);
    try {
      BuildFullSql(Str, Q, Simple);
      Q.SetSqlText(Str);
      CountSelection();
      BuildCols(Simple);
      DumbSetFooter();
    } catch (...) {
      //ClrFooter();
    }
    if (G.ur.set_to_end)
        DSet->Last();else DSet->First();
}
//---------------------------------------------------------------------------
void TFrmBrwsAccWindow::GetRem()
{
    bool NeedSelect=true;
    /*
    if (!NeedSelect && LastRemOpt.FldName!=CurrOpt.FldName)
        NeedSelect=true;
    if (!NeedSelect && LastRemOpt.BegDate!=CurrOpt.DateBeg)
        NeedSelect=true;
    if (!NeedSelect && LastRemOpt.EndDate!=CurrOpt.DateEnd)
        NeedSelect=true;
    if (!NeedSelect && LastRemOpt.Article!=CurrOpt.FilterArticle)
        NeedSelect=true;
    if (!NeedSelect && LastRemOpt.Ver!=CurrOpt.FilterVer)
        NeedSelect=true;
    */
    if (NeedSelect)
    {
        AmsQuery AQ;
        if (CurrOpt.FldName=="")
        {
            if (!CurrOpt.PerNotPlain)
            {
                AQ.SetSqlText(
                "begin "
                    " select max(SC.VALUE), max(SD.VALUE) into :CT, :DT"
                    " from *.TURNS T, *.SUMS SC, *.SUMS SD"
                    " where T.CODE=:CODE and T.BEG_DATE=:BEG_DATE and"
                        " T.END_DATE=:END_DATE and"
                        " T.D_TURN_ID=SD.SUM_ID and"
                        " T.C_TURN_ID=SC.SUM_ID;"
                    " select max(S.VALUE) into :BR"
                    " from *.REMS R, *.SUMS S"
                    " where R.CODE=:CODE and R.REM_DATE=:BEG_DATE and"
                        " R.END_VAL_ID=S.SUM_ID;"
                    " select max(S.VALUE) into :ER"
                    " from *.REMS R, *.SUMS S"
                    " where R.CODE=:CODE and R.REM_DATE=:END_DATE and"
                        " R.END_VAL_ID=S.SUM_ID;"
                "end;");
            }
            else
            {
                AQ.SetSqlText(
                "begin "
                    " *.V_RT (:BR, :DT, :CT, :ER, :CODE, :BEG_DATE, :END_DATE);"
                "end;");
            };
        }
        else  // FldName!=''
        {
            if (!CurrOpt.PerNotPlain)
            {
                AnsiString Ss;

                if (CurrOpt.WithoutVer) {
                  Ss = "begin "
                    " declare vDtSumId number; vCtSumId number;"
                    " begin "
                        " select T.D_TURN_ID, T.C_TURN_ID into vDtSumId, vCtSumId"
                        " from *.TURNS T"
                        " where T.CODE=:CODE and T.BEG_DATE=:BEG_DATE and"
                            " T.END_DATE=:END_DATE;"
                        " select sum(SC."+CurrOpt.FldName+") into :CT "
                        "from *."+CurrOpt.AnName+" SC "
                        "where SC.SUMID=vCtSumId and SC.AARTICLE=:ARTICLE  "//and SC.AVER=:VER;"
                        +(CurrOpt.WithoutVer?AnsiString(";\r\n"):AnsiString("and SC.AVER=:VER;\r\n"))+
                        " select sum(SC."+CurrOpt.FldName+") into :DT "
                        " from *."+CurrOpt.AnName+" SC "
                        "where SC.SUMID=vDtSumId and SC.AARTICLE=:ARTICLE "//and SC.AVER=:VER;"
                        +(CurrOpt.WithoutVer?AnsiString(";\r\n"):AnsiString(" and SC.AVER=:VER;\r\n"))+
                        " select sum(S."+CurrOpt.FldName+") into :BR"
                        " from *.REMS R, *."+CurrOpt.AnName+" S"
                        " where R.CODE=:CODE and R.REM_DATE=:BEG_DATE and"
                            " R.END_VAL_ID=S.SUMID and S.AARTICLE=:ARTICLE "//and S.AVER=:VER;"
                        +(CurrOpt.WithoutVer?AnsiString(";\r\n"):AnsiString("and S.AVER=:VER;\r\n"))+
                        " select sum(S."+CurrOpt.FldName+") into :ER"
                        " from *.REMS R, *."+CurrOpt.AnName+" S"
                        " where R.CODE=:CODE and R.REM_DATE=:END_DATE and"
                            " R.END_VAL_ID=S.SUMID and S.AARTICLE=:ARTICLE "//and S.AVER=:VER;"
                        +(CurrOpt.WithoutVer?AnsiString(";\r\n"):AnsiString("and S.AVER=:VER;\r\n"))+
                    "end;end;";
                } else {
                  Ss = "begin "
                    " declare vDtSumId number; vCtSumId number;"
                    " begin "
                        " select T.D_TURN_ID, T.C_TURN_ID into vDtSumId, vCtSumId"
                        " from *.TURNS T"
                        " where T.CODE=:CODE and T.BEG_DATE=:BEG_DATE and"
                            " T.END_DATE=:END_DATE;"
                        " select max(SC."+CurrOpt.FldName+") into :CT "
                        "from *."+CurrOpt.AnName+" SC "
                        "where SC.SUMID=vCtSumId and SC.AARTICLE=:ARTICLE  "//and SC.AVER=:VER;"
                        +(CurrOpt.WithoutVer?AnsiString(";\r\n"):AnsiString("and SC.AVER=:VER;\r\n"))+
                        " select max(SC."+CurrOpt.FldName+") into :DT "
                        " from *."+CurrOpt.AnName+" SC "
                        "where SC.SUMID=vDtSumId and SC.AARTICLE=:ARTICLE and SC.AVER=:VER;"
                        " select max(S."+CurrOpt.FldName+") into :BR"
                        " from *.REMS R, *."+CurrOpt.AnName+" S"
                        " where R.CODE=:CODE and R.REM_DATE=:BEG_DATE and"
                            " R.END_VAL_ID=S.SUMID and S.AARTICLE=:ARTICLE "//and S.AVER=:VER;"
                        +(CurrOpt.WithoutVer?AnsiString(";\r\n"):AnsiString("and S.AVER=:VER;\r\n"))+
                        " select max(S."+CurrOpt.FldName+") into :ER"
                        " from *.REMS R, *."+CurrOpt.AnName+" S"
                        " where R.CODE=:CODE and R.REM_DATE=:END_DATE and"
                            " R.END_VAL_ID=S.SUMID and S.AARTICLE=:ARTICLE "//and S.AVER=:VER;"
                        +(CurrOpt.WithoutVer?AnsiString(";\r\n"):AnsiString("and S.AVER=:VER;\r\n"))+
                    "end;end;";
                }

                AQ.SetSqlText(Ss);

            }
            else // Period not plain and Have FLD
            {
                AQ.SetSqlText("begin \r\ndeclare "
                    "pBR number;\r\n"
                    "pDT number;\r\n"
                    "pCT number;\r\n"
                    "pER number;\r\n"
                    "pAccCode number:=:CODE;\r\n"
                    "pDtBeg date:=:BEG_DATE;\r\n"
                    "pDtEnd date:=:END_DATE;\r\n"
                    "vBeg date;\r\n"
                    "vEnd date;\r\n"
                    "vTmp1 number;\r\n"
                    "vTmp2 number;\r\n"
                    "vTmp0 number;\r\n"
                    "vDT number:=0;\r\n"
                    "vCT number:=0;\r\n"
                    "vBeg1 date;\r\n"
                    "vEnd1 date;\r\n"
                "begin\r\n"
                    "-- Находим остаток на начало\r\n"
                    "select BEG_DATE, END_DATE into vBeg, vEnd\r\n"
                    "from *.CINTS\r\n"
                    "where\r\n"
                    "    (END_DATE-BEG_DATE)<35 and\r\n"
                    "    pDtBeg between BEG_DATE and END_DATE;\r\n"
                    "if vBeg=pDtBeg or vEnd=pDtBeg then\r\n"
                    "    select "+CurrOpt.FldName+" into pBR from *."+CurrOpt.AnName+"\r\n"
                    "    where SUMID =\r\n"
                    "        (select END_VAL_ID from *.REMS\r\n"
                    "         where CODE=pAccCode and REM_DATE=pDtBeg) and\r\n"
                    "         AARTICLE=:ARTICLE " +
                    (CurrOpt.WithoutVer?AnsiString(";\r\n"):AnsiString("and AVER=:VER;\r\n"))+
                    "else\r\n"
                    "    --- Определяем куда дальше считать\r\n"
                    "    select count(*) into vTmp1 from *.OP_ITEMS OI, *.OPERS O\r\n"
                    "    where\r\n"
                    "        OI.CODE=pAccCode and\r\n"
                    "        OI.OP_DATE>= vBeg and OI.OP_DATE< pDtBeg and\r\n"
                    "        OI.OP_ID = O.OP_ID and\r\n"
                    "        OI.OP_SUBID = O.OP_SUBID and\r\n"
                    "        O.STATUS= 1 and\r\n"
                    "        O.POST= 0;\r\n"
                    "    select count(*) into vTmp2\r\n"
                    "    from *.OP_ITEMS OI, *.OPERS O\r\n"
                    "    where\r\n"
                    "        OI.CODE=pAccCode and\r\n"
                    "        OI.OP_DATE>= pDtBeg and OI.OP_DATE< vEnd and\r\n"
                    "        OI.OP_ID = O.OP_ID and\r\n"
                    "        OI.OP_SUBID = O.OP_SUBID and\r\n"
                    "        O.STATUS= 1 and\r\n"
                    "        O.POST= 0;\r\n"
                    "    -- Лучше посчитать до конца\r\n"
                    "    if vTmp1> vTmp2 then\r\n"
                    "        select sum(*.GetAccCorTotValue ('D', OI.SIDE, O.STORNO, S."+CurrOpt.FldName+")),\r\n"
                    "               sum(*.GetAccCorTotValue ('K', OI.SIDE, O.STORNO, S."+CurrOpt.FldName+"))\r\n"
                    "        into vDT, vCT\r\n"
                    "        from *.OP_ITEMS OI, *.OPERS O, *."+CurrOpt.AnName+" S\r\n"
                    "        where\r\n"
                    "            OI.CODE=pAccCode and\r\n"
                    "            OI.OP_DATE>= pDtBeg and OI.OP_DATE< vEnd and\r\n"
                    "            OI.OP_ID = O.OP_ID and\r\n"
                    "            OI.OP_SUBID = O.OP_SUBID and\r\n"
                    "            O.STATUS= 1 and\r\n"
                    "            O.POST= 0 and S.SUMID=OI.ITEM_SUM_ID and\r\n"
                    "         S.AARTICLE=:ARTICLE " +
                    (CurrOpt.WithoutVer?AnsiString(";\r\n"):AnsiString("and S.AVER=:VER;\r\n"))+

                    "        vDT:=nvl(vDT,0);\r\n"
                    "        vCT:=nvl(vCT,0);\r\n"
                    "       -- Находим остаток на конец\r\n"
                    "        select "+CurrOpt.FldName+" into vTmp1 from *."+CurrOpt.AnName+"\r\n"
                    "        where SUMID =\r\n"
                    "            (select END_VAL_ID from *.REMS\r\n"
                    "             where CODE=pAccCode and REM_DATE=vEnd) and \r\n"
                    "         AARTICLE=:ARTICLE " +
                    (CurrOpt.WithoutVer?AnsiString(";\r\n"):AnsiString("and AVER=:VER;\r\n"))+

                    "        pBR:=vTmp1-vDT+vCT;\r\n"
                    "    else\r\n"
                    "    -- Лучше считать до начала\r\n"
                    "        select sum(*.GetAccCorTotValue ('D', OI.SIDE, O.STORNO, S."+CurrOpt.FldName+")),\r\n"
                    "               sum(*.GetAccCorTotValue ('K', OI.SIDE, O.STORNO, S."+CurrOpt.FldName+"))\r\n"
                    "        into vDT, vCT\r\n"
                    "        from *.OP_ITEMS OI, *.OPERS O, *."+CurrOpt.AnName+" S\r\n"
                    "        where\r\n"
                    "            OI.CODE=pAccCode and\r\n"
                    "            OI.OP_DATE>= vBeg and OI.OP_DATE< pDtBeg and\r\n"
                    "            OI.OP_ID = O.OP_ID and\r\n"
                    "            OI.OP_SUBID = O.OP_SUBID and\r\n"
                    "            O.STATUS= 1 and\r\n"
                    "            O.POST= 0 and S.SUMID=OI.ITEM_SUM_ID and\r\n"
                    "         S.AARTICLE=:ARTICLE " +
                    (CurrOpt.WithoutVer?AnsiString(";\r\n"):AnsiString( "and S.AVER=:VER;\r\n"))+

                    "        vDT:=nvl(vDT,0);\r\n"
                    "        vCT:=nvl(vCT,0);\r\n"
                    "        -- Находим остаток на конец\r\n"
                    "        select "+CurrOpt.FldName+" into vTmp0 from *."+CurrOpt.AnName+"\r\n"
                    "        where SUMID =\r\n"
                    "            (select END_VAL_ID from *.REMS\r\n"
                    "             where CODE=pAccCode and REM_DATE=vEnd) and\r\n"
                    "         AARTICLE=:ARTICLE " +
                    (CurrOpt.WithoutVer?AnsiString(";\r\n"):AnsiString("and AVER=:VER;\r\n"))+

                    "        --находим обороты\r\n"
                    "        select max(SD."+CurrOpt.FldName+"), max(SC."+CurrOpt.FldName+") into vTmp1, vTmp2\r\n"
                    "        from *.TURNS T, *."+CurrOpt.AnName+" SD, *."+CurrOpt.AnName+" SC\r\n"
                    "        where\r\n"
                    "            T.BEG_DATE=vBeg and\r\n"
                    "            T.END_DATE=vEnd and\r\n"
                    "            T.CODE = pAccCode and\r\n"
                    "            T.D_TURN_ID=SD.SUMID and\r\n"
                    "            T.C_TURN_ID=SC.SUMID and\r\n"
                    "         SD.AARTICLE=:ARTICLE " +
                    (CurrOpt.WithoutVer?AnsiString("and\r\n"):AnsiString("and SD.AVER=:VER and\r\n"))+
                    "         SC.AARTICLE=:ARTICLE " +
                    (CurrOpt.WithoutVer?AnsiString(";\r\n"):AnsiString("and SC.AVER=:VER;\r\n"))+


                    "        vDT:=nvl(vTmp1,0)-vDT;\r\n"
                    "        vCT:=nvl(vTmp2,0)-vCT;\r\n"
                    "        pBR:=vTmp0-vDT+vCT;\r\n"
                    "    end if; -- Считаем до начала или конца\r\n"
                    "end if;  -- Точно попали в начало периода\r\n"
                    "--------------------------------------------------\r\n"
                    "-- Находим обороты\r\n"
                    "--------------------------------------------------\r\n"
                    "if vEnd<pDtEnd then\r\n"
                    "    vTmp0:=0;\r\n"
                    "    vBeg:=vEnd;\r\n"
                    "    while (vTmp0=0) loop\r\n"
                    "        select max(END_DATE) into vEnd from *.CINTS\r\n"
                    "        where   BEG_DATE=vBeg and\r\n"
                    "                END_DATE<=pDtEnd;\r\n"
                    "        if vEnd is NULL then\r\n"
                    "            vTmp0:=1;\r\n"
                    "        else\r\n"
                    "            --находим обороты\r\n"
                    "            select max(SD."+CurrOpt.FldName+"), max(SC."+CurrOpt.FldName+") into vTmp1, vTmp2\r\n"
                    "            from *.TURNS T, *."+CurrOpt.AnName+" SD, *."+CurrOpt.AnName+" SC\r\n"
                    "            where\r\n"
                    "                T.BEG_DATE=vBeg and\r\n"
                    "                T.END_DATE=vEnd and\r\n"
                    "                T.CODE = pAccCode and\r\n"
                    "                T.D_TURN_ID=SD.SUMID and\r\n"
                    "                T.C_TURN_ID=SC.SUMID and\r\n"
                    "         SD.AARTICLE=:ARTICLE " +
                    (CurrOpt.WithoutVer?AnsiString("and\r\n"):AnsiString("and SD.AVER=:VER and\r\n"))+
                    "         SC.AARTICLE=:ARTICLE " +
                    (CurrOpt.WithoutVer?AnsiString(";\r\n"):AnsiString("and SC.AVER=:VER;\r\n"))+
                    "            vDT:=nvl(vTmp1,0)+vDT;\r\n"
                    "            vCT:=nvl(vTmp2,0)+vCT;\r\n"
                    "            vBeg:=vEnd;\r\n"
                    "        end if;\r\n"
                    "    end loop;\r\n"
                    "    --- Находим остаток оборотов\r\n"
                    "    if (vBeg!=pDtEnd) then\r\n"
                    "        select sum(*.GetAccCorTotValue ('D', OI.SIDE, O.STORNO, S."+CurrOpt.FldName+")),\r\n"
                    "               sum(*.GetAccCorTotValue ('K', OI.SIDE, O.STORNO, S."+CurrOpt.FldName+"))\r\n"
                    "        into vTmp1, vTmp2\r\n"
                    "        from *.OP_ITEMS OI, *.OPERS O, *."+CurrOpt.AnName+" S\r\n"
                    "        where\r\n"
                    "            OI.CODE=pAccCode and\r\n"
                    "            OI.OP_DATE>= vBeg and OI.OP_DATE< pDtEnd and\r\n"
                    "            OI.OP_ID = O.OP_ID and\r\n"
                    "            OI.OP_SUBID = O.OP_SUBID and\r\n"
                    "            O.STATUS= 1 and\r\n"
                    "            O.POST= 0 and S.SUMID=OI.ITEM_SUM_ID and\r\n"
                    "         AARTICLE=:ARTICLE " +
                    (CurrOpt.WithoutVer?AnsiString(";\r\n"):AnsiString("and AVER=:VER;\r\n"))+
                    "        vDT:=vDT+nvl(vTmp1,0);\r\n"
                    "        vCT:=vCt+nvl(vTmp2,0);\r\n"
                    "    end if;\r\n"
                    "else\r\n"
                    "    -- Находим обороты между двумя датами\r\n"
                    "    select sum(*.GetAccCorTotValue ('D', OI.SIDE, O.STORNO, S."+CurrOpt.FldName+")),\r\n"
                    "           sum(*.GetAccCorTotValue ('K', OI.SIDE, O.STORNO, S."+CurrOpt.FldName+"))\r\n"
                    "    into vTmp1, vTmp2\r\n"
                    "    from *.OP_ITEMS OI, *.OPERS O, *."+CurrOpt.AnName+" S\r\n"
                    "    where\r\n"
                    "        OI.CODE=pAccCode and\r\n"
                    "        OI.OP_DATE>= pDtBeg and OI.OP_DATE< pDtEnd and\r\n"
                    "        OI.OP_ID = O.OP_ID and\r\n"
                    "        OI.OP_SUBID = O.OP_SUBID and\r\n"
                    "        O.STATUS= 1 and\r\n"
                    "        O.POST= 0 and S.SUMID=OI.ITEM_SUM_ID and\r\n"
                    "         AARTICLE=:ARTICLE " +
                    (CurrOpt.WithoutVer?AnsiString(";\r\n"):AnsiString("and AVER=:VER;\r\n"))+
                    "    vDT:=nvl(vTmp1,0);\r\n"
                    "    vCT:=nvl(vTmp2,0);\r\n"
                    "end if;\r\n"
                    "pDT:=vDT;\r\n"
                    "pCT:=vCT;\r\n"
                    "pER:=pBR+pDT-pCT;\r\n"
                ":BR:=pBR; :DT:=pDT; :CT:=pCT; :ER:=pER;\r\nend;\r\n  end;");
            }

            if (CurrOpt.FilterArticle!="")
            {
                AQ.DeclVar("ARTICLE", CurrOpt.FilterArticle);
                if (!CurrOpt.WithoutVer)
                  AQ.DeclVar("VER", CurrOpt.FilterVer);
            }
            else
            {
                AQ.DeclVar("ARTICLE", " ");
                if (!CurrOpt.WithoutVer)
                  AQ.DeclVar("VER", 0);
            }
        }
        AQ.DeclVar("CT", 0.0);
        AQ.DeclVar("DT", 0.0);
        AQ.DeclVar("BR", 0.0);
        AQ.DeclVar("ER", 0.0);
        AQ.DeclVar("CODE", CurrOpt.AccCode);
        AQ.DeclVar("BEG_DATE", CurrOpt.DateBeg);
        AQ.DeclVar("END_DATE", CurrOpt.DateEnd);
        AQ.Exec("Находим обороты и остатки по счету");
        BegR=AQ.GetVarSum("BR");
        EndR=AQ.GetVarSum("ER");
        DTurn=AQ.GetVarSum("DT");
        CTurn=AQ.GetVarSum("CT");

        if (fabs(EndR-BegR-DTurn+CTurn)>0.0001 )
        {
            ErrorMessage("Ост на начало+ Дебет - Кредит != Ост на конец");
        }

        LastRemOpt.FldName=CurrOpt.FldName;
        LastRemOpt.BegDate=CurrOpt.DateBeg;
        LastRemOpt.EndDate=CurrOpt.DateEnd;
        LastRemOpt.Article=CurrOpt.FilterArticle;
        LastRemOpt.Ver=CurrOpt.FilterVer;
    };
};
//---------------------------------------------------------------------------
AnsiString TFrmBrwsAccWindow::GetFirstFieldName()
{
    if (FirstFieldName=="")
    {
        AmsQuery AQ;
        AQ.SetSqlText("select REALNAME from *.DBSTR where DBID=:DBID and DELETED=0 order by ORD");
        AQ.DeclVar("DBID", CurrOpt.AnCode);
        AQ.Exec("Выбираем поле для фильтра");
        bool Finded = false;
        FirstFieldName="AVER";
        while (!Finded && AQ.NotEof())
        {
            AnsiString Res=AQ.GetFldStr(0).UpperCase();
            if (Res!="AARTICLE" &&
                Res!="AVER" &&
                Res!="SUMID")
            {
                Finded=true;
                FirstFieldName=Res;
            }
            else
                AQ.Next();
        }
    }
    return FirstFieldName;
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrwsAccWindow::ActPerExecute(TObject *Sender)
{
    TFrmPeriod* F ;
    Application->CreateForm(__classid(TFrmPeriod), &F);
    F->SetInitPeriod(CurrOpt.DateBeg,CurrOpt.DateEnd) ;

    int res=F->ShowModal();
    if (res==1)
    {
        CurrOpt.DateBeg=F->DSet->FieldByName("POINT_DATE")->AsDateTime ;
        CurrOpt.DateEnd=F->DSet1->FieldByName("END_DATE")->AsDateTime;
        CurrOpt.PerNotPlain=false;
        RefreshOpt();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrwsAccWindow::ActAnaliticShowExecute(TObject *Sender)
{
    ViewAnalitic(false, false, DSet->FieldByName("ITEM_SUM_ID")->AsInteger,
                    DSet->FieldByName("SETNAME")->AsString,
                    "Аналитика операции");
}
//---------------------------------------------------------------------------
void TFrmBrwsAccWindow::AnViewEnd(void)
{
    AnView=NULL;
}
//---------------------------------------------------------------------------
void TFrmBrwsAccWindow::FindOk()
{
    if (!CurrOpt.ByDate)
        TFrmOperListRoot::FindOk();
};
//---------------------------------------------------------------------------
void TFrmBrwsAccWindow::BuildFullSql(AnsiString& Sql, DS_Q& Q, AnsiString& Simple)
{
    Q.DelVars();
    if (FilterSetted)
    {
        CurrOpt.AddWhere="";
        FrmFilter->FormWhereCond(CurrOpt.AddWhere, Q, true);
    }
    else
        CurrOpt.AddWhere=" ";
    CurrOpt.OrderBy=SortTxt;
    AW.BuildSqlQuery(CurrOpt, Q, Sql, Simple, true);
};
//---------------------------------------------------------------------------
void TFrmBrwsAccWindow::ChangedFilter()
{
    DSet->Active=false;
    DS_Q Q(DSet);
    Q.DelVars();
    AnsiString Str=" ";
    AnsiString Simple=" ";

    BuildFullSql(Str, Q, Simple);

    Q.SetSqlText(Str);
    ExecODS( DSet );
    CountSelection();
    MakeTitle();
}
//---------------------------------------------------------------------------
AnsiString TFrmBrwsAccWindow::FormFindSql(DS_Q& Q)
{
    Q.DeclVar("BEG_DATE", CurrOpt.DateBeg);
    Q.DeclVar("END_DATE", CurrOpt.DateEnd);
    Q.DeclVar("CODE", CurrOpt.AccCode);
    return "select O.OP_ID, O.OP_SUBID from *.OPERS O, *.OP_ITEMS OI, *.SUMS S "
        "where O.OP_DATE>=:BEG_DATE and O.OP_DATE<:END_DATE and CODE=:CODE "
        "and O.STATUS=1 and O.OP_ID=OI.OP_ID and O.OP_SUBID=OI.OP_SUBID and "
        "OI.ITEM_SUM_ID=S.SUM_ID ";
}
//---------------------------------------------------------------------------
void TFrmBrwsAccWindow::ChangedOrder()
{
    DSet->Active=false;
    DS_Q Q(DSet);
    Q.DelVars();
    AnsiString Sql="";
    AnsiString Simple="";
    BuildFullSql(Sql, Q, Simple);
    ExecODS(DSet );
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrwsAccWindow::RxSplitter1PosChanged(TObject *Sender)
{
    if (Grid2->Top>StatusBar->Top)
    {
        Grid2->Top=StatusBar->Top;
        StatusBar->Top=Grid2->Top+Grid2->Height;
    }
}
//---------------------------------------------------------------------------
void TFrmBrwsAccWindow::RefreshAfterOper(bool otlozh)
{
    bool h=false;
    if (HaveOpId)
    {
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

    }
    else
    {
        if (otlozh){
          DSet->Next();
          if (DSet->Eof) {DSet->Prior(); h=true;}
        }
        TDateTime CurrDate = DSet->FieldByName("OP_DATE")->AsDateTime;
        DSet->DisableControls();
        DSet->Refresh();
        TSearchRecordOptions SetSR;
        SetSR<<srFromBeginning;
        DSet->SearchRecord("OP_DATE", CurrDate, SetSR);
        if (otlozh) {if (h) DSet->Last(); else DSet->Prior();}

    }
    GetRem();
    MakeTitle();

    DumbSetFooter();
    DSet->EnableControls();
    //if (G.ur.set_to_end)
    //   DSet->Last();
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrwsAccWindow::FormDestroy(TObject *Sender)
{
    AccsToWin.Del(this);
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrwsAccWindow::ActAnalChooseExecute(TObject *Sender)
{
//
    TFrmAskAnalSumType2* F;
    Application->CreateForm(__classid(TFrmAskAnalSumType2), &F);
    int SumType=F->AskAnalSumType();
    if (SumType==-1)
        return;
    AmsQuery AQ;
    int SumCode=0;
    AnsiString SetName="";
    AnsiString WCaption;
    AnsiString QQ="";
    switch (SumType)
    {
        case 0: // аналитика остатка на начало
            QQ=CurrOpt.PerNotPlain?"R_ASM(:CODE, :RDATE)":"RI(:CODE, :RDATE)";
            AQ.SetPLText(":SUMID:=*."+QQ+";"
                        "select max(SETNAME) into :SETNAME from *.ACCS "
                        "where CODE=:CODE;");
            AQ.DeclVar("SUMID",0);
            AQ.DeclVar("CODE",CurrOpt.AccCode);
            AQ.DeclVar("RDATE",CurrOpt.DateBeg);
            WCaption="Аналитика остатка на начало";
        break;
        case 4: // аналитика остатка на Конец
            QQ=CurrOpt.PerNotPlain?"R_ASM(:CODE, :RDATE)":"RI(:CODE, :RDATE)";
            AQ.SetPLText(":SUMID:=*."+QQ+";"
                        "select max(SETNAME) into :SETNAME from *.ACCS "
                        "where CODE=:CODE;");
            AQ.DeclVar("SUMID",0);
            AQ.DeclVar("CODE",CurrOpt.AccCode);
            AQ.DeclVar("RDATE",CurrOpt.DateEnd);
            WCaption="Аналитика остатка на конец";
        break;
        case 1: // аналитика оборота по дебету
            AQ.SetPLText(":SUMID:=*.DTI(:CODE, :BEG_DATE, :END_DATE);"
                        "select max(SETNAME) into :SETNAME from *.ACCS "
                        "where CODE=:CODE;");
            AQ.DeclVar("SUMID",0);
            AQ.DeclVar("CODE",CurrOpt.AccCode);
            AQ.DeclVar("BEG_DATE",CurrOpt.DateBeg);
            AQ.DeclVar("END_DATE",CurrOpt.DateEnd);
            WCaption="Аналитика оборота по дебету";
        break;
        case 2: // аналитика оборота по CREDITU
            AQ.SetPLText(":SUMID:=*.CTI(:CODE, :BEG_DATE, :END_DATE);"
                        "select max(SETNAME) into :SETNAME from *.ACCS "
                        "where CODE=:CODE;");
            AQ.DeclVar("SUMID",0);
            AQ.DeclVar("CODE",CurrOpt.AccCode);
            AQ.DeclVar("BEG_DATE",CurrOpt.DateBeg);
            AQ.DeclVar("END_DATE",CurrOpt.DateEnd);
            WCaption="Аналитика оборота по кредиту";
        break;
        case 3: // Аналитика текущей операции
            ActAnaliticShow->Execute();
        break;
    }
    if (SumType!=3)
    {
        AQ.DeclVar("SETNAME", "");
        AQ.Exec("Выбираем аналитическую сумму");
        SumCode=AQ.GetVarInt("SUMID");
        AnsiString SetName=AQ.GetVarStr("SETNAME");
        ViewAnalitic(true, false, SumCode, SetName,WCaption+" "+Caption, true);
    }

}
//---------------------------------------------------------------------------
void TFrmBrwsAccWindow::ViewAnalitic(bool AskSet_, bool NeedDel, int SumId_, const AnsiString& SetName_,
                            const AnsiString& Caption_, bool NeedDistr)
{
    if (!AnView)
    {
        AnsiString St=SetName_;
        if (AskSet_ && AskMessage("Показывать в наборе установок \""+SetName_+"\"?", false)!=ID_YES)
        {
            TFrmGetSetName* F;
            Application->CreateForm(__classid(TFrmGetSetName), &F);
            St=F->GetSetName(CurrOpt.AnCode, "Выберите другой набор установок");
        }
        Application->CreateForm(__classid(TFrmAnView), &AnView);

        AnView->Begin_(NeedDel, this, CurrOpt.AnCode, CurrOpt.AnName,
            St,SumId_, CurrOpt.BalFld, CurrOpt.AccCode, NeedDistr);
        AnView->Caption=Caption_;
    }
    else
    {
        AnView->SumIdChange(NeedDel, SumId_,SetName_);
        AnView->Caption=Caption_;
        AnView->BringToFront();
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrwsAccWindow::ActShowDefferedExecute(TObject *Sender)
{
//
    TFrmPostOpers* Frm;
    Application->CreateForm(__classid(TFrmPostOpers), &Frm);
    Frm->Begin(CurrOpt.AccCode, "Отложенные операции по "+BegTitle);
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrwsAccWindow::ActRunFormExecute(TObject *Sender)
{
//
    if (!FrmChooseForm)
    {
        Application->CreateForm(__classid(TFrmChooseForm), &FrmChooseForm);
    }
    FrmChooseForm->BeginForm(CurrOpt);
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrwsAccWindow::ActPrintExecute(TObject *Sender)
{
    //
    //----------------------------------------------------
    // ВПИХИВАЕМ ДАННЫЕ ОБ СЧЕТЕ в интерпретатор
    //----------------------------------------------------
    AmsQuery AQ;
    AQ.SetSqlTextReady("select * from "+BaseUserName+".REMS_TURNS where CODE=:CODE");
    AQ.DeclVar("CODE", CurrOpt.AccCode);
    AQ.Exec("Данные о счете");
    TGenLocalVars LV;
    OpVars* OV = LV.VARS();
    AnsiString WhereAdd="";
    AnsiString WhereTxt="";
    if (FilterSetted)
    {
        FrmFilter->FormStrWhereCond (WhereAdd, true);
        WhereTxt="Выборка:"+FrmFilter->GetDescr();
    }

    OV->AddVarInt("PERIOD_NOT_PLAIN",CurrOpt.PerNotPlain);
    OV->AddVarDate("PERIOD_DATE_BEG", CurrOpt.DateBeg);
    OV->AddVarDate("PERIOD_DATE_END", CurrOpt.DateEnd);

    OV->AddVarStr("WHERE_ADD", WhereAdd);
    OV->AddVarStr("WHERE_ADD_TXT", WhereTxt);
    if (AQ.NotEof())
    {
        OV->AddVars(AQ);
        OV->AddVarInt("COR",CurrOpt.DetailedCorresp);
        OV->AddVarInt("DEBCRED", CurrOpt.DK==SBrwsAccWindowOptions::dkBoth?0:CurrOpt.DK);
        OV->AddVarInt("BYDATES",CurrOpt.ByDate);
        OV->AddVarInt("FILT",CurrOpt.FilterArticle!="");
        OV->AddVarInt("VER",CurrOpt.FilterVer);
        OV->AddVarStr("FILT_FLD",CurrOpt.FldName!=""?CurrOpt.FldName:CurrOpt.BalFld);
        OV->AddVarStr("ARTICLE",CurrOpt.FilterArticle);
        OV= LV.ACCV();
        OV->AddVars(AQ);

    }
    BeginExecReport("ФОРМ_СЧЕТ", &LV);
    LV.ClearVars();

}
//---------------------------------------------------------------------------

void __fastcall TFrmBrwsAccWindow::ActAnalEndRemExecute(TObject *Sender)
{
//
    AmsQuery AQ;
    int SumCode=0;
    AnsiString SetName="";
    AnsiString WCaption;
    // аналитика остатка на Конец
    //
    AnsiString QQ=CurrOpt.PerNotPlain?"R_ASM(:CODE, :RDATE)":"RI(:CODE, :RDATE)";
    AQ.SetPLText(":SUMID:=*."+QQ+";"
            "select max(SETNAME) into :SETNAME from *.ACCS where CODE=:CODE;");
    AQ.DeclVar("SUMID",0);
    AQ.DeclVar("CODE",CurrOpt.AccCode);
    AQ.DeclVar("RDATE",CurrOpt.DateEnd);
    AQ.DeclVar("SETNAME", "");
    WCaption="Аналитика остатка на конец";
    AQ.Exec("Выбираем аналитическую сумму");
    SumCode=AQ.GetVarInt("SUMID");
    SetName=AQ.GetVarStr("SETNAME");
    ViewAnalitic(false, CurrOpt.PerNotPlain,SumCode, SetName,WCaption+" "+Caption, true);
}
//---------------------------------------------------------------------------
TDateTime GetServerTime()
{
    AmsQuery AQ;
    TDateTime Res=0;
    AQ.SetSqlTextReady("select SYSDATE from DUAL");
    AQ.Exec("Получаем время с сервера");
    if (AQ.NotEof())
    {
        Res= AQ.GetFldDate(0);
    }
    return Res;
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrwsAccWindow::ActRecalcExecute(TObject *Sender)
{
    //
    if (AskMessage("Пересчитать остатки и обороты по счету?", false)==ID_YES)
    {
        TDateTime BegCalc= GetServerTime();
        AnsiString Mess ="Пересчет начался "+FormatDateTime("dd.mm.yy hh:nn:ss",BegCalc);
        ShortMessage(Mess);
        do {
        AmsQuery AQ;
        try
        {
            AQ.SetSqlTextReady("ALTER ROLLBACK SEGMENT MYROLL ONLINE");
            AQ.Exec("");
        } catch(...) {}

        AQ.SetPLText("*.bookkern.RecalcAccount(:code,:beg_date,:end_date);"
            "select count(*) into :OP_COUNT from *.UNAPP_OPERS;"
            "select count(*) into :ITEM_COUNT from *.UNAPP_ITEMS;commit;"
            ":SESS_ID:=USERENV('SESSIONID');");

        AQ.DeclVar("CODE", CurrOpt.AccCode);
        AQ.DeclVar("BEG_DATE", CurrOpt.DateBeg);
        AQ.DeclVar("END_DATE", CurrOpt.DateEnd);
        AQ.DeclVarInt("SESS_ID");
        AQ.DeclVarInt("OP_COUNT");
        AQ.DeclVarInt("ITEM_COUNT");
        AQ.Exec("Начинаем пересчет");
        int OstOp = AQ.GetVarInt("OP_COUNT");
        int OstIt = AQ.GetVarInt("ITEM_COUNT");
        int SessId = AQ.GetVarInt("SESS_ID");

        ShortMessage(Mess+". Осталось пересчитать "+IntToStr(OstOp)+" операций, "+
            IntToStr(OstIt)+" элементов.");
        AQ.Clear();
        AQ.SetPLText(":Rest:=*.bookkern.ApplyUndoAll(:SESS_ID,100);commit;"
            "select count(*) into :OP_COUNT from *.UNAPP_OPERS;"
            "select count(*) into :ITEM_COUNT from *.UNAPP_ITEMS;commit;");
        AQ.DeclVarInt("SESS_ID", SessId);
        AQ.DeclVarInt("OP_COUNT");
        AQ.DeclVarInt("ITEM_COUNT");
        AQ.DeclVarInt("Rest");
        int Rest = 100;
        while (Rest)
        {
            AQ.Exec("Пересчитываем очередную порцию");
            Rest=AQ.GetVarInt("REST");
            OstOp = AQ.GetVarInt("OP_COUNT");
            OstIt = AQ.GetVarInt("ITEM_COUNT");
            ShortMessage(Mess+". Осталось пересчитать "+IntToStr(OstOp)+" операций, "+
                IntToStr(OstIt)+" элементов.");
        }
        }while(0);

        TDateTime EndCalc= GetServerTime();
        Mess +=" окончен "+FormatDateTime("dd.mm.yy hh:nn:ss",EndCalc);
        ShortMessage(Mess);
        RefreshOpt();
    }
}
//---------------------------------------------------------------------------
void TFrmBrwsAccWindow::CountSelection(void)
{
    HaveWhereTotal=false;
    if (FilterSetted)
    {
        AmsQuery AQ;

        if (FilterSetted)
        {
            CurrOpt.AddWhere="";
            DS_Q D(AQ());
            FrmFilter->FormWhereCond(CurrOpt.AddWhere, D, true);
        }

        if (
            (CurrOpt.FldName==""|| CurrOpt.FldName==CurrOpt.BalFld) &&
            (CurrOpt.FilterArticle==" " || CurrOpt.FilterArticle==""))
        {
            AQ.SetSqlText("select "
                " sum (*.GetAccCorTotValue('D', OI.SIDE, O.STORNO, S.VALUE)) DSUM, "
                " sum (*.GetAccCorTotValue('K', OI.SIDE, O.STORNO, S.VALUE)) CSUM"
                " from "
                    "    *.OP_ITEMS OI, "
                    "    *.OPERS O, "
                    "    *.SUMS S "
                    "where "
                    "    OI.OP_DATE>= :BEG_DATE and OI.OP_DATE<:END_DATE and "
                    "    OI.CODE =:CODE and "
                    "    O.OP_ID = OI.OP_ID and "
                    "    O.OP_SUBID = OI.OP_SUBID and "
                    "    O.POST = 0 and "
                    "    O.STATUS = 1 "+ CurrOpt.AddWhere+" and "
                    "    OI.ITEM_SUM_ID=S.SUM_ID ");
            AQ.DeclVar("BEG_DATE", CurrOpt.DateBeg);
            AQ.DeclVar("END_DATE", CurrOpt.DateEnd);
            AQ.DeclVar("CODE"    , CurrOpt.AccCode);
            HaveWhereTotal=true;
        }
        else
        {
            if (CurrOpt.FldName=="") CurrOpt.FldName=CurrOpt.BalFld;
            if (CurrOpt.FilterArticle=="")
            {
                CurrOpt.FilterArticle=" ";
                CurrOpt.FilterVer=0;
            }

            bool FldAddable = false;
            do {
                // Узнаем поле СУММИРУЕМОЕ?
                AmsQuery AQ;
                AQ.SetSqlText(
                    "select DEF_OPT from *.DBSTR where DBID=:DBID and REALNAME=:REALNAME");
                AQ.DeclVar("DBID", CurrOpt.AnCode);
                AQ.DeclVar("REALNAME", CurrOpt.FldName);
                AQ.Exec("Находим тип поля");
                if (AQ.NotEof())
                {
                    if (AQ.GetFldInt(0) & Addable) FldAddable=true;
                }
            } while (0);

            if (FldAddable)
            {
                HaveWhereTotal=true;
                AnsiString Sql="select "
                    " sum (*.GetAccCorTotValue('D', OI.SIDE, O.STORNO, "+
                        CurrOpt.FldName+")) DSUM, "
                    " sum (*.GetAccCorTotValue('K', OI.SIDE, O.STORNO, "
                        +CurrOpt.FldName+")) CSUM"
                    " from "
                        "    *.OP_ITEMS OI, "
                        "    *.OPERS O, "
                        "    *."+CurrOpt.AnName+" AN "
                        "where "
                        "    OI.OP_DATE>= :BEG_DATE and OI.OP_DATE<:END_DATE and "
                        "    OI.CODE =:CODE and "
                        "    O.OP_ID = OI.OP_ID and "
                        "    O.OP_SUBID = OI.OP_SUBID and "
                        "    O.POST = 0 and "
                        "    O.STATUS = 1 "+ CurrOpt.AddWhere+" and "
                        "    OI.ITEM_SUM_ID=AN.SUMID and AN.AARTICLE=:ARTICLE ";
                if ( !CurrOpt.WithoutVer)
                {
                    Sql+="and AN.AVER=:VER";
                    AQ.DeclVar("VER", CurrOpt.FilterVer);
                }
                AQ.SetSqlText(Sql);
                AQ.DeclVar("ARTICLE", CurrOpt.FilterArticle);
                AQ.DeclVar("BEG_DATE", CurrOpt.DateBeg);
                AQ.DeclVar("END_DATE", CurrOpt.DateEnd);
                AQ.DeclVar("CODE"    , CurrOpt.AccCode);
            }
        };
        if (HaveWhereTotal)
        {
            AQ.Exec("Находим обороты по выборке");
            if (AQ.NotEof())
            {
                WTDTurn=AQ.GetFldSum(0);
                WTCTurn=AQ.GetFldSum(1);
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrwsAccWindow::DSetAfterScroll(TDataSet *DataSet)
{
    TMAScroll->Enabled=false;
    if (HaveOpId)
    {
        TMAScroll->Interval=1000;
        TMAScroll->Enabled=true;
    }
    Label1->Caption="Информация о текущей проводке";

    TDelay->Interval=1000;
    TDelay->Enabled= true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrwsAccWindow::TDelayTimer(TObject *Sender)
{
    TDelay->Enabled=false;

    TOracleDataSet* ODS = DSet;
    TAmsDBGrid * DBG= Grid1;

    if (Grid1->SelectedRows->Count>1)
    {
        ODS->DisableControls();
        AnsiString CurrPos = ODS->Bookmark;


        vector<SOutTotal>Tot;
        for (int i =1; i < Grid1->Columns->Count; i++)
        {
            TColumn* C = Grid1->Columns->Items[i];
            if (C->Field->DataType==ftFloat	)
            {
                SOutTotal S;
                S.FldNo=C->Field->FieldNo;
                S.Sum=0;
                S.ColNo=i;
                Tot.push_back(S);
            }
            else
            {
                if (i==1)
                    AFEnd->SetVal("Выделенные ряды", eefSelected, 1);
                else
                    AFEnd->SetVal("", eefSelected, i);
            }
        }


        for (int j = 0 ; j< DBG->SelectedRows->Count; j++)
        {
            ODS->GotoBookmark((void *)DBG->SelectedRows->Items[j].c_str());
            int stornojkl;
            for (int j=0;j<ODS->Fields->Count;j++)if (UpperCase(ODS->Fields->Fields[j]->FieldName)=="STORNO") {stornojkl=j;break;}

            for (unsigned int i = 0; i< Tot.size() ; i++)
            {
                SOutTotal& OT = Tot[i];
                TField* Fld=ODS->Fields->FieldByNumber(OT.FldNo);

                if ((!CurrOpt.ByDate)) {
                  if ((ODS->Fields->Fields[stornojkl]->AsInteger!=0)&&(!CurrOpt.DetailedCorresp)){
                    if (!Fld->IsNull)
                      OT.Sum-=(double)(Fld->Value);
                  } else {
                    if (!Fld->IsNull)
                      OT.Sum+=(double)(Fld->Value);
                  }
                } else {
                  if (!Fld->IsNull)
                      OT.Sum+=(double)(Fld->Value);
                }      
            }
        }

        for (unsigned int i = 0 ; i< Tot.size(); i++)
        {
            SOutTotal& OT = Tot[i];
            if (OT.Sum)
                AFEnd->SetVal(OT.Sum, eefSelected, OT.ColNo+1);
            else
                AFEnd->SetVal("", eefSelected, OT.ColNo+1);
        }

        ODS->GotoBookmark((void*) CurrPos.c_str());
        ODS->EnableControls();

        AFEnd->Height= (HaveWhereTotal? 4 : 3) * (AFEnd->DefaultRowHeight+1);
        AFEnd->RowHeights[eefSelected]=AFEnd->DefaultRowHeight;
    }
    else
        HideSelTotal();
    TDelay->Enabled=false;
}
//---------------------------------------------------------------------------
void TFrmBrwsAccWindow::HideSelTotal(void)
{
    if (AFEnd->RowHeights[eefSelected]!=-1)
    {
        AFEnd->RowHeights[eefSelected]=-1;
        AFEnd->Height= (HaveWhereTotal? 3 : 2) * (AFEnd->DefaultRowHeight+1);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrwsAccWindow::ActNotPlainPeriodExecute(
      TObject *Sender)
{
//
    TFrmChooseNotPlainPeriod* Frm;
    Application->CreateForm(__classid(TFrmChooseNotPlainPeriod), &Frm);
    if (Frm->ChooseNotPlainPeriod(CurrOpt.DateBeg, CurrOpt.DateEnd, CurrOpt.SyntOnly))
    {
        CurrOpt.PerNotPlain=true;
        RefreshOpt();
    }
    Frm->Release();
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrwsAccWindow::ActRecalcBeginExecute(TObject *Sender)
{
    if (AskMessage("Пересчитать остатки по счету на начало ведения бухгалтерии?", false)==ID_YES)
    {
        AmsQuery AQ;
        AQ.SetPLText("*.bookkern.RecalcBegin(:code);" );

        AQ.DeclVar("CODE", CurrOpt.AccCode);
        AQ.Exec("Начинаем пересчет");

        RefreshOpt();
    }
}
//---------------------------------------------------------------------------


void TFrmBrwsAccWindow::DumbSetFooter() //CREATED AS ANSWER ON QUESTION #3 MADE 13.03.02
{
  int jkl, stornojkl;

  try {
  if (DSet->Active) {
    for (int j=0;j<DSet->Fields->Count;j++)if (UpperCase(DSet->Fields->Fields[j]->FieldName)=="DSUM") {jkl=j;break;}
    for (int j=0;j<DSet->Fields->Count;j++)if (UpperCase(DSet->Fields->Fields[j]->FieldName)=="STORNO") {stornojkl=j;break;}

    double ajkl[100];
    AnsiString HODS=DSet->RowId();
    DSet->First();
    for (int j=0;j<100;j++) ajkl[j]=0;
    DSet->DisableControls();
    while (!DSet->Eof) {
      for (int j=jkl;j<DSet->Fields->Count;j++)
        if ((!CurrOpt.ByDate)) {
          if ((DSet->Fields->Fields[stornojkl]->AsInteger!=0)&&(!CurrOpt.DetailedCorresp))
            ajkl[j-jkl]-=(DSet->Fields->Fields[j]->AsFloat);
          else
            ajkl[j-jkl]+=(DSet->Fields->Fields[j]->AsFloat);
        } else ajkl[j-jkl]+=(DSet->Fields->Fields[j]->AsFloat);
      DSet->Next();
    }

    DSet->First();
    if (DtCol>0) {
      for (int j=DtCol;j<AFEnd->ColCount;j++){
        AFEnd->SetVal(ajkl[j-DtCol], 3-FilterSetted, j);
      }
    } else {

      for (int j=0;j<DSet->Fields->Count;j++)if (UpperCase(DSet->Fields->Fields[j]->FieldName)=="CSUM") {jkl=j;break;}
      for (int j=0;j<DSet->Fields->Count;j++)if (UpperCase(DSet->Fields->Fields[j]->FieldName)=="STORNO") {stornojkl=j;break;}
      while (!DSet->Eof) {
        for (int j=jkl;j<DSet->Fields->Count;j++)
          if ((!CurrOpt.ByDate)) {
            if ((DSet->Fields->Fields[stornojkl]->AsInteger!=0)&&(!CurrOpt.DetailedCorresp))
              ajkl[j-jkl]-=(DSet->Fields->Fields[j]->AsFloat);
            else
              ajkl[j-jkl]+=(DSet->Fields->Fields[j]->AsFloat);
          } else
          ajkl[j-jkl]+=(DSet->Fields->Fields[j]->AsFloat);
        DSet->Next();
      }

      for (int j=CtCol;j<AFEnd->ColCount;j++){
        AFEnd->SetVal(ajkl[j-CtCol], 3-FilterSetted, j);
      }
    }
    DSet->First();
    while (!DSet->Eof) {
      if (HODS==DSet->RowId()) break;
      DSet->Next();}
  }
  } __finally {
      DSet->EnableControls();
  }
}

void __fastcall TFrmBrwsAccWindow::ActSelExecute(TObject *Sender)
{
   TFrmOperListRoot::ActSelExecute(Sender);
   DumbSetFooter();
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrwsAccWindow::ActAllExecute(TObject *Sender)
{
   TFrmOperListRoot::ActAllExecute(Sender);
   DumbSetFooter();
}
//---------------------------------------------------------------------------








void __fastcall TFrmBrwsAccWindow::ActFindExecute(TObject *Sender)
{
  TFrmOperListRoot::ActFindExecute(Sender);        
}
//---------------------------------------------------------------------------


