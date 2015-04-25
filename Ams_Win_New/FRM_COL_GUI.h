//---------------------------------------------------------------------------

#ifndef FRM_COL_GUIH
#define FRM_COL_GUIH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <ImgList.hpp>
#include <Map.h>
#include <Vector.h>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include "ams_win_c.h"
//---------------------------------------------------------------------------
class TFrmColGUI : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel3;
    TPanel *Panel4;
        TImage *Im7;
        TImage *Im4;
        TImage *Im1;
        TImage *Im6;
        TImage *Im5;
        TImage *Im2;
        TImage *Im3;
    void __fastcall Im4Click(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall Im7Click(TObject *Sender);
    void __fastcall Im6Click(TObject *Sender);
    void __fastcall Im5Click(TObject *Sender);
    void __fastcall Im1Click(TObject *Sender);
    void __fastcall Im2Click(TObject *Sender);
    void __fastcall Im3Click(TObject *Sender);
private:
    void __fastcall Begin();
public:		// User declarations
    __fastcall TFrmColGUI(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmColGUI *FrmColGUI;
//---------------------------------------------------------------------------
#endif
