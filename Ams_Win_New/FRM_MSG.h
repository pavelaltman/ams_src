//---------------------------------------------------------------------------

#ifndef FRM_MSGH
#define FRM_MSGH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmMsg : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TBitBtn *BBOk;
    void __fastcall Panel1Resize(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmMsg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMsg *FrmMsg;
//---------------------------------------------------------------------------
#endif
