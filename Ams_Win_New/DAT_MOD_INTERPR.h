//---------------------------------------------------------------------------

#ifndef DAT_MOD_INTERPRH
#define DAT_MOD_INTERPRH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AmsLogon.h"
#include "Oracle.hpp"
//---------------------------------------------------------------------------
class TDatMod : public TDataModule
{
__published:	// IDE-managed Components
    TOracleSession *OSession;
    TAmsLogon *OLogon;
    TOracleQuery *OracleQuery1;
    TOracleQuery *OracleQuery2;
    TOracleQuery *OracleQuery3;
    TOracleQuery *OracleQuery4;
    TOracleQuery *OracleQuery5;
    TOracleQuery *OracleQuery6;
    TOracleQuery *OracleQuery7;
    TOracleQuery *OracleQuery8;
    TOracleQuery *OracleQuery9;
    TOracleQuery *OracleQuery10;
    void __fastcall DataModuleCreate(TObject *Sender);
protected:	// User declarations
    std::list<TOracleQuery*> FreeOQ;
public:		// User declarations
    __fastcall TDatMod(TComponent* Owner);
    TOracleQuery* GetOQ();
    void ReleaseOQ(TOracleQuery* OQ);
    int FreeQueryCount(){return FreeOQ.size();};
};
//---------------------------------------------------------------------------
extern PACKAGE TDatMod *DatMod;
//---------------------------------------------------------------------------
#endif
