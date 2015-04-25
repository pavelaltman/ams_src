//---------------------------------------------------------------------------

#ifndef FRM_COLORGUIH
#define FRM_COLORGUIH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ActnList.hpp>
//---------------------------------------------------------------------------
class TFrmColorGUI : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TImage *imLT;
    TImage *imRT;
    TImage *imLB;
    TImage *imMB;
    TImage *imRB;
    void __fastcall imLTMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall imRTMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall imLBMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall imMBMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall imRBMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
private:	// User declarations
    bool    bLT;
    bool    bRT;
    bool    bLB;
    bool    bMB;
    bool    bRB;
    void __fastcall SetFalse();
public:		// User declarations
    __fastcall TFrmColorGUI(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmColorGUI *FrmColorGUI;
//---------------------------------------------------------------------------
#endif
