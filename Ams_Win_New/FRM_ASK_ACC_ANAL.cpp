//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_ASK_ACC_ANAL.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmAskAccAnal *FrmAskAccAnal;
//---------------------------------------------------------------------------
__fastcall TFrmAskAccAnal::TFrmAskAccAnal(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmAskAccAnal::BBOkClick(TObject *Sender)
{
    ModalResult=1;    
}
//---------------------------------------------------------------------------
void __fastcall TFrmAskAccAnal::BitBtn1Click(TObject *Sender)
{
    ModalResult=-1;
}
//---------------------------------------------------------------------------
TAccName TFrmAskAccAnal::AskAccAnal(int AnalId)
{
    AmsQuery AQ;
    AQ.SetSqlTextReady(
        //        0    1      2
        "select CODE, NAME, DESCR from "+BaseUserName+".ACCS "
        "where ANAL_ID=:ANAL_ID and "+BaseUserName+".HAVEPRIV(CODE)=1 order by NAME"
    );
    AQ.DeclVar("ANAL_ID", AnalId);
    AQ.Exec("Получаем список счетов");
    Accs.clear();
    TStrings* Str = LBAccs->Items;
    Str->Clear();
    while (AQ.NotEof())
    {
        AnsiString UserName = TAccName::ToUserName(AQ.GetFldStr(1));
        Accs[UserName]=TAccName( AQ.GetFldInt(0), AQ.GetFldStr(1),AQ.GetFldStr(2));
        Str->Add(UserName+" "+AQ.GetFldStr(2));
        AQ.Next();
    }

    if (ShowModal()>0)
    {
        if (LBAccs->ItemIndex!=-1)
        {
            AnsiString UserName=Str->Strings[LBAccs->ItemIndex];
            int u = UserName.Pos(" ");
            UserName = UserName.SubString(1, u-1);
            return Accs[UserName];
        }
    }
    return TAccName();
}
//---------------------------------------------------------------------------

