//---------------------------------------------------------------------------

#ifndef FRM_OPER_HISTH
#define FRM_OPER_HISTH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "OracleData.hpp"
#include "RXDBCtrl.hpp"
#include <Buttons.hpp>
#include <Db.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TFrmOperHist : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TBitBtn *BBOk;
    TRxDBGrid *RxDBGrid1;
    TOracleDataSet *ODS;
    TDataSource *DataSource1;
private:	// User declarations
public:		// User declarations
    __fastcall TFrmOperHist(TComponent* Owner);
    void ShowHist (int _op_id, int _op_subid) ;
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOperHist *FrmOperHist;
//---------------------------------------------------------------------------
#endif
