//---------------------------------------------------------------------------

#ifndef FRM_INPUT_VALUEH
#define FRM_INPUT_VALUEH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFrmInputValue : public TForm
{  
__published:    // IDE-managed Components
    TLabel *LabPrompt;
    TEdit *EdValue;
    TBitBtn *BBCancel;
    TBitBtn *BBOk;
    void __fastcall BBCancelClick(TObject *Sender);
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
private:    // User declarations
public:     // User declarations
    __fastcall TFrmInputValue(TComponent* Owner);
    // ¬вод строки пользователем
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmInputValue *FrmInputValue;
//---------------------------------------------------------------------------
#endif
