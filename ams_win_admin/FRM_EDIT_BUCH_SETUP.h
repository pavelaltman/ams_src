//---------------------------------------------------------------------------

#ifndef FRM_EDIT_BUCH_SETUPH
#define FRM_EDIT_BUCH_SETUPH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "OracleData.hpp"
#include "OracleNavigator.hpp"
#include <Db.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "AmsDBGrid.h"
#include "RxDBCtrl.hpp"
#include "RXDBCtrl.hpp"
//---------------------------------------------------------------------------
class TFrmEditBuchSetup : public TForm
{
__published:	// IDE-managed Components
    TDataSource *DataSource1;
    TOracleDataSet *ODSLic;
    TOracleNavigator *OracleNavigator1;
    TDBGrid *DBGrid1;
    TOracleNavigator *OracleNavigator2;
    TDataSource *DataSource2;
    TOracleDataSet *ODSBooks;
    TIntegerField *ODSLicID;
    TStringField *ODSLicFIRM;
    TStringField *ODSLicDOC_PATH;
    TIntegerField *ODSBooksID;
    TIntegerField *ODSBooksSUB_ID;
    TStringField *ODSBooksBASE_USER;
    TStringField *ODSBooksSYS_BASE_USER;
    TStringField *ODSBooksSTATUS;
    TStringField *ODSBooksLAST_UPD;
    TStringField *ODSBooksDESCR;
    TAmsDBGrid *AmsDBGrid1;
    void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmEditBuchSetup(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmEditBuchSetup *FrmEditBuchSetup;
//---------------------------------------------------------------------------
#endif
