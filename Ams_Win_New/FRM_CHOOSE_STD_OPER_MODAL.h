//---------------------------------------------------------------------------

#ifndef FRM_CHOOSE_STD_OPER_MODALH
#define FRM_CHOOSE_STD_OPER_MODALH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AmsStringGrid.h"
#include "FRM_CHOOSE_STD_OPER.h"
#include "Oracle.hpp"
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------
class TFrmChooseStdOperModal : public TFrmChooseStdOper
{
__published:	// IDE-managed Components
private:	// User declarations
    int SelCondId;
public:		// User declarations
    __fastcall TFrmChooseStdOperModal(TComponent* Owner);
    virtual void MakeSomething(void);
    int ChooseStdOper();
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmChooseStdOperModal *FrmChooseStdOperModal;
//---------------------------------------------------------------------------
#endif
