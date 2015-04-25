//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "UTILS.h"
#include "FRM_INPUT_VALUE.h"
#include "FRM_ASK_SUM.h"
#include "FRM_ASK_DATE.h"
//#include "FRM_EXIT.h"

bool ToEnglI;
//---------------------------------------------------------------------------
//char BaseUserName []="NCOM";
// Для преобразования номера колонки Excel в ее букв представление
char* XLSC(int ColNum, // Номер колонки
                  char *Buf) // Куда писать результат работы
{
    int i = ColNum;
    *Buf=0;
    Buf[1]=0;
    Buf[2]=0;
    if ( i>0  && i<257 )
    {
      if (i>26)
      {
        Buf[0]='A'+(i-1)/26-1;
        Buf[1]='A'+(i-1)%26;
      }
      else Buf[0]='A'+i-1;
    }
    return Buf;
}

//---------------------------------------------------------------------------
// Заменяет комбинацию "*." на "ИМЯ_БАЗОВОГО_ПОЛЬЗОВАТЕЛЯ." в
// тексте SQL запроса
AnsiString ReplaceBaseUser (const AnsiString& Sql)
{
    AnsiString S=Sql;
    AnsiString Res;
    int i= 1;
    int j = 1;
    AnsiString ReplaceStr = BaseUserName + ".";
    if (ReplaceStr == ".") ReplaceStr = "";
    while (i = S.Pos("*."))
    {
        Res+=S.SubString(j,i-j)+ ReplaceStr;
        j=i+2;
        S[i]=' ';
    };
    Res+=S.SubString(j, S.Length()-j+1);
    return Res;
}
void ErrorMessage (const AnsiString& S)
{
  TApplication* App = Application;
  App->MessageBox (S.c_str(), "AMS Бухгалтерия", MB_OK+MB_ICONEXCLAMATION+MB_APPLMODAL);
};

void OkMessage (const AnsiString& S)
{
  TApplication* App = Application;
  App->MessageBox ( S.c_str(), "AMS Бухгалтерия", MB_OK+MB_APPLMODAL);
};

void Message (const AnsiString& S)
{
  TApplication* App = Application;
  App->MessageBox (S.c_str(), "AMS Бухгалтерия", MB_OK+MB_APPLMODAL);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

int AskMessage (const AnsiString& S, bool DefNo)
{
  TApplication* App = Application;
  return App->MessageBox ( S.c_str(), "AMS Бухгалтерия", MB_YESNO+MB_ICONQUESTION+
                     MB_APPLMODAL+(DefNo?MB_DEFBUTTON2:MB_DEFBUTTON1));
}
//---------------------------------------------------------------------------
int AskMessage3 (const AnsiString& S, bool DefNo)
{
  TApplication* App = Application;
  return App->MessageBox (S.c_str(), "AMS Бухгалтерия", MB_YESNOCANCEL+MB_ICONQUESTION+
                     MB_APPLMODAL+(DefNo?MB_DEFBUTTON2:MB_DEFBUTTON1));
}
//---------------------------------------------------------------------------
int AskStringIntern(const AnsiString& Prompt, int MaxLen, AnsiString& ResVal)
{
    TFrmInputValue* Frm;
    TApplication* App = Application;
    App->CreateForm(__classid(TFrmInputValue), &Frm);
    Frm->LabPrompt->Caption = Prompt;
    Frm->EdValue->Text = ResVal;
    Frm->EdValue->MaxLength = MaxLen;
    int Res =0;
    if (Frm->ShowModal()>0)
    {
        Res = 1;
        if (ToEnglI)
        {
            AnsiString S = Frm->EdValue->Text;
            bool Changed=false;
            for (int i=1; i<=S.Length(); i++)
            {
                if (S[i]=='і')
                {
                    S[i]='i';
                    Changed=true;
                }
                else
                if (S[i]=='І')
                {
                    S[i]='I';
                    Changed=true;
                }
            }
            if (Changed)
            {
                int CPos = Frm->EdValue->SelStart;
                int SelLength=Frm->EdValue->SelLength;
                Frm->EdValue->Text=S;
                Frm->EdValue->SelStart=CPos;
                Frm->EdValue->SelLength=SelLength;
            }
        }
        ResVal = Frm->EdValue->Text;
    }
    Frm->Release();
    return Res;
};
//---------------------------------------------------------------------------
int AskString(const AnsiString& Prompt, int MaxLen, AnsiString& ResVal)
{
    return AskStringIntern(Prompt, MaxLen,ResVal);
};
//---------------------------------------------------------------------------
int AskInt(const AnsiString& Prompt, int& ResVal)
{
    bool Maked = false;
    int Result = 0;
    do
    {
        AnsiString DefVal = IntToStr(ResVal);
        Result = AskStringIntern(Prompt, 9, DefVal);
        if (!Result) Maked = true;
        else
        {
            try
            {
                if (DefVal=="")
                {
                    DefVal="0";
                }
                int Res = DefVal.ToInt();
                ResVal = Res;
                Maked = true;
            }
            catch(...)
            {
                ErrorMessage ("Неправильно введено целое значение");
            }
        }
    } while (!Maked);
    return Result;
};
//---------------------------------------------------------------------------
int AskDate(const AnsiString& Prompt, TDateTime& ResVal)
{
    TFrmAskDate* Frm;
    Application->CreateForm(__classid(TFrmAskDate), &Frm);
    int Res = Frm->AskDate(Prompt, ResVal);
    Frm->Release();
    return Res;
};
//---------------------------------------------------------------------------
int AskSum(const AnsiString& Prompt, double& ResVal)
{
    bool Maked = false;
    TFrmAskSum* Frm;
    Application->CreateForm(__classid(TFrmAskSum), &Frm);
    int Result = Frm->AskSum(Prompt, ResVal);
    Frm->Release();
    return Result;
};
//--------------------------------------------------------------
double GetSum (const AnsiString& Val, int ShowErr)
{
    AnsiString S = Val.Trim();
    int i = S.Pos(" ");
    if (i)
    {
        S= S.SubString(1, i);
    }
    for ( int i = 2; i<S.Length(); i++)
    {
        if (S[i]== '.' || S[i]==',' || S[i]=='-')
            S[i]= DecimalSeparator;
    }
    double Res = 0;
    try
    {
        Res= StrToFloat(S);
    }
    catch (EConvertError & E)
    {
        if (ShowErr)
        {
            throw;
        }
    }
    return Res;
};
//--------------------------------------------------------------
TDateTime GetDate(const AnsiString& Val, int ShowErr)
{
    AnsiString S = Val.Trim();
    int i = S.Pos(" ");
    if (i)
    {
        S= S.SubString(1, i);
    }
    for ( int i = 1; i<S.Length(); i++)
    {
        if (S[i]== '.' || S[i]==',' || S[i]=='-'|| S[i]=='/')
            S[i]= DateSeparator;
    }
    TDateTime Res = 0;
    try
    {
        Res= StrToDateTime(S);
    }
    catch (EConvertError & E)
    {
        if (ShowErr)
        {
            throw;
        }
    }
    return Res;
};
//---------------------------------------------------------------------------
int AskQuestion(const AnsiString& Prompt)
{
    TFrmExit* Frm;
    Application->CreateForm(__classid(TFrmExit), &Frm);
    if (Frm->ShowForm(Prompt)==mrOk)
    return ID_YES;
};
/*
int CreateSum(Sum &value)
{
 AmsQuery Q ;
 Q.SetPLText("*.CreateSum(:sum_id,:value);") ;
 Q.DeclVarInt("SUM_ID") ;
 Q.DeclVarSum("VALUE",value) ;
 Q.Exec("Create sum") ;
 return Q.GetVarInt("SUM_ID") ;
}

int NewOpId()
{
 AmsQuery Q ;
 Q.SetPLText("SELECT NEXTVAL INTO :ID FROM *.OPERSEQ FROM DUAL") ;
 Q.DeclVarInt("ID") ;
 Q.Exec("Oper seq") ;
 return Q.GetVarInt("ID") ;
}
*/

