//---------------------------------------------------------------------------

#ifndef FRM_COPY_STD_OPH
#define FRM_COPY_STD_OPH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include "Oracle.hpp"
//---------------------------------------------------------------------------
class TFrmCopyStdOp : public TForm
{
__published:	// IDE-managed Components
    TLabel *LabQuestion;
    TLabel *Label1;
    TComboBox *CBSchema;
    TLabel *Label2;
    TEdit *EdOpId;
    TBitBtn *BBCancel;
    TBitBtn *BBOk;
    TOracleQuery *OQuery;
    void __fastcall BBCancelClick(TObject *Sender);
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmCopyStdOp(TComponent* Owner);
    int CopyStdOp(int OpId, const AnsiString& OpName);
protected:
    bool Inited;
    int SelfOpId;
};
//---------------------------------------------------------------------------
//extern PACKAGE TFrmCopyStdOp *FrmCopyStdOp;
//---------------------------------------------------------------------------
#endif
