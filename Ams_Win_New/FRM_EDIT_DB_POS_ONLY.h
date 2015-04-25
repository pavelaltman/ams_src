//---------------------------------------------------------------------------

#ifndef FRM_EDIT_DB_POS_ONLYH
#define FRM_EDIT_DB_POS_ONLYH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "FRM_EDIT_DB_POS.h"
#include <ActnList.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include "OracleData.hpp"
#include <Db.hpp>
//---------------------------------------------------------------------------
// Для ввода данных о связанной БД в операции
//---------------------------------------------------------------------------
class TGenLocalVars;
class TFrmInputOper;
class TFrmEditDbPosOnly : public TFrmEditDbPos
{
__published:	// IDE-managed Components
    TOracleDataSet *ODS1;
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall BBCancelClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
protected:	// User declarations
    TFldsDescrDbPos FD;
    FieldSetM FSM;
    virtual void RunProg(bool Post, bool SetFlds, bool ClrFld, bool ClearF4= false, bool UseFsm=true);
    bool db_prog_comp;
    Prog db_prog;
    TGenLocalVars* Vars;
    TFrmInputOper*FrmInputOper;
    virtual void LastElPressed();

public:		// User declarations
    __fastcall TFrmEditDbPosOnly(TComponent* Owner);
    void Init(const AnsiString& DbName_, const AnsiString& SetName_, TGenLocalVars* Vars_);

    int EditPosNM(TFrmInputOper*FrmInputOper_,const AnsiString& Art, int Ver, int Flags);
    int EditPosNMOk(TFrmInputOper*FrmInputOper_,const AnsiString& Art, int Ver, int Flags);
    int EditPos(const AnsiString& Art, int Ver, int Flags);
    TFldsDescrDbPos& FDescr () {return FD;};
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmEditDbPosOnly *FrmEditDbPosOnly;
//---------------------------------------------------------------------------
#endif
