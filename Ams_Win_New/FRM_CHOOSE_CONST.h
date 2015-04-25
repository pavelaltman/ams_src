//---------------------------------------------------------------------------

#ifndef FRM_CHOOSE_CONSTH
#define FRM_CHOOSE_CONSTH
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
//---------------------------------------------------------------------------
class TFrmChooseConst : public TFrmChooseObj
{
__published:	// IDE-managed Components
    TAction *ActEdit;
private:	// User declarations
public:		// User declarations
    __fastcall TFrmChooseConst(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmChooseConst *FrmChooseConst;
//---------------------------------------------------------------------------
#endif
