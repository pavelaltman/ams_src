//---------------------------------------------------------------------------

#ifndef BRWS_ACC_WINDOWH
#define BRWS_ACC_WINDOWH
//---------------------------------------------------------------------------
// ���������� ��������������� ����� ��� ������������ SQL �������� � ����
// �������� �����
//---------------------------------------------------------------------------
struct SBrwsAccWindowOptions
{
    enum enDK {dkDebet=1, dkCredit=2, dkBoth=3};
    enDK DK;
    bool ByDate;           //
    bool Analitic;         //
    bool SortByModifDate;  //
    bool DetailedCorresp;  // ����������� ��������������� ������
    int  AccsTreeDepth;    // ����� ����� ���������� -1 - ������ ������
                           // (0-1-2) == LEVEL
    bool CheckOps;
    //--------------------------------------------------------------------
    AnsiString FldName;    // ��� ���� �� �������� ������ ����� ���������
    AnsiString AnName;     // ��� ���������
    AnsiString SetName;    // 
    int AnCode;            // ��� ���������
    int DbCode;            // ��� ���� ������ (�� ������� ����� ���������)
    //--------------------------------------------------------------------
    TDate DateBeg;
    TDate DateEnd;
    int AccCode;
    AnsiString BalFld;
    //--------------------------------------------------------------------
    AnsiString AddWhere;
    AnsiString OrderBy;
    //--------------------------------------------------------------------
    bool WithoutVer;
    AnsiString FilterArticle;   // �� ������ �������� ���������
    int FilterVer;              //
    //--------------------------------------------------------------------
    // ��� ��������� �������
    bool PerNotPlain;           // ������ �� ������
    bool SyntOnly;              // ������ ���������� ���� ����� ��������
};
//---------------------------------------------------------------------------
class TBrwsAccWindow
{
public:
    struct SAccs
    {
        bool Debet;
        int Code;
        AnsiString Name;
    };
    TBrwsAccWindow():Setted(0){};
    void BuildSqlQuery( SBrwsAccWindowOptions& CurrOpt,
                        DS_Q& DS, AnsiString& Sql, AnsiString& Simple, bool Deep=false);
    void BuildRemQuery(
            DS_Q& DS, AnsiString& Sql,
            int& DtAcc, int& CtAcc);//  ���������� �-�� ������ �� �/��
protected:
    static AnsiString Str[2][4];
    SBrwsAccWindowOptions PrevOpt;
    bool Setted;            // ���� �� ����������� �����
    vector<SAccs> SelAcc;
};
//---------------------------------------------------------------------------
#endif
