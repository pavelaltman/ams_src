//---------------------------------------------------------------------------

#ifndef FRM_CONDS_EDITH
#define FRM_CONDS_EDITH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBCtrls.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "OracleData.hpp"
#include <Db.hpp>
//---------------------------------------------------------------------------
#include "AMS_UTILS.h"
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include "Oracle.hpp"

class TFrmCondsEdit : public TForm
{
__published:	// IDE-managed Components
    TDBGrid *DBGridObjs;
    TPanel *Panel1;
    TSplitter *Splitter3;
    TDBGrid *DBGrid3;
    TDBGrid *DBGrid2;
    TSplitter *Splitter1;
    TSplitter *Splitter2;
    TDataSource *DataSource3;
    TOracleDataSet *QCondItems;
    TDataSource *DataSource1;
    TOracleDataSet *QObjs;
    TDataSource *DataSource2;
    TOracleDataSet *QConds;
    TToolBar *ToolBar1;
    TToolButton *ToolButton1;
    TToolButton *ToolButton2;
    TToolButton *ToolButton3;
    TOracleQuery *OQ;
    TOracleQuery *OQ2;
    TPanel *Panel2;
    TDBMemo *DBMemo1;
    TSplitter *Splitter4;
    TDBMemo *DBMemo2;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall ToolButton2Click(TObject *Sender);
    void __fastcall ToolButton1Click(TObject *Sender);
    void __fastcall ToolButton3Click(TObject *Sender);
protected:	// User declarations
    // Форма с категориями
    TFrmCatTree* Cats;
    bool CloseByUser;
    void __fastcall NewCatChoose(TFrmCatTree* Ct, int CatId);

    AnsiString CurrSchemaName;
    //====================================================
    // Для режима копирования проводок в другую бухгалтерию
    AnsiString WhereCopyConds;
    TFrmCatTree* WCCats;
    void __fastcall WCNewCatChoose(TFrmCatTree* Ct, int CatId);
public:		// User declarations
    __fastcall TFrmCondsEdit(TComponent* Owner);
    Begin(const AnsiString& SchemaName);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmCondsEdit *FrmCondsEdit;
//---------------------------------------------------------------------------
#endif
