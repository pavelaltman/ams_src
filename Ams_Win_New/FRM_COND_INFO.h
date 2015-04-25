//---------------------------------------------------------------------------

#ifndef FRM_COND_INFOH
#define FRM_COND_INFOH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AmsStringGrid.h"
#include <Grids.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFrmCondInfo : public TForm
{
__published:	// IDE-managed Components
    TAmsStringGrid *SG;
    TBitBtn *BBOk;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall BBOkClick(TObject *Sender);
protected:	// User declarations
    int CondId;
public:		// User declarations
    __fastcall TFrmCondInfo(TComponent* Owner);
    void CondInfo(int CondId_, const AnsiString& CondText);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmCondInfo *FrmCondInfo;
//---------------------------------------------------------------------------
#endif
