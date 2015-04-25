//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "AmsInputInt.h"
#include "AMS_EXC.h"
#pragma link "AmsInputStr"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAmsInputInt *)
{
    new TAmsInputInt(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAmsInputInt::TAmsInputInt(TComponent* Owner)
    : TAmsInputStr(Owner)
{
    AmsMaxLen = 9;    
}
//---------------------------------------------------------------------------
namespace Amsinputint
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TAmsInputInt)};
         RegisterComponents("Samples", classes, 0);
    }
}
//---------------------------------------------------------------------------
int TAmsInputInt::GetResultInt()
{
    AnsiString S =GetResultStr(); 
    int i = 0 ;
    if (S!="" && S[1]!=' ')
    {
        try {
            i = S.ToInt();
        }
        catch (EConvertError& E) {
            THROW_AMS_EXC(" Поле ввода \""+GetPrompt()+"\"\r\n должно быть целым ");
        };
    };
    return i; 
};
//---------------------------------------------------------------------------
