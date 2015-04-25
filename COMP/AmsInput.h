//---------------------------------------------------------------------------

#ifndef AmsInputH
#define AmsInputH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "AmsInputHistory.h"
#include <Oracle.hpp>
//---------------------------------------------------------------------------
class TAmsInput;
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TMultiCopy)(TAmsInput* IS, const AnsiString& WhatCopy);
//---------------------------------------------------------------------------
class PACKAGE TAmsInput : public TWinControl
{
protected:
    TLabel* FPrompt;
    TAmsInputHistory* FInputHistory;
    int FPromptLen;

    int FldIndex; // Индекс поля в TFldsDescr; -1 если не используется 

    AnsiString __fastcall GetPrompt(){return FPrompt->Caption;};
    void __fastcall SetPrompt(AnsiString Val){FPrompt->Caption= Val;};

    bool __fastcall GetAmsEnabled(){return Enabled;};
    virtual void __fastcall SetAmsEnabled(bool Val)
    {
        FPrompt->Font->Color=Val?clWindowText:clGrayText;
        Enabled = Val;
    };

    virtual int __fastcall GetPromptLen(){return FPrompt->Width;};
    virtual void __fastcall SetPromptLen(int Val)
    {
        FPrompt->Width=Val;
        FPromptLen = Val;
        if (!ControlState.Contains(csReadingState) &&
            !ControlState.Contains(csCreating))
        {Resize();};
    };

    virtual int __fastcall GetPromptMaxLen(){return FPrompt->Canvas->TextWidth(FPrompt->Caption);};
    void __fastcall SetPromptMaxLen(int Val){};

    virtual void __fastcall Loaded(void);


    virtual int __fastcall GetAmsMaxLen()=0;
    virtual void __fastcall SetAmsMaxLen(int Val)=0;

    TNotifyEvent FOnNextEl;
    TNotifyEvent FOnPredEl;
    TNotifyEvent FOnPressEnter;
    TNotifyEvent FOnChanged;

    TMultiCopy FOnMultiCopy;

    virtual void __fastcall SetIsNull(bool Val)=0;
    virtual bool __fastcall GetIsNull()=0;

    DYNAMIC void __fastcall KeyDown(Word &Key, Classes::TShiftState Shift);
    void __fastcall MultiCopyKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
    DYNAMIC void __fastcall DoEnter(void);

    TWinControl* Pred;
    TWinControl* Post;
    TWinControl* FirstAct;
    void __fastcall AmsChanged(System::TObject* Sender);
public:
    __fastcall TAmsInput(TComponent* Owner);
    //------------------------------------------------------------------------------
    virtual void SetDefaultVal (const AnsiString& DefVal){}; // Заполнение элем ввола значением
    virtual void SetDefaultVal (TDateTime DefVal ){};
    virtual void SetDefaultVal (int DefVal){};      // Заполнение элем ввола значением
    virtual void SetDefaultVal (long double Fld){}; // Заполнение элем ввола значением
    virtual void SetDefaultVal (TField* )=0; // Заполнение элем ввола значением
    //------------------------------------------------------------------------------
    virtual AnsiString GetResultStr()=0;        // Получить результат ввода как строку
    virtual void Clear()=0;                     // Очистить содержимое элем ввода
    virtual void SetValToFld (TField* Fld) = 0; // Значение элем ввода присвоить полю
    virtual void SetValToVar (TOracleQuery* OQ, const AnsiString& VarName ) = 0; // Устанавливаем значение
    //-----------------------------------------------------
    void SetPred (TWinControl* Pred_) {Pred=Pred_;};
    void SetPost (TWinControl* Post_) {Post=Post_;};
    void SetActPred () {if (Pred) Pred->SetFocus();};
    void SetActPost () {if (Post) Post->SetFocus();};

    // Позиционирует курсор за последий символ ввода
    virtual void GoBack();
__published:
    __property int AmsMaxLen ={read=GetAmsMaxLen, write=SetAmsMaxLen};
    __property int AmsFldIndex ={read=FldIndex, write=FldIndex, default =-1};
    __property AnsiString AmsPrompt ={read=GetPrompt, write=SetPrompt};
    __property Align;
    __property bool AmsEnabled={read=GetAmsEnabled, write =SetAmsEnabled};
    __property bool AmsIsNull={read=GetIsNull, write =SetIsNull, default= false};
    __property Visible={default=0};
    //__property Enabled;
    __property ShowHint;
    __property Anchors;
    __property TabStop;
    __property TabOrder;

    __property TNotifyEvent OnAmsPreviosElement = {read=FOnPredEl, write=FOnPredEl};
    __property TNotifyEvent OnAmsNextElement    = {read=FOnNextEl, write=FOnNextEl};
    __property TNotifyEvent OnAmsPressEnter    = {read=FOnPressEnter, write=FOnPressEnter};
    __property TNotifyEvent OnAmsChanged    = {read=FOnChanged, write=FOnChanged};

    __property TMultiCopy   OnAmsMultiCopy      = {read=FOnMultiCopy, write=FOnMultiCopy};

    // Длина подсказки в пикселах
    __property int AmsPromptLen ={read=GetPromptLen, write=SetPromptLen};
    // Сколько подсказка может занимать чтобы влезала полностью
    __property int AmsPromptMaxLen ={read=GetPromptMaxLen, write=SetPromptMaxLen}; 
    __property TAmsInputHistory* AmsInputHistory = {read=FInputHistory, write = FInputHistory};
};
//---------------------------------------------------------------------------
#endif
 