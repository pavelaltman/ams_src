//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_GET_SET_NAME.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmGetSetName *FrmGetSetName;
//---------------------------------------------------------------------------
__fastcall TFrmGetSetName::TFrmGetSetName(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

AnsiString TFrmGetSetName::GetSetName(int OID, const AnsiString& Prompt_)
{
    AmsQuery AQ;
    Caption = Prompt_;
    CBSetName->Clear();
    CBSetName->Text=" ";
    CBSetName->Items->Add(" ");
    AQ.SetSqlText("select SETNAME from *.DBOPT where DBID=:DBID");
    AQ.DeclVar("DBID", OID);
    AQ.Exec("");
    if (AQ()->Eof)
        return " ";
    FillCB (CBSetName->Items, AQ());
    ShowModal();
    return CBSetName->Text;
}
