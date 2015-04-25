//---------------------------------------------------------------------------
#ifndef Frm_Cat_TreeH
#define Frm_Cat_TreeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Oracle.hpp"
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <Buttons.hpp>
#include <vector>
#include "AMS_UTILS.h"
//---------------------------------------------------------------------------
class TFrmCatTree : public TForm
{
__published:	// IDE-managed Components
  TTreeView *TV;
  TOracleQuery *Q;
  TToolBar *ToolBar1;
  TToolButton *SelObjCat;
  void __fastcall TVChange(TObject *Sender, TTreeNode *Node);
  void __fastcall SelObjCatClick(TObject *Sender);
private:	// User declarations
protected:
   TCatNewEvent CatNewEvent;
   std::map<int,TTreeNode*> Mem;  
public:		// User declarations
  __fastcall TFrmCatTree(TComponent* Owner);
  AnsiString GetSelected()
  {
    if (TV->Selected)
        return TV->Selected->Text;
    else
        return "";
  };
  //std::vector<int> Categories;
  void ShowCategories (const AnsiString& BaseUserName_, int OType, TCatNewEvent CatNewEv);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmCatTree *FrmCatTree;
//---------------------------------------------------------------------------
#endif
