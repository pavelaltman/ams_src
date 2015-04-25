//---------------------------------------------------------------------------

#ifndef FRM_BUILD_BUSY_PROGH
#define FRM_BUILD_BUSY_PROGH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "Oracle.hpp"
//---------------------------------------------------------------------------
class TFrmBuildBusyProg : public TForm
{
__published:	// IDE-managed Components
    TMemo *MemSql;
    TPanel *Panel1;
    TBitBtn *BBCancel;
    TBitBtn *BBApply;
    TOracleQuery *DBQ;
    TOracleQuery *AnQ;
    TOracleQuery *ODbLinks;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall BBApplyClick(TObject *Sender);
    void __fastcall BBCancelClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmBuildBusyProg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmBuildBusyProg *FrmBuildBusyProg;
//---------------------------------------------------------------------------
#endif
