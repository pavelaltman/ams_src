//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "FLDS_DESCR.h"
#include "AMS_READ_CNF.h"
SFldOptAdd EmptyOpt;

//---------------------------------------------------------------------------
#pragma package(smart_init)

void TFldsDescr::RebuildDbRef()
{
    FRefs.clear();
    for (unsigned int i = 0; i< FOpt.size(); i++)
    {
        if (FOpt[i].DBId >0)
        {
            SRefs Ref;
            Ref.MainFld = i;
            Ref.SetName = FOpt[i].SetName;
            if(FOpt[i].Type == 5) // Ссылка на БД
            {
                SFldOpt* FO = & FOpt[i];
                AnsiString VerName = FO->RealName+"_V";
                unsigned int j= 0;
                while (j < FOpt.size() && FOpt[j].RealName!= VerName) j++;
                if (j<FOpt.size())
                {
                    Ref.VerFld = j;
                    FRefs[i]=Ref;
                }
                else
                {
                    THROW_AMS_EXC("Не смог найти поле-версию "+VerName);
                }
            }
        }
    }
};
//---------------------------------------------------------------------------
void TFldsDescr::GetCommaText (AnsiString& Res)
{
    bool FirstTime = true;
    for (int i = 0 ; i< Count(); i++)
    {
        if (FirstTime)
            FirstTime = false;
        else
            Res+=", ";
        Res+=FOpt[i].RealName;
    };
}
//---------------------------------------------------------------------------
void TFldsDescr::PrepareOQ (AmsQuery& AQ, bool An)
{
    AQ.SetSqlTextReady(
    //            0          1       2         3         4        5
    " select S.REALNAME, S.VTYPE, S.RDBID, S.MAXLEN, S.ALIAS, S.WIDTH, "
    //                              6                 7
    " nvl (UO.DISP_OPT, S.DISP_OPT) DispOpt, nvl(O.OPT, S.DEF_OPT ) Opt,"
    //   8         9                     10              11
    " S.DEC, C.COMMENTS, nvl(S.LINES,0) Lines, S.DB_SetName "  //:SETNAME SetName "
    " from "+BaseUserName+".DBSTR S, "+BaseUserName+".DBOPTOV O, "+
    BaseUserName+".UDBOPT UO, ALL_COL_COMMENTS C"
    " where "
        " S.DBID = :DBID and "
        " S.DELETED = 0 and "
        " UO.DBID     (+)= S.DBID and "
        " UO.REALNAME (+)= S.REALNAME and "
        " UO.SETNAME  (+)= :SETNAME and "
        " UO.USERNAME (+)= USER and "
        " O.DBID(+) = :DBID and "
        " O.SETNAME (+) = :SETNAME and "
        " O.REALNAME (+) = S.REALNAME and "
        " C.OWNER (+)= :OWNER and "
        " C.TABLE_NAME (+)= :TABLE_NAME and "
        " C.COLUMN_NAME (+)= S.REALNAME "
    " order by ORD ");
    AQ.DeclVar("SETNAME", SetName);
    AQ.DeclVar("OWNER", BaseUserName);
    if (!An)
    {
        AQ.DeclVar("DBID", DbId);
        AQ.DeclVar("TABLE_NAME", DbName);
    }
    else
    {
        AQ.DeclVar("DBID", AnId);
        AQ.DeclVar("TABLE_NAME", AnName);
    }
};
//---------------------------------------------------------------------------
void TFldsDescr::FillFO(AmsQuery& AQ, SFldOpt& X, const AnsiString& AddFullName)
{
        //            0          1       2         3         4        5
        // select S.REALNAME, S.VTYPE, S.RDBID, S.MAXLEN, S.ALIAS, S.WIDTH, "
        //                              6                 7         8
        // nvl (UO.DISP_OPT, S.DISP_OPT) DispOpt, S.DEF_OPT Opt, S.DEC "
        X.RealName= AQ()->FieldAsString(0).UpperCase();
        X.Type = AQ()->FieldAsInteger(1);
        X.DBId= AQ()->FieldIsNull(2)? -1: AQ()->FieldAsInteger(2);
        X.FldMaxWidth= AQ()->FieldAsInteger(3);
        X.Alias     = AQ()->FieldAsString (4);
        X.DiplWidth = AQ()->FieldAsInteger(5);
        X.DispOpt   = AQ()->FieldAsInteger(6);
        X.DefOpt    = AQ()->FieldAsInteger(7);
        X.MaxPoints = AQ()->FieldAsInteger(8);
        X.Hint      = AQ()->FieldAsString(9);
        X.Lines     = AQ()->FieldAsInteger(10);
        X.SetName   = AQ()->FieldAsString (11);
        X.FldNo=-1;
        if (AddFullName!="D")
            X.SqlAlias   = AddFullName+"_"+X.RealName;
        else
            X.SqlAlias   = X.RealName;

        X.FullName = AddFullName+"."+X.RealName;
};
//---------------------------------------------------------------------------
void TFldsDescr::ReadDbDescr()
{
    AmsQuery AQ;
    if (SetName =="") SetName =" " ;
    PrepareOQ(AQ, false);
    AQ.Exec("Выбираем поля и установки к БД");
    Clear();
    SFldOpt X;
    X.FldAddInfo=aiDb;
    while (AQ.NotEof())
    {
        FillFO (AQ, X, "D");
        Add(X);
        AQ.Next();
    }
}
//---------------------------------------------------------------------------
void TFldsDescr::ReadAnDescr()
{
    //
    AmsQuery AQ;
    {
        // У нас не существует запией в AN_FLD_ORDER => поля следуют как обычно
        ReadDbDescr();
        AQ.Clear();
        std::vector<SFldOpt> TmpAn;  // Считаем все аналитические поля
        SFldOpt X;
        X.FldAddInfo=aiAn;
        X.FldNo =-1;
        PrepareOQ(AQ, true);
        AQ.Exec("Выбираем поля и установки аналитики");
        while (AQ.NotEof())
        {
            FillFO (AQ, X, "AN");
            TmpAn.push_back(X);
            AQ.Next();
        }

        // Определяем "остаточные" поля
        X.DispOpt= 0;
        X.FldAddInfo=aiAnR;
        for (unsigned i=0;i<TmpAn.size();i++)
        {
            SFldOpt& XX = TmpAn[i];
            if ( XX.DefOpt & RemFld )
            {
                X.DefOpt = XX.DefOpt;
                X.Alias = "О"+XX.Alias;
                X.RealName = "R"+XX.RealName;
                X.FullName = "RAN."+XX.RealName;
                X.SqlAlias="R"+XX.SqlAlias;
                X.Type = XX.Type;
                X.FldMaxWidth = XX.FldMaxWidth;
                X.MaxPoints= XX.MaxPoints;
                X.DiplWidth = XX.DiplWidth;
                Add(X);
            }
        }

        // Вставляем дополнительные поля
        AQ.Clear();
        //                        0       1    2        3       4
        AQ.SetSqlText("select REALNAME, NAME, CODE, TURNTYPE, ALIAS "
            "from *.DBADDINFO "
            "where "
                "DBID=:DBID and "
                "SETNAME=:SETNAME "
            "order by REALNAME, NAME");
        AQ.DeclVar("DBID", AnId);
        AQ.DeclVar("SETNAME", SetName);
        X.FldAddInfo=aiAdd;
        X.DispOpt= 0;
        AQ.Exec("Дополнительные поля");
        while (AQ.NotEof())
        {
            // Находим родительское поле
            AnsiString S =AQ.GetFldStr(0);
            unsigned i = 0;
            bool Find = false;
            while (i< TmpAn.size() && !Find)
            {
                SFldOpt& A = TmpAn[i];
                if (A.RealName == S)
                    Find = true;
                else i++;
            }
            if (Find)
            {
                SFldOpt& A = TmpAn[i];
                X.DefOpt = A.DefOpt;
                X.RealName = A.RealName;
                X.FullName = AQ.GetFldStr(3)+IntToStr(AQ.GetFldInt(2))+"."+X.RealName;
                X.Alias = AQ.GetFldStr(4);
                X.SqlAlias= AQ.GetFldStr(1);
                X.AddOpt.SubAccCode = AQ.GetFldInt(2);
                X.AddOpt.ParentFldName = A.RealName;
                X.AddOpt.TurnType = AQ.GetFldStr(3);
                FOpt.push_back(X);
            }
            AQ.Next();
        }
        //-----------------------------------------------------------------
        FOpt.insert(FOpt.end(), TmpAn.begin(), TmpAn.end());
    }
    AQ.Clear();
    AQ.SetSqlText("SELECT max(1) FROM *.AN_FLD_ORDER where ANID=:ANID and SETNAME=:SETNAME");
    AQ.DeclVar("ANID", AnId);
    AQ.DeclVar("SETNAME", SetName);
    AQ.Exec("");
    if (!AQ.IsFldNull())
    {
        //
        AQ.SetSqlText("SELECT NAME FROM *.AN_FLD_ORDER "
            "where ANID=:ANID and SETNAME=:SETNAME order by FLD_ORD");
        AQ.Exec("");
        SFldOpt TmpSave;
        int j = 0;
        while (AQ.NotEof())
        {
            AnsiString S=AQ.GetFldStr(0);
            int i = j;
            while (i<Count() && FOpt[i].SqlAlias!= S) i++;
            if (i<Count())
            {
                if (i != j)
                {
                    TmpSave=FOpt[i];FOpt[i]=FOpt[j];FOpt[j]=TmpSave;
                }
                j++;
            }
            else
            {
                ErrorMessage("Поле "+S+" не найдено");
            }
            AQ.Next();
        }
    }
    BuildAddSums();
}
//---------------------------------------------------------------------------
void TFldsDescr::ReadFldsDescr()
{
    NullVerChecked=false;
    Clear();
    if (AnId == -1)
    {
        ReadDbDescr();
        RebuildDbRef();
    }
    else
    {
        ReadAnDescr();
        RebuildDbRef();
    }
}
//---------------------------------------------------------------------------
bool TFldsDescr::IsNullRule()
{
    if (!NullVerChecked)
    {
        AmsQuery AQ;
        AQ.SetSqlTextReady(
        "select POPTS from "+BaseUserName+".OBJS where OID=:OID");
        AQ.DeclVarInt("OID", DbId);
        AQ.Exec("Существует ли установка нулевых версий для БД");
        NullVerRule=false;
        if (AQ.NotEof())
        {
            NullVerRule= AQ.GetFldInt(0) & DBO_ZEROVER;
        }
        NullVerChecked = true;

    }
    return NullVerRule;
}
//---------------------------------------------------------------------------
AnsiString TFldsDescr::GetAsString (AmsQuery& AQ, int FldIndx)
{
    AnsiString Res;
    try
    {
        SFldOpt& FO = At(FldIndx);
        switch( FO.Type)
        {
            case aftInt:
            case aftBool:
            {
                int T = AQ.GetFldInt(FO.RealName);
                Res=IntToStr(T);
            }
            break;
            case  aftDate:
            {
                TDateTime T = AQ.GetFldDate(FO.RealName);
                Res=DateToStr(T);
            }
            break;
            case aftSumma:
            {
                double T = AQ.GetFldSum(FO.RealName);
                Res=FloatToStr(T);
            }
            break;
            case aftStr:
                Res=AQ.GetFldStr(FO.RealName);
            break;
        }
    }
    catch(...)
    {}
    return Res;
};
//---------------------------------------------------------------------------
void TFldsDescr::BuildAddSums ()
{
    AddFldsDescr.clear();
    SAddAnSumDescr SD;
    // Формируем только уникальные суммы
    for (int i=0 ; i< Count(); i++)
    {
        SFldOpt& FO = At(i);
        if (FO.FldAddInfo == aiAdd)
        {
            SD.TurnType = FO.AddOpt.TurnType;
            SD.SubAccCode = FO.AddOpt.SubAccCode;
            // Ищем совпадающие обороты
            unsigned int j = 0;
            while (j< AddFldsDescr.size() && !(SD==AddFldsDescr[j])) j++;

            if (j<AddFldsDescr.size())
                FO.AddOpt.AnSumIndx = j; // Нашли такую аналит сумму
            else
            {   // Необходимо формировать новую
                FO.AddOpt.AnSumIndx = AddFldsDescr.size();
                AddFldsDescr.push_back (SD);
            }

        }
    }
}
//---------------------------------------------------------------------------
void TFldsDescr::FormAddAnSums( std::vector<int>*  AddAnSums_)
{
    // Узнаем коды аналитических сумм
    if (AddAnSums_)
    {
        std::vector<int>& AddAnSums=*AddAnSums_;
        AmsQuery AQ;
        AQ.DeclVarInt("RES");
        AQ.DeclVarInt("CODE");
        for (unsigned int i=0; i<AddFldsDescr.size(); i++)
        {
            SAddAnSumDescr SD = AddFldsDescr[i];
            AQ.SetSqlTextReady("begin :RES:="+BaseUserName+"."+SD.TurnType+"I (:CODE);end;");
            AQ.SetVar("CODE", SD.SubAccCode);
            AQ.Exec("Узнем аналитические суммы");
            int Res = AQ.GetVarInt("RES");
            AddAnSums.push_back(Res);
        }
    }
}
//---------------------------------------------------------------------------

