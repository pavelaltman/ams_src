//---------------------------------------------------------------------------

#ifndef FRM_FILTER_FORMH
#define FRM_FILTER_FORMH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AmsDBGrid.h"
#include "AmsFooter.h"
#include "FRM_BRWS_ACC_WINDOW.h"
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
class TFrmFilterForm : public TFrmBrwsAccWindow
{
__published:	// IDE-managed Components
    TButton *EscBut;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall EscButClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmFilterForm(TComponent* Owner);
    void SetNewArticle(const AnsiString& Art, int Ver, bool WithoutVer);
    void BeginFilter(int AccCode, const AnsiString& Art, int Ver, bool WithoutVer);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmFilterForm *FrmFilterForm;
//---------------------------------------------------------------------------
#endif
