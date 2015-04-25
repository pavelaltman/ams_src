//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "ADMIN_DAT_MOD.h"
#include "MAIN_FRM.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Oracle"
#pragma resource "*.dfm"
TDatMod *DatMod;
TOracleQuery* TDatMod::GetOQ()
{
    // Для создания многопоточного отчета
    TOracleQuery* OQ=NULL;
    if (FreeQuery.size()!=0)
    {
        OQ= FreeQuery.front();
        FreeQuery.pop_front();
    }
    else
    {
        // добавляем запросик
        OQ = new TOracleQuery (this);
        OQ->Session= MainFrm->OSess;
        AddQuery.push_back(OQ);
    }
    return OQ;
};
//---------------------------------------------------------------------------
void TDatMod::ReleaseOQ(TOracleQuery* OQ)
{
    // Для создания многопоточного отчета
    FreeQuery.push_front(OQ);
};
//---------------------------------------------------------------------------
__fastcall TDatMod::TDatMod(TComponent* Owner)
    : TDataModule(Owner)
{
    FreeQuery.push_back(OQPool1);
    FreeQuery.push_back(OQPool2);
    FreeQuery.push_back(OQPool3);
    FreeQuery.push_back(OQPool4);
    FreeQuery.push_back(OQPool5);
}
//---------------------------------------------------------------------------

void __fastcall TDatMod::DataModuleDestroy(TObject *Sender)
{
    std::list<TOracleQuery*>::iterator i = AddQuery.begin();
    while (i != AddQuery.end())
    {
        delete *i;
        i++;
    }
}
//---------------------------------------------------------------------------

