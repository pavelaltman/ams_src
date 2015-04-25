//---------------------------------------------------------------------------

#ifndef FRM_READ_EXCEL_IMPH
#define FRM_READ_EXCEL_IMPH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "OracleData.hpp"
#include <Db.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
#include <DBCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmReadExcelImp : public TForm
{
__published:	// IDE-managed Components
    TOracleDataSet *ODS;
    TDataSource *DataSource1;
    TDBGrid *DBGrid1;
    TIntegerField *ODSDB_ID;
    TStringField *ODSSHORT_NAME;
    TStringField *ODSDESCR;
    TDBMemo *DBMemo1;
    TBitBtn *BitBtn1;
    TBitBtn *BBOk;
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
    std::vector<int> * Res;
    TFldsDescr* FD;
    int DbId;
public:		// User declarations
    __fastcall TFrmReadExcelImp(TComponent* Owner);
    int ReadExcelImp(int DbId_, TFldsDescr* FD_, std::vector<int> * Res_);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmReadExcelImp *FrmReadExcelImp;
//---------------------------------------------------------------------------
#endif
