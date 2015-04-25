//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_ASK_DK.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmAskDK *FrmAskDK;
//---------------------------------------------------------------------------
__fastcall TFrmAskDK::TFrmAskDK(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmAskDK::BBOkClick(TObject *Sender)
{
    ModalResult=1;    
}
//---------------------------------------------------------------------------
void __fastcall TFrmAskDK::BBCancelClick(TObject *Sender)
{
    ModalResult=-1;    
}
//---------------------------------------------------------------------------
AnsiString TFrmAskDK::AskDK()
{
    AnsiString Res="";
    if (ShowModal()==1)
    {
        if (RGr->ItemIndex)
            Res="Ê";
        else
            Res="Ä";
    }
    return Res;
}
//---------------------------------------------------------------------------
