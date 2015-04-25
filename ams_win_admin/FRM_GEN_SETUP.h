//---------------------------------------------------------------------------

#ifndef FRM_GEN_SETUPH
#define FRM_GEN_SETUPH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFrmGenSetup : public TForm
{
__published:	// IDE-managed Components
    TCheckBox *ChBAlwaysYes;
    TCheckBox *ChBOnlyBad;
    TButton *Button1;
    void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmGenSetup(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmGenSetup *FrmGenSetup;
//---------------------------------------------------------------------------
#endif
