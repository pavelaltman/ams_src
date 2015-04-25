//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "FRM_SORT_ORDER.h"
#include "DAT_MOD.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmSortOrder *FrmSortOrder;
//---------------------------------------------------------------------------
__fastcall TFrmSortOrder::TFrmSortOrder(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmSortOrder::BButAllDownClick(TObject *Sender)
{
    int i = ChLB->ItemIndex;
    int Count  = ChLB->Items->Count;
    if ( i!= -1)
    {
        if (i!= (Count-1) )
        {
            TStrings* S= ChLB->Items;
            AnsiString AS= S->Strings[i];
            int Curr = Mapping [i];
            bool Ch = ChLB->Checked[i];
            for (int j = i ; j< Count-1; j++)
            {
                S->Strings[j]=S->Strings[j+1];
                Mapping [j] =Mapping[j+1];
                ChLB->Checked[j] =  ChLB->Checked[j+1];
            }
            S->Strings[Count-1]=AS;
            Mapping[Count-1] = Curr;
            ChLB->ItemIndex = Count-1;
            ChLB->Checked[Count-1]=Ch;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmSortOrder::BButAllUpClick(TObject *Sender)
{
    //
    int i = ChLB->ItemIndex;
    if ( i!= -1)
    {
        if (i)
        {
            TStrings* S= ChLB->Items;
            AnsiString AS= S->Strings[i];
            int Curr = Mapping[i];
            bool Ch = ChLB->Checked[i];
            for (int j = i; j >0 ; j--)
            {
                Mapping[j]=Mapping[j-1];
                S->Strings[j]= S->Strings[j-1];
                ChLB->Checked[j] =  ChLB->Checked[j-1];
            }
            S->Strings[0]=AS;
            Mapping [0]= Curr;
            ChLB->Checked[0]=Ch;
            ChLB->ItemIndex = 0;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmSortOrder::BButOneDownClick(TObject *Sender)
{
    int i = ChLB->ItemIndex;
    int Count  = ChLB->Items->Count;
    if ( i!= -1)
    {
        if (i!= (Count-1) )
        {
            TStrings* S= ChLB->Items;
            AnsiString AS= S->Strings[i];
            int Curr = Mapping [i];
            bool Ch = ChLB->Checked[i];
            S->Strings[i]=S->Strings[i+1];
            Mapping[i] = Mapping [i+1];
            ChLB->Checked[i] =  ChLB->Checked[i+1];
            S->Strings[i+1]=AS;
            Mapping[i+1] = Curr;
            ChLB->ItemIndex = i+1;
            ChLB->Checked[i+1]=Ch;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmSortOrder::BButOneUpClick(TObject *Sender)
{
    int i = ChLB->ItemIndex;
    if ( i!= -1)
    {
        if (i)
        {
            TStrings* S= ChLB->Items;
            AnsiString AS= S->Strings[i];
            int Curr = Mapping [i];
            bool Ch = ChLB->Checked[i];
            S->Strings[i]=S->Strings[i-1];
            Mapping[i]= Mapping[i-1];
            ChLB->Checked[i] =  ChLB->Checked[i-1];
            S->Strings[i-1]=AS;
            Mapping[i-1]=Curr;
            ChLB->Checked[i-1]=Ch;
            ChLB->ItemIndex = i-1;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmSortOrder::BBCancelClick(TObject *Sender)
{
    ModalResult=-1;
}
//---------------------------------------------------------------------------
void __fastcall TFrmSortOrder::BBOkClick(TObject *Sender)
{
    ModalResult = 1;
}
//-------------------------------------------------------------------------
void TFrmSortOrder::AddStrs(const AnsiString& DispVal, const AnsiString& ResVal)
{
    int Res = ChLB->Items->Count;
    ChLB->Items->Add(DispVal);
    Vals.push_back(ResVal);
    Mapping.push_back(Res);
}
//-------------------------------------------------------------------------
bool TFrmSortOrder::HaveInRes(int Id )
{
    unsigned int i = 0;
    while ( i< Mapping.size() && Mapping[i]!= Id) i++;
    return (i< Mapping.size() && ChLB->Checked[i]) ;
}
//-------------------------------------------------------------------------
void TFrmSortOrder::ClearAll()
{
    ChLB->Items->Clear();
    Mapping.clear();
    Vals.clear();
}
//-------------------------------------------------------------------------
void TFrmSortOrder::GetCommaText(AnsiString& Res, bool NoWaitForUser)
{
    if (NoWaitForUser || ShowModal()==1)
    {
        bool FirstTime=true;
        for (unsigned int i = 0 ; i< Mapping.size(); i++)
        {
            if (ChLB->Checked[i])
            {
                if (FirstTime)
                    FirstTime = false;
                else
                    Res+= ", ";    
                Res+= Vals[Mapping[i]];
            }
        }
    }
}
//-------------------------------------------------------------------------
void TFrmSortOrder::SetSelection(int BegInd, bool Val)
{
    unsigned int i = 0;
    while ( i< Mapping.size() && Mapping[i]!= BegInd) i++;
    if (i< Mapping.size())
        ChLB->Checked[i]= Val ;
}
//-------------------------------------------------------------------------

void __fastcall TFrmSortOrder::FormActivate(TObject *Sender)
{
    TopMostForm = this;
}
//---------------------------------------------------------------------------

