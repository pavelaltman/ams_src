//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_BROWS_DB.h"
#include "ams_win_c.h"
#include "DAT_MOD.h"
#include "FRM_EDIT_DB_POS.h"
#include "FRM_FIND_DB_POS2.h"
#include "FRM_SORT_ORDER.h"
#include "FRM_QUICK_SEARCH.h"
#include "FRM_SET_FLD.h"
#include "FRM_HIST_VIEW.h"
#include "prog.h"
#include "orcpp.h"
#include "LOCAL_VARS.h"
#include <Clipbrd.hpp>
#include "FRM_ASK_FLDS_NAMES.h"
#include "amsclnt.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AMS_TREE_VIEW"
#pragma link "AmsDbCatView"
#pragma link "Oracle"
#pragma link "OracleData"
#pragma link "RXDBCtrl"
#pragma link "AmsDBGrid"
#pragma resource "*.dfm"
//TFrmBrowsDb *FrmBrowsDb;

#define PROH_NEWART  1
#define PROH_SAMEDOC 2
#define APPLY_OPERS  4
#define SUB_STRING   8
#define PROH_UPDATE  16
#define DONOT_ASK    32
// Осуществлять перекодировку в WIN1251
#define AMSTO1251    64

extern void ClrAction(TAction* );
//---------------------------------------------------------------------------
__fastcall TFrmBrowsDb::TFrmBrowsDb(TComponent* Owner)
    : TForm(Owner)
{
    EditedNow=false;
    EdNewAnal=false;
    NewAnSumid=-1;
    NeedRecalcArt=false;
    ColsReady = false;
    CatsReady = false;
    FindInCurrCat = false;
    FrmEditDbPos = NULL;
    FrmSortOrder = NULL;
    FrmQuickSearch= NULL;
    Vars=NULL;
    FrosenWidth = 0;
    NormalWidth = 0;
    WithoutCats = false;
    WithoutVer = false;
    OrderByClause = "D.ARTICLE, D.VER";
    HaveCatsSort = true;
    DefaultSort = true;
    FocusWhat = -1;
    DBG->RowsHeight = 18;
    FSM = new FieldSetM(&FldsDescr, ODS);
    if (GC::ReadOnly)
    {
        ClrAction(ActNew);
        ClrAction(ActDel);
        ClrAction(ActCopy);
        ClrAction(ActMove);
        ClrAction(ActMoveAll);
        ClrAction(ActCalc);
        ClrAction(ActCopyFromExcel);
    }
}
//---------------------------------------------------------------------------
void TFrmBrowsDb::Begin(int DBID,
                        const AnsiString& DBName,
                        const AnsiString& DBDescr,
                        const AnsiString& SetName)
{
    db_prog_comp = false;
    an_prog_comp = false;

    CurrDBId = DBID;
    OQ->SQL->Text= "select nvl(HIST, 0), Name  from "+BaseUserName+".OBJS "
    "where OID=:OID";
    OQ->DeleteVariables();
    OQ->DeclareVariable("OID", otInteger);
    OQ->SetVariable("OID", DBID);
    try
    {
        ExecSQL(OQ, "Находим название базы данных");
    }
    catch(...)
    {
        THROW_AMS_EXC ("Нет базы данных с таким кодом "+IntToStr(DBID));
    }
    CurrDBName = OQ->FieldAsString(1);
    ActHist->Enabled=(OQ->FieldAsInteger(0)==1);

    CurrSetName = SetName;
    CurrCatId = -1;
    if (SetName == "" || SetName == " ")
        Caption = "Просмотр базы данных ["+CurrDBName+"] "+DBDescr;
    else
        Caption = "Просмотр базы данных ["+CurrDBName+"/"+SetName+"] "+DBDescr;
    RefreshCats();
    BuildCols();
    TTreeNode* TN  = CV->Items->GetFirstNode();
    CV->Selected=TN;

    if (CV->Items->Count==1)
    {
        CV->Hide();
        if (!CV->Visible && Visible)
            DBG->SetFocus();
    }
    ShowMode();

}
//---------------------------------------------------------------------------
void TFrmBrowsDb::BuildCols(void)
{
    DBG->Columns->Clear();
    int FixedCols = 0;
/*    if (GC::GUI)
    {
        TColumn * Col = DBG->Columns->Add();
        Col->Title->Caption = "BLOB";
        Col->Width = 16;
        Col->FieldName = "BLOB";
        FixedCols++;
    }*/

    ColsReady = false;


    int FrozenTotWidth =0;
    //int TotWidth = 0;
    int W= DBG->Canvas->TextWidth("0");
    FldsDescr.SetDbDescr(CurrDBName, CurrDBId, CurrSetName);
    FldsDescr.ReadFldsDescr();
    TField* Fld;

      BuildSqlQuery(ODS);
    ExecODS(ODS);

    TDBGridColumns* Cols;

    for (int i = 0 ; i< FldsDescr.Count(); i++)
    {
        SFldOpt& XX = FldsDescr[i];
        Fld = ODS->FieldByName(XX.SqlAlias);
        XX.FldNo = ODS->Fields->IndexOf(Fld);
        if (!((XX.DefOpt & Hidden) || (XX.DispOpt & Hidden)))
        {
            int AddW = XX.DiplWidth*W;
            if ((XX.DefOpt & Frozen) || (XX.DispOpt & Frozen))
            {
                Cols=DBG->Columns;
                FrozenTotWidth += AddW+6;

                TColumn * Col = Cols->Add();
                Col->Field= Fld;
                Col->Title->Caption = XX.Alias;
                Col->Width = AddW+5;
                FixedCols++;
            }
        }
    }


    for (int i = 0 ; i< FldsDescr.Count(); i++)
    {
        SFldOpt& XX = FldsDescr[i];
        Fld = ODS->FieldByName(XX.SqlAlias);
        XX.FldNo = ODS->Fields->IndexOf(Fld);
        if (!((XX.DefOpt & Hidden) || (XX.DispOpt & Hidden)))
        {
            int AddW = XX.DiplWidth*W;
            if (! ((XX.DefOpt & Frozen) || (XX.DispOpt & Frozen)))
            {
                Cols=DBG->Columns;
                FrozenTotWidth += AddW+6;

                TColumn * Col = Cols->Add();
                Col->Field= Fld;
                Col->Title->Caption = XX.Alias;
                Col->Width = AddW+5;
            }
        }
    }
    if (CV->Items->Count<2)
    {
        CV->Width=0;
    }
    DBG->FixedCols=FixedCols;
    FrozenTotWidth+=CV->Width+44;
    if (FrozenTotWidth<AmsWinC->Width-16)
    {
        if (AmsWinC->Width < Left+FrozenTotWidth)
        {
            Left =AmsWinC->Width-FrozenTotWidth-16;
        }
        Width = FrozenTotWidth;
    }
    if (AmsWinC->ClientHeight_()< Top+Height)
    {
        Top= AmsWinC->ClientTop_()+8;
        if (AmsWinC->ClientHeight_()< Top+Height)
        {
            Height=AmsWinC->ClientHeight_()-Top-8;
        }
    }
    FormatSums();
    ColsReady = true;
    PanGridResize(this);

}
//---------------------------------------------------------------------------
void TFrmBrowsDb::RefreshCats(void)
{
    // Обновляются категории
    CatsReady =false;
    CV->Begin(BaseUserName, CurrDBId,"");
    if (CurrCatId != -1)
        CV->MakeActive(CurrCatId);
    else
    {
        if (CV->Items->Count)
        {
            TTreeNode* TN = CV->Items->GetFirstNode();
            TN->Expanded = true;
        }
    }
    CatsReady = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    AmsWinC->FindActiveMDIChild();
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

    delete FSM;
    Action = caFree;
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsDb::CVChange(TObject *Sender, TTreeNode *Node)
{
    if (CatsReady && ColsReady && ODS->Active)
    {
        if (CV->GetCurrCode()!= CurrCatId)
        {
            CurrCatId = CV->GetCurrCode();
            CV->SelectCurr();
            AnsiString S =Node->Text;
            AnsiString S1="";
            TTreeNode * TN= Node;

            while (TN  = TN->Parent)
            {
                if (TN->Text!="/")
                {
                    S1= TN->Text+" / "+S;
                    S=S1;
                }
            }
            if (S=="/")
                Lab1->Caption = S;
            else
                Lab1->Caption = " / "+S;

            if (!WithoutCats)
            {
                ODS->DisableControls();
                ODS->SetVariable("CATID", CurrCatId);
                ODS->Active= false;
                ExecODS(ODS);
                FormatSums();
                ODS->EnableControls();
            }
            CatChanged ();
        }
    }
}
//---------------------------------------------------------------------------
void TFrmBrowsDb::CatChanged (){};
//---------------------------------------------------------------------------
void TFrmBrowsDb::BeginSelection (){};
//---------------------------------------------------------------------------
void TFrmBrowsDb::EndSelection(){};
//---------------------------------------------------------------------------
void TFrmBrowsDb::SetFilterOn(){};
//---------------------------------------------------------------------------
void TFrmBrowsDb::SetFilterOff(){};
//---------------------------------------------------------------------------
void TFrmBrowsDb::ActEdDbPosExec(int BegFlags)
{
    if (!FrmEditDbPos)
    {
        Application->CreateForm (__classid (TFrmEditDbPos), &FrmEditDbPos);
        FrmEditDbPos->Top= 50;
        FrmEditDbPos->Left= 50;
        FrmEditDbPos->FrmBrowsDb=this;
        FrmEditDbPos->Position = poMainFormCenter;
        //
    }
    int Flags = BegFlags | eoDubbing;
    if (GC::ReadOnly) Flags|= eoReadOnly;
    CurrFlags = Flags;
    //---------------------------------------------------------------------
    RunProg(0, 1, 0);
    //---------------------------------------------------------------------
    if (!WithoutVer)
    {   //Если с версиями
        if (!(Flags & eoNewPos)&& (Flags&eoDbFlds))
        { // Редактируем позицию (а не дублируем)
            //ODS->Edit();
            OQ->SQL->Text = "select "+BaseUserName+
                ".IsBusyPos( :DBID, :ARTICLE, :VER) from DUAL";
            OQ->DeleteVariables();
            OQ->DeclareVariable("DBID", otInteger);
            OQ->DeclareVariable("ARTICLE", otString);
            OQ->DeclareVariable("VER", otInteger);

            OQ->SetVariable("DBID", CurrDBId);
            OQ->SetVariable("ARTICLE", ODS->FieldByName("ARTICLE")->AsString);
            OQ->SetVariable("VER", ODS->FieldByName("VER")->AsInteger);
            ExecSQL(OQ, "Занятость позиции");
            if (!OQ->Eof)
            {
                if (OQ->FieldAsInteger(0))
                    Flags |= eoBusyPos;
            }
        }
    }
    else
    { // Без версий
        Flags|=eoHideVersion;
    };

    AnsiString WhatEdit = Flags&eoDbFlds?" БД ":" аналитики ";
    AnsiString AddInfo;
    if (GC::ViewDebug)
    {
        AddInfo="["+CurrDBName;
        if (Flags&eoAnFlds)
            AddInfo+="/"+CurrAnName+"]";
        else
            AddInfo+="]";
    }
    if (Flags & eoNewPos)
    {
        FrmEditDbPos->Caption = "Дублирование позиции "+WhatEdit+AddInfo;

        // Копируем аналитики и запоминаем аналитические суммы
        CopySums.clear();
        for (int i = 0 ; i<FldsDescr.Count();i++)
        {
            SFldOpt& FO = FldsDescr[i];
            if (FO.Type==aftInt && FO.DBId>0 && FO.FldAddInfo==aiDb )
            {
                if (!FSM->At(i)->isNull())
                {
                    AmsQuery AQ;
                    AQ.SetPLText("*.CreateSum(:SUMID, 0);"
                        "*.CopyAnalSum(:AnalId,:SUMID,:SRC);commit;");
                    AQ.DeclVar("SUMID", 0);
                    AQ.DeclVar("ANALID", FO.DBId);

                    int OldSumId = FSM->At(i)->Number();
                    AQ.DeclVar("SRC", OldSumId);
                    AQ.Exec("Находим копию аналитической суммы");
                    int NewSumId = AQ.GetVarInt("SUMID");
                    CopySums.push_back(NewSumId);
                    FSM->At(i)->SetNumber(NewSumId);
                }
            }
        }
    }
    else
    {
        FrmEditDbPos->Caption = "Редактирование позиции "+WhatEdit+AddInfo;
    }
    FrmEditDbPos->WithoutCats=WithoutCats;
    FrmEditDbPos->EditDbPos(ODS, Flags, FSM, &FldsDescr, HaveFirstDigit, FirstDigit, false, Vars);
    HaveFirstDigit = false;
    DBG->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::ActEdDbPosExecute(TObject *Sender)
{
    int Flags = eoDubbing | eoDbFlds;
    if (!Sender)
    {
        Flags|=eoNewPos;
    }
    CurrFlags = Flags;
    ActEdDbPosExec (Flags);
    EditedNow=true;
    DatMod->OSession->Commit();

}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsDb::CVKeyPress(TObject *Sender, char &Key)
{
    if ( Key ==13 || Key ==32)
    {
        if (CV->Selected->HasChildren)
            CV->Selected->Expanded= !CV->Selected->Expanded;
        else
            DBG->SetFocus();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsDb::CVChanging(TObject *Sender, TTreeNode *Node,
      bool &AllowChange)
{
    if (CV->Selected)
    {
        int Res = GetKeyState(VK_SHIFT);
        if (!(Res & 0x80))
            CV->DeSelectCurr();
    }
}
//---------------------------------------------------------------------------


void __fastcall TFrmBrowsDb::PanGridResize(TObject *Sender)
{
/*    DBGFrosen ->Left = 0;
    DBGFrosen ->Top  = 0;
    DBGFrosen ->Width  = FrosenWidth+32;

    DBG ->Left = DBGFrosen ->Width - 20;
    DBG ->Top  = 0;
    DBG ->Height = PanGrid->Height;
    DBG ->Width  = PanGrid->Width-DBG ->Left;
    if (DBG ->Width-12 < NormalWidth)
        DBGFrosen ->Height = PanGrid->Height- 16;
    else
        DBGFrosen ->Height = PanGrid->Height;
*/
}
//---------------------------------------------------------------------------
// Устанавливает на позицию АРТИКУл Версия ЕСлИ известна категория то указать
void TFrmBrowsDb::SetPos (const AnsiString& Art, int Ver, int CatId)
{
    if (Art=="") return ;
    if (CatId == -1 && !WithoutCats)
    {
        OQ->SQL->Text=" select CATID from "+BaseUserName+"."+CurrDBName+
            " where ARTICLE = :ARTICLE and VER = :VER";
        OQ->DeleteVariables();
        OQ->DeclareVariable("ARTICLE", otString);
        OQ->DeclareVariable("VER", otInteger);
        OQ->SetVariable("ARTICLE", Art);
        OQ->SetVariable("VER", Ver);
        ExecSQL(OQ,"Находим категорию");
        if (!OQ->Eof)
        {
            CatId = OQ->FieldAsInteger(0);
        }
        else
        {
            Message("Не нашел позицию в базе данных "+CurrDBName+"\r\n"
                "["+Art+"]["+Ver+"]");
        }
    }

    if (CatId != -1 || WithoutCats)
    {
        CV->MakeActive(CatId);
        CVChange(this, CV->Selected);
        if (WithoutVer)
        {
            TSearchRecordOptions SetSR;
            SetSR<<srFromBeginning;
            ODS->SearchRecord("ARTICLE", Art, SetSR);
            if (Visible) DBG->SetFocus();
        }
        else
        {
            Variant Vals[2] ;
            Vals[0]=Art;
            Vals[1]=Ver;
            Variant VecVals  = VarArrayOf(Vals, 1);
            TSearchRecordOptions SetSR;
            SetSR<<srFromBeginning;
            ODS->SearchRecord("ARTICLE;VER", VecVals, SetSR);
            if (Visible) DBG->SetFocus();
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsDb::ActFindExecute(TObject *Sender)
{
    //
    FindCat = false;
    if (!FrmFindDbPos)
    {
        Application ->CreateForm(__classid(TFrmFindDbPos), &FrmFindDbPos);
        FrmFindDbPos->Prepare(&FldsDescr, CurrDBName);
        FrmFindDbPos->AddStr = "F";
    }
    if (FrmFindDbPos->FindDbPos())
    {
        DS_Q D(OQFind);
        OQFind->Clear();
        AnsiString WhereAdd ="select D.CATID, D.ARTICLE, D.VER from "+BaseUserName+"."+
            CurrDBName+" D, "+BaseUserName+".DBCATTREE CT where D.CATID = CT.OWNCAT ";

        if (FindInCurrCat)
        {
            WhereAdd+="and D.CATID=:CATID ";
            D.DeclVar("CATID", CurrCatId);
        }
        FindInCurrCat = false;


        FrmFindDbPos->FormWhereCond (WhereAdd, D, true);
        if (FrmFilter && FilterOn)
        {
            FrmFilter->FormWhereCond (WhereAdd, D, true);
        }
        WhereAdd+=AddCond;
        WhereAdd+=" order by CT.TORD, "+GetOrderBy();
        OQFind->SQL->Text = WhereAdd;
        ExecSQL(OQFind, "Поиск позиции в БД");
        ActFindAgainExecute(this);
    };
}
//---------------------------------------------------------------------------
void TFrmBrowsDb::TakeAnSum()
{
};
//---------------------------------------------------------------------------
void TFrmBrowsDb::VirtKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    return;
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::DBGKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{


    VirtKeyDown(Sender, Key,Shift);
    if (Key > 47  && Key < 58)
    {
        if (!Shift.Contains(ssAlt) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssShift))
        {
            BeginEdit( Key - 48);
            Key = 0;
        }
    }
    if (Key == 118 && Shift.Contains(ssAlt))  // ------ Alt+F7
    {
        ActFindInCurrCat->Execute();
    }
    if (Key >95 && Key < 106)
    {
        if (!Shift.Contains(ssAlt) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssShift))
        {
            BeginEdit( Key - 96);
            Key = 0;
        }
    }
    if (Key == 72) //-------- Alt+Р
    {
        if (Shift.Contains(ssAlt))
            ActVisibilityMode->Execute();
    }
    if (Key == 66) //-------- Alt+И
    {
        if (Shift.Contains(ssAlt))
            ActHist->Execute();
    }
    if (Key == 68)  // -------- Alt+В
    {
        if (Shift.Contains(ssAlt))
            TakeAnSum();
    }
    if (Key==71)   // Альт +П
    {
        if (Shift.Contains(ssAlt))
            EditOperVars();
    }
    if (Key==65)   // Альт+Ф
    {
        if (Shift.Contains(ssAlt))
            SetFilter();
    }
    if (Key ==13)
    {
        if (Shift.Contains(ssCtrl))
            ActEdit->Execute();
        else
            ActEditAn->Execute();
    }
    if (Key == 36)
    {
        ODS->First();
        Key = 0;
    }
    if (Key == 35)
    {
        ODS->Last();
        Key=0;
    }
    if (Key==45)
    {
        ActNew->Execute();
    }
    if (Key==46)
    {
        ActDel->Execute();
    }
    if (Key ==106)                        // * Без категорий
        ActBezKatExecute(this);

    if (Key == 78 && Shift.Contains(ssAlt))// Альт+Т
    {
        ActSortIerarchExecute(this);
    }

    if (Key==84 && Shift.Contains(ssAlt)) // Альт+Е
    {
        ActMove->Execute();
    };

    if (Key==76 && Shift.Contains(ssAlt)) // Альт+Д
    {
        ActCopy->Execute();
    };

    SHORT Res = GetKeyState(VK_RCONTROL);
    if (Res & 0x8000)
    {
        if (!QuickArtSearch)
        {
            if (!FrmQuickSearch)
            {
                Application->CreateForm(__classid(TFrmQuickSearch), &FrmQuickSearch);
                FrmQuickSearch->Parent = this;
            }
            FrmQuickSearch->Top= CV->Top+CV->Height-
                    FrmQuickSearch->Height-1;
            FrmQuickSearch->Left =DBG->Left;
            FrmQuickSearch->SetSeaching(this);
            FrmQuickSearch->SetFocus();
            FrmQuickSearch->SetFocusedControl(FrmQuickSearch->EdInp);
        };
    }

    if (DBG->SelectedRows->Count>1)
    {
        BeginSelection();
    }
    else
    {
        EndSelection();
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::ActDelExecute(TObject *Sender)
{
    if (FocusWhat==0)
    {
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
                        ODS->Delete();
                    }
                    catch (Exception& E)
                    {
                        ErrorMessage(E.Message);
                    }
                }
                DBG->SelectedRows->Clear();
            }
        }
        else
        {
            if (AskMessage("Удалить текущую позицию?")==ID_YES)
            {
                try
                {
                    ODS->Delete();
                }
                catch (Exception& E)
                {
                        ErrorMessage(E.Message);
                }
            };
        };
    }
    if (FocusWhat ==1)
    {
        ActDelCatsExecute(this);
    }
    DatMod->OSession->Commit();
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsDb::ActInsExecute(TObject *Sender)
{
    if (FocusWhat==0)
    {
        if (!FrmEditDbPos)
        {
            Application->CreateForm (__classid (TFrmEditDbPos), &FrmEditDbPos);
            FrmEditDbPos->Top= 50;
            FrmEditDbPos->Left= 50;
            FrmEditDbPos->WithoutCats=WithoutCats;
            FrmEditDbPos->FrmBrowsDb=this;
        }
        //ODS->Insert();
        int Flags = eoNewPos | eoDbFlds;
        CurrFlags = Flags;
        //---------------------------------------------------------
        RunProg(0,0,1);
        //---------------------------------------------------------
        EditedNow=true;
        FrmEditDbPos->EditDbPos(ODS, Flags, FSM, &FldsDescr,false, 0, false);
    }
    if (FocusWhat==1 || Sender==N2 )
    {
        ActInsCatExecute(this);
    }
    DatMod->OSession->Commit();
}
//---------------------------------------------------------------------------
AnsiString TFrmBrowsDb::GetOrderBy(void)
{
    if (!FrmSortOrder || DefaultSort)
    {
        if (WithoutCats && HaveCatsSort)
        {
            return "TORD, ARTICLE, VER";
        }
        else
        {
            return "ARTICLE, VER";
        }
    }
    else
    {
        return OrderByClause;
    }
}
//---------------------------------------------------------------------------
void TFrmBrowsDb::BuildSqlQuery(TOracleDataSet* ODS_)
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

    if ( !WithoutVer)
    {
        AnsiString SelBeg = "select D.ROWID";
        for ( int i = 0; i< FldsDescr.Count(); i++)
        {
            SelBeg +=", "+FldsDescr[i].FullName+" "+FldsDescr[i].SqlAlias;
        }
        SelBeg +=" from ";
        if (WithoutCats && HaveCatsSort)
        {
            ODS_->SQL->Text =
            SelBeg+BaseUserName+"."+CurrDBName+" D, "+
                                        BaseUserName+".DBCATTREE CT "
            " where CT.DBID = :CATID and CT.OWNCAT = D.CATID AND "
            " D.ARTICLE != ' ' " + GetFilterCond(DS_Q(ODS_))+
            " order by " + GetOrderBy();
            ODS_->DeclareVariable("CATID", otInteger);
            ODS_->SetVariable("CATID", CurrDBId);
        }
        else
        {
            if (WithoutCats && !HaveCatsSort)
            {   // Без категорий и сортировка не иерархическая
                ODS_->SQL->Text =
                SelBeg+BaseUserName+"."+CurrDBName+" D "
                " where D.ARTICLE != ' ' "+ GetFilterCond(DS_Q(ODS_))+
                " order by " + GetOrderBy();
            }
            else
            {
                ODS_->SQL->Text =
                SelBeg+BaseUserName+"."+CurrDBName+" D "
                " where D.ARTICLE != ' ' and "
                " D.CATID = :CATID "+ GetFilterCond(DS_Q(ODS_))+" order by " + GetOrderBy();
                ODS_->DeclareVariable("CATID", otInteger);
                ODS_->SetVariable("CATID", CurrCatId);
            }
        }
    }
    else
    {   // Без версий
        // Формируем строку с макс поз
        AnsiString SelMax = "select ";
        for ( int i = 0; i< FldsDescr.Count(); i++)
        {
            if (i)
                SelMax+=", ";
            if ( FldsDescr[i].RealName != "ARTICLE")
                SelMax +="max("+FldsDescr[i].FullName+") as "+FldsDescr[i].SqlAlias;
            else
                SelMax+="ARTICLE";
        }

        if (WithoutCats && HaveCatsSort)
        {
            ODS_->SQL->Text =
            SelMax+", max(CT.TORD) as TORD from "+BaseUserName+"."+CurrDBName+" D, "+
                                        BaseUserName+".DBCATTREE CT "
            " where CT.DBID = :CATID and CT.OWNCAT = D.CATID AND "
            " D.ARTICLE != ' ' " + GetFilterCond(DS_Q(ODS_))+" group by ARTICLE "+
            "order by " + GetOrderBy();
            ODS_->DeclareVariable("CATID", otInteger);
            ODS_->SetVariable("CATID", CurrDBId);
        }
        else
        {
            if (WithoutCats && !HaveCatsSort)
            {   // Без категорий и сортировка не иерархическая
                ODS_->SQL->Text =
                SelMax+" from "+BaseUserName+"."+CurrDBName+" D "
                " where D.ARTICLE != ' ' "+ GetFilterCond(DS_Q(ODS_))+" group by ARTICLE "+
                " order by " + GetOrderBy();
            }
            else
            {
                ODS_->SQL->Text =
                SelMax+" from "+BaseUserName+"."+CurrDBName+" D "
                " where D.ARTICLE != ' ' and "
                " D.CATID = :CATID "+ GetFilterCond(DS_Q(ODS_))+" group by ARTICLE "+
                " order by " + GetOrderBy();
                ODS_->DeclareVariable("CATID", otInteger);
                ODS_->SetVariable("CATID", CurrCatId);
            }
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsDb::ODSAfterScroll(TDataSet *DataSet)
{
    if (WithoutCats)
    {
        int CC=CV->GetCurrCode();
        CurrCatId = ODS->FieldByName("CATID")->AsInteger;
        CV->MakeActive(CurrCatId);
        if (CC!= CurrCatId)
        {
            TTreeNode* N = CV->Selected;

            if (N)
            {
                AnsiString Res;
                while (N)
                {
                    Res=N->Text+"/"+Res;
                    N=N->Parent;
                }
                Lab1->Caption=Res;
            }
        }
    }

    if (DBG->SelectedRows->Count>0)
    {
        BeginSelection();
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::ActBezKatExecute(TObject *Sender)
{
    WithoutCats = !WithoutCats;
    RefreshView();
    ShowMode();
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::ActFindAgainExecute(TObject *Sender)
{
    //
    if (FindCat)
    {
        FindNextCatExec();
    }
    else
    {
        if (FrmFindDbPos)
        {
            if (!OQFind->Eof)
            {
                int CatId=OQFind->FieldAsInteger(0);
                AnsiString Art= OQFind->FieldAsString(1);
                int Ver = OQFind->FieldAsInteger(2);
                OQFind->Next();
                SetPos (Art, Ver, CatId);
            }
            else
            {
                Message("Поиск завершен");
            };
        };
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::DBGKeyPress(TObject *Sender, char &Key)
{
    if (QuickArtSearch && FrmQuickSearch)
    {
        FrmQuickSearch->SetFocus();
    }

    if (DBG->SelectedRows->Count>1)
    {
        BeginSelection();
    }
    else
    {
        EndSelection();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsDb::ActSortOrderExecute(TObject *Sender)
{
//
    if (!FrmSortOrder)
    {
        Application->CreateForm(__classid(TFrmSortOrder), &FrmSortOrder);
        FrmSortOrder->AddStrs("Категории", "CT.TORD");
        for (int i =0; i< FldsDescr.Count(); i++)
        {
            if ( !(FldsDescr[i].DefOpt & Hidden) && !(FldsDescr[i].DispOpt & Hidden))
            {
                FrmSortOrder->AddStrs(FldsDescr[i].Alias, FldsDescr[i].RealName );
            }
        }
    }
    AnsiString SO = "";
    FrmSortOrder->GetCommaText(SO);
    if (SO != "")
    {
        if (
            (HaveCatsSort != FrmSortOrder->HaveInRes()) ||
            (SO != OrderByClause) ||
            DefaultSort
           )
        {
            OrderByClause = SO;
            HaveCatsSort = FrmSortOrder->HaveInRes();
            DefaultSort = false;
            RefreshView();

        }
    }
    ShowMode();
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::ActSortIerarchExecute(TObject *Sender)
{
    if (WithoutCats && DefaultSort)
    {
        HaveCatsSort = !HaveCatsSort;
        RefreshView();
        ShowMode();
    }
}
//---------------------------------------------------------------------------
void TFrmBrowsDb::RefreshView(void)
{
    ODS->DisableControls();
    int CatId = CurrCatId;
    int TL = DBG->LeftCol;
    int Col = DBG->Col;
    AnsiString Art = ODS->FieldByName("ARTICLE")->AsString;
    int Ver = ODS->FieldByName("VER")->AsInteger;
    BuildSqlQuery(ODS);
    ExecODS(ODS);
    DBG->LeftCol = TL;
    DBG->Col = Col;
    FormatSums();
    //ODS->Active = true;
    SetPos(Art, Ver, CatId);
    ODS->EnableControls();
    ViewUpdated();
}
//---------------------------------------------------------------------------
void TFrmBrowsDb::ViewUpdated()
{
};
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::ActSortBackExecute(TObject *Sender)
{
    if (!DefaultSort)
    {
        DefaultSort = true;
        RefreshView();
        ShowMode();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsDb::ActTestExecute(TObject *Sender)
{
    AnsiString Res = "";
    FrmSortOrder->GetCommaText(Res);
    Lab1->Caption=Res;
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsDb::FormActivate(TObject *Sender)
{
    if ((EditedNow && FrmEditDbPos)||(EdNewAnal))
    {
       if (!FrmEditDbPos->bJKL) {
        FrmEditDbPos->BringToFront();
        FrmEditDbPos->SetFocus();
       }
    }
    else
    {
        TopMostForm = this;
        if (!CV->Visible)
            DBG->SetFocus();
    }
}
//---------------------------------------------------------------------------
void TFrmBrowsDb::ShowMode()
{
    if (!DefaultSort)
        SBar->Panels->Items[0]->Text = "Сорт пользователя";
    else
        SBar->Panels->Items[0]->Text = "";

    if (WithoutCats)
    {
        SBar->Panels->Items[1]->Text = "Без категорий";
        CV->Enabled=false;
    }
    else
    {
        SBar->Panels->Items[1]->Text = "";
        CV->Enabled=true;
    }

    if (WithoutCats && HaveCatsSort && DefaultSort)
        SBar->Panels->Items[2]->Text = "Иерархическая";
    else
        SBar->Panels->Items[2]->Text = "";

    if (WithoutVer)
        SBar->Panels->Items[3]->Text = "Без версий";
    else
        SBar->Panels->Items[3]->Text = "";
    if (FilterOn)
    {
        if (FrmFilter)
        {
            SBar->Panels->Items[4]->Text="Выборка:"+FrmFilter->GetDescr();
        }
    }
    else
        SBar->Panels->Items[4]->Text = "";

}
//---------------------------------------------------------------------------
void TFrmBrowsDb::QuickSearchNewKey (AnsiString SubStr)
{
    AnsiString ValS = SubStr+"*" ;
    Variant Val= ValS;
    TSearchRecordOptions SetSR;
    SetSR<<srIgnoreCase<<srWildcards;
    ODS->SearchRecord("ARTICLE", Val, SetSR);
}
//---------------------------------------------------------------------------
void TFrmBrowsDb::EndQuickSearch()
{
    QuickArtSearch= false;
    ActiveControl=DBG;
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::ActSVerExecute(TObject *Sender)
{
    WithoutVer= !WithoutVer;
    ActNew->Enabled= !WithoutVer; 
    ActCopy->Enabled= !WithoutVer; 
    if (WithoutVer)
    {
        ActWVer->Hint="Режим видимости с версиями";
        ActWVer->Caption="Показать версии";
    }
    else
    {
        ActWVer->Hint="Режим видимости БД без версий";
        ActWVer->Caption="Спрятать версии";
    }
    RefreshView();
    ShowMode();
}
//---------------------------------------------------------------------------
int TFrmBrowsDb::GetCurrDbCat()
{
    return CurrCatId;
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::ActInsCatExecute(TObject *Sender)
{
    if (CV->Selected)
    {
        AnsiString CatName ="";
        if (AskString("Название новой категории", 200, CatName))
        {
            TOracleQuery* OQ= DatMod->GetOQ();
            try
            {
                OQ->DeleteVariables();
                OQ->DeclareVariable("DBID",otInteger);
                OQ->DeclareVariable("CATID",otInteger);
                OQ->DeclareVariable("DESCR",otString);
                OQ->DeclareVariable("KEY",otInteger);
                OQ->DeclareVariable("RES",otInteger);
                OQ->ClearVariables();
                OQ->SetVariable("DBID", CurrDBId);
                OQ->SQL->Text = "begin "+BaseUserName+".InsertDBCat(:DBID, "
                ":CATID, :DESCR, :KEY, :RES); "+BaseUserName+
                ".MakeDBTree(:DBID);commit; end;";
                TTreeNode * TN;
                int Code;
                TTreeNode * BeforeNode=NULL;
                TTreeNode * RootNode = NULL;
                if (Sender)
                {
                    TN = CV->Selected->Parent;
                    Code = CV->GetCurrCode(); 
                    if (!TN)
                    {
                        TN=CV->Items->Item[0];
                        RootNode= TN;     
                    }
                    else
                    {
                        OQ->SetVariable("KEY", Code);
                        BeforeNode= CV->Selected;
                    }
                }
                else
                {
                    TN=CV->Selected;
                    RootNode = TN;
                    Code = CV->GetCurrCode();
                }
                OQ->SetVariable("CATID", CV->GetCode(TN)); 
                OQ->SetVariable("DESCR", CatName); 
             
                ExecSQL(OQ, "Вставка новой категории");
                Code = OQ->GetVariable("KEY");
                CV->NewLeaf(RootNode, BeforeNode, CatName, Code);             
            }
            catch (...)
            {
                DatMod->ReleaseOQ(OQ);
                throw;
            }
            DatMod->ReleaseOQ(OQ);
        }
    }    
}
//---------------------------------------------------------------------------
AnsiString TFrmBrowsDb::GetFilterCond(DS_Q D)
{
    //Возвращает SQL where выражение
    AnsiString S="";
    if (FrmFilter && FilterOn)
    {
        //DS_Q D(ODS);
        FrmFilter->FormWhereCond(S, D, true);
    }
    S+=AddCond;
    return S;
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsDb::CVKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
//
    switch (Key)
    {
        case 38: // Стрелка вверх
        case 40: // Стрелка вниз
            if (Shift.Contains(ssShift))
            {
                TTreeNode* T1 = CV->Selected;
                TTreeNode* T2 = CV->MoveNode(T1, Key==38);
                if (T2)
                {
                    TOracleQuery* OQ = DatMod->GetOQ();
                    try
                    {
                        OQ->SQL->Text = "begin "+BaseUserName+
                        ".ChangeDbCats( :DBID, :CAT1, :CAT2); commit; end;";
                        OQ->DeleteVariables();
                        OQ->DeclareVariable("DBID",otInteger);
                        OQ->DeclareVariable("CAT1",otInteger);
                        OQ->DeclareVariable("CAT2",otInteger);
                        OQ->SetVariable("DBID", CurrDBId);
                        OQ->SetVariable("CAT1", CV->GetCode(T1));
                        OQ->SetVariable("CAT2", CV->GetCode(T2));
                        ExecSQL(OQ,"Меняем местами категории");

                    }
                    catch (...)
                    {
                        DatMod->ReleaseOQ(OQ);
                        throw;
                    }
                    DatMod->ReleaseOQ(OQ);
                }
                Key = 0;
            }
        break;

        case 45:  // Ins
            if (Shift.Contains(ssCtrl))
                ActInsCatExecute(NULL);    // Новая подкатегория
            else
                ActInsCatExecute(this);    // Новая категория
        break;

        case 46:  // Del
            ActDelCatsExecute(this);
        break;

        case 84: // Е Перемещение категорий
            if (Shift.Contains(ssShift))
            {
                ActMoveCatsExecute(this);
            }
        break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::ActDelCatsExecute(TObject *Sender)
{
//
    if (AskMessage("Вы действительно хотите удалить выбранные категории?")==ID_YES)
    {
        TAmsSelObjsList* OL = CV->GetSelectedCats();
        TOracleQuery* OQ = DatMod->GetOQ();
        try
        {
            OQ->SQL->Text= "begin :RES := "+BaseUserName+
                ".DelDbCat(:DBID, :CATID);"+BaseUserName+
                ".MakeDbTree(:DBID); commit; end;";
            OQ->DeleteVariables();
            OQ->DeclareVariable("DBID", otInteger);
            OQ->DeclareVariable("CATID", otInteger);
            OQ->DeclareVariable("RES", otInteger);
            OQ->SetVariable("DBID", CurrDBId);
            OQ->SetVariable("RES", 0);
            AnsiString BadDels="";
            int Co = OL->Count();
            for (int i = 0; i< Co; i++)
            {
                OQ->SetVariable("CATID", (*OL)[i]);
                ExecSQL(OQ, "Удаление категории");
                if ( OQ->GetVariable("RES")==1)
                {
                    CV->DelNodeByCode((*OL)[i]);
                }
                else
                {
                    BadDels += " "+IntToStr((*OL)[i]);
                }

            }
            if (BadDels!= "")
            {
                ErrorMessage("Категории"+BadDels+" не были удалены");
            }
        }
        catch (...)
        {
            DatMod->ReleaseOQ(OQ);
            throw;
        }
        DatMod->ReleaseOQ(OQ);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::ActMoveToCatExecute(TObject *Sender)
{
    if (FocusWhat==0)
    {
        //  Перемещение выбранных позиций БД в к-л категорию
        int CatId = DatMod->GetDbCat(CurrDBId, CurrDBName);
        if (CatId)
        {
            TOracleQuery* OQ= DatMod->GetOQ();
            try
            {
                OQ->DeleteVariables();
                OQ->DeclareVariable("CATID", otInteger);
                OQ->DeclareVariable("ARTICLE", otString);
                OQ->SetVariable("CATID", CatId);
                if (WithoutVer)
                {
                    OQ->SQL->Text = "begin update "+BaseUserName+"."+CurrDBName+
                        " set CATID= :CATID where ARTICLE = :ARTICLE; commit; end;";
                }
                else
                {
                    OQ->SQL->Text = "begin update "+BaseUserName+"."+CurrDBName+
                        " set CATID= :CATID where ARTICLE = :ARTICLE and VER= :VER; commit; end;";
                    OQ->DeclareVariable("VER", otInteger);
                }
                if (DBG->SelectedRows->Count>0)
                {
                    for (int i = 0; i< DBG->SelectedRows->Count; i++)
                    {
                        ODS->GotoBookmark((void *)DBG->SelectedRows->Items[i].c_str());
                        OQ->SetVariable("ARTICLE", ODS->FieldByName("ARTICLE")->AsString);
                        if (!WithoutVer)
                            OQ->SetVariable("VER", ODS->FieldByName("VER")->AsInteger);
                        ExecSQL(OQ,"Перемещение в другую категорию");
                    }
                    DBG->SelectedRows->Clear();
                }
                else
                {
                    OQ->SetVariable("ARTICLE", ODS->FieldByName("ARTICLE")->AsString);
                    if (!WithoutVer)
                        OQ->SetVariable("VER", ODS->FieldByName("VER")->AsInteger);
                    ExecSQL(OQ,"Перемещение в другую категорию");
                }
            }
            catch(...)
            {
                DatMod->ReleaseOQ(OQ);
                throw;
            }
            DatMod->ReleaseOQ(OQ);
            ODS->Next();
            RefreshView();
        };
    }
    if (FocusWhat ==1)
    {
        ActMoveCatsExecute(this);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::ActMoveCatsExecute(TObject *Sender)
{
    // Перемещение категорий в др категорию
    int CatId = DatMod->GetDbCat(CurrDBId, CurrDBName);
    if (CatId)
    {
        TOracleQuery* OQ = DatMod->GetOQ();
        try
        {
            OQ->SQL->Text = "begin "+BaseUserName+
            ".MoveDbCatTo(:DBID, :CATID, :CATIDTO);"
            +BaseUserName+".MakeDbTree(:DBID); commit; end;";
            OQ->DeleteVariables();
            OQ->DeclareVariable("DBID",otInteger);
            OQ->DeclareVariable("CATID",otInteger);
            OQ->DeclareVariable("CATIDTO",otInteger);
            OQ->SetVariable("DBID"      ,  CurrDBId);
            OQ->SetVariable("CATIDTO"   , CatId);
            TAmsSelObjsList* OL = CV->GetSelectedCats();
            TTreeNode* TN = CV->GetNodeByCode(CatId);

            int Co = OL->Count();
            for (int i = 0; i<Co; i++)
            {
                OQ->SetVariable("CATID", (*OL)[i]);
                ExecSQL(OQ,"Перемещаем категории");
                TTreeNode* CurrNode= CV->GetNodeByCode((*OL)[i]);
                CurrNode->MoveTo(TN, naAddChild);
            }
        }
        catch (...)
        {
            DatMod->ReleaseOQ(OQ);
            throw;
        }
        DatMod->ReleaseOQ(OQ);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::ActMoveAllExecute(TObject *Sender)
{
    if (FocusWhat == 0)
    {
        // Перемещаем все позиции БД текущей категории в др категорию
        int CatId = DatMod->GetDbCat(CurrDBId, CurrDBName);
        if (CatId)
        {
            TOracleQuery* OQ= DatMod->GetOQ();
            try
            {
                OQ->SQL->Text="begin update "+BaseUserName+"."+CurrDBName+
                " set CATID = :NEWCATID where CATID = :CATID; commit; end;";
                OQ->DeleteVariables();
                OQ->DeclareVariable("CATID", otInteger);
                OQ->DeclareVariable("NEWCATID", otInteger);
                OQ->SetVariable("CATID", CurrCatId);
                OQ->SetVariable("NEWCATID", CatId);
                ExecSQL(OQ, "Перемещаем все позициии в категорию");
                ODS->Refresh();
            }
            catch (...)
            {
                DatMod->ReleaseOQ(OQ);
                throw;
            }
            DatMod->ReleaseOQ(OQ);
        }
    }
    if (FocusWhat ==1)
    {
        ActMoveAllDbCatExecute(this);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsDb::ActMoveAllDbCatExecute(TObject *Sender)
{
    // Перемещаем все категории
    int CatId = DatMod->GetDbCat(CurrDBId, CurrDBName);
    if (CatId)
    {
        TOracleQuery* OQ= DatMod->GetOQ();
        try
        {
            OQ->SQL->Text="begin "+BaseUserName+
            ".MoveAllDbCatTo(:DBID, :CATID, :CATIDTO); commit; end;";
            OQ->DeleteVariables();
            OQ->DeclareVariable("DBID", otInteger);
            OQ->DeclareVariable("CATID", otInteger);
            OQ->DeclareVariable("CATIDTO", otInteger);
            OQ->SetVariable("DBID", CurrDBId);
            OQ->SetVariable("CATID", CurrCatId);
            OQ->SetVariable("CATIDTO", CatId);
            ExecSQL(OQ, "Перемещаем все подкатегории в категорию");
            TTreeNode* NewCat = CV->GetNodeByCode(CatId);
            TTreeNode* CurrNode = CV->Selected;
            while (CurrNode->Count)
            {
                TTreeNode* TN = CurrNode->Item[0];
                TN->MoveTo(NewCat, naAddChild);
            }
        }
        catch (...)
        {
            DatMod->ReleaseOQ(OQ);
            throw;
        }
        DatMod->ReleaseOQ(OQ);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::CVEnter(TObject *Sender)
{
    FocusWhat = 1;
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::DBGFrosenEnter(TObject *Sender)
{
    FocusWhat = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::ActRefreshCatExecute(TObject *Sender)
{
    RefreshCats();
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsDb::ActNewSubCatExecute(TObject *Sender)
{
    ActInsCatExecute(NULL);
}
//---------------------------------------------------------------------------


void __fastcall TFrmBrowsDb::ActFindCatExecute(TObject *Sender)
{
    FindCat = true;
    AnsiString FindLastCat = AskString("Находим категорию по подстроке.",200, FindCatName);
    if (FindLastCat!="")
    {
        FindCatName = FindLastCat.UpperCase();
        FindCatIndex =-1;
        FindNextCatExec();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsDb::DBGFrosenDblClick(TObject *Sender)
{
//
    ActEdit->Execute();
}
//---------------------------------------------------------------------------
void TFrmBrowsDb::FindNextCatExec()
{
    int i =FindCatIndex+1;
    AnsiString FindLastCat=FindCatName;
    TTreeNode* ResNode = NULL;

    while (!ResNode && i<CV->Items->Count)
    {
        TTreeNode* TN = CV->Items->Item[i];
        AnsiString S = TN->Text.UpperCase();
        if ( S.Pos(FindLastCat))
        {
            ResNode= TN;
        }
        else i++;
    };
    FindCatIndex= i;
    if (ResNode)
    {
        CV->Selected = ResNode;
    }
    else
        ShowMessage("Нет больше категорий с такой строкой");
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::ActCopyExecute(TObject *Sender)
{
    ActEdDbPosExecute(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::ActSelExecute(TObject *Sender)
{
//
    if (!FrmFilter)
    {
        Application->CreateForm(__classid(TFrmFindDbPos), &FrmFilter);
        FrmFilter->Prepare(&FldsDescr, CurrDBName);
        FrmFilter->Caption="Установить выборку для БД ["+CurrDBName+"]";
    }
    if (FrmFilter->SetFilter())
    {
        FilterOn = true;
        RefreshView();
        ShowMode();
    }
    if (FilterOn) SetFilterOn();
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::ActAllExecute(TObject *Sender)
{
    //
    if (FilterOn)
    {
        FilterOn = false;
        RefreshView();
        ShowMode();
    }
    if (!FilterOn) SetFilterOff();
}
//---------------------------------------------------------------------------
void TFrmBrowsDb::SetFld(int Flags)
{
    // Установка поля для выбранных позиций, категории, выборки
    TFrmSetFld* F ;
    Application->CreateForm(__classid(TFrmSetFld), &F);
    if (F->SetFld(&FldsDescr))
    {
        OQ->DeleteVariables();
        SFldOpt* FO = F->GetSelFld();
        F->FormSqlQuery(OQ);
        AnsiString AndStr ;
        AnsiString S;
        if (Flags & pfNullsOnly)
        {
            S+=FO->RealName +" is NULL" ;
            AndStr = " and ";
        }
        if (!FO->DefOpt & AlwaysFree)
        {
            // Добавляем условие занятости позиции
            S+=AndStr+BaseUserName+".IsBusyPos( :DBID, ARTICLE, VER) != 1";
            OQ->DeclareVariable("DBID", otInteger);
            OQ->SetVariable("DBID", CurrDBId);
            AndStr = " and ";
        }

        if (Flags & pfAllCat)    // Устанавливаем поле по выбранной категории
        {
            if (AskMessage("По поддеревьям тоже установить?") == ID_YES)
                S+= AndStr+"CATID in (select OWNCAT from "+BaseUserName+".DBCAT "
                    "start with OWNCAT = :CATID connect by prior OWNCAT = CATID) ";
            else
                S+=AndStr+"CATID= :CATID";
            OQ->DeclareVariable("CATID", otInteger);
            OQ->SetVariable("CATID", CurrCatId);

            OQ->SQL->Text =OQ->SQL->Text+S;
            ExecSQL(OQ, "Устанавливаем поле");
        }
        else if (Flags & pfFilteredPos) // Устанавливаем поле по выборке по условию
        {
            bool PutAnd = AndStr != "";
            if (!WithoutCats)
            {
                S += AndStr+"CATID = :CATID";
                OQ->DeclareVariable("CATID", otInteger);
                OQ->SetVariable("CATID", CurrCatId);
                PutAnd = true;
                AndStr = " and ";
            }
            if (FrmFilter)
            {
                DS_Q D (OQ);
                FrmFilter->FormWhereCond (S, D, PutAnd);
            }

            OQ->SQL->Text =OQ->SQL->Text+S;
            ExecSQL(OQ,"Устанавливаем поле");
        }
        else if (Flags & pfSelected)
        {                          // Устанавливаем поле по выбранным позициям БД
            OQ->SQL->Text = S;
            if ( WithoutVer )
                OQ->SQL->Text = AndStr+OQ->SQL->Text + " ARTICLE = :ARTICLE ";
            else
                OQ->SQL->Text=  AndStr+OQ->SQL->Text + " ARTICLE = :ARTICLE and VER = :VER ";

            if (DBG->SelectedRows->Count>0)
            {
                for (int i = 0; i< DBG->SelectedRows->Count; i++)
                {
                    ODS->GotoBookmark((void *)DBG->SelectedRows->Items[i].c_str());
                    OQ->SetVariable("ARTICLE",ODS->FieldByName("ARTICLE")->AsString);
                    if (!WithoutVer )
                        OQ->SetVariable("VER",ODS->FieldByName("VER")->AsInteger);
                    ExecSQL(OQ, "Устанавливаем поле");
                }
            }
            else
            {
                OQ->SetVariable("ARTICLE",ODS->FieldByName("ARTICLE")->AsString);
                if (!WithoutVer )
                    OQ->SetVariable("VER",ODS->FieldByName("VER")->AsInteger);
                ExecSQL(OQ, "Устанавливаем поле");
            };
        }
        DatMod->OSession->Commit();
        RefreshView();
    }
    F->Release();
};
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::MenSelSetFldClick(TObject *Sender)
{
    // Устанавливаем поле
    SetFld(pfFilteredPos);
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::MenSelDelAllClick(TObject *Sender)
{
    if (FilterOn && FrmFilter)
    {
        if (AskMessage("Удалить все позиции, удовлетворяющие критерию "+
                FrmFilter->GetDescr())==ID_YES)
        {
            OQ->DeleteVariables();

            OQ->SQL->Text=" delete from "+BaseUserName+"."+CurrDBName+" D where ";

            bool PutAnd = false;
            if ( !WithoutCats)
            {
                OQ->SQL->Text=OQ->SQL->Text+" CATID = :CATID ";
                OQ->DeclareVariable("CATID", otInteger);
                OQ->SetVariable("CATID", CurrCatId);
                PutAnd = true;
            }
            AnsiString S ="";
            DS_Q D (OQ);
            FrmFilter->FormWhereCond(S, D, PutAnd);
            OQ->SQL->Text=OQ->SQL->Text+S;
            ExecSQL(OQ, "Удаление выборки");
            RefreshView();
        };
    }
    else
    {
        ErrorMessage("Сначала установите критерий");
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsDb::MenCatSetFldClick(TObject *Sender)
{
    // Устанавливаем поле
    SetFld(pfAllCat);
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::MenSelSetFreeFldClick(TObject *Sender)
{
    SetFld(pfFilteredPos | pfNullsOnly );
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::ActHistExecute(TObject *Sender)
{
//
    TFrmHistView * HW=NULL;
    Application->CreateForm(__classid(TFrmHistView), &HW);
    HW->Begin(CurrDBId, CurrDBName, &FldsDescr, ODS);
    HW->ShowHist(ODS->FieldByName("ARTICLE")->AsString,
                ODS->FieldByName("VER")->AsInteger);
    HW->Release();
    DBG->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsDb::DBGMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    //LabTest->Caption = IntToStr(X)+":"+IntToStr(Y);
    int LeftSpace = 11;
    if (DBG->FixedCols>0 && Y<20)
    {
        if (!FCChange)
        {
            int i = 1;
            int CurrWidth = LeftSpace;
            bool Finded = false;
            while (i<=DBG->FixedCols && !Finded )
            {
                CurrWidth += DBG->ColWidths[i]+1;
                if ( (CurrWidth - X) < 2 && (CurrWidth - X)>-2)
                {
                    Finded = true;
                    DBG->Cursor=crHSplit;
                }
                else i++;
            }
            if (!Finded)
            {
                DBG->Cursor=crDefault;
            }
            if (Finded && Shift.Contains(ssLeft))
            {
                // включаем механизм изменения ширины колонки
                FixedColsChange = i;
                FCChange= true;
            }
        }
        else
        {
            if (!Shift.Contains(ssLeft))
            {
                FCChange=false;
                DBG->Cursor = crDefault;
            }
            else
            {
                int BeginX = LeftSpace;
                int i =1;
                while (i<FixedColsChange)
                {
                    BeginX+= DBG->ColWidths[i++];
                }
                int CurrWidth = DBG->ColWidths[FixedColsChange];
                //Lab2->Caption = IntToStr (BeginX);
                int Diff = CurrWidth+BeginX - X;
                if (Diff+5 < CurrWidth)
                {
                    DBG->ColWidths[FixedColsChange] = CurrWidth - Diff-1;
                }
            }
        }
    }
    else
    {
        FCChange=false;
        DBG->Cursor=crDefault;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::ActEditDbCatNameExecute(TObject *Sender)
{
//
    if (CV->Selected && CV->Selected!=CV->Items->GetFirstNode())
    {
        AnsiString NewVal = CV->Selected->Text;
        if (!AskString("Введите новое название категории", 200, NewVal))
        {
            return;
        }
        if (NewVal != "" && NewVal != CV->Selected->Text)
        {
            OQ->DeleteVariables();
            OQ->SQL->Text = "begin update "+BaseUserName+".DBCAT set DESCR = :DESCR "
            "where DBID = :DBID and OWNCAT = :OWNCAT; commit;end;";
            OQ->DeclareVariable ( "DBID"  , otInteger);
            OQ->DeclareVariable ( "OWNCAT", otInteger);
            OQ->DeclareVariable ( "DESCR", otString);
            OQ->SetVariable("DBID", CurrDBId);
            OQ->SetVariable("OWNCAT", CurrCatId);
            OQ->SetVariable("DESCR", NewVal);
            ExecSQL(OQ, "Изменяем название категории");
            CV->Selected->Text = NewVal;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::ActBuildCatTreeExecute(TObject *Sender)
{
//
    OQ->SQL->Text="begin "+BaseUserName+".MakeDBTree(:DBID); commit; end;";
    OQ->DeleteVariables();
    OQ->DeclareVariable("DBID", otInteger);
    OQ->SetVariable("DBID", CurrDBId);
    ExecSQL(OQ, "Перестраиваем дерево категорий");
}
//---------------------------------------------------------------------------
void TFrmBrowsDb::RunProg(bool Post, bool SetFlds, bool ClearFlds, bool ClearF4,
        bool UseFsm)
{
    if (CurrFlags & eoDbFlds)
    {
        if (!db_prog_comp)
        {
            ProgFile pf;
            char fname[256];
            sprintf(fname,"%s\\%s.prg",GC::DocPath.c_str(),CurrDBName.c_str());
            pf.Init(fname);
            try {
                db_prog.Compile(&pf);
                db_prog.Link(GC::DocPath.c_str());
            } catch(...) {}
        }
        db_prog_comp = true;

        IP ip;
        ip.Enter();
        extern int cur_arg;
        int pre_cur_arg=cur_arg ;
        cur_arg=-1 ;

        ::string set = ToString(CurrSetName);

        try
        {
            if (UseFsm)
                FSM->LoadData(ip.Base, SetFlds, ClearFlds, ClearF4);
            if (!(CurrFlags& eoReadOnly))
            {
                ip.Base.Structure().Var("SET").assign(VAL(set));
                ip.Base.Structure().Var("PRE").assign(VAL((int)!Post));
                // Вставляем opv и accv
                if (Vars)
                {
                    ip.SetVars(Vars);
                    ip.Base.AddStruct(Vars->VARS()->Stru());
                }
                ip.Base.Structure().Var("SET").assign(VAL(set));       //EO CH SET
                ip.Base.Structure().Var("PRE").assign(VAL((int)!Post));//EO CH SET
                //if(aux_view && aux_rec) ip.Base.MakeOraRec(*aux_view,*aux_rec,1);
                db_prog.Run(ip);
            }
        }
        catch(OraError ERR)
        {
            ERR.message(0,0,"After prog error");
        }
        ip.Leave();
        cur_arg=pre_cur_arg ;
    }

    if (CurrFlags & eoAnFlds)
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

        IP ip;
        ip.Enter();
        extern int cur_arg;
        int pre_cur_arg=cur_arg ;
        cur_arg=-1 ;

        ::string set = ToString(CurrSetName);

        try
        {
            if (UseFsm) FSM->LoadData(ip.Base, SetFlds, ClearFlds, ClearF4);
            if (!(CurrFlags& eoReadOnly))
            {
                ip.Base.Structure().Var("SET").assign(VAL(set));
                ip.Base.Structure().Var("PRE").assign(VAL((int)!Post));

                if (Vars)
                {
                    ip.SetVars(Vars);
                    ip.Base.AddStruct(Vars->VARS()->Stru());
                }
                ip.Base.Structure().Var("SET").assign(VAL(set));       //EO CH SET
                ip.Base.Structure().Var("PRE").assign(VAL((int)!Post));//EO CH SET

                an_prog.Run(ip);
            }
        }
        catch(OraError ERR)
        {
            ERR.message(0,0,"After prog error");
        }
        ip.Leave();
        cur_arg=pre_cur_arg ;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsDb::ActCalcExecute(TObject *Sender)
{
    if (WithoutVer)
    {
        Message ("Работает только в режиме с версиями");
        return;
    }

    AmsQuery AQ;
    if (DBG->SelectedRows->Count>0)
    {
        if (AskMessage("Пересчитать выделенные позиции БД?")==ID_YES)
        {
            for (int i = 0; i< DBG->SelectedRows->Count; i++)
            {
                ODS->GotoBookmark((void *)DBG->SelectedRows->Items[i].c_str());
                ODS->Edit();
                RunProg(1, 1, 0);
                for (int i = 0; i< FldsDescr.Count(); i++)
                {
                    if ( FldsDescr[i].RealName != "CATID" &&
                         FldsDescr[i].RealName != "ARTICLE" &&
                         FldsDescr[i].RealName != "VER" )
                    {
                        TField* Fld = ODS->Fields->Fields[FldsDescr[i].FldNo];
                        if (!Fld->DataType << ftString)
                          FSM->At(i)->SetVal(Fld);
                    }
                }
                ODS->Post();
            }
        }
    }
    else
    {
        if (AskMessage("Пересчитать все показываемые позиции БД?")==ID_YES)
        {
            ODS->First();
            while (!ODS->Eof)
            {
                ODS->Edit();
                RunProg(1, 1, 0);
                for (int i = 0; i< FldsDescr.Count(); i++)
                {
                    if ( FldsDescr[i].RealName != "CATID" &&
                         FldsDescr[i].RealName != "ARTICLE" &&
                         FldsDescr[i].RealName != "VER" )
                    {
                        TField* Fld = ODS->Fields->Fields[FldsDescr[i].FldNo];
                        if (!Fld->DataType << ftString)
                          FSM->At(i)->SetVal(Fld);
                    }
                }
                ODS->Post();
                ODS->Next();
            }
        }
    };
    //RefreshView();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void __fastcall TFrmBrowsDb::DBGCellClick(TColumn
 *Column)
{
    GetAsyncKeyState(VK_SHIFT);
    GetAsyncKeyState(VK_CONTROL);
    if (!GetAsyncKeyState(VK_SHIFT) && !GetAsyncKeyState(VK_CONTROL))
    {
        if (DBG->SelectedRows->Count<2)
        {
            DBG->SelectedRows->Clear();
        }
    }

    if (DBG->SelectedRows->Count>1)
    {
        BeginSelection();
    }
    else
    {
        EndSelection();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsDb::PopupMenu3Popup(TObject *Sender)
{
//
    GetAsyncKeyState(VK_SHIFT);
    GetAsyncKeyState(VK_CONTROL);
    if (!GetAsyncKeyState(VK_SHIFT) && !GetAsyncKeyState(VK_CONTROL))
    {
        if (DBG->SelectedRows->Count<2)
        {
            DBG->SelectedRows->Clear();
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::ActHideCatWindowExecute(TObject *Sender)
{
    if (CV->Visible)
    {
        ActHideCatWindow->ImageIndex = 20;
        ActHideCatWindow->Caption = "Показать дерево категорий";
        ActHideCatWindow->Hint = "Показать дерево категорий";
    }
    else
    {
        ActHideCatWindow->ImageIndex = 115;
        ActHideCatWindow->Caption = "Скрыть дерево категорий";
        ActHideCatWindow->Hint = "Скрыть дерево категорий";
    }
    CV->Visible= !CV->Visible;
    if (CV->Visible && CV->Width<10)
    {
        CV->Width=50;
    }
}
//---------------------------------------------------------------------------
void TFrmBrowsDb::FormatSums()
{
    // Форматируем суммы как пользователь хотел
    for (int i = 0; i<FldsDescr.Count();i++)
    {
        SFldOpt& XX = FldsDescr[i];
        if (XX.Type==aftSumma && XX.FldNo!=-1)
        {
            TNumericField* NFld = dynamic_cast<TNumericField*>(ODS->Fields->Fields[XX.FldNo]);
            AnsiString S;
            int jjj=0;
            if (XX.MaxPoints==2)
            {
                NFld->DisplayFormat=",0.00";
                S=",0.00";
            }
            else
            {
                S= ",0.";
                int i =0;
                while (i++<XX.MaxPoints) {
                    S+="0"; jjj++;
                }
                NFld->DisplayFormat=S;
            }
            for (int hhh=0;hhh<=5-jjj;hhh++)
              NFld->DisplayFormat+="#";

            if ((XX.DispOpt & DebetCredit) && (XX.DefOpt & DebetCredit))
            {
                TField * Fld=ODS->Fields->FindField(XX.SqlAlias+"dt");
                if (Fld)
                {
                    NFld=dynamic_cast<TNumericField*>(Fld);
                    NFld->DisplayFormat=S;
                    NFld=dynamic_cast<TNumericField*>(ODS->FieldByName(XX.SqlAlias+"ct"));
                    NFld->DisplayFormat=S;
                }
            }

        }

    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::ActEditAnExecute(TObject *Sender)
{
    ActEdit->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::ActVisibilityModeExecute(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------
bool TFrmBrowsDb::BuildInsQuery (AnsiString& S, AmsQuery& AQ, int FldType)
{
    // S должна содержать надпись insert into Tabl (
    AnsiString Pars;
    bool FirstTime=true;
    for (int i = 0; i<FldsDescr.Count(); i++)
    {
        SFldOpt& FO = FldsDescr[i];
        if (FO.FldAddInfo == FldType && FO.FldNo!= -1)
        {
            TField* Fld = ODS->Fields->Fields[FO.FldNo];
            if (! Fld->IsNull)
            {
                if (FirstTime)
                {
                    FirstTime=false;
                }
                else
                {
                    S+=", ";
                    Pars+=", ";
                }
                S+=FO.RealName;
                Pars+=":"+FO.RealName;
                switch (FO.Type)
                {
                    case aftDate:
                        AQ.DeclVar(FO.RealName, Fld->AsDateTime);
                    break;
                    case aftSumma:
                        AQ.DeclVar(FO.RealName, Fld->AsFloat);
                    break;
                    case aftInt:
                    case aftBool:
                        AQ.DeclVar(FO.RealName, Fld->AsInteger);
                    break;
                    case aftStr:
                        AQ.DeclVar(FO.RealName, Fld->AsString);
                    break;
                }
            }
        }
    }
    S+=") values ("+Pars+")";
    return !FirstTime;
};
//---------------------------------------------------------------------------
// Вернет true если есть что updatить
bool TFrmBrowsDb::BuildUpdQuery (AnsiString& S, AmsQuery& AQ, int FldType, TOracleDataSet* ODS_)
{
    // S должна содержать надпись update Tabl set
    bool FirstTime=true;
    for (int i = 0; i<FldsDescr.Count(); i++)
    {
        SFldOpt& FO = FldsDescr[i];
        if (FO.FldAddInfo == FldType && FO.FldNo!= -1)
        {
            TField* Fld = ODS_->Fields->Fields[FO.FldNo];
            if (! Fld->IsNull)
            {
                if (Fld->OldValue.IsNull() || Fld->OldValue != Fld->Value)
                {
                    if (FirstTime)
                    {
                        FirstTime=false;
                    }
                    else
                    {
                        S+=", ";
                    }
                    S+=FO.RealName+"="+":"+FO.RealName;
                    switch (FO.Type)
                    {
                        case aftDate:
                            AQ.DeclVar(FO.RealName, Fld->AsDateTime);
                        break;
                        case aftSumma:
                            AQ.DeclVar(FO.RealName, Fld->AsFloat);
                        break;
                        case aftInt:
                        case aftBool:
                            AQ.DeclVar(FO.RealName, Fld->AsInteger);
                        break;
                        case aftStr:
                            AQ.DeclVar(FO.RealName, Fld->AsString);
                        break;
                    }
                }
            }
            else
            {
                if (!Fld->OldValue.IsNull())
                {
                    if (FirstTime)
                        FirstTime=false;
                    else
                        S+=", ";
                    S+=FO.RealName+"="+":"+FO.RealName;
                    switch (FO.Type)
                    {
                        case aftDate:
                            AQ.DeclVarDate(FO.RealName);
                        break;
                        case aftSumma:
                            AQ.DeclVarSum(FO.RealName);
                        break;
                        case aftInt:
                        case aftBool:
                            AQ.DeclVarInt(FO.RealName);
                        break;
                        case aftStr:
                            AQ.DeclVarStr(FO.RealName);
                        break;
                    }
                }
            }
        }
    }

    if (WithoutVer && !NeedRecalcArt)
        S+=" where ARTICLE=:ART";
    else
        S+=" where ROWID=:CURR_ROWID";
    return !FirstTime;
};
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::ODSApplyRecord(TOracleDataSet *Sender,
      char Action, bool &Applied, AnsiString &NewRowId)
{
    AmsQuery AQ;
    if (Action == 'I')
    {
        AnsiString S= "insert into "+BaseUserName+"."+CurrDBName+"(";
        BuildInsQuery(S, AQ, aiDb);
        AQ.SetSqlTextReady(S);
        AQ.Exec("Вставка позиции БД");
        AQ.Clear();
        AQ.SetSqlTextReady(
            "select ROWID from "+BaseUserName+"."+CurrDBName+
            " where ARTICLE=:ARTICLE and VER=:VER");
        AQ.DeclVarStr("ARTICLE", ODS->FieldByName("ARTICLE")->AsString);
        AQ.DeclVarInt("VER", ODS->FieldByName("VER")->AsInteger);
        AQ.Exec("Получаем ROWID");
        if (AQ.NotEof())
        {
            NewRowId=AQ.GetFldStr(0);
        }
        Applied = true;
    }
    else
    if (Action == 'U')
    {
        AnsiString S= "update "+BaseUserName+"."+CurrDBName+" set ";
        BuildUpdQuery(S, AQ, aiDb,Sender);
        AQ.SetSqlTextReady(S);
        if (WithoutVer)
            AQ.DeclVar("ART", ODS->FieldByName("ARTICLE")->AsString);
        else
            AQ.DeclVar("CURR_ROWID", ODS->RowId());
        AQ.Exec("Update позиции БД");
        Applied = true;
    }
    else
    if (Action == 'D')
    {
        if(WithoutVer)
        {
            AQ.SetPLText(":RES:=*."+CurrDBName+"_DELPOS(:ART,NULL,:RES_TXT,:RES1);");
            AQ.DeclVar("RES",0);
            AQ.DeclVar("RES1",0);
            AQ.DeclVar("RES_TXT","");
            AQ.DeclVar("ART", ODS->FieldByName("ARTICLE")->AsString);
            AQ.DeclVar("VER", ODS->FieldByName("VER")->AsInteger);

        }
        else
        {
            if (FldsDescr.IsNullRule() && !ODS->FieldByName("VER")->AsInteger )
            {
                AQ.SetSqlTextReady(
                    "select 1 from "+BaseUserName+"."+CurrDBName+
                    " where ARTICLE=:ARTICLE and VER!=0 and ROWNUM<2");
                AQ.DeclVar("ARTICLE", ODS->FieldByName("ARTICLE")->AsString);
                AQ.Exec("Проверяем условие нулевых версий");
                if (AQ.NotEof())
                {
                    THROW_AMS_EXC("Сначала необходимо удалить все не нулевые версии");
                }
                AQ.Clear();
            }
            AQ.SetPLText(":RES:=*."+CurrDBName+"_DELPOS(:ART,:VER,:RES_TXT,:RES1);");
            AQ.DeclVar("RES1",0);
            AQ.DeclVar("RES",0);
            AQ.DeclVar("RES_TXT","");
            AQ.DeclVar("ART", ODS->FieldByName("ARTICLE")->AsString);
            AQ.DeclVar("VER", ODS->FieldByName("VER")->AsInteger);
        }
        AQ.Exec("Удаление позиции");
        if (AQ.GetVarInt("RES")==0)
        {
            THROW_AMS_EXC("Немогу удалить. "+AQ.GetVarStr("RES_TXT"));
        }

        Applied = true;
    }

    if (WithoutVer)
        Applied = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsDb::ActSumInfoExecute(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------


void __fastcall TFrmBrowsDb::ActGotoExecute(TObject *Sender)
{
    // Перейти по категориям
    int CatId = DatMod->GetDbCat(CurrDBId, CurrDBName, CurrCatId);
    if (CatId!=0)
    {
        CV->MakeActive(CatId);
    }
}
//---------------------------------------------------------------------------
void TFrmBrowsDb::BeginEdit(int Key)
{
//
    int Flags = eoDbFlds;
    CurrFlags=Flags;
    HaveFirstDigit = true;
    FirstDigit = Key;
    ActEdDbPosExec (Flags);
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::ActFindInCurrCatExecute(TObject *Sender)
{
    //
    FindInCurrCat = true;
    ActFind->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::ActWCatExecute(TObject *Sender)
{
//
    ActBezKatExecute(this);    
}
//---------------------------------------------------------------------------
void TFrmBrowsDb::EditOperVars()
{
};
//---------------------------------------------------------------------------
void TFrmBrowsDb::SetFilter()
{
};
//---------------------------------------------------------------------------
void DefineVar (AmsQuery& AQ, SFldOpt& FO, const AnsiString& Val)
{
    switch (FO.Type)
    {
        case aftInt:
        case aftBool:
            AQ.DeclVarInt(FO.RealName, Val.ToInt());
        break;

        case aftStr:
            AQ.DeclVarStr(FO.RealName, Val);
        break;

        case aftSumma:
            AQ.DeclVarSum(FO.RealName, Val.ToDouble());
        break;
        case aftDate:
            AQ.DeclVarDate(FO.RealName, StrToDate(Val));
        break;
    }
}
//---------------------------------------------------------------------------
int InsAndUpd (TFldsDescr& FldsDescr, AnsiString& S, bool NoVer, std::vector<int>&Indx)
{
    int i = 1;
    int Counter=0;

    AmsQuery AQ;

    bool IgnoreProhNewArt= false;

    while (i<S.Length())
    {
        AnsiString SqlIns1="CATID, ARTICLE, VER";
        AnsiString SqlIns2=":CATID, :ARTICLE, :VER";
        AnsiString SqlUpd;
        AQ.Clear();

        unsigned int FldIndx=0;
        AnsiString NextWord;
        AnsiString Art, Ver;
        if (NoVer) AQ.DeclVarInt("VER", 0);
        // строим запрос для вставки/updat
        AnsiString AddStr;
        int Res = 0;
        do {
            Res=GetNextWord(S, i, NextWord);
            if (Res<2)
            {
                if (FldIndx< Indx.size())
                {
                    SFldOpt& FO = FldsDescr[Indx[FldIndx]];
                    if (FO.RealName=="ARTICLE" || FO.RealName=="VER")
                    {
                        if (FO.RealName=="ARTICLE")
                            Art=NextWord;
                        else
                            Ver=NextWord;
                        if (NextWord!="")
                            DefineVar(AQ, FO, NextWord);
                    }
                    else
                    if (NextWord!="")
                    {
                        SqlIns1+=", "+FO.RealName;
                        SqlIns2+=", :"+FO.RealName;
                        bool Upd= false;
                        if (
                            (FO.DispOpt & Editable) &&
                            (FO.DefOpt & Editable) && (FO.DefOpt & AlwaysFree))
                        {
                            if (!NoVer || (FO.DefOpt&CommonVer))
                            {
                                if (SqlUpd!="") SqlUpd+=", ";
                                SqlUpd+=FO.RealName+"=:"+FO.RealName;
                                Upd=true;
                            }
                        }
                        DefineVar(AQ, FO, NextWord);
                        if (!Upd)
                        {
                            AddStr+=":"+FO.RealName+":=:"+FO.RealName+"; ";
                        }
                    }
                    FldIndx++;
                }
                if (Res==1)
                {
                    AnsiString Sql;
                    Sql="begin declare i number(1); begin "
                    "select max(1) into i from *."+FldsDescr.GetDbName()+
                    " where ARTICLE=:ARTICLE";
                    if (NoVer) Sql+="; ";
                    else Sql+=" and VER=:VER;";
                    Sql+="if i=1 then "
                    "update *."+FldsDescr.GetDbName()+" set "+SqlUpd+
                    " where ARTICLE=:ARTICLE";
                    if (NoVer) Sql+=";";
                    else Sql+=" and VER=:VER; ";
                    Sql+=":RES_UPD:=1; else :RES_UPD:=-1; end if; "
                        +AddStr+"end;commit;end;";
                    if (Art!="")
                    {
                        AQ.DeclVarInt("RES_UPD");
                        ShortMessage(Art+" "+Ver);
                        AQ.SetSqlText(Sql);
                        AQ.Exec("update производим");
                        if (AQ.GetVarInt("RES_UPD")==-1)
                        {  // Если не обнаружено такого артикула
                            Sql="";
                            if (G.ur.imp_opt&PROH_NEWART)
                            {
                                if (!IgnoreProhNewArt && AskMessage("Встретился артикул "+Art+" "+Ver+
                                    " прекратить выполнение импорта?")==ID_YES)
                                {
                                    THROW_AMS_EXC("Прекращено выполнение импорта");
                                }
                                else
                                {
                                    if (!IgnoreProhNewArt && AskMessage("Больше не задавать этот вопрос", false)==ID_YES)
                                        IgnoreProhNewArt= true;
                                }
                            }
                            else
                            {
                                if ((G.ur.imp_opt&DONOT_ASK) ||
                                    AskMessage("Вставить новый артикул "+Art+" "+Ver+"?", false)==ID_YES
                                )
                                {
                                    AQ.DeclVarInt("CATID");
                                    if (!NoVer)
                                    {
                                        Sql+="begin :RES_UPD:=0; select max(CATID) into :CATID from *."+
                                        FldsDescr.GetDbName()+" where ARTICLE=:ARTICLE;"
                                        "if :CATID is NULL then "
                                        "select max(OWNCAT) into :CATID from *.DBCAT D, *.OBJS "
                                        " where NAME=:NAME and OID=DBID and D.CATID is NULL;"
                                        "end if;";
                                    }
                                    else
                                    {
                                        Sql+="select max(OWNCAT) into :CATID from *.DBCAT D, *.OBJS "
                                        " where NAME=:NAME and OID=DBID and D.CATID is NULL;";
                                    }
                                    AQ.DeclVar("NAME", FldsDescr.GetDbName());
                                    Sql+="insert into *."+FldsDescr.GetDbName()+" ("+SqlIns1+
                                    ") values ("+SqlIns2+"); "
                                    "commit; end;";
                                    AQ.SetSqlText(Sql);
                                    ShortMessage("Вставка "+Art+" "+Ver);
                                    AQ.Exec("Вставка ");
                                    Counter++;
                                }
                            }
                        }
                        else
                            Counter++;
                    } // Art!=""
                }
            }
        }
        while (Res<1);
    }
    return Counter;
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsDb::ActCopyFromExcelExecute(TObject *Sender)
{
//
    if (Clipboard()->HasFormat(CF_TEXT))
    {
        AnsiString S = Clipboard()->AsText;
        std::vector<int> Indx;
        TFrmAskFldsNames* Frm;
        Application->CreateForm(__classid(TFrmAskFldsNames), &Frm);
        if (Frm->AskFldsNames(Indx, S, FldsDescr)==1)
        {
            bool HaveArt= false;
            bool HaveVer= false;
            unsigned int i =0;
            while (i<(Indx.size()-1) && (!HaveArt || !HaveVer))
            {
                if (FldsDescr[Indx[i]].RealName=="ARTICLE")
                    HaveArt=true;
                if (FldsDescr[Indx[i]].RealName=="VER")
                    HaveVer=true;
                i++;
            }
            if (!HaveArt)
            {
                ErrorMessage("Нет артикула, вставка невозможна");
                return;
            }
            int Count=InsAndUpd (FldsDescr, S, !HaveVer, Indx);
            RefreshView();
            Message("Произведено "+IntToStr(Count)+" изменений");
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsDb::ButEnterClick(TObject *Sender)
{
//
    if (CV->Focused())
    {
        if (CV->Selected->HasChildren)
            CV->Selected->Expanded= !CV->Selected->Expanded;
        else
            DBG->SetFocus();
    }
    else
    if (DBG->Focused())
    {
        ActEditAn->Execute();
    }
}
//---------------------------------------------------------------------------
OpVars* TFrmBrowsDb::GetOpVars(){return Vars->VARS();};
//---------------------------------------------------------------------------

void __fastcall TFrmBrowsDb::ActEscExitExecute(TObject *Sender)
{
  this->Close();
}
//---------------------------------------------------------------------------







