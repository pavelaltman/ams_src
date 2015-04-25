//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_BUILD_AN_FUNC.h"
#include "MAIN_FRM.h"
#include "AMS_UTILS.h"
#include "PRINT_ERROR.h"
#include "FRM_GEN_SETUP.h"
#include "AMS_QUERY.h"
// "..\amsora\analytic.h"
/*
enum
{
        Addable         =0x1,        // 1
        Totalable       =0x2,        // 2
        CommonVer       =0x4,        // 4
        DebetCredit     =0x8,        // 8
        Hidden          =0x10,       // 16
        SystemFld       =0x100,      // 256
        Frozen          =0x200,      // 512
        Editable        =0x400,      // 1024
        AlwaysFree      =0x800,      // 2048
        EditRecalc      =0x1000,     // 4096
        ZestFld         =0x8000,     // 32768
        RemFld          =0x10000,    // 65536
        ShowFld         =0x20000,    // 131072
        DistrFld        =0x40000,    // 262144
        NoUpdateImp     =0x80000,    // 524288
};
*/
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Oracle"
#pragma resource "*.dfm"

struct FldsOptions
{
public:
    AnsiString RealName;
    bool Addable;
    bool InclToSum;

    FldsOptions(const AnsiString& RealName_,
                bool Addable_ ,
                bool InclToSum_):
    RealName(RealName_), Addable(Addable_), InclToSum(InclToSum_){};                                 
};
typedef std::vector<FldsOptions>::iterator ItFields; 

TFrmBuildAnFunc *FrmBuildAnFunc;
//---------------------------------------------------------------------------
__fastcall TFrmBuildAnFunc::TFrmBuildAnFunc(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmBuildAnFunc::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    TFrmGenSetup *GS = (TFrmGenSetup*) SetupFrm;
    GS->Release();    
    Action = caFree;    
}
//---------------------------------------------------------------------------
int TFrmBuildAnFunc::BuildThis(int BuildWhat)
{
    MemSQL->Lines->Clear();
    int Res =0;
    if (BuildWhat == 1 || BuildWhat==-1)
    {
        BuildAAS_All();
    }
    if (BuildWhat == 2 || BuildWhat==-1)
    {
        BuildHistSupport();
    }
    if (BuildWhat == 3 || BuildWhat==-1)
    {
        BuildRenPos();
    }
    if (BuildWhat == 4 || BuildWhat==-1)
    {
        BuildIsBusyPos();
    }
    if (BuildWhat == 5 || BuildWhat==-1)
    {
        BuildAnInfo();
    }
    if (BuildWhat == 6 || BuildWhat==-1)
    {
        BuildGetKolPos();
    }
    if (BuildWhat == 7 || BuildWhat==-1)
    {
        BuildDelDbPos();
    }
    if (BuildWhat == 8 || BuildWhat==-1)
    {
        BuildShortDbInfo();
    }
    if (BuildWhat == 9 || BuildWhat==-1)
    {
        BuildCAS();
    }
    if (BuildWhat == 10 || BuildWhat==-1)
    {
        BuildAASW_All();
    }
    Message("Закончил работу");
    return Res;
}
//---------------------------------------------------------------------------
void TFrmBuildAnFunc::BuildAAS(int AnCode, const AnsiString& AnName, const AnsiString& Opt, AnsiString& Res)
{
    if (Opt=="")
    {
        OQ3->SQL->Text =
        "select REALNAME, DEF_OPT from DBSTR where DBID = :OID and DELETED=0";
        OQ3->DeleteVariables();
        OQ3->DeclareVariable("OID", otInteger);
        OQ3->SetVariable("OID", AnCode);
    }
    else
    {
        OQ3->SQL->Text = "select S.REALNAME, NVL(O.OPT, S.DEF_OPT) OPT "
              " from DBSTR S, DBOPTOV O "
              " where "
              "     S.DBID = :ANID and "
              "     O.DBID (+) = S.DBID and "
              "     O.REALNAME(+) = S.REALNAME and "
              "     O.SETNAME (+) = :SETNAME and S.DELETED=0";
        OQ3->DeleteVariables();
        OQ3->DeclareVariable("ANID", otInteger);
        OQ3->DeclareVariable("SETNAME", otString);
        OQ3->SetVariable("ANID", AnCode);
        OQ3->SetVariable("SETNAME", Opt);
    }
    ExecSQL(OQ3, "Выбираем поля аналитик");
    std::vector<FldsOptions> Fields;
    bool A, InclInSum;

    while (!OQ3->Eof)
    {
        int Opt = OQ3->FieldAsInteger(1);
        if (!(Opt & SystemFld))
        {
            A= Opt & Addable;
            InclInSum = Opt & ZestFld;
            Fields.push_back(FldsOptions(OQ3->FieldAsString(0), A, InclInSum));
        }
        OQ3->Next();
    }
    Res += "create or replace procedure "+AnName+"_"+Opt+"_AAS(lid number, "
    "rid number,sign number, pCopy number default 1) as\r\n"
    "CURSOR rcrsr IS\r\n"
        "select *\r\n"
    "   from "+AnName+" R where SumId=rid;\r\n"
    "lRow ROWID;\r\n"
    "l "+AnName+"%ROWTYPE;\r\n"
    "begin\r\n"
    " FOR r in rcrsr LOOP\r\n"
    "    select max(ROWID) into lRow\r\n"
    "    from "+AnName+"\r\n"
    "    where\r\n"
    "       SUMID = lid and\r\n"
    "       AARTICLE = r.AARTICLE and\r\n"
    "       AVER = r.AVER;\r\n"
    "    if lRow is NULL then\r\n";
    ItFields i = Fields.begin();
    bool FirstTime = true;
    AnsiString Tmp="";
    while (i != Fields.end())
    {
        if ((*i).InclToSum)
        {
            if (FirstTime)
                FirstTime = false;
            else
                Tmp +=" or ";
            Tmp += "(r."+(*i).RealName+" is not NULL and r."+(*i).RealName+"!=0)";
        }
        i++;
    };
    if (FirstTime )
    {
        THROW_AMS_EXC("Для аналитики "+AnName+" ["+Opt+"] не задано поле включение в сумму");
    };

    Res+="       if "+Tmp+" then\r\n";
    AnsiString Tmp1,TmpR1, TmpR;
    Tmp="sumid, aarticle, aver";
    TmpR= "lid, r.aarticle, r.aver";
    Tmp1=Tmp;
    TmpR1=TmpR;
    i= Fields.begin();
    FirstTime = true;
    bool FirstTime1 = true;
    while (i!= Fields.end())
    {
        if ((*i).Addable)
        {
            Tmp+= ", "+(*i).RealName;
            Tmp1+=", "+(*i).RealName;
            TmpR+=", sign * r."+(*i).RealName;
            TmpR1+=", sign * r."+(*i).RealName;
        }
        else
        {
            Tmp+= ", "+(*i).RealName;
            TmpR+=", r."+(*i).RealName;
        }

        i++;
    };
    Res+=
         "          if pCopy=1 then\r\n"
         "              insert into "+AnName+" ("+Tmp+") values\r\n"
         "              ("+TmpR+");\r\n"
         "          else\r\n"
         "              insert into "+AnName+" ("+Tmp1+") values\r\n"
         "              ("+TmpR1+");\r\n"
         "          end if;\r\n"
         "      end if;\r\n"
         "    else\r\n"
         "       select * into l\r\n"
         "       from "+AnName+"\r\n"
         "       where ROWID = lRow;\r\n";
    i=Fields.begin();
    Tmp = "";
    Tmp1 = "";
    TmpR="";
    FirstTime = true;
    while (i!= Fields.end())
    {
        if ( (*i).InclToSum)
        {
            if (FirstTime)
                FirstTime = false;
            else
                TmpR+=" and ";
            if ( (*i).Addable)
            {
                Tmp+="              l."+(*i).RealName+":=NVL(L."+(*i).RealName+
                    ",0)+NVL(sign*r."+(*i).RealName+",0);\r\n";
                Tmp1+="             l."+(*i).RealName+":=NVL(L."+(*i).RealName+
                    ",0)+NVL(sign*r."+(*i).RealName+",0);\r\n";
            }
            else
            {
                Tmp+="              l."+(*i).RealName+":=NVL(r."+(*i).RealName+
                    ",0);\r\n";
            }
            TmpR += "(l."+(*i).RealName+" is NULL or l."+(*i).RealName+"=0)";
        }
        i++;
    }
    Res+="          if pCopy = 1 then\r\n"+Tmp+
         "          else\r\n"+Tmp1+
         "          end if;\r\n"
         "          if "+TmpR+" and l.aarticle != ' ' then\r\n"
         "              delete "+AnName+" where ROWID = lRow;\r\n"
         "          else\r\n"
         "              if (pCopy=1) then\r\n";
    i=Fields.begin();
    Tmp="                  update "+AnName+" set\r\n";
    Tmp1="                  update "+AnName+" set\r\n";
    FirstTime = true;
    FirstTime1 = true;
    while (i!=Fields.end())
    {
        if (FirstTime)
            FirstTime = false;
        else
            Tmp+=",\r\n";
        if ((*i).Addable)
        {
            if (FirstTime1)
                FirstTime1 = false;
            else
                Tmp1+=",\r\n";
            if ((*i).InclToSum)
            {
                Tmp+="                  "+
                    (*i).RealName+"=l."+(*i).RealName;
                Tmp1+="                  "+
                    (*i).RealName+"=l."+(*i).RealName;
            }
            else
            {
                Tmp+="                  "+
                    (*i).RealName+"=NVL(l."+(*i).RealName+",0)+NVL(sign*r."+
                    (*i).RealName+", 0)";
                Tmp1+="                  "+
                    (*i).RealName+"=NVL(l."+(*i).RealName+",0)+NVL(sign*r."+
                    (*i).RealName+", 0)";
            }
        }
        else
            Tmp+="                  "+
                (*i).RealName+"=r."+(*i).RealName;
        i++;
    };
    Res+=Tmp+"\r\n                  where ROWID = lRow;\r\n"
            "               else\r\n"+Tmp1+
            "\r\n                   where ROWID = lRow;\r\n"
            "               end if;\r\n"
            "          end if;\r\n"
            "     end if;\r\n"
            "  end LOOP;\r\n"
            "end;";

};
//---------------------------------------------------------------------------
void __fastcall TFrmBuildAnFunc::FormCreate(TObject *Sender)
{
    TFrmGenSetup *GS=NULL;
    Application->CreateForm(__classid(TFrmGenSetup), (&GS));
    SetupFrm=GS;
}
//---------------------------------------------------------------------------
int TFrmBuildAnFunc::CheckAndApply
                        (const AnsiString& FuncName,
                         const AnsiString& FuncText,
                         const AnsiString& PlSqlType)
{
    // Возвращает:
    // -1 - функции не существует
    // -2 - существует объект но не совпадает с переданным типом  FUNCTION PROCEDURE TRIGGER
    // -3 - Функция существует но скомпилирована с ошибками
    // -4 - Функция существует и правильна но текст функции отличный от нового
    // 0   - Функция существует и правильна и ее текст совпадает с нашим текстом

    int Res = 0;
    OQ4->SQL->Text = "select STATUS, OBJECT_TYPE from USER_OBJECTS "
     " where "
	 " object_name = :PROC_NAME ";
    OQ4->DeleteVariables();
    OQ4->DeclareVariable("PROC_NAME", otString);
    OQ4->SetVariable("PROC_NAME", FuncName);
    ExecSQL(OQ4, "Состояние процедуры");

    if (OQ4->Eof)
        Res= -1;
    else
    if (OQ4->FieldAsString(0)!= "VALID")
        Res= -3;
    else
    if (OQ4->FieldAsString(1)!= PlSqlType)
        Res= -3;
    else
    {
        if (PlSqlType == "TRIGGER")
        {
            OQ4->SQL->Text = "select TRIGGER_BODY from USER_TRIGGERS "
                "where TRIGGER_NAME= :PROC_NAME";
        }
        else
        {
            OQ4->SQL->Text = "select TEXT from USER_SOURCE where "
                             "NAME = :PROC_NAME order by LINE";
        }
        ExecSQL(OQ4, "Код программы");
        AnsiString StoredText="";
        while (!OQ4->Eof)
        {
            if (PlSqlType == "TRIGGER")
            {
                StoredText= OQ4->Field(0);

                int i = 1;
                /*while(i< StoredText.Length())
                {
                    if (StoredText[i]=='\r')
                        StoredText.Delete(i,1);
                    else
                        i++;
                }
                */
                i = StoredText.Length();

                if (!StoredText[i])
                    StoredText.Delete(i, 1);
            }
            else
            {
                StoredText += OQ4->FieldAsString(0);
            }
            /*int STLen = StoredText.Length();
            if (StoredText[STLen-1]=='\r' && StoredText[STLen]=='\r\n')
                StoredText.Delete(STLen-1, 1);
            */
            OQ4->Next();
        }

        int Beg = FuncText.Pos("begin");

        AnsiString WaitFor = FuncText.SubString(Beg, FuncText.Length()-Beg+1);
        Beg= StoredText.Pos("begin");
        StoredText.Delete(1,Beg-1);


        int i = 1;
        while ( i<= StoredText.Length() && i<= WaitFor.Length() &&
                StoredText[i]== WaitFor[i]) i++;

        if (StoredText != WaitFor && (StoredText != WaitFor+"\r\n") )
            Res= -4;
        else
            MemResult->Lines->Add("Текст процедури "+FuncName+" не изменился");
    }
    return Res;
};
//---------------------------------------------------------------------------
int TFrmBuildAnFunc::BuildAAS_All()
{
    //
    Caption = "Строим функции ААS ";
    AnsiString AddAnalSum=
        "CREATE OR REPLACE PROCEDURE \r\n"
        "AddAnalSum(anal_id NUMBER,     -- Код объекта аналитики \r\n"
        "           new_sum_id NUMBER,  -- Код суммы к которой прибавляем (отнимаем)\r\n"
        "           old_sum_id NUMBER,  -- Код суммы которую прибавляем (отнимаем)\r\n"
        "           sign NUMBER,        -- +1 - прибавление, -1 - отнимание\r\n"
        "           pOptSet varchar2 default NULL, -- набор установок\r\n"
        "           pCopyFlds number default 1)  -- 1-копировать несуммируемые поля \r\n"
        "                                        -- 0 - не копировать\r\n"
        "as\r\n"
        "begin\r\n";

    std::vector <FldsOptions> FO;
    AnsiString AAS="";

    OQ1->SQL->Text="select OID, NAME from OBJS where OTYPE=5 order by ord";
    OQ2->SQL->Text ="select op.SETNAME "
        " from	dbopt op "
        " where op.DBID= :OID ";
    OQ2->DeleteVariables();
    OQ2->DeclareVariable("OID", otInteger);

    ExecSQL(OQ1, "Выбираем аналитики");
    AnsiString AnName;
    int FirstTime=1;
    AnsiString IfC = " if ";
    while (!OQ1->Eof)
    {
        AnsiString FuncBody ="";
        int AnId =OQ1->FieldAsInteger(0);
        OQ2->SetVariable("OID", OQ1->FieldAsInteger(0));
        AnName= OQ1->FieldAsString(1);

        BuildAAS( AnId, AnName, "", FuncBody);
        if (CheckAndApply (AnName+"__AAS", FuncBody, "PROCEDURE"))
        {
            CompileProc(OQ4, AnName+"__AAS", FuncBody, MemResult->Lines);
        }
        AAS+=FuncBody+"\r\n/\r\n";

        ExecSQL(OQ2, "Выбираем наборы установок для аналитик");
        AddAnalSum += IfC+"anal_id = "+ IntToStr(OQ1->FieldAsInteger(0))+" then\r\n"
            "   if pOptSet is NULL then\r\n"
            "       "+AnName+"__AAS(new_sum_id, old_sum_id, sign, pCopyFlds);\r\n";
        if (FirstTime)
        {
            FirstTime = 0;
            IfC= " elsif ";
        };
        while (!OQ2->Eof)
        {
            AnsiString Opt = OQ2->FieldAsString(0);
            FuncBody = "";
            BuildAAS( AnId, AnName, Opt, FuncBody);
            if (CheckAndApply (AnName+"_"+Opt+"_AAS", FuncBody, "PROCEDURE"))
            {
                CompileProc(OQ4, AnName+"_"+Opt+"_AAS", FuncBody, MemResult->Lines);
            }
            MemSQL->Lines->Add(FuncBody+"\r\n/\r\n");

            AmsQuery AQ;
            AQ.SetSqlTextReady("grant execute on "+AnName+"_"+Opt+"_AAS to "+BaseUserName+"ROLE");
            AQ.Exec("Выдаем права");

            AddAnalSum +="   elsif pOptSet= '"+Opt+"' then\r\n"
            "       "+AnName+"_"+Opt+"_AAS(new_sum_id, old_sum_id, sign, pCopyFlds);\r\n";
            OQ2->Next();
        }

        AddAnalSum +=
            "   else\r\n"
            "       RAISE_APPLICATION_ERROR (-20001, ' Не сгенерирована функция "
            " AAS для аналитики "+AnName+" и набора установок '||pOptSet);\r\n"
            "   end if;\r\n\r\n";
        OQ1->Next();
    };
    AddAnalSum+=" end if;\r\nend;\r\n";
    /*
    if (CheckAndApply ("ADDANALSUM", AddAnalSum, "PROCEDURE"))
    {
        CompileProc(OQ4, "ADDANALSUM", AddAnalSum, MemResult->Lines);
    }

    MemSQL->Lines->Add( AddAnalSum+"/\r\n");
    */
    MemSQL->Lines->Add("------ AddAnalSum построены ");
    return 0;
}
//---------------------------------------------------------------------------
int TFrmBuildAnFunc::BuildHistSupport()
{
    Caption = "Строим триггеры для поддрежки истории изменений";
    OQ1->SQL->Text = "select OID, NAME from OBJS where OTYPE=4 and HIST = 1";
    OQ1->DeleteVariables();
    ExecSQL(OQ1, "Все исторические базы данных");
    AnsiString FullText = "";
    AnsiString S="";
    AnsiString S1="";
    while (!OQ1->Eof)
    {
        BuildTextHistSupp(OQ1->FieldAsInteger(0),OQ1->FieldAsString(1), S, S1);
        if (CheckAndApply (OQ1->FieldAsString(1)+"_TR_I", S, "TRIGGER"))
        {
            CompileProc(OQ4, OQ1->FieldAsString(1)+"_TR_I", S, MemResult->Lines);
        }
        MemSQL->Lines->Add(S+"\r\n/\r\n");
        if (CheckAndApply (OQ1->FieldAsString(1)+"_TR_UD", S1, "TRIGGER"))
        {
            CompileProc(OQ4, OQ1->FieldAsString(1)+"_TR_UD", S1, MemResult->Lines);
        }
        MemSQL->Lines->Add(S1+"\r\n/\r\n");

        OQ1->Next();
    };
    MemSQL->Lines->Add("-- Исторические триггеры построены ");
    return 0;
}
//---------------------------------------------------------------------------
int TFrmBuildAnFunc::BuildRenPos()
{
    Caption="Строим процедуры изменения артикула и версии";
    OQ1->SQL->Text = "select OID, NAME, POPTS from OBJS where OTYPE = 4 order by ORD";
    ExecSQL(OQ1, "Выбираем все базы данных");
    AnsiString FullText="";
    AnsiString FuncText="";
    while (!OQ1->Eof)
    {
        BuildRenPos(OQ1->FieldAsInteger(0), OQ1->FieldAsString(1), OQ1->FieldAsInteger(2), FuncText);
        if (CheckAndApply (OQ1->FieldAsString(1)+"_RENAME", FuncText, "PROCEDURE"))
        {
            CompileProc(OQ4, OQ1->FieldAsString(1)+"_RENAME", FuncText, MemResult->Lines);
        }
        MemSQL->Lines->Add(FuncText+"\r\n/\r\n");
        AmsQuery AQ(OQ2);
        AQ.Clear();
        AQ.SetSqlTextReady(" grant execute on "+OQ1->FieldAsString(1)+"_RENAME to "+
                            BaseUserName+"ROLE ");
        AQ.Exec("Выдаем права");

        OQ1->Next();
    }
    //MemSQL->Lines->Add(FullText);
    MemSQL->Lines->Add("-- Построены функции изменения артикула ");
    return 0;
}
//---------------------------------------------------------------------------
void TFrmBuildAnFunc::BuildTextHistSupp (int DbId, const AnsiString& DbName,
            AnsiString& FuncText1, AnsiString& FuncText)
{
    AnsiString DbIdStr= IntToStr(DbId);
    AnsiString Flds ="";
    AnsiString OldFlds ="";
    OQ2->SQL->Text ="select REALNAME from DBSTR "
                    "where DBID = :DBID and DELETED = 0 order by ORD";

    OQ2->DeleteVariables();
    OQ2->DeclareVariable("DBID", otInteger);
    OQ2->SetVariable("DBID", DbId);

    ExecSQL(OQ2, "Поля базы данных");

    while (!OQ2->Eof)
    {
        AnsiString S = OQ2->FieldAsString(0).UpperCase();
        if (S != "ARTICLE" && S != "VER" && S!= "CATID")
        {
            Flds +=", "+S;
            OldFlds += ", :OLD."+S;
        }
        OQ2->Next();
    };


    FuncText1=
      "create or replace TRIGGER\r\n"+DbName+
      "_TR_I after insert on "+DbName+"\r\n"
      "for each row\r\n"
      "declare\r\n"
      "    vDbRev number(8);\r\n"
      "    vClient varchar2(30);\r\n"
      "begin\r\n"
      "    DBMS_APPLICATION_INFO.READ_CLIENT_INFO (vClient);\r\n"
      "    if vClient > 'V000.0000' then\r\n"
      "        select max (REVISION) into vDbRev from dbhistory\r\n"
      "        where\r\n"
      "            DB_ID = "+DbIdStr+" and\r\n"
      "            ARTICLE = :NEW.ARTICLE and\r\n"
      "            VER = :NEW.VER;\r\n"
      "\r\n"
      "        if vDbRev is NULL then\r\n"
      "            vDbRev:= -1;\r\n"
      "        else\r\n"
      "            vDbRev:=vDbRev +1;\r\n"
      "        end if;\r\n"
      "\r\n"
      "        insert into dbhistory\r\n"
      "            (DB_ID,    ARTICLE,        VER,  REVISION,    REV_TIME, USERNAME)\r\n"
      "        values ("+DbIdStr+", :NEW.ARTICLE,  :NEW.VER, vDbRev   ,  sysdate(), user());\r\n"
      "    end if;\r\n"
      "end;\r\n";

    FuncText=
      "create or replace TRIGGER\r\n"
      +DbName+"_TR_UD after update or delete on "+DbName+"\r\n"
      "for each row\r\n"
      "declare\r\n"
      "    vDbRev number(8);\r\n"
      "    vClient varchar2(30);\r\n"
      "begin\r\n"
      "    DBMS_APPLICATION_INFO.READ_CLIENT_INFO (vClient);\r\n"
      "    if vClient > 'V000.0000' then\r\n"
      "        if DELETING OR (:OLD.CATID = :NEW.CATID) then      ----- если не перемещение по категориям\r\n"
      "            select max (REVISION) into vDbRev from dbhistory\r\n"
      "            where\r\n"
      "                DB_ID = "+DbIdStr+" and\r\n"
      "                ARTICLE = :OLD.ARTICLE and\r\n"
      "                VER = :OLD.VER;\r\n"
      "\r\n"
      "            if vDbRev is NULL then\r\n"
      "                if DELETING then\r\n"
      "                    vDbRev := 32766;\r\n"
      "                else\r\n"
      "                    vDbRev:= 0;\r\n"
      "                end if;\r\n"
      "            else\r\n"
      "                vDbRev:=vDbRev +1;\r\n"
      "            end if;\r\n"
      "\r\n"
      "            insert into dbhistory\r\n"
      "                    (DB_ID,       ARTICLE,     VER,    REVISION,   REV_TIME, USERNAME)\r\n"
      "            values ("+DbIdStr+", :old.ARTICLE,  :old.VER, vDbRev   ,  sysdate(), user());\r\n"
      "\r\n"
      "            insert into "+DbName+"_H\r\n"
      "                 (CatId,     ARTICLE,        VER"+Flds+" )\r\n"
      "            values(vDbRev, :OLD.ARTICLE, :OLD.VER"+OldFlds+");\r\n"
      "        end if;\r\n"
      "    end if;\r\n"
      "end;\r\n";
};
//---------------------------------------------------------------------------
void TFrmBuildAnFunc::BuildIsBusyPos()
{
};
//---------------------------------------------------------------------------
void TFrmBuildAnFunc::BuildRenPos(int DbId, const AnsiString& DbName, int POpts, AnsiString& FuncText)
{
    bool Finded = false;
    if (POpts & DBO_ZEROVER)
        Finded = true;
    if (!Finded)
    {
        AmsQuery AQ(OQ2);
        AQ.Clear();
        AQ.SetSqlText(
          "select nvl(OPTS,0) from *.ACCS "
          "where "
            "ANAL_ID in (select OID from *.OBJS where PARENT=:PARENT) "
          "order by OPTS ");
        AQ.DeclVarInt("PARENT",DbId);
        AQ.Exec("Выбираем все счета с аналитикой, которые ссылаются на данную БД");
        while( !Finded && AQ.NotEof())
        {
            if (AQ.GetFldInt(0) & AF_ZEROVER)
                Finded= true;
            else
                AQ.Next();
        }
    }
    OQ2->SQL->Text="select REALNAME from DBSTR where DBID = :DBID"
        " and DELETED = 0 order by ORD";
    OQ2->DeleteVariables();
    OQ2->DeclareVariable("DBID", otInteger);
    OQ2->SetVariable("DBID", DbId);
    ExecSQL(OQ2, "Поля БД");
    AnsiString Flds="";
    while (!OQ2->Eof)
    {
        AnsiString S =OQ2->FieldAsString(0).UpperCase();
        if (S!="ARTICLE" && S!="VER" && S !="CATID")
        {
            Flds+=", "+OQ2->FieldAsString(0);
        }
        OQ2->Next();
    }



    FuncText="create or replace procedure\r\n"+ DbName+"_RENAME (\r\n"
        "   pOldArt varchar2,\r\n"
        "   pOldVer number,\r\n"
        "   pNewArt varchar2,\r\n"
        "   pNewVer number) as\r\n"
        "       vRes number(1);\r\n"
        "begin\r\n";
    // Проверки если существует нулевая версия
    if (Finded)
    {
      FuncText+=
      "   if pOldVer=0 then\r\n"
      "      select max(1) into vRes\r\n"
      "      from "+ DbName+"\r\n"
      "      where ARTICLE=pOldArt and VER != 0;\r\n"
      "      if vRes=1 then\r\n"
      "          raise_application_error(-20017,'Для базы данных "+ DbName+
      " включена опция нулевых версий.\r\n"
      "   Попытайтесь дублировать позицию');\r\n"
      "      end if;\r\n"
      "     end if;\r\n"
      "\r\n"
      "     if pNewVer!=0 then\r\n"
      "         if pNewArt=pOldArt and pOldVer=0  then\r\n"
      "             raise_application_error(-20017,'Для базы данных "+ DbName+
      " включена опция нулевых версий.\r\n"
      "   Попытайтесь дублировать позицию');\r\n"
      "         end if;\r\n"
      "      select max(1) into vRes\r\n"
      "      from "+ DbName+"\r\n"
      "      where ARTICLE=pNewArt and VER=0;\r\n"
      "      if vRes is NULL then\r\n"
      "          raise_application_error(-20017,'Для базы данных "+ DbName+
      " включена опция нулевых версий.\r\n"
      "   Попытайтесь сначала ввести нулевую версию');\r\n"
      "      end if;\r\n"
      "     end if;\r\n";
    }
    FuncText+="   begin\r\n"
        "       insert into "+DbName+"( CATID, ARTICLE, VER"+Flds+")\r\n"
        "       select CATID, pNewArt, pNewVer"+Flds+
        " from "+DbName+"\r\n"
        "       where ARTICLE = pOldArt AND VER = pOldVer;\r\n"
            "   exception when others then\r\n"
            "       raise_application_error(-20016, 'Уже существует такой артикул+версия');\r\n"
            "   end;\r\n";
        OQ2->SQL->Text= "select REALNAME, VTYPE, NAME from DBSTR, OBJS "
        "where "
	    "RDBID = (select OID from OBJS where NAME = :NAME) and "
	    "VTYPE =5 and OID = DBID and DELETED=0";
        OQ2->DeleteVariables();
        OQ2->DeclareVariable("NAME", otString);
        OQ2->SetVariable("NAME", DbName);
        ExecSQL(OQ2, "Какие БД ссылаются на позицию");
        if (!OQ2->Eof)
            FuncText+="\r\n-- Update DB REFS\r\n";
        while (!OQ2->Eof)
        {
            FuncText+= "update "+OQ2->FieldAsString(2)+
                " set "+OQ2->FieldAsString(0)+"= pNewArt, "+
                OQ2->FieldAsString(0)+"_V = pNewVer where "+
                OQ2->FieldAsString(0)+"= pOldArt AND "+
                OQ2->FieldAsString(0)+"_V = pOldVer;\r\n";
            OQ2->Next();
        }
        OQ2->SQL->Text= "select NAME from OBJS "
        "where "
	    "PARENT = (select OID from OBJS where NAME = :NAME) and "
	    "OTYPE =5";
        ExecSQL(OQ2, "Какие аналитики существуют");
        if (!OQ2->Eof)
            FuncText+="\r\n---- Update Anals\r\n";
        while (!OQ2->Eof)
        {
            FuncText+= "update "+OQ2->FieldAsString(0)+
                " set AARTICLE= pNewArt, AVER = pNewVer where "
                " AARTICLE = pOldArt AND AVER = pOldVer;\r\n";
            OQ2->Next();
        };
        FuncText+="delete from "+DbName+
            " where ARTICLE = pOldArt AND VER = pOldVer;\r\n";
        FuncText+="end;\r\n";
}
//---------------------------------------------------------------------------

int TFrmBuildAnFunc::BuildAnInfo(void)
{
    Caption="Строим процедуры короткой информации об аналитике";
    AmsQuery AQ (OQ1);
    AQ.SetSqlTextReady( "select O1.OID, O1.NAME, O2.OID, O2.NAME "
                        "from OBJS O1, OBJS O2 "
                        "    where "
                        "O1.OTYPE = 5 and "
	                    "O2.OID=O1.PARENT order by O1.ORD");
    AQ.Exec("Выбираем все аналитики");
    AnsiString FuncText=
    "create or replace function\r\n"
    "GETANALINFO ( pSumId number, pAnId number)\r\n"
    "                     return varchar2 as\r\n"
    "    Ret varchar2(100);\r\n"
    "    Art varchar2(30);\r\n"
    "    V   number;\r\n"
    "    temp varchar2(100);\r\n"
    "    num_pos number;\r\n"
    "begin\r\n"
    "   Ret:='';";
    while (AQ.NotEof())
    {
        BuildAnInfo(AQ.GetFldInt(0), AQ.GetFldStr(1),
                    AQ.GetFldInt(2), AQ.GetFldStr(3),FuncText);
        AQ.Next();
    }
    FuncText+="     return Ret;\r\nend;";

    if (CheckAndApply ("GETANALINFO", FuncText, "FUNCTION"))
    {
        CompileProc(OQ4, "GETANALINFO", FuncText, MemResult->Lines);
    }

    MemSQL->Lines->Add(FuncText+"\r\n/\r\n");
    MemSQL->Lines->Add("-- Построена функция краткой информации ");

    FuncText=
      "CREATE OR REPLACE FUNCTION\r\n"
      "ANALINFO(oper_id NUMBER, oper_subid NUMBER,acode NUMBER) RETURN VARCHAR2 AS\r\n"
      " ret VARCHAR2(100) ;\r\n"
      " anal_id NUMBER ;\r\n"
      " sum_id NUMBER ;\r\n"
      "BEGIN\r\n"
      " ret:='' ;\r\n"
      "\r\n"
      " SELECT anal_id INTO anal_id FROM accs WHERE code=acode ;\r\n"
      "\r\n"
      " IF anal_id>0 THEN\r\n"
      "   SELECT item_sum_id INTO sum_id\r\n"
      "   FROM op_items\r\n"
      "   WHERE\r\n"
      "    op_id=oper_id AND op_subid=oper_subid AND code=acode ;\r\n"
      "   ret:= GetAnalInfo(sum_id, anal_id);\r\n"
      " end if;\r\n"
      " return ret ;\r\n"
      "END ;\r\n";

    if (CheckAndApply ("ANALINFO", FuncText, "FUNCTION"))
    {
        CompileProc(OQ4, "ANALINFO", FuncText, MemResult->Lines);
    }

    MemSQL->Lines->Add(FuncText+"\r\n/\r\n");
    MemSQL->Lines->Add("-- Построена функция краткой информации для операций");

    return 0;
}

void TFrmBuildAnFunc::BuildAnInfo(int AnId, const AnsiString& AnName,
                                  int DbId, const AnsiString& DbName, AnsiString& ResFunc)
{
    AmsQuery AQ;
    TFldsDescr FldsDescr;
    FldsDescr.SetAnDescr(DbName, DbId, AnName, AnId, "");
    FldsDescr.ReadFldsDescr();
    ResFunc+=
    "   if pAnId="+IntToStr(AnId)+" then\r\n"
    "        SELECT count(*) INTO num_pos FROM "+AnName+" WHERE sumid= pSumId;\r\n"
    "        IF num_pos>2 THEN\r\n"
    "           ret:=TO_CHAR(num_pos-1) || ' наименований ';\r\n";




//------------------------------------------------------CH

    bool FirstTime = true;
    bool HaveInfo= false;
    FirstTime=true;
   for (int i=0; i< FldsDescr.Count(); i++)
    {
        SFldOpt& FO = FldsDescr[i];
        if ((FO.FldAddInfo==aiAn) && (FO.DefOpt&ShowFld) && (FO.DefOpt&Totalable))
        {
            if (FirstTime)
            {
                if (!HaveInfo)
                {
                    ResFunc+=
                    "         SELECT MAX(AARTICLE),MAX(AVER) INTO ART,V FROM "+AnName+"\r\n"
                    "           WHERE SUMID= pSumId AND AARTICLE!=' ';\r\n";
                    HaveInfo = true;
                }
                ResFunc+="         SELECT substr(SUM("+FO.RealName+") ";
                FirstTime = false;
            }
            else
            {
                ResFunc+=
                "||' '|| SUM( "+FO.RealName+") ";
            }
        }
   }


    if (!FirstTime)
    {
        HaveInfo = true;
        ResFunc+=
        ",1,100) INTO temp FROM "+AnName+"\r\n"
        "           where SUMID=pSumId AND AARTICLE!=' ';   \r\n"
                                       //and AARTICLE=ART AND AVER=V;
        "         ret:=ret || temp || ' ';\r\n";
    }
  //  if (!HaveInfo)
  //  {
  //      ResFunc += " Ret:='';\r\n";
  //  }

//-----------------------------------------------------EO CH
    ResFunc+=
    "        ELSIF num_pos=2 THEN\r\n";

    FirstTime = true;
    HaveInfo= false;
    for (int i=0; i< FldsDescr.Count(); i++)
    {
        SFldOpt& FO = FldsDescr[i];
        if (FO.FldAddInfo==aiDb && (FO.DefOpt&ShowFld))
        {
            if (FirstTime)
            {
                ResFunc+=
                "         SELECT AARTICLE,AVER INTO ART,V FROM "+AnName+"\r\n"
                "           WHERE SUMID= pSumId AND AARTICLE!=' ';\r\n"
                "         SELECT substr("+FO.RealName;
                FirstTime = false;
            }
            else
            {
                ResFunc+=
                "||' '||"+FO.RealName;
            }
        }
    }
    if (!FirstTime)
    {
        HaveInfo = true;
        ResFunc+=
        ", 1, 100) INTO temp FROM "+DbName+"\r\n"
        "           WHERE ARTICLE=ART AND VER=V;\r\n"
        "         ret:=ret || temp || ' ';\r\n";
    }
    FirstTime=true;
    for (int i=0; i< FldsDescr.Count(); i++)
    {
        SFldOpt& FO = FldsDescr[i];
        if (FO.FldAddInfo==aiAn && (FO.DefOpt&ShowFld))
        {
            if (FirstTime)
            {
                if (!HaveInfo)
                {
                    ResFunc+=
                    "         SELECT AARTICLE,AVER INTO ART,V FROM "+AnName+"\r\n"
                    "           WHERE SUMID= pSumId AND AARTICLE!=' ';\r\n";
                    HaveInfo = true;
                }
                ResFunc+="         SELECT substr("+FO.RealName;
                FirstTime = false;
            }
            else
            {
                ResFunc+=
                "||' '||"+FO.RealName;
            }
        }
    }
    if (!FirstTime)
    {
        HaveInfo = true;
        ResFunc+=
        ",1,100) INTO temp FROM "+AnName+"\r\n"
        "           where SUMID=pSumId and AARTICLE=ART AND AVER=V;\r\n"
        "         ret:=ret || temp || ' ';\r\n";
    }
    if (!HaveInfo)
    {
        ResFunc += " Ret:='';\r\n";
    }

    ResFunc+=
    "      END IF ;\r\n"
    "      return Ret;\r\n"
    "  END IF ;\r\n";
}

void TFrmBuildAnFunc::BuildGetKolPos()
{
    Caption="Строим функции количества позиций в аналитике";
    MemSQL->Lines->Add("-- Строим функции количества позиций в аналитике\r\n");
    AmsQuery AQ (OQ1);
    AQ.SetSqlTextReady( "select O1.OID, O1.NAME "
                        "from OBJS O1 "
                        "where "
                        "   O1.OTYPE = 5 "
	                    "order by O1.ORD");
    AQ.Exec("Выбираем все аналитики");
    AnsiString FuncText=
    "create or replace function\r\n"
    "GETKOLPOS ( pSumId number, pAnId number)\r\n"
    "                     return number as\r\n"
    "    num_pos number :=0;\r\n"
    "begin\r\n";
    while (AQ.NotEof())
    {
        int AnId =AQ.GetFldInt(0);
        AnsiString AnName=AQ.GetFldStr(1);

        FuncText+=
            "if pAnId="+IntToStr(AnId)+" then\r\n"
            "    select count(*) into num_pos from "+AnName+"\r\n"
            "    where SUMID=pSumId;\r\n"
            "    if num_pos>0 then\r\n"
            "        num_pos:=num_pos-1;\r\n"
            "    end if;\r\n"
            "    return num_pos;\r\n"
            "end if;\r\n";
        AQ.Next();
    }
    FuncText+="     return num_pos;\r\nend;";

    if (CheckAndApply ("GETKOLPOS", FuncText, "FUNCTION"))
    {
        CompileProc(OQ4, "GETKOLPOS", FuncText, MemResult->Lines);
    }

    MemSQL->Lines->Add(FuncText+"\r\n/\r\n");
    MemSQL->Lines->Add("-- Построили функции количества позиций в аналитике ");

    AQ.Clear();
    AQ.SetSqlTextReady("grant execute on GetKolPos to "+BaseUserName+"ROLE");
    AQ.Exec("Выдаем права");
    MemSQL->Lines->Add("grant execute on GetKolPos to "+BaseUserName+"ROLE;\r\n/\r\n");
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
AnsiString Chk(
    "if not (pResStr is NULL) then\r\n"
    "    return 0;\r\n"
    "end if;\r\n");

void TFrmBuildAnFunc::BuildCheckDbRefToDb (int DbId, const AnsiString& DbName,
            AnsiString& TxtVer, AnsiString& TNoVer)
{
    AmsQuery AQ;
    AQ.SetSqlTextReady (
        //        0       1        2          3
        "select O.NAME, O.OID, D.REALNAME, O.DESCR "
        "from DBSTR D, OBJS O "
        "where D.RDBID=:OID and D.VTYPE=5 and "
            "D.DBID = O.OID and O.OTYPE=4 ");
    AQ.DeclVar("OID", DbId);
    AQ.Exec("Все базы данных, имеющие ссылку на данную базу");
    bool FirstTime=true;


    while (AQ.NotEof())
    {
        if (FirstTime)
        {
            FirstTime=false;
            TxtVer+="\r\n--Проверяем базы данных ссылающиеся на данное поле\r\n";
            TNoVer+="\r\n--Проверяем базы данных ссылающиеся на данное поле\r\n";
        }
        TNoVer+=
            "select max('Ссылка из БД "+AQ.GetFldStr(3)+" '||D.ARTICLE||' '||D.VER) into pResStr\r\n"
            "from "+AQ.GetFldStr(0)+" D\r\n"
            "where D."+AQ.GetFldStr(2)+"=pArt and ROWNUM <2\r\n;"+Chk;
        TxtVer+=
            "select max('Ссылка из БД "+AQ.GetFldStr(3)+" '||D.ARTICLE||' '||D.VER) into pResStr\r\n"
            "from "+AQ.GetFldStr(0)+" D\r\n"
            "where D."+AQ.GetFldStr(2)+"=pArt and  D."+AQ.GetFldStr(2)+"_V=pVer and ROWNUM <2\r\n;"+Chk;
        AQ.Next();
    }
};
//---------------------------------------------------------------------------
void TFrmBuildAnFunc::BuildCheckAnRefToDb (int DbId, const AnsiString& DbName,
                AnsiString& TxtVer, AnsiString& TNoVer)
{
    AmsQuery AQ;
    AQ.SetSqlTextReady (
        //        0       1        2          3
        "select O.NAME, O.OID, D.REALNAME, O.DESCR "
        "from DBSTR D, OBJS O "
        "where D.RDBID=:OID and D.VTYPE=5 and "
            "D.DBID = O.OID and O.OTYPE=5 ");
    AQ.DeclVar("OID", DbId);
    AQ.Exec("Все аналитики, имеющие ссылку на данную базу");
    bool FirstTime=true;


    while (AQ.NotEof())
    {
        if (FirstTime)
        {
            FirstTime=false;
            TxtVer+="\r\n--Проверяем аналитики ссылающиеся на данное поле\r\n";
            TNoVer+="\r\n--Проверяем аналитики ссылающиеся на данное поле\r\n";
        }
        TNoVer+=
            "select max('Ссылка из аналитики "+AQ.GetFldStr(3)+" sumid='|| "
            "D.SUMID||' '||D.AARTICLE||' '||D.AVER) into pResStr\r\n"
            "from "+AQ.GetFldStr(0)+" D\r\n"
            "where D."+AQ.GetFldStr(2)+"=pArt and ROWNUM <2\r\n;"+Chk;
        TxtVer+=
            "select max('Ссылка из аналитики "+AQ.GetFldStr(3)+" sumid='|| "
            "D.SUMID||' '||D.AARTICLE||' '||D.AVER) into pResStr\r\n"
            "from "+AQ.GetFldStr(0)+" D\r\n"
            "where D."+AQ.GetFldStr(2)+"=pArt and  D."+AQ.GetFldStr(2)+"_V=pVer and ROWNUM <2\r\n;"+Chk;
        AQ.Next();
    }
};
//---------------------------------------------------------------------------
void TFrmBuildAnFunc::BuildCheckOpers(int DbId, const AnsiString& DbName,
                AnsiString& TxtVer, AnsiString& TNoVer)
{
    AmsQuery AQ;
    AQ.SetSqlTextReady (
        //        0       1
        "select NAME, OID from OBJS where PARENT=:PARENT and OTYPE=5 ");
    AQ.DeclVar("PARENT", DbId);
    AQ.Exec("Все аналитики по данной базе");
    bool FirstTime=true;


    while (AQ.NotEof())
    {
        if (FirstTime)
        {
            FirstTime=false;
            TxtVer+="\r\n--Проверяем операции в аналитиках\r\n";
            TNoVer+="\r\n--Проверяем операции в аналитиках\r\n";
        }
        TNoVer+=
            "select max('Операция по счету '|| A.NAME || ' '|| O.DOC_NUM ||' '\r\n"
            "||to_char(OI.OP_DATE, 'DD-MM-YY')) into pResStr\r\n"
            "from "+AQ.GetFldStr(0)+" AN, op_items OI, opers O, ACCS A\r\n"
            "where\r\n"
            "    AARTICLE=pArt and\r\n"
            "    AN.SUMID=OI.ITEM_SUM_ID and\r\n"
            "    O.OP_ID=OI.OP_ID and\r\n"
            "    O.OP_SUBID=OI.OP_SUBID and\r\n"
            "    A.CODE=OI.CODE and ROWNUM <2;\r\n"+Chk;

        TxtVer+=
            "select max('Операция по счету '|| A.NAME || ' '|| O.DOC_NUM ||' '\r\n"
            "||to_char(OI.OP_DATE, 'DD-MM-YY')) into pResStr\r\n"
            "from "+AQ.GetFldStr(0)+" AN, op_items OI, opers O, ACCS A\r\n"
            "where\r\n"
            "    AARTICLE=pArt and AVER=pVer and\r\n"
            "    AN.SUMID=OI.ITEM_SUM_ID and\r\n"
            "    O.OP_ID=OI.OP_ID and\r\n"
            "    O.OP_SUBID=OI.OP_SUBID and\r\n"
            "    A.CODE=OI.CODE and ROWNUM <2;\r\n"+Chk;
        AQ.Next();
    }
};
//---------------------------------------------------------------------------
void TFrmBuildAnFunc::BuildCheckRems (int DbId, const AnsiString& DbName,
                AnsiString& TxtVer, AnsiString& TNoVer)
{
    AmsQuery AQ;
    AQ.SetSqlTextReady (
        //        0       1
        "select NAME, OID from OBJS where PARENT=:PARENT and OTYPE=5 ");
    AQ.DeclVar("PARENT", DbId);
    AQ.Exec("Все аналитики по данной базе");
    bool FirstTime=true;


    while (AQ.NotEof())
    {
        if (FirstTime)
        {
            FirstTime=false;
            TxtVer+="\r\n--Проверяем остатки\r\n";
            TNoVer+="\r\n--Проверяем остатки\r\n";
        }
        TNoVer+=
            "select max('Отстаток по счету '|| A.NAME || ' на '|| "
            "to_char(R.REM_DATE, 'DD-MM-YY')) into pResStr\r\n"
            "from "+AQ.GetFldStr(0)+" AN, REMS R, ACCS A\r\n"
            "where\r\n"
            "    AARTICLE=pArt and\r\n"
            "    AN.SUMID=R.END_VAL_ID and\r\n"
            "    A.CODE=R.CODE and ROWNUM <2;\r\n"+Chk;
        TxtVer+=
            "select max('Отстаток по счету '|| A.NAME || ' на '|| "
            "to_char(R.REM_DATE, 'DD-MM-YY')) into pResStr\r\n"
            "from "+AQ.GetFldStr(0)+" AN, REMS R, ACCS A\r\n"
            "where\r\n"
            "    AARTICLE=pArt and AVER=pVer and\r\n"
            "    AN.SUMID=R.END_VAL_ID and\r\n"
            "    A.CODE=R.CODE and ROWNUM <2;\r\n"+Chk;
        AQ.Next();
    }
};
//---------------------------------------------------------------------------
void TFrmBuildAnFunc::BuildCheckDbRefToAn(int DbId, const AnsiString& DbName,
            int AnId, const AnsiString& AnName, AnsiString& TxtVer, AnsiString& TNoVer)
{
    AmsQuery AQ;
    AQ.SetSqlTextReady (
        //        0       1        2          3
        "select O.NAME, O.OID, D.REALNAME, O.DESCR "
        "from DBSTR D, OBJS O "
        "where D.RDBID=:OID and D.VTYPE=5 and "
            "D.DBID = O.OID and O.OTYPE=4 ");
    AQ.DeclVar("OID", AnId);
    AQ.Exec("Все БД, имеющие ссылку на данную аналитику");
    bool FirstTime=true;

    while (AQ.NotEof())
    {
        if (FirstTime)
        {
            FirstTime=false;
            TxtVer+="\r\n--Проверяем БД ссылающиеся на аналитику\r\n";
            TNoVer+="\r\n--Проверяем БД ссылающиеся на аналитику\r\n";
        }
        TNoVer+=
            "select max('Сылка на аналитику из БД "+AQ.GetFldStr(3)+" '||\r\n";
            "     BD.ARTICLE||' '||BD.VER) into pResStr\r\n";
            "from "+AQ.GetFldStr(0)+", "+AnName+" AN\r\n";
            "where BD."+AQ.GetFldStr(2)+"=AN.SUMID and\r\n";
            "      AN.AARTICLE=pArt and ROWNUM < 2;\r\n"+Chk;
        TxtVer+=
            "select max('Сылка на аналитику из БД "+AQ.GetFldStr(3)+" '||\r\n";
            "     BD.ARTICLE||' '||BD.VER) into pResStr\r\n";
            "from "+AQ.GetFldStr(0)+", "+AnName+" AN\r\n";
            "where BD."+AQ.GetFldStr(2)+"=AN.SUMID and\r\n";
            "      AN.AARTICLE=pArt and AN.AVER=pVer and ROWNUM < 2;\r\n"+Chk;
        AQ.Next();
    }
};
//---------------------------------------------------------------------------
void TFrmBuildAnFunc::BuildCheckAnRefToAn(int DbId, const AnsiString& DbName, int AnId, const AnsiString& AnName,
                                AnsiString& TxtVer, AnsiString& TNoVer)
{
    AmsQuery AQ;
    AQ.SetSqlTextReady (
        //        0       1        2          3
        "select O.NAME, O.OID, D.REALNAME, O.DESCR "
        "from DBSTR D, OBJS O "
        "where D.RDBID=:OID and D.VTYPE=5 and "
            "D.DBID = O.OID and O.OTYPE=4 ");
    AQ.DeclVar("OID", AnId);
    AQ.Exec("Все аналитики, имеющие ссылку на данную аналитику");
    bool FirstTime=true;

    while (AQ.NotEof())
    {
        if (FirstTime)
        {
            FirstTime=false;
            TxtVer+="\r\n--Проверяем аналитики ссылающиеся на аналитику\r\n";
            TNoVer+="\r\n--Проверяем аналитики ссылающиеся на аналитику\r\n";
        }
        TNoVer+=
            "select max('Сылка на аналитику из аналитики "+AQ.GetFldStr(3)+" SUMID='||\r\n"
            "     BD.SUMID||' Art='||\r\n"
            "     BD.AARTICLE||' '||BD.AVER) into pResStr\r\n"
            "from "+AQ.GetFldStr(0)+" BD, "+AnName+" AN\r\n"
            "where BD."+AQ.GetFldStr(2)+"=AN.SUMID and\r\n"
            "      AN.AARTICLE=pArt and ROWNUM < 2;\r\n"+Chk;
        TxtVer+=
            "select max('Сылка на аналитику из аналитики "+AQ.GetFldStr(3)+" SUMID='||\r\n"
            "     BD.SUMID||' Art='||\r\n"
            "     BD.AARTICLE||' '||BD.AVER) into pResStr\r\n"
            "from "+AQ.GetFldStr(0)+" BD, "+AnName+" AN\r\n"
            "where BD."+AQ.GetFldStr(2)+"=AN.SUMID and\r\n"
            "      AN.AARTICLE=pArt and AN.AVER=pVer and ROWNUM < 2;\r\n"+Chk;
        AQ.Next();
    }
};
//---------------------------------------------------------------------------
void TFrmBuildAnFunc::BuildDelDbPos()
{
    Caption="Строим функции удаления позиции БД";
    MemSQL->Lines->Add("-- Строим функции удаления позиции БД\r\n");
    AmsQuery AQ (OQ1);
    AQ.SetSqlTextReady( "select O1.OID, O1.NAME "
                        "from OBJS O1 "
                        "where "
                        "   O1.OTYPE = 4 "
	                    "order by O1.ORD");
    AQ.Exec("Выбираем все БД");
    AmsQuery A2 (OQ2);
    A2.Clear();
    A2.SetSqlTextReady("select OID, NAME from OBJS where PARENT=:PARENT and OTYPE=5");
    A2.DeclVar("PARENT", 0);
    while (AQ.NotEof())
    {
        int DbId =AQ.GetFldInt(0);
        AnsiString DbName=AQ.GetFldStr(1);

        AnsiString FuncText="create or replace function\r\n"+
        DbName+"_DELPOS ( pArt varchar2,\r\n"
        "    pVer number,\r\n"
        "    pResStr out varchar2, LEV in out number ) return number as\r\n"
        "      Pr number;\r\n"
        "begin\r\n"
        "    if LEV > 100 then\r\n"
        "        pResStr:='Не могу удалить позицию. Какой-то нестандартный CONSTRAINT';\r\n"
        "        return 0;\r\n"
        "    else\r\n"
        "        Lev:=Lev+1;\r\n"
        "    end if;\r\n"
        "    ----- Проверяем имеет ли пользователь права на удаление из таблицы\r\n"
        "    select max(PR3) into Pr from PRIVTEST where POID="+IntToStr(DbId)+" and UNAME=USER;\r\n"
        "    if PR is NULL or PR=0 then\r\n"
        "        pResStr:= 'Нет прав на удаление';\r\n"
        "        return 0;\r\n"
        "    end if;\r\n"
        "    if pVer is NULL then    ---- Удаление со спрятанными версиями\r\n";

        AnsiString TxtVer=
        "    begin\r\n"
        "        delete from "+DbName+" where ARTICLE=pArt and VER=pVer;\r\n"
        "        return 1;\r\n"
        "    exception\r\n"
        "        when others then\r\n";

        AnsiString TNoVer=
        "    begin\r\n"
        "        delete from "+DbName+" where ARTICLE=pArt;\r\n"
        "        return 1;\r\n"
        "    exception\r\n"
        "        when others then\r\n";

        BuildCheckDbRefToDb (DbId, DbName, TxtVer, TNoVer);
        BuildCheckAnRefToDb (DbId, DbName, TxtVer, TNoVer);
        BuildCheckOpers(DbId, DbName, TxtVer, TNoVer);
        BuildCheckRems(DbId, DbName, TxtVer, TNoVer);

        A2.DeclVar("PARENT", DbId);
        A2.Exec("Идем по всем аналитикам");
        while (A2.NotEof())
        {
            int AnId = A2.GetFldInt(0);
            AnsiString AnName= A2.GetFldStr(1);
            BuildCheckDbRefToAn (DbId, DbName, AnId, AnName, TxtVer, TNoVer);
            BuildCheckAnRefToAn (DbId, DbName, AnId, AnName, TxtVer, TNoVer);
            A2.Next();
        }

        A2.Exec("Идем по всем аналитикам");
        while (A2.NotEof())
        {
            AnsiString AnName= A2.GetFldStr(1);
            TxtVer+="delete from "+AnName+" where AARTICLE=pArt and AVER=pVer;\r\n";
            TNoVer+="delete from "+AnName+" where AARTICLE=pArt;\r\n";
            A2.Next();
        }
        TxtVer+="return "+DbName+"_DELPOS (pArt, pVer, pResStr, Lev);\r\n";
        TNoVer+="return "+DbName+"_DELPOS (pArt, pVer, pResStr, Lev);\r\n";

        FuncText+=TNoVer+"NULL; end;\r\nelse\r\n"+TxtVer+"NULL; end;\r\nend if;\r\nreturn 1;\r\nend;";


        if (CheckAndApply (DbName+"_DELPOS", FuncText, "FUNCTION"))
        {
            CompileProc(OQ4, DbName+"_DELPOS", FuncText, MemResult->Lines);
        }

        MemSQL->Lines->Add(FuncText+"\r\n/\r\n");
        MemSQL->Lines->Add("-- Построили функцию "+DbName+"_DELPOS" );
        AmsQuery A1(OQ4);
        A1.Clear();
        A1.SetSqlTextReady("grant execute on "+DbName+"_DELPOS to "+BaseUserName+"ROLE");
        A1.Exec("Выдаем права");

        AQ.Next();
    }
}
//---------------------------------------------------------------------------
int TFrmBuildAnFunc::BuildShortDbInfo(void)
{
    Caption="Строим процедуры короткой информации об БД";
    AmsQuery AQ (OQ1);
    AQ.SetSqlTextReady( "select O1.OID, O1.NAME "
                        "from OBJS O1 where "
                        "O1.OTYPE = 4 order by O1.ORD");
    AQ.Exec("Выбираем все аналитики");
    AnsiString FuncText=
    "create or replace function\r\n"
    "GETDBSHORTINFO(pDbName varchar2, pArt varchar2, pVer number)\r\n"
    "                     return varchar2 as\r\n"
    "    Ret varchar2(250);\r\n"
    "    DbName varchar2(30);\r\n"
    "begin\r\n"
    "   Ret:=''; DbName:=UPPER(pDbName);\r\n";
    while (AQ.NotEof())
    {
        BuildShortDbInfo(AQ.GetFldInt(0), AQ.GetFldStr(1), FuncText);
        AQ.Next();
    }
    FuncText+="     return Ret;\r\nend;";

    if (CheckAndApply ("GETDBSHORTINFO", FuncText, "FUNCTION"))
    {
        CompileProc(OQ4, "GETDBSHORTINFO", FuncText, MemResult->Lines);
    }
    AQ.Clear();
    AQ.SetSqlTextReady("grant execute on GETDBSHORTINFO to "+BaseUserName+"ROLE");
    AQ.Exec("Права на GETDBSHORTINFO");

    MemSQL->Lines->Add(FuncText+"\r\n/\r\n");
    MemSQL->Lines->Add("-- Построена функция краткой информации о БД");
    return 0;
}
//---------------------------------------------------------------------------
void TFrmBuildAnFunc::BuildShortDbInfo(int DbId, const AnsiString& DbName, AnsiString& ResFunc)
{
    AmsQuery AQ;
    TFldsDescr FldsDescr;
    FldsDescr.SetDbDescr(DbName, DbId, "");
    FldsDescr.ReadFldsDescr();

    bool FirstTime = true;
    for (int i=0; i< FldsDescr.Count(); i++)
    {
        SFldOpt& FO = FldsDescr[i];
        if (FO.FldAddInfo==aiDb && (FO.DefOpt&ShowFld))
        {
            if (FirstTime)
            {
                ResFunc+=
                "   if DbName='"+DbName+"' then\r\n"
                "         SELECT substr ("+FO.RealName;
                FirstTime = false;
            }
            else
            {
                ResFunc+=
                "||' '||"+FO.RealName;
            }
        }
    }
    if (!FirstTime)
    {
        ResFunc+=
        " , 1, 240) INTO Ret FROM "+DbName+"\r\n"
        "           WHERE ARTICLE=pART AND VER=pVer;\r\n"
        "       return Ret;\r\n"
        "   end if;\r\n";
    }
}
//---------------------------------------------------------------------------
int TFrmBuildAnFunc::BuildCAS()
{
    Caption = "Строим функции САS ";
    AnsiString AddAnalSum=
        "CREATE OR REPLACE PROCEDURE \r\n"
        "CopyAnalSum(anal_id NUMBER,     -- Код объекта аналитики \r\n"
        "           new_sum_id NUMBER,  -- Код суммы в которую копируем\r\n"
        "           old_sum_id NUMBER)  -- Код суммы которую копируем\r\n"
        "as\r\n"
        "begin\r\n";

    std::vector <FldsOptions> FO;
    AnsiString AAS="";

    OQ1->SQL->Text="select OID, NAME from OBJS where OTYPE=5 order by ord";
    OQ2->SQL->Text ="select op.SETNAME "
        " from	dbopt op "
        " where op.DBID= :OID ";
    OQ2->DeleteVariables();
    OQ2->DeclareVariable("OID", otInteger);

    ExecSQL(OQ1, "Выбираем аналитики");
    AnsiString AnName;
    int FirstTime=1;
    AnsiString IfC = " if ";
    while (!OQ1->Eof)
    {
        AnsiString FuncBody ="";
        int AnId =OQ1->FieldAsInteger(0);
        OQ2->SetVariable("OID", OQ1->FieldAsInteger(0));
        AnName= OQ1->FieldAsString(1);

        BuildCAS( AnId, AnName, "", FuncBody);
        if (CheckAndApply (AnName+"_CAS", FuncBody, "PROCEDURE"))
        {
            CompileProc(OQ4, AnName+"_CAS", FuncBody, MemResult->Lines);
            AmsQuery AQ(OQ4);
            AQ.Clear();
            AQ.SetSqlTextReady("grant execute on "+AnName+"_CAS to "+BaseUserName+"ROLE");
            AQ.Exec("Выдача прав");
        }
        AAS+=FuncBody+"\r\n/\r\n";
        MemSQL->Lines->Add(FuncBody+"\r\n/\r\n");
        AmsQuery AQ;
        AQ.SetSqlTextReady("grant execute on "+AnName+"_CAS to "+BaseUserName+"ROLE");
        AQ.Exec("Выдаем права");
        OQ1->Next();
    };
    MemSQL->Lines->Add("-- Построена функции CAS");

    /*
    if (CheckAndApply ("COPYANALSUM", AddAnalSum, "PROCEDURE"))
    {
        CompileProc(OQ4, "COPYANALSUM", AddAnalSum, MemResult->Lines);
        AmsQuery AQ(OQ4);
        AQ.Clear();
        AQ.SetSqlTextReady("grant execute on COPYANALSUM to "+BaseUserName+"ROLE");
        AQ.Exec("Выдача прав");
    }

    MemSQL->Lines->Add( AddAnalSum+"/\r\n");

    MemSQL->Lines->Add("------ CopyAnalSum построены ");
    */
    return 0;
}
//---------------------------------------------------------------------------
void TFrmBuildAnFunc::BuildCAS(int AnCode, const AnsiString& AnName, const AnsiString& Opt, AnsiString& Res)
{
    if (Opt=="")
    {
        OQ3->SQL->Text =
        "select REALNAME, 1 from DBSTR where DBID = :OID and DELETED=0";
        OQ3->DeleteVariables();
        OQ3->DeclareVariable("OID", otInteger);
        OQ3->SetVariable("OID", AnCode);
    }
    else
    {
        OQ3->SQL->Text = "select S.REALNAME, 1 "
              " from DBSTR S, DBOPTOV O "
              " where "
              "     S.DBID = :ANID and "
              "     O.DBID (+) = S.DBID and "
              "     O.REALNAME(+) = S.REALNAME and "
              "     O.SETNAME (+) = :SETNAME and S.DELETED=0";
        OQ3->DeleteVariables();
        OQ3->DeclareVariable("ANID", otInteger);
        OQ3->DeclareVariable("SETNAME", otString);
        OQ3->SetVariable("ANID", AnCode);
        OQ3->SetVariable("SETNAME", Opt);
    }
    ExecSQL(OQ3, "Выбираем поля аналитик");
    std::vector<FldsOptions> Fields;
    bool A, InclInSum;

    while (!OQ3->Eof)
    {
        AnsiString FN = OQ3->FieldAsString(0);
        FN=FN.UpperCase();
        int Opt = OQ3->FieldAsInteger(1);
        if (FN!="SUMID" && FN!="AARTICLE" && FN!="AVER" && FN!="OP_DATE")
        {
            A= Opt & Addable;
            InclInSum = Opt & ZestFld;
            Fields.push_back(FldsOptions(OQ3->FieldAsString(0), A, InclInSum));
        }
        OQ3->Next();
    }

    Res += "create or replace procedure "+AnName+"_CAS(lid number, "
    "rid number) as\r\n"
    "begin\r\n"
    //"   delete from "+AnName+" where sumid=lid;\r\n"
    ;
    ItFields i= Fields.begin();
    bool FirstTime = true;
    AnsiString Tmp="   insert into "+AnName+"(sumid, AARTICLE, AVER";
    AnsiString Tmp1="\r\n      ( select lid, AARTICLE, AVER";
    while (i!= Fields.end())
    {
        Tmp+= ", "+(*i).RealName;
        Tmp1+=", "+(*i).RealName;
        i++;
    };
    Res+=Tmp+")"+Tmp1+"\r\n      from "+AnName+" where SUMID=rid );\r\nend;";
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TFrmBuildAnFunc::BuildAASW(int AnCode, const AnsiString& AnName, const AnsiString& Opt, AnsiString& Res)
{
    if (Opt=="")
    {
        OQ3->SQL->Text =
        "select REALNAME, DEF_OPT from DBSTR where DBID = :OID and DELETED=0";
        OQ3->DeleteVariables();
        OQ3->DeclareVariable("OID", otInteger);
        OQ3->SetVariable("OID", AnCode);
    }
    else
    {
        OQ3->SQL->Text = "select S.REALNAME, NVL(O.OPT, S.DEF_OPT) OPT "
              " from DBSTR S, DBOPTOV O "
              " where "
              "     S.DBID = :ANID and "
              "     O.DBID (+) = S.DBID and "
              "     O.REALNAME(+) = S.REALNAME and "
              "     O.SETNAME (+) = :SETNAME and S.DELETED=0";
        OQ3->DeleteVariables();
        OQ3->DeclareVariable("ANID", otInteger);
        OQ3->DeclareVariable("SETNAME", otString);
        OQ3->SetVariable("ANID", AnCode);
        OQ3->SetVariable("SETNAME", Opt);
    }
    ExecSQL(OQ3, "Выбираем поля аналитик");
    std::vector<FldsOptions> Fields;
    bool A, InclInSum;

    while (!OQ3->Eof)
    {
        int Opt = OQ3->FieldAsInteger(1);
        if (!(Opt & SystemFld))
        {
            A= Opt & Addable;
            InclInSum = Opt & ZestFld;
            Fields.push_back(FldsOptions(OQ3->FieldAsString(0), A, InclInSum));
        }
        OQ3->Next();
    }
    Res += "create or replace procedure "+AnName+"_"+Opt+"_AASW(lid number, "
    "sign number, rcrsr in out AMS_Types.GenCursor) as\r\n"
    "lRow ROWID;\r\n"
    "l "+AnName+"%ROWTYPE;\r\n"
    "r "+AnName+"%ROWTYPE;\r\n"
    "begin\r\n"
    "  LOOP\r\n"
    "    fetch rcrsr into r;\r\n"
    "    EXIT WHEN rcrsr %NOTFOUND; -- exit when last row is fetched\r\n"
    "    select max(ROWID) into lRow\r\n"
    "    from "+AnName+"\r\n"
    "    where\r\n"
    "       SUMID = lid and\r\n"
    "       AARTICLE = r.AARTICLE and\r\n"
    "       AVER = r.AVER;\r\n"
    "    if lRow is NULL then\r\n";
    ItFields i = Fields.begin();
    bool FirstTime = true;
    AnsiString Tmp="";
    while (i != Fields.end())
    {
        if ((*i).InclToSum)
        {
            if (FirstTime)
                FirstTime = false;
            else
                Tmp +=" or ";
            Tmp += "(r."+(*i).RealName+" is not NULL and r."+(*i).RealName+"!=0)";
        }
        i++;
    };
    if (FirstTime )
    {
        THROW_AMS_EXC("Для аналитики "+AnName+" ["+Opt+"] не задано поле включение в сумму");
    };

    Res+="       if "+Tmp+" then\r\n";
    AnsiString Tmp1,TmpR1, TmpR;
    Tmp="sumid, aarticle, aver";
    TmpR= "lid, r.aarticle, r.aver";
    Tmp1=Tmp;
    TmpR1=TmpR;
    i= Fields.begin();
    FirstTime = true;
    bool FirstTime1 = true;
    while (i!= Fields.end())
    {
        if ((*i).Addable)
        {
            Tmp+= ", "+(*i).RealName;
            Tmp1+=", "+(*i).RealName;
            TmpR+=", sign * r."+(*i).RealName;
            TmpR1+=", sign * r."+(*i).RealName;
        }
        else
        {
            Tmp+= ", "+(*i).RealName;
            TmpR+=", r."+(*i).RealName;
        }

        i++;
    };
    Res+=
         "              insert into "+AnName+" ("+Tmp+") values\r\n"
         "              ("+TmpR+");\r\n"
         "      end if;\r\n"
         "    else\r\n"
         "       select * into l\r\n"
         "       from "+AnName+"\r\n"
         "       where ROWID = lRow;\r\n";
    i=Fields.begin();
    Tmp = "";
    Tmp1 = "";
    TmpR="";
    FirstTime = true;
    while (i!= Fields.end())
    {
        if ( (*i).InclToSum)
        {
            if (FirstTime)
                FirstTime = false;
            else
                TmpR+=" and ";
            if ( (*i).Addable)
            {
                Tmp+="              l."+(*i).RealName+":=NVL(L."+(*i).RealName+
                    ",0)+NVL(sign*r."+(*i).RealName+",0);\r\n";
                Tmp1+="             l."+(*i).RealName+":=NVL(L."+(*i).RealName+
                    ",0)+NVL(sign*r."+(*i).RealName+",0);\r\n";
            }
            else
            {
                Tmp+="              l."+(*i).RealName+":=NVL(r."+(*i).RealName+
                    ",0);\r\n";
            }
            TmpR += "(l."+(*i).RealName+" is NULL or l."+(*i).RealName+"=0)";
        }
        i++;
    }
    Res+=Tmp+
         "          if "+TmpR+" and l.aarticle != ' ' then\r\n"
         "              delete "+AnName+" where ROWID = lRow;\r\n"
         "          else\r\n"
         "              if (1=1) then\r\n";
    i=Fields.begin();
    Tmp="                  update "+AnName+" set\r\n";
    Tmp1="                  update "+AnName+" set\r\n";
    FirstTime = true;
    FirstTime1 = true;
    while (i!=Fields.end())
    {
        if (FirstTime)
            FirstTime = false;
        else
            Tmp+=",\r\n";
        if ((*i).Addable)
        {
            if (FirstTime1)
                FirstTime1 = false;
            else
                Tmp1+=",\r\n";
            if ((*i).InclToSum)
            {
                Tmp+="                  "+
                    (*i).RealName+"=l."+(*i).RealName;
                Tmp1+="                  "+
                    (*i).RealName+"=l."+(*i).RealName;
            }
            else
            {
                Tmp+="                  "+
                    (*i).RealName+"=NVL(l."+(*i).RealName+",0)+NVL(sign*r."+
                    (*i).RealName+", 0)";
                Tmp1+="                  "+
                    (*i).RealName+"=NVL(l."+(*i).RealName+",0)+NVL(sign*r."+
                    (*i).RealName+", 0)";
            }
        }
        else
            Tmp+="                  "+
                (*i).RealName+"=r."+(*i).RealName;
        i++;
    };
    Res+=Tmp+"\r\n                  where ROWID = lRow;\r\n"
            "               end if;\r\n"
            "          end if;\r\n"
            "     end if;\r\n"
            "  end LOOP;\r\n"
            "end;";

};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
int TFrmBuildAnFunc::BuildAASW_All()
{
    //
    Caption = "Строим функции условных ААS  ";
    AnsiString AddAnalSum=
        "CREATE OR REPLACE PROCEDURE \r\n"
        "AddAnalSumW(anal_id NUMBER,     -- Код объекта аналитики \r\n"
        "           new_sum_id NUMBER,  -- Код суммы к которой прибавляем (отнимаем)\r\n"
        "           sign NUMBER,        -- +1 - прибавление, -1 - отнимание\r\n"
        "           pOptSet varchar2 default NULL, -- набор установок\r\n"
        "           rcrsr in out AMS_Types.GenCursor)-- курсор с наложенным условием  \r\n"
        " -- должен выглядеть как\r\n"
        " --    cursor S is select AN.* from *.ANAL_NAME AN, *.DB_NAME DB where\r\n"
        " --          AN.SUMID= :ADD_SUMID and AN.AARTICLE=DB.ARTICLE and AN.AVER=DB.VER and (user_where)\r\n"
        "as\r\n"
        "begin\r\n";

    std::vector <FldsOptions> FO;
    AnsiString AAS="";

    OQ1->SQL->Text="select OID, NAME from OBJS where OTYPE=5 order by ord";
    OQ2->SQL->Text ="select op.SETNAME "
        " from	dbopt op "
        " where op.DBID= :OID ";
    OQ2->DeleteVariables();
    OQ2->DeclareVariable("OID", otInteger);

    ExecSQL(OQ1, "Выбираем аналитики");
    AnsiString AnName;
    int FirstTime=1;
    AnsiString IfC = " if ";
    while (!OQ1->Eof)
    {
        AnsiString FuncBody ="";
        int AnId =OQ1->FieldAsInteger(0);
        OQ2->SetVariable("OID", OQ1->FieldAsInteger(0));
        AnName= OQ1->FieldAsString(1);

        BuildAASW( AnId, AnName, "", FuncBody);
        if (CheckAndApply (AnName+"__AASW", FuncBody, "PROCEDURE"))
        {
            CompileProc(OQ4, AnName+"__AASW", FuncBody, MemResult->Lines);
        }
        AAS+=FuncBody+"\r\n/\r\n";

        ExecSQL(OQ2, "Выбираем наборы установок для аналитик");
        AddAnalSum += IfC+"anal_id = "+ IntToStr(OQ1->FieldAsInteger(0))+" then\r\n"
            "   if pOptSet is NULL then\r\n"
            "       "+AnName+"__AASW(new_sum_id, sign, rcrsr);\r\n";
        if (FirstTime)
        {
            FirstTime = 0;
            IfC= " elsif ";
        };
        while (!OQ2->Eof)
        {
            AnsiString Opt = OQ2->FieldAsString(0);
            FuncBody = "";
            BuildAASW( AnId, AnName, Opt, FuncBody);
            if (CheckAndApply (AnName+"_"+Opt+"_AASW", FuncBody, "PROCEDURE"))
            {
                CompileProc(OQ4, AnName+"_"+Opt+"_AASW", FuncBody, MemResult->Lines);
            }
            MemSQL->Lines->Add(FuncBody+"\r\n/\r\n");

            AmsQuery AQ;
            AQ.SetSqlTextReady("grant execute on "+AnName+"_"+Opt+"_AASW to "+BaseUserName+"ROLE");
            AQ.Exec("Выдаем права");

            AddAnalSum +="   elsif pOptSet= '"+Opt+"' then\r\n"
            "       "+AnName+"_"+Opt+"_AASW(new_sum_id, sign, rcrsr);\r\n";
            OQ2->Next();
        }

        AddAnalSum +=
            "   else\r\n"
            "       RAISE_APPLICATION_ERROR (-20001, ' Не сгенерирована функция "
            " AASW для аналитики "+AnName+" и набора установок '||pOptSet);\r\n"
            "   end if;\r\n\r\n";
        OQ1->Next();
    };
    AddAnalSum+=" end if;\r\nend;\r\n";

    if (CheckAndApply ("ADDANALSUMW", AddAnalSum, "PROCEDURE"))
    {
        CompileProc(OQ4, "ADDANALSUMW", AddAnalSum, MemResult->Lines);
        AmsQuery AQ;
        AQ.SetSqlTextReady("grant execute on ADDANALSUMW to "+BaseUserName+"ROLE");
        AQ.Exec("Выдаем права");
    }

    MemSQL->Lines->Add( AddAnalSum+"/\r\n");

    MemSQL->Lines->Add("------ Условные AddAnalSum построены ");
    return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



void __fastcall TFrmBuildAnFunc::N2Click(TObject *Sender)
{
//
    if (SD->Execute())
    {
        MemSQL->Lines->SaveToFile(SD->FileName);
        Message("Готово");
    }
}
//---------------------------------------------------------------------------

