//---------------------------------------------------------------------------

#ifndef FRM_EXITH
#define FRM_EXITH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFrmExit : public TForm
{
__published:	// IDE-managed Components
    TBitBtn *mbYes;
    TBitBtn *mbNo;
    TLabel *Label1;
    void __fastcall mbYesClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmExit(TComponent* Owner);
    int __fastcall ShowForm(AnsiString QueryStr);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmExit *FrmExit;
//---------------------------------------------------------------------------
#endif
