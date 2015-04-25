//---------------------------------------------------------------------------

#ifndef FRM_CREATE_QRYH
#define FRM_CREATE_QRYH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
typedef struct {
 int Id;
 AnsiString TableName;
 } TTreeDBData;

class TFrmCreateQry : public TForm
{
__published:	// IDE-managed Components
    TTreeView *tvDB;
    void __fastcall FormActivate(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmCreateQry(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmCreateQry *FrmCreateQry;
//---------------------------------------------------------------------------
#endif
