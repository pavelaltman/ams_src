//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_CALC.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RXSpin"
#pragma link "Placemnt"
#pragma resource "*.dfm"
TFrmCalc *FrmCalc;
//---------------------------------------------------------------------------
__fastcall TFrmCalc::TFrmCalc(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmCalc::ButCalcClick(TObject *Sender)
{
    if (ChB->Text!="")
    {
        TGenLocalVars LV;
        OpVars* OV = LV.VARS();
        OV->AddVarSum("RES", 0.0);

        // Выполняем пред формулу
        IP ip;
        ip.SetVars(&LV);
        ip.Enter();
        Prog prog;

        try
        {
            int ok ;
            ProgStr pf;
            char Str[256];
            AnsiString Vals= "Res=0.0;Res=";
            AnsiString S=ChB->Text;
            for (int i=1; i<=S.Length();i++)
            {
                if (S[i]==DecimalSeparator)
                {
                    S[i]='.';
                }
            }
/*            if (ChBRoundUp->Checked)
                Vals+="Round("+S+","+IntToStr((int)SEKolZnak->Value)+")";
            else   */
            Vals+=S;
            strcpy(Str, Vals.c_str());
            pf.Init(Str);

            prog.Compile(&pf) ;
            prog.Link(GC::DocPath.c_str());

            int cur_arg=-1 ;
            if (ok)
                prog.Run(ip);
        }
        catch(OraError ERR)
        {
            ERR.message() ;
        }
        ip.Leave();

        AnsiString S="0.";
        int j=2;
        if (SEKolZnak->Enabled) j=(int)SEKolZnak->Value;
        for (int i=0; i<j ;i++) S+="0";
        for (int i=0; i<6-j ;i++) S+="#";

        EdRes->Text=FormatFloat (S, OV->st->Var("RES").Number());


        ChB->Items->Insert(0, EdRes->Text+" = "+ChB->Text);
        //EdRes->SetFocus();
        //EdRes->SelectAll();
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmCalc::ButCloseClick(TObject *Sender)
{
    ModalResult=-1;
    Hide();
}
//---------------------------------------------------------------------------
void __fastcall TFrmCalc::ActOkExecute(TObject *Sender)
{
   ButCalcClick(Sender);
   ModalResult=1;
}
//---------------------------------------------------------------------------
void __fastcall TFrmCalc::ChBRoundUpClick(TObject *Sender)
{
    SEKolZnak->Enabled = ChBRoundUp->Checked;
    if (ChB->Text!="")
        ButCalcClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TFrmCalc::SEKolZnakChange(TObject *Sender)
{
    ButCalcClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TFrmCalc::FormShow(TObject *Sender)
{
    ChB->SetFocus();
    ChB->SelectAll();
}
//---------------------------------------------------------------------------

