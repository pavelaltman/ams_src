//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_CHOOSE_DB_POS.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AMS_TREE_VIEW"
#pragma link "AmsDbCatView"
#pragma link "FRM_BROWS_DB"
#pragma link "Oracle"
#pragma link "OracleData"
#pragma link "RXDBCtrl"
#pragma link "AmsDBGrid"
#pragma resource "*.dfm"
TFrmChooseDbPos *FrmChooseDbPos;
//---------------------------------------------------------------------------
__fastcall TFrmChooseDbPos::TFrmChooseDbPos(TComponent* Owner)
    : TFrmBrowsDb(Owner), filterwas(false)
{
}
//---------------------------------------------------------------------------
// Выбор позиции базы данных пользователем
int TFrmChooseDbPos::SelectDbPos (AnsiString& Article, int& Ver,
        const AnsiString& SetName)
{
    DBG->SelectedRows->Clear();
    int Res = 0;
    Caption = "Выберите позицию базы данных и нажмите Ок";
    SetPos(Article, Ver, -1);
    if (Article!="" && Article!=" ") filterwas=true;
    if ( ShowModal()>0)
    {
        Article = ODS->FieldByName("ARTICLE")->AsString;
        Ver = ODS->FieldByName("VER")->AsInteger;
        Res = 1;
    };
    return Res;
};
//---------------------------------------------------------------------------
void __fastcall TFrmChooseDbPos::ActOkExecute(TObject *Sender)
{
    ModalResult = 1;
}
//---------------------------------------------------------------------------
void __fastcall TFrmChooseDbPos::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    TFrmBrowsDb::FormClose(Sender, Action);
    Action = caHide;
}
//---------------------------------------------------------------------------

void __fastcall TFrmChooseDbPos::DBGFrosenDblClick(TObject *Sender)
{
//
    ModalResult = 1;
}
//---------------------------------------------------------------------------

void __fastcall TFrmChooseDbPos::BBOkClick(TObject *Sender)
{
    ModalResult = 1;
}
//---------------------------------------------------------------------------

void __fastcall TFrmChooseDbPos::BitBtn1Click(TObject *Sender)
{
    ModalResult = -1;
}
//---------------------------------------------------------------------------

void __fastcall TFrmChooseDbPos::ButEnterClick(TObject *Sender)
{
    if (ActiveControl!=CV)
    {
        BBOkClick(this);
    }
    else
        TFrmBrowsDb::ButEnterClick(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFrmChooseDbPos::FormShow(TObject *Sender)
{
  if (filterwas) DBG->SetFocus();
}
//---------------------------------------------------------------------------

