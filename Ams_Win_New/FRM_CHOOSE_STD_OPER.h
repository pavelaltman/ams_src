//---------------------------------------------------------------------------

#ifndef FRM_CHOOSE_STD_OPERH
#define FRM_CHOOSE_STD_OPERH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AmsStringGrid.h"
#include "FRM_CHOOSE_OBJ.h"
#include "Oracle.hpp"
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------
class TFrmChooseStdOper : public TFrmChooseObj
{
__published:	// IDE-managed Components
    TAction *ActShowInfo;
    TMenuItem *N2;
        TButton *Button2;
        TToolButton *ToolButton2;
        TToolButton *ToolButton3;
        TAction *ActNonStdOper;
        TAction *ActRefresh;
    void __fastcall ActShowInfoExecute(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall ToolButton2Click(TObject *Sender);
        void __fastcall ActRefreshExecute(TObject *Sender);
        void __fastcall ActNonStdOperExecute(TObject *Sender);
private:	// User declarations
        int EscCounter;  // Для организации выхода по 2 м Esc подряд
public:		// User declarations
    __fastcall TFrmChooseStdOper(TComponent* Owner);
    void Begin();
    virtual void MakeSomething(void);
    virtual void FCKeyDown (WORD &Key, TShiftState Shift);

    void EscClear(){EscCounter++;}; // Для организации выхода по 2 м Esc подряд
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmChooseStdOper *FrmChooseStdOper;
//---------------------------------------------------------------------------
#endif
