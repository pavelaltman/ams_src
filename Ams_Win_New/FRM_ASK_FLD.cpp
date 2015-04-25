//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FRM_ASK_FLD.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AmsInput"
#pragma link "AmsInputDate"
#pragma link "AmsInputInt"
#pragma link "AmsInputStr"
#pragma link "AmsInputSum"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BBCancelClick(TObject *Sender)
{
    ModalResult=-1;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BBOkClick(TObject *Sender)
{
    ModalResult = 1;
}
//-----------------------------------------------------------------------
