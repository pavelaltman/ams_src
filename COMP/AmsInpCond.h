//---------------------------------------------------------------------------

#ifndef AmsInpCondH
#define AmsInpCondH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class PACKAGE TAmsInpCond : public TWinControl
{
protected:
    static const int VPos;
    TComboBox * CBSign;
    TEdit* EdTxt;
    TLabel* Lab;
    int Type;
    void RecalcOutput();
    AnsiString __fastcall GetAmsPrompt (){return Lab->Caption;};
    void __fastcall SetAmsPrompt (const AnsiString& Val){Lab->Caption=Val; RecalcOutput();};
    void __fastcall CBKeyPress(TObject *Sender, char &Key);
    void __fastcall InpKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
    DYNAMIC void __fastcall DoEnter(void);
    TWinControl* Pred;
    TWinControl* Post;
public:
    __fastcall TAmsInpCond(TComponent* Owner);
    void SetFld(const AnsiString& Prompt, int FldType, const AnsiString& HintVal);
    void ClrCond();
    AnsiString GetStr();
    AnsiString GetSign();
    int GetInt();
    double GetSum();
    TDateTime GetDate();
    int GetPromptLen();
    void SetPromptLen(int PLen);
    //-----------------------------------------------------
    void SetPred (TWinControl* Pred_) {Pred=Pred_;};
    void SetPost (TWinControl* Post_) {Post=Post_;};
    void SetActPred () {if (Pred) Pred->SetFocus();};
    void SetActPost () {if (Post) Post->SetFocus();};
__published:
    __property AnsiString AmsPrompt = {read=GetAmsPrompt, write = SetAmsPrompt};
    __property Align;
    __property Anchors;
};
//---------------------------------------------------------------------------
#endif
 