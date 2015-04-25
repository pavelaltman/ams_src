//---------------------------------------------------------------------------

#ifndef FRM_ASK_DATEH
#define FRM_ASK_DATEH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ToolEdit.hpp"
#include <Buttons.hpp>
#include <Mask.hpp>
#include <ActnList.hpp>
//---------------------------------------------------------------------------
class TFrmAskDate : public TForm
{
__published:	// IDE-managed Components
    TDateEdit *DE;
    TBitBtn *BBOk;
    TBitBtn *BBCancel;
    TLabel *LabPrompt;
    TActionList *ActionList1;
    TAction *ActOk;
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall BBCancelClick(TObject *Sender);
    void __fastcall ActOkExecute(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmAskDate(TComponent* Owner);
    int AskDate(const AnsiString& Prompt_, TDateTime& Val);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmAskDate *FrmAskDate;
//---------------------------------------------------------------------------
#endif
