//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_ASK_SUM.h"
#include "FRM_CALC.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AmsInput"
#pragma link "AmsInputStr"
#pragma link "AmsInputSum"
#pragma resource "*.dfm"
TFrmAskSum *FrmAskSum;
//---------------------------------------------------------------------------
__fastcall TFrmAskSum::TFrmAskSum(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmAskSum::BBOkClick(TObject *Sender)
{
    ModalResult=1;
}
//---------------------------------------------------------------------------
void __fastcall TFrmAskSum::BBCancelClick(TObject *Sender)
{
    ModalResult=-1;
}
//---------------------------------------------------------------------------
int TFrmAskSum::AskSum(const AnsiString& UserPrompt,double& ResValue)
{
    Label1->Caption=UserPrompt;
    IS->SetDefaultVal(ResValue);
    IS->AmsPromptLen=0;
    int Res=0;
    if (ShowModal()==1)
    {
        ResValue=IS->GetResultSum();
        Res=1;
    }
    return Res;
}
//---------------------------------------------------------------------------
void __fastcall TFrmAskSum::ActOkExecute(TObject *Sender)
{
    ModalResult=1;
}
//---------------------------------------------------------------------------




void __fastcall TFrmAskSum::FormShow(TObject *Sender)
{
  AnsiString s="TEdit";
  for (int j=IS->ComponentCount-1;j>=0;j--) {
    if (s==IS->Components[j]->ClassName()) {
     ((TEdit *)IS->Components[j])->SetFocus();
    }
  }
}
//---------------------------------------------------------------------------

