//---------------------------------------------------------------------------

#ifndef FRM_SET_AN_FLD_ORDERH
#define FRM_SET_AN_FLD_ORDERH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Oracle.hpp"
#include "AmsStringGrid.h"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
struct SFlds
{
   AnsiString RealName;
   AnsiString Name;
   AnsiString Descr;
   int DbId;
   int FldType;
};
//---------------------------------------------------------------------------
class TFrmSetAnFldOrder : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TComboBox *CBSetName;
    TAmsStringGrid *SG;
    TBitBtn *BitBtn1;
    TBitBtn *BBOk;
    TBitBtn *BBApply;
    TOracleQuery *OQ;
    void __fastcall BBApplyClick(TObject *Sender);
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall CBSetNameChange(TObject *Sender);
    void __fastcall SGRowMoved(TObject *Sender, int FromIndex,
          int ToIndex);
private:	// User declarations
protected:
    TFldsDescr FldsDescr;

    int Dirty;
    int AnalId;
    int CurrSetId;
    AnsiString CurrSetName;
    AnsiString AnalName;
    AnsiString AnalDescr;
    AnsiString DbName;
    int DbId;
    void SetChanged();
    void FillFieldDescr();
public:		// User declarations
    __fastcall TFrmSetAnFldOrder(TComponent* Owner);
    void SetAnFldOrder(int AnId_, const AnsiString& AnName_, const AnsiString& AnDescr_);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmSetAnFldOrder *FrmSetAnFldOrder;
//---------------------------------------------------------------------------
#endif
