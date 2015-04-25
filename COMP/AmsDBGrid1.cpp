//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AmsDBGrid1.h"
#pragma link "RXDBCtrl"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAmsDBGrid1 *)
{
    new TAmsDBGrid1(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAmsDBGrid1::TAmsDBGrid1(TComponent* Owner)
    : TRxDBGrid(Owner)
{
}
//---------------------------------------------------------------------------
namespace Amsdbgrid1
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TAmsDBGrid1)};
         RegisterComponents("Samples", classes, 0);
    }
}
//---------------------------------------------------------------------------
