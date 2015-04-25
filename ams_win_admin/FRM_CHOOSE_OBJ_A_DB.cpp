//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_CHOOSE_OBJ_A_DB.h"
#include "AMS_ADM_UTILS.h"
#include "FRM_ACCS_TIES.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AmsStringGrid"
#pragma link "FRM_CHOOSE_OBJ_ADM"
#pragma link "Oracle"

#pragma resource "*.dfm"
TFrmChooseObjADb *FrmChooseObjADb;
//---------------------------------------------------------------------------
__fastcall TFrmChooseObjADb::TFrmChooseObjADb(TComponent* Owner)
    : TFrmChooseObj(Owner)
{
}
//---------------------------------------------------------------------------
void TFrmChooseObjADb::BeginDb(int OType)
{
    Begin(OType);
}
//---------------------------------------------------------------------------
void __fastcall TFrmChooseObjADb::ActCopyDataExecute(TObject *Sender)
{
    //
    int OId = StrToInt(SG->Cells[0][SG->Row]);
    AnsiString OName = SG->Cells[1][SG->Row];



    AnsiString ToName=LastCopyWhere;
    if (!AskString("В какую бухгалтерию вносить изменения?", 30, ToName))
    {
        return;
    }
    LastCopyWhere=ToName;
    // Есть ли такая БД в другой бухгалтерии
    AmsQuery AQ;
    AQ.SetSqlTextReady("select OID from "+ToName+".OBJS where NAME=:NAME and OTYPE=:OTYPE");
    AQ.DeclVar("NAME", OName);
    AQ.DeclVar("OTYPE", otDB);
    AQ.Exec("Выбираем код БД куда копировать данные");
    int NewOId=0;
    if (AQ.NotEof())
    {
        NewOId=AQ.GetFldInt(0);
    }
    else
    {
        return;
    }

    OrSess->Connected=false;
    OrLogon->Caption="Пароль для бухгалтерии "+ToName;
    OrSess->LogonUsername=ToName;
    OrSess->LogonDatabase=MainFrm->OSess->LogonDatabase;
    OrLogon->Execute();
    if (OrSess->Connected)
    {
        if (MRes->Height<10)
        {
            MRes->Height=25;
        }
        MRes->Lines->Text="";
        OrSess->Connected=true;
        OSc->Lines->Text="grant insert on "+ToName+"."+OName+" to "+BaseUserName;
        OSc->Execute();
        MRes->Lines->Text=OSc->Output->Text;
    }
    else
    {
        return;
    }



    // Проверку на идентичность структур не делаем


    AnsiString Str;
    std::map<int, int> Cats;
    // Строим структуру категорий
    AQ.Clear();
    // Запрос все категории из старой БД
    AQ.SetSqlTextReady(
         //             0     1       2      3
         "select LEVEL LEV, DESCR, OWNCAT, CATID "
         "from "+BaseUserName+".DBCAT "
         "start with DBID=:DBID and CATID is NULL "
         "connect by DBID=:DBID and prior OWNCAT=CATID "
         "order by LEVEL, ORD");
    AQ.DeclVar("DBID", OId);
    AQ.Exec("Выбираем категории из базы-эталона");
    AmsQuery A1;
    // Запрос Категории в существующей таблице
    A1.SetSqlTextReady (
        "select OWNCAT "
        "from "+ToName+".DBCAT "
        "where LEVEL=:LEV and DESCR=:DESCR "
        "start with DBID=:DBID and CATID is NULL "
        "CONNECT by DBID=:DBID and PRIOR OWNCAT=CATID ");
    A1.DeclVar("DBID", NewOId);
    A1.DeclVar("LEV", 0);
    A1.DeclVar("DESCR", "");

    int CatsIns=0;
    int PosIns=0;
    while (AQ.NotEof())
    {
        int CurrCat=AQ.GetFldInt(2);
        AnsiString Descr= AQ.GetFldStr(1);

        A1.SetVar("LEV", AQ.GetFldInt(0));
        A1.SetVar("DESCR", AQ.GetFldStr(1));
        A1.Exec("Выбираем категорию базы-зеркала");
        if (A1.NotEof())
        {
            Cats[AQ.GetFldInt(2)]=A1.GetFldInt(0);
        }
        else
        {
            // Вставляем новую категорию в базу-зеркало
            AmsQuery A2;
            CatsIns++;
            A2.SetSqlTextReady(
                "begin "+ToName+".InsertDBCat(:DBID,:CATID,:DESCR, :KEY, :RES);"
                "commit;end;");
            A2.DeclVar("DBID", NewOId);
            A2.DeclVar("CATID", Cats[AQ.GetFldInt(3)]);
            A2.DeclVarInt("KEY");
            A2.DeclVar("DESCR", AQ.GetFldStr(1));
            A2.DeclVarInt("RES");
            A2.Exec("Вставляем новую категорию в базу-зеркало");
            Cats[AQ.GetFldInt(2)]=A2.GetVarInt("KEY");
        }
        AQ.Next();
    }
    AQ.Clear();
    AQ.SetSqlTextReady("select REALNAME from "+BaseUserName+".DBSTR "
        "where DBID=:DBID and DELETED=0 order by ORD");
    AnsiString Str1="CATID, ARTICLE, VER";
    AnsiString Str2=":CATID, ARTICLE, VER";
    AQ.DeclVar("DBID", OId);
    AQ.Exec("Выбор полей БД");
    while (AQ.NotEof())
    {
        AnsiString RN = AQ.GetFldStr(0).UpperCase();
        if (RN!="CATID" && RN!="ARTICLE" && RN!="VER")
        {
            Str1+=", "+RN;
            Str2+=", "+RN;
        }
        AQ.Next();
    }

    AnsiString InsertStr="insert into "+ToName+"."+OName+"("+Str1+") "
        "( select "+Str2+" from "+BaseUserName+"."+OName+" where "
        "ARTICLE=:ARTICLE and VER=:VER)";
    AQ.Clear();
    AQ.SetSqlTextReady(InsertStr);
    AQ.DeclVar("ARTICLE","");
    AQ.DeclVar("VER",0);
    AQ.DeclVar("CATID",0);
    A1.Clear();
    A1.SetSqlText(
        "select * from "
        "   (select DB.ARTICLE ART, D.ARTICLE, D.VER, D.CATID "
            "from "+BaseUserName+"."+OName+" D, "+ToName+"."+OName+" DB "
            "where "
                "D.ARTICLE = DB.ARTICLE (+) and "
                "D.VER=DB.VER (+)) S "
            "where ART is null");
    A1.Exec("Выбираем новые позиции базы данных");
    while (A1.NotEof())
    {
        AQ.DeclVar("ARTICLE", A1.GetFldStr(1));
        AQ.DeclVar("VER",A1.GetFldInt(2));
        AQ.DeclVar("CATID", Cats[A1.GetFldInt(3)]);
        AQ.Exec("Вставка новой позиции в базу-зеркало");
        PosIns++;
        A1.Next();
    }
    AQ.Clear();
    AQ.SetSqlTextReady("commit");
    AQ.Exec("");

    AnsiString Mess;
    if (!PosIns && !CatsIns)
        Mess="Нет новых позиций";
    else
    {
        if (CatsIns)
            Mess="Всталено "+IntToStr(CatsIns)+" категорий\r\n";
        if (PosIns)
            Mess="Всталено "+IntToStr(PosIns)+" позиций\r\n";
    }

    MRes->Lines->Text="";
    OSc->Lines->Text="revoke insert on "+ToName+"."+OName+" from "+BaseUserName;
    OSc->Execute();
    MRes->Lines->Text=OSc->Output->Text;
    OrSess->Connected=false;

    Message(Mess);

}
//---------------------------------------------------------------------------
void __fastcall TFrmChooseObjADb::ActUpdStructExecute(TObject *Sender)
{
    //
    if (SG->Cells[0][SG->Row]=="")
    {
        return;
    }
    int OId = StrToInt(SG->Cells[0][SG->Row]);
    AnsiString OName = SG->Cells[1][SG->Row];

    AnsiString ToName=LastCopyWhere;
    if (!AskString("В какую бухгалтерию вносить изменения?", 30, ToName))
    {
        return;
    }
    LastCopyWhere=ToName;

    // Сначала добиваемся чтобы все поля были одинаковыми
    AmsQuery AQ;
    AQ.SetSqlTextReady("select OID from "+ToName+".OBJS where NAME=:NAME");
    AQ.DeclVar("NAME", OName);
    AQ.Exec("Получаем код объекта в новой бухгалтерии");
    if (!AQ.NotEof())
    {
        ErrorMessage("Объект "+OName+" не существует в бухгалтерии "+ToName);
        return;
    }
    int NewOId = AQ.GetFldInt(0);
    AQ.Clear();
    AQ.SetSqlTextReady(
    //             0         1         2         3          4
    "select D1.REALNAME, D1.VTYPE, D1.MAXLEN, D3.VTYPE, D3.MAXLEN from "+BaseUserName+".DBSTR D1, "+
        ToName+".DBSTR D3 "
    "where "
	    "D1.DBID=:DBID and D1.DELETED=0 and D3.DBID(+)=:NEW_DBID and D3.REALNAME(+)=D1.REALNAME and "
	    "not exists ( select 1 from "+ToName+".DBSTR D2 "
			"where "
				"D2.DBID = :NEW_DBID and "
				"D2.REALNAME = D1.REALNAME and "
				"D2.VTYPE=D1.VTYPE and "
				"nvl(D2.MAXLEN,-1) = nvl(D1.MAXLEN,-1))");

    AQ.DeclVar("DBID", OId);
    AQ.DeclVar("NEW_DBID", NewOId);

    AQ.Exec ("Поля с различиями");
    AnsiString AddStr="";
    AnsiString ModifyStr="";
    AnsiString DBNew="";
    while (AQ.NotEof())
    {
        if (AQ.IsFldNull(3))
        {
            if (AddStr!="") AddStr+=", ";
            AddStr+=AQ.GetFldStr(0)+" "+
                GetOraType(AQ.GetFldInt(1), AQ.GetFldInt(2));
            DBNew+="insert into "+ToName+".DBSTR "
                "(DBID, REALNAME,VTYPE,MAXLEN) values ("+
             IntToStr(NewOId)+", '"+
                   AQ.GetFldStr(0)+"',"+
                   IntToStr(AQ.GetFldInt(1))+","+IntToStr(AQ.GetFldInt(2))+");\r\n";
        }
        else
        {
            // Тип не изменился
            if (AQ.GetFldInt(1) == AQ.GetFldInt(3))
            {
                if (AQ.GetFldInt(2)>AQ.GetFldInt(4))
                { // Увеличилось количество знаков
                    if (ModifyStr!="") ModifyStr+=", ";
                    ModifyStr+=AQ.GetFldStr(0)+" "+
                        GetOraType(AQ.GetFldInt(1), AQ.GetFldInt(2));
                }

                DBNew+="update "+ToName+".DBSTR "
                "set MAXLEN="+IntToStr(AQ.GetFldInt(2))+
                " where DBID="+IntToStr(NewOId)+
                " and REALNAME='"+ AQ.GetFldStr(0)+"';\r\n";
            }
            else
            {
                // Тип изменился
                if (ModifyStr!="") ModifyStr+=", ";
                ModifyStr+=AQ.GetFldStr(0)+" "+
                        GetOraType(AQ.GetFldInt(1), AQ.GetFldInt(2));

                DBNew+="update "+ToName+".DBSTR "
                "set VTYPE="+IntToStr(AQ.GetFldInt(1))+
                ", MAXLEN="+IntToStr(AQ.GetFldInt(2))+
                " where DBID="+IntToStr(NewOId)+
                " and REALNAME='"+ AQ.GetFldStr(0)+"';\r\n";
            }
        }
        AQ.Next();
    }

    //
    AnsiString ScriptText;
    AQ.Clear();
    if ( AddStr != ""  || ModifyStr !="")
    {
        ScriptText="alter table "+ToName+"."+OName;
        if (AddStr!="")
        {
            ScriptText+=" add ("+AddStr+")";
        }
        if (ModifyStr!="")
        {
            ScriptText+=" modify("+ModifyStr+")";
        }
        ScriptText+="\r\n/\r\n";
    };

    //  Копируем установки
    AQ.Clear();
    AQ.SetSqlTextReady(
        //        0
        "select SETNAME from "+BaseUserName+".DBOPT D1 "
        "where DBID=:DBID and "
        "not exists (select 1 from "+ ToName+".DBOPT D2 "
                        "where D2.DBID=:NEW_DBID and D1.SETNAME=D2.SETNAME)");
    AQ.DeclVar("DBID", OId);
    AQ.DeclVar("NEW_DBID", NewOId);
    AQ.Exec("Выбираем установки которых еще нет в базе");
    while (AQ.NotEof())
    {
        DBNew+="insert into "+ToName+".DBOPT (DBID, SETNAME) values "
            " ("+IntToStr(NewOId)+", '"+AQ.GetFldStr(0)+"');\r\n";
        AQ.Next();
    }

    // Установки к полям от набора установок
    AQ.Clear();
    AQ.SetSqlTextReady(
        //        0            1
        "select D1.SETNAME, D1.REALNAME from "+BaseUserName+".DBOPTOV D1 "
        "where DBID=:DBID and "
        "not exists (select 1 from "+ ToName+".DBOPTOV D2 "
                        "where D1.DBID=:NEW_DBID and "
                        "D1.SETNAME=D2.SETNAME and "
                        " D1.REALNAME= D2.REALNAME)");
    AQ.DeclVar("DBID", OId);
    AQ.DeclVar("NEW_DBID", NewOId);
    AQ.Exec("Выбираем наборы для которых еще нет в базе");
    while (AQ.NotEof())
    {
        DBNew+="insert into "+ToName+".DBOPTOV "
        "(DBID, SETNAME, REALNAME) values ("+IntToStr(NewOId)+",'"+
            AQ.GetFldStr(0)+"','"+AQ.GetFldStr(1)+"');\r\n";
        AQ.Next();
    }
    //
    // Теперь обновления
    AQ.Clear();
    AQ.SetSqlTextReady(
        //        0         1        2        3     4   5     6           7
        "select REALNAME, ALIAS, DISP_OPT, WIDTH, DEC, ORD, DEF_OPT, DB_SETNAME "
        "from "+BaseUserName+".DBSTR where DBID=:DBID and DELETED=0");
    AQ.DeclVar("DBID", OId);
    AQ.Exec("Хотим обновление установок полей");
    while (AQ.NotEof())
    {
        DBNew+="update "+ToName+".DBSTR set "
            "ALIAS='"+AQ.GetFldStr(1)+"', "
            "DISP_OPT="+IntToStr(AQ.GetFldInt(2))+", "
            "WIDTH="+IntToStr(AQ.GetFldInt(3))+", "
            "DEC="+IntToStr(AQ.GetFldInt(4))+", "
            "ORD="+IntToStr(AQ.GetFldInt(5))+", "
            "DEF_OPT="+IntToStr(AQ.GetFldInt(6))+", "
            "DB_SETNAME='"+AQ.GetFldStr(7)+"' "
            "where DBID="+IntToStr(NewOId)+" and REALNAME='"+AQ.GetFldStr(0)+"';\r\n";

        AQ.Next();
    }
    // Обновление установок к полям в других наборах установок
    AQ.Clear();
    AQ.SetSqlTextReady(
        //        0         1        2        3
        "select SETNAME, REALNAME, OPT,  DB_SETNAME "
        "from "+BaseUserName+".DBOPTOV where DBID=:DBID ");
    AQ.DeclVar("DBID", OId);
    AQ.Exec("Хотим обновление установок полей для разл наборов установок");
    while (AQ.NotEof())
    {
        DBNew+="update "+ToName+".DBOPTOV set "
            "OPT="+IntToStr(AQ.GetFldInt(2))+", "
            "DB_SETNAME='"+AQ.GetFldStr(3)+"' "
            "where DBID="+IntToStr(NewOId)+" and "
            "SETNAME='"+AQ.GetFldStr(0)+"' and "
            "REALNAME='"+AQ.GetFldStr(1)+"';\r\n";

        AQ.Next();
    }

    // Теперь обновления доп полей
    AQ.Clear();
    TFrmAccsTies::TieAccsAn(BaseUserName, ToName, OId, AQ());

    AQ.Clear();
    AQ.SetSqlText(
        //        0            1          2      3         4           5
        "select I.SETNAME, I.REALNAME, I.NAME, A.CODE, I.TURNTYPE, I.ALIAS "
        "from *.DBADDINFO I, *.ACCS A1, *.ACCS_TIES AT, "+ToName+".ACCS A "
        "where DBID=:DBID and A1.CODE=I.CODE and AT.ACC_NAME=A1.NAME and "
        " A.NAME=ACC_NAME2");
    AQ.DeclVar("DBID", OId);
    AQ.Exec("Хотим обновление дополнительных полей");

    while (AQ.NotEof())
    {
        DBNew+="update "+ToName+".DBADDINFO set "
            "CODE="+IntToStr(AQ.GetFldInt(3))+", "
            "TURNTYPE='"+AQ.GetFldStr(4)+"', "
            "ALIAS='"+AQ.GetFldStr(5)+"' "
            "where DBID="+IntToStr(NewOId)+" and "
            "SETNAME='"+AQ.GetFldStr(0)+"' and "
            "REALNAME='"+AQ.GetFldStr(1)+"' and "
            "NAME='"+AQ.GetFldStr(2)+"';\r\n";

        AQ.Next();
    }


    if (DBNew!="")
    {
        ScriptText+=DBNew+"\r\ncommit;\r\n";
        DBNew="";
    }
    //


    if (ScriptText != "")
    {
        OrSess->Connected=false;
        OrLogon->Caption="Пароль для бухгалтерии "+ToName;
        OrSess->LogonUsername=ToName;
        OrSess->LogonDatabase=MainFrm->OSess->LogonDatabase;
        if (OrLogon->Execute())
        {
            if (MRes->Height<10)
            {
                MRes->Height=25;
            }
            MRes->Lines->Text="";
            OrSess->Connected=true;
            OSc->Lines->Text=ScriptText;
            OSc->Execute();
            MRes->Lines->Text=OSc->Output->Text;
            OrSess->Connected=false;
        }
    }

    //
}
//---------------------------------------------------------------------------
void __fastcall TFrmChooseObjADb::FormDestroy(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TFrmChooseObjADb::FormClose(TObject *Sender,
      TCloseAction &Action)
{
//
    Action = caFree;
}
//---------------------------------------------------------------------------


void __fastcall TFrmChooseObjADb::OScAfterCommand(TOracleScript *Sender,
      bool &Handled)
{
    //
}
//---------------------------------------------------------------------------

