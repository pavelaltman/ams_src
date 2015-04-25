//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_CHOOSE_FORM.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AmsStringGrid"
#pragma link "FRM_CHOOSE_OBJ"
#pragma link "Oracle"
#pragma resource "*.dfm"
TFrmChooseForm *FrmChooseForm=NULL;
//---------------------------------------------------------------------------
__fastcall TFrmChooseForm::TFrmChooseForm(TComponent* Owner)
    : TFrmChooseObj(Owner)
{
}
//---------------------------------------------------------------------------
void TFrmChooseForm::BeginForm(int AccCode_)
{
    AccCode = AccCode_;
    Begin(otRepForm);
}
//---------------------------------------------------------------------------
void TFrmChooseForm::MakeSomething(void)
{
    if (SG->Cells[0][SG->Row]!="")
    {
        if (AccCode==-1)
        {
            //----------------------------------------------------
            // ÂÏÈÕÈÂÀÅÌ ÄÀÍÍÛÅ ÎÁ Ñ×ÅÒÅ â èíòåðïðåòàòîð
            //----------------------------------------------------
            AmsQuery AQ;
            AQ.SetSqlTextReady("select * from "+BaseUserName+".REMS_TURNS where CODE=:CODE");
            AQ.DeclVar("CODE", CurrOpt.AccCode);
            AQ.Exec("Äàííûå î ñ÷åòå");
            if (AQ.NotEof())
            {
                OpVars* OV = LV.VARS();
                OV->AddVars(AQ);
                OV->AddVarInt("COR",CurrOpt.DetailedCorresp);
                OV->AddVarInt("DEBCRED", CurrOpt.DK==SBrwsAccWindowOptions::dkBoth?0:CurrOpt.DK);
                OV->AddVarInt("BYDATES",CurrOpt.ByDate);
                OV->AddVarInt("FILT",CurrOpt.FilterArticle!="");
                OV->AddVarInt("VER",CurrOpt.FilterVer);
                OV->AddVarStr("FILT_FLD",CurrOpt.FldName);
                OV->AddVarStr("ARTICLE",CurrOpt.FilterArticle);
                OV= LV.ACCV();
                OV->AddVars(AQ);
            }
            BeginExecReport(SG->Cells[1][SG->Row], &LV);
            LV.ClearVars();
        }
        else
        {
            //----------------------------------------------------
            // ÂÏÈÕÈÂÀÅÌ ÄÀÍÍÛÅ ÎÁ Ñ×ÅÒÅ â èíòåðïðåòàòîð
            //----------------------------------------------------
            AmsQuery AQ;
            AQ.SetSqlTextReady("select * from "+BaseUserName+".REMS_TURNS where CODE=:CODE");
            AQ.DeclVar("CODE", AccCode);
            AQ.Exec("Äàííûå î ñ÷åòå");
            if (AQ.NotEof())
            {
                OpVars* OV = LV.VARS();
                OV->AddVars(AQ);
                OV->AddVarInt("COR",0);
                OV->AddVarInt("DEBCRED",0);
                OV->AddVarInt("BYDATES",0);
                OV->AddVarInt("FILT",0);
                OV->AddVarInt("VER",0);
                OV->AddVarStr("FILT_FLD","ÊÎË");
                OV->AddVarStr("ARTICLE","");
                OV= LV.ACCV();
                OV->AddVars(AQ);
                OV->AddVarInt("COR",0);
                OV->AddVarInt("DEBCRED",0);
                OV->AddVarInt("BYDATES",0);
                OV->AddVarInt("FILT",0);
                OV->AddVarInt("VER",0);
                OV->AddVarStr("FILT_FLD","ÊÎË");
                OV->AddVarStr("ARTICLE","");
            }
            BeginExecReport(SG->Cells[1][SG->Row], &LV);
            LV.ClearVars();
        }
    }
}
//---------------------------------------------------------------------------
void TFrmChooseForm::BeginForm(SBrwsAccWindowOptions& CurrOpt_)
{
    AccCode=-1;
    CurrOpt=CurrOpt_;
    Begin(otRepForm);
};
//---------------------------------------------------------------------------
void __fastcall TFrmChooseForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    FrmChooseForm=NULL;
    Action=caFree;
}
//---------------------------------------------------------------------------

void __fastcall TFrmChooseForm::FormDestroy(TObject *Sender)
{
//
    
}
//---------------------------------------------------------------------------

