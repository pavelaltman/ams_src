//---------------------------------------------------------------------------

#ifndef FRM_CHOOSE_DOCH
#define FRM_CHOOSE_DOCH
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
#include <Buttons.hpp>
//---------------------------------------------------------------------------
struct SCodeAndName
{
    int Code;
    AnsiString Name;
};
//---------------------------------------------------------------------------
class TFrmChooseObjModal : public TFrmChooseObj
{
__published:	// IDE-managed Components
    TPanel *Panel2;
    TBitBtn *BitBtn1;
    TBitBtn *BBOk;
    TAction *ActOk;
    void __fastcall ActOkExecute(TObject *Sender);
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmChooseObjModal(TComponent* Owner);
    AnsiString ChooseModal(int);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmChooseObjModal *FrmChooseObjModal;
//---------------------------------------------------------------------------
#endif
