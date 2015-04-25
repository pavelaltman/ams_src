//---------------------------------------------------------------------------

#ifndef FRM_HIST_VIEWH
#define FRM_HIST_VIEWH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "OracleData.hpp"
#include <Db.hpp>
#include "RXDBCtrl.hpp"
#include <Buttons.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <ActnList.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TFrmEditDbPos;
//---------------------------------------------------------------------------
class TFrmHistView : public TForm
{
__published:	// IDE-managed Components
    TOracleDataSet *ODS;
    TPanel *Panel1;
    TBitBtn *BBOk;
    TRxDBGrid *RxDBGrid1;
    TDataSource *DataSource1;
    TOracleDataSet *ODS_H;
    TActionList *ActionList1;
    TAction *ActShowOld;
    TAction *ActMakeOk;
    TPopupMenu *PopupMenu1;
    TMenuItem *N1;
    TMenuItem *N2;
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall ActShowOldExecute(TObject *Sender);
    void __fastcall RxDBGrid1DblClick(TObject *Sender);
    void __fastcall ActMakeOkExecute(TObject *Sender);
protected:	// User declarations
    int DbId;
    AnsiString DbName;
    AnsiString Article;
    int Ver;
    TFldsDescr* FldsDescr;
    TOracleDataSet* ParentODS;
    TFrmEditDbPos* FrmEditDbPos;
public:		// User declarations
    __fastcall TFrmHistView(TComponent* Owner);
    void Begin(int DbId_, const AnsiString& DbName_, TFldsDescr* FldsDescr_, TOracleDataSet* ParentODS_);
    void ShowHist (const AnsiString& Article, int Ver);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmHistView *FrmHistView;
//---------------------------------------------------------------------------
#endif
