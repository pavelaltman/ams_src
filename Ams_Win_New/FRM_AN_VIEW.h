//---------------------------------------------------------------------------

#ifndef FRM_AN_VIEWH
#define FRM_AN_VIEWH
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
class TFrmAnView : public TFrmBrowsAn
{
__published:	// IDE-managed Components
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
    bool NeedDelAnSum;
public:		// User declarations
    __fastcall TFrmAnView(TComponent* Owner);
    void SumIdChange(bool NeedDelAnSum_,int NewSumId, const AnsiString& SetName_);
    void Begin_(bool NeedDelAnSum_, TForm* Parent_, int AnCode, const AnsiString& AnName,
        const AnsiString& SetName, int AnSumEd_, const AnsiString& BalFld,
        int AccCode_, bool NeedDistr);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmAnView *FrmAnView;
//---------------------------------------------------------------------------
#endif
