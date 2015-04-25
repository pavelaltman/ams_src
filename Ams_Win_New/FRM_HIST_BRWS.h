//---------------------------------------------------------------------------

#ifndef FRM_HIST_BRWSH
#define FRM_HIST_BRWSH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AMS_TREE_VIEW.h"
#include "AmsDbCatView.h"
#include "FRM_BROWS_DB.h"
#include "Oracle.hpp"
#include "OracleData.hpp"
#include "RXDBCtrl.hpp"
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <Db.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include "AmsDBGrid.h"
//---------------------------------------------------------------------------
class TFrmHistBrws : public TFrmBrowsDb
{
__published:	// IDE-managed Components
protected:	// User declarations
    virtual void BuildSqlQuery(TOracleDataSet*);
public:		// User declarations
    virtual void Begin(int DBID, const AnsiString& DBName,
                const AnsiString& DBDescr,
                const AnsiString& SetName);
    __fastcall TFrmHistBrws(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmHistBrws *FrmHistBrws;
//---------------------------------------------------------------------------
#endif
