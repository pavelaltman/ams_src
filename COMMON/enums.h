#ifndef AMS_ENUMS_H
#define AMS_ENUMS_H

enum AmsOType
{otDoc=1, otRepForm, otStdOper, otDB, otAnal, otConst, otViewFrm};
//------------------------------------------------------------------
enum AmsObjOpt
{DBO_NOT_FIRST_SHOW=1, DBO_ZEROVER=2};
//------------------------------------------------------------------
enum AmsAccOpts
{aoNegRemWarring = 0x02, aoZeroVer=0x10};
//------------------------------------------------------------------
enum AmsAccsOpt
{AF_ZEROVER=16};
//------------------------------------------------------------------
enum EnAmsFldType
{
    aftDate = 1, aftSumma = 2, aftInt = 3, aftStr =5, aftBool=6, aftPoint=7
};
//------------------------------------------------------------------
enum EnAmsCmpSign
{
    acsEqual = 0, acsMore, acsLess, acsAlmost
};
//------------------------------------------------------------------
enum EnFldOpt
{
        Addable         =0x1,        // 1
        Totalable       =0x2,        // 2
        CommonVer       =0x4,        // 4
        DebetCredit     =0x8,        // 8
        Hidden          =0x10,       // 16
        MultiLine       =0x20,       // 32
        SystemFld       =0x100,      // 256
        Frozen          =0x200,      // 512
        Editable        =0x400,      // 1024
        AlwaysFree      =0x800,      // 2048
        EditRecalc      =0x1000,     // 4096
        ZestFld         =0x8000,     // 32768
        RemFld          =0x10000,    // 65536
        ShowFld         =0x20000,    // 131072
        DistrFld        =0x40000,    // 262144
        NoUpdateImp     =0x80000,    // 524288
        NoImp           =0x100000,   // 1048576
        ZestFldInp      =0x200000,   // 2097152

};
//------------------------------------------------------------------
enum EnDbPosEditOpt
{
    eoNewPos        = 0x01,     // ����� �������
    eoDubbing       = 0x02,     // ��������� �������
    eoHideVersion   = 0x04,     // ����������� �� ����������� ��������
    eoBusyPos       = 0x08,     // ����������� ������� �������
    eoReadOnly      = 0x10,
    eoShtrixCode    = 0x20,     // ���� �������������� ����� ��� �������
    eoDbFlds        = 0x100,    // ���������� ���� ��
    eoAnFlds        = 0x200     // ���������� ���� ���������
};
//------------------------------------------------------------------
enum EnProcessFldOpt
{
    pfSelected     = 0x1,       // ��������� ������� ��
    pfAllCat       = 0x2,       // �� ���� ���������
    pfFilteredPos  = 0x4,       // �� ���� ��������, ��������� � QBE
    pfNullsOnly    = 0x80       // ������ ������� ��������
};
//------------------------------------------------------------------
// ����� ���� � �������� ����� ��� ������ � ������ ������
enum EnFldAddInfo
{
    aiDb = 0x01,       // ���� ���� ������
    aiAn = 0x02,       // ���� ���������
    aiAnR= 0x04,       // ���� ������� � ���������
    aiAdd= 0x08        // �������������� ����
};
//---------------------------------------------------------------------------
// ����� �������������� ������������� ����� -> ��� ����, �������, ��������� �������
enum EnAnSumModes {asmSelPos, asmAllBase, asmRems};
//---------------------------------------------------------------------------
#endif
