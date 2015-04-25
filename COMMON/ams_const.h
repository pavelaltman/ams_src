//---------------------------------------------------------------------------

#ifndef AMS_CONSTH
#define AMS_CONSTH
//---------------------------------------------------------------------------
// ��������� � AMS �����������

#define CAMS_BadObjId           -1
// ���� ��������
#define CAMS_ObjType_Doc        1 
#define CAMS_ObjType_Frm        2
#define CAMS_ObjType_StdOper    3
#define CAMS_ObjType_DB         4
#define CAMS_ObjType_Anal       5

// ����� �������������� ���� ��������� � ������ � ������� � �����
namespace ParamsStr
{
    const int MaxVals=12;
    const AnsiString Vals[MaxVals]={
    "0- ������",
    "1- �����",
    "2- �����",
    "3- ����",
    "4 - ���������",
    "5 - ������ ���������",
    "6- �������",
    "7- ������ ���������",
    "8- ������ �����",
    "9- ������ � �������������",
    "10- ������� ���� ������",
    "11- ������ ������� ���� ������" };

};

void FillStringsByParamsStr (TStrings* S)
{
    for (int i =0; i<ParamsStr::MaxVals; i++)
        S->Add(ParamsStr::Vals[i]);
};





#endif
