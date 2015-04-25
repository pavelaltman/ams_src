//---------------------------------------------------------------------------

#ifndef AmsAccViewH
#define AmsAccViewH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "oracle.hpp"
//---------------------------------------------------------------------------
class PACKAGE TAmsTreeView : public TTreeView
{
private:
protected:
    TOracleQuery* OQ;
    bool FRefreshEveryTime;
    AnsiString FBaseSchema;
public:
    __fastcall TAmsTreeView(TComponent* Owner);
    
    
__published:
    __property TOracleQuery* OraQuery = {read= OQ,  write=OQ};
    __property bool RefreshEveryTime = {read =FRefreshEveryTime, write=FRefreshEveryTime, default =false};
    __property AnsiString BaseSchema={read=FBaseSchema, write=FBaseSche
    
};
//---------------------------------------------------------------------------
#endif
