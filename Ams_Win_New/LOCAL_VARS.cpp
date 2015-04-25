//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "LOCAL_VARS.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
void TGenLocalVars::ClearVars()
{
    ACCV()->Structure().MakeEmpty();
    VARS()->Structure().MakeEmpty();
    OPV ()->Structure().MakeEmpty();
};
//---------------------------------------------------------------------------
void TGenLocalVars::MakeCopy (const TGenLocalVars& CPY)
{
    
};
//---------------------------------------------------------------------------

