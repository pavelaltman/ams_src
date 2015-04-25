//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_ASK_DATE.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ToolEdit"
#pragma resource "*.dfm"
TFrmAskDate *FrmAskDate;
//---------------------------------------------------------------------------
__fastcall TFrmAskDate::TFrmAskDate(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
int TFrmAskDate::AskDate(const AnsiString& Prompt_, TDateTime& Val)
{
    LabPrompt->Caption=Prompt_;
    DE->Date=Val;
    int Result=0;
    if (ShowModal()==1)
    {
        Result=1;
        Val= (int) DE->Date;
    }
    return Result;

}
//---------------------------------------------------------------------------
void __fastcall TFrmAskDate::BBOkClick(TObject *Sender)
{
    ModalResult=1;    
}
//---------------------------------------------------------------------------
void __fastcall TFrmAskDate::BBCancelClick(TObject *Sender)
{
    ModalResult=-1;    
}
//---------------------------------------------------------------------------
void __fastcall TFrmAskDate::ActOkExecute(TObject *Sender)
{
    ModalResult=1;    
}
//---------------------------------------------------------------------------
