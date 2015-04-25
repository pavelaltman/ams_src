//---------------------------------------------------------------------------

#ifndef AmsInputStrH
#define AmsInputStrH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "AmsInput.h"
//---------------------------------------------------------------------------
enum EnPromptPos {ppTop, ppLeft};
//---------------------------------------------------------------------------
class PACKAGE TAmsInputStr : public TAmsInput
{
private:
protected:
    TEdit* FInpVal;
    TButton* FBut;
    TButton* FClrBut;

    bool FHistButVis;

    TNotifyEvent FOnChangeEdit;

    virtual int __fastcall GetAmsMaxLen(){return FInpVal->MaxLength;};
    virtual void __fastcall SetAmsMaxLen(int Val){FInpVal->MaxLength=Val;};

    virtual void __fastcall SetAmsEnabled(bool Val);
    
    void __fastcall SetHistButVis (bool NewVal)
    {
        if (!NewVal)
        {
            FBut->Visible= false;
            FHistButVis = false;
        }
        else
        {
            if (FInputHistory)
            {
                FBut->Visible= true;
                FHistButVis = true;
            }
        }
    };

    virtual void __fastcall Loaded(void);
    DYNAMIC void __fastcall Resize(void);

    void __fastcall ChangeEdit(System::TObject* Sender);
    void __fastcall EditExit(System::TObject* Sender)
    {if (FInputHistory) FInputHistory->InsHistVal(FInpVal->Text);};

    virtual void ChangeEdit2();

    void __fastcall HistButPressed(System::TObject* Sender);
    void __fastcall ClrButPressed(System::TObject* Sender);

    void __fastcall InpEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);

    virtual void __fastcall SetIsNull(bool Val)
    {if (Val) FInpVal->Text = "";};
    virtual bool __fastcall GetIsNull()
    {return FInpVal->Text == "";};

public:
    __fastcall TAmsInputStr(TComponent* Owner);
    virtual void Clear(){FInpVal->Text = "";};
    virtual void SetDefaultVal (const AnsiString& DefVal){FInpVal->Text = DefVal;};
    virtual void SetDefaultVal (TField* Fld)
    {if (Fld->IsNull) SetIsNull(true); else FInpVal->Text = Fld->AsString;};
    virtual void SetValToFld (TField* Fld) // Значение элем ввода присвоить полю
    {Fld->AsString= FInpVal->Text;};

    virtual void SetInputField(const AnsiString& Prompt,
                       int MaxLen=50,
                       const AnsiString& DefVal=AnsiString())
    {

        SetAmsMaxLen(MaxLen);
        FInpVal->Text = DefVal;
    };
    AnsiString GetResultStr(){return FInpVal->Text;};
    virtual void SetValToVar (TOracleQuery* OQ, const AnsiString& VarName)
    {
        OQ->DeclareVariable(VarName, otString);
        OQ->SetVariable(VarName,GetResultStr());
    };

    // Позиционирует курсор за последий символ ввода
    virtual void GoBack();

__published:
    __property TNotifyEvent OnChangeEdit = {read=FOnChangeEdit, write=FOnChangeEdit};
    __property bool AmsHistButVisible = {read = FHistButVis, write = SetHistButVis};
};
//---------------------------------------------------------------------------
#endif
 