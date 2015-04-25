//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "AMS_EXC.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TAmsException::TAmsException(const AnsiString& Msg,
                             const AnsiString& FileName,
                             int Line):Exception(Msg+"\r\n["+FileName+", L:"+
                                        IntToStr(Line)+"] ")
{
};


