//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AboutBoxUnit.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TAboutBox *AboutBox;
//---------------------------------------------------------------------
__fastcall TAboutBox::TAboutBox(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------
void __fastcall TAboutBox::Label2Click(TObject *Sender)
{
  ShellExecute(Application->Handle, "open", "http:\\\\www.ams.kiev.ua", NULL, NULL, 0);
}
//---------------------------------------------------------------------------
          

void __fastcall TAboutBox::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if (Key==27) Close();
}
//---------------------------------------------------------------------------

void __fastcall TAboutBox::FormShow(TObject *Sender)
{
  Version->Caption="Версия 5.2  (Сборка 2905)";
}
//---------------------------------------------------------------------------


