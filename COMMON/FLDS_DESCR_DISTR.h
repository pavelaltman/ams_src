//---------------------------------------------------------------------------

#ifndef FLDS_DESCR_DISTRH
#define FLDS_DESCR_DISTRH
#include "orcpp.h"
#include "prog.h"
//---------------------------------------------------------------------------
// ��� ����� ����
//---------------------------------------------------------------------------
class TFldsDescrAnDestr: public TFldsDescr
{
protected:
    int AnSumOst;
    int AnSumEd;
    std::vector<int> AddAnSums;  // ����� ������������� ����� ��� �������������� �����
    FieldSetM FSM;
    Prog an_prog;
    bool an_prog_comp;
    TGenLocalVars* Vars;
public:
    void SetAnSum( int SumOst, int SumEd)
        {AnSumOst=SumOst;AnSumEd=SumEd;};
    void ReadDescr (const AnsiString& AnName_, const AnsiString& SetName_);
    // ��������� SQL ������ ��� ����� ����
    void BuildSqlQuery(TOracleDataSet* ODS);
    void BuildSqlQueryRecalc(TOracleDataSet* ODS);

    void Recalc(bool ClrFlds, TOracleDataSet* ODS, TGenLocalVars* Vars_ , int REQuil);

    void RunProg(bool Post, bool SetFlds, bool ClearFlds, bool ClearF4);

    void IndexFields(TOracleDataSet* ODS);
    std::vector<int>& AddSumList(){return AddAnSums;};
};
//---------------------------------------------------------------------------
// ��� �������������� ������� �� (��������������) � �������� �����������
//---------------------------------------------------------------------------
class TFldsDescrDbPos: public TFldsDescr
{
    AnsiString OdsSql;
    int RootCat;
public:
    void ReadDescr (const AnsiString& DbName_, const AnsiString& SetName_);
    int GetRootCat() {return RootCat;};
    // ��������� SQL ������ ��� �������������� ������� ��
    void BuildSqlQuery(TOracleDataSet* ODS);
};
//---------------------------------------------------------------------------
#endif
