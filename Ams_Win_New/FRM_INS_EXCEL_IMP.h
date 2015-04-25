//---------------------------------------------------------------------------

#ifndef FRM_INS_EXCEL_IMPH
#define FRM_INS_EXCEL_IMPH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "OracleData.hpp"
#include <Db.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <DBCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFrmInsExcelImp : public TForm
{
__published:	// IDE-managed Components
    TEdit *EdName;
    TLabel *Label1;
    TMemo *MemDescr;
    TLabel *Label2;
    TOracleDataSet *ODS;
    TDataSource *DataSource1;
    TDBGrid *DBGrid1;
    TDBMemo *DBMemo1;
    TIntegerField *ODSDB_ID;
    TStringField *ODSSHORT_NAME;
    TStringField *ODSDESCR;
    TBitBtn *BitBtn1;
    TBitBtn *BBOk;
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
protected:	// User declarations
    int DbId;
    std::vector<int> *Res;
    TFldsDescr* FldsDescr;
public:		// User declarations
    __fastcall TFrmInsExcelImp(TComponent* Owner);
    void InsExcelImp(int DBId,TFldsDescr* FD, std::vector<int> * Res);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmInsExcelImp *FrmInsExcelImp;
//---------------------------------------------------------------------------
#endif
