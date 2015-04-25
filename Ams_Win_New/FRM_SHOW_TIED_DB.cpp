//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_SHOW_TIED_DB.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AmsStringGrid"
#pragma resource "*.dfm"
TFrmShowTiedDb *FrmShowTiedDb;
//---------------------------------------------------------------------------
__fastcall TFrmShowTiedDb::TFrmShowTiedDb(TComponent* Owner)
    : TForm(Owner)
{
    SG->Cells[0][0]="Название";
    SG->Cells[1][0]="Значение";
}
//---------------------------------------------------------------------------
void TFrmShowTiedDb::ShowTiedDb(TDataSet* DSet)
{
    ReadTiedBase (  DSet->FieldByName("OP_ID")->AsInteger,
                    DSet->FieldByName("OP_SUBID")->AsInteger);
}
//---------------------------------------------------------------------------
int TFrmShowTiedDb::ReadTiedBase (int OP_ID, int OP_SUBID)
{
    //TODO: Add your source code here
    AmsQuery AQ;
    AQ.SetSqlTextReady(
          "begin "+BaseUserName+".GetLinkDb (:OP_ID,:OP_SUBID,"
          ":DB_NAME,:ARTICLE,:VER,:TIED_TO_DOC);end;");

    AQ.DeclVar("OP_ID",   OP_ID);
    AQ.DeclVar("OP_SUBID",OP_SUBID);
    AQ.DeclVar("DB_NAME", "");
    AQ.DeclVar("ARTICLE", "");
    AQ.DeclVar("VER",     0);
    AQ.DeclVar("TIED_TO_DOC",0);

    AQ.Exec("Находим имя связанной базы данных");
    AnsiString Article = AQ.GetVarStr("ARTICLE");
    int Ver=AQ.GetVarInt("VER");
    AnsiString TB =AQ.GetVarStr("DB_NAME");
    AQ.Clear();
    Caption="";

    if (TB!="")
    {
        if (TiedBase!=TB)
        {
            TiedBase=TB;
            delete FldsDescr;
            FldsDescr=new TFldsDescr;
            int DbId = GetObjId(BaseUserName, TiedBase, AQ());
            FldsDescr->SetDbDescr(TiedBase, DbId, "");
            FldsDescr->ReadFldsDescr();
            VisFlds.clear();
            SG->RowCount=2;
            SG->Cells[0][1]="";
            SG->Cells[1][1]="";
            for (int i=0; i< FldsDescr->Count(); i++)
            {
                SFldOpt& FO = FldsDescr->At(i);
                if (
                    ((FO.DefOpt & Editable) &&(FO.DispOpt & Editable)) ||
                    (!(FO.DefOpt & Hidden) && !(FO.DispOpt & Hidden)) )
                {
                    VisFlds.push_back(i);
                }
            }
            for (unsigned i=0; i< VisFlds.size(); i++)
            {
                if (i)
                    SG->RowCount= SG->RowCount+1;
                SG->Cells[0][i+1]= FldsDescr->At(VisFlds[i]).Alias;
            }
        }
        AQ.Clear();
        Caption="Связанная с операцией БД ";
        if (GC::ViewDebug)
        {
            Caption+=" "+TB+"["+Article+"/"+IntToStr(Ver)+"]";
        }
        AQ.SetSqlText("select * from *."+TiedBase+
                    " where ARTICLE=:ARTICLE and VER=:VER");
        AQ.DeclVar("ARTICLE", Article);
        AQ.DeclVar("VER", Ver);
        AQ.ExecSilent();
        if (AQ.NotEof())
        {
            for (unsigned i=0; i< VisFlds.size(); i++)
            {
                SG->Cells[1][i+1]=FldsDescr->GetAsString(AQ,VisFlds[i]);
            }
        }
        else
        {
            for (unsigned i=0; i< VisFlds.size(); i++)
            {
                SG->Cells[1][i+1]="";
            }
        }
    }
    else
    {
        SG->RowCount=2;
        SG->Cells[0][1]="";
        SG->Cells[1][1]="";
    }
    Show();
    return 0;
}
//---------------------------------------------------------------------------
void __fastcall TFrmShowTiedDb::Button1Click(TObject *Sender)
{
    Hide();    
}
//---------------------------------------------------------------------------

