//---------------------------------------------------------------------------

#ifndef FRM_ASK_ACC_CODEH
#define FRM_ASK_ACC_CODEH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AMS_TREE_VIEW.h"
#include "AmsAccsView.h"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include "Oracle.hpp"
//---------------------------------------------------------------------------
class TFrmAskAccCode : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TAmsAccsView *AccsView;
    TBitBtn *BBCancel;
    TBitBtn *BBOk;
    TOracleQuery *OracleQuery1;
    TEdit *EdAccCode;
    void __fastcall BBCancelClick(TObject *Sender);
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall EdAccCodeChange(TObject *Sender);
    void __fastcall AccsViewDblClick(TObject *Sender);
protected:	// User declarations
    void MakeSelection();
    bool __fastcall WantChildKey(Controls::TControl* Child,
                            Messages::TMessage &Message);

public:		// User declarations
    __fastcall TFrmAskAccCode(TComponent* Owner);
    int AskAccCode(const AnsiString& BaseUser, int BegCode, int FirstNode=0);
    int AskAccCode(const AnsiString& BaseUser, const AnsiString& BegCode, int FirstNode=0);
    AnsiString AskAccName(const AnsiString& BaseUser, int BegCode, int FirstNode=0);
    AnsiString AskAccName(const AnsiString& BaseUser, const AnsiString& BegCode, int FirstNode=0);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmAskAccCode *FrmAskAccCode;
//---------------------------------------------------------------------------
#endif
