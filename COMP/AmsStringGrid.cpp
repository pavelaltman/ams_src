//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "AmsStringGrid.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAmsStringGrid *)
{
    new TAmsStringGrid(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAmsStringGrid::TAmsStringGrid(TComponent* Owner)
    : TStringGrid(Owner)
{
}
//---------------------------------------------------------------------------
namespace Amsstringgrid
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TAmsStringGrid)};
         RegisterComponents("Samples", classes, 0);
    }
}
//---------------------------------------------------------------------------
 