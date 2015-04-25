//---------------------------------------------------------------------------
#ifndef FRM_ACCSH
#define FRM_ACCSH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "OracleData.hpp"
#include <Db.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include "AMS_UTILS.h"
//---------------------------------------------------------------------------
class TFrmAccs : public TForm
{
__published:	// IDE-managed Components
  TOracleDataSet *Q;
  TToolBar *ToolBar1;
  TToolButton *ToolButton1;
  TToolButton *ToolButton3;
  TDataSource *DataSource1;
  TDBGrid *DBGrid1;
  void __fastcall ToolButton1Click(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall ToolButton3Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TFrmAccs(TComponent* Owner);
};
//---------------------------------------------------------------------------
//extern PACKAGE TFrmAccs *FrmAccs;
//---------------------------------------------------------------------------
#endif
