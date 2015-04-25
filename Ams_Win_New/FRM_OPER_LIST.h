//---------------------------------------------------------------------------

#ifndef FRM_OPER_LISTH
#define FRM_OPER_LISTH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AmsDBGrid.h"
#include "FRM_OPER_LIST_ROOT.h"
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
#include "Oracle.hpp"
//---------------------------------------------------------------------------
class TFrmOperList : public TFrmOperListRoot
{
__published:	// IDE-managed Components
private:	// User declarations
public:		// User declarations
    __fastcall TFrmOperList(TComponent* Owner);  
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOperList *FrmOperList;
//---------------------------------------------------------------------------
#endif
