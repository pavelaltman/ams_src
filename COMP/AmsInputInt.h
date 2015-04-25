//---------------------------------------------------------------------------

#ifndef AmsInputIntH
#define AmsInputIntH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "AmsInputStr.h"
//---------------------------------------------------------------------------
class PACKAGE TAmsInputInt : public TAmsInputStr
{
protected:
    virtual void SetDefaultVal (const AnsiString& DefVal){TAmsInputStr::SetDefaultVal(DefVal);};
    virtual void SetInputField(const AnsiString& Prompt, 
                       int MaxLen=50, 
                       const AnsiString& DefVal=AnsiString())
    {TAmsInputStr::SetInputField(Prompt, MaxLen, DefVal);};

public:
    __fastcall TAmsInputInt(TComponent* Owner);
    void SetDefaultVal(int DefVal){SetDefaultVal(IntToStr(DefVal));};
    virtual void SetDefaultVal (TField* Fld)
    {
        if (Fld->IsNull) FInpVal->Text = "";
         else FInpVal->Text = IntToStr(Fld->AsInteger);
    };
    virtual void SetInputFieldInt(const AnsiString& Prompt, int DefaultVal=0)
    {SetInputField(Prompt, 9, IntToStr(DefaultVal));};
    int GetResultInt();
    virtual void SetValToFld (TField* Fld) // Значение элем ввода присвоить полю 
    {
        if (AmsIsNull) Fld->Clear(); 
        else Fld->AsInteger= GetResultInt();
    };
    virtual void SetValToVar (TOracleQuery* OQ, const AnsiString& VarName)
    {
        OQ->DeclareVariable(VarName, otInteger);
        if (!AmsIsNull)
            OQ->SetVariable(VarName,GetResultInt());
    };
__published:
};
//---------------------------------------------------------------------------
#endif
