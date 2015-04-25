//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_ASK_AN_SUM_TYPE.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmAskAnalSumType *FrmAskAnalSumType;
//---------------------------------------------------------------------------
__fastcall TFrmAskAnalSumType::TFrmAskAnalSumType(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
int TFrmAskAnalSumType::AskAnalSumType()
{
    int Res =-1;
    if (ShowModal()>0)
    {
        Res = RGSel->ItemIndex;   
    }
    return Res;
}
//---------------------------------------------------------------------------
void __fastcall TFrmAskAnalSumType::BBOkClick(TObject *Sender)
{
    ModalResult=1;    
}
//---------------------------------------------------------------------------
void __fastcall TFrmAskAnalSumType::BitBtn1Click(TObject *Sender)
{
    ModalResult= -1;    
}
//---------------------------------------------------------------------------
