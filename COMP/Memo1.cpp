//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Memo1.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TMemo1 *)
{
    new TMemo1(NULL);
}
//---------------------------------------------------------------------------
__fastcall TMemo1::TMemo1(TComponent* Owner)
    : TMemo(Owner)
{
}
//---------------------------------------------------------------------------
namespace Memo1
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TMemo1)};
         RegisterComponents("Samples", classes, 0);
    }
}
//---------------------------------------------------------------------------
 