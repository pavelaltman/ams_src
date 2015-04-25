//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_WINDOWS_LIST.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmWindowsList *FrmWindowsList;
//---------------------------------------------------------------------------
__fastcall TFrmWindowsList::TFrmWindowsList(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
TForm* TFrmWindowsList::SelWindow(TForm * F)
{
    PF=F;
    LBWin->Items->Clear();
    for (int i = 0; i< F->MDIChildCount;i++)
    {
        if (F->MDIChildren[i]!= this)
            LBWin->Items->Add(F->MDIChildren[i]->Caption);
    }
    if (F->MDIChildCount)
    {
        LBWin->ItemIndex=0;
    }   

    TForm* Res =NULL;  
    if (ShowModal()>0)
    {
        int i = 0;
        while ( i<PF->MDIChildCount && 
         (PF->MDIChildren[i]->Caption != LBWin->Items->Strings[LBWin->ItemIndex]))
        i++;
        if (i<PF->MDIChildCount)
        {
            Res=PF->MDIChildren[i] ;
        };  
    };
    return Res;
}
//---------------------------------------------------------------------------

void __fastcall TFrmWindowsList::LBWinKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if (Key ==27)
    {
        ModalResult = -1;
    }    
    else
    if (Key == 13)
    {
        if (LBWin->ItemIndex != -1)
        {
            if (LBWin->ItemIndex < PF->MDIChildCount)
            {
                ModalResult=1;
            }    
        }    
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmWindowsList::LBWinClick(TObject *Sender)
{
    ModalResult = 1;    
}
//---------------------------------------------------------------------------


void __fastcall TFrmWindowsList::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    ModalResult = 1;    
}
//---------------------------------------------------------------------------

