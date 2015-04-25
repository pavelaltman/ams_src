//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "AmsObjCatView.h"
#include "AMS_CONST.h"
#pragma link "AMS_TREE_VIEW"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAmsObjCatView *)
{
    new TAmsObjCatView(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAmsObjCatView::TAmsObjCatView(TComponent* Owner)
    : TAmsTreeView(Owner)
{
}
//---------------------------------------------------------------------------
namespace Amsobjcatview
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TAmsObjCatView)};
         RegisterComponents("Samples", classes, 0);
    }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TAmsObjCatView::PrepareSQL(const AnsiString& BeginPoint_)
{
    if (BeginPoint_ =="")
    {

        FQueryText="select LEVEL, OWNCAT, DESCR, nvl(CATID, -1) PARENT from "+FBaseSchema+".CATTREE "
                   "start with CATID is NULL "
                   "connect by PRIOR OWNCAT = CATID order by TORD ";
        OQ->SQL->Text = FQueryText;
        OQ->DeleteVariables();
        BeginPoint = CAMS_BadObjId;
    }
    else
    {
        OQ->SQL->Text = " select OWNCAT from "+FBaseSchema+".OBJCAT where DESCR= :BEGIN_POINT";
        OQ->DeleteVariables();
        OQ->DeclareVariable("BEGIN_POINT", otString);
        OQ->SetVariable("BEGIN_POINT", BeginPoint_);
        if (OQ->Eof)
            BeginPoint = CAMS_BadObjId+1;
        else
            BeginPoint = OQ->FieldAsInteger(0);

        FQueryText="select LEVEL, OWNCAT, DESCR, nvl(CATID, -1) PARENT from "+FBaseSchema+".CATTREE "
                   "start with CATID = :BEGIN_POINT "
                   "connect by PRIOR OWNCAT = CATID order by TORD ";
        OQ->DeleteVariables();
        OQ->DeclareVariable("BEGIN_POINT", otInteger);
        OQ->SetVariable("BEGIN_POINT", BeginPoint);
    }
};
//---------------------------------------------------------------------------
void TAmsObjCatView::PrepareSQL(int BeginPoint_)
{
    BeginPoint = BeginPoint_;
    if (BeginPoint_ == CAMS_BadObjId)
    {
        FQueryText="select LEVEL, OWNCAT, DESCR, nvl(CATID, -1) PARENT from "+FBaseSchema+".CATTREE "
                   "start with CATID is NULL "
                   "connect by PRIOR OWNCAT = CATID order by TORD ";
        OQ->SQL->Text = FQueryText;
        OQ->DeleteVariables();
    }
    else
    {
        FQueryText="select LEVEL, OWNCAT, DESCR, nvl(CATID, -1) PARENT from "+FBaseSchema+".CATTREE "
                   "start with CATID = :BEGIN_POINT "
                   "connect by PRIOR OWNCAT = CATID order by TORD ";
        OQ->DeleteVariables();
        OQ->DeclareVariable("BEGIN_POINT", otInteger);
        OQ->SetVariable("BEGIN_POINT", BeginPoint);
    };
};
//---------------------------------------------------------------------------
AnsiString TAmsObjCatView::FormNodeText()
{
    return OQ->FieldAsString(2);
};
//---------------------------------------------------------------------------
TAmsTreeData* TAmsObjCatView::FormTreeData (TTreeNode* TN)
{
    return new TAmsTreeData(OQ->FieldAsInteger(1), TN);
};
//---------------------------------------------------------------------------
int TAmsObjCatView::GetParentCode()
{
    return OQ->FieldAsInteger(3);
};
//---------------------------------------------------------------------------
int TAmsObjCatView::GetCurrLevel()
{
    return OQ->FieldAsInteger(0);
};
//---------------------------------------------------------------------------