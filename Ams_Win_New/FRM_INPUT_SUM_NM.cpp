//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_INPUT_SUM_NM.h"
#include "FRM_INPUT_OPER.h"
#include "FRM_CALC.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AmsInput"
#pragma link "AmsInputStr"
#pragma link "AmsInputSum"
#pragma resource "*.dfm"
TFrmInputSumNM *FrmInputSumNM;
//---------------------------------------------------------------------------
__fastcall TFrmInputSumNM::TFrmInputSumNM(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TFrmInputSumNM::InputSumNM(TFrmInputOper* InputOper_,
                    const AnsiString& Caption_,
                    Sum BeginVal)
{
    InputOper=InputOper_;
    Label1->Caption = Caption_;

    IS->SetDefaultVal(BeginVal);
    IS->AmsPromptLen=0;
    IS->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TFrmInputSumNM::BBOkClick(TObject *Sender)
{
//
    double Res =IS->GetResultSum();
    InputOper->SyntOk(Res);
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFrmInputSumNM::Action1Execute(TObject *Sender)
{
    BBOkClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TFrmInputSumNM::BBCancelClick(TObject *Sender)
{
    InputOper->SyntCancel();
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmInputSumNM::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Action=caFree;    
}
//---------------------------------------------------------------------------

void __fastcall TFrmInputSumNM::ActnCalcExecute(TObject *Sender)
{
    TWinControl* AC= this->ActiveControl;
    TAmsInput* AI=dynamic_cast<TAmsInput*>(AC->Parent);

    if (FrmCalc->ShowModal()==1)
    {
        if (AI)
            AI->SetDefaultVal(FrmCalc->EdRes->Text);
    };
        
}
//---------------------------------------------------------------------------

