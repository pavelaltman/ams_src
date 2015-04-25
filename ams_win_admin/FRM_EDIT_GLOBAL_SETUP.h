//---------------------------------------------------------------------------

#ifndef FRM_EDIT_GLOBAL_SETUPH
#define FRM_EDIT_GLOBAL_SETUPH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "OracleData.hpp"
#include "OracleNavigator.hpp"
#include <Db.hpp>
#include <ExtCtrls.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TFrmEditGlobalSetup : public TForm
{
__published:	// IDE-managed Components
    TDataSource *DataSource1;
    TOracleDataSet *ODS;
    TOracleNavigator *OracleNavigator1;
    TDBGrid *DBGrid1;
    TStringField *ODSVAL_TYPE;
    TStringField *ODSVALUE;
    TStringField *ODSVAL_DESCR;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmEditGlobalSetup(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmEditGlobalSetup *FrmEditGlobalSetup;
//---------------------------------------------------------------------------
#endif
