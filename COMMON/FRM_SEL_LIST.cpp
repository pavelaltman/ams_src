//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_SEL_LIST.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmSelList *FrmSelList;
//---------------------------------------------------------------------------
__fastcall TFrmSelList::TFrmSelList(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
AnsiString TFrmSelList::AskFieldStr(TOracleQuery* OQ, const AnsiString& Msg, TInclude FQ)
{
    LB->Items->Clear();
    Lab->Caption = Msg;
    while (!OQ->Eof)
    {
        if (FQ==NULL || FQ(OQ))
        {
            LB->Items->Add (OQ->FieldAsString(0));
        }
        OQ->Next();
    }
    AnsiString Res = "";
    if (LB->Items->Count)
    {
        LB->ItemIndex=0;
    }
    if (ShowModal()>0)
    {
        Res= LB->Items->Strings[LB->ItemIndex];
    }
    return Res;
}
//---------------------------------------------------------------------------
AnsiString TFrmSelList::AskFieldStr2(TOracleQuery* OQ, const AnsiString& Msg, TInclude FQ)
{
    LB->Items->Clear();
    Lab->Caption = Msg;
    std::vector<AnsiString> ResVal;
    while (!OQ->Eof)
    {
        if (FQ==NULL || FQ(OQ))
        {
            ResVal.push_back(OQ->FieldAsString(1));
            LB->Items->Add (OQ->FieldAsString(0));
        }
        OQ->Next();
    }
    AnsiString Res = "";
    if (LB->Items->Count)
    {
        LB->ItemIndex=0;
    }
    if (ShowModal()>0)
    {
        Res= ResVal[LB->ItemIndex];
    }
    return Res;
}
//---------------------------------------------------------------------------
void __fastcall TFrmSelList::BBOkClick(TObject *Sender)
{
    ModalResult = 1;
}
//---------------------------------------------------------------------------
void __fastcall TFrmSelList::BitBtn1Click(TObject *Sender)
{
    ModalResult = -1;    
}
//---------------------------------------------------------------------------
void __fastcall TFrmSelList::FormActivate(TObject *Sender)
{
    LB->SetFocus();
}
//---------------------------------------------------------------------------

