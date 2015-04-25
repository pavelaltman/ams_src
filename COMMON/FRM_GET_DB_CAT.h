//---------------------------------------------------------------------------

#ifndef FRM_GET_DB_CATH
#define FRM_GET_DB_CATH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AMS_TREE_VIEW.h"
#include "AmsDbCatView.h"
#include "Oracle.hpp"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ToolWin.hpp>
#include <ActnList.hpp>
//---------------------------------------------------------------------------
class TFrmGetDbCat : public TForm
{
__published:	// IDE-managed Components
    TAmsDbCatView *CV;
    TPanel *Panel1;
    TBitBtn *BBCancel;
    TBitBtn *BBOk;
    TToolBar *ToolBar1;
    TToolButton *TBRefresh;
    TOracleQuery *OQ;
    TActionList *ActionList1;
    TAction *ActOk;
    void __fastcall BBCancelClick(TObject *Sender);
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall TBRefreshClick(TObject *Sender);
    void __fastcall CVKeyPress(TObject *Sender, char &Key);
    void __fastcall CVChanging(TObject *Sender, TTreeNode *Node,
          bool &AllowChange);
    void __fastcall CVChange(TObject *Sender, TTreeNode *Node);
    void __fastcall ActOkExecute(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
private:	// User declarations
    int CurrCode;
    int CurrDbId;
    AnsiString CurrDbName;
public:		// User declarations
    __fastcall TFrmGetDbCat(TComponent* Owner);
    int GetDbCat(int DbId, const AnsiString& DbName, int BegCode= -1);
    int GetDbCatsList(int DbId, const AnsiString& DbName, AnsiString& Res);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmGetDbCat *FrmGetDbCat;
//---------------------------------------------------------------------------
#endif
