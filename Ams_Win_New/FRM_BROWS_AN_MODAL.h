//---------------------------------------------------------------------------

#ifndef FRM_BROWS_AN_MODALH
#define FRM_BROWS_AN_MODALH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AMS_TREE_VIEW.h"
#include "AmsDbCatView.h"
#include "AmsDBGrid.h"
#include "AmsFooter.h"
#include "FRM_BROWS_AN.h"
#include "Oracle.hpp"
#include "OracleData.hpp"
#include "RXDBCtrl.hpp"
#include <ActnList.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Db.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------
class TFrmBrowsAnModal : public TFrmBrowsAn
{
__published:	// IDE-managed Components
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall BBCancelClick(TObject *Sender);
    void __fastcall ActOk_Execute(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmBrowsAnModal(TComponent* Owner);
    int BrowsAnModal (TForm* Parent_,int AnCode, const AnsiString& AnName,
        const AnsiString& SetName, const AnsiString& Descr,
        int AnSumEd_, int AnSumOst_, const BegMode& BM= BegMode());
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmBrowsAnModal *FrmBrowsAnModal;
//---------------------------------------------------------------------------
#endif
