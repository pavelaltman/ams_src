//---------------------------------------------------------------------------

#ifndef FRM_SORT_ORDERH
#define FRM_SORT_ORDERH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <CheckLst.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmSortOrder : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel5;
    TBitBtn *BButAllUp;
    TBitBtn *BButOneUp;
    TBitBtn *BButOneDown;
    TBitBtn *BButAllDown;
    TPanel *Panel1;
    TCheckListBox *ChLB;
    TBitBtn *BBCancel;
    TBitBtn *BBOk;
    void __fastcall BButAllDownClick(TObject *Sender);
    void __fastcall BButAllUpClick(TObject *Sender);
    void __fastcall BButOneDownClick(TObject *Sender);
    void __fastcall BButOneUpClick(TObject *Sender);
    void __fastcall BBCancelClick(TObject *Sender);
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
protected:	// User declarations
    std::vector<int> Mapping;
    std::vector<AnsiString> Vals;
public:		// User declarations
    __fastcall TFrmSortOrder(TComponent* Owner);
    void AddStrs(const AnsiString& DispVal, const AnsiString& ResVal);
    bool HaveInRes(int Id = 0);
    void ClearAll();
    void GetCommaText(AnsiString& Res, bool NoWaitForUser= false);
    void SetSelection(int BegInd, bool Val);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmSortOrder *FrmSortOrder;
//---------------------------------------------------------------------------
#endif
