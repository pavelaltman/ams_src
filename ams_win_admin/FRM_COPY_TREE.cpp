//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "FRM_COPY_TREE.h"
#include "Frm_Cat_Tree.h"
#include "MAIN_FRM.h"
#include "FRM_ACCS_TIES.h"
#include "AMS_ADM_UTILS.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AMS_TREE_VIEW"
#pragma link "AmsObjCatView"
#pragma link "Oracle"
#pragma resource "*.dfm"
TFrmCopyTree *FrmCopyTree;
//---------------------------------------------------------------------------
__fastcall TFrmCopyTree::TFrmCopyTree(TComponent* Owner)
    : TForm(Owner), CatTree(0)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmCopyTree::FromCatNewEvent(TFrmCatTree* Sender, int NewCat)
{
/*
    bool Continue = false;
    if (AskMessage("Копировать категорию "+CatTree->GetSelected()+"?")==ID_YES)
    {
        FromCatId=NewCat;
        Progress->Lines->Add("Копируем категорию "+CatTree->GetSelected()+" ["+IntToStr(NewCat)+"]");
        AnsiString S= LastCopyWhere;
        if (AskString("В какую бухгалтерию копировать?", 30, S))
        {
            LastCopyWhere= S.UpperCase();
            ToName=LastCopyWhere;
            Progress->Lines->Add("в бухгалтерию "+ToName);
            CatTree->ShowCategories(ToName, -1, ToCatNewEvent);
            Continue=true;
        }
    }

    if (!Continue)
    {
        Close();
    }
*/
};
//---------------------------------------------------------------------------
void __fastcall TFrmCopyTree::ToCatNewEvent(TFrmCatTree* Sender, int NewCat)
{
/*
    bool Continue = false;
    if (AskMessage("Копировать в категорию "+CatTree->GetSelected()+"?")==ID_YES)
    {
        Progress->Lines->Add("в категорию "+CatTree->GetSelected()+" ["+IntToStr(NewCat)+"]");
        ToCatId=NewCat;
        BBApply->Visible= true;
        BBApply->Enabled= true;
        Continue=true;
    }
    if (!Continue)
    {
        Close();
    }
*/
};
//---------------------------------------------------------------------------
void TFrmCopyTree::Init()
{
    PC->ActivePage=TS1;
    PCChange(this);
    ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TFrmCopyTree::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    /*
    if (CatTree)
    {
        CatTree->Release();
    }
    */
    Action=caFree;

}
//---------------------------------------------------------------------------
int InsObj (const AnsiString& ToName, int ToCatId, int OType,
            const AnsiString& OName, const AnsiString& Descr)
{
    int Res=0;

    AmsQuery AQ;
    AQ.SetSqlTextReady(
        "begin "
        " declare vEx number (1);"
        "begin "
        " select max(1) into vEx from "+ToName+".OBJS where NAME=:NAME;"
        " if vEx is null then "+
        ToName+".InsertObj(:CID, :TYP , :NAME, :DES, :KEY, :RES);commit;"
        "end if;"
        "end;"
        "end;");
    AQ.DeclVar("NAME", OName);
    AQ.DeclVar("CID", ToCatId);
    AQ.DeclVar("TYP", OType);
    AQ.DeclVar("DES", Descr);
    AQ.DeclVarInt("KEY");
    AQ.DeclVarInt("RES");
    AQ.Exec("Вставка объекта в другую бухгалтерию");
    Res=AQ.GetVarInt("KEY");
    return Res;
}
//---------------------------------------------------------------------------
int ConstCopyTo (const AnsiString& ToName, int ToCatId, int ObjId,
                 const AnsiString& OName, const AnsiString& Descr)
{
    int NewObjId= InsObj (ToName, ToCatId, otConst, OName, Descr);
    if (NewObjId)
    {
        AmsQuery AQ;
        AQ.SetSqlTextReady(
            "begin "
            " insert into "+ToName+".CONSTS (OID, CONST_TYPE, VALUE) "
            " (select :NEW_OID, CONST_TYPE, VALUE from "+BaseUserName+".CONSTS"
            " where OID=:OID);"
            "commit;end;");
        AQ.DeclVar("NEW_OID", NewObjId);
        AQ.DeclVar("OID", ObjId);
        AQ.Exec("Константу копируем");
    }
    return NewObjId;
}
//---------------------------------------------------------------------------
int DbCopyTo (const AnsiString& ToName, int ToCatId, int ObjId, int OType,
                 const AnsiString& OName, const AnsiString& Descr,
                 const AnsiString& ParentName, TFrmCopyTree* F)
{
    int NewObjId= InsObj (ToName, ToCatId, OType, OName, Descr);
    if (NewObjId)
    {
        AmsQuery AQ;
        TFrmAccsTies::TieAccsAn(BaseUserName, ToName, ObjId,AQ());
        AQ.Clear();
        AQ.SetSqlTextReady(
          "begin "
          "declare"
          "    vFromId number(8):= :OID;"
          "    vToId number(8):= :NEW_OID;"
          "    cursor S is"
          "                    select * from "+BaseUserName+".DBSTR"
          "                    where DBID=vFromId and DELETED=0;"
          "    vId number(8);"
          "begin"
          "    insert into "+ToName+".DBOPT"
          "        (DBID, SETNAME)"
          "    (select vToId, SETNAME from "+BaseUserName+".DBOPT where DBID=vFromId);"
          "    insert into "+ToName+".DBOPTOV"
          "        (DBID, SETNAME, REALNAME, OPT, DB_SETNAME)"
          "    (select vToId, SETNAME, REALNAME, OPT, DB_SETNAME"
          "     from "+BaseUserName+".DBOPTOV"
          "     where DBID=vFromId);"
          "    for X in S loop"
          "        vId:=NULL;"
          "        insert into "+ToName+".DBSTR"
          "            (DBID,     REALNAME,  VTYPE,  RDBID, MAXLEN,   DELETED,     ALIAS,"
          "             DISP_OPT, WIDTH,     DEC,    ORD,   DEF_OPT,  DB_SETNAME,  LINES)"
          "        values"
          "            (vToId,    X.REALNAME,X.VTYPE,vId,   X.MAXLEN, X.DELETED,   X.ALIAS,"
          "            X.DISP_OPT,X.WIDTH,   X.DEC,  X.ORD, X.DEF_OPT,X.DB_SETNAME,X.LINES);"
          "    end loop;"
          "    insert into "+ToName+".DBADDINFO"
          "        (   DBID,  SETNAME,  REALNAME,   NAME,   CODE,   TURNTYPE,   ALIAS)"
          "    (select vToId,I.SETNAME, I.REALNAME, I.NAME, A.CODE, I.TURNTYPE, I.ALIAS"
          "     from "+BaseUserName+".DBADDINFO I, "+BaseUserName+".ACCS C, "+ToName+".ACCS A"
          "     where"
          "        I.DBID=vFromId and"
          "        C.CODE=I.CODE and"
          "        A.NAME     = ( select ACC_NAME2"
          "                      from "+BaseUserName+".ACCS_TIES T where"
          "                      T.BASEUSERNAME='"+ToName+"' and"
          "                      T.ACC_NAME=C.NAME));"
          "end;"
          "commit;"
          "end;");

        AQ.DeclVar("NEW_OID", NewObjId);
        AQ.DeclVar("OID", ObjId);
        AQ.Exec("Копируем структуру базы данных");



        // Определяем структуру для Oracle таблицы
        AQ.Clear();
        AQ.SetSqlTextReady (
            "select REALNAME, VTYPE, MAXLEN  from "+BaseUserName+".DBSTR "
            "where DBID=:DBID and DELETED=0 order by ORD");
        AQ.DeclVar("DBID", ObjId);
        AQ.Exec("Выбираем поля базы данных/аналитики");
        AnsiString S="create table "+ToName+"."+OName+ "(";
        bool FirstTime = true;
        int i =0;
        while (AQ.NotEof())
        {
            if (FirstTime)
                FirstTime= false;
            S+=AQ.GetFldStr(0)+" "+
                GetOraType (AQ.GetFldInt(1), AQ.GetFldInt(2))+", ";
            i++;
            if (i==3 && OType!=otDB)
            {
                S+="op_date date NULL,";
            }
            AQ.Next();
        }
        if (!FirstTime)
        {
            if (OType==otDB)
            {
                S+= "CONSTRAINT PK_"+OName+" PRIMARY KEY (ARTICLE, VER), "
                    "CONSTRAINT FK_CAT"+OName+" FOREIGN KEY (CATID) "
                        "REFERENCES "+ToName+".DBCAT (OWNCAT))";
            }
            else
            {
                AQ.Clear();
                AQ.SetSqlTextReady(
                    "select NAME from "+BaseUserName+".OBJS "
                    "where OID=(select PARENT from "+BaseUserName+".OBJS where OID=:OID)");
                AQ.DeclVar("OID", ObjId);
                AQ.Exec("Имя базы данных");
                AnsiString DbName="";
                if(AQ.NotEof())
                {
                    DbName=AQ.GetFldStr(0);
                }
                else
                {
                    ErrorMessage("Не могу найти родительскую базу для аналитики "+OName);
                    return 0;
                }
                S+="CONSTRAINT PK_AN_"+OName+" PRIMARY KEY (SUMID, AARTICLE, AVER), "
                    "CONSTRAINT FK_AN_"+OName+"_SUMS FOREIGN KEY (SUMID) "
                        "REFERENCES "+ToName+".SUMS (SUM_ID) ON DELETE CASCADE, "
                    "CONSTRAINT FK_DBAN_"+OName+" FOREIGN KEY (AARTICLE, AVER) "
                        "REFERENCES "+ToName+"."+ParentName+" (ARTICLE, VER))";
            }
            // делаем базу от имени пользователя куда копируем
            if (!F->OSess->Connected)
            {
                F->OLogon->Caption="Пароль для базы "+ToName;
                F->OSess->LogonDatabase = MainFrm->OSess->LogonDatabase;
                F->OSess->LogonUsername=ToName;
                F->OLogon->Execute();
                if (!F->OSess->Connected)
                    return 0;
            }

            do {
                AmsQuery A1(F->OterBaseQ);
                A1.Clear();
                A1.SetSqlTextReady(S+" ");
                //A1.Exec("Создаем БД/Аналитику");
                A1.ExecSilent();
                A1.SetSqlTextReady("grant insert on "+OName+" to "+ToName+"SUPER");
                A1.Exec("Право вставки выдаем");
            } while(0);

            // Для базы данных создаем начальную категорию а также вставляем
            // ИТОГО- Article+Ver делаем дерево категорий
            if (OType==otDB)
            {
                AQ.Clear();
                AQ.SetSqlTextReady(
                    "begin "+ToName+
                      ".InsertDBCat(:DID,:CID,:DES,:KEY,:RES);"
                      "insert into "+ToName+"."+OName+
                      "(ARTICLE,VER) values (' ',0);"+
                      ToName+".MakeDBTree(:DID);"
                      "commit;"
                    "end;");
                AQ.DeclVarInt("RES");
                AQ.DeclVarInt("KEY");
                AQ.DeclVarInt("CID");
                AQ.DeclVar("DES","/");
                AQ.DeclVar("DID",NewObjId);
                AQ.Exec("Для базы данных создаем начальную категорию");
            }
        }
        else
        {
            return 0;
        }
    }
    return NewObjId;
}
//---------------------------------------------------------------------------
int DbCopyToAdd(const AnsiString& ToName, int ObjId, int OType,
                 const AnsiString& OName, int Parent, TFrmCopyTree* F)
{
    // Для аналитик необходимо править парента
    AmsQuery AQ;
    if (OType==otAnal)
    {
        AQ.SetSqlTextReady(
        "begin "
        "update "+ToName+".OBJS set PARENT="
            "(select max(OID) from "+ToName+".OBJS where NAME="
                "(select NAME from "+BaseUserName+".OBJS where OID=:OID)"
            ") "
        "where NAME=:NAME;"
        "commit;end;");
        AQ.DeclVar("OID" , Parent);
        AQ.DeclVar("NAME", OName );
        AQ.Exec("Правим парента для аналитк");
        AQ.Clear();
    };

    // Для всех поля-ссылки
    AQ.SetSqlText(
    //          0         1           2                          3                4
    "select D.REALNAME, D.VTYPE, O.NAME TIED_DB_NAME, O1.OID NEW_TIED_DB_ID, O2.OID NEW_OID "
    "from *.DBSTR D, *.OBJS O, "+ToName+".OBJS O1, "+ToName+".OBJS O2 "
    "where "
        "DBID=:DBID and RDBID is not NULL and "
        "O.OID=D.RDBID and "
        "O1.NAME=O.NAME and "
        "O2.NAME=:NAME");
    AQ.DeclVar("DBID",ObjId);
    AQ.DeclVar("NAME",OName);
    AQ.Exec("Выбираем все поля-ссылки");
    AnsiString S="";
    AnsiString G="";
    int NewOId =0;
    while (AQ.NotEof())
    {
        if (AQ.GetFldInt(1)==5) // ссылка на БД
        {

            // от имени пользователя куда копируем
            if (!F->OSess->Connected)
            {
                F->OLogon->Caption="Пароль для базы "+ToName;
                F->OSess->LogonDatabase = MainFrm->OSess->LogonDatabase;
                F->OSess->LogonUsername=ToName;
                F->OLogon->Execute();
                if (!F->OSess->Connected)
                    return 0;
            }

            AmsQuery A1(F->OterBaseQ);
            A1.Clear();
            A1.SetSqlTextReady(
            "ALTER TABLE "+ToName+"."+OName+" ADD( CONSTRAINT FK_"+OName+"_"+
            AQ.GetFldStr(0)+" FOREIGN KEY ("+AQ.GetFldStr(0)+","+AQ.GetFldStr(0)+"_V) "
            " REFERENCES "+ToName+"."+AQ.GetFldStr(2)+"(ARTICLE, VER))");
            //A1.Exec("Делаем ссылки на БД");
            A1.ExecSilent();
        }
        G+="update "+ToName+".DBSTR set RDBID="+IntToStr(AQ.GetFldInt(3))+
        " where DBID=:DBID and REALNAME='"+AQ.GetFldStr(0)+"'; ";
        NewOId=AQ.GetFldInt(4);
        AQ.Next();
    }



    if (G!="")
    {
        // от имени пользователя куда копируем
        if (!F->OSess->Connected)
        {
            F->OLogon->Caption="Пароль для базы "+ToName;
            F->OSess->LogonDatabase = MainFrm->OSess->LogonDatabase;
            F->OSess->LogonUsername=ToName;
            F->OLogon->Execute();
            if (!F->OSess->Connected)
                return 0;
        }

        do {
            AmsQuery A1(F->OterBaseQ);
            A1.Clear();
            A1.DeclVar("DBID", NewOId);
            A1.SetPLText(G+"commit;");
            A1.Exec("Ставим всем полям-ссылкам признак ссылки");
        } while(0);
    }
    return 1;
}
//---------------------------------------------------------------------------
int DocCopyTo (const AnsiString& ToName, int ToCatId, int ObjId, int OType,
                 const AnsiString& OName, const AnsiString& Descr)
{
    int NewObjId= InsObj (ToName, ToCatId, OType, OName, Descr);
    if (NewObjId)
    {
        AmsQuery AQ;
        AQ.SetSqlTextReady(
            "begin "
            " update "+ToName+".OBJS SET (DUP,AUTON,POPTS,RANGE)="
            "(select DUP,AUTON,POPTS,RANGE from "+BaseUserName+".OBJS where OID=:OID)"
            " where OID=:NEW_OID; commit; end;");
        AQ.DeclVar("NEW_OID", NewObjId);
        AQ.DeclVar("OID", ObjId);
        AQ.ExecSilent();
    }
    return NewObjId;
}

//---------------------------------------------------------------------------
void __fastcall TFrmCopyTree::BBApplyClick(TObject *Sender)
{
    // Тут и будем копировать
    AmsQuery AQ;
    AQ.SetSqlText(
        "select OWNCAT, CATID, DESCR, LEVEL, TORD from *.CATTREE start with OWNCAT=:OWNCAT "
        "connect by prior OWNCAT=CATID order by LEVEL, TORD");
    AQ.DeclVar("OWNCAT", FromCatId);
    AQ.Exec("Выбираем категории");
    // По категории получаем ее родит категорию в другой бухгалтерии
    std::map<int,int> ParentCat;
    // По категории получаем категорию в другой бухгалтерии
    std::map<int,int> Cat;
    AmsQuery A1;
    A1.SetSqlTextReady (
        "begin "
        " declare vTmp number;"
        "begin "
        "   if :DES='/' then"
        "       :DES:='--';"
        "   end if;"
        "   select max(OWNCAT) into vTmp from "+ToName+".OBJCAT "
        "   where CATID=:CID and DESCR=:DES; "
        "   if vTmp is NULL then "+
                ToName+".InsertCat(:CID, :DES , :KEY, :RES);"
        "   else "
        "       :KEY:=vTmp;"
        "   end if;"
        "end;"
        "commit; end;");
    A1.DeclVarInt("CID");
    A1.DeclVarStr("DES");
    A1.DeclVarInt("KEY");
    A1.DeclVarInt("RES");
    // Копируем дерево подкатегорий
    Progress->Lines->Add(" ");
    Progress->Lines->Add(" ");
    Progress->Lines->Add("Строим категории ");
    while (AQ.NotEof())
    {
        int OWNCAT = AQ.GetFldInt(0);
        int CATID = AQ.GetFldInt(1);

        int Parent =0;
        if (OWNCAT==FromCatId)
        {
            ParentCat[OWNCAT]=ToCatId;
            Parent=ToCatId;
        }
        else
        {
            Parent=Cat[CATID];
            ParentCat[OWNCAT]=Parent;
        }

        A1.ClearVars();
        A1.SetVar("CID", Parent);
        A1.SetVar("DES", AQ.GetFldStr(2));
        A1.Exec("Вставляем новую категорию в другую бухгалтерию");

        int CurrCat= A1.GetVarInt("KEY");
        Cat[OWNCAT]=CurrCat;
        Progress->Lines->Add("____Категория "+AQ.GetFldStr(2)+" вставлена ["+
                IntToStr(OWNCAT)+"->"+IntToStr(CurrCat)+"] ");
        this->Refresh();
        AQ.Next();
    }

    // Копируем базы данных и аналитики из категорий
    AQ.Clear();
    A1.Clear();
    AQ.SetSqlText(
        //       0         1       2        3       4        5
        "select O.CATID, O.OID, O.OTYPE, O.NAME, O.DESCR, O2.NAME from *.OBJS O, *.OBJS O2 "
        "where O.CATID in (select OWNCAT "
        "   from *.CATTREE start with OWNCAT=:OWNCAT "
        "   connect by prior OWNCAT=CATID) and O.OTYPE in (4,5) and"
        "   O.PARENT=O2.OID(+) order by O.OTYPE, O.ORD");
    AQ.DeclVar("OWNCAT", FromCatId);
    AQ.Exec("Выбираем только БД и аналитики");
    while (AQ.NotEof())
    {
        int NewCatId = Cat[AQ.GetFldInt(0)];
        switch (AQ.GetFldInt(2))
        {
            case otDB:
            case otAnal:
                if (DbCopyTo (ToName, NewCatId, AQ.GetFldInt(1), AQ.GetFldInt(2),
                    AQ.GetFldStr(3),AQ.GetFldStr(4),AQ.GetFldStr(5), this))
                {
                    Progress->Lines->Add("-DB- "+AQ.GetFldStr(3)+" вставлена ");
                }
                else
                {
                    Progress->Lines->Add("--- DB -- "+AQ.GetFldStr(3)+" не смог создать ");
                }
            break;
            case otViewFrm:
            break;
        }
        this->Refresh();
        AQ.Next();
    }

    // Тут делаем связи между таблицами
    AQ.SetSqlText(
        //       0      1     2      3     4
        "select CATID, OID, OTYPE, NAME, PARENT from *.OBJS "
        "where CATID in (select OWNCAT "
        "   from *.CATTREE start with OWNCAT=:OWNCAT "
        "   connect by prior OWNCAT=CATID) and OTYPE in (4,5)");
    AQ.Exec("Выбираем только БД и аналитики");
    while (AQ.NotEof())
    {
        switch (AQ.GetFldInt(2))
        {
            case otDB:
            case otAnal:
                if (DbCopyToAdd (ToName, AQ.GetFldInt(1), AQ.GetFldInt(2),
                    AQ.GetFldStr(3),AQ.GetFldInt(4), this))
                {
                    Progress->Lines->Add("-DB- "+AQ.GetFldStr(3)+" подправлена ");
                }
                else
                {
                    Progress->Lines->Add("--- DB -- "+AQ.GetFldStr(3)+" не правим ");
                }
            break;
            case otViewFrm:
            break;
        }
        this->Refresh();
        AQ.Next();
    }


    // Копируем объекты из категорий (кроме стандартных операций
    AQ.Clear();
    A1.Clear();
    AQ.SetSqlText(
        //       0      1     2      3     4
        "select CATID, OID, OTYPE, NAME, DESCR from *.OBJS "
        "where CATID in (select OWNCAT "
        "   from *.CATTREE start with OWNCAT=:OWNCAT "
        "   connect by prior OWNCAT=CATID) order by ORD");
    AQ.DeclVar("OWNCAT", FromCatId);
    AQ.Exec("Выбираем все объекты");
    Progress->Lines->Add("Копируем объекты");
    while (AQ.NotEof())
    {
        int NewCatId = Cat[AQ.GetFldInt(0)];
        switch (AQ.GetFldInt(2))
        {
            case otConst:
                if (ConstCopyTo (ToName, NewCatId, AQ.GetFldInt(1), AQ.GetFldStr(3),AQ.GetFldStr(4)))
                {
                    Progress->Lines->Add("-CONST - "+AQ.GetFldStr(3)+" вставлена ");
                }
                else
                {
                    Progress->Lines->Add("--- CONST -- "+AQ.GetFldStr(3)+" уже существует ");
                }
            break;
            case otDoc:
            case otRepForm:
                if (DocCopyTo (ToName, NewCatId, AQ.GetFldInt(1), AQ.GetFldInt(2), AQ.GetFldStr(3),AQ.GetFldStr(4)))
                {
                    Progress->Lines->Add("-DOC- "+AQ.GetFldStr(3)+" вставлен ");
                }
                else
                {
                    Progress->Lines->Add("--- DOC -- "+AQ.GetFldStr(3)+" уже существует ");
                }
            break;

            case otStdOper:
            break;
            case otViewFrm:
            break;
        }
        this->Refresh();
        AQ.Next();
    }
    OkMessage("Объекты скопированы.\r\n"
     "Необходимо запустить amsadmin для бухгалтерии\r\n"+
     ToName+" c параметром ADDUSERS!!!");
}
//---------------------------------------------------------------------------

void __fastcall TFrmCopyTree::PCChange(TObject *Sender)
{
//
    if (PC->ActivePage==TS1)
    {
        Page1Selected();
    }
    else
    if (PC->ActivePage==TS2)
    {
        Page2Selected();
    }
    else
    if (PC->ActivePage==TS3)
    {
        Page3Selected();
    }
    else
    if (PC->ActivePage==TS4)
    {
        Page4Selected();
    }
}
//---------------------------------------------------------------------------
void TFrmCopyTree::Page1Selected(void)
{
    ButPrev->Enabled=false;
    ButNext->Enabled=true;
    BBApply->Visible=false;
    BBApply->Enabled=false;

    if (!OCFrom->Selected)
        OCFrom->Begin(BaseUserName, "");

}
//---------------------------------------------------------------------------
void TFrmCopyTree::Page2Selected(void)
{
    ButPrev->Enabled=true;
    ButNext->Enabled=true;
    BBApply->Visible=false;
    BBApply->Enabled=false;
}
//---------------------------------------------------------------------------
void TFrmCopyTree::Page3Selected(void)
{
    ButPrev->Enabled=true;
    ButNext->Enabled=true;
    BBApply->Visible=false;
    BBApply->Enabled=false;
    if (EdWhereCopy->Text!="")
    {
        ToName=EdWhereCopy->Text;
        if (OCTo->AmsBaseSchema != ToName)
            OCTo->Begin(ToName, "");
    }
    else
    {
        ErrorMessage("Сначала укажите название бухгалтерии куда копировать");
    }
}
//---------------------------------------------------------------------------
void TFrmCopyTree::Page4Selected(void)
{
    ButPrev->Enabled=true;
    ButNext->Enabled=false;

    bool Cont= true;
    if (OCFrom->Selected)
    {
        FromCatId=OCFrom->GetCurrCode();
        LabFromCat->Caption=OCFrom->Selected->Text;

    }
    else
    {
        Cont = false;
    }

    if (EdWhereCopy->Text!="")
    {
        ToName=EdWhereCopy->Text.UpperCase();
        EdWhereCopy->Text=ToName;
        LabBuchName->Caption=ToName;
    }
    else
    {
        Cont=false;
    }


    if (OCTo->Selected)
    {
        ToCatId=OCTo->GetCurrCode();
        LabToCat->Caption=OCTo->Selected->Text;
    }
    else
    {
        Cont=false;
    }
    BBApply->Visible=Cont;
    BBApply->Enabled=Cont;
}
//---------------------------------------------------------------------------
void __fastcall TFrmCopyTree::ButNextClick(TObject *Sender)
{
    PC->ActivePageIndex=PC->ActivePageIndex+1;
    PCChange(this);
}
//---------------------------------------------------------------------------

void __fastcall TFrmCopyTree::ButPrevClick(TObject *Sender)
{
    PC->ActivePageIndex=PC->ActivePageIndex-1;
    PCChange(this);
}
//---------------------------------------------------------------------------


