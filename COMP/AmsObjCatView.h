//---------------------------------------------------------------------------

#ifndef AmsObjCatViewH
#define AmsObjCatViewH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "AMS_TREE_VIEW.h"
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class PACKAGE TAmsObjCatView : public TAmsTreeView
{
private:
protected:
    virtual void PrepareSQL(const AnsiString& BeginPoint_);
    virtual void PrepareSQL(int BeginPoint_);
    virtual AnsiString    FormNodeText();
    virtual TAmsTreeData* FormTreeData (TTreeNode* TN);
    virtual int GetParentCode();
    virtual int GetCurrLevel();

    bool FSecureShow;        
public:
    __fastcall TAmsObjCatView(TComponent* Owner);
__published:
     // определяет что показываем только то что есть в списке PRIVTEST   
    __property bool AmsSecureShow = {read =FSecureShow, 
                                     write=FSecureShow, default =true};
    
};
//---------------------------------------------------------------------------
#endif
