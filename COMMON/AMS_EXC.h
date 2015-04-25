//---------------------------------------------------------------------------

#ifndef AMS_EXCH
#define AMS_EXCH
#include "sysutils.hpp"
//---------------------------------------------------------------------------
#define THROW_AMS_EXC(Msg) throw (TAmsException (Msg, __FILE__, __LINE__))  
#define AMS_EXC(Msg,it) throw (TAmsException (Msg, it))
//---------------------------------------------------------------------------
class TAmsException :public Exception {
public:
    int IconType ;
    TAmsException(const AnsiString& Msg, const AnsiString& FileName, int Line);
    TAmsException(const AnsiString& Msg, int it)
                  :Exception(Msg),IconType(it) {}
};


#endif
