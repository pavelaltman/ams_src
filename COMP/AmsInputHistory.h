//---------------------------------------------------------------------------

#ifndef AmsInputHistoryH
#define AmsInputHistoryH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFrmInputHistory;
//---------------------------------------------------------------------------
class PACKAGE TAmsInputHistory : public TComponent
{
private:
    int FHistLen;
protected:
    int  __fastcall GetHistLen ()      {return FHistLen;};
    void __fastcall SetHistLen (int HL);
    TFrmInputHistory* Frm;
public:
    __fastcall TAmsInputHistory(TComponent* Owner);
    // Вставить новую запись в историю ввода
    void InsHistVal (const AnsiString& Val);
    // Получить запись путем выбора пользователем
    AnsiString GetHistVal (TWinControl* WhoAsk= NULL);
__published:
    // длина списка истории ввода 
    __property int AmsHistLen = {read=GetHistLen, write=SetHistLen};
};
//---------------------------------------------------------------------------
#endif
 