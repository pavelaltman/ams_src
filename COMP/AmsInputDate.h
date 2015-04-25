//---------------------------------------------------------------------------

#ifndef AmsInputDateH
#define AmsInputDateH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "AmsInputStr.h"
//---------------------------------------------------------------------------
class PACKAGE TAmsInputDate : public TAmsInputStr
{
private:
protected:
    virtual void SetDefaultVal (const AnsiString& DefVal){TAmsInputStr::SetDefaultVal(DefVal);};
    virtual void SetInputField(const AnsiString& Prompt, 
                       int MaxLen=10, 
                       const AnsiString& DefVal=AnsiString())
    {TAmsInputStr::SetInputField(Prompt, MaxLen, DefVal);};
public:
    __fastcall TAmsInputDate(TComponent* Owner);
    virtual void SetDefaultVal(TDateTime DefVal){SetDefaultVal(DateToStr(DefVal));};

    TDateTime GetResultDat();
    virtual void SetDefaultVal (TField* Fld)
    {
        if (Fld->IsNull) FInpVal->Text =""; 
        else FInpVal->Text = DateToStr(Fld->AsDateTime);
    };
    virtual void SetValToFld (TField* Fld) // Значение элем ввода присвоить полю
    {
        if (AmsIsNull) Fld->Clear();
        else Fld->AsDateTime= GetResultDat();
    };
    virtual void SetValToVar (TOracleQuery* OQ, const AnsiString& VarName)
    {
        OQ->DeclareVariable(VarName, otDate);
        if (AmsIsNull)
            OQ->SetVariable(VarName,GetResultDat());
    };
__published:
};
//---------------------------------------------------------------------------
#endif
