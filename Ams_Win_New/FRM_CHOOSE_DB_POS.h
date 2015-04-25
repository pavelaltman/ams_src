//---------------------------------------------------------------------------

#ifndef FRM_CHOOSE_DB_POSH
#define FRM_CHOOSE_DB_POSH
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
#include <ActnList.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Db.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include "RXDBCtrl.hpp"
#include "AmsDBGrid.h"
//---------------------------------------------------------------------------
class TFrmChooseDbPos : public TFrmBrowsDb
{
__published:	// IDE-managed Components
    TAction *ActOk;
    TPanel *Panel2;
    TBitBtn *BitBtn1;                
    TBitBtn *BBOk;
    void __fastcall ActOkExecute(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall DBGFrosenDblClick(TObject *Sender);
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall ButEnterClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
    bool filterwas;
public:		// User declarations
    __fastcall TFrmChooseDbPos(TComponent* Owner);
    bool GetWithoutVer() {return WithoutVer;};
    // ¬ыбор позиции базы данных пользователем
    int SelectDbPos (AnsiString& Article, int& Ver, const AnsiString& SetName);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmChooseDbPos *FrmChooseDbPos;
//---------------------------------------------------------------------------
#endif
