//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "FRM_HIST_VIEW.h"
#include "DAT_MOD.h"
#include "FRM_EDIT_DB_POS.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "OracleData"
#pragma link "RXDBCtrl"
#pragma resource "*.dfm"
TFrmHistView *FrmHistView;
//---------------------------------------------------------------------------
__fastcall TFrmHistView::TFrmHistView(TComponent* Owner)
    : TForm(Owner)
{
    ODS->SQL->Text =
    "select H.REVISION, H.REV_TIME, H.USERNAME, U.FULL_NAME "
    "from "+BaseUserName+".dbhistory H, "+BaseUserName+".USER_OPTIONS U "
    "where "
	"H.DB_ID = :DB_ID and "
	"H.ARTICLE = :ARTICLE and "
	"H.VER = :VER and "
	"H.USERNAME = U.USERNAME ";

    ActShowOld->ShortCut=13;
}
//---------------------------------------------------------------------------
void __fastcall TFrmHistView::BBOkClick(TObject *Sender)
{
    ModalResult =1;
}
//-------------------------------------------------------------------------
void TFrmHistView::Begin(int DbId_, const AnsiString& DbName_,
                TFldsDescr* FldsDescr_, TOracleDataSet* ParentODS_)
{
    ParentODS = ParentODS_;
    DbId = DbId_;
    DbName = DbName_;

    ODS->SetVariable("DB_ID", DbId);
    Caption = "История изменения в базе данных "+DbName;
    FldsDescr= FldsDescr_;

}
//-------------------------------------------------------------------------
void TFrmHistView::ShowHist (const AnsiString& Article_, int Ver_)
{
    Article= Article_;
    Ver=Ver_;
    ODS->SetVariable("ARTICLE", Article);
    ODS->SetVariable("VER", Ver);
    ODS->Active=false;
    ODS->Active = true;
    ShowModal();
}
//-------------------------------------------------------------------------
void __fastcall TFrmHistView::FormCreate(TObject *Sender)
{
     FrmEditDbPos=NULL;
}
//---------------------------------------------------------------------------

void __fastcall TFrmHistView::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Action =caHide;
}
//---------------------------------------------------------------------------

void __fastcall TFrmHistView::ActShowOldExecute(TObject *Sender)
{
//
        if (!ODS->Eof)
        {

            if (!FrmEditDbPos)
            {
                FrmEditDbPos = new TFrmEditDbPos(this);
            }

            AnsiString S = "select ";
            FldsDescr->GetCommaText(S);
            S+=" from "+BaseUserName+"."+DbName+"_H where CATID =:CATID "
                "and ARTICLE = :ARTICLE and VER = :VER";
            ODS_H->SQL->Text=S;
            ODS_H->DeclareVariable("CATID"     , otInteger);
            ODS_H->DeclareVariable("ARTICLE"   , otString);
            ODS_H->DeclareVariable("VER"       , otInteger);
            ODS_H->SetVariable("ARTICLE", Article);
            ODS_H->SetVariable("VER"    , Ver);
            ODS_H->SetVariable("CATID"  , ODS->FieldByName("REVISION")->AsInteger);
            ODS_H->Active = false;
            ODS_H->Active = true;
            FrmEditDbPos->EditDbPos (ODS_H, eoDubbing|eoReadOnly|eoDbFlds, NULL, FldsDescr);
        }
}
//---------------------------------------------------------------------------
void __fastcall TFrmHistView::RxDBGrid1DblClick(TObject *Sender)
{
    ActShowOld->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TFrmHistView::ActMakeOkExecute(TObject *Sender)
{
    if (AskMessage("Восстановить это состояние в БД?", false)==ID_YES)
    {
        AnsiString Cols;
        bool FirstTime=true;
        for (int i = 0; i < FldsDescr->Count(); i++)      
        {
            SFldOpt& FO = FldsDescr->At(i);
            if (FO.FldAddInfo==aiDb)
            {
                AnsiString Upp=FO.RealName.UpperCase();
                if (Upp!="ARTICLE" && Upp!="VER" && Upp!="CATID")
                {
                    if (!FirstTime) Cols+=", ";
                    else FirstTime=false;
                    Cols += Upp;
                }
            }
        }
        AnsiString Res="begin declare Tmp number; begin \r\n"
        "select max(CATID) into Tmp from *."+DbName+" where ARTICLE=:ART and VER=:VER;\r\n"
        "if Tmp is NULL then \r\n"
        "   select max(CATID) into Tmp from *."+DbName+" where ARTICLE=:ART;\r\n"
        "   if Tmp is NULL then \r\n"
        "       select min(OWNCAT) into Tmp from *.DBCAT \r\n"
        "       where DBID="+IntToStr(DbId)+" and CATID is NULL;\r\n"
        "   end if; -- нет такого артикула \r\n"
        "   insert into *."+DbName+" (CATID, ARTICLE, VER, "+Cols+") \r\n"
        "   (select Tmp, ARTICLE, VER, "+Cols+"\r\n"
        "    from *."+DbName+"_H where ARTICLE=:ART and VER=:VER and CATID=:REV);\r\n"
        "else  -- имеем в БД такой артикул-версия\r\n"
        "   update *."+DbName+" set ("+Cols+")= \r\n"
        "   (select "+Cols+" \r\n"
        "       from *."+DbName+"_H where ARTICLE=:ART and VER=:VER and CATID=:REV)\r\n"
        "   where ARTICLE=:ART and VER=:VER;\r\n"
        "end if;\r\n"
        "end;end;";
        AmsQuery AQ;
        AQ.SetSqlText(Res);
        AQ.DeclVar("ART", Article);
        AQ.DeclVar("VER", Ver);
        AQ.DeclVar("REV", ODS->FieldByName("REVISION")->AsInteger);
        AQ.Exec("Восстанавливаем что было");
        ParentODS->RefreshRecord();        
    }
}
//---------------------------------------------------------------------------

