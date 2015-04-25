//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_EDIT_MEMO.h"
#include "THREAD_READ_FILE.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmEditMemo *FrmEditMemo;
//---------------------------------------------------------------------------
__fastcall TFrmEditMemo::TFrmEditMemo(TComponent* Owner)
    : TForm(Owner)
{
    ThreadRF = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFrmEditMemo::BBCancelClick(TObject *Sender)
{
    if (ThreadRF)
    {
        ThreadRF->Terminate();
    }
    ModalResult=-1;
    if (FormStyle==fsNormal)
    {
        Close();
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmEditMemo::BBOkClick(TObject *Sender)
{
    ModalResult = 1;
    if (FormStyle==fsNormal)
    {
        Close();
    }
}
//-------------------------------------------------------------------------

void __fastcall TFrmEditMemo::N2Click(TObject *Sender)
{
// Считываем из файла
    if (
        (Mem->Lines->Text !="") &&
        AskQuestion("Текущее содержимое будет заменено.\r\nВам это подходит?")==ID_YES &&
        OD->Execute()
    )
    {
        Mem->Lines->Clear();
        std::ifstream F(OD->FileName.c_str());
        while (!F.eof())
        {
            char Buf[5000];
            F.getline(Buf,5000);
            Mem->Lines->Add(Buf);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmEditMemo::N3Click(TObject *Sender)
{
// Записать файл
    if (SD->Execute())
    {
        std::ofstream F(SD->FileName.c_str());
        for (int i = 0; i< Mem->Lines->Count; i++)
            F<<Mem->Lines->Strings[i]<<"\n";
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmEditMemo::N5Click(TObject *Sender)
{
    ModalResult = 1;
}
//---------------------------------------------------------------------------
int TFrmEditMemo::EditMemo(TMemo* M, const AnsiString& PromptStr)
{
    Caption = PromptStr;
    int Res = 0;
    ThreadRF =0;
    Mem->Lines->Text = M->Lines->Text;
    if ( ShowModal()>0)
    {
        M->Lines->Text= Mem->Lines->Text;
        Res=1;
    }
    return Res;
}
//---------------------------------------------------------------------------
void TFrmEditMemo::ShowFile(const AnsiString& FileName, const AnsiString& Caption_)
{
    Caption = Caption_;
    Mem->Lines->Clear();
    ThreadRF = new TThreadReadFile(FileName, Mem->Lines);
    Show();
}
//---------------------------------------------------------------------------
void __fastcall TFrmEditMemo::FormDestroy(TObject *Sender)
{
    if (ThreadRF)
    {
        ThreadRF->Terminate();
    }
    ThreadRF=0;
}
//---------------------------------------------------------------------------

void __fastcall TFrmEditMemo::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    if (ThreadRF)
    {
        ThreadRF->Terminate();
    }
    ThreadRF=0;
}
//---------------------------------------------------------------------------


void __fastcall TFrmEditMemo::N6Click(TObject *Sender)
{
  Mem->Print(FrmEditMemo->Caption);
}
//---------------------------------------------------------------------------

