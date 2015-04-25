//---------------------------------------------------------------------------

#ifndef FRM_OPER_LIST_TIED_OPERSH
#define FRM_OPER_LIST_TIED_OPERSH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AmsDBGrid.h"
#include "FRM_OPER_LIST_ROOT.h"
#include "Oracle.hpp"
#include "OracleData.hpp"
#include "RXDBCtrl.hpp"
#include "RXSplit.hpp"
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <Db.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------
class TFrmOperListTiedOpers : public TFrmOperListRoot
{
__published:	// IDE-managed Components
private:	// User declarations
public:		// User declarations
    __fastcall TFrmOperListTiedOpers(TComponent* Owner);
    void Init(int _post=0, int _del=0, int _bound=0,AnsiString _text="",int _allowed=0) ;
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOperListTiedOpers *FrmOperListTiedOpers;
//---------------------------------------------------------------------------
#endif
