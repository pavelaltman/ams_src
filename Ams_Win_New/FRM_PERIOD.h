//---------------------------------------------------------------------------

#ifndef FRM_PERIODH
#define FRM_PERIODH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "OracleData.hpp"
#include "RXDBCtrl.hpp"
#include <Db.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFrmPeriod : public TForm
{
__published:	// IDE-managed Components
    TDataSource *DSrc;
    TOracleDataSet *DSet;
    TRxDBGrid *RxDBGrid1;
    TRxDBGrid *RxDBGrid2;
    TDataSource *DSrc1;
    TOracleDataSet *DSet1;
    TBitBtn *BBOk;
    TBitBtn *BBCancel;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall BBCancelClick(TObject *Sender);
    void __fastcall BBOkClick(TObject *Sender);
private:	// User declarations
public:		// User declarations

    TDateTime DT_beg , DT_end ;
    void SetInitPeriod(TDateTime beg,TDateTime end) ;
    __fastcall TFrmPeriod(TComponent* Owner) ;
    int Period(TDateTime& BegP, TDateTime& EndP);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmPeriod *FrmPeriod;
//---------------------------------------------------------------------------
#endif
