//---------------------------------------------------------------------------
#ifndef FLDS_DESCRH
#define FLDS_DESCRH
//---------------------------------------------------------------------------
// ��� ����������� ��������� ( �������� ����� �� � ��������)
struct SFldOptAdd
{
    SFldOptAdd():ParentFldName(), AnSumIndx(-1), SubAccCode(-1), TurnType(){};
    AnsiString ParentFldName;
    int AnSumIndx;            // ������ ������������� ����� ����������� � FrmBrowsAn
    int SubAccCode;
    AnsiString TurnType;
};
struct SFldOpt
{
    int DispOpt;     // ���������������� ���������
    int DefOpt;      // �����������������
    int Type;        // ��� ����
    int DBId;        // ��� ����� ���� ������ �� �� ��� ���������
    int FldMaxWidth; // ������������ ���������� �������
    int MaxPoints;   // ���������� ���������� ������
    int DiplWidth;   // ������ ������������
    int FldNo;       // ����� ���� � �������������� SQL �������
    int Lines;       // ���������� ����� ��� ����������� ��� ��������� ����
    AnsiString RealName;    // �������� ���� � �������
    AnsiString Alias;       // ��������� ������������
    AnsiString Hint;        // ����������� ��������� (�OMMENT ����)
    AnsiString FullName;    // �������� ������� � SQL �������
    AnsiString SqlAlias;    // Alias ���� � SQL �������
    AnsiString SetName;     // ����� ��������� ��� ������ ����-������ �� ��
    int FldAddInfo;         // ����� ���� - ��, ��, ���, ��������������
    SFldOptAdd AddOpt;      // ��� ����� - ��� ��� �����
};
//---------------------------------------------------------------------------
struct SRefs
{
    int MainFld;
    int VerFld;
    AnsiString SetName;
};

// ������ ��� ����� � ���������������
class Node;
class Struct;
class AmsQuery;

//---------------------------------------------------------------------------
// �������� �������������� �����
struct SAddAnSumDescr
{
    AnsiString TurnType;
    int SubAccCode;
    int operator == (const SAddAnSumDescr& X )
    { return SubAccCode==X.SubAccCode && TurnType==X.TurnType;};
};
//---------------------------------------------------------------------------
// ������ �����
//---------------------------------------------------------------------------
class TFldsDescr
{
protected:
    std::vector<SFldOpt> FOpt;
    std::map<int, SRefs> FRefs; // ���� ����� ���� (����������), ���������, ��� ����� ��
    int DbId;
    AnsiString DbName ;
    int AnId;
    AnsiString AnName;
    AnsiString SetName;

    bool NullVerChecked;
    bool NullVerRule;


    void ReadDbDescr();
    void ReadAnDescr();

    void Add(const SFldOpt& NewElement){FOpt.push_back(NewElement);};
    void Clear() {FOpt.clear();};
    void RebuildDbRef(); // ������������� ��� ������ �� �� � �� ���������

    void PrepareOQ (AmsQuery& AQ_, bool An=false);
    void FillFO(AmsQuery& AQ_, SFldOpt& FO, const AnsiString& AddFullName="D");

    std::vector<SAddAnSumDescr> AddFldsDescr;
public:
    TFldsDescr():NullVerChecked(false){};
    void SetDbDescr(const AnsiString& DbName_, int DbId_, const AnsiString& SetName_)
    {DbName=DbName_; DbId=DbId_; SetName=SetName_;AnName="";AnId=-1;};
    void SetAnDescr(const AnsiString& DbName_, int DbId_,
                    const AnsiString& AnName_, int AnId_,
                    const AnsiString& SetName_)
    {DbName=DbName_; AnName=AnName_; DbId=DbId_; AnId=AnId_;SetName=SetName_;};

    void ReadFldsDescr();

    const AnsiString& GetDbName(){return DbName;};
    const AnsiString& GetSetName() {return SetName;};
    const AnsiString& GetAnName(){return AnName;};

    SFldOpt& operator [](int Ind) {return FOpt[Ind];};
    SFldOpt& At(int Ind) {return FOpt[Ind];};
    int Count () {return FOpt.size();};
    //---------------------------------------------------
    SRefs& GetRefs(int FldInd)
    {return FRefs[FldInd];};

    // ��������� ������������� ����� ������� ������
    void GetCommaText (AnsiString& Res);

    ~TFldsDescr(){Clear();};
    //---------------------------------------------------
    // ���������� true ���� ���� ������� ������� ������
    bool IsNullRule();

    AnsiString GetAsString (AmsQuery& AQ, int FldIndx);

    std::vector<SAddAnSumDescr>& GetAddFldsDescr()
    { return AddFldsDescr;};
    const std::vector<SAddAnSumDescr>& GetAddFldsDescr() const
    { return AddFldsDescr;};
    void FormAddAnSums( std::vector<int>*  AddAnSums_);
    void BuildAddSums();
};
#endif
