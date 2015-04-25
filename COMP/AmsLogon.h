//---------------------------------------------------------------------------

#ifndef AmsLogonH
#define AmsLogonH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "Oracle.hpp"
//---------------------------------------------------------------------------
class PACKAGE TAmsLogon : public TOracleLogon
{
private:
protected:
    AnsiString FBuchName;
public:
    __fastcall TAmsLogon(TComponent* Owner);
	bool __fastcall AmsExecute(void);
__published:
    __property AnsiString AmsBuchName ={read=FBuchName, write=FBuchName};
};
//---------------------------------------------------------------------------
#endif
