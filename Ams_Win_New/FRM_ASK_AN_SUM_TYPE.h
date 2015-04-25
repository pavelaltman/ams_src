//---------------------------------------------------------------------------

#ifndef FRM_ASK_AN_SUM_TYPEH
#define FRM_ASK_AN_SUM_TYPEH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmAskAnalSumType : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TBitBtn *BitBtn1;
    TBitBtn *BBOk;
    TRadioGroup *RGSel;
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmAskAnalSumType(TComponent* Owner);
    int AskAnalSumType();
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmAskAnalSumType *FrmAskAnalSumType;
//---------------------------------------------------------------------------
#endif
