//---------------------------------------------------------------------------

#ifndef FRM_ANAL_SETUPH
#define FRM_ANAL_SETUPH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AmsStringGrid.h"
#include <ActnList.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFrmAnalSetup : public TForm
{
__published:	// IDE-managed Components
    TActionList *ActionList1;
    TAmsStringGrid *SG;
    TBitBtn *BitBtn1;
    TBitBtn *BBOk;
    TAction *ActOk;
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall ActOkExecute(TObject *Sender);
    void __fastcall SGDblClick(TObject *Sender);
    void __fastcall SGDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
protected:	// User declarations
    struct SFOpt
    {
        AnsiString Name;
        int Width;
        bool Hide;
        bool Edit;
        bool Razv;
    };
    std::vector<SFOpt> FO;
    AnsiString AnName, AnDescr;
    int AccCode, AnalId;
    Graphics::TBitmap* B1;
    Graphics::TBitmap* B2;

public:		// User declarations
    __fastcall TFrmAnalSetup(TComponent* Owner);
    void AnalSetup(int AccCode, int AnalId);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmAnalSetup *FrmAnalSetup;
//---------------------------------------------------------------------------
#endif
