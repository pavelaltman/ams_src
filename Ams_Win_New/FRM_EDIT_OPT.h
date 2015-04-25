//---------------------------------------------------------------------------

#ifndef FRM_EDIT_OPTH
#define FRM_EDIT_OPTH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <CheckLst.hpp>
//---------------------------------------------------------------------------
class TOptList
{
    struct SOpt
    {
        int Val;
        AnsiString ValName;
        SOpt (int V, const AnsiString& VN):
            Val(V), ValName(VN){};
    };
    std::vector<SOpt> Opt;
public:
    int Count() const {return Opt.size();};
    int ValInt(int Indx) const {return Opt[Indx].Val;};
    const AnsiString& ValName(int Indx) const {return Opt[Indx].ValName;};
    void Add (int Val, const AnsiString Descr){Opt.push_back(SOpt(Val, Descr));};
    void Add (const AnsiString Descr, int Val){Opt.push_back(SOpt(Val, Descr));};
};
//---------------------------------------------------------------------------
class TFrmEditOpt : public TForm
{
__published:	// IDE-managed Components
    TBitBtn *BBOk;
    TBitBtn *BBCancel;
    TCheckListBox *ChLB;
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall BBCancelClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmEditOpt(TComponent* Owner);
    int EditOpt(const AnsiString& Caption_, unsigned int& Res, const TOptList& OL);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmEditOpt *FrmEditOpt;
//---------------------------------------------------------------------------
#endif
