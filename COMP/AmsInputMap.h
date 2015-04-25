#ifndef AmsInputMapH
#define AmsInputMapH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "AmsInputStr.h"
//---------------------------------------------------------------------------
class PACKAGE TAmsInputMap : public TAmsInputStr
{
private:
protected:
    void __fastcall ChangeEdit(System::TObject* Sender);
    void __fastcall HistButPressed(System::TObject* Sender);
    TNotifyEvent FAmsButPressed;

    TButton* HBut;
    void __fastcall ChangeActControl(System::TObject* Sender);

    int FAmsDbId;

    AnsiString __fastcall GetArticle () { return FInpVal->Text;};
    void __fastcall SetArticle (const AnsiString& Art) {FInpVal->Text= Art;};
    virtual void __fastcall SetAmsEnabled(bool Val);

public:
    __fastcall TAmsInputMap(TComponent* Owner);
    virtual void SetDefaultVal (TField* Fld){FInpVal->Text = Fld->AsString;};
    virtual void SetDefaultVal (const AnsiString& DefVal)
        {TAmsInputStr::SetDefaultVal(DefVal);};    
__published:
    __property TNotifyEvent OnAmsButPressed = {read=FAmsButPressed, write= FAmsButPressed}; 
    __property AnsiString AmsArticle ={read=GetArticle, write=SetArticle};
    __property int AmsDbId ={read=FAmsDbId, write=FAmsDbId, default = -1};        
};
//---------------------------------------------------------------------------
#endif
 