//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "AMS_QUERY.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
AmsQuery::AmsQuery():FromPool(true)
{OQ=0;OQ=DatMod->GetOQ();Clear();};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
AmsQuery::~AmsQuery()
{if (FromPool) DatMod->ReleaseOQ(OQ);};
//---------------------------------------------------------------------------
void DS_Q::DeclVar (const AnsiString& VarName, int Var)
{
    if (OQ)
    {
        OQ->DeclareVariable(VarName, otInteger);
        OQ->SetVariable(VarName, Var);
    }
    else
    {
        ODS->DeclareVariable(VarName, otInteger);
        ODS->SetVariable(VarName, Var);
    }
};
//---------------------------------------------------------------------------
void DS_Q::DeclVar (const AnsiString& VarName, const AnsiString& Var)
{
    if (OQ)
    {
        OQ->DeclareVariable(VarName, otString);
        OQ->SetVariable(VarName, Var);
    }
    else
    {
        ODS->DeclareVariable(VarName, otString);
        ODS->SetVariable(VarName, Var);
    }
};
//---------------------------------------------------------------------------
void DS_Q::DeclVar (const AnsiString& VarName, TDateTime Var)
{
    if (OQ)
    {
        OQ->DeclareVariable(VarName, otDate);
        OQ->SetVariable(VarName, Var);
    }
    else
    {
        ODS->DeclareVariable(VarName, otDate);
        ODS->SetVariable(VarName, Var);
    }
};
//---------------------------------------------------------------------------
void DS_Q::DeclVar (const AnsiString& VarName, double Var)
{
    if (OQ)
    {
        OQ->DeclareVariable(VarName, otFloat);
        OQ->SetVariable(VarName, Var);
    }
    else
    {
        ODS->DeclareVariable(VarName, otFloat);
        ODS->SetVariable(VarName, Var);
    }
};
//---------------------------------------------------------------------------
void DS_Q::DelVars()
{
    if (OQ)
    {
        OQ->DeleteVariables();
    }
    else
    {
        ODS->DeleteVariables();
    }
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void AmsQuery::Exec(const char* Msg)
{
       ExecSQL(OQ, Msg);
};
//---------------------------------------------------------------------------
int AmsQuery::ExecSilent()
{
    int Res = 0;
    try
    {
        if (AmsViewDebug)
            Exec("");
        else
            OQ->Execute();
    }
    catch(...)
    {
        Res=-1;
    }
    return Res;
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
AnsiString LookForVars(const AnsiString& Sql, int& BegPos)
{
    int i = BegPos?BegPos:1;
    bool Finded = false;
    AnsiString Res;
    char* Term=" ,<>=:\t\r\n\"\'+-/";
    while ( i<=Sql.Length() && !Finded)
    {
        if ( Sql[i]==':')
        {
            i++;
            int j= i;
            if (j<=Sql.Length() && Sql[j]!='=')
            {
                while (j<=Sql.Length() && !strchr(Term, Sql[j])) j++;
                Res = Sql.SubString(i, j-i);
                BegPos = j;
                Finded=true;
            }
        }
        i++;
    };
    return Res.UpperCase();
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void AmsQuery::BindVars (Struct* Str)
{
/*
    OQ->DeleteVariables();
    int i = 1;
    AnsiString S;
    while ( (S=LookForVars(S,i))!="")
    {
        Node* Val =Str->FindVar(S.c_str());
        if ( !Val )
        {
            THROW_AMS_EXC("ѕеременна€ подстановки "+S+" не определена в структуре");
        }
        else
        {
            Val->DeclareVar(this);
            Val->SetVar(this);
        }
    }
    while (S!="");

*/
};
//---------------------------------------------------------------------------
void GetVars (Struct* Str)
{
};
//---------------------------------------------------------------------------
void DS_Q::SetSqlText(const AnsiString& SqlText)
{
    if (OQ)
        OQ->SQL->Text = ReplaceBaseUser(SqlText);
    else
        ODS->SQL->Text = ReplaceBaseUser(SqlText);
}
//---------------------------------------------------------------------------

