//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_EDIT_BUCH_SETUP.h"
#include "MAIN_FRM.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "OracleData"
#pragma link "OracleNavigator"
#pragma link "AmsDBGrid"
#pragma link "RXDBCtrl"
#pragma link "RXDBCtrl"
#pragma resource "*.dfm"
TFrmEditBuchSetup *FrmEditBuchSetup;
//---------------------------------------------------------------------------
__fastcall TFrmEditBuchSetup::TFrmEditBuchSetup(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmEditBuchSetup::FormCreate(TObject *Sender)
{
    //
    int BuchNum=0;
    do {
        AmsQuery AQ;
        AQ.SetSqlTextReady("select ID from AMS.BOOKS where BASE_USER=:BASE_USER");
        AQ.DeclVar("BASE_USER", BaseUserName);
        AQ.Exec("Находим код бухгалтерии");
        if (AQ.NotEof())
        {
            BuchNum=AQ.GetFldInt(0);
        }
    } while (0);
    ODSLic->Active=true;
    if (BuchNum)
    {
        TSearchRecordOptions SO;
        SO << srFromBeginning;
        ODSLic->SearchRecord("ID",BuchNum, SO);
        ODSBooks->SetVariable("ID", BuchNum);
    }
    ODSBooks->Active=true;
    if (BuchNum)
    {
        TSearchRecordOptions SO;
        SO << srFromBeginning;
        ODSBooks->SearchRecord("BASE_USER", BaseUserName, SO);
    }
}
//---------------------------------------------------------------------------

