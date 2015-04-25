//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "BRWS_ACC_WINDOW.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

AnsiString TBrwsAccWindow::Str[2][4]=
{
{  // 0
    "from "                             //  0
	    "*.OP_ITEMS OI, "
	    "*.OPERS O, "
	    "*.OP_ITEMS OI2, "
	    "*.ACCS A, "
	    "*.COR_ITEMS CI, "
	    "*.SUMS S "
    "where "
        "OI.OP_DATE>=:BEG_DATE and OI.OP_DATE<:END_DATE and "
        "OI.CODE =:CODE and " ,

    "O.OP_ID = OI.OP_ID and "           //  1
	"O.OP_SUBID = OI.OP_SUBID and "
	"O.POST = 0 and "
	"O.STATUS = 1 and "
	"OI2.OP_ID = OI.OP_ID and "
	"OI2.OP_SUBID = OI.OP_SUBID and "
	"OI2.SIDE!=OI.SIDE and "
	"OI2.CODE=A.CODE AND ",


	"CI.OP_ID=OI.OP_ID and "            // 2
	"CI.OP_SUBID = OI.OP_SUBID and ",

    "CI.ITEM_SUM_ID=S.SUM_ID"           //  3
},

{   //1
    "from "                             //  0
	    "*.OP_ITEMS OI, "
	    "*.OPERS O, "
	    "*.OP_ITEMS OI2, "
	    "*.ACCS A, "
	    "*.COR_ITEMS CI, "
	    "*.SUMS S "
    "where "
        "OI.OP_DATE>=:BEG_DATE and OI.OP_DATE<:END_DATE and "
        "OI.CODE =:CODE and " ,

    "O.OP_ID = OI.OP_ID and "           //  1
	"O.OP_SUBID = OI.OP_SUBID and "
	"O.POST = 0 and "
	"O.STATUS = 1 and "
	"OI2.OP_ID = OI.OP_ID and "
	"OI2.OP_SUBID = OI.OP_SUBID and "
	"OI2.SIDE!=OI.SIDE and "
	"OI2.CODE=A.CODE AND A.NODE=0 and "
	"CI.OP_ID=OI.OP_ID and "
	"CI.OP_SUBID = OI.OP_SUBID and ",

    "CI.ITEM_SUM_ID=S.SUM_ID"           //  2
}

};
//---------------------------------------------------------------------------
void TBrwsAccWindow::BuildSqlQuery(SBrwsAccWindowOptions& CurrOpt,
                                DS_Q& DS, AnsiString& Sql, AnsiString& Simple, bool Deep)
{
    if (CurrOpt.PerNotPlain)
    {
        if (CurrOpt.SyntOnly)
        {
            //CurrOpt.FldName="";
            //CurrOpt.FilterArticle="";
        }
    }
    bool NeedChange = true;  // Потом пересмотрим
    if (NeedChange)
    {
        // Формируем From и Where Части запроса
        AnsiString SideStr;
        AnsiString SideStrAdd;
        AnsiString AccFilter = " A.NODE = 0 and ";
        if (CurrOpt.DK==SBrwsAccWindowOptions::dkDebet)
        {
            SideStr="OI.SIDE='D' and ";
            SideStrAdd="CI.D_CODE= OI.CODE and CI.C_CODE=OI2.CODE and ";
        }
        else
        if (CurrOpt.DK==SBrwsAccWindowOptions::dkCredit)
        {
            SideStr="OI.SIDE='K' and ";
            SideStrAdd="CI.D_CODE= OI2.CODE and CI.C_CODE=OI.CODE and ";
        }
        else
        if (CurrOpt.DK==SBrwsAccWindowOptions::dkBoth)
        {
            SideStr="";
            SideStrAdd="("
            "(OI.SIDE='D' and CI.D_CODE= OI.CODE and CI.C_CODE=OI2.CODE) or "
            "(OI.SIDE='K' and CI.D_CODE=OI2.CODE and CI.C_CODE=OI.CODE)"
            ") and ";
        }

        if (CurrOpt.AccsTreeDepth!=-1)
        {
            AccFilter=" A.LEV="+IntToStr(CurrOpt.AccsTreeDepth)+" and ";
        }
        AnsiString FromWhere=
            Str[0][0]+ SideStr+ Str[0][1]+AccFilter+Str[0][2]+
                        SideStrAdd+ Str[0][3];

        AmsQuery AQ;
        if (CurrOpt.DetailedCorresp)
        {   // Субсчета в корреспонденции
            // Находим все субсчета, которые будут в нашем запросе
            AQ.Clear ();
            //                                0        1       2
            AQ.SetSqlText("select distinct OI.SIDE, A.CODE, A.NAME "+FromWhere+
                        " order by OI.SIDE, A.NAME");
            AQ.DeclVar("CODE", CurrOpt.AccCode);
            AQ.DeclVar("BEG_DATE", CurrOpt.DateBeg);
            AQ.DeclVar("END_DATE", CurrOpt.DateEnd);
            AQ.Exec("Выбираем все счета которые участвуют в оборотах");
            SelAcc.clear();
            SAccs S;
            while (AQ.NotEof())
            {
                S.Debet=AQ.GetFldStr(0)=="D";
                S.Code=AQ.GetFldInt(1);
                S.Name=(S.Debet?AnsiString(""):AnsiString("K"))+TAccName::ToUserName(AQ.GetFldStr(2));
                SelAcc.push_back(S);
                AQ.Next();
            };

            if ((SelAcc.size()==0)&&(CurrOpt.AccsTreeDepth!=-1))
            {
                THROW_AMS_EXC ("Нет счетов с таким уровнем вложения");
            };

            // Группируем по датам
            if (CurrOpt.ByDate)
            {
                Simple="select OP_DATE";
                Sql="select O.OP_DATE";
                if (CurrOpt.DK & SBrwsAccWindowOptions::dkDebet)
                {
                    Sql+=", sum (*.GetAccCorTotValue('D', OI.SIDE, O.STORNO, S.VALUE)) DSUM";
                    Simple+=", 0.0 DSUM";
                }
                unsigned int i=0;
                while (i<SelAcc.size() && SelAcc[i].Debet)
                {
                    Sql+=", sum (*.GetAccCorValue ("+IntToStr(SelAcc[i].Code)+
                          ", A.CODE, 'D', OI.SIDE, O.STORNO, S.VALUE)) \""+SelAcc[i].Name+"\"";
                    Simple+=", 0.0 \""+SelAcc[i].Name+"\"";
                    i++;
                }
                while (i<SelAcc.size())
                {
                    Sql+=", sum (*.GetAccCorValue ("+IntToStr(SelAcc[i].Code)+
                          ", A.CODE, 'K', OI.SIDE, O.STORNO, S.VALUE)) \""+SelAcc[i].Name+"\"";
                    Simple+=", 0.0 \""+SelAcc[i].Name+"\"";
                    i++;
                }
                if (CurrOpt.DK & SBrwsAccWindowOptions::dkCredit)
                {
                    Sql+=", sum (*.GetAccCorTotValue ('K', OI.SIDE, O.STORNO, S.VALUE)) CSUM ";
                    Simple+=", 0.0 CSUM ";
                }
                Sql+=" "+FromWhere+CurrOpt.AddWhere+" group by O.OP_DATE ";
            }
            else
            {   // Нет группировки по датам /
                AnsiString AnalAdd;
                AnsiString An;
                if (CurrOpt.Analitic)
                {
                    AnalAdd="max(*.GetAnalInfo(OI.ITEM_SUM_ID, :AN_CODE)) ANAL,";
                    An="' ' ANAL,";
                    DS.DeclVar("AN_CODE", CurrOpt.AnCode);
                }
                Simple="select OP_ID, OP_SUBID,"+An+" ITEM_SUM_ID, SETNAME, 0.0 storno, "
                " Text, OP_DATE, DOC_TYPE, DOC_NUM ";
                Sql="select O.ROWID, O.OP_ID, O.OP_SUBID,"+AnalAdd+" max(OI.ITEM_SUM_ID) ITEM_SUM_ID,"
                " max(OI.SETNAME) SETNAME, "
                    " max(O.STORNO) STORNO, max(O.TEXT) TEXT, "
                    "max (O.OP_DATE) OP_DATE, max(O.DOC_TYPE) DOC_TYPE, max(O.DOC_NUM) DOC_NUM ";
                if (CurrOpt.DK & SBrwsAccWindowOptions::dkDebet)
                {
                    Sql+=", sum (*.GetAccCorTotValue('D', OI.SIDE, O.STORNO, S.VALUE)) DSUM ";
                    Simple+=", 0.0 DSUM ";
                }
                unsigned int i=0;
                while (i<SelAcc.size() && SelAcc[i].Debet)
                {
                    Sql+=", sum (*.GetAccCorValue ("+IntToStr(SelAcc[i].Code)+
                          ", A.CODE, 'D', OI.SIDE, O.STORNO, S.VALUE)) \""+SelAcc[i].Name+"\"";
                    Simple+=", 0.0 \""+SelAcc[i].Name+"\"";
                    i++;
                }
                while (i<SelAcc.size())
                {
                    Sql+=", sum (*.GetAccCorValue ("+IntToStr(SelAcc[i].Code)+
                          ", A.CODE, 'K', OI.SIDE, O.STORNO, S.VALUE)) \""+SelAcc[i].Name+"\"";
                    Simple+=", 0.0 \""+SelAcc[i].Name+"\"";
                    i++;
                }
                if (CurrOpt.DK & SBrwsAccWindowOptions::dkCredit)
                {
                    Sql+=", sum (*.GetAccCorTotValue ('K', OI.SIDE, O.STORNO, S.VALUE)) CSUM ";
                    Simple+=", 0.0 CSUM";
                }
                Sql+=" "+FromWhere+CurrOpt.AddWhere+" group by O.ROWID, O.OP_ID, O.OP_SUBID order by max(O.OP_DATE), O.OP_ID, O.OP_SUBID ";
            }
        }
        else
        {   // Нет развернутой корреспонденции счетов
            AnsiString DKAddWhere="";
            if (CurrOpt.DK!=SBrwsAccWindowOptions::dkBoth)
            {
                DKAddWhere=" OI.SIDE=:SIDE and ";
                DS.DeclVar("SIDE", CurrOpt.DK==SBrwsAccWindowOptions::dkDebet? "D":"K");
            }

            if (CurrOpt.ByDate)
            {// Группируем по датам

                Simple="select OP_DATE, 0.0 DSUM, 0.0 CSUM ";

                if (CurrOpt.FilterArticle=="" &&
                        (CurrOpt.FldName==""||CurrOpt.FldName==CurrOpt.BalFld))
                {
                    Sql="select O.OP_DATE"
                        ", sum (*.GetAccCorTotValue('D', OI.SIDE, O.STORNO, S.VALUE)) DSUM"
                        ", sum (*.GetAccCorTotValue('K', OI.SIDE, O.STORNO, S.VALUE)) CSUM";


                    Sql+=
                    " from "
                    "    *.OP_ITEMS OI, "
                    "    *.OPERS O, "
                    "    *.SUMS S "
                    "where "
                    "    OI.OP_DATE>= :BEG_DATE and OI.OP_DATE<:END_DATE and "
                    "    OI.CODE =:CODE and "+DKAddWhere+
                    "    O.OP_ID = OI.OP_ID and "
                    "    O.OP_SUBID = OI.OP_SUBID and "
                    "    O.POST = 0 and "
                    "    O.STATUS = 1 "+ CurrOpt.AddWhere+" and "
                    "    OI.ITEM_SUM_ID=S.SUM_ID "
                    "group by O.OP_DATE ";
                }
                else // Группируем по датам вместе с фильтром
                {
                    AnsiString Art=CurrOpt.FilterArticle;
                    int Ver= CurrOpt.FilterVer;
                    if (Art=="")
                    {
                        Art= " " ;
                        Ver=0;
                    }

                    AnsiString FldAdd=" max(";
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
                            if (AQ.GetFldInt(0) & Addable) FldAdd="sum(";
                        }
                    } while (0);

                    AnsiString AnGlue;
                    if (CurrOpt.WithoutVer)
                    {
                        AnGlue= " and AN.AARTICLE=:ARTICLE ";
                        DS.DeclVar("ARTICLE", Art);
                    }
                    else
                    {
                        AnGlue= " and AN.AARTICLE=:ARTICLE and AN.AVER=:VER ";
                        DS.DeclVar("ARTICLE", Art);
                        DS.DeclVar("VER", Ver);
                    }


                    Sql="select O.OP_DATE"
                        ", "+FldAdd+"*.GetAccCorTotValue('D', OI.SIDE, O.STORNO, AN."
                                +CurrOpt.FldName+")) DSUM"
                        ", "+FldAdd+"*.GetAccCorTotValue('K', OI.SIDE, O.STORNO, AN."
                                +CurrOpt.FldName+")) CSUM";
                    Sql+=
                    " from "
                    "    *.OP_ITEMS OI, "
                    "    *.OPERS O, "
                    "    *."+CurrOpt.AnName+" AN "
                    "where "
                    "    OI.OP_DATE>= :BEG_DATE and OI.OP_DATE<:END_DATE and "
                    "    OI.CODE =:CODE and "+DKAddWhere+
                    "    O.OP_ID = OI.OP_ID and "
                    "    O.OP_SUBID = OI.OP_SUBID and "
                    "    O.POST = 0 and "
                    "    O.STATUS = 1 "+ CurrOpt.AddWhere+" and "
                    "    OI.ITEM_SUM_ID=AN.SUMID "+AnGlue+
                    "group by O.OP_DATE ";
                }
            }
            else
            {   // Нет группировки по датам
                AnsiString AnalAdd;
                AnsiString An;
                if (CurrOpt.Analitic)
                {
                    AnalAdd="*.GetAnalInfo(OI.ITEM_SUM_ID, :AN_CODE) ANAL,";
                    DS.DeclVar("AN_CODE", CurrOpt.AnCode);
                    An=" ' ' ANAL, ";
                }

                if ( CurrOpt.AnName!="" && CurrOpt.FldName!="" )
                {   // Выбрано конкретное поле
                    if (CurrOpt.WithoutVer && CurrOpt.FilterArticle!="")
                    {
                        AmsQuery AQ;
                        // Узнаем поле СУММИРУЕМОЕ?
                        AQ.SetSqlText(
                            "select DEF_OPT from *.DBSTR where DBID=:DBID and REALNAME=:REALNAME");
                        AQ.DeclVar("DBID", CurrOpt.AnCode);
                        AQ.DeclVar("REALNAME", CurrOpt.FldName);
                        AQ.Exec("Находим тип поля");
                        AnsiString FldAdd=" max(";
                        if (AQ.NotEof())
                        {
                            if (AQ.GetFldInt(0) & Addable) FldAdd="sum(";
                        }
                        Simple="select OP_ID, OP_SUBID, ITEM_SUM_ID, SETNAME, "
                            " 0.01 as STORNO, SIDE, TEXT, OP_DATE,"
                            " DOC_TYPE, DOC_NUM, 0.0 DSUM, 0.0 CSUM ";
                        Sql="select * from (select O.OP_ID, O.OP_SUBID, "
                            " max(OI.ITEM_SUM_ID) ITEM_SUM_ID, max(OI.SETNAME) SETNAME, "
                            "max(O.STORNO) STORNO, max(OI.SIDE) SIDE, max(O.TEXT) TEXT, "
                            "max(O.OP_DATE) OP_DATE,"
                            "max(O.DOC_TYPE) DOC_TYPE, max(O.DOC_NUM) DOC_NUM, "
                            +FldAdd+"*.GetAccDKValue('D',OI.SIDE,AN."+CurrOpt.FldName+")) DSUM, "
                            +FldAdd+"*.GetAccDKValue('K',OI.SIDE,AN."+CurrOpt.FldName+")) CSUM "
                        "from "
                            "*.OP_ITEMS OI, "
                            "*.OPERS O, "
                            "*."+CurrOpt.AnName+" AN "
                        "where "
                            "OI.OP_DATE>= :BEG_DATE and OI.OP_DATE<:END_DATE and "
                            "OI.CODE =:CODE and " + DKAddWhere +
                            "O.OP_ID = OI.OP_ID and "
                            "O.OP_SUBID = OI.OP_SUBID and "
                            "O.POST = 0 and "
                            "O.STATUS = 1 and "
                            "OI.ITEM_SUM_ID=AN.SUMID and "
                            "AN.AARTICLE=:ARTICLE "+CurrOpt.AddWhere+
                            " GROUP BY O.OP_ID, O.OP_SUBID) S "
                            "order by OP_DATE, OP_ID, OP_SUBID";
                        DS.DeclVar("ARTICLE", CurrOpt.FilterArticle);
                    }
                    else
                    {
                        Simple="select OP_ID, OP_SUBID, ITEM_SUM_ID, SETNAME, "
                            "STORNO, SIDE, TEXT, OP_DATE,"
                            "DOC_TYPE, DOC_NUM,"+An+" 0.0 DSUM, 0.0 CSUM ";
                        Sql="select O.ROWID, O.OP_ID, O.OP_SUBID, OI.ITEM_SUM_ID, OI.SETNAME, "
                            "O.STORNO, OI.SIDE, O.TEXT, O.OP_DATE,"
                            " O.DOC_TYPE, O.DOC_NUM, "+AnalAdd+
                            "*.GetAccDKValue('D',OI.SIDE,AN."+CurrOpt.FldName+") DSUM, "
                            "*.GetAccDKValue('K',OI.SIDE,AN."+CurrOpt.FldName+")     CSUM "
                        "from "
                            "*.OP_ITEMS OI, "
                            "*.OPERS O, "
                            "*."+CurrOpt.AnName+" AN "
                        "where "
                            "OI.OP_DATE>= :BEG_DATE and OI.OP_DATE<:END_DATE and "
                            "OI.CODE =:CODE and " + DKAddWhere +
                            "O.OP_ID = OI.OP_ID and "
                            "O.OP_SUBID = OI.OP_SUBID and "
                            "O.POST = 0 and "
                            "O.STATUS = 1 and "
                            "OI.ITEM_SUM_ID=AN.SUMID and "
                            "AN.AARTICLE=:ARTICLE and AN.AVER=:VER "+CurrOpt.AddWhere;
                        if (CurrOpt.FilterArticle!="")
                        {
                            DS.DeclVar("ARTICLE", CurrOpt.FilterArticle);
                            DS.DeclVar("VER", CurrOpt.FilterVer);
                        }
                        else
                        {
                            DS.DeclVar("ARTICLE", " ");
                            DS.DeclVar("VER", 0);
                        }
                    }
                }
                else // Конкретное поле не выбрано
                {
                    Simple="select OP_ID, OP_SUBID, ITEM_SUM_ID, SETNAME, "
                        " STORNO, SIDE,  TEXT, OP_DATE,"
                        " DOC_TYPE, DOC_NUM,"+An+"0.0 DSUM, 0.0 CSUM ";
                    Sql="select O.ROWID, O.OP_ID, O.OP_SUBID, OI.ITEM_SUM_ID, OI.SETNAME, "
                        "O.STORNO, OI.SIDE, O.TEXT, O.OP_DATE, "
                            "O.DOC_TYPE, O.DOC_NUM,"+AnalAdd+
                        "*.GetAccDKValue('D', OI.SIDE, S.VALUE) DSUM, "
                        "*.GetAccDKValue('K', OI.SIDE, S.VALUE) CSUM "
                        "from "
                        "    *.OP_ITEMS OI,"
                        "    *.OPERS O,"
                        "    *.SUMS S "
                        "where"
                        "    OI.OP_DATE>=:BEG_DATE and OI.OP_DATE<:END_DATE and "
                        "    OI.CODE =:CODE and " + DKAddWhere +
                        "    O.OP_ID = OI.OP_ID and "
                        "    O.OP_SUBID = OI.OP_SUBID and "
                        "    O.POST = 0 and "
                        "    O.STATUS = 1 and "
                        "    OI.ITEM_SUM_ID=S.SUM_ID "+CurrOpt.AddWhere ;
                }
                if (!CurrOpt.WithoutVer) Sql+=CurrOpt.OrderBy;
            }
        }
    }

//---------------------------- CHANGED AS ANSWER ON QUESTION #3 MADE 13.03.02
    AnsiString Strx=Sql;
    AnsiString AnGluex;
    AnsiString Artx=CurrOpt.FilterArticle;
    int Verx= CurrOpt.FilterVer;
    if (CurrOpt.FilterArticle!="" && CurrOpt.DetailedCorresp){
      if (CurrOpt.WithoutVer)
        {
            AnGluex= " and AN.AARTICLE=:ARTICLE ";
            DS.DeclVar("ARTICLE", Artx);
        }
        else
        {
            AnGluex= " and AN.AARTICLE=:ARTICLE and AN.AVER=:VER ";
            DS.DeclVar("ARTICLE", Artx);
            DS.DeclVar("VER", Verx);
        }

      AnsiString BeforeWhere=Strx.SubString(1, Strx.Pos("where")-1);
      AnsiString AfterWhere=Strx.SubString(Strx.Pos("where"), Strx.Length());
      Strx=BeforeWhere+" ,*."+CurrOpt.AnName+" AN "+AfterWhere;

      AnsiString BeforeGroup=Strx.SubString(1, Strx.Pos("group by")-1);
      AnsiString AfterGroup=Strx.SubString(Strx.Pos("group by"), Strx.Length());
      Strx=BeforeGroup+" and OI.ITEM_SUM_ID=AN.SUMID "+AnGluex+AfterGroup;
    }
    Sql=Strx;

//-----------------------------EO CHANGED AS ANSWER ON QUESTION #3 MADE 13.03.02





    DS.DeclVar("BEG_DATE", CurrOpt.DateBeg);
    DS.DeclVar("END_DATE", CurrOpt.DateEnd);
    DS.DeclVar("CODE"    , CurrOpt.AccCode);
    PrevOpt = CurrOpt;
};
//---------------------------------------------------------------------------
void TBrwsAccWindow::BuildRemQuery(DS_Q& DS, AnsiString& Sql,int& DtAcc, int& CtAcc)
{
    unsigned int i=0;

    AnsiString SideStr;
    AnsiString SideStrAdd;
    if (PrevOpt.DK==SBrwsAccWindowOptions::dkDebet)
    {
        SideStr="OI.SIDE='D' and ";
        SideStrAdd="CI.D_CODE= OI.CODE and CI.C_CODE=OI2.CODE and ";
    }
    else
    if (PrevOpt.DK==SBrwsAccWindowOptions::dkCredit)
    {
        SideStr="OI.SIDE='K' and ";
        SideStrAdd="CI.D_CODE= OI2.CODE and CI.C_CODE=OI.CODE and ";
    }
    else
    if (PrevOpt.DK==SBrwsAccWindowOptions::dkBoth)
    {
        SideStr="";
        SideStrAdd="("
        "(OI.SIDE='D' and CI.D_CODE= OI.CODE and CI.C_CODE=OI2.CODE) or "
        "(OI.SIDE='K' and CI.D_CODE=OI2.CODE and CI.C_CODE=OI.CODE)"
        ") and ";
    }

    Sql = "select ";
    i=0;
    while (i<SelAcc.size() && SelAcc[i].Debet)
    {
        if (i) Sql+=", ";
        Sql+="sum (*.GetAccCorValue ("+IntToStr(SelAcc[i].Code)+
              ", A.CODE, 'D', OI.SIDE, O.STORNO, S.VALUE)) \""+SelAcc[i].Name+"\"";
        i++;
    }
    DtAcc = i;
    while (i<SelAcc.size())
    {
        if (i) Sql+=", ";
        Sql+="sum (*.GetAccCorValue ("+IntToStr(SelAcc[i].Code)+
              ", A.CODE, 'K', OI.SIDE, O.STORNO, S.VALUE)) \""+SelAcc[i].Name+"\"";
        i++;
    }
    CtAcc = SelAcc.size()-DtAcc;

    AnsiString AccFilter=" A.NODE=0 and ";
    if (PrevOpt.AccsTreeDepth!=-1)
    {
        AccFilter=" A.LEV="+IntToStr(PrevOpt.AccsTreeDepth)+" and ";
    }
    Sql+=" "+Str[0][0]+ SideStr+ Str[0][1]+AccFilter+Str[0][2]+
                        SideStrAdd+ Str[0][3];

    DS.DeclVar("BEG_DATE", PrevOpt.DateBeg);
    DS.DeclVar("END_DATE", PrevOpt.DateEnd);
    DS.DeclVar("CODE"    , PrevOpt.AccCode);
}
//---------------------------------------------------------------------------

