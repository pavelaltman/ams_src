//---------------------------------------------------------------------------
#include "AMS_HDR.h"
#pragma hdrstop

#include <vector>

#include "MAIN_FRM.h"
#include "FRM_ACCS.h"
#include "FRM_ACCS_STR.h"
#include "PRINT_ERROR.h"
#include "Frm_Cat_Tree.h"
#include "FRM_COPY_STD_OP.h"
#include "FRM_CONDS_EDIT.h"
#include "AMS_UTILS.h"
#include "FRM_INPUT_VALUE.h"
#include "FRM_BUILD_BUSY_PROG.h"
#include "FRM_BUILD_AN_FUNC.h"
#include "FRM_GIVE_RIGHTS.h"
#include "AMS_READ_CNF.h"
#include "FRM_CHOOSE_OBJ_ADM.h"
#include "FRM_COPY_TREE.h"
#include "FRM_CHOOSE_OBJ_A_DB.h"
#include "FRM_EDIT_GLOBAL_SETUP.h"
#include "FRM_EDIT_BUCH_SETUP.h"
#include "FRM_INPUT_PASS.h"
#include "FRM_SEL_CPS_DATE.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Oracle"
#pragma link "OracleData"
#pragma resource "*.dfm"
TMainFrm *MainFrm;
AnsiString LastCopyWhere;
AnsiString WhereCopyPwd;
//---------------------------------------------------------------------------
__fastcall TMainFrm::TMainFrm(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TMainFrm::ActShowAccExecute(TObject *Sender)
{
  //  TFrmAccs * F;
  //  Application->CreateForm (__classid(TFrmAccs),   &F);
  //  F->Begin(
 //FrmAccs->Show();
 //FrmAccsStr->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainFrm::N3Click(TObject *Sender)
{
  if (AskMessage("Переконнектится?")==ID_YES)
  {
    InitConn();
  }
}
//---------------------------------------------------------------------------
int TMainFrm::InitConn()
{
  int i = 0;
  OSess->Connected= false;
  if (OLogon->Execute())
  {
    BaseUserName = OSess->LogonUsername.UpperCase();
    Caption="В помощь администратору AMS бухгалтерии ["+
               BaseUserName+"]";
    StatusBar1->Panels->Items[1]->Text = BaseUserName;
    //QObjs     ->Active = true;
    //QConds    ->Active = true;
    //QCondItems->Active = true;
    //FrmAccsStr->SelAccsClick(this);
    //FrmCatTree->SelObjCatClick(this);
    i =1;
  }
  return i;
}
//---------------------------------------------------------------------------
void __fastcall TMainFrm::N5Click(TObject *Sender)
{
// Копирование стандртных проводок
    //FrmCopyStdOp->CopyStdOp(QObjs->FieldByName("OID")->AsInteger,
    //       QObjs->FieldByName("DESCR")->AsString) ;        
}
//---------------------------------------------------------------------------



void __fastcall TMainFrm::N8Click(TObject *Sender)
{
    // Показываем стандартные операции
    TFrmCondsEdit* F;
    Application->CreateForm(__classid(TFrmCondsEdit), &F);
    F->Begin(BaseUserName);
}
//---------------------------------------------------------------------------


void __fastcall TMainFrm::N16Click(TObject *Sender)
{
    // Изменяем базового пользователя
    AnsiString S = BaseUserName;
    if (AskString("Новый базовый пользователь", 30, S))
    {
        BaseUserName= S.UpperCase();
        StatusBar1->Panels->Items[1]->Text = BaseUserName;
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainFrm::FormCreate(TObject *Sender)
{
    OSess->LogonPassword=GC::UserPwd;
    OSess->LogonUsername=BaseUserName;
    OSess->LogonDatabase=GC::ConnStr;
    //OLogon->

    InitConn();
}
//---------------------------------------------------------------------------


void __fastcall TMainFrm::N17Click(TObject *Sender)
{
//
    TFrmBuildBusyProg* F = NULL;
    Application->CreateForm(__classid(TFrmBuildBusyProg), &F);
    F->Show();
}
//---------------------------------------------------------------------------


void __fastcall TMainFrm::N18Click(TObject *Sender)
{
// Генерируем функции для аналитики :
// Выбор поля
    TFrmBuildAnFunc* F = NULL;
    Application->CreateForm(__classid(TFrmBuildAnFunc), &F);
    F->Show();
    F->BuildThis(-1);
        
}
//---------------------------------------------------------------------------


void __fastcall TMainFrm::MenBuildRightsClick(TObject *Sender)
{
    //
    if (AskMessage("Перестроить пользовательский доступ?")==ID_YES)
    {
        OQ1->DeleteVariables();
        OQ2->DeleteVariables();
        OQ2->DeclareVariable("PUSER", otString);
        OQ1->SQL->Text = "select USERNAME from "+BaseUserName+".USER_OPTIONS ";
        OQ2->SQL->Text = "begin  "+BaseUserName+".PRIV_USER (:PUSER); end;";
        ExecSQL(OQ1, "Выбираем пользователей");
        while (!OQ1->Eof)
        {
            OQ2->SetVariable("PUSER", OQ1->FieldAsString(0));
            ExecSQL(OQ2,  "Строим для пользователя доступ");
            OQ1->Session->Commit();
            OQ1->Next();
        };
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainFrm::MenGiveRightsClick(TObject *Sender)
{
    TFrmGiveRights* F = NULL;
    Application->CreateForm(__classid(TFrmGiveRights), &F);
    F->Show();
}
//---------------------------------------------------------------------------


void __fastcall TMainFrm::ActMakeAnOrderExecute(TObject *Sender)
{
//
    TFrmChooseObj* F ;
    Application->CreateForm(__classid(TFrmChooseObj) , &F);
    F->Begin(otAnal);
}
//---------------------------------------------------------------------------

void __fastcall TMainFrm::N20Click(TObject *Sender)
{
    //  Копирование подкатегорий
    TFrmCopyTree* Frm;
    Application->CreateForm(__classid(TFrmCopyTree), &Frm);
    Frm->Init();
}
//---------------------------------------------------------------------------

void __fastcall TMainFrm::Gh1Click(TObject *Sender)
{
    //  Просмотр БД
    TFrmChooseObjADb* Frm;
    Application->CreateForm(__classid(TFrmChooseObjADb), &Frm);
    Frm->BeginDb(otDB);
}
//---------------------------------------------------------------------------

void __fastcall TMainFrm::N21Click(TObject *Sender)
{
    // Просмотр Аналитик
    TFrmChooseObjADb* Frm;
    Application->CreateForm(__classid(TFrmChooseObjADb), &Frm);
    Frm->BeginDb(otAnal);
}
//---------------------------------------------------------------------------

AnsiString BatPath;
AnsiString RestorePath;
AnsiString SavePath;
AnsiString Run;
AnsiString CurrCommandLine;
//---------------------------------------------------------------------------
AnsiString GetVal(const AnsiString& ValName)
{
    AnsiString Res="";
    AmsQuery AQ;
    AQ.SetSqlText("select VALUE from AMS.SYS_INFO where VAL_TYPE=:VAL_TYPE");
    AQ.DeclVar("VAL_TYPE", ValName);
    AQ.Exec("Находим значение глобальных установок");
    if (AQ.NotEof())
    {
        Res=AQ.GetFldStr(0);
    }
    return Res;
};
//---------------------------------------------------------------------------
AnsiString GetDocPath(const AnsiString& UserName)
{
    AnsiString Res="";
    AmsQuery AQ;
    AQ.SetSqlTextReady("select DOC_PATH from AMS.LICENSES L, AMS.BOOKS B "
     " where B.BASE_USER=:BASE_USER and B.ID=L.ID");
    AQ.DeclVar("BASE_USER", UserName);
    AQ.Exec("Находим DOC_PATH");
    if (AQ.NotEof())
    {
        Res=AQ.GetFldStr(0);
    }
    return Res;
};
//---------------------------------------------------------------------------
void SaveUsersAndRoles( const AnsiString& SavePath,
                        const AnsiString& UserName,
                        const AnsiString& NewName)
{
    // Сохраняем роли и пользователей по ролям
    AmsQuery AQ;
    AQ.SetSqlTextReady ("select GRANTED_ROLE from DBA_ROLE_Privs where "
        "GRANTEE=USER and GRANTED_ROLE LIKE USER||'%' and ADMIN_OPTION='YES'");
    AQ.Exec("Выбираем все НАШИ роли");
    AnsiString FName=SavePath+"\\creroles_"+UserName+".sql";
    ofstream F1 (FName.c_str());
    FName=SavePath+"\\updprivs_"+UserName+".sql";
    ofstream F2 (FName.c_str());

    while (AQ.NotEof())
    {
        AnsiString RolePred=AQ.GetFldStr(0);
        AnsiString NewRole="&1."+RolePred.SubString(UserName.Length()+1, RolePred.Length());
        F1<<"create role "<<NewRole.c_str()<<";\ngrant "<<NewRole.c_str()
        <<" to &1. with admin option;\n";
        AnsiString Upd="update OBJPRIV P set P.RUNAME='"+NewRole+
            "' where P.RUNAME='"+
            RolePred+"' and not exists (select 1 from OBJPRIV O "
            "where O.OID=P.OID and O.RUNAME='"+NewRole+"' and O.RU=P.RU);\n";
        F2<<Upd.c_str();
        AQ.Next();
    };
    F1<<"exit;\n";
    F2<<"exit;\n";
    F1.close();
    F2.close();

    AQ.Clear();
    AQ.SetSqlTextReady (
        "select GRANTEE, GRANTED_ROLE from DBA_ROLE_PRIVS "
        "where GRANTED_ROLE in "
            "(select GRANTED_ROLE from DBA_ROLE_Privs where "
            "GRANTEE=USER and GRANTED_ROLE LIKE USER||'%' and "
            "ADMIN_OPTION='YES') and GRANTEE in (select USERNAME from USER_OPTIONS where USERNAME!=USER)");
    AQ.Exec("Выбираем всех пользователей с нашими ролями");
    FName=SavePath+"\\USER_ROLES_"+UserName+".sql";
    ofstream F3 (FName.c_str());
    while (AQ.NotEof())
    {
        AnsiString RolePred=AQ.GetFldStr(1);
        AnsiString NewRole="&1."+RolePred.SubString(UserName.Length()+1, RolePred.Length());
        F3<<"grant "<<NewRole.c_str()<<" to "<<AQ.GetFldStr(0).c_str()<<";\n";
        AQ.Next();
    };
    F3.close();
};

void SaveBuch (const AnsiString& WhatBuch, const AnsiString& WhatBuchPass)
{
    BatPath= GetVal("SavePath");
    SavePath= GetVal("SavePath");
    SaveUsersAndRoles (SavePath, WhatBuch, WhatBuch);
    Run=BatPath+"\\save_buh.bat";
    CurrCommandLine=WhatBuch+" "+WhatBuchPass+" "+MainFrm->OSess->LogonDatabase+" "
        +GetDocPath(BaseUserName);
    ShellExecute(MainFrm->Handle ,	"open",	Run.c_str(), CurrCommandLine.c_str(),
        BatPath.c_str(),SW_SHOW);
}
//---------------------------------------------------------------------------
void __fastcall TMainFrm::ActSaveBuchExecute(TObject *Sender)
{
    SaveBuch (BaseUserName, OSess->LogonPassword);
}
//---------------------------------------------------------------------------
void __fastcall TMainFrm::ActEditGlobalSetupExecute(TObject *Sender)
{
//
    TFrmEditGlobalSetup* Frm;
    Application->CreateForm(__classid(TFrmEditGlobalSetup), &Frm);
    Frm->Show();
}
//---------------------------------------------------------------------------
void __fastcall TMainFrm::ActEditLocalSetsExecute(TObject *Sender)
{
//
    TFrmEditBuchSetup* Frm;
    Application->CreateForm(__classid(TFrmEditBuchSetup), &Frm);
    Frm->Show();
}
//---------------------------------------------------------------------------
AnsiString MakeRestore (bool OtherName, bool AvtRestore=false,
            const AnsiString& FromName=AnsiString(),
            const AnsiString& ToName=AnsiString())
{
    AnsiString RestName=BaseUserName;
    if (AvtRestore)
    {
        RestName=ToName;
    }
    else
    if (OtherName)
    {
        AskString("Как будет называться бухгалтерия", 30, RestName);
        RestName=RestName.UpperCase();
    }

    AnsiString UsedName=BaseUserName;
    if (AvtRestore)
        UsedName=FromName;
    else
        AskString ("Из какой бухгалтерии выполнять восстановление", 30, UsedName);
    UsedName=UsedName.UpperCase();
    Message("Вы выбрали функцию восстановления бухгалтерии из сохраненного файла\r\n"
    "Необходимо понимать, что существующие данные с таким же именем\r\n"
    "Будут уничтожены");
    if (AskMessage("Желаете продолжить восстановление бухгалтерии "+
        RestName+"?", false)== ID_NO) return "";
    RestorePath= GetVal("RestorePath");

    if (!AvtRestore)
        Message("Необходимо поместить в директорий "+RestorePath+" файл с дапмом "+
                UsedName+".rar ");
    if (AskMessage("Желаете продолжить восстановление бухгалтерии "+
        RestName+"?", false)== ID_NO) return "";
    AnsiString MyPass="";
    do {
        MyPass=FrmInputPass->InputPass("Новый пароль для "+RestName);
    }while(FrmInputPass->InputPass("Поторите пароль для "+RestName)!=MyPass);

    AnsiString SysPass= FrmInputPass->InputPass("Пароль для SYS");

    Run=RestorePath+"\\restore_buh.bat";
    CurrCommandLine=RestName+" "+MyPass+" "+SysPass+" "+GetVal("SqlPath")+
        " "+UsedName+" ";

    if (AvtRestore)
        CurrCommandLine+=GetVal("SavePath");

    if (RestName==BaseUserName)
    {
        if (AskMessage("Необходимо разорвать соединение, будет восстановлена текущая бухгалтерия.\r\n"
            "Продолжить?", false)==ID_NO) return "";
        MainFrm->OSess->Connected=false;
    }
    ShellExecute(MainFrm->Handle ,	"open",	Run.c_str(), CurrCommandLine.c_str(),
        RestorePath.c_str(),SW_SHOW);
    return MyPass;
}
//---------------------------------------------------------------------------
void __fastcall TMainFrm::ActRestoreAsExecute(TObject *Sender)
{
    MakeRestore (true);
}
//---------------------------------------------------------------------------
void __fastcall TMainFrm::ActRestoreBuchExecute(TObject *Sender)
{
    MakeRestore(false);
}
//---------------------------------------------------------------------------
void __fastcall TMainFrm::ActClosePeriodExecute(TObject *Sender)
{
//
    TDate Divide=0;
    Message("Выберите дату, начиная с которой будет продолжаться бухгалтерия");
    TFrmSelCpsDate* Frm;
    Application->CreateForm(__classid(TFrmSelCpsDate), &Frm);
    if (Frm->SelCpsDate(Divide))
    {
        Message("В процессе работы программы не трогайте компьютер!!!");
        int MaxSubId=0;
        int BuchId =0;
        int CurrSubId;
        AmsQuery AQ;
        AQ.SetPLTextReady(
        "select max(ID), max(SUB_ID) into :BUCH_ID, :CURR_SUB_ID from "
        "AMS.BOOKS where BASE_USER=:BASE_USER;"
        "select max(SUB_ID) into :MAX_SUB_ID from "+BaseUserName+".BOOKS where ID=:BUCH_ID;"
        "select max(point_date) into :LAST_DATE from CPS; "
        "select min(point_date) into :BEG_DATE from CPS;"
        );
        AQ.DeclVar("BASE_USER", BaseUserName);
        AQ.DeclVar("BUCH_ID", 0);
        AQ.DeclVar("CURR_SUB_ID", 0);
        AQ.DeclVar("MAX_SUB_ID", 0);
        AQ.DeclVarDate("LAST_DATE");
        AQ.DeclVarDate("BEG_DATE");
        AQ.Exec("Находим ID и SUB_ID бухгалтерии");
        TDate DtBeg=AQ.GetVarDate("BEG_DATE");
        TDate DtEnd=AQ.GetVarDate("LAST_DATE");

        if (DtBeg==Divide || DtEnd==Divide)
        {
            ErrorMessage(   "Необходимо выбрать другой месяц для закрытия.\r\n"
                            "Возможно необходимо начать новый месяц");
            return;
        }

        BuchId=AQ.GetVarInt("BUCH_ID");
        CurrSubId=AQ.GetVarInt("CURR_SUB_ID");
        MaxSubId=AQ.GetVarInt("MAX_SUB_ID")+1;
        AnsiString NewBuch=BaseUserName+IntToStr(MaxSubId);
        if (!AskString ("Базовый пользователь для новой бухгалтерии?", 30, NewBuch))
        {
            return;
        }
        ActSaveBuch->Execute();
        while (AskMessage("Закончилось ли сохранение бухгалтерии?")!=ID_YES);
        AnsiString Pass= MakeRestore (true, true, BaseUserName,NewBuch);
        while (AskMessage("Закончилось ли создание бухгалтерии?")!=ID_YES);
        if (BuchId)
        {
            AQ.Clear();
            AQ.SetPLText(
            "insert into AMS.BOOKS "
            "(ID,SUB_ID,BASE_USER,SYS_BASE_USER,STATUS,LAST_UPD,DESCR) values"
            "(:ID,:SUB_ID,:BASE_USER,:BASE_USER,'АРХИВ',SYSDATE,:DESCR);commit;");
            AQ.DeclVarInt("ID", BuchId);
            AQ.DeclVarInt("SUB_ID", MaxSubId);
            AQ.DeclVarStr("BASE_USER", NewBuch);
            AQ.DeclVarStr("DESCR", DateToStr(DtBeg)+" - "+DateToStr(Divide));
            AQ.Exec("Вставка описания");
        }

        BatPath= GetVal("SqlPath");
        Run=BatPath+"\\del_int.bat";
        CurrCommandLine=NewBuch+" "+Pass+" "+
            FormatDateTime("dd-mm-yyyy",DtBeg)+" "+FormatDateTime("dd-mm-yyyy", Divide);
        ShellExecute(this->Handle ,	"open",	Run.c_str(), CurrCommandLine.c_str(),
            BatPath.c_str(),SW_SHOW);
        while (AskMessage("Закончилось ли удаление лишних периодов?")!=ID_YES);
        // Сохраняем то что получилось
        SaveBuch (NewBuch, Pass);
        while (AskMessage("Закончилось ли сохранение бухгалтерии?")!=ID_YES);

        BatPath= GetVal("SqlPath");
        Run=BatPath+"\\del_int.bat";
        CurrCommandLine=BaseUserName+" "+OSess->LogonPassword+" "+
            FormatDateTime("dd-mm-yyyy",Divide)+" "+FormatDateTime("dd-mm-yyyy", DtEnd);
        ShellExecute(this->Handle ,	"open",	Run.c_str(), CurrCommandLine.c_str(),
            BatPath.c_str(),SW_SHOW);
        while (AskMessage("Закончилось ли удаление лишних периодов?")!=ID_YES);
        // Сохраняем то что получилось
        ActSaveBuch->Execute();
    }
    Frm->Release();
}
//---------------------------------------------------------------------------



