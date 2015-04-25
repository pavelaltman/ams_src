//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_CHOOSE_STD_OPER_MODAL.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AmsStringGrid"
#pragma link "FRM_CHOOSE_STD_OPER"
#pragma link "Oracle"
#pragma resource "*.dfm"
TFrmChooseStdOperModal *FrmChooseStdOperModal;
//---------------------------------------------------------------------------
__fastcall TFrmChooseStdOperModal::TFrmChooseStdOperModal(TComponent* Owner)
    : TFrmChooseStdOper(Owner)
{
}
//---------------------------------------------------------------------------
int TFrmChooseStdOperModal::ChooseStdOper()
{
    Begin();
    int Res =0;
    Hide();
    if (ShowModal()==1)
    {
        Res=SelCondId;
    }
    return Res;
};
//---------------------------------------------------------------------------
void TFrmChooseStdOperModal::MakeSomething(void)
{
    SelCondId = (SG->Cells[0][SG->Row]).ToIntDef(0);
    if (SelCondId)
    {
        ModalResult=1;
    };
};
//---------------------------------------------------------------------------
