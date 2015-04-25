//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "AmsDbCatView.h"
#include "AMS_CONST.h"
#pragma link "AMS_TREE_VIEW"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAmsDbCatView *)
{
    new TAmsDbCatView(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAmsDbCatView::TAmsDbCatView(TComponent* Owner)
    : TAmsTreeView(Owner)
{
}
//---------------------------------------------------------------------------
namespace Amsdbcatview
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TAmsDbCatView)};
         RegisterComponents("Samples", classes, 0);
    }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TAmsDbCatView::PrepareSQL(const AnsiString& BeginPoint_)
{
    OQ->DeleteVariables();
    if (BeginPoint_ =="")
    {
        FQueryText="select LEVEL, OWNCAT, DESCR, nvl(CATID, -1) CATID from "+FBaseSchema+".DBCAT "
                   "where DBID = :DBID start with CATID is NULL "
                   "connect by PRIOR OWNCAT = CATID order by LEVEL, ORD ";
        OQ->SQL->Text = FQueryText;
        BeginPoint = CAMS_BadObjId;
    }
    else
    {
        OQ->SQL->Text = " select OWNCAT from "+FBaseSchema+".DBCAT "
        " where DBID= :DBID and DESCR= :BEGIN_POINT";
        OQ->DeclareVariable("BEGIN_POINT", otString);
        OQ->SetVariable("BEGIN_POINT", BeginPoint_);
        OQ->DeclareVariable("DBID", otInteger);
        OQ->SetVariable("DBID", FOId);
        if (OQ->Eof)
            BeginPoint = CAMS_BadObjId+1;
        else
            BeginPoint = OQ->FieldAsInteger(0);

        FQueryText="select LEVEL, OWNCAT, DESCR, nvl(CATID, -1) CATID from "+FBaseSchema+".DBCAT "
                   "where DBID = :DBID start with CATID = :BEGIN_POINT "
                   "connect by PRIOR OWNCAT = CATID order by LEVEL, ORD ";
        OQ->DeleteVariables();
        OQ->DeclareVariable("BEGIN_POINT", otInteger);
        OQ->SetVariable("BEGIN_POINT", BeginPoint);
    }
    OQ->DeclareVariable("DBID", otInteger);
    OQ->SetVariable("DBID", FOId);

};
//---------------------------------------------------------------------------
void TAmsDbCatView::PrepareSQL(int BeginPoint_)
{
    BeginPoint = BeginPoint_;
    if (BeginPoint_ == CAMS_BadObjId)
    {
        FQueryText="select LEVEL, OWNCAT, DESCR, nvl(CATID, -1) CATID from "+FBaseSchema+".DBCAT "
                   "where DBID = :DBID start with CATID is NULL "
                   "connect by PRIOR OWNCAT = CATID order by LEVEL, ORD ";
        OQ->SQL->Text = FQueryText;
        OQ->DeleteVariables();
    }
    else
    {
        FQueryText="select LEVEL, OWNCAT, DESCR, nvl(CATID, -1) CATID from "+FBaseSchema+".DBCAT "
                   "where DBID = :DBID start with CATID = :BEGIN_POINT "
                   "connect by PRIOR OWNCAT = CATID order by LEVEL, ORD ";
        OQ->DeleteVariables();
        OQ->DeclareVariable("BEGIN_POINT", otInteger);
        OQ->SetVariable("BEGIN_POINT", BeginPoint);
    };
    OQ->DeclareVariable("DBID", otInteger);
    OQ->SetVariable("DBID", FOId);
};
//---------------------------------------------------------------------------
AnsiString TAmsDbCatView::FormNodeText()
{
    return OQ->FieldAsString(2);
};
//---------------------------------------------------------------------------
TAmsTreeData* TAmsDbCatView::FormTreeData (TTreeNode* TN)
{
    return new TAmsTreeData(OQ->FieldAsInteger(1), TN);
};
//---------------------------------------------------------------------------
int TAmsDbCatView::GetParentCode()
{
    return OQ->FieldAsInteger(3);
};
//---------------------------------------------------------------------------
int TAmsDbCatView::GetCurrLevel()
{
    return OQ->FieldAsInteger(0);
};
//---------------------------------------------------------------------------

