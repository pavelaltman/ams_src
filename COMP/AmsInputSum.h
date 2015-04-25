//---------------------------------------------------------------------------

#ifndef AmsInputSumH
#define AmsInputSumH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "AmsInputStr.h"
//---------------------------------------------------------------------------
class PACKAGE TAmsInputSum : public TAmsInputStr
{
private:
protected:
    virtual void SetDefaultVal (const AnsiString& DefVal){TAmsInputStr::SetDefaultVal(DefVal);};
    virtual void SetInputField(const AnsiString& Prompt, 
                       int MaxLen=50, 
                       const AnsiString& DefVal=AnsiString())
    {TAmsInputStr::SetInputField(Prompt, MaxLen, DefVal);};
    virtual void ChangeEdit2();
public:
    __fastcall TAmsInputSum(TComponent* Owner);
    virtual void SetDefaultVal(long double DefVal){SetDefaultVal(FloatToStr(DefVal));};
    virtual void SetDefaultVal (TField* Fld)
    {
        if (Fld->IsNull)  FInpVal->Text= "";
        else FInpVal->Text= FloatToStr(Fld->AsFloat);
    };
    double GetResultSum();    
    virtual void SetValToFld (TField* Fld) // Значение элем ввода присвоить полю 
    {
        if (AmsIsNull) Fld->Clear();
        else Fld->AsFloat= GetResultSum();
    };
    virtual void SetValToVar (TOracleQuery* OQ, const AnsiString& VarName)
    {
        OQ->DeclareVariable(VarName, otFloat);
        if (!AmsIsNull)
            OQ->SetVariable(VarName,GetResultSum());
    };
__published:
};
//---------------------------------------------------------------------------
#endif
