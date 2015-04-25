//---------------------------------------------------------------------------

#ifndef AmsInputStrMLH
#define AmsInputStrMLH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "AmsInput.h"
//---------------------------------------------------------------------------
class PACKAGE TAmsInputStrML : public TAmsInput
{
protected:
    TMemo * FMem;
    int FLinesCount;
    int FHeight;
    
    int __fastcall GetLineCount(){return FLinesCount;};
    void __fastcall SetLineCount(int Val);
    DYNAMIC void __fastcall Resize(void);

    virtual int __fastcall GetPromptMaxLen(){return 0;};

    virtual void __fastcall SetIsNull(bool Val)
    {if (Val) FMem->Lines->Text="";};
    virtual bool __fastcall GetIsNull()
    {return FMem->Lines->Text != "";};

    virtual void __fastcall SetAmsEnabled(bool Val)
    {
        TAmsInput::SetAmsEnabled(Val);
        FMem->Enabled = Val;
    };        
public:
    __fastcall TAmsInputStrML(TComponent* Owner);
    virtual int __fastcall GetAmsMaxLen(){return FMem->MaxLength;};
    virtual void __fastcall SetAmsMaxLen(int Val){FMem->MaxLength=Val;};
    virtual void SetDefaultVal (const AnsiString& DefVal)
    {FMem->Lines->Text=DefVal;}; // Заполнение элем ввола значением
    virtual void SetDefaultVal (TField* Fld)
    {FMem->Lines->Text=Fld->AsString;}; // Заполнение элем ввола значением
    virtual AnsiString GetResultStr()
    {return FMem->Lines->Text;};        // Получить результат ввода как строку
    virtual void Clear()
    {FMem->Lines->Clear();};            // Очистить содержимое элем ввода
    virtual void SetValToFld (TField* Fld)
    {Fld->AsString =FMem->Lines->Text;};// Значение элем ввода присвоить полю
    virtual void SetValToVar (TOracleQuery* OQ, const AnsiString& VarName )
    {
        OQ->DeclareVariable(VarName, otString);
        OQ->SetVariable(VarName, GetResultStr());
    }; // Устанавливаем значение

    
__published:
    __property int AmsLineCount ={read=GetLineCount, write=SetLineCount};
    
};
//---------------------------------------------------------------------------
#endif
