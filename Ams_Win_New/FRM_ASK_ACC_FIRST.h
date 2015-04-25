//---------------------------------------------------------------------------

#ifndef FRM_ASK_ACC_FIRSTH
#define FRM_ASK_ACC_FIRSTH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFrmAskAccCode;
//---------------------------------------------------------------------------
class TFrmAskAccFirst : public TForm
{
__published:	// IDE-managed Components
    TEdit *EdAcc;
    TBitBtn *BBOk;
    TButton *ButChooseAcc;
    TBitBtn *BBCancel;
    void __fastcall ButChooseAccClick(TObject *Sender);
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall BBCancelClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
protected:	// User declarations
    TFrmAskAccCode* FrmAskAccCode;
public:		// User declarations
    __fastcall TFrmAskAccFirst(TComponent* Owner);
    AnsiString GetAccName();
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmAskAccFirst *FrmAskAccFirst;
//---------------------------------------------------------------------------
#endif
