//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AmsInputPoint.h"
#pragma link "AmsInput"
#pragma link "AmsInputAnal"
#pragma link "AmsInputDbPos"
#pragma link "AmsInputStr"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAmsInputPoint *)
{
    new TAmsInputPoint(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAmsInputPoint::TAmsInputPoint(TComponent* Owner)
    : TAmsInputAnal(Owner)
{
    FBut->Caption = "K";
    FBut->Hint = "Ввод объекта на карте";
    AmsAnalInfo="Не определено";
}
//---------------------------------------------------------------------------
namespace Amsinputpoint
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TAmsInputPoint)};
         RegisterComponents("Samples", classes, 0);
    }
}
//---------------------------------------------------------------------------
