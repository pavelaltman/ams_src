//---------------------------------------------------------------------------

#ifndef AmsDataSetH
#define AmsDataSetH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "OracleData.hpp"
#include <Db.hpp>
//---------------------------------------------------------------------------
class PACKAGE TAmsDataSet : public TOracleDataSet
{
private:
protected:
    TStringList* FAmsSQL;
    TStrings* __fastcall GetAmsSQL_ ();
    void __fastcall SetAmsSQL_(TStrings* SL);
    bool __fastcall GetAmsActive_ ();
    void __fastcall SetAmsActive_(bool Val);
    AnsiString InitUser;
public:
    __fastcall TAmsDataSet(TComponent* Owner);
    virtual __fastcall ~TAmsDataSet();
__published:
    __property bool AmsAcitve ={read=GetAmsActive_, write=SetAmsActive_};
    __property TStrings* AmsSQL={read=GetAmsSQL_, write=SetAmsSQL_};
    __property AnsiString AmsInitSchema={read=InitUser, write=InitUser};

};
//---------------------------------------------------------------------------
#endif
