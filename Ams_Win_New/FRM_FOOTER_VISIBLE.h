//---------------------------------------------------------------------------

#ifndef FRM_FOOTER_VISIBLEH
#define FRM_FOOTER_VISIBLEH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFooterCntrl;
class TFrmFooterVisible : public TForm
{
__published:	// IDE-managed Components
    TRadioGroup *FVAll;
    TRadioGroup *FVFiltered;
    TRadioGroup *FVCurrCat;
    TRadioGroup *FVSubTree;
    TRadioGroup *FVSelected;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    void __fastcall BitBtn2Click(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
protected:	// User declarations
    std::vector<TRadioGroup*> AllRG;
public:		// User declarations
    __fastcall TFrmFooterVisible(TComponent* Owner);
    int FooterVisible(TFooterCntrl* FooterCntrl);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmFooterVisible *FrmFooterVisible;
//---------------------------------------------------------------------------
#endif
