//---------------------------------------------------------------------------

#ifndef FRM_OPER_LIST_ROOT_H
#define FRM_OPER_LIST_ROOT_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "OracleData.hpp"
#include "RXDBCtrl.hpp"
#include <Db.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include "OracleNavigator.hpp"
#include <ExtCtrls.hpp>
#include "RXSplit.hpp"
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include "AmsDBGrid.h"
#include "Oracle.hpp"
//---------------------------------------------------------------------------
class TFrmFindDbPos;
class TFrmSortOrder;
class TFrmShowTiedDb;
//---------------------------------------------------------------------------
class TFrmOperListRoot : public TForm
{
__published:	// IDE-managed Components
    TDataSource *DSrc;
    TOracleDataSet *DSet;
    TOracleDataSet *DSetItems;
    TDataSource *DSrcItems;
    TRxDBGrid *Grid2;
    TStringField *DSetItemsASIDE;
    TStringField *DSetItemsANAME;
    TStringField *DSetItemsANALINFO;
    TFloatField *DSetItemsVALUE;
    TLabel *Label1;
    TActionList *ActionList1;
    TAction *ActEdit;
    TAction *ActS;
    TAction *ActDel;
    TAction *ActRead;
    TAction *ActSel;
    TAction *ActNew;
    TAction *ActAll;
    TAction *ActFind;
    TAction *ActFindAg;
    TAction *ActSort;
    TAction *ActAnSort;
    TAction *ActPer;
    TAction *ActPrint;
    TAction *ActHist;
    TPopupMenu *PopupMenu1;
    TMenuItem *N1;
    TMenuItem *Enter1;
    TMenuItem *Alt1;
    TMenuItem *N2;
    TMenuItem *N3;
    TMenuItem *Alt4;
    TMenuItem *N4;
    TMenuItem *N5;
    TMenuItem *N6;
    TMenuItem *N7;
    TMenuItem *N8;
    TMenuItem *N9;
    TMenuItem *Alt2;
    TMenuItem *Alt3;
    TToolBar *ToolBar1;
    TToolButton *ToolButton1;
    TToolButton *ToolButton2;
    TToolButton *ToolButton4;
    TToolButton *ToolButton5;
    TToolButton *ToolButton6;
    TToolButton *ToolButton7;
    TToolButton *ToolButton8;
    TToolButton *ToolButton9;
    TToolButton *ToolButton10;
    TToolButton *ToolButton11;
    TToolButton *ToolButton12;
    TToolButton *ToolButton13;
    TToolButton *ToolButton14;
    TToolButton *ToolButton3;
    TRxSplitter *RxSplitter1;
    TAmsDBGrid *Grid1;
    TOracleQuery *FindQ;
    TAction *ActShowTiedBase;
    TMenuItem *MenShowDB;
    TButton *Button1;
    TTimer *TMAScroll;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall ActDelExecute(TObject *Sender);
    void __fastcall DSetApplyRecord(TOracleDataSet *Sender, char Action,
          bool &Applied, AnsiString &NewRowId);
    void __fastcall ActHistExecute(TObject *Sender);
    void __fastcall ActPerExecute(TObject *Sender);
    void __fastcall ActSExecute(TObject *Sender);
    void __fastcall ActEditExecute(TObject *Sender);
    void __fastcall ActReadExecute(TObject *Sender);
    void __fastcall ActNewExecute(TObject *Sender);
    void __fastcall Grid3KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall Grid1DblClick(TObject *Sender);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall ActFindExecute(TObject *Sender);
    void __fastcall ActFindAgExecute(TObject *Sender);
    void __fastcall ActSelExecute(TObject *Sender);
    void __fastcall ActSortExecute(TObject *Sender);
    void __fastcall ActAnSortExecute(TObject *Sender);
    void __fastcall ActAllExecute(TObject *Sender);
    void __fastcall ActShowTiedBaseExecute(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall TMAScrollTimer(TObject *Sender);
    void __fastcall DSetAfterScroll(TDataSet *DataSet);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall ActPrintExecute(TObject *Sender);
        void __fastcall Grid1GetCellParams(TObject *Sender, TField *Field,
          TFont *AFont, TColor &Background, bool Highlight);
protected:	// User declarations
    virtual void KeyEvent (WORD &Key, TShiftState Shift);
    bool ShowTiedBase;
    TFrmShowTiedDb* FrmShowTiedDb;
protected:
    bool SortSetted;
    AnsiString SortTxt;
    TFldsDescr FldsDescr;
    TFrmSortOrder* FrmSortOrder;
    TFrmFindDbPos* FrmFind;
    bool FindSetted;
    TFrmFindDbPos* FrmFilter;
    bool FilterSetted;
    // вызывается Если изменился фильтр
    virtual void ChangedFilter();
    // вызывается Если изменилась сортировка
    virtual void ChangedOrder();
    // Пользователь начал новый поиск
    virtual void FindOk();
    virtual void FindAgain();
    // Базовый select запрос для поиска
    virtual AnsiString FormFindSql(DS_Q& Q);
    // Подгонка FullName для выбора из разных таблиц
    virtual void UpdFldsDescr();
    AnsiString FormName;
    virtual void BuildFullSql   (AnsiString& Sql, DS_Q& Q);
    virtual void GetFromAndWhere(AnsiString& Sql, DS_Q& Q);
    virtual void GetOrderBy(AnsiString& Sql);
public:		// User declarations
    __fastcall TFrmOperListRoot(TComponent* Owner);

    void ChildFormClosed(TForm* Frm);

    TDateTime Beg , End ; // Начало и конец периода видимости
    int post ; // Отложенные операции
    int del ; // Удаленные операции
    int bound ; // Связанные операции
    AnsiString bound_parent ;
    int allowed ; // Разрешенные

    void Init(int _post=0, int _del=0, int _bound=0,AnsiString _text="",int _allowed=0) ;
    void FormTitle() ;
    virtual void RefreshAfterOper(bool otlozh=false);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmOperListRoot *FrmOperListRoot;
//---------------------------------------------------------------------------
#endif
