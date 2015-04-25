//---------------------------------------------------------------------------

#ifndef AmsAccsViewH
#define AmsAccsViewH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "AMS_TREE_VIEW.h"
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class PACKAGE TAmsAccsView : public TAmsTreeView
{ 
private:
protected:
    virtual void PrepareSQL(const AnsiString& BeginPoint_);
    virtual void PrepareSQL(int BeginPoint_);
    virtual AnsiString    FormNodeText();
    virtual TAmsTreeData* FormTreeData (TTreeNode* TN);
    virtual int GetParentCode();        
    virtual int GetCurrLevel();
public:
    __fastcall TAmsAccsView(TComponent* Owner);
__published:
};
//---------------------------------------------------------------------------
#endif
