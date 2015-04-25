//---------------------------------------------------------------------------
#pragma hdrstop

#include "vector.h"
#include "AMS_UTILS.h"
#include "Oracle.hpp"
#include "OracleData.hpp"
#include <stdio.h>
#include "AMS_EXC.h"
#include <fstream>
#include "print_error.h"
#include "AMS_READ_CNF.h"
#include "UTILS.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

AnsiString BaseUserName = "INIT";
bool AmsSqlDebug= false;
bool AmsViewDebug=false;

//---------------------------------------------------------------------------
bool HaveRole(const AnsiString& RoleName, TOracleQuery* OQ)
{
    bool Res= false;
    OQ->Clear();
    OQ->SQL->Text="select 1 from SESSION_ROLES where ROLE=:ROLE";
    OQ->DeclareVariable("ROLE", otString);
    OQ->SetVariable("ROLE",RoleName.UpperCase());
    ExecSQL(OQ,"Проверяем роль");
    if (!OQ->Eof)
    {
        Res=true;
    }
    return Res;
}
//---------------------------------------------------------------------------
void SetAction (TAction* Act)
{
    Act->Visible=true;
    Act->Enabled= true;
};
//---------------------------------------------------------------------------
void ClrAction(TAction* Act)
{
    Act->Visible=false;
    Act->Enabled=false;
}
//---------------------------------------------------------------------------
AnsiString TAccName::ToUserName(const AnsiString& SysName)
{
 char buf [40];
 if (SysName=="")
    return "";
 char* name = SysName.c_str();

 if(*name==0)
   { *buf=0; return (AnsiString)buf; }

 int t_new=0 ;

 // Skipping letter and slash
 {
  const char *t=name;
  if(isalpha(*t))
  {
   if (*t=='B') t_new=1 ;
   t++;
   if(*t=='/') t++;
  }
  strcpy(buf,t);
 }

 // Убираем лишние нули (но не с самого начала)
 {
  char *t;
  for(t=buf;*t && *t!='/';t++);
  for(;*t;)
    if(t[0]=='/' && t[1]=='0' && (t[2]!='/' && t[2])) strcpy(t+1,t+2);
    else t++;
 }

 // Для новых стандартов убираем слэши после 1-го и 2-го уровней
 if (t_new)
 {
  char *t=buf ;
  int  i=0 ;
  for( ; *t ; )
   if (*t=='/')
   {
    strcpy(t,t+1) ;
    i++ ;
    if (i>1) break ;
   }
   else t++ ;
 }
 return (AnsiString)buf ;
};
//---------------------------------------------------------------------------
int new_plan = 0;
//---------------------------------------------------------------------------
void NormalizeItem(char *it,int l,char *oit)
{
 int n=0;
 char *t,r[10];
 if(*it=='0')
   if(oit) strcpy(oit,it);
   else return;
 for(t=it;*t && isdigit(*t);t++) { n*=10; n+=*t-'0'; }
 sprintf(r,"%0*d%s",l,n,t);
 if(oit) strcpy(oit,r);
 else strcpy(it,r);
};
//---------------------------------------------------------------------------
AnsiString TAccName::ToSysName(const AnsiString& UserName)
{

 char nam [100];
 if (UserName=="")
    return "";
 char* onam = UserName.c_str();
 strcpy (nam, onam);

 char *nm=nam;
 char res[40]="A/";
 int new_plan=GC::NewPlan;
 if (new_plan) strcpy(res,"B/") ;

 int l= new_plan ? 1 : 2,
     l_flag=0 ;

 if(isalpha(*nm)) { res[0]=*nm; nm++; }
 if(*nm=='/') nm++;

 // Добавляем разделители после 1-го и 2-го уровней
 if (new_plan && nm[0]!='0')
 {
  char *p=nm ;
  if (strlen(p)>1 && *(p+1)!='/')
  {
   memmove(p+2,p+1,strlen(p)) ;
   *(p+1)='/' ;
   p=p+2 ;
  }
  if (strlen(p)>1 && *(p+1)!='/')
  {
   memmove(p+2,p+1,strlen(p)) ;
   *(p+1)='/' ;
  }
 }


 char *n;
 int done;
 for(done=0;!done;)
 {
  for(n=nm;*n && *n!='/';n++);
  if(!*n) done=1;
  else *n=0;
  if(!isalpha(*nam) && l<=2)
  {
    char *t=nm;
    for(;*t && isdigit(*t);t++);
    if((t-nm)>2) res[0]='Z';
  }
  if(res[0]=='Z') l=3;
  NormalizeItem(nm,l,res+strlen(res));

  if(l==2) l=3;
  if (l==1)
    if (l_flag)
     l=3 ;
    else
     l_flag=1 ;

  if(!done) { strcat(res,"/"); nm=n+1; }
 }

 if (!strcmp(res,"A/00"))
  strcpy(res,"A") ;

 if (!strcmp(res,"B/00"))
  strcpy(res,"B") ;

 return (AnsiString)res;
};
//---------------------------------------------------------------------------
AnsiString TAccName::ToSysNameDescr(const AnsiString& UserNameDescr)
{
    int Pos =UserNameDescr.Pos(" ");
    AnsiString UserName = UserNameDescr.SubString(1, Pos-1);
    return ToSysName(UserName); 
}; 
//---------------------------------------------------------------------------
TAccName::TAccName (int Code_, const AnsiString& UNameDescr):CodeOk(true)
{
    Code = Code_; 
    int Pos = UNameDescr.Pos(" ");
    SysAccName = TAccName::ToSysName(UNameDescr.SubString(1, Pos-1));
    Descr=UNameDescr.SubString(Pos+1, UNameDescr.Length()-Pos);
};
//---------------------------------------------------------------------------
AnsiString TAccName::GetUserName() const 
{ 
    return ToUserName (SysAccName);
};
//---------------------------------------------------------------------------
int TAccName::GetAccCodeInt (Oracle::TOracleQuery* OQ)
{
    OQ->SQL->Text = 
      "select CODE from "+BaseUserName+".ACCS where NAME = :VarStr1";
    OQ->DeleteVariables();
    OQ->DeclareVariable("VARSTR1", otString);
    OQ->SetVariable("VARSTR1", SysAccName);
    OQ->Execute();
    
    if (OQ->Eof)
    {
        Code=-1;
        CodeOk = false;    
    }
    else
    {
        Code = OQ->FieldAsInteger(0);
        CodeOk = true;
    };
    if (Code==-1)
        THROW_AMS_EXC("Не нашел код для счета c именем "+SysAccName);
    return Code;
}
//---------------------------------------------------------------------------
void TAccName::SetByCode( int Code_, Oracle::TOracleQuery* OQ)
{
    OQ->SQL->Text="select NAME from "+BaseUserName+".ACCS where CODE =:ACC_CODE";
    OQ->DeleteVariables();
    OQ->DeclareVariable("ACC_CODE", otInteger);
    OQ->SetVariable("ACC_CODE", Code_);
    OQ->Execute();
    if (OQ->Eof)
    {
        Code = -1;
        SysAccName="";            
    }
    else
    {
        Code = Code_;
        CodeOk = true;
        SysAccName = OQ->FieldAsString(0);
    };
    if (Code==-1)
        THROW_AMS_EXC ("Не нашел счет с кодом "+IntToStr(Code_));
};
//---------------------------------------------------------------------------
int PrepareListAmsBuch( TOracleQuery* OQ, TStrings* Str, const AnsiString& NotEq)
{
    if (NotEq=="")
        OQ->SQL->Text = "select OWNER from all_tables A where A.TABLE_NAME='OBJS' ";
    else
        OQ->SQL->Text = "select OWNER from all_tables A where A.TABLE_NAME='OBJS' "
          " and OWNER != '"+NotEq+"' ";
    OQ->Execute();
    Str->Clear();
    while (!OQ->Eof)
    {
        Str ->Add(OQ->FieldAsString(0));
        OQ->Next();
    };
    return Str->Count;
};
//---------------------------------------------------------------------------
int GetObjId (const AnsiString& Schema, const AnsiString& ObjName,
              Oracle::TOracleQuery* OQ)
{
    OQ->SQL->Text="select OID from "+Schema+".OBJS where NAME = :OBJ_NAME";
    OQ->DeleteVariables();
    OQ->DeclareVariable("OBJ_NAME", otString);
    OQ->SetVariable("OBJ_NAME", ObjName);
    ExecSQL(OQ, "Функция GetObjId:");
    if (OQ->Eof)
        THROW_AMS_EXC("Объекта "+ObjName+" нет в бухгалтерии "+Schema);
    else
        return OQ->FieldAsInteger(0);
};
//---------------------------------------------------------------------------
template <class T>
void OutSQL (T* OQ, const AnsiString& Prompt, const char* FileName)
{
    AnsiString Var;
    if (OQ->VariableCount() )
    {
        Var="\r\n----------- Variables --------------\r\n";
        for (int i = 0 ; i< OQ->VariableCount(); i++)
        {
            AnsiString VN=OQ->VariableName(i);
            VN[1]=' ';
            Var+="variable "+VN;
            switch (OQ->VariableType(i))
            {
                case otInteger:
                    Var+=" number;\r\n";
                break;
                case otString:
                     Var+=" varchar2;\r\n";
                break;
                case otDate:
                     Var+=" date;\r\n";
                break;
                case otFloat:
                     Var+=" number;\r\n";
                break;
                default:
                    Var+=" Can't show\r\n";
            }
        }
        Var+="begin \r\n";
        for (int i = 0 ; i< OQ->VariableCount(); i++)
        {
            AnsiString S;
            if (!OQ->GetVariable(OQ->VariableName(i)).IsNull())
            {
                switch (OQ->VariableType(i))
                {
                    case otInteger:
                        S=IntToStr((int)OQ->GetVariable(OQ->VariableName(i)));
                    break;
                    case otString:
                        S="'"+OQ->GetVariable(OQ->VariableName(i))+"'";
                    break;
                    case otDate:
                        S="'"+OQ->GetVariable(OQ->VariableName(i))+"'";
                    break;
                    case otFloat:
                        S=FloatToStr((double)OQ->GetVariable(OQ->VariableName(i)));
                    break;
                    default:
                        S=" Can't show ";
                }
                Var += "    "+OQ->VariableName(i)+":="+S+";\r\n";
            }
        }
        Var+="end;\r\n/\r\n";
    }
    bool Mess = false;
    ios_base::openmode OM = ios_base::out | ios_base::app;
    if (!FileName)
    {
        FileName = "error.sql";
        Mess = true;
        OM = ios_base::out | ios_base::trunc;
    }
    std::fstream F (FileName, OM);
    F<<Var.c_str()<<OQ->SubstitutedSQL().c_str()<<"\n";
    F.close();
    if (Mess && AmsViewDebug)
        ErrorMessage (Prompt+OQ->SQL->Text+Var);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
/*
void OutSQL (TOracleDataSet* OQ, const AnsiString& Prompt,
            const char* FileName=NULL)
{
    AnsiString Var;
    if (OQ->VariableCount() )
    {
        Var="\r\n-- Variables --\r\n";
        for (int i = 0 ; i< OQ->VariableCount(); i++)
        {
            AnsiString S;
            if (OQ->GetVariable(OQ->VariableName(i)).IsNull())
                S="NULL";
            else
            {
                switch (OQ->VariableType(i))
                {
                    case otInteger:
                        S=IntToStr((int)OQ->GetVariable(OQ->VariableName(i)));
                    break;
                    case otString:
                        S=OQ->GetVariable(OQ->VariableName(i));
                    break;
                    case otDate:
                        S=OQ->GetVariable(OQ->VariableName(i));
                    break;
                    case otFloat:
                        S=FloatToStr((double)OQ->GetVariable(OQ->VariableName(i)));
                    break;
                    default:
                        S=" Can't show ";
                }
            }
            Var += "--"+OQ->VariableName(i)+" = " + S+"\r\n";
        }
    }
    bool Mess = false;
    ios_base::openmode OM = ios_base::out | ios_base::app;
    if (!FileName)
    {
        FileName = "error.sql";
        Mess = true;
        OM = ios_base::out | ios_base::trunc;
    }
    std::fstream F (FileName, OM);
    F<<OQ->SubstitutedSQL().c_str()<<Var.c_str()<<"\n";
    F.close();
    if (Mess && AmsViewDebug)
        ErrorMessage (Prompt+OQ->SQL->Text+Var);
}
*/
//---------------------------------------------------------------------------
int DescribeSQL(Oracle::TOracleQuery* OQ, const AnsiString& Prompt)
{
    try {
        OQ->Describe();
    }
    catch (EOracleError& E)
    {
        OutSQL(OQ, Prompt+"\r\n"+ E.Message+"\r\n------------\r\n");
        throw;
    };
    return 0;
};
//--------------------------------------------------------------
//---------------------------------------------------------------------------
int ExecSQL(Oracle::TOracleQuery* OQ, const AnsiString& Prompt)
{
    try {
        if (AmsSqlDebug)
            OutSQL(OQ, " Prompt\r\n", "Debug.sql");

        OQ->Execute();
    }
    catch (EOracleError& E)
    {
        OutSQL(OQ, Prompt+"\r\n"+ E.Message+"\r\n------------\r\n");
        throw;
    };
    return 0;
};                                                          
//--------------------------------------------------------------
int ExecSQLNoExc(Oracle::TOracleQuery* OQ)
{
    try {
        if (AmsSqlDebug)
            OutSQL(OQ, " Prompt\r\n", "Debug.sql");

        OQ->Execute();
    }
    catch (EOracleError& E)
    {
    };
    return 0;
};
//--------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
int ExecODS(TOracleDataSet* ODS)
{
    try {
        if (AmsSqlDebug)
            OutSQL(ODS, " Prompt\r\n", "Debug.sql");
        ODS->Active = true;
    }
    catch (Exception& E)
    {
        OutSQL(ODS, "Error\r\n"+ E.Message+"\r\n------------\r\n");
        throw;
    };
    return 0;
};
//--------------------------------------------------------------
void FillCB (TStrings * StrList, TOracleQuery* OQ, int FldNo)
{
    StrList->Clear();
    while (!OQ->Eof)
    {
        StrList->Add(OQ->FieldAsString(FldNo));
        OQ->Next();
    };
}
//--------------------------------------------------------------
// Запрос выполняется внутри и не вытирает содержимое StringList
void FillStr (TStrings * StrList, TOracleQuery* OQ, int FldNo)
{
    ExecSQL(OQ,"");
    while (!OQ->Eof)
    {
        StrList->Add(OQ->FieldAsString(FldNo));
        OQ->Next();
    };
};
//--------------------------------------------------------------
int CompileProc(
    Oracle::TOracleQuery* OQ,
    const AnsiString& ProcName,
    const AnsiString& ProcBody, TStrings* Log)
{
    int Res = 0;
    OQ->SQL->Text= ProcBody;
    OQ->DeleteVariables();
    ExecSQL(OQ, "Компилируем процедуру");
    OQ->SQL->Text=
     "select line, position, text "
     "from user_errors "
     "where name = :PROC_NAME";
    OQ->DeclareVariable("PROC_NAME", otString);
    OQ->SetVariable("PROC_NAME", ProcName);
    ExecSQL(OQ, "Получаем ошибки компиляции");
    if (OQ->Eof)
    {
        Log->Add("Процедура "+ProcName+" скомпилирована успешно");
    }
    else
    {
        Log->Add("Ошибки при компиляции процедуры "+ProcName);
        while (!OQ->Eof)
        {
            Res++;
            Log->Add (IntToStr(Res)+" ["+IntToStr (OQ->FieldAsInteger(0))+"/"
                    +IntToStr (OQ->FieldAsInteger(1))+"] "+ OQ->FieldAsString(2));
            OQ->Next();
        }
    }
    return Res;
};
//--------------------------------------------------------------
// Возвращает строку-уникальный идентификатор текущей сессии
void GetCurrSessionId(Oracle::TOracleQuery* OQ, AnsiString& ResStr)
{
    OQ->Clear();
    OQ->SQL->Text=
        "begin select DBMS_SESSION.unique_session_id() into :str1 from DUAL;end;";
    OQ->DeclareVariable("STR1", otString);
    ExecSQL(OQ, "Уникальный ид сессии");
    ResStr = OQ->GetVariable("STR1");
};
//--------------------------------------------------------------

