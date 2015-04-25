//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "AMS_TREE_VIEW.h"
#include "AMS_UTILS.h"
#include "AMS_CONST.h"
#include "print_error.h"
#include "AMS_EXC.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
// Класс для сохранения последлвательности выделенных объектов
//---------------------------------------------------------------------------
void TAmsSelObjsList::SelectObj (int OId)
{
    unsigned int i = 0;
    while (i< OL.size())
    {
        if ( OL[i]== OId)
            return; 
        else i++;        
    };
    OL.push_back(OId);    
};    
//---------------------------------------------------------------------------
void TAmsSelObjsList::DeSelectObj (int OId)
{
    unsigned int i =0;
    while (i<OL.size())
    {
        if (OL[i]==OId)
        {
            for (unsigned int j = i+1; j< OL.size(); j++)
                OL[j-1]=OL[j];
            OL.pop_back();
            break;    
        }    
        else i++;
    }
};
//---------------------------------------------------------------------------
void TAmsSelObjsList::GetCommaText (AnsiString& Res)
{
    Res="";
    for (unsigned int i = 0 ; i < OL.size(); i++)
    {
        if (i>0) Res+=", ";
        Res+= IntToStr(OL[i]);
    }
};    
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
static inline void ValidCtrCheck(TAmsTreeView *)
{
    //new TAmsTreeView(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAmsTreeView::TAmsTreeView(TComponent* Owner)
    : TTreeView(Owner),Ready(false), FQueryShared(true), OQ(NULL)
{
}
//---------------------------------------------------------------------------
namespace Ams_tree_view
{
    void __fastcall PACKAGE Register()
    {
         //TComponentClass classes[1] = {__classid(TAmsTreeView)};
         //RegisterComponents("Samples", classes, 0);
    }
}
//---------------------------------------------------------------------------
void TAmsTreeView::Begin(const AnsiString& BaseSchema_, 
                         const AnsiString& BeginPoint_)
{
    if (!OQ)
    {
        THROW_AMS_EXC("TAmsTreeView: Не задан TOracleQuery");
    }
    FBaseSchema = BaseSchema_;
    PrepareSQL(BeginPoint_);
    Ready =true;
    FillTheTreeView(false);
};
//---------------------------------------------------------------------------
void TAmsTreeView::Begin(const AnsiString& BaseSchema_, int BeginPoint_)
{
    if (!OQ)
    {
        THROW_AMS_EXC("TAmsTreeView: Не задан TOracleQuery");
    }
    FBaseSchema = BaseSchema_;
    PrepareSQL(BeginPoint_);
    Ready =true;
    FillTheTreeView(false);
};
//---------------------------------------------------------------------------
void TAmsTreeView::Clear ()
{
    ClearDel<TAmsTreeData*> X;
    std::for_each(TreeData.begin(), TreeData.end(), X);
    TreeData.clear();
    Items->Clear();
};

//---------------------------------------------------------------------------
void __fastcall TAmsTreeView::FillTheTreeView(bool DeepQuerySet)
{
    if (DeepQuerySet)
    {
        OQ->SQL->Text=FQueryText;
        OQ->DeleteVariables();
        if (BeginPoint != CAMS_BadObjId)
        {
            OQ->DeclareVariable("BEGIN_POINT", otInteger);
            OQ->SetVariable("BEGIN_POINT", BeginPoint);
        }    
    }                                

    ExecSQL(OQ,"TREE_VIEW Component");
    Clear();
    TTreeNode* TN;
    bool FirstTime = true;
    while (!OQ->Eof)
    {
        int ParentCode = GetParentCode();
        //int CurrLevel = GetCurrLevel();
        if (FirstTime || ParentCode==-1 || ParentCode==BeginPoint )
        {
            FirstTime = false;
            TN = Items->AddChild(NULL, FormNodeText());
        }
        else
        {
            TN = Track[ParentCode];
            if (TN)
                TN=Items->AddChild(TN, FormNodeText());
        }

        if (TN)
        {
            TN->ImageIndex=-1;
            TN->SelectedIndex=-1;
            TAmsTreeData* F = FormTreeData(TN);
            TreeData.push_back(F);
            TN->Data=F;
            Track[F->GetCode()]= TN;
        }
      OQ->Next();
    };
};
//---------------------------------------------------------------------------
// Сделать активной категорию с кодом Code
void TAmsTreeView::MakeActive (int Code)
{
    TTreeNode* TN = Track[Code];
    if (TN)
        Selected= TN;
};
//---------------------------------------------------------------------------
// Возвращает код Активной категории
int TAmsTreeView::GetCurrCode ()
{
    int Res= -1;
    if (!Selected)
    {
        return 0;
    }
    else
    {
        TAmsTreeData* TD = (TAmsTreeData*) Selected->Data;
        if (TD)
            Res=TD->GetCode();
    }
    return Res;
};
//---------------------------------------------------------------------------
// Возвращает код Активной категории
int TAmsTreeView::GetCode (TTreeNode* TN)
{
    int Res= -1;
    if (TN)
    {
        TAmsTreeData* TD = (TAmsTreeData*) TN->Data;
        if (TD)
            Res=TD->GetCode();
    }
    return Res;      
};
//---------------------------------------------------------------------------
// Выбрать активную категорию и поместить в список выбранных категорий
void TAmsTreeView::SelectCurr()
{
    if (Selected)
    {
        Selected ->ImageIndex= 4;
        Selected ->SelectedIndex= 5;
        int CurrCode = GetCurrCode();
        if (CurrCode!=-1)
            SelectedCats.SelectObj(CurrCode);
    }
};
//---------------------------------------------------------------------------
// Убрать активную категорию из списка выбранных категорий
void TAmsTreeView::DeSelectCurr()
{
    Selected ->ImageIndex= -1;
    Selected ->SelectedIndex= -1;
    int CurrCode = GetCurrCode();
    if (CurrCode!=-1)
        SelectedCats.DeSelectObj(CurrCode);    
};
//---------------------------------------------------------------------------
void TAmsTreeView::RefreshView()
{
    if (Ready)    
        FillTheTreeView(FQueryShared);
    else
        ErrorMessage("AmsTreeView::RefreshView. Сначала вызовите функцию Begin");
};
//---------------------------------------------------------------------------
TTreeNode* TAmsTreeView::NewLeaf(TTreeNode* Root, TTreeNode* TN, 
                                const AnsiString& Txt, int Code)
{
    TTreeNode* NewNode;
    if (TN)
    {
        NewNode= Items->Insert (TN, Txt);    
    }    
    else
    {
        NewNode= Items->AddChild(Root, Txt); 
    }

    Track[Code]=NewNode;
    TAmsTreeData* TD= new TAmsTreeData (Code, NewNode);
    NewNode->Data = TD;
    return NewNode;
};
//---------------------------------------------------------------------------
TTreeNode* TAmsTreeView::MoveNode (TTreeNode* TN, bool Up)
{
    TTreeNode* Par= TN->Parent;
    TTreeNode* Res = NULL;
    if (Par)
    {
        int CurrPos = TN->Index;
        if (Up && CurrPos)
        {
            Res = Par->Item[CurrPos-1];
            TN->MoveTo(Res, naInsert);
        }
        if ( !Up && (CurrPos< (Par->Count-1)))
        {
            Res = Par->Item[CurrPos+1];
            Res->MoveTo(TN, naInsert);
        }
    }
    return Res;
};
//---------------------------------------------------------------------------
void TAmsTreeView::DelNodeByCode( int Code)
{
    TTreeNode* TN = GetNodeByCode(Code);
    if (TN && TN->Parent)
    {
        Track [Code]= NULL;
        TN->Delete();
    }
};
//---------------------------------------------------------------------------

