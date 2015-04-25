//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "DAT_MOD_INTERPR.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AmsLogon"
#pragma link "Oracle"
#pragma resource "*.dfm"
TDatMod *DatMod;
//---------------------------------------------------------------------------
__fastcall TDatMod::TDatMod(TComponent* Owner)
    : TDataModule(Owner)
{
}
//---------------------------------------------------------------------------
TOracleQuery* TDatMod::GetOQ()
{
    TOracleQuery* OQ;
    if (FreeOQ.size())
    {
         OQ=FreeOQ.back();
         FreeOQ.pop_back();
    }
    else
    {
        THROW_AMS_EXC("Закончились свободные запросы");
    };
    return OQ;
};
//---------------------------------------------------------------------------
void TDatMod::ReleaseOQ(TOracleQuery* OQ)
{
    FreeOQ.push_back(OQ);
};
//---------------------------------------------------------------------------
extern char username[], connect_str[], userpass[];
extern void InitCType();

void __fastcall TDatMod::DataModuleCreate(TObject *Sender)
{
    OSession->LogonUsername=username;
    OSession->LogonPassword= userpass;
    OSession->LogonDatabase=connect_str;
    OSession->Connected=true;
    InitCType();

    FreeOQ.push_back (OracleQuery1);
    FreeOQ.push_back (OracleQuery2);
    FreeOQ.push_back (OracleQuery3);
    FreeOQ.push_back (OracleQuery4);
    FreeOQ.push_back (OracleQuery5);
    FreeOQ.push_back (OracleQuery6);
    FreeOQ.push_back (OracleQuery7);
    FreeOQ.push_back (OracleQuery8);
    FreeOQ.push_back (OracleQuery9);
    FreeOQ.push_back (OracleQuery10);
}
//---------------------------------------------------------------------------

