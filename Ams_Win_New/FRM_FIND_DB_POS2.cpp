//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "FRM_FIND_DB_POS2.h"
#include "DAT_MOD.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AmsInpCond"
#pragma resource "*.dfm"
TFrmFindDbPos *FrmFindDbPos;
//---------------------------------------------------------------------------
__fastcall TFrmFindDbPos::TFrmFindDbPos(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
bool TFrmFindDbPos::FindDbPos()
{
    //
    bool Res = false;
    if (ShowModal()> 0) {
        Res = true;
        AnsiString s="TEdit";
        if (s==((TEdit*)ActiveControl)->ClassName()) {
          ((TEdit*)ActiveControl)->SelStart=0;
          ((TEdit*)ActiveControl)->SelLength=((TEdit*)ActiveControl)->Text.Length();
        }
    }
    return Res;
    /*
    if (FindNext)
    {
        if (SqlFormed)
        {
            if (!OQ->Eof)
            {
                ResCatId = OQ->FieldAsInteger(0);
                ResArt  = OQ->FieldAsString(1);
                ResVer  = OQ->FieldAsInteger(2);
                OQ->Next();
                Res = true;
            }
            else
            {
                if (AskMessage("Поиск завершен. Начинать заново?")== ID_YES)
                {
                    ExecSQL(OQ,"Поиск позиции БД");
                    Res = FindDbPos(true,ResCatId,ResArt,ResVer);
                }
            }
        }
    }
    else
    {
        if (ShowModal()==1)
        {
            FormSqlQuery(AddWhere);
            ExecSQL(OQ, "Поиск позиции БД");
            Res = FindDbPos(true,ResCatId,ResArt,ResVer);
        }

    }
    return Res; */
}
//---------------------------------------------------------------------------
void TFrmFindDbPos::Prepare(TFldsDescr* FldsDescr_, const AnsiString& DbName_)
{
    DbName = DbName_;
    FldsDescr = FldsDescr_;
    SFldCondToFldOpt FO;
    for (unsigned int i =0; i< IC.size(); i++)
    {
        FO = IC[i];
        delete FO.FCond;
    }
    IC.clear();
    int X = 25;
    int MaxLen = 0;

    for (int i = 0 ; i< FldsDescr->Count(); i++)
    {
        int DO = (*FldsDescr)[i].DefOpt;
        int DSO = (*FldsDescr)[i].DispOpt;
        if ( (!(DO & Hidden) && !(DSO & Hidden)) ||
            ( (DO & Editable)&&(DSO & Editable)) )
        {
            FO.FOpt = &((*FldsDescr)[i]);
            TAmsInpCond* ICC = new TAmsInpCond(this);
            FO.FCond=ICC;
            ICC->Parent = SBox;
            ICC->Top =X;
            ICC->SetFld(FO.FOpt->Alias, FO.FOpt->Type,FO.FOpt->Hint);
            X+= ICC->Height;
            ICC->Align=alTop;
            if (ICC->GetPromptLen()>MaxLen)
                MaxLen = ICC->GetPromptLen();
            FO.FCond=ICC;
            IC.push_back(FO);
        }
    }

    FirstC = NULL;
    TAmsInpCond* LastC  = NULL;
    for (unsigned int i = 0 ; i< IC.size();i++)
    {
        if (!i)
            FirstC= IC[i].FCond;
        else
        {
            IC[i-1].FCond->SetPost (IC[i].FCond);
            IC[i].FCond->SetPred(IC[i-1].FCond);
        }
        LastC= IC[i].FCond;
        IC[i].FCond->SetPromptLen(MaxLen);
    }

    if (FirstC && LastC)
    {
        //FirstC->SetPred (FirstC);
        LastC->SetPost(BBOk);
    }

    Top = 50;
    X+= 60;
    if (X> (Screen->Height-100))
    {
        X=Screen->Height-100;
    }
    Height = X;
}
//---------------------------------------------------------------------------
bool TFrmFindDbPos::FormWhereCond (AnsiString& SqlWhere, DS_Q& D, bool FirstAnd)
{
    bool FirstTime = !FirstAnd;
    AnsiString S = "";
    for (unsigned int i = 0; i<IC.size(); i++)
    {
        TAmsInpCond * ICC = IC[i].FCond;

        if (ICC->GetStr()!="")
        {
            SFldOpt* FO = IC[i].FOpt;
            if (FirstTime)
                FirstTime = false;
            else
                S+=" AND ";

            AnsiString Sign;
            AnsiString CmpWhat = "CMP_WHAT"+IntToStr(i)+AddStr;
            if (ICC->GetSign()=="~")
                Sign = " LIKE :"+CmpWhat;
            else
                Sign = ICC->GetSign() +" :"+ CmpWhat;

            switch (FO->Type)
            {
                case aftStr:
                {
                    AnsiString StrVar =ICC->GetStr().UpperCase();
                    StrVar=ChangeUkrI(StrVar);
                    if (ICC->GetSign()=="~")
                        StrVar = "%"+StrVar+"%";
                    D.DeclVar(CmpWhat, StrVar);
                    S+= "UPPER("+FO->FullName+")"+Sign;
                }
                break;

                case aftInt:
                {
                    D.DeclVar(CmpWhat,ICC->GetInt());
                }
                break;
                case aftSumma:
                    D.DeclVar(CmpWhat, ICC->GetSum());
                break;
                case aftDate:
                    D.DeclVar (CmpWhat, ICC->GetDate());
                break;
            }
            if (FO->Type!= aftStr)
                S+= FO->FullName+Sign;
        }
    }
    SqlWhere += S;
    return !FirstTime;
}
//---------------------------------------------------------------------------
bool TFrmFindDbPos::FormStrWhereCond (AnsiString& SqlWhere, bool FirstAnd)
{
    bool FirstTime = !FirstAnd;
    AnsiString S = "";
    for (unsigned int i = 0; i<IC.size(); i++)
    {
        TAmsInpCond * ICC = IC[i].FCond;

        if (ICC->GetStr()!="")
        {
            SFldOpt* FO = IC[i].FOpt;
            if (FirstTime)
                FirstTime = false;
            else
                S+=" AND ";

            AnsiString Sign;
            AnsiString StrVar =ICC->GetStr().UpperCase();
            StrVar=ChangeUkrI(StrVar);
            if (ICC->GetSign()=="~")
                Sign = " LIKE ";
            else
                Sign = ICC->GetSign() +" ";

            switch (FO->Type)
            {
                case aftStr:
                {
                    AnsiString Res="";
                    char Str[2];
                    Str[1]=0;
                    for (int i = 1; i<= StrVar.Length();i++)
                    {
                        if (StrVar[i]=='\'')
                        {
                            Res+="''";
                        }
                        else
                        {
                            Str[0]=StrVar[i];
                            Res+=Str;
                        }
                    }
                    if (ICC->GetSign()=="~")
                        StrVar = "'%"+Res+"%'";
                    else
                        StrVar= "'"+Res+"'";
                    S+= "UPPER("+FO->FullName+")"+Sign+StrVar;
                }
                break;

                case aftInt:
                case aftBool:
                {
                    int i=ICC->GetInt();
                    Sign+= IntToStr(i);
                }
                break;
                case aftSumma:
                {
                    AnsiString DD =FloatToStr(ICC->GetSum());
                    int i = DD.Pos(',');
                    if (i)
                        DD[i]='.';
                    Sign+=DD ;
                }
                break;
                case aftDate:
                    Sign+="'"+ FormatDateTime ("dd.mm.yyyy",ICC->GetDate())+"'";
                break;
            }
            if (FO->Type!= aftStr)
                S+= FO->FullName+Sign;
        }
    }
    SqlWhere += S;
    return !FirstTime;
}
//---------------------------------------------------------------------------
void __fastcall TFrmFindDbPos::BBOkClick(TObject *Sender)
{
    ModalResult = 1;
}
//---------------------------------------------------------------------------
void __fastcall TFrmFindDbPos::BBCancelClick(TObject *Sender)
{
    ModalResult = -1;
}
//---------------------------------------------------------------------------
void __fastcall TFrmFindDbPos::FormActivate(TObject *Sender)
{
    TopMostForm = this;
}
//---------------------------------------------------------------------------
int TFrmFindDbPos::SetFilter(void)
{
    int Res = 0;
    if (ShowModal()>0)
    {
        Res=1;
           AnsiString s="TEdit";
        if (s==((TEdit*)ActiveControl)->ClassName()) {
          ((TEdit*)ActiveControl)->SelStart=0;
          ((TEdit*)ActiveControl)->SelLength=((TEdit*)ActiveControl)->Text.Length();
        }
    }
    return Res;
}
//---------------------------------------------------------------------------
AnsiString TFrmFindDbPos::GetDescr(void)
{
    //
    AnsiString S;
    for (unsigned int i = 0; i< IC.size(); i++)
    {
        TAmsInpCond* ICC = IC[i].FCond;
        if (ICC->GetStr()!="")
        {
            if (S!="") S+="; ";
            SFldOpt * FO = IC[i].FOpt;
            S+= FO->Alias+ ICC->GetSign()+ICC->GetStr();
        }
    }
    return S;
}
//---------------------------------------------------------------------------
void __fastcall TFrmFindDbPos::ActInsAndExecExecute(TObject *Sender)
{
    BBOkClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TFrmFindDbPos::ActDelAllCondExecute(TObject *Sender)
{
    for (unsigned int i = 0; i < IC.size(); i++)
    {
        TAmsInpCond* ICC = IC[i].FCond;
        ICC->ClrCond();
    }
}
//---------------------------------------------------------------------------


void __fastcall TFrmFindDbPos::FormShow(TObject *Sender)
{
    if (FirstC)
    {
        ActiveControl=BBOk;
        ActiveControl=FirstC;
        FirstC= NULL;
    }
}
//---------------------------------------------------------------------------

