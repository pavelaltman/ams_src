//---------------------------------------------------------------------------

#ifndef FRM_EDIT_MEMOH
#define FRM_EDIT_MEMOH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TThreadReadFile;
class TFrmEditMemo : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TMainMenu *MainMenu1;
    TMenuItem *N1;
    TMenuItem *N2;
    TMenuItem *N3;
    TMenuItem *N4;
    TMenuItem *N5;
    TBitBtn *BBCancel;
    TBitBtn *BBOk;
    TOpenDialog *OD;
    TSaveDialog *SD;
    TRichEdit *Mem;
        TPrintDialog *PrintDialog1;
        TMenuItem *N6;
    void __fastcall BBCancelClick(TObject *Sender);
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall N2Click(TObject *Sender);
    void __fastcall N3Click(TObject *Sender);
    void __fastcall N5Click(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall N6Click(TObject *Sender);
private:	// User declarations
    TThreadReadFile* ThreadRF;
public:		// User declarations
    __fastcall TFrmEditMemo(TComponent* Owner);
    int EditMemo(TMemo*, const AnsiString& PromptStr);
    void ShowFile(const AnsiString& FileName, const AnsiString& Caption_);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmEditMemo *FrmEditMemo;
//---------------------------------------------------------------------------
#endif
