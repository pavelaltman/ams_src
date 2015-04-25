//---------------------------------------------------------------------------

#ifndef AmsQueryH
#define AmsQueryH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "Oracle.hpp"
//---------------------------------------------------------------------------
class PACKAGE TAmsQuery : public TOracleQuery
{
private:
protected:
    TStringList* FAmsSQL;
    TStrings* __fastcall GetAmsSQL_ ();
    void __fastcall SetAmsSQL_(TStrings* SL);
    AnsiString InitUser;
public:
    __fastcall TAmsQuery(TComponent* Owner);
    void AmsExec();
    void Decl_Set(const AnsiString& VarName, const AnsiString& VarVal);
    void Decl_Set(const AnsiString& VarName, int VarVal);
    void Decl_Set(const AnsiString& VarName, double VarVal);
    void Decl_Set(const AnsiString& VarName, TDateTime VarVal);
    virtual __fastcall ~TAmsQuery();
__published:
    __property TStrings* AmsSQL={read=GetAmsSQL_, write=SetAmsSQL_};
    __property AnsiString AmsInitSchema={read=InitUser, write=InitUser};
};
//---------------------------------------------------------------------------
#endif
