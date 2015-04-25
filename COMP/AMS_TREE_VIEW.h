//---------------------------------------------------------------------------
#include "AMS_HDR.H"
#ifndef AMS_TREE_VIEWH
#define AMS_TREE_VIEWH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "oracle.hpp"
#include <vector>
#include <map>
#include <algorithm>
//---------------------------------------------------------------------------
class TAmsTreeData
{
protected:
    TTreeNode* TN;
    int Code;
public:
    TAmsTreeData (int Code_, TTreeNode* TN_):Code(Code_), TN(TN_){};
    virtual ~TAmsTreeData (){};
    virtual int GetCode() const {return Code;};
    virtual const AnsiString& GetDescr()const {return TN->Text;};
    virtual int GetLevel()const {return TN->Level;};
};

// ����� ��� ���������� ������������������ ���������� ��������
class TAmsSelObjsList
{
    std::vector<int> OL;
public:
    void Clear(){OL.clear();};
    // �������� ���������� ��������
    int operator[](int Index) {return OL[Index];};
    // ����� ������� ���������
    int Count() {return OL.size();};
    // ������� ������� (�������� �� ����������)
    void SelectObj (int OId);
    // ������� ������� �� ������ ��������� ���������     
    void DeSelectObj (int OId);
    // ��������� ������ - ��������� �������� �.� �������
    void GetCommaText (AnsiString& Res);    
};

class PACKAGE TAmsTreeView : public TTreeView
{
private:
protected:
    // ����������� ������ ������������ ������� �����(���������)
    std::vector<TAmsTreeData*> TreeData;
    std::map<int, TTreeNode*> Track;
    // ������ ��������� ���������
    TAmsSelObjsList SelectedCats;
    // ������� OQ �� ���� �������� ���� ������� �������� �� ����
    TOracleQuery* OQ;
    bool FQueryShared;
    // ���� TRUE �� ������������ ���������� �� ����� ������ ������ ��� 
    // ��� ������������ ��������� �� ������� �����/����
    bool FRefreshEveryTime;
    // �������� ����� �� ������� ����� ������
    AnsiString FBaseSchema;
    // ����������� ����� SQL �������
    AnsiString FQueryText;
    // �����-������ ������
    int BeginPoint;

    bool Ready;
    
    void __fastcall FillTheTreeView(bool DeepQuerySet=true);
    void Clear ();
    virtual void PrepareSQL(const AnsiString& BeginPoint_)=0;
    virtual void PrepareSQL(int BeginPoint_)=0;
    virtual AnsiString FormNodeText()=0;
    virtual TAmsTreeData* FormTreeData(TTreeNode*)=0;
    virtual int GetParentCode()=0;
    virtual int GetCurrLevel()=0;
public:
    __fastcall TAmsTreeView(TComponent* Owner);
    __fastcall virtual ~TAmsTreeView (){Clear();};
    
    void RefreshView();
    virtual void Begin(const AnsiString& BaseSchema_, const AnsiString& BeginPoint_);
    virtual void Begin(const AnsiString& BaseSchema_, int BeginPoint_);
    // ������� �������� ��������� � ����� Code
    void MakeActive (int Code);
    // ���������� TreeNode �� ���� ���������
    TTreeNode* GetNodeByCode (int Code){ return Track[Code];};
    // ���������� ��� �������� ���������
    int GetCurrCode () ;
    // ���������� ��� ��������� �� TTreeNode*
    int GetCode (TTreeNode* ) ;
    // ������� �������� ��������� � ��������� � ������ ��������� ���������
    void SelectCurr();
    // ������ �������� ��������� �� ������ ��������� ���������
    void DeSelectCurr();
    // ������ ��������� ���������
    TAmsSelObjsList* GetSelectedCats () {return &SelectedCats;};
    // ������� ������ ����� ����� TN, � ����� � ������ Root
    // �������� ����� Txt, ��� ��� Code
    TTreeNode* NewLeaf (TTreeNode* Root, TTreeNode* TN, const AnsiString& Txt, int Code);
    // ������� TN ����� Up = True ��� ���� Up=False, 
    // ��������� ��������� �� TreeNode c ������� ���������� �������
    // NULL �� � ��� �� ����������
    TTreeNode* MoveNode (TTreeNode* TN, bool Up);
    // ������� Node �� ���� ����������
    void DelNodeByCode( int Code);
    
__published:
    __property bool AmsOraQueryShared = {read =FQueryShared, 
                                        write=FQueryShared, default =true};
    __property TOracleQuery* AmsOraQuery = {read= OQ,  write=OQ};
    __property bool AmsRefreshEveryTime = {read =FRefreshEveryTime, 
                                        write=FRefreshEveryTime, default =false};
    __property AnsiString AmsBaseSchema={read=FBaseSchema, write=FBaseSchema};
};
//---------------------------------------------------------------------------
#endif
