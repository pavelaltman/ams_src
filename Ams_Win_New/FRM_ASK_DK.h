//---------------------------------------------------------------------------

#ifndef FRM_ASK_DKH
#define FRM_ASK_DKH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmAskDK : public TForm
{
__published:	// IDE-managed Components
    TBitBtn *BBOk;
    TBitBtn *BBCancel;
    TRadioGroup *RGr;
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall BBCancelClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmAskDK(TComponent* Owner);
    AnsiString AskDK();
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmAskDK *FrmAskDK;
//---------------------------------------------------------------------------
#endif
