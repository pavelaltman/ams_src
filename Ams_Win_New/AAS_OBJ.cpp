//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "AAS_OBJ.h"
extern const char* unknownfuncerr;

//---------------------------------------------------------------------------
#pragma package(smart_init)
AasObj::AasObj(int AnCode_, const string& AnName_,
    const string& WhereAdd_, Struct* S )
{
    AnCode=AnCode_;
    AnName=AnName_;
    WhereAdd=WhereAdd_;
    CondValues=S;
    ResSum=0;

    cursor crsr;
    OraDML DML(&crsr);
    string Query;
    AuxRec aux;
    if (AnName=="")
    {
        Query="select NAME into :str1 from *.OBJS where OID=:int1;";
        aux.int1=AnCode;
    }
    if (AnCode==0)
    {
        Query="select OID into :int1 from *.OBJS where NAME=:str1;";
        aux.str1=AnName;
    }
    Query+="select NAME into :str2 from *.OBJS "
        " where OID=(select PARENT from *.OBJS where OID=:int1);";
    Query+="*.CreateTmpSum(:int2, 0, 'AAS Object' );commit;";
    DML.DoPL(Query,aux);
    if (AnName=="")
    {
        AnName=aux.str1;
    }
    if (AnCode==0)
    {
        AnCode=aux.int1;
    }
    DbName=aux.str2;
    ResSum=aux.int2;
    
    els.push_back(new Node("ASUMID"));
    els.push_back(new Node("ADD_SIGN"));
    els.push_back(new Node("RES_SUMID"));
    els.push_back(new Node("ADD_ANAL_ID"));
    els.push_back(new Node("ADD_SET_NAME"));

};
//---------------------------------------------------------------------------
StackItem AasObj::Method(Node &node,::string const &method,int pc,StackItem *stk)
{
    if(method=="COND" && (pc==1 || pc==2))
    {
        WhereAdd=stk[1-pc].String();
        if (pc==2)
        {
            CondValues=stk[2-pc].Stru();
        }
        return StackItem(1);
    }
    if(method=="RESULT")
    {
        return StackItem(ResSum);
    }
    if(method=="SAVERES")
    {
        int R= ResSum;
        ResSum=0;
        return StackItem(R);
    }
    if(method=="DELETE")
    {
        if (ResSum)
        {
            cursor crsr;
            OraDML DML(&crsr);
            AuxRec aux;
            aux.int1=ResSum;
            DML.DoPL("*.DeleteIfTmpSum(:int1);commit;", aux);
        }
        ResSum=0;
        return StackItem();
    }
    if(method=="ADD" && (pc==1 || pc==2 || pc==3) )
    {
        string SetName="";
        if (pc==3)
        {
            SetName = stk[3-pc].String();
        }
        int Sign=1;
        if (pc==2)
        {
            Sign = stk[2-pc].Number();
        }
        int AddSum=stk[1-pc].Number();
        Struct * CondVal = CondValues?CondValues:this;
        if (CondVal)
        {
            Node* N=CondVal->FindVar("ASUMID");
            if (!N)
                N=CondVal->AddVar(new Node("ASUMID"));
            N->assign(VAL(AddSum));

            N=CondVal->FindVar("ADD_ANAL_ID");
            if (!N)
                N=CondVal->AddVar(new Node("ADD_ANAL_ID"));
            N->assign(VAL(AnCode));


            N=CondVal->FindVar("ADD_SET_NAME");
            if (!N)
                N=CondVal->AddVar(new Node("ADD_SET_NAME"));
            N->assign(VAL(SetName));

            N=CondVal->FindVar("ADD_SIGN");
            if (!N)
                N=CondVal->AddVar(new Node("ADD_SIGN"));
            N->assign(VAL(Sign));

            N=CondVal->FindVar("RES_SUMID");
            if (!N)
                N=CondVal->AddVar(new Node("RES_SUMID"));
            N->assign(VAL(ResSum));
            string Query;
            if (WhereAdd!="")
            {
                Query="declare rcrsr *.AMS_Types.GenCursor;"
                    "begin "
                    "   open rcrsr for "
                    "       select AN.* from *."+AnName+" AN, *."+DbName+" DB "
                    "       where AN.SUMID=:ASUMID and AN.AARTICLE=DB.ARTICLE and "
                    "       AN.AVER=DB.VER and "+WhereAdd+";"
                    "   *.AddAnalSumW(:ADD_ANAL_ID, :RES_SUMID, :ADD_SIGN, :ADD_SET_NAME, rcrsr);"
                    "   commit;"
                    "end;";
            }
            else
            {
                Query="*.AddAnalSum(:ADD_ANAL_ID, :RES_SUMID, :ASUMID, :ADD_SIGN);commit;";
            }
            cursor crsr;
            OraDML DML(&crsr);
            DML.DoPL(Query,*CondVal);
        }
        return StackItem(1);
    }

    prog_ptr->RTError(101,::string(unknownfuncerr)+"  "+method);
    return StackItem();
}
//------------------------------------------------------------------------------
AasObj::~AasObj()
{
    if (ResSum)
    {
        cursor crsr;
        OraDML DML(&crsr);
        AuxRec aux;
        aux.int1=ResSum;
        DML.DoPL("*.DeleteIfTmpSum(:int1);commit;", aux);
        ResSum=0;
    }
}
//------------------------------------------------------------------------------

