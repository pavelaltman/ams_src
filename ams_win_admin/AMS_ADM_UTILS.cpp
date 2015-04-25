//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "AMS_ADM_UTILS.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// ������ �� ��������� �� ������� OBJS Oracle-���
AnsiString GetOraType (int AmsOType, int AmsMaxLen)
{
    AnsiString S="";
    switch (AmsOType)
    {
        case aftDate:
            S="DATE";
        break;
        case aftSumma:
            S="NUMBER(*,6)";
        break;
        case aftInt:
            S="NUMBER(8)";
        break;
        case aftStr:
            S="VARCHAR2("+IntToStr(AmsMaxLen)+")";
        break;
        case aftBool:
            S="NUMBER(1)";
        break;
    }
    return S;
}
//---------------------------------------------------------------------------
