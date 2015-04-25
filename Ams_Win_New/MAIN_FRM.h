//---------------------------------------------------------------------------

#ifndef MAIN_FRMH
#define MAIN_FRMH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TMainFrm : public TForm
{
__published:	// IDE-managed Components
    TButton *Button1;
    TStatusBar *SB;
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
public:		// User declarations
    __fastcall TMainFrm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainFrm *MainFrm;
//---------------------------------------------------------------------------
#endif
