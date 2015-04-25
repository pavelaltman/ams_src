//---------------------------------------------------------------------------

#ifndef FRM_BROWS_DBH
#define FRM_BROWS_DBH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AMS_TREE_VIEW.h"
#include "AmsDbCatView.h"
#include "Oracle.hpp"
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include "OracleData.hpp"
#include <Db.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <ActnList.hpp>
#include <DBTables.hpp>
#include <Menus.hpp>
#include <DBActns.hpp>
#include <Buttons.hpp>
#include <Tabs.hpp>
#include "RXDBCtrl.hpp"
#include "AmsDBGrid.h"
#include "prog.h"

class TFrmEditDbPos;
class TFrmFindDbPos;
class TFrmSortOrder;
class TFrmQuickSearch;
class FieldSetM;
class OpVars;
//---------------------------------------------------------------------------
class TFrmBrowsDb : public TForm
{
__published:	// IDE-managed Components
    TOracleQuery *OQ;
    TAmsDbCatView *CV;
    TSplitter *Splitter1;
    TOracleDataSet *ODS;
    TDataSource *DataSource1;
    TLabel *Lab1;
    TStatusBar *SBar;
    TPopupMenu *PopupMenu2;
    TMenuItem *ActMoveAllDbCat1;
    TActionList *ActionList2;
    TAction *ActEdit;
    TAction *ActDel;
    TAction *ActSel;
    TAction *ActNew;
    TAction *ActAll;
    TAction *ActSort;
    TAction *ActAnSort;
    TAction *ActPrint;
    TAction *ActMove;
    TAction *ActMoveAll;
    TAction *ActCopy;
    TAction *ActPerehod;
    TAction *ActWCat;
    TAction *ActWVer;
    TAction *ActCalc;
    TAction *ActFind;
    TAction *ActFindCat;
    TAction *ActFindAg;
    TAction *ActOpSel;
    TAction *ActOpTree;
    TAction *ActDelAll;
    TAction *ActCatFld;
    TAction *ActNull;
    TPopupMenu *PopupMenu3;
    TMenuItem *MenuItem1;
    TMenuItem *Enter1;
    TMenuItem *MenuItem2;
    TMenuItem *Alt1;
    TMenuItem *MenuItem3;
    TMenuItem *Alt4;
    TMenuItem *MenuItem5;
    TMenuItem *MenuItem6;
    TMenuItem *MenuItem7;
    TMenuItem *MenuItem8;
    TMenuItem *MenuItem9;
    TMenuItem *MenuItem10;
    TMenuItem *MenuItem11;
    TMenuItem *MenuItem12;
    TMenuItem *MenuItem13;
    TMenuItem *ShiftF71;
    TMenuItem *MenuItem14;
    TMenuItem *MenSelDelAll;
    TMenuItem *MenSelSetFld;
    TMenuItem *MenSelSetFreeFld;
    TMenuItem *MenuItem17;
    TMenuItem *MenCatSetFld;
    TToolBar *ToolBar3;
    TToolButton *ToolButton3;
    TToolButton *ToolButton21;
    TToolButton *ToolButton22;
    TToolButton *ToolButton23;
    TToolButton *ToolButton24;
    TToolButton *ToolButton25;
    TToolButton *ToolButton26;
    TToolButton *ToolButton28;
    TToolButton *ToolButton29;
    TToolButton *ToolButton30;
    TToolButton *ToolButton31;
    TToolButton *ToolButton32;
    TToolButton *ToolButton33;
    TToolButton *ToolButton35;
    TToolButton *ToolButton36;
    TToolButton *ToolButton37;
    TAction *ActRefreshCat;
    TMenuItem *N1;
    TMenuItem *N2;
    TMenuItem *N3;
    TAction *ActNewSubCat;
    TMenuItem *CtrlIns1;
    TMenuItem *N4;
    TMenuItem *ShiftF72;
    TAction *ActSelFld;
    TAction *ActTest;
    TAction *ActHist;
    TAction *ActEditDbCatName;
    TMenuItem *N5;
    TAction *ActBuildCatTree;
    TToolButton *ToolButton1;
    TMenuItem *N6;
    TMenuItem *N7;
    TOracleQuery *OQFind;
    TAction *ActHideCatWindow;
    TMenuItem *N8;
    TMenuItem *N9;
    TToolButton *ToolButton2;
    TAction *ActEditAn;
    TAction *ActVisibilityMode;
    TAction *ActSumInfo;
    TPanel *Panel1;
    TAmsDBGrid *DBG;
    TAction *ActGoto;
    TMenuItem *MenGoto;
    TAction *ActFindInCurrCat;
    TMenuItem *AltF71;
    TAction *ActCopyFromExcel;
    TToolButton *ToolButton4;
    TMenuItem *Excel1;
    TButton *ButEnter;
        TAction *ActEscExit;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall CVChange(TObject *Sender, TTreeNode *Node);
    void __fastcall ActEdDbPosExecute(TObject *Sender);
    void __fastcall CVKeyPress(TObject *Sender, char &Key);
    void __fastcall CVChanging(TObject *Sender, TTreeNode *Node,
          bool &AllowChange);
    void __fastcall PanGridResize(TObject *Sender);
    void __fastcall ActFindExecute(TObject *Sender);
    void __fastcall DBGKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall ActDelExecute(TObject *Sender);
    void __fastcall ActInsExecute(TObject *Sender);
    void __fastcall ODSAfterScroll(TDataSet *DataSet);
    void __fastcall ActBezKatExecute(TObject *Sender);
    void __fastcall ActFindAgainExecute(TObject *Sender);
    void __fastcall DBGKeyPress(TObject *Sender, char &Key);
    void __fastcall ActSortOrderExecute(TObject *Sender);
    void __fastcall ActSortIerarchExecute(TObject *Sender);
    void __fastcall ActSortBackExecute(TObject *Sender);
    void __fastcall ActTestExecute(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall ActSVerExecute(TObject *Sender);
    void __fastcall ActInsCatExecute(TObject *Sender);
    void __fastcall CVKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall ActDelCatsExecute(TObject *Sender);
    void __fastcall ActMoveToCatExecute(TObject *Sender);
    void __fastcall ActMoveCatsExecute(TObject *Sender);
    void __fastcall ActMoveAllExecute(TObject *Sender);
    void __fastcall ActMoveAllDbCatExecute(TObject *Sender);
    void __fastcall CVEnter(TObject *Sender);
    void __fastcall DBGFrosenEnter(TObject *Sender);
    void __fastcall ActRefreshCatExecute(TObject *Sender);
    void __fastcall ActNewSubCatExecute(TObject *Sender);
    void __fastcall ActFindCatExecute(TObject *Sender);
    void __fastcall DBGFrosenDblClick(TObject *Sender);
    void __fastcall ActCopyExecute(TObject *Sender);
    void __fastcall ActSelExecute(TObject *Sender);
    void __fastcall ActAllExecute(TObject *Sender);
    void __fastcall MenSelSetFldClick(TObject *Sender);
    void __fastcall MenSelDelAllClick(TObject *Sender);
    void __fastcall MenCatSetFldClick(TObject *Sender);
    void __fastcall MenSelSetFreeFldClick(TObject *Sender);
    void __fastcall ActHistExecute(TObject *Sender);
    void __fastcall DBGMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
    void __fastcall ActEditDbCatNameExecute(TObject *Sender);
    void __fastcall ActBuildCatTreeExecute(TObject *Sender);
    void __fastcall ActCalcExecute(TObject *Sender);
    void __fastcall DBGCellClick(TColumn *Column);
    void __fastcall PopupMenu3Popup(TObject *Sender);
    void __fastcall ActHideCatWindowExecute(TObject *Sender);
    void __fastcall ActEditAnExecute(TObject *Sender);
    void __fastcall ActVisibilityModeExecute(TObject *Sender);
    void __fastcall ODSApplyRecord(TOracleDataSet *Sender, char Action,
          bool &Applied, AnsiString &NewRowId);
    void __fastcall ActSumInfoExecute(TObject *Sender);
    void __fastcall ActGotoExecute(TObject *Sender);
    void __fastcall ActFindInCurrCatExecute(TObject *Sender);
    void __fastcall ActWCatExecute(TObject *Sender);
    void __fastcall ActCopyFromExcelExecute(TObject *Sender);
    void __fastcall ButEnterClick(TObject *Sender);
        void __fastcall ActEscExitExecute(TObject *Sender);
public:		// User declarations
    __fastcall TFrmBrowsDb(TComponent* Owner);
    void Begin(int DBID, const AnsiString& DBName,
                const AnsiString& DBDescr,
                const AnsiString& SetName);
    // Устанавливает на позицию АРТИКУл Версия ЕСЛИ известна категория то указать
    void SetPos (const AnsiString& Art, int Ver, int CatId= -1);
    void RefreshView(void);
    void QuickSearchNewKey (AnsiString SubStr);
    void EndQuickSearch();
    void RunProg(bool Post, bool SetFlds, bool ClrFld, bool ClearF4= false, bool UseFsm=true);
    int GetCurrDbCat ();
    void SetAddCond(const AnsiString AddCond_){AddCond=AddCond_;};
    virtual OpVars* GetOpVars();
    void EndEdit() {EditedNow=false;};
    bool EdNewAnal;
    int NewAnSumid;
    bool WithoutVer;            // Показ без версий
protected:
    std::vector<int> CopySums; // При дублировании позиции тут хранятся
                            // аналитические суммы которые образовали через CAS
    AnsiString AddCond;
    bool EditedNow;

    int CurrDBId;             // Код базы данных
    AnsiString CurrSetName;   // Набор установок
    AnsiString CurrDBName;    // Название базы данных
    int CurrCatId;            // Категория, которая сейчас просматривается на экране
    AnsiString CurrAnName;    //
    //------------------------------------------------------
    bool ColsReady;           // Формирование колонок просмотра закончено ?
    bool CatsReady;           // Дерево категорий считано ?
    int FrosenWidth;          // Ширина грида замороженных полей
    int NormalWidth;          // Ширина грида не замороженных полей

    TGenLocalVars* Vars;      // OPV() и ACCV()

    bool ZeroVerChecked;
    bool ZeroVer;
    bool NeedRecalcArt;
    //-------------------------------------------------------
    TFldsDescr FldsDescr;
    int CurrFlags;
    FieldSetM*  FSM;        // Поля для связи с интерпретатором
    Prog db_prog;           // Скомпилированная программа на интерпретаторе
    bool db_prog_comp;      // Была ли скомпилирована программа интерпретатора
    Prog an_prog;
    bool an_prog_comp;
    //-------------------------------------------------------
    TFrmEditDbPos* FrmEditDbPos;     // Форма для редактирования позиции БД
    TFrmFindDbPos* FrmFindDbPos;     // Форма для поиска по базе данных
    TFrmFindDbPos* FrmFilter;        // Форма для введения фильтра
    TFrmSortOrder* FrmSortOrder;
    TFrmQuickSearch* FrmQuickSearch;
    //-------------------------------------------------------
    //AnsiString PartArt;     // Для хранения артикула, вводимого ч.з прав контрол
    bool QuickArtSearch;
    int FocusWhat;
    AnsiString  FindCatName;   // Какую категорию ищем (подстрока)
    int         FindCatIndex;  // Индекс в TreeView c какого начинаем
    bool        FindCat;       // True - ищем категорию по Shift - F7
                               // False - позицию БД
    //-------------------------------------------------------
    AnsiString OrderByClause;
    bool WithoutCats;           // Просмотр в режиме без категорий ?
    bool HaveCatsSort;          // Сортировать иерархически
    bool DefaultSort;           // Сортировка по умолчанию - Артикул версия

    bool FilterOn;              // Режим фильтра включен
    int FixedColsChange;        // номер изменяемого столбца
    bool FCChange;              // true  - изменяем ширину столбца
                                // false - не изменяем
    //-------------------------------------------------------
    virtual void BuildCols(void);
    void RefreshCats(void);
    virtual AnsiString GetOrderBy(void);
    virtual void BuildSqlQuery(TOracleDataSet*);
    virtual void ShowMode();
    AnsiString GetFilterCond(DS_Q D);
    void FindNextCatExec();
    void SetFld(int Flags);
    void FormatSums();
    void ActEdDbPosExec(int BegFlags);
    // Построение запросов на Update или Insert в БД или Аналитику
    bool BuildInsQuery (AnsiString& S, AmsQuery& OQ, int FldType);
    bool BuildUpdQuery (AnsiString& S, AmsQuery& OQ, int FldType, TOracleDataSet* ODS_);

    // Для дописки в аналитике
    virtual void CatChanged (); // Категория изменилась

    virtual void BeginSelection(); // Пользователь выделил более одной строки
    virtual void EndSelection();   // Пользоваель убрал выделение строк

    virtual void SetFilterOn();
    virtual void SetFilterOff();

    virtual void ViewUpdated();

    virtual void TakeAnSum();
    virtual void BeginEdit(int Key);
    virtual void EditOperVars();
    virtual void SetFilter();

    bool HaveFirstDigit;
    int FirstDigit;
    bool FindInCurrCat;
    virtual void VirtKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);

};
//---------------------------------------------------------------------------
//extern PACKAGE TFrmBrowsDb *FrmBrowsDb;
//---------------------------------------------------------------------------
#endif
