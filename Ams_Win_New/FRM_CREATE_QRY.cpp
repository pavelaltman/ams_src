//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "AMS_HDR.h"
#include "FRM_CREATE_QRY.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmCreateQry *FrmCreateQry;
//---------------------------------------------------------------------------
__fastcall TFrmCreateQry::TFrmCreateQry(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFrmCreateQry::FormActivate(TObject *Sender)
{
    AmsQuery AQ;
    AQ.SetSqlText("select O.OID, O.DESCR, D.REALNAME from *.OBJS O, *.DBSTR D "
                  "where O.OTYPE=4 and O.OID=D.DBID  "
                  "and D.ALIAS not in('ÊàòÈÄ','ÀÐÒÈÊÓË','ÂÅÐ') order by O.OID");
    AQ.Exec("Âûáèðàåì âñå áàçû äàííûõ");
    int CurDBID=-1;
    TTreeNode* tnd=NULL;
    while (AQ.NotEof())
    {

        TTreeDBData *td=new TTreeDBData();
        td->Id=AQ.GetFldInt(0);
        td->TableName=AQ.GetFldStr(1);
        if (CurDBID==td->Id)
           tvDB->Items->AddChildObject(tnd, AQ.GetFldStr(2),td);
        else
        {
           CurDBID=td->Id;
           tvDB->Items->AddObject(NULL, AQ.GetFldStr(2),td);
        }
        AQ.Next();
    }
}
