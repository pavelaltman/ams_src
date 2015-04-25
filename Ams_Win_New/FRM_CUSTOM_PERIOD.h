//---------------------------------------------------------------------------

#ifndef FRM_CUSTOM_PERIODH
#define FRM_CUSTOM_PERIODH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ToolEdit.hpp"
#include <ExtCtrls.hpp>
#include <Mask.hpp>
#include "Placemnt.hpp"
//---------------------------------------------------------------------------
class TFrmCustomPeriod : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TPanel *Panel2;
        TPanel *Panel3;
        TDirectoryEdit *DirectoryEdit1;
        TDirectoryEdit *DirectoryEdit2;
        TDirectoryEdit *DirectoryEdit3;
        TStaticText *StaticText1;
        TStaticText *StaticText2;
        TStaticText *StaticText3;
        TPanel *Panel4;
        TButton *ButClose;
        TFormStorage *FormStorage1;
        void __fastcall ButCloseClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFrmCustomPeriod(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmCustomPeriod *FrmCustomPeriod;
//---------------------------------------------------------------------------
#endif
