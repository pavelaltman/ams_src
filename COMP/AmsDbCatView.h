//---------------------------------------------------------------------------

#ifndef AmsDbCatViewH
#define AmsDbCatViewH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "AMS_TREE_VIEW.h"
#include <ComCtrls.hpp>
#include "AMS_utils.h"
//---------------------------------------------------------------------------
class PACKAGE TAmsDbCatView : public TAmsTreeView
{
private:
protected:
    int FOId;
    virtual void Begin(const AnsiString& BaseSchema_, const AnsiString& BeginPoint_)
    {TAmsTreeView::Begin(BaseSchema_, BeginPoint_);};
    virtual void Begin(const AnsiString& BaseSchema_, int BeginPoint_)
    {TAmsTreeView::Begin(BaseSchema_, BeginPoint_);};
    //--------------------------------------------------------
    virtual void PrepareSQL(const AnsiString& BeginPoint_);
    virtual void PrepareSQL(int BeginPoint_);
    virtual AnsiString    FormNodeText();
    virtual TAmsTreeData* FormTreeData (TTreeNode* TN);
    virtual int GetParentCode();        
    virtual int GetCurrLevel();        

public:
    __fastcall TAmsDbCatView(TComponent* Owner);

    // DbId - код базы данных (из OBJS)
    virtual void Begin(const AnsiString& BaseSchema_, int DbId, const AnsiString& BeginPoint_)
    {FOId=DbId; Begin(BaseSchema_, BeginPoint_);};
    virtual void Begin(const AnsiString& BaseSchema_, int DbId, int BeginPoint_)
    {FOId=DbId; Begin(BaseSchema_, BeginPoint_);};

    // DBName имя базы данных (NAME из OBJS)
    virtual void Begin(const AnsiString& BaseSchema_, const AnsiString& DbName, 
                       const AnsiString& BeginPoint_)
    {Begin(BaseSchema_, GetObjId(BaseSchema_, DbName, OQ), BeginPoint_);};
    virtual void Begin(const AnsiString& BaseSchema_, const AnsiString& DbName,
                       int BeginPoint_)
    {Begin(BaseSchema_, GetObjId(BaseSchema_, DbName, OQ), BeginPoint_);};
                       
__published:
    
};
//---------------------------------------------------------------------------
#endif
