//---------------------------------------------------------------------------

#ifndef FRM_SHOW_TIED_DBH
#define FRM_SHOW_TIED_DBH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AmsStringGrid.h"
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TFrmShowTiedDb : public TForm
{
__published:	// IDE-managed Components
    TAmsStringGrid *SG;
    TButton *Button1;
    void __fastcall Button1Click(TObject *Sender);
protected:	// User declarations
    AnsiString TiedBase;
    TFldsDescr * FldsDescr;
    std::vector<int> VisFlds;
public:		// User declarations
    __fastcall TFrmShowTiedDb(TComponent* Owner);
    void ShowTiedDb(TDataSet* DSet);
    int ReadTiedBase (int OP_ID, int OP_SUBID);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmShowTiedDb *FrmShowTiedDb;
//---------------------------------------------------------------------------
#endif
