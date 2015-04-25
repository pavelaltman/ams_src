//---------------------------------------------------------------------------

#ifndef FOOTER_CNTRLH
#define FOOTER_CNTRLH
#include "AmsFooter.h"

//---------------------------------------------------------------------------
// ������ ��� �������� � ������� �������� ��
enum EnShowFooter {sfUsual, sfAlways, sfNotShow};

class TFooterInfo
{
public:
    struct SFooterInfo
    {
        bool ShowLine;
        FieldSetAnTotal* WhatShow;
        AnsiString TotalName;
        EnShowFooter ShowFooter;
    };
protected:
    std::vector<SFooterInfo> FI;
public:
    int Add(FieldSetAnTotal* WhatShow, bool Show= true, AnsiString Prompt="");
    void Hide (int I);
    void Show (int I);
    SFooterInfo& At(int i) { return FI[i];}; 
    int Count() {return FI.size();};
    FieldSetAnTotal& operator [] (int i) { return *(FI[i].WhatShow);};
    bool Visible (int i)const { return FI[i].ShowLine;};
};
//---------------------------------------------------------------------------
// ��������� �����������
class TFooterCntrl
{
    struct STotalMapping
    {
        int FldsDescrIndx;
        int ColIndx;
    };
    TAmsFooter* Footer;
    TFldsDescr* FldsDescr;
    AnsiString Sql;
    bool Builded;
    bool NeedDistr;
    std::vector<STotalMapping> ColsMap;
public:
    TFooterInfo FI;
    void Tie (TAmsDBGrid* , std::vector<int>& ColIndx);
    void SetFooter(TAmsFooter* AF_, TFldsDescr* FD_) { Footer= AF_;FldsDescr= FD_;};
    void Draw ();
    void SetDistr(bool NeedDistr_){NeedDistr=NeedDistr_;};
    const AnsiString& GetSql () const {return Sql;};
    bool HaveFooter() const { return (Sql!="" && ColsMap.size());};
    // �������� ������� ������ �������� � ������� �������
    void AddLine (TOracleDataSet* ODS_, bool OldVal = false);
    // ������ ������� ������ �������� �� ������� �������
    void SubLine (TOracleDataSet* ODS_, bool OldVal = false);
};
//---------------------------------------------------------------------------
#endif
