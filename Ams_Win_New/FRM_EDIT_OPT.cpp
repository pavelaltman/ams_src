//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_EDIT_OPT.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmEditOpt *FrmEditOpt;
//---------------------------------------------------------------------------
__fastcall TFrmEditOpt::TFrmEditOpt(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
int TFrmEditOpt::EditOpt(const AnsiString& Caption_,unsigned int& Res, const TOptList& OL)
{
    Caption= Caption_;
    ChLB->Items->Clear();
    for (int i = 0; i < OL.Count(); i++)
    {
        ChLB->Items->Add(OL.ValName(i));
        ChLB->Checked[i]= (Res & OL.ValInt(i));
    }
    int Ret =0;
    if (ShowModal()==1)
    {
        Ret=1;
        Res=0;
        for (int i = 0; i < ChLB->Items->Count; i++)
        {
            if (ChLB->Checked[i])
               Res += OL.ValInt(i);
        }

    }
    return Ret;
}
//---------------------------------------------------------------------------
void __fastcall TFrmEditOpt::BBOkClick(TObject *Sender)
{
    ModalResult=1;    
}
//---------------------------------------------------------------------------
void __fastcall TFrmEditOpt::BBCancelClick(TObject *Sender)
{
    ModalResult=-1;    
}
//---------------------------------------------------------------------------
