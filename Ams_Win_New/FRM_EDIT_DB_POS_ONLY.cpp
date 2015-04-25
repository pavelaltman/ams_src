//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop
#include "FLDS_DESCR_DISTR.h"
#include "FRM_EDIT_DB_POS_ONLY.h"
#include "DAT_MOD.h"
#include "LOCAL_VARS.h"
#include "FRM_INPUT_OPER.h"
#include "ams_win_c.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "FRM_EDIT_DB_POS"
#pragma link "OracleData"
#pragma resource "*.dfm"
TFrmEditDbPosOnly *FrmEditDbPosOnly;
//---------------------------------------------------------------------------
__fastcall TFrmEditDbPosOnly::TFrmEditDbPosOnly(TComponent* Owner)
    : TFrmEditDbPos(Owner),FSM(&FD, ODS1)
{
}
//---------------------------------------------------------------------------
void TFrmEditDbPosOnly::Init(const AnsiString& DbName_,
                const AnsiString& SetName_, TGenLocalVars* Vars_)
{
    Vars=Vars_;
    FD.ReadDescr(DbName_, SetName_);
    FD.BuildSqlQuery(ODS1);
    db_prog_comp= false;
}
//---------------------------------------------------------------------------
int TFrmEditDbPosOnly::EditPos(const AnsiString& Art, int Ver, int Flags)
{
    ODS1->Active= false;
    ODS1->SetVariable("ARTICLE", Art);
    ODS1->SetVariable("VER", Ver);
    ExecODS(ODS1);

    RunProg(0, 1, 0);
    EditDbPos(ODS1, Flags, &FSM, &FD);
    return 1;
}

//---------------------------------------------------------------------------
int TFrmEditDbPosOnly::EditPosNM(TFrmInputOper*FrmInputOper_,const AnsiString& Art, int Ver, int Flags)
{
    FrmInputOper=FrmInputOper_;
    ODS1->Active= false;
    ODS1->SetVariable("ARTICLE", Art);
    ODS1->SetVariable("VER", Ver);
    ExecODS(ODS1);

    RunProg(0, 1, 0);
    EditDbPos(ODS1, Flags, &FSM, &FD, false, 0, false);
    if (FirstInputEl)
    {
        FirstInputEl->SetFocus();
    }
    Top = (AmsWinC->Height - Height)>>1;
    Left = (AmsWinC->Width - Width)>>1;
    WindowState=wsNormal;
    return 1;
};
//---------------------------------------------------------------------------
int TFrmEditDbPosOnly::EditPosNMOk(TFrmInputOper*FrmInputOper_,const AnsiString& Art, int Ver, int Flags)
{
    FrmInputOper=FrmInputOper_;
    ODS1->Active= false;
    ODS1->SetVariable("ARTICLE", Art);
    ODS1->SetVariable("VER", Ver);
    ExecODS(ODS1);

    RunProg(0, 1, 0);
    EditDbPos(ODS1, Flags, &FSM, &FD, false, 0, false);
    BBOkClick(this);
    return 1;
};
//---------------------------------------------------------------------------
void TFrmEditDbPosOnly::RunProg(bool Post, bool SetFlds, bool ClrFld, bool ClearF4, bool UseFsm)
{
    if (!db_prog_comp)
    {
        ProgFile pf;
        AnsiString fname=GC::DocPath+"\\"+FD.GetDbName()+".prg";
        pf.Init(fname.c_str());
        try {
            db_prog.Compile(&pf);
            db_prog.Link(GC::DocPath.c_str());
        }
        catch(OraError ERR)
        {
            ERR.message(0,0,"Error");
        }
    }
    db_prog_comp = true;

    IP ip;
    ip.Enter();
    extern int cur_arg;
    int pre_cur_arg=cur_arg ;
    cur_arg=-1 ;

    ::string set = ToString(FD.GetSetName());

    try
    {
        FSM.LoadData(ip.Base, SetFlds, ClrFld, ClearF4);
        ip.Base.Structure().Var("SET").assign(VAL(set));
        ip.Base.Structure().Var("PRE").assign(VAL((int)!Post));
        // Вставляем opv и accv
        if (Vars)
        {
            ip.SetVars(Vars);
            ip.Base.AddStruct(Vars->VARS()->Stru());
        }
        db_prog.Run(ip);
    }
    catch(OraError ERR)
    {
        ERR.message(0,0,"After prog error");
    }
    ip.Leave();
    cur_arg=pre_cur_arg ;
};
//---------------------------------------------------------------------------
void __fastcall TFrmEditDbPosOnly::FormDestroy(TObject *Sender)
{
    //delete FSM;
}
//---------------------------------------------------------------------------

void __fastcall TFrmEditDbPosOnly::FormActivate(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TFrmEditDbPosOnly::BBOkClick(TObject *Sender)
{
//
    OkBut();
    FrmInputOper->TiedDbEditEnd();
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmEditDbPosOnly::BBCancelClick(TObject *Sender)
{
    if (!Dirty || AskMessage("Не запоминать результаты изменений?", false)==ID_YES)
    {
        FrmInputOper->TiedDbEditEnd();
        Close();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmEditDbPosOnly::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Action=caFree;
}
//---------------------------------------------------------------------------
void TFrmEditDbPosOnly::LastElPressed()
{
    OkBut();
    FrmInputOper->TiedDbEditEnd();
    Close();
};

