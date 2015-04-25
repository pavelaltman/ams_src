//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FLDS_DESCR_DISTR.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TFldsDescrAnDestr::BuildSqlQuery(TOracleDataSet* ODS_)
{
    ODS_->Active = false;
    ODS_->DeleteVariables();
    DS_Q D (ODS_);

    // Без версий
    // Формируем строку с макс поз
    AnsiString SelMax = "select max (1) RID, max(AN.ROWID) ANALITIC_ROWID ";

    for ( int i = 0; i< Count(); i++)
    {
        if ( At(i).RealName != "ARTICLE")
        {
            if (At(i).DefOpt & Addable)
                SelMax +=", sum("+At(i).FullName+") as "+At(i).SqlAlias;
            else
                SelMax +=", max("+At(i).FullName+") as "+At(i).SqlAlias;
        }
        else
            SelMax+=", ARTICLE";
    }

    SelMax +=" from "+BaseUserName+"."+DbName+" D, ";
    SelMax +=BaseUserName+"."+AnName+" AN ";
    if (AnSumOst)
    {
        SelMax+=", "+BaseUserName+"."+AnName+" RAN";
    }

    // Для доп полей
    std::vector<SAddAnSumDescr>& DescrAnSum= GetAddFldsDescr();
    for (unsigned int i =0; i< AddAnSums.size(); i++)
    {
        SAddAnSumDescr& D = DescrAnSum[i];
        SelMax+=", "+BaseUserName+"."+AnName+" "+D.TurnType+
                IntToStr(D.SubAccCode)+" ";
    }


    SelMax+=" where ";
    SelMax+="D.ARTICLE=:ARTICLE and ";
    SelMax+=" AN.SUMID=:ANSUMID and D.ARTICLE=AN.AARTICLE and D.VER=AN.AVER";
    if (AnSumOst)
    {
        SelMax+=" and "
        " RAN.SUMID(+)=:RANSUMID and D.ARTICLE=RAN.AARTICLE(+) and D.VER=RAN.AVER(+) ";
        ODS_->DeclareVariable("RANSUMID", otInteger);
        ODS_->SetVariable("RANSUMID",AnSumOst);
    }
    // Для доп полей

    for (unsigned int i =0; i< DescrAnSum.size(); i++)
    {
        SAddAnSumDescr& DI = DescrAnSum[i];
        AnsiString A =DI.TurnType+IntToStr(DI.SubAccCode);

        SelMax+=" and "+A+".SUMID(+)=:"+A+"SUMID and D.ARTICLE="+A+
            ".AARTICLE(+) and D.VER="+A+".AVER(+) ";
        D.DeclVar(A+"SUMID", AddAnSums[i]);
    }

    ODS_->DeclareVariable("ANSUMID", otInteger);
    ODS_->SetVariable("ANSUMID",AnSumEd);
    SelMax+=" group by D.ARTICLE ";
    ODS_->SQL->Text = SelMax;
}
//---------------------------------------------------------------------------
void TFldsDescrAnDestr::BuildSqlQueryRecalc(TOracleDataSet* ODS_)
{
    ODS_->Active = false;
    ODS_->DeleteVariables();

    DS_Q D (ODS_);

    AnsiString SelBeg = "select D.ROWID DROW_ID, AN.ROWID ";
    for ( int i = 0; i< Count(); i++)
    {
        SFldOpt& FO = At(i);
        if (FO.FldAddInfo!=aiAnR || AnSumOst)
            SelBeg +=", "+FO.FullName+" "+FO.SqlAlias;
    }

    SelBeg +=" from "+BaseUserName+"."+AnName+" AN, "+BaseUserName+"."+DbName+" D ";
    if (AnSumOst)
    {
        SelBeg+=", "+BaseUserName+"."+AnName+" RAN";
    }

    // Для доп полей
    for (unsigned int i =0; i< AddAnSums.size(); i++)
    {
        SAddAnSumDescr& D = AddFldsDescr[i];
        SelBeg+=", "+BaseUserName+"."+AnName+" "+D.TurnType+
                IntToStr(D.SubAccCode)+" ";
    }

    SelBeg+=" where "
     "D.ARTICLE!=' ' and ";
    SelBeg+=" AN.SUMID=:ANSUMID and D.ARTICLE=AN.AARTICLE and D.VER=AN.AVER";
    if (AnSumOst)
    {
        SelBeg+=" and "
        " RAN.SUMID(+)=:RANSUMID and D.ARTICLE=RAN.AARTICLE(+) and D.VER=RAN.AVER(+) ";
        D.DeclVar("RANSUMID", AnSumOst);
    }

    // Для доп полей
    for (unsigned int i =0; i< AddAnSums.size(); i++)
    {
        SAddAnSumDescr& DI = AddFldsDescr[i];
        AnsiString A =DI.TurnType+IntToStr(DI.SubAccCode);

        SelBeg+=" and "+A+".SUMID(+)=:"+A+"SUMID and D.ARTICLE="+A+
            ".AARTICLE(+) and D.VER="+A+".AVER(+) ";
        D.DeclVar(A+"SUMID", AddAnSums[i]);
    }

    D.DeclVar("ANSUMID", AnSumEd);
    SelBeg+=" order by D.ARTICLE, D.VER";
    ODS_->SQL->Text=SelBeg;
}
//---------------------------------------------------------------------------
void TFldsDescrAnDestr::Recalc(bool ClrFlds, TOracleDataSet* ODS, TGenLocalVars* Vars_ , int REQuil)
{
    Vars=Vars_;
    // Список полей "включать в СУММУ"
    std::vector<int> InclInSum;
    // Список итого полей
    std::vector<int> TotalFlds;


    AnsiString UQ="begin update "+BaseUserName+"."+AnName+" set ";
    bool HaveForUpd= false;
    AmsQuery UpdQ;
    for (int i =0; i< Count(); i++)
    {
        SFldOpt& FO = At(i);
        if ( (FO.DefOpt & ZestFld) && (FO.FldAddInfo==aiAn)) //ZestFldInp
        {
            InclInSum.push_back(i);
        }
        if ( (FO.DefOpt & Totalable)&& (FO.DefOpt & Addable) && (FO.FldAddInfo==aiAn))
        {
            TotalFlds.push_back(i);
        }
        if ( !(FO.DefOpt & SystemFld) &&  (FO.FldAddInfo==aiAn))
        {
            if (HaveForUpd)
                UQ+=", ";
            else
                HaveForUpd= true;
            UQ+=FO.RealName+"=:"+FO.RealName;
            switch (FO.Type)
            {
                case aftDate:
                    UpdQ.DeclVarDate(FO.RealName);
                break;
                case aftSumma:
                    UpdQ.DeclVarSum(FO.RealName);
                break;
                case aftInt:
                case aftBool:
                    UpdQ.DeclVarInt(FO.RealName);
                break;
                case aftStr:
                    UpdQ.DeclVarStr(FO.RealName);
                break;
            }
        }
    }


    if (HaveForUpd)
    {
        UpdQ.SetSqlTextReady(UQ+" where ROWID=:CURR_ROWID; commit; end;");
        UpdQ.DeclVarStr("CURR_ROWID");
    }
    // для итого строки
    std::vector<long double> Total(TotalFlds.size());

    ODS->Active=false;
    ExecODS(ODS);
    IndexFields(ODS);
    FSM.SetDS(ODS);
        while (!ODS->Eof)
        {
            RunProg(0, 1, 0, ClrFlds);
            RunProg(1, 1, 0, false);
            // Проверка стоит ли включать строку в аналтику
            bool Incl = false;
            unsigned int i = 0;
            while (!Incl && i< InclInSum.size())
            {
                if (FSM.At(InclInSum[i])->Number())
                    Incl=true;
                else
                    i++;
            }

            if (Incl)
            {
                if (HaveForUpd)
                {
                    for (int i = 0; i< Count(); i++)
                    {
                        SFldOpt& FO = At(i);
                        if (!( FO.DefOpt & SystemFld) && (FO.FldAddInfo==aiAn) )
                        {
                            if (FO.FldNo!=-1)
                            {
                                FSM.At(i)->SetVar(UpdQ, FO.RealName);
                            }
                        }
                    }
                    AnsiString RowId=ODS->RowId();
                    UpdQ.SetVar("CURR_ROWID", RowId);
                    UpdQ.Exec("Что получилось");
                    for (unsigned int i = 0; i<TotalFlds.size(); i++)
                    {
                        int j= TotalFlds[i];
                        long double S= FSM.At(j)->Number();
                        Total[i]+=S;
                    }
                }
            }
            else
            {
               if (REQuil) {
                 AmsQuery AQ;
                 AQ.SetSqlTextReady(
                     "begin delete from "+BaseUserName+"."+AnName+
                     " where SUMID=:SUMID and AARTICLE=:AARTICLE and AVER=:AVER;"
                     "commit; end;");
                 AQ.DeclVar("AARTICLE", ODS->FieldByName("ARTICLE")->AsString);
                 AQ.DeclVar("AVER", ODS->FieldByName("VER")->AsInteger);
                 AQ.DeclVar("SUMID", AnSumEd);
                 AQ.Exec("Удаляем строку из аналитической суммы");
               }
            }
            ODS->Next();
        }// Отшагали по всем строкам аналит суммы
        //----------------------------------------------------
        // Переписываем итого строку
        AmsQuery AQ;
        AnsiString Q = "begin delete from "+ BaseUserName+"."+AnName+
            " where SUMID=:SUMID and AARTICLE =' ';"
            "insert into "+BaseUserName+"."+AnName+" (SUMID, AARTICLE, AVER";
        AnsiString Rest="";
        for (unsigned int i = 0 ; i< TotalFlds.size(); i++)
        {
            int j = TotalFlds[i];
            SFldOpt& FO = At(j);
            Q+=", "+FO.RealName;
            Rest+=", :"+FO.RealName;
            AQ.DeclVarSum(FO.RealName, Total[i]);
        }
        Q+=") values ( :SUMID, :AARTICLE, :AVER"+Rest+"); commit; end;";
        AQ.SetSqlTextReady(Q);
        AQ.DeclVar("SUMID", AnSumEd);
        AQ.DeclVar("AARTICLE", " ");
        AQ.DeclVar("AVER", 0);
        AQ.Exec("Вставляем тотал строку");
};
//---------------------------------------------------------------------------
void TFldsDescrAnDestr::RunProg(bool Post, bool SetFlds, bool ClearFlds, bool ClearF4)
{
        if (!an_prog_comp)
        {
            ProgFile pf;
            char fname[256];
            sprintf(fname,"%s\\%s.prg",GC::DocPath.c_str(),AnName.c_str());
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

        ::string set = ToString(SetName);

        try
        {
            FSM.LoadData(ip.Base, SetFlds, ClearFlds, ClearF4);
            ip.Base.Structure().Var("SET").assign(VAL(set));
            ip.Base.Structure().Var("PRE").assign(VAL((int)!Post));
            if (Vars)
            {
                ip.SetVars(Vars);
                ip.Base.AddStruct(Vars->VARS()->Stru());
            }
            an_prog.Run(ip);
        }
        catch(OraError ERR)
        {
            ERR.message(0,0,"After prog error");
        }
        ip.Leave();
        cur_arg=pre_cur_arg ;
}
//---------------------------------------------------------------------------
void TFldsDescrAnDestr::ReadDescr(const AnsiString& AnName_,const AnsiString& SetName_)
{
    SetName=SetName_;
    an_prog_comp=false;
    AnName=AnName_;
    FSM.SetFldsDescr(this);
    do
    {
        AmsQuery AQ;
        AQ.SetSqlText ("select OID, NAME from *.OBJS "
                "start with NAME = :NAME and OTYPE = 5 connect by prior PARENT = OID");
        AQ.DeclVar("NAME", AnName);
        AQ.Exec("Узнаем имя и код аналитики и БД");
        if (AQ.NotEof())
        {
            AnId = AQ.GetFldInt(0);
            AnName = AQ.GetFldStr(1);
            AQ.Next();
            DbId = AQ.GetFldInt(0);
            DbName = AQ.GetFldStr(1);
        }
        else
        {
            THROW_AMS_EXC ("Не нашел аналитики с именем "+AnName_);
        }
    }
    while (0);
    ReadFldsDescr();
};
//---------------------------------------------------------------------------
void TFldsDescrAnDestr::IndexFields(TOracleDataSet* ODS)
{
    for (int i = 0 ; i<Count(); i++)
    {
        SFldOpt& XX = At(i);
        TField* Fld = ODS->FieldByName(XX.SqlAlias);
        XX.FldNo = ODS->Fields->IndexOf(Fld);
    }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TFldsDescrDbPos::ReadDescr (const AnsiString& DbName_, const AnsiString& SetName_)
{
    AmsQuery AQ;
    AQ.SetSqlText(
        "begin select max(OID) into :OID from *.OBJS where NAME=:NAME; "
        "select max(OWNCAT) into :OWNCAT "
        "from *.DBCAT "
        "where DBID=:OID and CATID is NULL;end;");
    AQ.DeclVarInt ("OID"   , 0);
    AQ.DeclVarInt ("OWNCAT", 0);
    AQ.DeclVarStr ("NAME", DbName_);
    AQ.Exec("Получаем код корневой категории и код базы данных");
    RootCat=AQ.GetVarInt("OWNCAT");
    DbId=AQ.GetVarInt("OID");
    SetDbDescr(DbName_, DbId, SetName_);
    ReadFldsDescr();
    for ( int i = 0; i< Count(); i++)
    {
        if (
            At(i).RealName=="ARTICLE" ||
            At(i).RealName=="VER" ||
            At(i).RealName=="CATID" )
        {
            if (At(i).DefOpt&Editable)
                At(i).DefOpt=At(i).DefOpt^Editable;
        };
    }

}
//---------------------------------------------------------------------------
// Построить SQL запрос для редактирования позиции БД
void TFldsDescrDbPos::BuildSqlQuery(TOracleDataSet* ODS_)
{
    ODS_->Active = false;
    ODS_->DeleteVariables();

    AnsiString SelBeg = "select D.ROWID";
    for ( int i = 0; i< Count(); i++)
    {
        SelBeg +=", "+At(i).FullName+" "+At(i).SqlAlias;
        At(i).FldNo=i;
    }
    SelBeg +=" from ";
    ODS_->SQL->Text =
                SelBeg+BaseUserName+"."+DbName+" D "
                " where D.ARTICLE=:ARTICLE and D.VER=:VER";
    ODS_->DeclareVariable("ARTICLE", otString);
    ODS_->DeclareVariable("VER", otInteger);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
