//---------------------------------------------------------------------------

#ifndef FRM_CHOOSE_OPERH
#define FRM_CHOOSE_OPERH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AmsDBGrid.h"
#include "AmsFooter.h"
#include "FRM_OPER_LIST_ROOT.h"
#include "Oracle.hpp"
#include "OracleData.hpp"
#include "RXDBCtrl.hpp"
#include "RXSplit.hpp"
#include <ActnList.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Db.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include "FRM_BRWS_ACC_WINDOW.h"
class TFrmBrowsAn;
//---------------------------------------------------------------------------
class TFrmChooseOper : public TFrmOperListRoot
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TBitBtn *BBOk;
    TBitBtn *BBCancel;
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall BBCancelClick(TObject *Sender);
    void __fastcall Grid1DblClick(TObject *Sender);
    void __fastcall ActEditExecute(TObject *Sender);
protected:	// User declarations
    TFrmBrowsAn* ParentF;
    int AccCode;
    // Базовый select запрос для поиска
    virtual AnsiString FormFindSql(DS_Q& Q);
    virtual void BuildFullSql   (AnsiString& Sql, DS_Q& Q);
    virtual void KeyEvent (WORD &Key, TShiftState Shift);
public:		// User declarations
    __fastcall TFrmChooseOper(TComponent* Owner);
    void Begin_(TFrmBrowsAn* ParentF_, int AccCode);
    void Begin(int AccCode, const AnsiString& Caption_);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmChooseOper *FrmChooseOper;
//---------------------------------------------------------------------------
#endif
