//---------------------------------------------------------------------------

#ifndef FRM_INPUT_HISTORYH
#define FRM_INPUT_HISTORYH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFrmInputHistory : public TForm
{
__published:	// IDE-managed Components
    TComboBox *CBInpHist;
    void __fastcall CBInpHistKeyPress(TObject *Sender, char &Key);
    void __fastcall CBInpHistChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmInputHistory(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmInputHistory *FrmInputHistory;
//---------------------------------------------------------------------------
#endif
