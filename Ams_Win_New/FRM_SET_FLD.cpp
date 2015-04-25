//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "FRM_SET_FLD.h"
#include "DAT_MOD.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Oracle"
#pragma link "AmsInput"
#pragma link "AmsInputDate"
#pragma link "AmsInputInt"
#pragma link "AmsInputStr"
#pragma link "AmsInputSum"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TFrmSetFld::TFrmSetFld(TComponent* Owner)
    : TForm(Owner)
{
    ActInput=NULL;
    SqlFormed = false;
    CurrInd = -1;
    Dirty = false;
}
//---------------------------------------------------------------------------
void TFrmSetFld::Prepare(TFldsDescr* FldsDescr_)
{
    FldsDescr = FldsDescr_;
    DbName = FldsDescr->GetDbName();
    LBFieldsNames->Items->Clear();
    Tie.clear();
    for (int i = 0 ; i< FldsDescr->Count(); i++)
    {
        if ( ((*FldsDescr)[i].DefOpt & Editable) && ((*FldsDescr)[i].DispOpt & Editable))
        {
            if (
                ((*FldsDescr)[i].RealName != "ARTICLE") &&
                ((*FldsDescr)[i].RealName != "VER")
               )
            {
                LBFieldsNames->Items->Add((*FldsDescr)[i].Alias);
                Tie.push_back(i);
            }
        }
    }
    CurrInd=-1;
    LBFieldsNames->ItemIndex=0;
    CheckChange();
}
//---------------------------------------------------------------------------
void TFrmSetFld::FormSqlQuery(TOracleQuery* OQ)
{
    SFldOpt* FO = &((*FldsDescr)[Tie[LBFieldsNames->ItemIndex]]);

    OQ->SQL->Text = " update "+BaseUserName+"."+
        DbName+" D set "+FO->RealName+"= :UPD_VAL where ";

    switch (FO->Type)
    {
        case aftInt:
            OQ->DeclareVariable("UPD_VAL", otInteger);
            OQ->SetVariable("UPD_VAL", EdFldVal->Text.ToIntDef(0));
        break;
        case aftSumma:
            OQ->DeclareVariable("UPD_VAL", otFloat);
            OQ->SetVariable("UPD_VAL", GetSum(EdFldVal->Text));
        break;
        case aftStr:
            OQ->DeclareVariable("UPD_VAL", otString);
            OQ->SetVariable("UPD_VAL", EdFldVal->Text);
        break;
        case aftDate:
            OQ->DeclareVariable("UPD_VAL", otDate);
            OQ->SetVariable("UPD_VAL", GetDate(EdFldVal->Text));
        break;
    }
}

//---------------------------------------------------------------------------
void __fastcall TFrmSetFld::BBOkClick(TObject *Sender)
{
    ModalResult = 1;
}
//---------------------------------------------------------------------------

void __fastcall TFrmSetFld::BBCancelClick(TObject *Sender)
{
    ModalResult = -1;
}
//---------------------------------------------------------------------------

void __fastcall TFrmSetFld::FormActivate(TObject *Sender)
{
    TopMostForm = this;
}
//---------------------------------------------------------------------------
AnsiString TFrmSetFld::GetDescr(void)
{
    //
    SFldOpt* FO = &((*FldsDescr)[Tie[LBFieldsNames->ItemIndex]]);
    AnsiString S= FO->RealName+"="+EdFldVal->Text;
    return S;
}
//---------------------------------------------------------------------------
void __fastcall TFrmSetFld::ActInsAndExecExecute(TObject *Sender)
{
    //SbSaveClick(this);
    BBOkClick(this);
}
//---------------------------------------------------------------------------
void TFrmSetFld::CheckChange()
{
//
}
//---------------------------------------------------------------------------
int TFrmSetFld::SetFld(TFldsDescr* FD)
{
    Prepare (FD);
    int Res =0;
    if (ShowModal()>0)
    {
        if (AskMessage("Вы действительно желаете установить поле "+GetDescr()+"?")==ID_YES)
        {
            Res = 1+IsAlwaysFree();
        }
    }
    return Res;
}
//---------------------------------------------------------------------------
int TFrmSetFld::IsAlwaysFree()
{
    // Возвращает 0х80, если по выбранному полю
    // есть установка ВСЕГДА ДОСТУПНО ДЛЯ РЕДАКТИРОВАНИЯ
    SFldOpt* FO = &((*FldsDescr)[Tie[LBFieldsNames->ItemIndex]]);
    int Res = 0;
    if (FO->DefOpt & AlwaysFree)
    {
        Res = 0x80;
    }
    return Res;
}
//---------------------------------------------------------------------------
SFldOpt* TFrmSetFld::GetSelFld()
{
    return &((*FldsDescr)[Tie[LBFieldsNames->ItemIndex]]);
}
//---------------------------------------------------------------------------

void __fastcall TFrmSetFld::LBFieldsNamesExit(TObject *Sender)
{
    //
    CheckChange();
    //ActInput->BringToFront();
    //ActInput->SetFocus();
}
//---------------------------------------------------------------------------

