//---------------------------------------------------------------------------

#ifndef FRM_BRWS_ACC_WINDOWH
#define FRM_BRWS_ACC_WINDOWH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "FRM_OPER_LIST_ROOT.h"
#include "OracleData.hpp"
#include "RXDBCtrl.hpp"
#include "RXSplit.hpp"
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <Db.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>

//---------------------------------------------------------------------------
#include "BRWS_ACC_WINDOW.h"
#include "FRM_OPER_LIST.h"
#include "AmsFooter.h"
#include "AmsDBGrid.h"
#include "RxCombos.hpp"
#include <Dialogs.hpp>
#include "Oracle.hpp"
//---------------------------------------------------------------------------
class TFrmAnView;   
//---------------------------------------------------------------------------
class TFrmBrwsAccWindow : public TFrmOperListRoot
{
__published:	// IDE-managed Components
    TAction *ActViewOpt;
    TAmsFooter *AFEnd;
    TAmsFooter *AFBeg;
    TStringField *DSetTEXT;
    TFloatField *DSetDOC_NUM_V;
    TIntegerField *DSetSTORNO;
    TFloatField *DSetOP_ID;
    TFloatField *DSetOP_SUBID;
    TAction *ActFiltr;
    TAction *ActFieldName;
    TMenuItem *N10;
    TMenuItem *N11;
    TMenuItem *N12;
    TAction *ActAnaliticShow;
    TMenuItem *N13;
    TStatusBar *StatusBar;
    TAction *ActAnalChoose;
    TAction *ActAnalEndRem;
    TMenuItem *Alt5;
    TAction *ActRunForm;
    TAction *ActShowDeffered;
    TAction *ActRecalc;
    TMenuItem *N14;
    TMenuItem *N15;
    TMenuItem *AltM1;
    TTimer *TDelay;
    TAction *ActNotPlainPeriod;
    TMenuItem *N16;
    TAction *ActRecalcBegin;
    TMenuItem *N17;
    void __fastcall ActViewOptExecute(TObject *Sender);
    void __fastcall Grid1AmsFooterSyncho(TAmsDBGrid *Sender,
          SSynchoCause &Cause);
    void __fastcall Grid1GetCellParams(TObject *Sender, TField *Field,
          TFont *AFont, TColor &Background, bool Highlight);
    void __fastcall DSetCalcFields(TDataSet *DataSet);
    void __fastcall ActFiltrExecute(TObject *Sender);
    void __fastcall ActFieldNameExecute(TObject *Sender);
    void __fastcall ActPerExecute(TObject *Sender);
    void __fastcall ActAnaliticShowExecute(TObject *Sender);
    void __fastcall RxSplitter1PosChanged(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall ActAnalChooseExecute(TObject *Sender);
    void __fastcall ActShowDefferedExecute(TObject *Sender);
    void __fastcall ActRunFormExecute(TObject *Sender);
    void __fastcall ActPrintExecute(TObject *Sender);
    void __fastcall ActAnalEndRemExecute(TObject *Sender);
    void __fastcall ActRecalcExecute(TObject *Sender);
    void __fastcall DSetAfterScroll(TDataSet *DataSet);
    void __fastcall TDelayTimer(TObject *Sender);
    void __fastcall ActNotPlainPeriodExecute(TObject *Sender);
    void __fastcall ActRecalcBeginExecute(TObject *Sender);
        void __fastcall ActSelExecute(TObject *Sender);
        void __fastcall ActAllExecute(TObject *Sender);
        void __fastcall ActFindExecute(TObject *Sender);
protected:	// User declarations
    struct SOutTotal {
        int FldNo;
        double Sum;
        int ColNo;
    };

    enum EnEndFooter{eefEmpty=0,eefSelected,eefF5,eefTurns,eefRems,eefEnd};

    AnsiString BegTitle;
    SBrwsAccWindowOptions CurrOpt;
    TBrwsAccWindow AW;

    virtual void KeyEvent (WORD &Key, TShiftState Shift);
    void BuildCols(AnsiString Simple);

    Sum BegR;   // Остаток на начало
    Sum DTurn;  // Оборот по кредиту
    Sum CTurn;  // Оборот по дебету
    Sum EndR;   // Остаток на конец

    // для неровного периода
    int AS_BegR, AS_EndR, AS_DTurn, AS_CTurn;

    bool HaveWhereTotal;
    Sum WTDTurn;
    Sum WTCTurn;

    struct SLastRemOpt
    {
        AnsiString FldName;
        TDate BegDate;
        TDate EndDate;
        AnsiString Article;
        int Ver;
    };
    SLastRemOpt LastRemOpt;

    //-------------------------------
    //  Для подсчета остатка по датам.
    Sum CurrRem;
    TFloatField* DSum;
    TFloatField* CSum;
    TFloatField* DayRemD;
    TFloatField* DayRemC;
    TDateTimeField* OpDateFld;
    TField* StornoFld;
    bool HaveDayRem;

    TColor Dk;

    std::map<TDate, int> DayIndex;
    std::vector<Sum> DayRemV;
    //-------------------------------
    bool HaveOpId;

    int DtCol;  // Какая колонка отвечает за дебет
    int CtCol;  //                        за кредит

    void OutDK(TAmsFooter* F, double Val, int DtCol, int CtCol, int Row);
    OutTurns(TAmsFooter* F, double Val, int Col, int Row);
    void ClrFooter();
    void CalcSize(int GridWidth);

    std::vector<TField*> Vec;

    TFloatField* CreateSumField(const AnsiString& FldName);
    int CreateSumCol (TFloatField* Fld, const AnsiString& ColTitle, int Width);

    TDateTimeField* CreateDateField(const AnsiString& FldName);
    int CreateDateCol (TField* Fld, const AnsiString& ColTitle, int Width);

    TField* CreateTxtField(const AnsiString& FldName, int MaxSize=250);
    int CreateTxtCol (TField* Fld, const AnsiString& ColTitle, int Width);

    TField* CreateIntField(const AnsiString& FldName);
    int CreateIntCol (TField* Fld, const AnsiString& ColTitle, int Width);

    TIntegerField* CreateInt_Field(const AnsiString& FldName);
    int CreateInt_Col (TField* Fld, const AnsiString& ColTitle, int Width);

    void RefreshOpt();
    void GetRem();         // Получить остаток и обороты
    AnsiString GetFirstFieldName();
    //-----------------------------------------------------
    AnsiString FirstFieldName;

    TFrmAnView* AnView;

    // вызывается Если изменился фильтр
    virtual void ChangedFilter();
    virtual void FindOk();
    // Базовый select запрос для поиска
    virtual AnsiString FormFindSql(DS_Q& Q);
        // вызывается Если изменилась сортировка
    virtual void ChangedOrder();
    virtual void BuildFullSql   (AnsiString& Sql, DS_Q& Q, AnsiString& Simple);
    void ViewAnalitic(bool AskSet_, bool NeedDel, int SumId_, const AnsiString& SetName_,
                    const AnsiString& Caption_, bool NeedDistr=false);
    void CountSelection(void);
    void HideSelTotal(void);

    int TmpRemAnSum;// Для хранения временных аналит сумм остатков, оборотов,
                    // находим для неровного периода

    void DumbSetFooter(); //CREATED AS ANSWER ON QUESTION #3 MADE 13.03.02

public:		// User declarations
    __fastcall TFrmBrwsAccWindow(TComponent* Owner);
    void Begin(int AccCode, const AnsiString& Title,
                    const AnsiString& Art=AnsiString(""), int Ver=0);
    void MakeTitle(void);
    void AnViewEnd(void);
    virtual void RefreshAfterOper(bool otlozh=false);
    int GetAccCode(){return CurrOpt.AccCode;};
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmBrwsAccWindow *FrmBrwsAccWindow;
//---------------------------------------------------------------------------
#endif
