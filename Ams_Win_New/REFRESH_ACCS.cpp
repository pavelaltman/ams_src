//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "REFRESH_ACCS.h"
#include "FRM_BRWS_ACC_WINDOW.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
TAccsToWin AccsToWin;

TParentAndChild<TFrmBrowsAn,TFrmBrwsAccWindow> PC_FilterPos;

//---------------------------------------------------------------------------
TAccsToWin::TAccsToWin()
{
    AccsToWinCS=new TCriticalSection();
};
//---------------------------------------------------------------------------
void TAccsToWin::Refresh(bool otlozh)
{
    AccsToWinCS->Enter();
    try {
        IT i=WindowsList.begin();
        while (i!=WindowsList.end())
        {
            (*i)->RefreshAfterOper(otlozh);
            i++;
        }
    }
    __finally {
        AccsToWinCS->Leave();
    }
};
//---------------------------------------------------------------------------
void TAccsToWin::Add(TFrmOperListRoot* Frm)
{
    AccsToWinCS->Enter();
    try {
        IT i= WindowsList.begin();
        while (i!=WindowsList.end())
        {
            if ((*i)==Frm) return;
            else i++;
        }
        WindowsList.push_back(Frm);
    }
    __finally {
        AccsToWinCS->Leave();
    }
};
//---------------------------------------------------------------------------
void TAccsToWin::Del(TFrmOperListRoot* Frm)
{
    AccsToWinCS->Enter();
    try {
        IT i= WindowsList.begin();
        IT j;
        while (i!=WindowsList.end())
        {
            if ((*i)==Frm)
            {
                j=i;
                i++;
                WindowsList.erase(j);
            }
            else i++;
        }
    }
    __finally {
        AccsToWinCS->Leave();
    }
};
//---------------------------------------------------------------------------
void TRefreshAccs::Refresh(std::vector<int>& AccsList)
{
    unsigned int i=0;
    while (i<AccsList.size())
    {
        TAccsToWin& X = Aw[AccsList[i]];
        X.Refresh();
        i++;
    };
}
//---------------------------------------------------------------------------
/*
void TRefreshAccs::Add(TFrmOperListRoot* Frm)
{
    int AccCode=Frm->GetAccCode();
    TAccsToWin& X = Aw[AccCode];
    X.Add(Frm);
};
//---------------------------------------------------------------------------
void TRefreshAccs::Del(TFrmOperListRoot* Frm)
{
    int AccCode=Frm->GetAccCode();
    TAccsToWin& X = Aw[AccCode];
    X.Del(Frm);
};
//---------------------------------------------------------------------------
void TRefreshAccs::OpChange (int OpId)
{
    // Œ¡ÕŒ¬Àﬂ≈Ã ¬—≈ Œ Õ¿ — ƒ¿ÕÕ€Ã» —◊≈“¿Ã»
    AmsQuery Q ;
    Q.SetSqlText("select distinct CODE from *.OP_ITEMS where OP_ID=:OP_ID");
    Q.DeclVar("OP_ID", OpId);
    Q.Exec(" Ó‰˚ Ò˜ÂÚÓ‚");

    std::vector<int> AccsList;
    while (Q.NotEof())
    {
        AccsList.push_back(Q.GetFldInt(0));
        Q.Next();
    }

    if (AccsList.size())
    {
        Refresh(AccsList);
    }
    Q.Clear();
};
//---------------------------------------------------------------------------
*/
