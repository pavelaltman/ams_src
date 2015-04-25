//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "AmsInputSum.h"
#include "AMS_EXC.h"
#pragma link "AmsInputStr"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAmsInputSum *)
{
    new TAmsInputSum(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAmsInputSum::TAmsInputSum(TComponent* Owner)
    : TAmsInputStr(Owner)
{
}
//---------------------------------------------------------------------------
namespace Amsinputsum
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TAmsInputSum)};
         RegisterComponents("Samples", classes, 0);
    }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
double TAmsInputSum::GetResultSum()
{
    AnsiString S =GetResultStr();
    char SP = ThousandSeparator;
    AnsiString S1="";
    char ADD[2];
    ADD[1]=0;
    for (int i=1; i<=S.Length(); i++)
    {
        if (S[i]!=SP)
        {
            ADD[0]=S[i];
            S1+=ADD;
        }
    }
    S=S1;

    double Res = 0 ;
    if (S!="" && S[1]!=' ')
    {
        for (int i =S[1]=='-'?2:1; i< S.Length();i++)
        {
            if (S[i]=='.' || S[i]==',' || S[i]=='-')
                S[i]=DecimalSeparator;
        }
        try {
            Res = S.ToDouble();
        }
        catch (EConvertError& E) {
            THROW_AMS_EXC(" Поле ввода \""+GetPrompt()+"\" = "+GetResultStr()+"\r\n должно быть суммой");
        };
    };
    return Res;
};
//---------------------------------------------------------------------------
void TAmsInputSum::ChangeEdit2()
{
    char SP = ThousandSeparator;
    AnsiString S = FInpVal->Text;
    AnsiString S1="";
    char ADD[2];
    ADD[1]=0;
    int Comma=0;
    int OldComma=0;
    for (int i=1; i<=S.Length(); i++)
    {
        if (S[i]!=SP)
        {
            ADD[0]=S[i];
            S1+=ADD;
        }
        if (S[i]=='.' || S[i]==',' || (S[i]=='-' && i>1))
        {
            Comma=S1.Length();
            OldComma=i;
        }
    }
    if (!Comma)
    {
        Comma=S1.Length();
        OldComma=S.Length();
    }
    else
    {
        Comma--;
    }
    int Rest = Comma % 3 ;
    Rest=3-Rest;

    AnsiString Res="";
    for (int i = 1; i<=Comma;i++)
    {
        if (Rest==3)
        {
            if (i!=1) Res+=SP;
            Rest=1;
        }
        else Rest++;
        Res+=S1[i];
    }
    Res+= S1.SubString(Comma+1, S1.Length()-Comma);
    Comma=Res.Length();

    if (Res!=S)
    {
        int CPos = FInpVal->SelStart-OldComma+Comma;
        int SelLength=FInpVal->SelLength;

        FInpVal->Text=Res;
        FInpVal->SelStart=CPos;
        FInpVal->SelLength=SelLength;
    }
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

