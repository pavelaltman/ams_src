//---------------------------------------------------------------------------

#ifndef FRM_CHOOSE_OBJ_ADMH
#define FRM_CHOOSE_OBJ_ADMH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AMS_TREE_VIEW.h"
#include "AmsObjCatView.h"
#include "Oracle.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <ToolWin.hpp>
#include "AmsStringGrid.h"
#include <ActnList.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TFrmChooseObj : public TForm
{
__published:	// IDE-managed Components
    TSplitter *Splitter1;
    TOracleQuery *OQ;
    TPanel *Panel1;
    TToolBar *ToolBar1;
    TTreeView *TV;
    TToolButton *ToolButton1;
    TAmsStringGrid *SG;
    TActionList *ActionList1;
    TAction *ActDelPos;
    TPopupMenu *PopupMenu1;
    TMenuItem *N1;
    void __fastcall ToolButton1Click(TObject *Sender);
    void __fastcall TVChange(TObject *Sender, TTreeNode *Node);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall TVEditing(TObject *Sender, TTreeNode *Node,
          bool &AllowEdit);
    void __fastcall TVKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall SGKeyPress(TObject *Sender, char &Key);
    void __fastcall SGDblClick(TObject *Sender);
    void __fastcall SGResize(TObject *Sender);
private:	// User declarations
    std::vector<TAmsTreeData*> TreeData;
    void ClearTree();
    // Выбранная категория
    int CurrCode;
    // Тип объектов. которые ищем: 1-докум, 2-
    int CurrOType;
    int Cleared;
public:		// User declarations
    __fastcall TFrmChooseObj(TComponent* Owner);
    void Begin(int OType);
protected:
    void RefreshList(void);
    void EditDB (int);
    virtual void MakeSomething(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmChooseObj *FrmChooseObj;
//---------------------------------------------------------------------------
#endif
