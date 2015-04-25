//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_CHOOSE_NOT_PLAIN_PERIOD.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ToolEdit"
#pragma link "RXSpin"
#pragma resource "*.dfm"
TFrmChooseNotPlainPeriod *FrmChooseNotPlainPeriod;
//---------------------------------------------------------------------------
__fastcall TFrmChooseNotPlainPeriod::TFrmChooseNotPlainPeriod(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmChooseNotPlainPeriod::BBOkClick(TObject *Sender)
{
    if (DEBeg->Date>DEEnd->Date)
    {
        ErrorMessage("ƒата начала должна быть меньше, чем дата конца");
    }
    else
    {
        ModalResult=1;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmChooseNotPlainPeriod::BitBtn1Click(TObject *Sender)
{
    ModalResult=-1;    
}
//---------------------------------------------------------------------------
bool TFrmChooseNotPlainPeriod::ChooseNotPlainPeriod(TDateTime& BegPeriod,
                                                TDateTime& EndPeriod,
                                                bool& SyntOnly)
{
    DEBeg->Date = BegPeriod;
    DEEnd->Date = EndPeriod;
    ChBSyntOnly->Checked= SyntOnly;
    bool Res = false;
    if (ShowModal()==1)
    {
        BegPeriod=(int)DEBeg->Date;
        EndPeriod=(int)DEEnd->Date;
        SyntOnly=ChBSyntOnly->Checked;
        Res=true;
    }
    return Res;
}
//---------------------------------------------------------------------------
void __fastcall TFrmChooseNotPlainPeriod::Action1Execute(TObject *Sender)
{
    BBOkClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TFrmChooseNotPlainPeriod::ChBJustAddClick(TObject *Sender)
{
    if (ChBJustAdd->Checked)
    {
        SpEdit->Enabled=true;
        DEEnd->Enabled=false;
        DEEnd->Date=DEBeg->Date+(int)SpEdit->Value;
    }
    else
    {
        SpEdit->Enabled=false;
        DEEnd->Enabled=true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmChooseNotPlainPeriod::SpEditChange(TObject *Sender)
{
    if (SpEdit->Enabled)
    {
        DEEnd->Date=DEBeg->Date+(int)SpEdit->Value;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmChooseNotPlainPeriod::DEBegChange(TObject *Sender)
{
    SpEditChange(Sender);
}
//---------------------------------------------------------------------------

