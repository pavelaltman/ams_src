//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FRM_COLORGUI.h"
#include "ams_win_c.h"
#include "FRM_CHOOSE_STD_OPER.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmColorGUI *FrmColorGUI;
extern void __fastcall ActInpOperExecute(TObject *Sender);
//---------------------------------------------------------------------------
__fastcall TFrmColorGUI::TFrmColorGUI(TComponent* Owner)
    : TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TFrmColorGUI::imLTMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
   if (!bLT)
   {
      SetFalse();
      bLT=true;
      imLT->Picture->LoadFromFile("d:\\andrei\\map.bmp");
   };
}
//---------------------------------------------------------------------------

void __fastcall TFrmColorGUI::SetFalse()
{
    bLT=false;
    imLT->Picture->LoadFromFile("d:\\Program Files\\Common Files\\Borland Shared\\Images\\Splash\\256Color\\factory.bmp");
    bRT=false;
    imRT->Picture->LoadFromFile("d:\\Program Files\\Common Files\\Borland Shared\\Images\\Splash\\256Color\\handshak.bmp");
    bLB=false;
    imLB->Picture->LoadFromFile("d:\\Program Files\\Common Files\\Borland Shared\\Images\\Splash\\256Color\\finance.bmp");
    bMB=false;
    imMB->Picture->LoadFromFile("d:\\Program Files\\Common Files\\Borland Shared\\Images\\Splash\\256Color\\shipping.bmp");
    bRB=false;
    imRB->Picture->LoadFromFile("d:\\Program Files\\Common Files\\Borland Shared\\Images\\Splash\\256Color\\chemical.bmp");
}

void __fastcall TFrmColorGUI::imRTMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
   if (!bRT)
   {
      SetFalse();
      bRT=true;
      imRT->Picture->LoadFromFile("d:\\andrei\\map.bmp");
   };    
}
//---------------------------------------------------------------------------
void __fastcall TFrmColorGUI::imLBMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
   if (!bLB)
   {
      SetFalse();
      bLB=true;
      imLB->Picture->LoadFromFile("d:\\andrei\\map.bmp");
   };
}
//---------------------------------------------------------------------------
void __fastcall TFrmColorGUI::imMBMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
   if (!bMB)
   {
      SetFalse();
      bMB=true;
      imMB->Picture->LoadFromFile("d:\\andrei\\map.bmp");
   };
}
//---------------------------------------------------------------------------
void __fastcall TFrmColorGUI::imRBMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
   if (!bRB)
   {
      SetFalse();
      bRB=true;
      imRB->Picture->LoadFromFile("d:\\andrei\\map.bmp");
   };
}
//---------------------------------------------------------------------------


