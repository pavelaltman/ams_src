//---------------------------------------------------------------------------

#ifndef FRM_CHOOSE_NOT_PLAIN_PERIODH
#define FRM_CHOOSE_NOT_PLAIN_PERIODH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ToolEdit.hpp"
#include <Buttons.hpp>
#include <Mask.hpp>
#include <ActnList.hpp>
#include "RXSpin.hpp"
//---------------------------------------------------------------------------
class TFrmChooseNotPlainPeriod : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TDateEdit *DEBeg;
    TDateEdit *DEEnd;
    TCheckBox *ChBSyntOnly;
    TBitBtn *BitBtn1;
    TBitBtn *BBOk;
    TLabel *Label2;
    TActionList *ActionList1;
    TAction *Action1;
    TCheckBox *ChBJustAdd;
    TRxSpinEdit *SpEdit;
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall Action1Execute(TObject *Sender);
    void __fastcall ChBJustAddClick(TObject *Sender);
    void __fastcall SpEditChange(TObject *Sender);
    void __fastcall DEBegChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmChooseNotPlainPeriod(TComponent* Owner);
    bool ChooseNotPlainPeriod(TDateTime& BegPeriod, TDateTime& EndPeriod, bool& SyntOnly);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmChooseNotPlainPeriod *FrmChooseNotPlainPeriod;
//---------------------------------------------------------------------------
#endif
