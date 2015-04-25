//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_BROWS_AN_MODAL.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AMS_TREE_VIEW"
#pragma link "AmsDbCatView"
#pragma link "AmsDBGrid"
#pragma link "AmsFooter"
#pragma link "FRM_BROWS_AN"
#pragma link "Oracle"
#pragma link "OracleData"
#pragma link "RXDBCtrl"
#pragma resource "*.dfm"
TFrmBrowsAnModal *FrmBrowsAnModal;
//---------------------------------------------------------------------------
__fastcall TFrmBrowsAnModal::TFrmBrowsAnModal(TComponent* Owner)
    : TFrmBrowsAn(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsAnModal::BBOkClick(TObject *Sender)
{
    ModalResult = 1;
        TFrmEditDbPos* Frm = dynamic_cast<TFrmEditDbPos*>(ParentF);
        if (Frm)
        {
            Frm->AnalOk();     
        }
    FreeExecuted= true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmBrowsAnModal::BBCancelClick(TObject *Sender)
{
    if (!Dirty || AskMessage("Выйти без сохранения?", false) ==ID_YES)
    {
        TFrmEditDbPos*  Frm = dynamic_cast<TFrmEditDbPos*>(ParentF);
        if (Frm)
        {
            Frm->AnalCancel();
        }
        FreeExecuted= true;
        ModalResult = -1;
    }
}
//---------------------------------------------------------------------------
int TFrmBrowsAnModal::BrowsAnModal (TForm* Parent_,int AnCode, const AnsiString& AnName,
        const AnsiString& SetName, const AnsiString& Descr,
        int AnSumEd_, int AnSumOst_, const BegMode& BM)
{
    PrepareForm(Parent_,AnCode,AnName,SetName,Descr,AnSumEd_,AnSumOst_,BM );
    return ShowModal();
};
//---------------------------------------------------------------------------


void __fastcall TFrmBrowsAnModal::ActOk_Execute(TObject *Sender)
{
    BBOkClick(Sender);
}
//---------------------------------------------------------------------------


