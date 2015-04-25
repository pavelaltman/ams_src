//---------------------------------------------------------------------------

#ifndef TestH
#define TestH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Oracle.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ActnList.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TFrmScript : public TForm
{
__published:	// IDE-managed Components
    TOracleScript *OScript;
    TMemo *Mem;
    TStatusBar *StatusBar1;
    TSplitter *Splitter1;
    TOracleSession *OSession;
    TActionList *ActionList1;
    TAction *ActExec;
    TAction *ActLogon;
    TMainMenu *MainMenu1;
    TOracleLogon *OLogon;
    TMemo *MemRes;
    TPopupMenu *PMenu;
    TAction *ActWhoAmI;
    TAction *ActClose;
    TAction *ActOpen;
    TAction *ActSave;
    TMenuItem *N1;
    TMenuItem *N2;
    TMenuItem *N4;
    TMenuItem *N5;
    TMenuItem *N6;
    TMenuItem *N7;
    TOpenDialog *OD;
    TSaveDialog *SD;
    TAction *ActChCoding;
    TMenuItem *N3;
    TMenuItem *N8;
    TMenuItem *N9;
    TMenuItem *N10;
    TMenuItem *N11;
    TAction *Action1;
    void __fastcall OScriptOutput(TOracleScript *Sender,
          const AnsiString Msg);
    void __fastcall ActLogonExecute(TObject *Sender);
    void __fastcall MemChange(TObject *Sender);
    void __fastcall ActOpenExecute(TObject *Sender);
    void __fastcall ActSaveExecute(TObject *Sender);
    void __fastcall RxTrayIcon1Click(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall ActChCodingExecute(TObject *Sender);
    void __fastcall ActExecExecute(TObject *Sender);
    void __fastcall N6Click(TObject *Sender);
protected:	// User declarations
    int Dirty;
    AnsiString CurrFileName;
    int WinCoding;
    int ModalForm;
public:		// User declarations
    __fastcall TFrmScript(TComponent* Owner);
    int GetScriptText(AnsiString& ScriptText, const AnsiString& CaptionText);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmScript *FrmScript;
//---------------------------------------------------------------------------
#endif


