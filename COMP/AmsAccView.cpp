//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AmsAccView.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAmsTreeView *)
{
    new TAmsAccView(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAmsTreeView::TAmsTreeView(TComponent* Owner)
    : TTreeView(Owner)
{
}
//---------------------------------------------------------------------------
namespace Amstreeview
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TAmsTreeView)};
         RegisterComponents("Samples", classes, 0);
    }
}
//---------------------------------------------------------------------------
 