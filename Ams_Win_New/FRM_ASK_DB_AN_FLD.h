//---------------------------------------------------------------------------

#ifndef FRM_ASK_DB_AN_FLDH
#define FRM_ASK_DB_AN_FLDH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "OracleData.hpp"
#include <Buttons.hpp>
#include <Db.hpp>
#include "AmsDBGrid.h"
#include "RXDBCtrl.hpp"
#include <DBGrids.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TFrmAskDbAnFld : public TForm
{
__published:	// IDE-managed Components
    TOracleDataSet *ODS;
    TBitBtn *BBOk;
    TBitBtn *BBCancel;
    TDataSource *DataSource1;
    TAmsDBGrid *AmsDBGrid1;
    TStringField *ODSNAME;
    TStringField *ODSDESCR;
    TStringField *ODSREALNAME;
    TStringField *ODSALIAS;
    TIntegerField *ODSDBID;
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall BBCancelClick(TObject *Sender);
protected:
    int Res;
public:		// User declarations
    __fastcall TFrmAskDbAnFld(TComponent* Owner);
    int AskDbAnFld(int&, int);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmAskDbAnFld *FrmAskDbAnFld;
//---------------------------------------------------------------------------
#endif
