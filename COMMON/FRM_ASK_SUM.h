//---------------------------------------------------------------------------

#ifndef FRM_ASK_SUMH
#define FRM_ASK_SUMH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AmsInput.h"
#include "AmsInputStr.h"
#include "AmsInputSum.h"
#include <Buttons.hpp>
#include <ActnList.hpp>
//---------------------------------------------------------------------------
class TFrmAskSum : public TForm
{
__published:	// IDE-managed Components
    TBitBtn *BBOk;
    TBitBtn *BBCancel;
    TLabel *Label1;
    TActionList *ActionList1;
    TAction *ActOk;
        TAmsInputSum *IS;
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall BBCancelClick(TObject *Sender);
    void __fastcall ActOkExecute(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmAskSum(TComponent* Owner);
    int AskSum(const AnsiString& UserPrompt, double& ResValue);
    int AskSumNM(const AnsiString& UserPrompt, double& ResValue);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmAskSum *FrmAskSum;
//---------------------------------------------------------------------------
#endif
