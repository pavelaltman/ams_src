//---------------------------------------------------------------------------

#ifndef ams_win_cH
#define ams_win_cH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <ImgList.hpp>
#include <Map.h>
#include <Vector.h>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include "RxGIF.hpp"
//---------------------------------------------------------------------------
class TFrmChooseDbPos;
class TFrmChooseObj;
class TFrmChooseStdOper;
class TFrmAccList;

extern TForm* LastForm;
extern TForm* SaveForm;
//---------------------------------------------------------------------------

typedef struct {
 int id,type;
 AnsiString descr, path;
 } TListViewData;
//---------------------------------------------------------------------------
class TAmsWinC : public TForm
{
__published:    // IDE-managed Components
    TToolBar *ToolBar1;
    TStatusBar *StatusBar1;
    TMainMenu *MainMenu1;
    TActionList *ActionList1;
    TMenuItem *N1;
    TMenuItem *N2;
    TMenuItem *N3;
    TMenuItem *N4;
    TMenuItem *N5;
    TMenuItem *N6;
    TMenuItem *N7;
    TMenuItem *N8;
    TMenuItem *N9;
    TMenuItem *N10;
    TMenuItem *N11;
    TMenuItem *N12;
    TMenuItem *N13;
    TMenuItem *N14;
    TMenuItem *N15;
    TAction *ActHelp;
    TMenuItem *N16;
    TMenuItem *N17;
    TMenuItem *N18;
    TMenuItem *N19;
    TAction *ActSetSqlDebug;
    TMenuItem *SQL2;
    TMenuItem *N20;
    TMenuItem *N21;
    TMenuItem *N22;
    TMenuItem *N23;
    TAction *ActInpOper;
    TAction *ActJurOper;
    TAction *ActRasrOper;
    TAction *ActOtlOper;
    TAction *ActOtForm;
    TAction *ActBD;
    TAction *ActConst;
    TAction *ActChangeName;
    TAction *ActExit;
    TAction *ActPerVis;
    TAction *ActNewM;
    TAction *ActDelsOper;
    TAction *ActDelsOtlOper;
    TAction *ActListWind;
    TAction *ActCascade;
    TAction *ActTile;
    TToolButton *ToolButton1;
    TToolButton *ToolButton2;
    TToolButton *ToolButton3;
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
    TToolButton *ToolButton15;
    TToolButton *ToolButton16;
    TToolButton *ToolButton17;
    TToolButton *ToolButton18;
    TMenuItem *TestAnSum1;
    TMenuItem *N24;
    TMenuItem *Debug1;
    TMenuItem *N25;
    TMenuItem *N26;
    TAction *ActCalc;
    TToolButton *ToolButton19;
    TToolButton *ToolButton20;
    TMenuItem *N27;
    TMenuItem *MNViewDebug;
    TMenuItem *N28;
    TMenuItem *N29;
    TMenuItem *N30;
    TAction *ActImport;
        TAction *ActImportSettings;
    TAction *ActShowMap;
    TAction *ActShowPlanAcc;
    TMainMenu *MainMenu2;
        TListView *lv;
        TAction *ActPeriod;
        TImageList *ImageList1;
        TMenuItem *N31;
        TMenuItem *N32;
        TToolButton *ToolButton21;
        TEdit *edShtrih;
        TMenuItem *N33;
        TMenuItem *N34;
        TMenuItem *N35;
    void __fastcall N11Click(TObject *Sender);
    void __fastcall N15Click(TObject *Sender);
    void __fastcall ActHelpExecute(TObject *Sender);
    void __fastcall N19Click(TObject *Sender);
    void __fastcall ActSetSqlDebugExecute(TObject *Sender);
        void __fastcall N14Click(TObject *Sender);
    void __fastcall N20Click(TObject *Sender);
    void __fastcall ActInpOperExecute(TObject *Sender);
    void __fastcall ActJurOperExecute(TObject *Sender);
    void __fastcall ActRasrOperExecute(TObject *Sender);
    void __fastcall ActOtlOperExecute(TObject *Sender);
    void __fastcall ActOtFormExecute(TObject *Sender);
    void __fastcall ActBDExecute(TObject *Sender);
    void __fastcall ActConstExecute(TObject *Sender);
    void __fastcall ActChangeNameExecute(TObject *Sender);
    void __fastcall ActExitExecute(TObject *Sender);
    void __fastcall ActPerVisExecute(TObject *Sender);
    void __fastcall ActDelsOperExecute(TObject *Sender);
    void __fastcall ActDelsOtlOperExecute(TObject *Sender);
    void __fastcall ActListWindExecute(TObject *Sender);
    void __fastcall ActCascadeExecute(TObject *Sender);
    void __fastcall ActTileExecute(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall ActNewMExecute(TObject *Sender);
    void __fastcall TestAnSum1Click(TObject *Sender);
    void __fastcall N24Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall ActCalcExecute(TObject *Sender);
    void __fastcall N26Click(TObject *Sender);
    void __fastcall N25Click(TObject *Sender);
    void __fastcall MNViewDebugClick(TObject *Sender);
    void __fastcall ActImportExecute(TObject *Sender);
    void __fastcall ActImportSettingsExecute(TObject *Sender);
    void __fastcall ActShowMapExecute(TObject *Sender);
    void __fastcall ActShowPlanAccExecute(TObject *Sender);
    void __fastcall lvClick(TObject *Sender);
        void __fastcall lvMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall N32Click(TObject *Sender);
        void __fastcall ToolButton21Click(TObject *Sender);
        void __fastcall edShtrihKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall edShtrihKeyPress(TObject *Sender, char &Key);
        void __fastcall N35Click(TObject *Sender);
private:        // User declarations
    bool Loaded;
    std::map <AnsiString, TFrmChooseDbPos*> ChoosePos;
    struct SKeyToFrm
    {
        AnsiString Key;
        TFrmChooseDbPos* Frm;
    };
    std::vector<SKeyToFrm> AllFrms;
    AnsiString GetKey (int DbId, const AnsiString& SetName){return IntToStr(DbId)+SetName;}
    int __fastcall CreateGUI();
    int __fastcall ExecGUIAction(AmsOType, int);
    TListViewData *LVD;
        __fastcall ExecInpOper(int);
        __fastcall ExecViewDB(int);
public:         // User declarations
    __fastcall TAmsWinC(TComponent* Owner);
    void SDisplay(int WindowNo, const AnsiString& Msg);
    TFrmChooseDbPos* GetChooseFrm(int DbId, const AnsiString& SetName);
    TFrmChooseObj* FrmChooseDb;
    TFrmChooseObj* FrmChooseForms;
    TFrmChooseObj* FrmChooseConst;
    TFrmChooseStdOper* FrmChooseStdOper;
    TFrmAccList*  FrmAccList;
    int ClientTop_ () {return ToolBar1->Top+ ToolBar1->Height;};
    int ClientBottom_ () {return StatusBar1->Top-1;};
    int ClientHeight_ () {return ClientBottom_()-ClientTop_();};
    void __fastcall FindActiveMDIChild();
    bool UseShtrih; // для ввода операций штрих-кода
    AnsiString  Shtrih; // значение штрих-кода
};
//---------------------------------------------------------------------------
extern PACKAGE TAmsWinC *AmsWinC;
//---------------------------------------------------------------------------
#endif

