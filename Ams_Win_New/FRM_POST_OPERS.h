//---------------------------------------------------------------------------

#ifndef FRM_POST_OPERSH
#define FRM_POST_OPERSH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AmsDBGrid.h"
#include "FRM_OPER_LIST.h"
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
class TFrmPostOpers : public TFrmOperList
{
__published:	// IDE-managed Components
protected:	// User declarations
    int AccCode;
    virtual AnsiString FormFindSql(DS_Q& Q);
    virtual void BuildFullSql   (AnsiString& Sql, DS_Q& Q);
    AnsiString CurrTitle;
    void FormTitle();
public:		// User declarations
    __fastcall TFrmPostOpers(TComponent* Owner);
    void Begin(int AccCode, const AnsiString& CurrTitle_);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmPostOpers *FrmPostOpers;
//---------------------------------------------------------------------------
#endif
