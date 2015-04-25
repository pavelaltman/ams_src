//---------------------------------------------------------------------------

#ifndef FRM_GET_SET_NAMEH
#define FRM_GET_SET_NAMEH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFrmGetSetName : public TForm
{
__published:	// IDE-managed Components
    TComboBox *CBSetName;
    TBitBtn *BBOk;
private:	// User declarations
public:		// User declarations
    __fastcall TFrmGetSetName(TComponent* Owner);
    AnsiString GetSetName(int OID, const AnsiString& Prompt_);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmGetSetName *FrmGetSetName;
//---------------------------------------------------------------------------
#endif
