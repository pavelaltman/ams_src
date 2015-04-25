//---------------------------------------------------------------------------

#ifndef FRM_VIEW_OPTIONS_AWH
#define FRM_VIEW_OPTIONS_AWH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include "BRWS_ACC_WINDOW.h"
#include <ActnList.hpp>
//---------------------------------------------------------------------------
class TFrmViewOptionsAW : public TForm
{
__published:	// IDE-managed Components
    TRadioGroup *RGOpt;
    TCheckBox *ChBCorresp;
    TComboBox *CBDepth;
    TCheckBox *ChBGroupByDate;
    TCheckBox *ChBShowAnal;
    TCheckBox *ChBSortByLastModif;
    TBitBtn *BBOk;
    TBitBtn *BBCancel;
    TCheckBox *ChBCheckOps;
    TActionList *ActionList1;
    TAction *ActOk;
    TLabel *Label1;
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall BBCancelClick(TObject *Sender);
    void __fastcall ActOkExecute(TObject *Sender);
    void __fastcall ChBCorrespClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmViewOptionsAW(TComponent* Owner);
    int ViewOptions(SBrwsAccWindowOptions& CurrOpt);
protected:
    void CheckRules(void);
    bool HaveAnal;
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmViewOptionsAW *FrmViewOptionsAW;
//---------------------------------------------------------------------------
#endif
