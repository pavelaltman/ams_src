//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "AmsInputHistory.h"
#include "FRM_INPUT_HISTORY.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAmsInputHistory *)
{
    new TAmsInputHistory(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAmsInputHistory::TAmsInputHistory(TComponent* Owner)
    : TComponent(Owner), FHistLen (20)
{
    Frm= new TFrmInputHistory(this);
}
//---------------------------------------------------------------------------
namespace Amsinputhistory
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TAmsInputHistory)};
         RegisterComponents("Samples", classes, 0);
    }
}
//---------------------------------------------------------------------------
void __fastcall TAmsInputHistory::SetHistLen (int HL)
{
    if (FHistLen>HL)
        for (int i = HL-1; i<FHistLen; i++)
            Frm->CBInpHist->Items->Delete(HL-1);
    FHistLen = HL;    
};
//---------------------------------------------------------------------------
// Вставить новую запись в историю ввода
void TAmsInputHistory::InsHistVal (const AnsiString& Val)
{
    if (Val=="") return ; 
    if (!Frm->CBInpHist->Items->Count) 
        Frm->CBInpHist->Items->Add(Val);
    else
    if (Frm->CBInpHist->Items->Strings[0]!= Val)
    {
        if (Frm->CBInpHist->Items->Count >= GetHistLen())
            Frm->CBInpHist->Items->Delete (GetHistLen()-1);
        Frm->CBInpHist->Items->Insert(0, Val);
    };    
};
//---------------------------------------------------------------------------
// Получить запись путем выбора пользователем
AnsiString TAmsInputHistory::GetHistVal (TWinControl* WhoAsk)
{
    if (WhoAsk)
    {
        Frm->Position= poDesigned;
        TPoint P = WhoAsk->ClientToScreen(Point(0,0));
        Frm->Top = P.y;
        Frm->Left= P.x;
        Frm->CBInpHist->Width=WhoAsk->Width;
        Frm->Width = WhoAsk->Width; 
    }
    else
        Frm->Position= poScreenCenter;
    if (Frm->CBInpHist->Items->Count)
        Frm->CBInpHist->Text = Frm->CBInpHist->Items->Strings[0];
    else
        Frm->CBInpHist->Text = "";
        
    if (Frm->ShowModal()==1)
        return Frm->CBInpHist->Text;
    else
        return "";    
};
//---------------------------------------------------------------------------
 
