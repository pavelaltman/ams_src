//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_CHOOSE_STD_OPER.h"
#include "FRM_COND_INFO.h"
#include "FRM_INPUT_OPER.h"
#include "ams_win_c.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AmsStringGrid"
#pragma link "FRM_CHOOSE_OBJ"
#pragma link "Oracle"
#pragma resource "*.dfm"
TFrmChooseStdOper *FrmChooseStdOper;

extern bool HaveRole(const AnsiString& RoleName, TOracleQuery* OQ);
//---------------------------------------------------------------------------
__fastcall TFrmChooseStdOper::TFrmChooseStdOper(TComponent* Owner)
    : TFrmChooseObj(Owner)
{
        EscCounter=0;
}
//---------------------------------------------------------------------------
void __fastcall TFrmChooseStdOper::ActShowInfoExecute(TObject *Sender)
{
//
    EscCounter=0;
    int CondId = (SG->Cells[0][SG->Row]).ToIntDef(0);
    if (CondId)
    {
        TFrmCondInfo* Frm;
        Application->CreateForm(__classid(TFrmCondInfo), &Frm);
        Frm->CondInfo(CondId, SG->Cells[2][SG->Row]);
    }
}
//---------------------------------------------------------------------------
void TFrmChooseStdOper::Begin()
{
    TFrmChooseObj::Begin(otStdOper);
}
//---------------------------------------------------------------------------
void TFrmChooseStdOper::MakeSomething(void)
{
    EscCounter=0;
    int CondId = (SG->Cells[0][SG->Row]).ToIntDef(0);
    if (CondId)
    {
        WindowState=wsMinimized;
        SendToBack();
        TFrmInputOper* Frm;
        Application->CreateForm(__classid (TFrmInputOper), &Frm);
        Frm->Init(0,0,0,0, CondId);
    };
};
//---------------------------------------------------------------------------
void __fastcall TFrmChooseStdOper::FormDestroy(TObject *Sender)
{
    AmsWinC->FrmChooseStdOper=0;
}
//---------------------------------------------------------------------------
void TFrmChooseStdOper::FCKeyDown (WORD &Key, TShiftState Shift)
{
//
};
//---------------------------------------------------------------------------

void __fastcall TFrmChooseStdOper::Button2Click(TObject *Sender)
{
    EscCounter++;
    WindowState=wsMinimized;
    SendToBack();
    // ѕровер€ем есть ли у пользовател€ право на введение
    // нестандартных проводок
    if (EscCounter<2)
    {
        AmsQuery AQ;
        if (    HaveRole(BaseUserName+"_INPUTOPER", AQ()) ||
                HaveRole(BaseUserName+"SUPER", AQ()))
        {
            TFrmInputOper* Frm;
            Application->CreateForm(__classid (TFrmInputOper), &Frm);
            Frm->Init(0,0,0,0,0);
        }
    }
    else
    {
        EscCounter=0;

    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmChooseStdOper::ToolButton2Click(TObject *Sender)
{
    WindowState=wsMinimized;
    SendToBack();
    TFrmInputOper* Frm;
    Application->CreateForm(__classid (TFrmInputOper), &Frm);
    Frm->Init(0,0,0,0,0);

}
//---------------------------------------------------------------------------

void __fastcall TFrmChooseStdOper::ActNonStdOperExecute(TObject *Sender)
{
  ToolButton2Click(Sender);
}

//---------------------------------------------------------------------------

void __fastcall TFrmChooseStdOper::ActRefreshExecute(TObject *Sender)
{
  ToolButton1Click(Sender);
}
//---------------------------------------------------------------------------

