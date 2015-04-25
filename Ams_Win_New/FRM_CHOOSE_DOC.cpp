//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FRM_CHOOSE_DOC.h"
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
AnsiString TFrmChooseObjModal::ChooseModal(int)
{
    //TODO: Add your source code here
}