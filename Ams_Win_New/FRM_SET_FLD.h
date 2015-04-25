//---------------------------------------------------------------------------

#ifndef FRM_SET_FLD_POSH
#define FRM_SET_FLD_POSH
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
//---------------------------------------------------------------------------
class TFrmSetFld : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TBitBtn *BBCancel;
    TBitBtn *BBOk;
    TPanel *Panel2;
    TSplitter *Splitter1;
    TLabel *Label2;
    TActionList *ActionList1;
    TAction *ActDelAllCond;
    TAction *ActInsAndExec;
    TListBox *LBFieldsNames;
    TAction *ActAddCond;
    TAction *ActDelCond;
    TEdit *EdFldVal;
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall BBCancelClick(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall ActInsAndExecExecute(TObject *Sender);
    void __fastcall LBFieldsNamesExit(TObject *Sender);
protected:
    TFldsDescr* FldsDescr;
    std::vector<int> Tie;   // индекс совпадает с LBFieldsNames
                            //
    TAmsInput* ActInput;
    AnsiString DbName;      // Ќазвание базы данных в которой ищем
    bool SqlFormed;         // ѕроизводилс€ ли поиск к-л до

    int CurrInd;

    bool Dirty;

    void CheckChange();
    int IsAlwaysFree();

public:		// User declarations
    __fastcall TFrmSetFld(TComponent* Owner);
    bool SetFld (int );
    void Prepare(TFldsDescr*);
    void FormSqlQuery(TOracleQuery * OQ);
    int SetFilter(void);
    AnsiString GetDescr(void);
    int SetFld(TFldsDescr*);
    SFldOpt* GetSelFld();
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmSetFld *FrmSetFld;
//---------------------------------------------------------------------------
#endif
