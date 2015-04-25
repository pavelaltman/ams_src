//---------------------------------------------------------------------------

#ifndef FRM_QUICK_SEARCHH
#define FRM_QUICK_SEARCHH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFrmBrowsDb;
//---------------------------------------------------------------------------
class TFrmQuickSearch : public TForm
{
__published:	// IDE-managed Components
    TEdit *EdInp;
    void __fastcall EdInpKeyPress(TObject *Sender, char &Key);
    void __fastcall EdInpChange(TObject *Sender);
protected:	// User declarations
    TFrmBrowsDb* FrmBrowsDb;
    bool Setted;
public:		// User declarations
    __fastcall TFrmQuickSearch(TComponent* Owner);
    void SetSeaching (TFrmBrowsDb* Frm);

};
//---------------------------------------------------------------------------
extern PACKAGE TFrmQuickSearch *FrmQuickSearch;
//---------------------------------------------------------------------------
#endif
