//---------------------------------------------------------------------------

#ifndef FRM_CHOOSE_FORMH
#define FRM_CHOOSE_FORMH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AmsStringGrid.h"
#include "FRM_CHOOSE_OBJ.h"
#include "Oracle.hpp"
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include "LOCAL_VARS.h"
#include "BRWS_ACC_WINDOW.h"
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
class TFrmChooseForm : public TFrmChooseObj
{
__published:	// IDE-managed Components
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormDestroy(TObject *Sender);
protected:	// User declarations
    int AccCode;
    TGenLocalVars LV;
    bool LocalVarsFormed;
    SBrwsAccWindowOptions CurrOpt;
public:		// User declarations
    __fastcall TFrmChooseForm(TComponent* Owner);
    void BeginForm(int AccCode_);
    void BeginForm(SBrwsAccWindowOptions& CurrOpt);
    virtual void MakeSomething(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmChooseForm *FrmChooseForm;
//---------------------------------------------------------------------------
#endif
