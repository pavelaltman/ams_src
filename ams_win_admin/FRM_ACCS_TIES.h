//---------------------------------------------------------------------------

#ifndef FRM_ACCS_TIESH
#define FRM_ACCS_TIESH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
#include "AMS_UTILS.h"
#include "Oracle.hpp"
#include <ActnList.hpp>
#include "AMS_TREE_VIEW.h"
#include "AmsAccsView.h"
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmAccsStr;
//---------------------------------------------------------------------------
class TFrmAccsTies : public TForm
{
__published:	// IDE-managed Components
    TStringGrid *StrGrid;
    TPanel *Panel1;
    TBitBtn *BBCancel;
    TBitBtn *BBOk;
    TActionList *ActionList1;
    TAction *ActSelAcc;
    TOracleQuery *OQ;
    TAmsAccsView *AccView;
    TSplitter *Splitter1;
    void __fastcall BBCancelClick(TObject *Sender);
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall StrGridDblClick(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall Splitter1CanResize(TObject *Sender, int &NewSize,
          bool &Accept);
    void __fastcall AccViewChange(TObject *Sender, TTreeNode *Node);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmAccsTies(TComponent* Owner);
    // Проверяет есть ли такие счета и связывает их
    static int TieAccs(const AnsiString& CurrSchema, 
                       const AnsiString& WhereSchema, 
                       int COND_ID, Oracle::TOracleQuery* OQ );
    // Проверяет есть ли такие счета и связывает их
    static int TieAccsAn(const AnsiString& CurrSchema,
                       const AnsiString& WhereSchema,
                       int DbId, Oracle::TOracleQuery* OQ);

    // Начальная инициализация
    void Begin(const AnsiString& CurrSchema, 
               const AnsiString& WhereSchema);
protected:
    int CurrRow;
    void Clear(void);
    AnsiString WhereSchemaName;
    AnsiString CurrSchemaName;
    void AddRow(Oracle::TOracleQuery* OQ);
    bool UserClose;
    // форма для введения номера счета
    TFrmAccsStr* Accs; 
    void __fastcall ChooseAcc (TFrmAccsStr* Sender, const TAccName& NewAccs);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmAccsTies *FrmAccsTies;
//---------------------------------------------------------------------------
#endif
