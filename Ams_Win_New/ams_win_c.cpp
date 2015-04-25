//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "amsclnt.h"

#include "ams_win_c.h"
#include "DAT_MOD.h"
#include "FRM_CHOOSE_OBJ.h"
#include "FRM_OPER_LIST.h"
#include "FRM_PERIOD.h"
#include "FRM_WINDOWS_LIST.h"
#include "FRM_CHOOSE_DB_POS.h"
#include "ADD_MODULE.h"
#include "FRM_ACC_LIST.h"
#include "FRM_CHOOSE_STD_OPER.h"
#include "FRM_CALC.h"
#include "Impoper.h"
#include "FRM_USER_OPTS.h"
#include "FRM_MAP.h"
#include "FRM_EXIT.h"
#include "FRM_INPUT_OPER.h"
#include "AboutBoxUnit.h"
#include "FRM_CUSTOM_PERIOD.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RxGIF"
#pragma resource "*.dfm"
#define CodeOperLen 5
#define ShLen 5
TAmsWinC *AmsWinC;

extern void ClrAction( TAction* );
extern void SetAction( TAction* );

void MakeRoleDepend()
{
    // Прячет Menu в зависимости от ролей
    ClrAction(AmsWinC->ActJurOper);
    ClrAction(AmsWinC->ActImport);
    ClrAction(AmsWinC->ActImportSettings);
    ClrAction(AmsWinC->ActInpOper);
    ClrAction(AmsWinC->ActOtlOper);
    ClrAction(AmsWinC->ActOtForm);
    ClrAction(AmsWinC->ActBD);
    ClrAction(AmsWinC->ActChangeName);
    ClrAction(AmsWinC->ActNewM);
    ClrAction(AmsWinC->ActDelsOper);
    ClrAction(AmsWinC->ActDelsOtlOper);
    ClrAction(AmsWinC->ActPerVis);
    //FrmAccList->N2->Visible=false;

    AmsQuery AQ;
    AQ.SetSqlTextReady(
        "select ROLE from SESSION_ROLES where ROLE LIKE '"+
        BaseUserName+"%'");
    AQ.Exec("Выясняем какие роли доступны");
    while(AQ.NotEof())
    {
        AnsiString Role= AQ.GetFldStr(0);
        if (Role==BaseUserName+"SUPER")
        {
            SetAction(AmsWinC->ActJurOper);
            SetAction(AmsWinC->ActImport);
            SetAction(AmsWinC->ActImportSettings);
            SetAction(AmsWinC->ActInpOper);
            SetAction(AmsWinC->ActOtlOper);
            SetAction(AmsWinC->ActOtForm);
            SetAction(AmsWinC->ActBD);
            SetAction(AmsWinC->ActChangeName);
            SetAction(AmsWinC->ActNewM);
            SetAction(AmsWinC->ActDelsOper);
            SetAction(AmsWinC->ActDelsOtlOper);
            SetAction(AmsWinC->ActPerVis);
            //FrmAccList->N2->Visible=true;
            return;
        }
        else
        if (Role==BaseUserName+"_NEWMONTH")
        {
            SetAction(AmsWinC->ActNewM);
        }
        else
        if (Role==BaseUserName+"_VIEWDB")
        {
            SetAction(AmsWinC->ActBD);
        }
        else
        if (Role==BaseUserName+"_VIEWFORM")
        {
            SetAction(AmsWinC->ActOtForm);
        }
        else
        if (Role==BaseUserName+"_EXIM")
        {
            SetAction(AmsWinC->ActImport);
            SetAction(AmsWinC->ActImportSettings);
        }
        else
        if (Role==BaseUserName+"_OPERLIST")
        {
            SetAction(AmsWinC->ActJurOper);
        }
        if (Role==BaseUserName+"_INPUTOPER")
        {
            SetAction(AmsWinC->ActInpOper);
        }
        else
        if (Role==BaseUserName+"_HISTORY")
        {
            SetAction(AmsWinC->ActDelsOper);
            SetAction(AmsWinC->ActDelsOtlOper);
        }
        else
        if (Role==BaseUserName+"_CASSA")//пока для работы кассиров
        {
            SetAction(AmsWinC->ActJurOper);
            SetAction(AmsWinC->ActInpOper);
        }
        else

        if (Role==BaseUserName+"_INPREMS")
        {
           //FrmAccList->N2->Visible=true;
        }
        else
        if (Role==BaseUserName+"_NEGREMS")
        {
            //
        }
        else
        if (Role==BaseUserName+"_DEADDATE")
        {
            //
        }
        else
        if (Role==BaseUserName+"_UNAPP")
        {
           SetAction(AmsWinC->ActOtlOper); //
        }
        else
        if (Role==BaseUserName+"_VIEWDATE")
        {
            //
        }
        else
        if (Role==BaseUserName+"_PERIODS")
        {
            SetAction(AmsWinC->ActPerVis);
        }

        AQ.Next();
    }
    if (GC::ReadOnly)
    {
        ClrAction(AmsWinC->ActJurOper);
        ClrAction(AmsWinC->ActImport);
        ClrAction(AmsWinC->ActImportSettings);
        ClrAction(AmsWinC->ActInpOper);
    }
}
//---------------------------------------------------------------------------
void ActivateForm(TForm* Frm)
{
    Frm->SetFocus();
    Frm->BringToFront();
    Frm->WindowState=wsNormal;
    if (Frm->WindowState==wsMinimized)
    {
        Frm->WindowState=wsNormal;
    }
}
//---------------------------------------------------------------------------
__fastcall TAmsWinC::TAmsWinC(TComponent* Owner)
    : TForm(Owner)
{
    FrmChooseDb= 0;
    FrmChooseForms=0;
    FrmChooseConst=0;
    FrmAccList=0;
    LVD=0;
    Loaded=true;
    Shtrih = "";
    UseShtrih = false;
//CH 28-03-02
    if (GC::GUI)
       lv->Visible=true;
    if (GC::ToolBar)
       ToolBar1->Visible=false;
    if (GC::Status)
       StatusBar1->Visible=false;
    if (GC::MainMenu)
       AmsWinC->Menu=NULL;
//EO CH 28-03-02
}
//---------------------------------------------------------------------------
void __fastcall TAmsWinC::N11Click(TObject *Sender)
{
    // Базы данных
    if (!FrmChooseDb)
    {
        Application->CreateForm (__classid(TFrmChooseObj), &FrmChooseDb);
        FrmChooseDb->Begin(otDB);
    }
    else
    {
        ActivateForm(FrmChooseDb);
    }
}
//---------------------------------------------------------------------------
void __fastcall TAmsWinC::N15Click(TObject *Sender)
{

    if (AskQuestion("Изменить имя пользователя?") == ID_YES)
    {
        DatMod->Login(true);
    }
}
//---------------------------------------------------------------------------

void TAmsWinC::SDisplay(int WindowNo, const AnsiString& Msg)
{
    StatusBar1->Panels->Items[WindowNo]->Text= Msg;
}
//---------------------------------------------------------------------------

void __fastcall TAmsWinC::ActHelpExecute(TObject *Sender)
{
    int Res = 0;

    TForm* Frm = NULL;
    if (TopMostForm)
        Frm = TopMostForm;
    else
        this->ActiveMDIChild;
    if (!Frm)
        Res = 1;
    else
        Res = Frm->HelpContext;
    Application->HelpContext(Res);
}
//---------------------------------------------------------------------------

void __fastcall TAmsWinC::N19Click(TObject *Sender)
{
    Tile();
}
//---------------------------------------------------------------------------
void __fastcall TAmsWinC::ActSetSqlDebugExecute(TObject *Sender)
{
    GC::SqlDebug= !GC::SqlDebug;
    AmsSqlDebug=GC::SqlDebug;
    if (GC::SqlDebug)
    {
        ActSetSqlDebug->Caption ="Убрать SQL отладку";
    }
    else
    {
        ActSetSqlDebug->Caption ="Установить SQL отладку";
    }
}

//---------------------------------------------------------------------------
TFrmChooseDbPos* TAmsWinC::GetChooseFrm(int DbId, const AnsiString& SetName)
{
    AnsiString S = GetKey(DbId, SetName);
    TFrmChooseDbPos * F;
    //if (!F)           //EO CH "Фильтр не обновляется"
    {
        Application->CreateForm(__classid(TFrmChooseDbPos), &F);
        ChoosePos[S]=F;
        SKeyToFrm FK;
        FK.Key=S;
        FK.Frm= F;
        try
        {
            F->Begin(DbId, "", "", SetName);
        }
        catch (...)
        {
            //ChoosePos[S]=NULL;
            F->Release();
            throw;
        }
        AllFrms.push_back(FK);
    }
    return F;
};
//---------------------------------------------------------------------------

void __fastcall TAmsWinC::N14Click(TObject *Sender)
{
    Application->Terminate();
}
//---------------------------------------------------------------------------
void __fastcall TAmsWinC::N20Click(TObject *Sender)
{
 TFrmPeriod* F;
 Application->CreateForm(__classid(TFrmPeriod), &F);

 F->SetInitPeriod(G.ur.beg_date.val.T(),G.ur.end_date.val.T()) ;

 int res=F->ShowModal();
 if (res==1)
 {
  DateTime DT ;
  DT=F->DSet->FieldByName("POINT_DATE")->AsDateTime ;
  G.ur.beg_date=DT ;
  DT=F->DSet1->FieldByName("END_DATE")->AsDateTime ;
  G.ur.end_date=DT ;

  AmsQuery Q ;
  Q.DeclVarDate("BEG_DATE",F->DSet->FieldByName("POINT_DATE")->AsDateTime) ;
  Q.DeclVarDate("END_DATE",F->DSet1->FieldByName("END_DATE")->AsDateTime) ;
  Q.SetSqlText(" UPDATE *.user_options SET beg_date=:beg_date, end_date=:end_date WHERE username=user") ;
  Q.Exec("Запись периода") ;

  char buf[20] , buf1[20] ;
  Application->Title=AnsiString("AMS-")+G.bn.bookname.val.data()+" ["+G.ur.beg_date.val.T().DateString()+" - "+G.ur.end_date.val.T().DateString()+"]";
  Caption = Application ->Title;
 }
}
//---------------------------------------------------------------------------
TForm* LastForm=NULL;
TForm* SaveForm=NULL;
//---------------------------------------------------------------------------
TForm* GetTopForm()
{
    return LastForm? LastForm : AmsWinC->ActiveMDIChild;
};
//---------------------------------------------------------------------------
void __fastcall TAmsWinC::ActInpOperExecute(TObject *Sender)
{
    lv->Visible=false;
    // Ввод операции
    TForm* LF = GetTopForm();
    TFrmOperListRoot* OL= dynamic_cast<TFrmOperListRoot*>(LF);
    SaveForm=OL;
    if (FrmChooseStdOper)
    {
        int i=0;
        while(i<MDIChildCount && FrmChooseStdOper!=MDIChildren[i])
        {
            i++;
        }
        if (i>=MDIChildCount)
        {
            FrmChooseStdOper=0;
        }
    }
    if (!FrmChooseStdOper)
    {
        TFrmChooseStdOper* Frm;
        Application->CreateForm (__classid(TFrmChooseStdOper), &Frm);
        Frm->Begin();
        FrmChooseStdOper=Frm;
    }
    else
    {
        FrmChooseStdOper->BringToFront();
        FrmChooseStdOper->WindowState=wsNormal;
        FrmChooseStdOper->SetFocus();
    };
}
//---------------------------------------------------------------------------
void __fastcall TAmsWinC::ActJurOperExecute(TObject *Sender)
{
    lv->Visible=false;
 TFrmOperList* F;
 Application->CreateForm(__classid(TFrmOperList), &F);
 F->Init();
 F->Show();
}
//---------------------------------------------------------------------------
void __fastcall TAmsWinC::ActRasrOperExecute(TObject *Sender)
{
    lv->Visible=false;
 TFrmOperList* F;
 Application->CreateForm(__classid(TFrmOperList), &F);
 F->Init(0,0,0,"",1);
 F->Show();
}
//---------------------------------------------------------------------------

void __fastcall TAmsWinC::ActOtlOperExecute(TObject *Sender)
{
    lv->Visible=false;
 TFrmOperList* F;
 Application->CreateForm(__classid(TFrmOperList), &F);
 F->Init(1);
 F->Show();

}
//---------------------------------------------------------------------------

void __fastcall TAmsWinC::ActOtFormExecute(TObject *Sender)
{
    lv->Visible=false;
    if (!FrmChooseForms)
    {
        Application->CreateForm (__classid(TFrmChooseObj), &FrmChooseForms);
        FrmChooseForms->Begin(otRepForm);
    }
    else
    {
        ActivateForm(FrmChooseForms);
    }
}
//---------------------------------------------------------------------------

void __fastcall TAmsWinC::ActBDExecute(TObject *Sender)
{
    lv->Visible=false;
    // Базы данных
    if (!FrmChooseDb)
    {
        Application->CreateForm (__classid(TFrmChooseObj), &FrmChooseDb);
        FrmChooseDb->Begin(otDB);
    }
    else
    {
        ActivateForm(FrmChooseDb);
    }
}
//---------------------------------------------------------------------------
void __fastcall TAmsWinC::ActConstExecute(TObject *Sender)
{
    lv->Visible=false;
    if (!FrmChooseConst)
    {
        Application->CreateForm (__classid(TFrmChooseObj), &FrmChooseConst);
        FrmChooseConst->Begin(otConst);
    }
    else
    {
        ActivateForm(FrmChooseConst);
    }
}
//---------------------------------------------------------------------------

void __fastcall TAmsWinC::ActChangeNameExecute(TObject *Sender)
{
    if (AskQuestion("Изменить имя пользователя?") == ID_YES)
    {
        DatMod->Login(true);
    }

}
//---------------------------------------------------------------------------

void __fastcall TAmsWinC::ActExitExecute(TObject *Sender)
{
    Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TAmsWinC::ActPerVisExecute(TObject *Sender)
{
 TFrmPeriod* F;
 Application->CreateForm(__classid(TFrmPeriod), &F);

 F->SetInitPeriod(G.ur.beg_date.val.T(),G.ur.end_date.val.T()) ;

 int res=F->ShowModal();
 if (res==1)
 {
  DateTime DT ;
  DT=F->DSet->FieldByName("POINT_DATE")->AsDateTime ;
  G.ur.beg_date=DT ;
  DT=F->DSet1->FieldByName("END_DATE")->AsDateTime ;
  G.ur.end_date=DT ;

  AmsQuery Q ;
  Q.DeclVar("BEG_DATE",F->DSet->FieldByName("POINT_DATE")->AsDateTime) ;
  Q.DeclVar("END_DATE",F->DSet1->FieldByName("END_DATE")->AsDateTime) ;
  Q.SetSqlText(" UPDATE *.user_options SET beg_date=:beg_date, end_date=:end_date WHERE username=user") ;
  Q.Exec("Запись периода") ;

  char buf[20] , buf1[20] ;
  Application->Title=AnsiString("AMS-")+G.bn.bookname.val.data()+" ["+G.ur.beg_date.val.T().DateString()+" - "+G.ur.end_date.val.T().DateString()+"]";
  Caption = Application ->Title;
 }

}
//---------------------------------------------------------------------------

void __fastcall TAmsWinC::ActDelsOperExecute(TObject *Sender)
{
    lv->Visible=false;
 TFrmOperList* F;
 Application->CreateForm(__classid(TFrmOperList), &F);
 F->Init(0,1);
 F->Show();

}
//---------------------------------------------------------------------------

void __fastcall TAmsWinC::ActDelsOtlOperExecute(TObject *Sender)
{
    lv->Visible=false;
 TFrmOperList* F;
 Application->CreateForm(__classid(TFrmOperList), &F);
 F->Init(1,1);
 F->Show();
}
//---------------------------------------------------------------------------
void __fastcall TAmsWinC::ActListWindExecute(TObject *Sender)
{
    TFrmWindowsList* F;
    Application->CreateForm(__classid(TFrmWindowsList), &F);
    TForm* SelForm= F->SelWindow(this);
    F->Hide();
    F->Release();
    if (SelForm)
    {
        SelForm->Show();
    }

}
//---------------------------------------------------------------------------

void __fastcall TAmsWinC::ActCascadeExecute(TObject *Sender)
{
    Cascade();
}
//---------------------------------------------------------------------------

void __fastcall TAmsWinC::ActTileExecute(TObject *Sender)
{
 Tile();
}
//---------------------------------------------------------------------------


void __fastcall TAmsWinC::FormCreate(TObject *Sender)
{
  HelpContext=1;
}
//---------------------------------------------------------------------------

void __fastcall TAmsWinC::ActNewMExecute(TObject *Sender)
{
 AmsQuery Q ;
 Q.DeclVarDate("POINT_DATE") ;
 Q.SetPLText("*.periods.NextDate(:POINT_DATE);") ;
 Q.Exec("Get next date") ;

 Word Y, M, D;
 DecodeDate (Q.GetVarDate("POINT_DATE").DateString(), Y, M, D);
 AnsiString s="Начать новый месяц "+FormatDateTime("mmmm" ,EncodeDate (Y, (M==1)?12:M-1, D))+"?";

 if (AskQuestion(s)==ID_YES)
 {
  try
  {
   Q.Clear() ;
   Q.SetSqlText("ALTER ROLLBACK SEGMENT MYROLL ONLINE") ;
   Q.Exec("alter roll") ;
  } catch(...) {}

  Q.Clear() ;
  Q.SetPLText("*.periods.BeginNextMonth;") ;
  Q.Exec("next month") ;
 }
}
//---------------------------------------------------------------------------

void __fastcall TAmsWinC::TestAnSum1Click(TObject *Sender)
{
    TFrmChooseObj* Frm;
    Application->CreateForm (__classid(TFrmChooseObj), &Frm);
    Frm->Begin(otAnal);
}
//---------------------------------------------------------------------------

void __fastcall TAmsWinC::N24Click(TObject *Sender)
{
    TFrmUserOpts* Frm;
    Application->CreateForm(__classid (TFrmUserOpts), &Frm);
    Frm->UserOpts();
    Frm->Release();
}
//---------------------------------------------------------------------------

void __fastcall TAmsWinC::FormClose(TObject *Sender, TCloseAction &Action)
{
//    if (AskQuestion("Выйти из бухгалтерии")==ID_YES)
    if (AskQuestion("     Выйти из бухгалтерии?")==ID_YES)
    {
        Action= caFree;
    }
    else
    {
        Action=caNone;
    }
    
}
//---------------------------------------------------------------------------

void __fastcall TAmsWinC::ActCalcExecute(TObject *Sender)
{
    FrmCalc->Show();
    FrmCalc->ChB->SetFocus();    
}
//---------------------------------------------------------------------------

void __fastcall TAmsWinC::N26Click(TObject *Sender)
{
    if (DatMod->OLogon->SetPassword())
    {
        Message("Пароль изменен успешно");
    }
}
//---------------------------------------------------------------------------

void __fastcall TAmsWinC::N25Click(TObject *Sender)
{
    AmsQuery OQ;
    OQ.SetSqlTextReady(
        "select FULL_NAME from "+BaseUserName+".USER_OPTIONS where USERNAME=USER");
    OQ.Exec("Имя пользователя");
    if (OQ.NotEof())
    {
        AnsiString UName=OQ.GetFldStr(0);
        if (AskString ("Полное имя пользователя", 80 , UName))
        {
            OQ.SetPLText("update *.USER_OPTIONS set FULL_NAME=:FULL_NAME where USERNAME=USER;commit;");
            OQ.DeclVar("FULL_NAME", UName);
            OQ.Exec("Изменяем имя пользователя");
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TAmsWinC::MNViewDebugClick(TObject *Sender)
{
    GC::ViewDebug=!GC::ViewDebug;
    if (GC::ViewDebug)
        MNViewDebug->Caption="Отменить VIEWDEBUG";
    else
        MNViewDebug->Caption="Установить VIEWDEBUG";
    AmsViewDebug = GC::ViewDebug;
}
//---------------------------------------------------------------------------
void __fastcall TAmsWinC::ActImportExecute(TObject *Sender)
{
//
    TextOpImport();
}
//---------------------------------------------------------------------------
extern void ImportOptions();
void __fastcall TAmsWinC::ActImportSettingsExecute(TObject *Sender)
{
//
    ImportOptions();
}
//---------------------------------------------------------------------------

void __fastcall TAmsWinC::ActShowMapExecute(TObject *Sender)
{
    lv->Visible=false;
 TFrmMap* F;
 Application->CreateForm(__classid(TFrmMap), &F);
 F->Show();
}
//---------------------------------------------------------------------------

void __fastcall TAmsWinC::ActShowPlanAccExecute(TObject *Sender)
{
    if (!FrmAccList)
    {
        Application->CreateForm (__classid(TFrmAccList), &FrmAccList);
    }
    else
    {
        ActivateForm(FrmAccList);
    }
}
//---------------------------------------------------------------------------


void __fastcall TAmsWinC::FindActiveMDIChild()
{
    if ((GC::GUI)&&(this->MDIChildCount==1))
       lv->Visible=true;
}
//---------------------------------------------------------------------------
//запускает объект по его коду
int __fastcall TAmsWinC::ExecGUIAction(AmsOType type, int id)
{
   lv->Visible=false;
   switch (type)
   {
     case   0: ActShowMapExecute(NULL); break;
     case   1: ActOtFormExecute(NULL); break;
//     case   2: ActShowMap(NULL);
     case   3: ExecInpOper(id); break;
     case   4: ExecViewDB(id); break;
//     case   5: ActShowMap(NULL);
     case   6: ActConstExecute(NULL); break;
     case   7: ActJurOperExecute(NULL); break;
     default : ShowMessage("Действие над объектом не определено"); lv->Visible=true;
   }
}
//---------------------------------------------------------------------------

int __fastcall TAmsWinC::CreateGUI()
{
    TImageList* il=new TImageList(this);
    lv->LargeImages = il;
    lv->SmallImages = il;
    il->Clear();
    il->Height = GC::SizePic;
    il->Width = GC::SizePic;
    AmsQuery OQ;
    OQ.SetSqlText(
        "select distinct aa.aarticle,a.otype, a.наим, a.path from "
        "*.ан_юзер AU, *.ан_экшион AA, *.op_items OI, *.op_items OIS, *.opers O, *.юзер U, *.экшион A "
        "where o.op_id=oi.op_id and o.op_subid=oi.op_subid and o.op_id=ois.op_id "
        "and o.op_subid=ois.op_subid and oi.item_sum_id=au.sumid and "
        "ois.item_sum_id=aa.sumid and u.article=au.aarticle and u.ver=au.aver "
        "and au.aarticle>' ' and aa.aarticle>' ' and u.article=:UNAME and a.article=aa.aarticle and a.ver=aa.aver ");
    OQ.DeclVar("UNAME", DatMod->OSession->LogonUsername);
    OQ.Exec("Имя пользователя");
    int i=0;
    while (OQ.NotEof())
    {
      LVD=new TListViewData();
      LVD->id=OQ.GetFldInt(0);
      LVD->type=OQ.GetFldInt(1);
      LVD->descr=OQ.GetFldStr(2);
      LVD->path=OQ.GetFldStr(3);
      il->FileLoad(rtBitmap, LVD->path, clNone);

      TListColumn  *NewColumn;
      TListItem  *ListItem;
      TListView  *ListView = lv;

      ListView->Parent = this;
      ListView->Align = alClient;

      ListItem = ListView->Items->Add();
      ListItem->Caption = OQ.GetFldStr(2);
      ListItem->ImageIndex=i++;
      ListItem->Data=(void*)LVD;
      OQ.Next();
    }

}
//---------------------------------------------------------------------------

void __fastcall TAmsWinC::lvClick(TObject *Sender)
{
     if (lv->SelCount)
         ExecGUIAction(((TListViewData*)(lv->Selected->Data))->type, ((TListViewData*)(lv->Selected->Data))->id);
}
//---------------------------------------------------------------------------

__fastcall TAmsWinC::ExecInpOper(int id)
{
        TFrmInputOper* Frm;
        Application->CreateForm(__classid (TFrmInputOper), &Frm);
        Frm->Init(0,0,0,0,id);
}
//---------------------------------------------------------------------------

__fastcall TAmsWinC::ExecViewDB(int id)
{
    ActBDExecute(NULL);
    FrmChooseDb->EditDB(id);
    FrmChooseDb->~TFrmChooseObj();
    FrmChooseDb=0;
}
//---------------------------------------------------------------------------

void __fastcall TAmsWinC::lvMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
    if (Loaded)
    {
       Loaded=false;
       CreateGUI();
    }
}
//---------------------------------------------------------------------------

void __fastcall TAmsWinC::N32Click(TObject *Sender)
{
   Application->CreateForm (__classid(TAboutBox), &AboutBox);
   AboutBox->ShowModal();

   delete AboutBox;
}
//---------------------------------------------------------------------------

void __fastcall TAmsWinC::ToolButton21Click(TObject *Sender)
{
   if (UseShtrih)
   {
     MDIChildren[0]->Show();      //активизируем план счетов
     UseShtrih = false;
   }
   else
   {
     edShtrih->SetFocus();
     UseShtrih = true;
   }
   Shtrih = "";
}
//---------------------------------------------------------------------------

void __fastcall TAmsWinC::edShtrihKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
      if (Key==13)
      {
        int id_ = StrToInt(Shtrih.SubString(1,CodeOperLen));
        ExecGUIAction(3, id_);
        Shtrih = "";
      }
}
//---------------------------------------------------------------------------

void __fastcall TAmsWinC::edShtrihKeyPress(TObject *Sender, char &Key)
{
          Shtrih = Shtrih+Key;
}
//---------------------------------------------------------------------------

void __fastcall TAmsWinC::N35Click(TObject *Sender)
{
  FrmCustomPeriod->Show();
  FrmCustomPeriod->ButClose->SetFocus();
}
//---------------------------------------------------------------------------

