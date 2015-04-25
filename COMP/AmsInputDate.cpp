//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "AmsInputDate.h"
#include "AMS_EXC.h"
#pragma link "AmsInputStr"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAmsInputDate *)
{
    new TAmsInputDate(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAmsInputDate::TAmsInputDate(TComponent* Owner)
    : TAmsInputStr(Owner)
{
    AmsMaxLen = 10;    
}
//---------------------------------------------------------------------------
namespace Amsinputdate
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TAmsInputDate)};
         RegisterComponents("Samples", classes, 0);
    }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TDateTime TAmsInputDate::GetResultDat()
{
    AnsiString S =GetResultStr(); 
    TDateTime Res = 0 ;
    if (S!="" && S[1]!=' ')
    {
        bool MakeChange = true;
        while (MakeChange) 
        {  
            MakeChange = false;
            int i =0;
            if (DateSeparator != ',') 
                i = S.Pos(",");
            if (!i && DateSeparator != '-') 
                i= S.Pos("-");
            if (!i && DateSeparator != '.') 
                i= S.Pos(".");
        
            if (i) 
            {
                S[i]=DateSeparator;
                MakeChange =true;
            };
        };
                
        try {
            Res = StrToDate(S);
        }
        catch (EConvertError& E) {
            THROW_AMS_EXC(" Поле ввода \""+GetPrompt()+"\" = "+GetResultStr()+"\r\n должно быть суммой");
        };
    };
    return Res; 
};
//---------------------------------------------------------------------------