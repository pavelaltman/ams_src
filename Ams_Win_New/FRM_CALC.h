//---------------------------------------------------------------------------

#ifndef FRM_CALCH
#define FRM_CALCH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include "RXSpin.hpp"
#include <Mask.hpp>
#include "Placemnt.hpp"
//---------------------------------------------------------------------------
class TFrmCalc : public TForm
{
__published:	// IDE-managed Components
    TComboBox *ChB;
    TLabel *Label1;
    TEdit *EdRes;
    TButton *ButCalc;
    TButton *ButClose;
    TActionList *ActionList1;
    TAction *ActOk;
    TCheckBox *ChBRoundUp;
    TRxSpinEdit *SEKolZnak;
        TFormStorage *FormStorage1;
    void __fastcall ButCalcClick(TObject *Sender);
    void __fastcall ButCloseClick(TObject *Sender);
    void __fastcall ActOkExecute(TObject *Sender);
    void __fastcall ChBRoundUpClick(TObject *Sender);
    void __fastcall SEKolZnakChange(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmCalc(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmCalc *FrmCalc;
//---------------------------------------------------------------------------
#endif
