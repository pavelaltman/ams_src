//---------------------------------------------------------------------------

#ifndef FLDS_DESCR_DISTRH
#define FLDS_DESCR_DISTRH
#include "orcpp.h"
#include "prog.h"
//---------------------------------------------------------------------------
// Для дистр лайн
//---------------------------------------------------------------------------
class TFldsDescrAnDestr: public TFldsDescr
{
protected:
    int AnSumOst;
    int AnSumEd;
    std::vector<int> AddAnSums;  // Здесь аналитические суммы для дополнительных полей
    FieldSetM FSM;
    Prog an_prog;
    bool an_prog_comp;
    TGenLocalVars* Vars;
public:
    void SetAnSum( int SumOst, int SumEd)
        {AnSumOst=SumOst;AnSumEd=SumEd;};
    void ReadDescr (const AnsiString& AnName_, const AnsiString& SetName_);
    // Построить SQL запрос для дистр лайн
    void BuildSqlQuery(TOracleDataSet* ODS);
    void BuildSqlQueryRecalc(TOracleDataSet* ODS);

    void Recalc(bool ClrFlds, TOracleDataSet* ODS, TGenLocalVars* Vars_ , int REQuil);

    void RunProg(bool Post, bool SetFlds, bool ClearFlds, bool ClearF4);

    void IndexFields(TOracleDataSet* ODS);
    std::vector<int>& AddSumList(){return AddAnSums;};
};
//---------------------------------------------------------------------------
// Для редактирования позиции БД (отдельновзятой) в ОПЕРАЦИИ применяется
//---------------------------------------------------------------------------
class TFldsDescrDbPos: public TFldsDescr
{
    AnsiString OdsSql;
    int RootCat;
public:
    void ReadDescr (const AnsiString& DbName_, const AnsiString& SetName_);
    int GetRootCat() {return RootCat;};
    // Построить SQL запрос для редактирования позиции БД
    void BuildSqlQuery(TOracleDataSet* ODS);
};
//---------------------------------------------------------------------------
#endif
