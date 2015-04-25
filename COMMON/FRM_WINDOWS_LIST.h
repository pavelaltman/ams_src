//---------------------------------------------------------------------------

#ifndef FRM_WINDOWS_LISTH
#define FRM_WINDOWS_LISTH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFrmWindowsList : public TForm
{
__published:	// IDE-managed Components
    TListBox *LBWin;
    void __fastcall LBWinKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall LBWinClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
    TForm* PF ;
public:		// User declarations
    __fastcall TFrmWindowsList(TComponent* Owner);
    TForm* SelWindow(TForm * F);

};
//---------------------------------------------------------------------------
extern PACKAGE TFrmWindowsList *FrmWindowsList;
//---------------------------------------------------------------------------
#endif
