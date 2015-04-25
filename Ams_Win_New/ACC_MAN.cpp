//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "ACC_MAN.h"
extern const char* unknownfuncerr;

//---------------------------------------------------------------------------

#pragma package(smart_init)

void SetVal (Struct* S, const string& ParName, int Val)
{
    Node* N=S->FindVar(ParName);
    if (!N)
        N=S->AddVar(new Node(ParName));
    N->assign(VAL(Val));
}
//---------------------------------------------------------------------------
AccMan::AccMan( const DateTime& DateBeg_, const DateTime& DateEnd_,
                const string& WhereAdd_, Struct* S)
{
    DateBeg=DateBeg_;
    DateEnd=DateEnd_;
    WhereAdd= WhereAdd_;
    CondValues=S;
    AnCode=0;
    AnName="";
    DbName="";
    ResSumBR=0;
    ResSumER=0;
    ResSumDT=0;
    ResSumCT=0;
    ResSumCorDT=0;
    ResSumCorCT=0;
    MapCount=0;
};
//---------------------------------------------------------------------------
void AccMan::ReadAccs (int pc,StackItem *stk, std::vector<string>& Vals, int& CurrPos)
{
    bool FindedEnd = false;
    while (!FindedEnd && CurrPos<=pc)
    {
        string UserAcc = stk[CurrPos-pc].String();
        if ((UserAcc[0] >='0' && UserAcc[0]<='9')||
            (UserAcc[0]=='Z'||UserAcc[0]=='B'||UserAcc[0]=='A'))
        {
            char accname[40] ;
            strcpy(accname,UserAcc.data()) ;
            AccRec::Normalize(accname) ;
            string NormAcc=accname;
            if (!NamesToAccs.count(NormAcc))
            {
                char Tmp[10];
                itoa (++MapCount, Tmp, 10);
                NamesToAccs[NormAcc]= Tmp;
            }
            unsigned int i=0;
            while (i<Vals.size() && Vals[i]!=NormAcc) i++;
            if (i>=Vals.size())
                Vals.push_back(NormAcc);
            CurrPos++;
        }
        else
        {
            FindedEnd=true;
        }
    }
};
//------------------------------------------------------------------------------
StackItem AccMan::Method(Node &node,::string const &method,int pc,StackItem *stk)
{
    if(method=="COND" && pc>0 && pc<3)
    {
        WhereAdd=stk[1-pc].String();
        if (pc==2)
        {
            CondValues=stk[2-pc].Stru();
        }
        return StackItem(1);
    }
    if(method=="SETACCBREM" && pc>=1)
    {
        int CurrPos=1;
        ReadAccs(pc, stk, BRNames, CurrPos);
        return StackItem();
    }
    if(method=="SETACCEREM" && pc>=1)
    {
        int CurrPos=1;
        ReadAccs(pc, stk, ERNames, CurrPos);
        return StackItem();
    }

    if(method=="SETACCDTURN" && pc>=1)
    {
        int CurrPos=1;
        ReadAccs(pc, stk, DTNames, CurrPos);
        return StackItem();
    }
    if(method=="SETCONDDTURN" && pc==1)
    {
        CondDt=stk[1-pc].String();
        if (pc==2)
        {
            CondValues=stk[2-pc].Stru();
        }
        return StackItem(1);
    }
    if(method=="SETACCCTURN" && pc>=1)
    {
        int CurrPos=1;
        ReadAccs(pc, stk, CTNames, CurrPos);
        return StackItem();
    }
    if(method=="SETCONDCTURN" && pc==1)
    {
        CondCt=stk[1-pc].String();
        if (pc==2)
        {
            CondValues=stk[2-pc].Stru();
        }
        return StackItem(1);
    }
    if(method=="SETACCCORDTURN" && pc>=1)
    {
        int CurrPos=1;
        ReadAccs(pc, stk, CorDTNames, CurrPos);
        if (pc>=CurrPos)
        {
            string NextWord =stk[CurrPos-pc].String();
            if (!strcmpi(NextWord.data(),"cor"))
            {
                CurrPos++;
                ReadAccs(pc, stk, CorDTNamesC, CurrPos);
            }
        }
        if (pc>=CurrPos)
        {
            string NextWord =stk[CurrPos-pc].String();
            if (!strcmpi(NextWord.data(),"corNOT"))
            {
                CurrPos++;
                ReadAccs(pc, stk, CorDTNamesNot, CurrPos);
            }
        }
        return StackItem();
    }

    if(method=="SETCONDCORDTURN" && pc==1)
    {
        CondCorDt=stk[1-pc].String();
        if (pc==2)
        {
            CondValues=stk[2-pc].Stru();
        }
        return StackItem(1);
    }
    if(method=="SETACCCORCTURN" && pc>=1)
    {
        int CurrPos=1;
        ReadAccs(pc, stk, CorCTNames, CurrPos);
        if (pc>=CurrPos)
        {
            string NextWord =stk[CurrPos-pc].String();
            if (!strcmpi(NextWord.data(),"cor"))
            {
                CurrPos++;
                ReadAccs(pc, stk, CorCTNamesD, CurrPos);
            }
        }

        if (pc>=CurrPos)
        {
            string NextWord =stk[CurrPos-pc].String();
            if (!strcmpi(NextWord.data(),"corNOT"))
            {
                CurrPos++;
                ReadAccs(pc, stk, CorCTNamesNot, CurrPos);
            }
        }
        return StackItem();
    }
    if(method=="SETCONDCORCTURN" && pc==1)
    {
        CondCorCt=stk[1-pc].String();
        if (pc==2)
        {
            CondValues=stk[2-pc].Stru();
        }
        return StackItem(1);
    }

    if(method=="ANALITIC" && pc==1)
    {
        DbName="";
        AnName="";
        AnCode=0;
        if (stk[1-pc].vtype()==gcl::VT_STRING)
            AnName=stk[1-pc].String();
        else
            AnCode=stk[1-pc].Number();

        FindDbDescr();
        return StackItem();
    }

    if(method=="EXEC")
    {
        if (DbName=="")
        {
            FindDbDescr();
        }
        Struct* S=NULL;
        if (pc==1)
        {
            S=stk[1-pc].Stru();
        }
        if (S)  CondValues=S;
        else
        {
            if (CondValues) S=CondValues;
            else S=this;
        }
        string Sql;
        BuildSql(Sql, S);
        cursor crsr;
        OraDML DML(&crsr);
        DML.DoPL(Sql, *S);
        SaveResults(S);
        return StackItem();
    }
    if(method=="SAVERESBREM")
    {
        int Tmp= ResSumBR;
        ResSumBR=0;
        return StackItem(Tmp);
    }
    if(method=="SAVERESEREM")
    {
        int Tmp= ResSumER;
        ResSumER=0;
        return StackItem(Tmp);
    }
    if(method=="SAVERESDTURN")
    {
        int Tmp= ResSumDT;
        ResSumDT=0;
        return StackItem(Tmp);
    }
    if(method=="SAVERESCTURN")
    {
        int Tmp= ResSumCT;
        ResSumCT=0;
        return StackItem(Tmp);
    }
    if(method=="SAVERESCORDTURN")
    {
        int Tmp= ResSumCorDT;
        ResSumCorDT=0;
        return StackItem(Tmp);
    }
    if(method=="SAVERESCORCTURN")
    {
        int Tmp= ResSumCorCT;
        ResSumCorCT=0;
        return StackItem(Tmp);
    }
    if(method=="RESULTBREM")
    {
        return StackItem(ResSumBR);
    }
    if(method=="RESULTEREM")
    {
        return StackItem(ResSumER);
    }
    if(method=="RESULTDTURN")
    {
        return StackItem(ResSumDT);
    }
    if(method=="RESULTCTURN")
    {
        return StackItem(ResSumCT);
    }
    if(method=="RESULTCORDTURN")
    {
        return StackItem(ResSumCorDT);
    }
    if(method=="RESULTCORCTURN")
    {
        return StackItem(ResSumCorCT);
    }

    prog_ptr->RTError(101,::string(unknownfuncerr)+"  "+method);
    return StackItem();
}
//------------------------------------------------------------------------------
bool AccMan::NeedCount (const string& What)
{
    return
        (What=="AS_BR" && BRNames.size()) ||
        (What=="AS_ER" && ERNames.size()) ||
        (What=="AS_DT" && DTNames.size()) ||
        (What=="AS_CT" && CTNames.size()) ||
        (What=="AS_COR_DT" && CorDTNames.size()&&(CorDTNamesC.size()||CorDTNamesNot.size())) ||
        (What=="AS_COR_CT" && CorCTNames.size()&&(CorCTNamesD.size()||CorCTNamesNot.size()));
};

//------------------------------------------------------------------------------
void AccMan::BuildSql(string& Sql, Struct* Pars)
{
    //===============================================================
    // Находим коды счетов
    //===============================================================
    string S1="";
    Sql=
    "    declare \r\n"
    "       vTmp number;\r\n"
    "       vDate date;\r\n";

    if (WhereAdd!="")
    {
        Sql+="      rcrsr *.AMS_Types.GenCursor;\r\n"
             "      vSumId number;\r\n"
             "      vOpCursor *.AMS_Types.GenCursor;\r\n";
    }

    map<string, string>::iterator i=NamesToAccs.begin();
    char Tmp [50];
    while (i!=NamesToAccs.end())
    {
        Sql+=string("      ACC_")+i->second+" number(8);\r\n";
        S1+=" select CODE into ACC_"+i->second+" from *.ACCS where NAME=:ACCN_"+i->second+";\r\n";
        string ParName="ACCN_"+i->second;
        Node* N=Pars->FindVar(ParName);
        if (!N)
            N=Pars->AddVar(new Node(ParName));
        N->assign(VAL(i->first));
        i++;
    }
    string ParName="AS_BR";
    Node* N;
    if (NeedCount(ParName))
    {
        N=Pars->FindVar(ParName);
        if (!N)
            N=Pars->AddVar(new Node(ParName));
        N->assign(VAL(ResSumBR));
        BuildRemSql(Sql, S1, 0);
    }

    ParName="AS_ER";
    if (NeedCount(ParName))
    {
        N=Pars->FindVar(ParName);
        if (!N)
            N=Pars->AddVar(new Node(ParName));
        N->assign(VAL(ResSumER));
        BuildRemSql(Sql, S1, 1);
    }
    ParName="AS_DT";
    if (NeedCount(ParName))
    {
        N=Pars->FindVar(ParName);
        if (!N)
            N=Pars->AddVar(new Node(ParName));
        N->assign(VAL(ResSumDT));
        BuildTurnSql(Sql, S1, 0);
    }
    ParName="AS_CT";
    if (NeedCount(ParName))
    {
        N=Pars->FindVar(ParName);
        if (!N)
            N=Pars->AddVar(new Node(ParName));
        N->assign(VAL(ResSumCT));
        BuildTurnSql(Sql, S1, 1);
    }
    ParName="AS_COR_DT";
    if (NeedCount(ParName))
    {
        N=Pars->FindVar(ParName);
        if (!N)
            N=Pars->AddVar(new Node(ParName));
        N->assign(VAL(ResSumDT));
        BuildCorTurnSql(Sql,S1, 0);
    }
    ParName="AS_COR_CT";
    if (NeedCount(ParName))
    {
        N=Pars->FindVar(ParName);
        if (!N)
            N=Pars->AddVar(new Node(ParName));
        N->assign(VAL(ResSumCT));
        BuildCorTurnSql(Sql,S1, 1);
    }
    ParName="DATE_BEG";
    N=Pars->FindVar(ParName);
    if (!N)
        N=Pars->AddVar(new Node(ParName));
    N->assign(VAL(DateBeg));

    ParName="DATE_END";
    N=Pars->FindVar(ParName);
    if (!N)
        N=Pars->AddVar(new Node(ParName));
    N->assign(VAL(DateEnd));

    ParName="ADD_ANAL_ID";
    N=Pars->FindVar(ParName);
    if (!N)
        N=Pars->AddVar(new Node(ParName));
    N->assign(VAL(AnCode));

    Sql+="begin\r\n"+S1+ "\r\nend;";
};
//------------------------------------------------------------------------------
void AccMan::BuildRemSql(string& Sql, string& Sql2, int RemType)
{
    string RS;
    std::vector<string> * Accs;
    if (!RemType)
    {
        RS=":AS_BR";
        Accs=&BRNames;
        Sql2+=" vDate:=:DATE_BEG;\r\n";
    }
    else
    {
        RS=":AS_ER";
        Accs=&ERNames;
        Sql2+=" vDate:=:DATE_END;\r\n";
    }
    Sql2+=" if "+RS+"=0 then\r\n"
    "       *.CreateTmpSum("+RS+", 0);\r\n"
    "   end if;\r\n" ;
    for (unsigned int i=0; i<Accs->size();i++)
    {
        BuildRemAccSql(Sql,Sql2, NamesToAccs[Accs->at(i)],RS);
    }
};
//------------------------------------------------------------------------------
void AccMan::BuildRemAccSql(string& Sql,string& Sql2, const string& AccCode, const string& AS)
{
    if (WhereAdd=="")
    {
        Sql2+=" vTmp:=*.R_ASM( ACC_"+AccCode+", vDate);\r\n"
              " *.AddAnalSum( :ADD_ANAL_ID, "+AS+", vTmp, 1);\r\n"
              " commit;\r\n"
              " *.DeleteIfTmpSum(vTmp);\r\n"
              " commit;\r\n";
    }
    else
    {
        Sql2+=
        "   *.GetBegSumAndDir(ACC_"+AccCode+", vDate, vTmp, vOpCursor);\r\n"
        "   open rcrsr for \r\n"
        "       select AN.* from *."+AnName+" AN, *."+DbName+" DB \r\n"
        "       where AN.SUMID=vTmp and AN.AARTICLE=DB.ARTICLE and \r\n"
        "       AN.AVER=DB.VER and "+WhereAdd+";\r\n"
        "   *.AddAnalSumW(:ADD_ANAL_ID, "+AS+", 1, '', rcrsr);\r\n"
        "   if vOpCursor%isopen then\r\n"
        "       LOOP\r\n"
        "         fetch vOpCursor into vSumId, vTmp ;\r\n"
        "         EXIT WHEN vOpCursor%NOTFOUND;\r\n"
        "         open rcrsr for \r\n"
        "            select AN.* from *."+AnName+" AN, *."+DbName+" DB \r\n"
        "            where AN.SUMID=vSumId and AN.AARTICLE=DB.ARTICLE and \r\n"
        "                  AN.AVER=DB.VER and "+WhereAdd+";\r\n"
        "            *.AddAnalSumW(:ADD_ANAL_ID, "+AS+", vTmp, '', rcrsr);\r\n"
        "          END LOOP;\r\n"
        "          Close vOpCursor;\r\n" 
        "   end if;\r\n";
    }
}
//------------------------------------------------------------------------------
void AccMan::BuildTurnSql(string& Sql, string& Sql2, int TurnType)
{
    string RS;
    std::vector<string> * Accs;
    string Cond;
    string SideLetter;
    string OpSign="";
    if (!TurnType)
    {
        RS=":AS_DT";
        Accs=&DTNames;
        if (CondDt!="") Cond=" and "+CondDt;
        SideLetter="SIDE='D' ";
    }
    else
    {
        RS=":AS_CT";
        Accs=&CTNames;
        if (CondCt!="") Cond=" and "+CondCt;
        SideLetter="SIDE='K' ";
        OpSign="-";
    }
    string AccsList;
    FormAccList(AccsList, Accs);

    Sql2+=" if "+RS+"=0 then\r\n"
    "       *.CreateTmpSum("+RS+", 0);\r\n"
    "   end if;\r\n"
    "   for x in (select OI.ITEM_SUM_ID, *.OpSign(OI.SIDE, O.STORNO) SI\r\n"
    "        from *.OP_ITEMS OI, *.OPERS O\r\n"
    "        where   OI.OP_DATE>=:DATE_BEG and OI.OP_DATE<:DATE_END and\r\n"
    "                OI."+AccsList+" and OI."+SideLetter+" and\r\n"
    "                OI.OP_ID=O.OP_ID and\r\n"
    "                OI.OP_SUBID=O.OP_SUBID and\r\n"
    "                O.STATUS=1 and O.POST=0 "+Cond+") LOOP\r\n";
    if (WhereAdd!="")
    {
        Sql2+=
        "   open rcrsr for "
        "       select AN.* from *."+AnName+" AN, *."+DbName+" DB \r\n"
        "       where AN.SUMID=X.ITEM_SUM_ID and AN.AARTICLE=DB.ARTICLE and\r\n"
        "       AN.AVER=DB.VER and "+WhereAdd+";\r\n"
        "   *.AddAnalSumW(:ADD_ANAL_ID, "+RS+", "+OpSign+"X.SI, '', rcrsr);\r\ncommit;\r\n";
    }
    else
    {
        Sql2+=
        "   *.AddAnalSum(:ADD_ANAL_ID, "+RS+", X.ITEM_SUM_ID, "+OpSign+"X.SI);\r\ncommit;\r\n";
    }
    Sql2+="   end loop;\r\n";
};
//------------------------------------------------------------------------------
void AccMan::BuildCorTurnSql(string& Sql, string& Sql2, int TurnType)
{
    string RS;
    std::vector<string> * Accs;
    std::vector<string> * CorAccs;
    std::vector<string> * NotAccs;
    string Cond;
    string SideLetter;
    string SideCorLetter;
    if (!TurnType)
    {
        RS=":AS_COR_DT";
        Accs=&CorDTNames;
        CorAccs=&CorDTNamesC;
        NotAccs=&CorDTNamesNot;
        if (CondCorDt!="") Cond=" and "+CondCorDt;
        SideLetter="SIDE='D' ";
        SideCorLetter="SIDE='K' ";
    }
    else
    {
        RS=":AS_COR_CT";
        if (CondCorCt!="") Cond=" and "+CondCorCt;
        SideLetter="SIDE='K' ";
        SideCorLetter="SIDE='D' ";
        Accs=&CorCTNames;
        CorAccs=&CorCTNamesD;
        NotAccs=&CorCTNamesNot;
    }
    string AccsList;
    FormAccList(AccsList, Accs);


    string AddCor1;
    string AddCor2;
    if (CorAccs->size())
    {
        AddCor1=", *.OP_ITEMS OI1 ";
        string Code_ ;
        FormAccList(Code_, CorAccs);
        AddCor2=" and OI1.OP_ID=OI.OP_ID and OI1.OP_SUBID=OI.OP_SUBID and OI1."+Code_+" ";
    }

    string AddCor3;
    if (NotAccs->size())
    {
        string Code_ ;
        FormAccList(Code_, NotAccs);
        AddCor3=" and not exist (select 1 from OP_ITEMS OI2 "
            " where "
            " OI2.OP_ID=OI.OP_ID and OI2.OP_SUBID=OI.OP_SUBID and OI2."+Code_+" and ROWNUM<2) ";
    }

    Sql2+=" if "+RS+"=0 then\r\n"
    "       *.CreateTmpSum("+RS+", 0);\r\n"
    "   end if;\r\n"
    "   for x in (select OI.ITEM_SUM_ID, *.OpSign(OI.SIDE, O.STORNO) SI\r\n"
    "        from *.OP_ITEMS OI, *.OPERS O "+AddCor1+"\r\n"
    "        where   OI.OP_DATE>=:DATE_BEG and OI.OP_DATE<:DATE_END and\r\n"
    "                OI."+AccsList+" and OI."+SideLetter+" and\r\n"
    "                OI.OP_ID=O.OP_ID and\r\n"
    "                OI.OP_SUBID=O.OP_SUBID and\r\n"
    "                O.STATUS=1 and O.POST=0 "+AddCor2+Cond+AddCor3+") LOOP\r\n";
    if (WhereAdd!="")
    {
        Sql2+=
        "   open rcrsr for \r\n"
        "       select AN.* from *."+AnName+" AN, *."+DbName+" DB \r\n"
        "       where AN.SUMID=X.ITEM_SUM_ID and AN.AARTICLE=DB.ARTICLE and \r\n"
        "       AN.AVER=DB.VER and "+WhereAdd+";\r\n"
        "   *.AddAnalSumW(:ADD_ANAL_ID, "+RS+", X.SI, '', rcrsr);\r\ncommit;\r\n";
    }
    else
    {
        Sql2+=
        "   *.AddAnalSum(:ADD_ANAL_ID, "+RS+", X.ITEM_SUM_ID, X.SI);\r\ncommit;\r\n";
    }
    Sql2+="   end loop;\r\n";
};
//------------------------------------------------------------------------------
void AccMan::FormAccList (string& AccsList, vector<string>* Accs)
{
    AccsList="CODE";
    if (Accs->size()>1)
    {
        AccsList+=" in (";
        for (unsigned int i=0; i<Accs->size();i++)
        {
            if (i) AccsList+=", ";
            AccsList+="ACC_"+NamesToAccs[Accs->at(i)];
        }
        AccsList+=")";
    }
    else
    {
        AccsList+="=ACC_"+NamesToAccs[Accs->at(0)];
    }
}
//------------------------------------------------------------------------------
AccMan::~AccMan()
{
    string Query;
    if (ResSumBR)
    {
        SetVal(this, "AS_BR", ResSumBR);
        Query+="*.DeleteIfTmpSum(:AS_BR);\r\n";
    }
    if (ResSumER)
    {
        SetVal(this, "AS_ER", ResSumER);
        Query+="*.DeleteIfTmpSum(:AS_ER);\r\n";
    }
    if (ResSumDT)
    {
        SetVal(this, "AS_DT", ResSumDT);
        Query+="*.DeleteIfTmpSum(:AS_DT);\r\n";
    }
    if (ResSumCT)
    {
        SetVal(this, "AS_CT", ResSumCT);
        Query+="*.DeleteIfTmpSum(:AS_CT);\r\n";
    }
    if (ResSumCorDT)
    {
        SetVal(this, "AS_COR_DT", ResSumCorDT);
        Query+="*.DeleteIfTmpSum(:AS_COR_DT);\r\n";
    }
    if (ResSumCorCT)
    {
        SetVal(this, "AS_COR_CT", ResSumCorCT);
        Query+="*.DeleteIfTmpSum(:AS_COR_CT);\r\n";
    }
    if (Query!="")
    {
        cursor crsr;
        OraDML DML(&crsr);
        Query+="commit;";
        DML.DoPL(Query, *this);
    }
}
//------------------------------------------------------------------------------
void AccMan::FindDbDescr()
{
    cursor crsr;
    OraDML DML(&crsr);
    string Query;
    AuxRec aux;
    if (AnName=="" && AnCode==0)
    {
        if (NamesToAccs.size())
        {
            aux.str3=NamesToAccs.begin()->first;
            Query+="select ANAL_ID into :int1 from *.ACCS where NAME=:Str3;"
            "select NAME into :str1 from *.OBJS where OID=:int1;";
        };
    }
    else
    {
        if (AnName=="")
        {
            Query+="select NAME into :str1 from *.OBJS where OID=:int1;";
            aux.int1=AnCode;
        }
        if (AnCode==0)
        {
            Query+="select OID into :int1 from *.OBJS where NAME=:str1;";
            aux.str1=AnName;
        }
    }
    Query+="select NAME into :str2 from *.OBJS "
        " where OID=(select PARENT from *.OBJS where OID=:int1);";
    DML.DoPL(Query,aux);
    if (AnName=="")
        AnName=aux.str1;
    if (AnCode==0)
        AnCode=aux.int1;
    DbName=aux.str2;
}
//------------------------------------------------------------------------------
void AccMan::SaveResults(Struct* Pars)
{
    string ParName="AS_BR";
    Node* N;
    if (NeedCount(ParName))
    {
        N=Pars->FindVar(ParName);
        if (N) ResSumBR=N->Number();
    }

    ParName="AS_ER";
    if (NeedCount(ParName))
    {
        N=Pars->FindVar(ParName);
        if (N) ResSumER=N->Number();
    }

    ParName="AS_DT";
    if (NeedCount(ParName))
    {
        N=Pars->FindVar(ParName);
        if (N) ResSumDT=N->Number();
    }

    ParName="AS_CT";
    if (NeedCount(ParName))
    {
        N=Pars->FindVar(ParName);
        if (N) ResSumCT=N->Number();
    }

    ParName="AS_COR_DT";
    if (NeedCount(ParName))
    {
        N=Pars->FindVar(ParName);
        if (N) ResSumCorDT=N->Number();
    }
    ParName="AS_COR_CT";
    if (NeedCount(ParName))
    {
        N=Pars->FindVar(ParName);
        if (N) ResSumCorCT=N->Number();
    }
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

