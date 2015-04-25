//---------------------------------------------------------------------------

#ifndef AmsInputAnalH
#define AmsInputAnalH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "AmsInput.h"
#include "AmsInputDbPos.h"
#include "AmsInputStr.h"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class PACKAGE TAmsInputAnal : public TAmsInputDbPos
{
public:
typedef void __fastcall (__closure *TAmsGetAnalInfoEvent)(TAmsInputAnal *Sender);
private:
protected:
    void ViewChanged ();
    //--------------------------------------
    int CurrSumId;
    int  __fastcall GetSumId ()
        {return FInpVal->Text.ToIntDef(0);};
    void __fastcall SetSumId (int SId)
        {CurrSumId=SId;FInpVal->Text=IntToStr(SId);ViewChanged();};
    //--------------------------------------
    int CurrAnalId;
    int  __fastcall GetAnalId(){return CurrAnalId;};
    void __fastcall SetAnalId(int AnId){CurrAnalId=AnId;ViewChanged();};
    //--------------------------------------
    AnsiString  __fastcall GetAnalInfo(){return FAnalVal->Text;};
    void __fastcall SetAnalInfo(AnsiString AnId){FAnalVal->Text=AnId;};
    //--------------------------------------
    TAmsGetAnalInfoEvent GetAnalInfo_;
    TEdit* FAnalVal;
    DYNAMIC void __fastcall Resize(void);

public:
    __fastcall TAmsInputAnal(TComponent* Owner);
    virtual void SetDefaultVal (TField* Fld){SetSumId(Fld->AsInteger);};
    virtual void SetDefaultVal (const AnsiString& DefVal)
        {SetDefaultVal(DefVal.ToIntDef(0));};
    virtual void SetDefaultVal (int DefVal)
        {SetSumId(DefVal);};

__published:
    __property int AmsSumId ={read=GetSumId, write=SetSumId};
    __property int AmsAnalId ={read=GetAnalId, write=SetAnalId};
    __property AnsiString AmsAnalInfo ={read=GetAnalInfo, write=SetAnalInfo};
    __property TAmsGetAnalInfoEvent OnAmsGetAnalInfo ={read=GetAnalInfo_, write=GetAnalInfo_};

};
//---------------------------------------------------------------------------
#endif
