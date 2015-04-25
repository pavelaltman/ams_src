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
    // ������������� �� ������� ������� ������ ���� �������� ��������� �� �������
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
    bool WithoutVer;            // ����� ��� ������
protected:
    std::vector<int> CopySums; // ��� ������������ ������� ��� ��������
                            // ������������� ����� ������� ���������� ����� CAS
    AnsiString AddCond;
    bool EditedNow;

    int CurrDBId;             // ��� ���� ������
    AnsiString CurrSetName;   // ����� ���������
    AnsiString CurrDBName;    // �������� ���� ������
    int CurrCatId;            // ���������, ������� ������ ��������������� �� ������
    AnsiString CurrAnName;    //
    //------------------------------------------------------
    bool ColsReady;           // ������������ ������� ��������� ��������� ?
    bool CatsReady;           // ������ ��������� ������� ?
    int FrosenWidth;          // ������ ����� ������������ �����
    int NormalWidth;          // ������ ����� �� ������������ �����

    TGenLocalVars* Vars;      // OPV() � ACCV()

    bool ZeroVerChecked;
    bool ZeroVer;
    bool NeedRecalcArt;
    //-------------------------------------------------------
    TFldsDescr FldsDescr;
    int CurrFlags;
    FieldSetM*  FSM;        // ���� ��� ����� � ���������������
    Prog db_prog;           // ���������������� ��������� �� ��������������
    bool db_prog_comp;      // ���� �� �������������� ��������� ��������������
    Prog an_prog;
    bool an_prog_comp;
    //-------------------------------------------------------
    TFrmEditDbPos* FrmEditDbPos;     // ����� ��� �������������� ������� ��
    TFrmFindDbPos* FrmFindDbPos;     // ����� ��� ������ �� ���� ������
    TFrmFindDbPos* FrmFilter;        // ����� ��� �������� �������
    TFrmSortOrder* FrmSortOrder;
    TFrmQuickSearch* FrmQuickSearch;
    //-------------------------------------------------------
    //AnsiString PartArt;     // ��� �������� ��������, ��������� �.� ���� �������
    bool QuickArtSearch;
    int FocusWhat;
    AnsiString  FindCatName;   // ����� ��������� ���� (���������)
    int         FindCatIndex;  // ������ � TreeView c ������ ��������
    bool        FindCat;       // True - ���� ��������� �� Shift - F7
                               // False - ������� ��
    //-------------------------------------------------------
    AnsiString OrderByClause;
    bool WithoutCats;           // �������� � ������ ��� ��������� ?
    bool HaveCatsSort;          // ����������� ������������
    bool DefaultSort;           // ���������� �� ��������� - ������� ������

    bool FilterOn;              // ����� ������� �������
    int FixedColsChange;        // ����� ����������� �������
    bool FCChange;              // true  - �������� ������ �������
                                // false - �� ��������
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
    // ���������� �������� �� Update ��� Insert � �� ��� ���������
    bool BuildInsQuery (AnsiString& S, AmsQuery& OQ, int FldType);
    bool BuildUpdQuery (AnsiString& S, AmsQuery& OQ, int FldType, TOracleDataSet* ODS_);

    // ��� ������� � ���������
    virtual void CatChanged (); // ��������� ����������

    virtual void BeginSelection(); // ������������ ������� ����� ����� ������
    virtual void EndSelection();   // ����������� ����� ��������� �����

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
