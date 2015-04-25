//---------------------------------------------------------------------------

#ifndef ADMIN_DAT_MODH
#define ADMIN_DAT_MODH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Oracle.hpp"
//---------------------------------------------------------------------------
class TDatMod : public TDataModule
{
__published:	// IDE-managed Components
    TOracleQuery *OQPool1;
    TOracleQuery *OQPool2;
    TOracleQuery *OQPool3;
    TOracleQuery *OQPool4;
    TOracleQuery *OQPool5;
    void __fastcall DataModuleDestroy(TObject *Sender);
private:	// User declarations
    std::list<TOracleQuery*> FreeQuery;
    std::list<TOracleQuery*> AddQuery;
public:		// User declarations
    __fastcall TDatMod(TComponent* Owner);
    TOracleQuery* GetOQ();
    void ReleaseOQ(TOracleQuery* OQ);    
};
//---------------------------------------------------------------------------
extern PACKAGE TDatMod *DatMod;
//---------------------------------------------------------------------------
#endif
