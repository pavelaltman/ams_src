//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_CHOOSE_OBJ_MODAL.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AmsStringGrid"
#pragma link "FRM_CHOOSE_OBJ"
#pragma link "Oracle"
#pragma resource "*.dfm"
TFrmChooseObjModal *FrmChooseObjModal;
//---------------------------------------------------------------------------
__fastcall TFrmChooseObjModal::TFrmChooseObjModal(TComponent* Owner)
    : TFrmChooseObj(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmChooseObjModal::ActOkExecute(TObject *Sender)
{
    ModalResult = 1;
}
//---------------------------------------------------------------------------
void __fastcall TFrmChooseObjModal::BBOkClick(TObject *Sender)
{
    ModalResult=1;
}
//---------------------------------------------------------------------------
void __fastcall TFrmChooseObjModal::BitBtn1Click(TObject *Sender)
{
    ModalResult=-1;
}
//---------------------------------------------------------------------------
int TFrmChooseObjModal::ChooseObjModal(int OType_, SCodeAndName& Res)
{
    int Ok = 0;
    Begin(OType_);
    Hide();
    if (ShowModal()>0)
    {
        Res.Code = (SG->Cells[0][SG->Row]).ToIntDef(-1);
        Res.Name = SG->Cells[1][SG->Row];
        Ok=1;
    }
    return Ok;
}
//---------------------------------------------------------------------------
void TFrmChooseObjModal::MakeSomething(void)
{
    ModalResult=1;
}
//---------------------------------------------------------------------------

void __fastcall TFrmChooseObjModal::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    ClearTree();
    Action = caHide;
}
//---------------------------------------------------------------------------

void __fastcall TFrmChooseObjModal::FormDestroy(TObject *Sender)
{
    //
        
}
//---------------------------------------------------------------------------

