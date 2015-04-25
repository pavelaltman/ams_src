//---------------------------------------------------------------------------

#ifndef FRM_ASK_ACC_ANALH
#define FRM_ASK_ACC_ANALH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmAskAccAnal : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TBitBtn *BitBtn1;
    TBitBtn *BBOk;
    TListBox *LBAccs;
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
    map <AnsiString, TAccName> Accs;
public:		// User declarations
    __fastcall TFrmAskAccAnal(TComponent* Owner);
    TAccName AskAccAnal(int AnalId);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmAskAccAnal *FrmAskAccAnal;
//---------------------------------------------------------------------------
#endif
