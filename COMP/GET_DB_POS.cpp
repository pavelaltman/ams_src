//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "GET_DB_POS.h"
#include "FRM_BROWS_DB.h"
#include "FRM_CHOOSE_DB_POS.h"
#include "ams_win_c.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
int GetDbPos (int DbId, AnsiString& Article, int& Ver, const AnsiString& SetName)
{
    TFrmChooseDbPos * Frm=AmsWinC->GetChooseFrm(DbId,SetName) ;
    return  Frm->SelectDbPos(Article, Ver, SetName);           
}; 
//---------------------------------------------------------------------------
int GetDbPos (int DbId, int& WithOutVer_, AnsiString& Article, int& Ver, const AnsiString& SetName)
{
    TFrmChooseDbPos * Frm=AmsWinC->GetChooseFrm(DbId,SetName) ;
    int Res = Frm->SelectDbPos(Article, Ver, SetName);
    WithOutVer_=Frm->GetWithoutVer();
    return Res;
};
//---------------------------------------------------------------------------
