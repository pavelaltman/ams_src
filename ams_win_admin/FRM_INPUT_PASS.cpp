//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_INPUT_PASS.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmInputPass *FrmInputPass;
//---------------------------------------------------------------------------
__fastcall TFrmInputPass::TFrmInputPass(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmInputPass::Button1Click(TObject *Sender)
{
    ModalResult=1;    
}
//---------------------------------------------------------------------------
AnsiString TFrmInputPass::InputPass(const AnsiString& Cap)
{
    // TODO: Add your source code here
    Caption=Cap;
    EdPass->Text="";
    AnsiString Res="";
    if (ShowModal()==1)
    {
        Res=EdPass->Text;
    }
    return Res;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

