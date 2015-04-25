//---------------------------------------------------------------------------

#ifndef FRM_INPUT_SUM_NMH
#define FRM_INPUT_SUM_NMH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ActnList.hpp>
#include "AmsInput.h"
#include "AmsInputStr.h"
#include "AmsInputSum.h"
//---------------------------------------------------------------------------
class TFrmInputOper;
class TFrmInputSumNM : public TForm
{
__published:	// IDE-managed Components
    TBitBtn *BBOk;
    TBitBtn *BBCancel;
    TLabel *Label1;
    TActionList *ActionList1;
    TAction *Action1;
    TAmsInputSum *IS;
        TAction *ActnCalc;
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall Action1Execute(TObject *Sender);
    void __fastcall BBCancelClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall ActnCalcExecute(TObject *Sender);
protected:	// User declarations
    TFrmInputOper* InputOper;
public:		// User declarations
    __fastcall TFrmInputSumNM(TComponent* Owner);
    void InputSumNM(TFrmInputOper* InputOper,
                    const AnsiString& Caption,
                    Sum BeginVal);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmInputSumNM *FrmInputSumNM;
//---------------------------------------------------------------------------
#endif
