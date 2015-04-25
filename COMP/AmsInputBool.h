//---------------------------------------------------------------------------

#ifndef AmsInputBoolH
#define AmsInputBoolH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "AmsInput.h"
//---------------------------------------------------------------------------
class PACKAGE TAmsInputBool : public TAmsInput
{
protected:
    TCheckBox* FChB;
    virtual void __fastcall Loaded(void);
    DYNAMIC void __fastcall Resize(void);

    bool __fastcall GetAmsChecked(){return FChB->Checked;};
    void __fastcall SetAmsChecked(bool Val){FChB->Checked=Val;};

    virtual int __fastcall GetAmsMaxLen(){return 1;};
    virtual void __fastcall SetAmsMaxLen(int Val){};

    virtual void __fastcall SetIsNull(bool Val)
    {
        if (Val) FChB->State=cbGrayed;
        else FChB->State= cbUnchecked;
    };
    virtual bool __fastcall GetIsNull()
    {
        return FChB->State==cbGrayed;
    };

public:
    __fastcall TAmsInputBool(TComponent* Owner);
    virtual void SetDefaultVal (TField* Fld)
    {
        if (Fld->IsNull) FChB->State=cbGrayed;
        else FChB->Checked = Fld->AsInteger==1;
    };
    virtual AnsiString GetResultStr()
    {
        if (FChB->State== cbGrayed) return "";
        else return IntToStr(FChB->State);
    };
    virtual void Clear(){SetIsNull(true);};
    virtual void SetValToFld (TField* Fld) // Значение элем ввода присвоить полю
    {
        if (GetIsNull())Fld->Clear();
        else Fld->AsInteger= FChB->State;
    };
    virtual void SetDefaultVal (const AnsiString& DefVal)
    {
        if (DefVal=="") SetIsNull(true);
        else FChB->State=(TCheckBoxState)(DefVal=="1");
    };

    virtual void SetDefaultVal (int DefVal){SetDefaultVal(IntToStr(DefVal));};
    
    virtual void SetValToVar (TOracleQuery* OQ, const AnsiString& VarName)
    {
        OQ->DeclareVariable(VarName, otInteger);
        if (!GetIsNull())
            OQ->SetVariable(VarName,FChB->State);
    };
__published:
    __property bool AmsChecked ={read=GetAmsChecked, write=SetAmsChecked};

};
//---------------------------------------------------------------------------
#endif
