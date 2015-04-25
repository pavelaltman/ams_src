//---------------------------------------------------------------------------

#ifndef FRM_USER_OPTSH
#define FRM_USER_OPTSH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "RXSpin.hpp"
#include <ExtCtrls.hpp>
#include "ToolEdit.hpp"
#include <Mask.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFrmUserOpts : public TForm
{
__published:	// IDE-managed Components
    TShape *Shape1;
    TLabel *Label1;
    TLabel *Label2;
    TRxSpinEdit *SESum;
    TLabel *Label3;
    TRxSpinEdit *SEKop;
    TCheckBox *ChBApps;
    TShape *Shape2;
    TLabel *Label4;
    TLabel *Label5;
    TDateEdit *DatInp;
    TCheckBox *ChBCurrDate;
    TCheckBox *ChBSortByDocNum;
    TCheckBox *ChBGotoEnd;
    TBitBtn *BBOk;
    TBitBtn *BBCancel;
    void __fastcall BBCancelClick(TObject *Sender);
    void __fastcall BBOkClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmUserOpts(TComponent* Owner);
    int UserOpts();
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmUserOpts *FrmUserOpts;
//---------------------------------------------------------------------------
#endif
