//---------------------------------------------------------------------------

#ifndef FRM_COPY_TREEH
#define FRM_COPY_TREEH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include "AMS_TREE_VIEW.h"
#include "AmsObjCatView.h"
#include "Oracle.hpp"
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmCatTree;
class TFrmCopyTree : public TForm
{
__published:	// IDE-managed Components
    TMemo *Progress;
    TBitBtn *BBApply;
    TPageControl *PC;
    TTabSheet *TS1;
    TTabSheet *TS2;
    TTabSheet *TS3;
    TTabSheet *TS4;
    TAmsObjCatView *OCFrom;
    TLabel *Label1;
    TEdit *EdWhereCopy;
    TLabel *Label2;
    TAmsObjCatView *OCTo;
    TLabel *Label3;
    TButton *ButPrev;
    TButton *ButNext;
    TOracleQuery *OQ;
    TLabel *Label4;
    TLabel *LabFromCat;
    TLabel *LabBuchName;
    TLabel *LabToCat;
    TLabel *Label9;
    TLabel *Label10;
    TButton *ButClose;
    TOracleSession *OSess;
    TOracleQuery *OterBaseQ;
    TOracleLogon *OLogon;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall BBApplyClick(TObject *Sender);
    void __fastcall PCChange(TObject *Sender);
    void __fastcall ButNextClick(TObject *Sender);
    void __fastcall ButPrevClick(TObject *Sender);
protected:	// User declarations
    void __fastcall FromCatNewEvent(TFrmCatTree* Sender, int NewCat);
    void __fastcall ToCatNewEvent(TFrmCatTree* Sender, int NewCat);

    AnsiString ToName;
    int FromCatId;
    int ToCatId;

    TFrmCatTree* CatTree ;
    void Page1Selected(void);
    void Page2Selected(void);
    void Page3Selected(void);
    void Page4Selected(void);
public:		// User declarations
    __fastcall TFrmCopyTree(TComponent* Owner);
    void Init();
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmCopyTree *FrmCopyTree;
//---------------------------------------------------------------------------
#endif
