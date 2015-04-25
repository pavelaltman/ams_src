//---------------------------------------------------------------------------

#ifndef FRM_SEL_CPS_DATEH
#define FRM_SEL_CPS_DATEH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmSelCpsDate : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TListBox *LBDates;
    TBitBtn *BBOk;
    TBitBtn *BBCancel;
    void __fastcall BBCancelClick(TObject *Sender);
    void __fastcall BBOkClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmSelCpsDate(TComponent* Owner);
    int SelCpsDate(TDateTime& BegDate);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmSelCpsDate *FrmSelCpsDate;
//---------------------------------------------------------------------------
#endif
