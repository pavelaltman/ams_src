//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DBImage1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RXDBCtrl"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
   DBGrid1->Columns->State=csDefault;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DBGrid1DrawColumnCell(TObject *Sender,
      const TRect &Rect, int DataCol, TColumn *Column,
      TGridDrawState State)
{
  if (Table1->FieldByName("Anal_Id")->AsInteger > 800)
    ImageList1->Draw(DBGrid1->Canvas,Rect.Left,Rect.Top,1,true);
}
//---------------------------------------------------------------------------
