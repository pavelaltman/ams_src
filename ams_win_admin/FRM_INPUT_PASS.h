//---------------------------------------------------------------------------

#ifndef FRM_INPUT_PASSH
#define FRM_INPUT_PASSH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFrmInputPass : public TForm
{
__published:	// IDE-managed Components
    TEdit *EdPass;
    TButton *Button1;
    void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmInputPass(TComponent* Owner);
    AnsiString InputPass(const AnsiString& Cap);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmInputPass *FrmInputPass;
//---------------------------------------------------------------------------
#endif
