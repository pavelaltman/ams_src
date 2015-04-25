//---------------------------------------------------------------------------

#ifndef REFRESH_ACCSH
#define REFRESH_ACCSH
#include "syncobjs.hpp"

//---------------------------------------------------------------------------
class TFrmOperListRoot;
//---------------------------------------------------------------------------
class TAccsToWin
{
protected:
    typedef std::list<TFrmOperListRoot*>::iterator IT;
    std::list<TFrmOperListRoot*> WindowsList;
    TCriticalSection* AccsToWinCS;
public:
    TAccsToWin();
    void Refresh(bool otlozh=false);
    void Add(TFrmOperListRoot* Frm);
    void Del(TFrmOperListRoot* Frm);
};
//---------------------------------------------------------------------------
class TRefreshAccs
{
protected:
    map<int, TAccsToWin> Aw;
public:
    void Refresh(std::vector<int>& AccsList);
    void Add(TFrmOperListRoot* Frm);
    void Del(TFrmOperListRoot* Frm);
    void OpChange (int OpId);
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Класс решает проблему закрытия MDI окна
template<class PF, class CF> class TParentAndChild
{
    std::map<PF*, CF*> ParentToChild;
    std::map<CF*, PF*> ChildToParent;
public:
    bool IsHaveChild(PF* ParentF, CF* ChildF)
    {
        CF* Ch= ParentToChild[ParentF];
        return (Ch==ChildF);
    };
    void ChildClose(CF* ChildF)
    {   PF* ParentF= ChildToParent[ChildF];
        ChildToParent.erase(ChildF);
        ParentToChild.erase(ParentF);
    };
    void SetChild (PF* ParentF, CF* ChildF)
    {
        ParentToChild[ParentF]=ChildF;
        ChildToParent[ChildF]=ParentF;
    };
};
//---------------------------------------------------------------------------
extern TAccsToWin AccsToWin;
//---------------------------------------------------------------------------
class TFrmBrowsAn;
class TFrmBrwsAccWindow;
extern TParentAndChild<TFrmBrowsAn,TFrmBrwsAccWindow> PC_FilterPos;
//---------------------------------------------------------------------------
#endif
