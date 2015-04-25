//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_VIEW_OPTIONS_AW.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmViewOptionsAW *FrmViewOptionsAW;
//---------------------------------------------------------------------------
__fastcall TFrmViewOptionsAW::TFrmViewOptionsAW(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
int TFrmViewOptionsAW::ViewOptions(SBrwsAccWindowOptions& CurrOpt)
{
    RGOpt->ItemIndex            =CurrOpt.DK-1;
    ChBCorresp->Checked         =CurrOpt.DetailedCorresp;
    ChBGroupByDate->Checked     =CurrOpt.ByDate;
    ChBShowAnal->Checked        =CurrOpt.Analitic;
    ChBSortByLastModif->Checked =CurrOpt.SortByModifDate;

    if (CurrOpt.AccsTreeDepth==-1)
        CBDepth->ItemIndex=0;
    else
        CBDepth->ItemIndex=CurrOpt.AccsTreeDepth-1;
    HaveAnal=CurrOpt.AnName!="";

    CBDepth->Text           =CBDepth->Items->Strings[CBDepth->ItemIndex];
    ChBCheckOps->Checked    =CurrOpt.CheckOps;
    CheckRules();
    int Res=0;
    if (ShowModal()==1)
    {
        Res=1;
        CurrOpt.DK                  =RGOpt->ItemIndex+1;
        CurrOpt.DetailedCorresp     =ChBCorresp->Checked;
        CurrOpt.ByDate              =ChBGroupByDate->Checked;
        CurrOpt.Analitic            =ChBShowAnal->Checked;
        CurrOpt.SortByModifDate     =ChBSortByLastModif->Checked;

        if (!CBDepth->ItemIndex)
            CurrOpt.AccsTreeDepth==-1;
        else
            CurrOpt.AccsTreeDepth=CBDepth->ItemIndex+1;

        CurrOpt.CheckOps=ChBCheckOps->Checked;
    }
    return Res;
}
//---------------------------------------------------------------------------
void __fastcall TFrmViewOptionsAW::BBOkClick(TObject *Sender)
{
    ModalResult=1;
}
//---------------------------------------------------------------------------
void __fastcall TFrmViewOptionsAW::BBCancelClick(TObject *Sender)
{
    ModalResult=-1;
}
//---------------------------------------------------------------------------

void __fastcall TFrmViewOptionsAW::ActOkExecute(TObject *Sender)
{
    BBOkClick(this);
}
//---------------------------------------------------------------------------
void TFrmViewOptionsAW::CheckRules(void)
{
    if (ChBGroupByDate->Checked)
    {
        ChBShowAnal->Enabled=false;
        ChBSortByLastModif->Enabled=false;
        ChBShowAnal->Checked=false;
        ChBSortByLastModif->Checked=false;
    }
    else
    {
        ChBShowAnal->Enabled=true;
        ChBSortByLastModif->Enabled=true;
    }
    if (!HaveAnal)
    {
        ChBShowAnal->Enabled=false;
    }
    CBDepth->Enabled=ChBCorresp->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFrmViewOptionsAW::ChBCorrespClick(TObject *Sender)
{
    CheckRules();    
}
//---------------------------------------------------------------------------

