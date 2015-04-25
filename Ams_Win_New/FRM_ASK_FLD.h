//---------------------------------------------------------------------------

#ifndef FRM_ASK_FLDH
#define FRM_ASK_FLDH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AmsInput.h"
#include "AmsInputDate.h"
#include "AmsInputInt.h"
#include "AmsInputStr.h"
#include "AmsInputSum.h"
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TBitBtn *BBOk;
    TBitBtn *BBCancel;
    TAmsInputDate *AmsInputDate1;
    TAmsInputSum *AmsInputSum1;
    TAmsInputStr *AmsInputStr1;
    TAmsInputInt *AmsInputInt1;
    void __fastcall BBCancelClick(TObject *Sender);
    void __fastcall BBOkClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
