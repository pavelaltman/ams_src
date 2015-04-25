//---------------------------------------------------------------------------
#ifndef AMS_UTILSH
#define AMS_UTILSH
#include <list>
#include <vector>
#include "AMS_EXC.h"
#include "OracleData.hpp"
//---------------------------------------------------------------------------
extern template <class T>
void OutSQL (T* OQ, const AnsiString& Prompt, const char* FileName=NULL);
//---------------------------------------------------------------------------
extern bool AmsSqlDebug;
extern bool AmsViewDebug;
namespace Oracle {
class TOracleQuery;
};
// ��� ������������� � ������������
template <class T> class ClearDel
{
public:
    void operator ()(T& x)
    {delete x;};
};
//---------------------------------------------------------------------------
class TFrmCatTree;
class TFrmAccsStr;
//---------------------------------------------------------------------------

typedef void __fastcall (__closure *TCatNewEvent)(TFrmCatTree* Sender, int NewCat);
//---------------------------------------------------------------------------

extern AnsiString BaseUserName;

// ����� ��� ��������������� ������ �����
class TAccName
{
    AnsiString SysAccName;
    
    bool CodeOk;
    int Code;
    int GetAccCodeInt (Oracle::TOracleQuery* OQ);
    AnsiString Descr;
public:
    TAccName():CodeOk(true),Code(-1){};
    TAccName(int Code_, const AnsiString& SysAccName_, const AnsiString& Descr_):
      CodeOk(true){Code = Code_; SysAccName = SysAccName_;Descr=Descr_;};
    // ���� ���� ����� "�����_��� ��������", �� ��� ���� �����������
    TAccName(int Code_, const AnsiString& UNameDescr);
    // ��� �������������� ������ ���� "�����_��� ��������" � ��������� ��� ����� 
    static AnsiString ToSysNameDescr(const AnsiString& UserNameDescr); 
    static AnsiString ToSysName(const AnsiString& UserName); 
    static AnsiString ToUserName(const AnsiString& SysName); 
    void SetSysAccName (const AnsiString& SAccName){CodeOk= false;SysAccName = SAccName;};
    void SetUserAccName(const AnsiString& UAccName){SetSysAccName(ToSysName(UAccName));};
    void SetByCode( int Code_, Oracle::TOracleQuery* OQ);
    const AnsiString& GetSysName () const { return SysAccName;};
    AnsiString GetUserName() const; 

    const AnsiString& GetDescr () const {return Descr;};
    // ���������� ��� �����
    int GetAccCode(Oracle::TOracleQuery* OQ) 
     {if (CodeOk) return Code; else return GetAccCodeInt(OQ);};
};
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TAccNewEvent)(TFrmAccsStr* Sender, 
                                                  const TAccName& NewAccs);
//---------------------------------------------------------------------------
// ��������� ��������� �� ������ � ������������ ��� ������ ��������
template <class T>
class TPStorage
{
typedef std::vector<T*>::iterator TPStorageIt;
protected:
    // ��� �������� �� ��������
    mutable TPStorageIt CurrPos;
    // ���� ture �� ���������� ������� �� �� ��� ���������
    bool MemOwner;

    std::vector<T*> List;
public:

    int Count () { return List.size();};
    void SwapPos ( int Pos1, int Pos2)
    {T* Tmp = List[Pos1]; List[Pos1]=List[Pos2];List[Pos2]=Tmp;};
    
    TPStorage (bool MemOwner_=true):MemOwner(MemOwner_){};

    T* operator[](int i){ return List[i];};
    int GetIndex(T* El)
    {
        unsigned int i = 0 ;
        while (i<List.size() && List[i]!=El) i++;
        if (i>=List.size())
            THROW_AMS_EXC("TPStorage: �� ����� ������ ��������");
        return i;
    };        
    
    void Insert (T* InsVal){List.push_back(InsVal);};
    // ��� �������� �� �������� --------------------------
    void Begin() {CurrPos =List.begin();};
    int Eof() {return CurrPos == List.end();};        
    T* NextVal(){T* Tmp= *CurrPos; CurrPos++; return Tmp;};  
    // ������� �������, ������� ������ ��������� ����� NextVal
    void DeletePred()
    { 
        TPStorageIt I = CurrPos; 
        I--;
        if (MemOwner) delete *I;
        List.erase(I);
    }
    //������� ������� �� ������� ��������� CurrPos
    void DeleteCurrPos() {CurrPos++;DeletePred();};
    //----------------------------------------------------
    // �������� ������� �� �-��� ���� Curr pos
    void ReplacePredPos(T* NewEl)
    {
        TPStorageIt I = CurrPos;
        I--;
        if (MemOwner) delete *I;
        *I=NewEl;
    };
    //----------------------------------------------------
#pragma option push -w-inl
    //----------------------------------------------------
    void Sort (int (*Less) (const T*&, const T*& ))
    {
        if (List.size()<2) return;
        unsigned int i = 1 ;
        bool Changed = true;
        while (Changed)
        {
            Changed=false;
            i=1;
            while (i<List.size())
            {
                if (Less(List[i],List[i-1]))
                {
                    T* Tmp= List[i]; List[i]=List[i-1]; List[i-1]=Tmp;    
                    Changed= true;
                };
                i=i+1;
            };        
        }
    };

    
    //----------------------------------------------------
    TPStorage (const TPStorage& Stor):MemOwner(false)
    {
        Stor.Begin();
        while (!Stor.Eof())
        {Insert(Stor.NextVal());}; 
    };
    //----------------------------------------------------
    void Clear()
    {
        if (MemOwner)
        {
            Begin();
            while (!Eof()) delete NextVal();
        };
        List.clear();
    };
    //----------------------------------------------------
    operator = (const TPStorage& Stor)
    {
        Clear();
        Stor.begin();
        while(!Stor.Eof()) Insert(Stor.NextVal());
    };
    //----------------------------------------------------
    void DeleteElement(T* El)
    {
        Begin();
        while (!Eof() && *CurrPos != El) CurrPos++;
        if (!Eof()) DeleteCurrPos();
    };
    //----------------------------------------------------
    void MoveFirst(T* El)
    {
        Begin();
        while (!Eof() && *CurrPos != El) CurrPos++;
        if (!Eof()) DeleteCurrPos();
    };
    //----------------------------------------------------
#pragma option pop
    ~TPStorage (){Clear();};            
};
//---------------------------------------------------------------------------
// ���������� ID ������� �� ��� ����� �� ���� OBJS 
extern int GetObjId (const AnsiString& Schema, 
                  const AnsiString& ObjName, 
                  Oracle::TOracleQuery* OQ);            
// �������������� ������ ���� ���� ��� ���� ������� OBJS
// ���� ������ �������� NotEq, �� ����� ������ ��� ����� �� �����������
// �����������
extern int PrepareListAmsBuch( Oracle::TOracleQuery* OQ, TStrings* Str, const AnsiString& NotEq="");

// ��������� ������, ���� ��������� ������ �� ������ �� ����� ����� � ����� 
// ����� � ���� ERR.SQL ������ ����� SQL �������
extern int ExecSQL(Oracle::TOracleQuery* OQ, const AnsiString& Prompt);
// ������� Exception ������
extern int ExecSQLNoExc(Oracle::TOracleQuery* OQ);

extern int DescribeSQL(Oracle::TOracleQuery* OQ, const AnsiString& Prompt);
extern int ExecODS(TOracleDataSet* ODS);
// ��������� ComboBox ���������� �� �������, ������ ���������� ��� ��������
extern void FillCB (TStrings * StrList, TOracleQuery* OQ, int FldNo=0);
// ������ ����������� ������ � �� �������� ���������� StringList
extern void FillStr (TStrings * StrList, TOracleQuery* OQ, int FldNo=0);

// ��������� ���� �� ������� ��������� : "create or replace function" 
//    ���������� 0 � ������ ���������� ����������
//    ������ ������ � Log)
extern int CompileProc(
    Oracle::TOracleQuery* OQ,
    const AnsiString& ProcName,
    const AnsiString& ProcBody, TStrings* Log);

// ���������� ������ ���������� ������������� ������� ������
extern void GetCurrSessionId(Oracle::TOracleQuery* OQ, AnsiString& ResStr);

// ������ �������� � ������ ������� �������� ���� ���������
extern void ShortMessage(const AnsiString& Msg);

extern bool HaveRole(const AnsiString& RoleName, TOracleQuery* OQ);
extern void SetAction(TAction* Act);
extern void ClrAction(TAction* Act);

#endif
