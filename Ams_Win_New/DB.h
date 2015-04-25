//---------------------------------------------------------------------------

#ifndef DBH
#define DBH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
//---------------------------------------------------------------------------
class TDataModule1 : public TDataModule
{
__published:	// IDE-managed Components
    TDatabase *DB1;
    TQuery *Query1;
private:	// User declarations
public:		// User declarations
    __fastcall TDataModule1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDataModule1 *DataModule1;
//---------------------------------------------------------------------------
#endif
