//---------------------------------------------------------------------------
#ifndef FRM_ACCS_STRH
#define FRM_ACCS_STRH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include "Oracle.hpp"
#include "AMS_UTILS.h"

//---------------------------------------------------------------------------
class TFrmAccsStr : public TForm
{
__published:	// IDE-managed Components
  TToolBar *ToolBar1;
  TToolButton *SelAccs;
  TTreeView *TV;
  TOracleQuery *Q;
  TToolButton *ToolButton2;
  void __fastcall SelAccsClick(TObject *Sender);
  void __fastcall TVChange(TObject *Sender, TTreeNode *Node);
  void __fastcall ToolButton2Click(TObject *Sender);
protected:	// User declarations
  bool Ready;
  TAccNewEvent AccNewEvent;
  AnsiString Schema;
  AnsiString BeginAcc;
  
  void Clear();
public:		// User declarations
  __fastcall TFrmAccsStr(TComponent* Owner);
  void Begin(const AnsiString& SchemaName, 
             const AnsiString& BeginAcc_, TAccNewEvent AccNewEv );

};
//---------------------------------------------------------------------------
//extern PACKAGE TFrmAccsStr *FrmAccsStr;
//---------------------------------------------------------------------------
#endif
