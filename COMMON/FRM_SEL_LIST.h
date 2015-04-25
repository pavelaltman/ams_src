//---------------------------------------------------------------------------

#ifndef FRM_SEL_LISTH
#define FRM_SEL_LISTH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
// ¬ключать ли поле в
typedef bool (*TInclude) (TOracleQuery* OQ);
//---------------------------------------------------------------------------
class TFrmSelList : public TForm
{
__published:	// IDE-managed Components
    TLabel *Lab;
    TListBox *LB;
    TPanel *Panel1;
    TBitBtn *BitBtn1;
    TBitBtn *BBOk;
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
protected:	// User declarations
    TInclude InclFunc;
public:		// User declarations
    __fastcall TFrmSelList(TComponent* Owner);
    AnsiString AskFieldStr(TOracleQuery* OQ, const AnsiString& Msg, TInclude FQ=NULL);
    // поле 0 отображаетс€ в списке возвращаетс€ соотв ему поле 1
    AnsiString AskFieldStr2(TOracleQuery* OQ, const AnsiString& Msg, TInclude FQ=NULL);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmSelList *FrmSelList;
//---------------------------------------------------------------------------
#endif
