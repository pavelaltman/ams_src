//---------------------------------------------------------------------------

#ifndef FRM_ASK_FLDS_NAMESH
#define FRM_ASK_FLDS_NAMESH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AmsStringGrid.h"
#include <Buttons.hpp>
#include <Grids.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------
class TFrmAskFldsNames : public TForm
{
__published:	// IDE-managed Components
    TAmsStringGrid *SG;
    TComboBox *CBColName;
    TComboBox *CBFldName;
    TButton *ButSet;
    TBitBtn *BitBtn1;
    TBitBtn *BBOk;
    TToolBar *ToolBar1;
    TActionList *ActionList1;
    TAction *ActReadFlds;
    TAction *ActWriteFlds;
    TToolButton *ToolButton1;
    TToolButton *ToolButton2;
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall ButSetClick(TObject *Sender);
    void __fastcall SGMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall ActReadFldsExecute(TObject *Sender);
    void __fastcall ActWriteFldsExecute(TObject *Sender);
private:	// User declarations
    std::vector<int> Tie;
    TFldsDescr* Descr;
    std::vector<int>* Res;
public:		// User declarations
    __fastcall TFrmAskFldsNames(TComponent* Owner);
    int AskFldsNames(std::vector<int>& Res, const AnsiString& Cpy, TFldsDescr& Descr);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmAskFldsNames *FrmAskFldsNames;
//---------------------------------------------------------------------------
// 0 - просто следующее слово
// 1 - конец рядка
// 2 - конец входящей строки
extern int GetNextWord(const AnsiString& Cpy, int& Pos, AnsiString& Res);
//---------------------------------------------------------------------------
#endif
