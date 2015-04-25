//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_GET_DB_CAT.h"
#include "DAT_MOD.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AMS_TREE_VIEW"
#pragma link "AmsDbCatView"
#pragma link "Oracle"
#pragma resource "*.dfm"
TFrmGetDbCat *FrmGetDbCat;
//---------------------------------------------------------------------------
__fastcall TFrmGetDbCat::TFrmGetDbCat(TComponent* Owner)
    : TForm(Owner)
{
    CurrDbId = -1;
    CurrCode = -1;
    CurrDbName= "";
}
//---------------------------------------------------------------------------
void __fastcall TFrmGetDbCat::BBCancelClick(TObject *Sender)
{
    ModalResult=-1;
}
//---------------------------------------------------------------------------
void __fastcall TFrmGetDbCat::BBOkClick(TObject *Sender)
{
    ModalResult = 1;    
}
//-------------------------------------------------------------------------
int TFrmGetDbCat::GetDbCat(int DbId, const AnsiString& DbName, int BegCode)
{
    int Res = 0;
    if (CurrDbId != DbId || CurrDbName != DbName)
    {
        CurrDbId = DbId;
        CurrDbName = DbName;
        CV->Begin(BaseUserName, CurrDbId, "");    
    }
    if (BegCode!=-1)
    {
        CV->MakeActive(BegCode);
    }
    if (ShowModal() >0)
    {
        Res= CV->GetCurrCode();    
    }
    return Res;
}
//---------------------------------------------------------------------------
void __fastcall TFrmGetDbCat::TBRefreshClick(TObject *Sender)
{
    CV->Selected=NULL;
    CV->Begin(BaseUserName, CurrDbId, "");    
}
//---------------------------------------------------------------------------
void __fastcall TFrmGetDbCat::CVKeyPress(TObject *Sender, char &Key)
{
    if (Key==13 || Key==32)
    {
        CV->Selected->Expanded = !CV->Selected->Expanded;
        Key = 0;
    }    
}
//---------------------------------------------------------------------------
void __fastcall TFrmGetDbCat::CVChanging(TObject *Sender, TTreeNode *Node,
      bool &AllowChange)
{
    if (CV->Selected)
    {
        int Res = GetKeyState(VK_SHIFT);
        if (!(Res & 0x80))
            CV->DeSelectCurr();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmGetDbCat::CVChange(TObject *Sender, TTreeNode *Node)
{
    CV->SelectCurr();
}
//---------------------------------------------------------------------------
int TFrmGetDbCat::GetDbCatsList(int DbId, const AnsiString& DbName, AnsiString& Res_)
{
    int Res = 0;
    if (CurrDbId != DbId || CurrDbName != DbName)
    {
        CurrDbId = DbId;
        CurrDbName = DbName;
        CV->Begin(BaseUserName, CurrDbId, "");
    }
    if (ShowModal() >0)
    {
        TAmsSelObjsList* SL = CV->GetSelectedCats ();
        Res_ = "";
        for (int i = 0; i< SL->Count(); i++)
        {
            if (i) Res_+=", ";
            Res_ += IntToStr((*SL)[i]);
        }
    }
    return Res;
}
//---------------------------------------------------------------------------

void __fastcall TFrmGetDbCat::ActOkExecute(TObject *Sender)
{
    ModalResult=1;
}
//---------------------------------------------------------------------------

void __fastcall TFrmGetDbCat::FormShow(TObject *Sender)
{
    CV->SetFocus();
}
//---------------------------------------------------------------------------

