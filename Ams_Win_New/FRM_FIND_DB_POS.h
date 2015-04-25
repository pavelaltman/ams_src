//---------------------------------------------------------------------------

#ifndef FRM_FIND_DB_POSH
#define FRM_FIND_DB_POSH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "Oracle.hpp"
#include "AmsInput.h"
#include "AmsInputDate.h"
#include "AmsInputInt.h"
#include "AmsInputStr.h"
#include "AmsInputSum.h"
#include <ActnList.hpp>
//---------------------------------------------------------------------------
struct SSearchDescr
{
    bool Clear;    // True -- удалили запись об ограничении
    int Type;      // “ип пол€ 
    int FldNo;     // Ќомер пол€ внутри TFldsDescr
    int EqSign;    // «нак равенства (неравенства)
    bool CaseSens; // при сравнении учитывать регистр букв 
    AnsiString CmpWhat;
    SSearchDescr(): Clear(true), FldNo(-1), EqSign(0), CaseSens(false){}; 
};  
//---------------------------------------------------------------------------
class TFrmFindDbPos : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TBitBtn *BBCancel;
    TBitBtn *BBOk;
    TPanel *Panel2;
    TSplitter *Splitter1;
    TStringGrid *SGrid;
    TRadioGroup *RGSign;
    TLabel *Label2;
    TCheckBox *ChBCaseSensive;
    TOracleQuery *OQ;
    TAmsInputDate *AIDate;
    TAmsInputSum *AISum;
    TAmsInputInt *AIInt;
    TAmsInputStr *AIStr;
    TBitBtn *BitBtn1;
    TBitBtn *BBDel;
    TSpeedButton *SpeedButton1;
    TActionList *ActionList1;
    TAction *ActDelAllCond;
    TAction *ActInsAndExec;
    TListBox *LBFieldsNames;
    TAction *ActAddCond;
    TAction *ActDelCond;
    void __fastcall CBFieldsNamesChange(TObject *Sender);
    void __fastcall SbSaveClick(TObject *Sender);
    void __fastcall SBDelClick(TObject *Sender);
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall BBCancelClick(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall ActInsAndExecExecute(TObject *Sender);
    void __fastcall LBFieldsNamesKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall CBFieldsNamesClick(TObject *Sender);
    void __fastcall ActDelAllCondExecute(TObject *Sender);
    void __fastcall LBFieldsNamesClick(TObject *Sender);
protected:
    TFldsDescr* FldsDescr;
    std::vector<SSearchDescr> WhatLookingFor; // индекс совпадает с CBFieldsNames
    TAmsInput* ActInput;
    AnsiString DbName;    // Ќазвание базы данных в которой ищем
    bool SqlFormed;       // ѕроизводилс€ ли поиск к-л до
    void FormSqlQuery();

    int CurrInd;

    bool Dirty;
    void RefreshMemo();
    void SaveCurrPos();
    void CheckChange();
      
public:		// User declarations
    __fastcall TFrmFindDbPos(TComponent* Owner);
    bool FindDbPos(bool FindNext, int& ResCatId, AnsiString& ResArt, int& ResVer);
    void Prepare(TFldsDescr*, const AnsiString& DbName_);
    AnsiString FormWhereClause(TOracleDataSet* ODS, bool DataSetModified = false );
    void FormWhereClause(TOracleQuery* OQ1, bool PutAnd );
    int SetFilter(void);
    AnsiString GetDescr(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmFindDbPos *FrmFindDbPos;
//---------------------------------------------------------------------------
#endif
