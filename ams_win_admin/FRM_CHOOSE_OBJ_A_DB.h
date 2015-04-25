//---------------------------------------------------------------------------

#ifndef FRM_CHOOSE_OBJ_A_DBH
#define FRM_CHOOSE_OBJ_A_DBH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AmsStringGrid.h"
#include "FRM_CHOOSE_OBJ_ADM.h"
#include "Oracle.hpp"
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>

//---------------------------------------------------------------------------
class TFrmChooseObjADb : public TFrmChooseObj
{
__published:	// IDE-managed Components
    TAction *ActCopyData;
    TAction *ActUpdStruct;
    TMenuItem *N2;
    TMenuItem *N3;
    TMemo *MRes;
    TSplitter *Splitter2;
    TOracleSession *OrSess;
    TOracleScript *OSc;
    TOracleLogon *OrLogon;
    void __fastcall ActCopyDataExecute(TObject *Sender);
    void __fastcall ActUpdStructExecute(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall OScAfterCommand(TOracleScript *Sender, bool &Handled);
protected:	// User declarations
    
public:		// User declarations
    __fastcall TFrmChooseObjADb(TComponent* Owner);
    void BeginDb(int OType);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmChooseObjADb *FrmChooseObjADb;
//---------------------------------------------------------------------------
#endif
