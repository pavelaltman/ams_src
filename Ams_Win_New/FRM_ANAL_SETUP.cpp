//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_ANAL_SETUP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AmsStringGrid"
#pragma resource "*.dfm"
TFrmAnalSetup *FrmAnalSetup;
//---------------------------------------------------------------------------
__fastcall TFrmAnalSetup::TFrmAnalSetup(TComponent* Owner)
    : TForm(Owner)
{
    B1= new Graphics::TBitmap();
    B2= new Graphics::TBitmap();
    DatMod -> ImageList1->GetBitmap(30, B1);
    DatMod -> ImageList1->GetBitmap(29, B2);
}
//---------------------------------------------------------------------------
void TFrmAnalSetup::AnalSetup(int AccCode_, int AnalId_)
{

    SG->Cells[0][0]="Поле";
    SG->Cells[1][0]="Ширина";
    SG->Cells[2][0]="Скрытость";
    SG->Cells[3][0]="Редактируемость";
    SG->Cells[4][0]="Развернутость";

    SG->Cells[0][1]="";
    SG->Cells[1][1]="";
    SG->Cells[2][1]="";
    SG->Cells[3][1]="";
    SG->Cells[4][1]="";


    AccCode=AccCode_;
    AnalId=AnalId_;
    FO.clear();
    AmsQuery AQ;
    AQ.SetSqlText("select NAME, DESCR from *.OBJS where OID=:OID");
    AQ.DeclVar("OID", AnalId);
    AQ.Exec("Имя и описание аналитики");
    if (AQ.NotEof())
    {
        AnName= AQ.GetFldStr(0);
        AnDescr=AQ.GetFldStr(1);
    }
    else
    {
        Close();
        return;
    }

    Caption="Установки для полей "+AnName+" "+AnDescr;
    AQ.Clear();
    //                        0       1       2        3       4
    AQ.SetSqlText("select REALNAME, ALIAS, DISP_OPT, WIDTH, DEF_OPT "
        "from *.DBSTR where DBID=:DBID and DELETED=0");
    AQ.DeclVar("DBID", AnalId);
    AQ.Exec("Поля аналитики и установки к ним");
    SFOpt F;
    int i=1;
    while (AQ.NotEof())
    {
        int DefOpt = AQ.GetFldInt(4);
        if ( (DefOpt & Editable) || !(DefOpt & Hidden))
        {
            int DispOpt = AQ.GetFldInt(2);
            F.Name= AQ.GetFldStr(0);
            F.Width=AQ.GetFldInt(3);
            F.Edit= DispOpt & Editable;
            F.Hide= DispOpt & Hidden;
            F.Razv= DispOpt & DebetCredit;
            FO.push_back(F);
            if (i>= SG->RowCount)
                SG->RowCount=i+1;
            SG->Cells[0][i]=AQ.GetFldStr(1);
            SG->Cells[1][i]=IntToStr(F.Width);
            SG->Cells[2][i]=IntToStr(F.Hide);
            SG->Cells[3][i]=IntToStr(F.Edit);
            SG->Cells[4][i]=IntToStr(F.Razv);
            i++;
        }
        AQ.Next();
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmAnalSetup::BBOkClick(TObject *Sender)
{
    for (unsigned i =0 ; i< FO.size(); i++)
    {
        int Row = i+1;
        SFOpt& F = FO[i];
        if (F.Width != SG->Cells[1][Row].ToIntDef(0) ||
            int(F.Hide)!=StrToInt(SG->Cells[2][Row])||
            int(F.Edit)!=StrToInt(SG->Cells[3][Row])||
            int(F.Razv)!=StrToInt(SG->Cells[4][Row]))
        {
            unsigned DefOpt=0;
            if (SG->Cells[2][Row]=="1") DefOpt+=Hidden;
            if (SG->Cells[3][Row]=="1") DefOpt+=Editable;
            if (SG->Cells[4][Row]=="1") DefOpt+=DebetCredit;

            AmsQuery AQ;
            AQ.SetPLText("update *.DBSTR set DISP_OPT=:DISP_OPT, WIDTH=:WIDTH "
              "where DBID=:DBID and REALNAME=:REALNAME; commit;");
            AQ.DeclVar("DISP_OPT", double(DefOpt));
            int Width = SG->Cells[1][Row].ToIntDef(0);
            AQ.DeclVar("WIDTH", Width);
            AQ.DeclVar("DBID", AnalId);
            AQ.DeclVar("REALNAME", F.Name);
            AQ.Exec("Вносим пользовательские установки");
        }
    }
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFrmAnalSetup::BitBtn1Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFrmAnalSetup::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Action=caFree;
}
//---------------------------------------------------------------------------
void __fastcall TFrmAnalSetup::ActOkExecute(TObject *Sender)
{
    BBOkClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TFrmAnalSetup::SGDblClick(TObject *Sender)
{
//
    int Col=SG->Col;
    int Row=SG->Row;
    if (Row>0 && Row<(SG->RowCount))
    {
        switch (Col)
        {
            case 1:
            {
                int Width = SG->Cells[Col][Row].ToIntDef(0);
                if (AskInt("Ширина поля для "+SG->Cells[0][Row], Width))
                {
                    SG->Cells[Col][Row]=IntToStr(Width);
                }
            }
            break;
            case 2:
            case 3:
            case 4:
                if (SG->Cells[Col][Row]=="1")
                    SG->Cells[Col][Row]="0";
                else
                    SG->Cells[Col][Row]="1";
            break;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmAnalSetup::SGDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
//
    if (ACol>1 && ARow>0)
    {
    Graphics::TBitmap* BM= NULL;
    bool WriteIt = SG->Cells[ACol][ARow]=="1";
    if (WriteIt)
    {
        if (State.Contains(gdSelected))
        {
          SG->Canvas->Brush->Color = clActiveCaption;
          BM=B1;
        }
        else
        {
          SG->Canvas->Brush->Color = SG->Color;
          BM=B2;
        }
    }
    else
    {
        if (State.Contains(gdSelected))
          SG->Canvas->Brush->Color = clActiveCaption;
        else
          SG->Canvas->Brush->Color = SG->Color;
    }
    SG->Canvas->FillRect(Rect);
    if (BM) SG->Canvas->Draw( Rect.left, Rect.top, BM);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmAnalSetup::FormDestroy(TObject *Sender)
{
    delete B1;
    B1=0;
    delete B2;
    B2=0;
}
//---------------------------------------------------------------------------
void __fastcall TFrmAnalSetup::FormCreate(TObject *Sender)
{
//
    Width=499;
    Height=249;
    SG->Height= Height-53;
    SG->Width=Width-SG->Left;
    BitBtn1->Left= Width-BitBtn1->Width-8;
    BitBtn1->Top= Height-(BitBtn1->Height+26);
    BBOk->Top=BitBtn1->Top;
}
//---------------------------------------------------------------------------

