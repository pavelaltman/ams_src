//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "AmsAccsView.h"
#include "AMS_CONST.h"
#include "AMS_UTILS.h"
#pragma link "AMS_TREE_VIEW"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAmsAccsView *)
{
    new TAmsAccsView(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAmsAccsView::TAmsAccsView(TComponent* Owner)
    : TAmsTreeView(Owner)
{
}
//---------------------------------------------------------------------------
namespace Amsaccsview
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TAmsAccsView)};
         RegisterComponents("Samples", classes, 0);
    }
}

//---------------------------------------------------------------------------
void TAmsAccsView::PrepareSQL(const AnsiString& BeginPoint_)
{
    if (BeginPoint_ =="")
    {
        FQueryText="select LEVEL, CODE, NAME, DESCR, nvl("+FBaseSchema+".GetParentAcc(code), -1) PARENT from "+FBaseSchema+".ACCS "
          "start with PARENT is NULL connect by PRIOR CODE = PARENT order by NAME";
        OQ->SQL->Text = FQueryText;
        OQ->DeleteVariables();
        BeginPoint = CAMS_BadObjId;
    }
    else
    {
        OQ->SQL->Text = " select CODE from "+FBaseSchema+".ACCS where NAME = :BEGIN_POINT";
        OQ->DeleteVariables();
        OQ->DeclareVariable("BEGIN_POINT", otString);
        OQ->SetVariable("BEGIN_POINT", BeginPoint_);
        ExecSQL(OQ, "Находим код счета");
        if (OQ->Eof)
            BeginPoint = CAMS_BadObjId+1;
        else
            BeginPoint = OQ->FieldAsInteger(0);

        FQueryText="select LEVEL, CODE, NAME, DESCR, nvl("+FBaseSchema+".GetParentAcc(code), -1) PARENT from "+FBaseSchema+".ACCS "
          "start with CODE = :BEGIN_POINT connect by PRIOR CODE = PARENT order by NAME";
        OQ->SQL->Text =FQueryText;
        OQ->DeleteVariables();
        OQ->DeclareVariable("BEGIN_POINT", otInteger);
        OQ->SetVariable("BEGIN_POINT", BeginPoint);
    }
};

//---------------------------------------------------------------------------
void TAmsAccsView::PrepareSQL(int BeginPoint_)
{
    BeginPoint = BeginPoint_;
    OQ->DeleteVariables();
    if (BeginPoint_ == CAMS_BadObjId)
    {
        FQueryText="select LEVEL, CODE, NAME, DESCR, nvl("+FBaseSchema+".GetParentAcc(code), -1) PARENT from "+FBaseSchema+".accs a, "+FBaseSchema+".privtest p "
         " where code=poid AND uname=USER AND pr0>0 AND "
         " code in (select code from "+FBaseSchema+".ACCS  start with PARENT is NULL connect by PRIOR CODE = PARENT) "
         " order by NAME" ;
    }
    else
    {
        FQueryText="select LEVEL, CODE, NAME, DESCR, nvl("+FBaseSchema+".GetParentAcc(code), -1) PARENT from "+FBaseSchema+".accs a, "+FBaseSchema+".privtest p "
         " where code=poid AND uname=USER AND pr0>0 AND "
         " code in (select code from "+FBaseSchema+".ACCS start with PARENT=:BEGIN_POINT connect by PRIOR CODE = PARENT) "
         " order by NAME" ;
        OQ->DeclareVariable("BEGIN_POINT", otInteger);
        OQ->SetVariable("BEGIN_POINT", BeginPoint);
    };
    OQ->SQL->Text = FQueryText;
};

//---------------------------------------------------------------------------
AnsiString TAmsAccsView::FormNodeText()
{
    return TAccName::ToUserName(OQ->FieldAsString(2))+" "+OQ->FieldAsString(3);
};

//---------------------------------------------------------------------------
TAmsTreeData* TAmsAccsView::FormTreeData (TTreeNode* TN)
{
    return new TAmsTreeData(OQ->FieldAsInteger(1), TN);
};
//---------------------------------------------------------------------------
int TAmsAccsView::GetParentCode()
{
    return OQ->FieldAsInteger(4);
};
//---------------------------------------------------------------------------
int TAmsAccsView::GetCurrLevel()
{
    return OQ->FieldAsInteger(0);
};
//---------------------------------------------------------------------------

