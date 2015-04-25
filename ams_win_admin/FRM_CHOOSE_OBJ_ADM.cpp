//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "FRM_CHOOSE_OBJ_ADM.h"
#include "FRM_SET_AN_FLD_ORDER.h"
#include "MAIN_FRM.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AMS_TREE_VIEW"
#pragma link "AmsObjCatView"
#pragma link "Oracle"
#pragma link "AmsStringGrid"
#pragma resource "*.dfm"
TFrmChooseObj *FrmChooseObj;
//---------------------------------------------------------------------------
/*
class TAmsTreeData
{
protected:
    TTreeNode* TN;
    int Code;
public:
    TAmsTreeData (int Code_, TTreeNode* TN_):Code(Code_), TN(TN_){};
    virtual ~TAmsTreeData (){};
    virtual int GetCode() const {return Code;};
    virtual const AnsiString& GetDescr()const {return TN->Text;};
    virtual int GetLevel()const {return TN->Level;};
};
*/
//---------------------------------------------------------------------------
void TFrmChooseObj::ClearTree()
{
    Cleared = 1;
    ClearDel<TAmsTreeData*> X;
    std::for_each(TreeData.begin(), TreeData.end(), X);
    TreeData.clear();
    TV->Items->Clear();
    Cleared = 0;
};
//---------------------------------------------------------------------------
__fastcall TFrmChooseObj::TFrmChooseObj(TComponent* Owner)
    : TForm(Owner)
{
    CurrCode = -1;
    Cleared= 0;
}
//---------------------------------------------------------------------------
void __fastcall TFrmChooseObj::ToolButton1Click(TObject *Sender)
{
    struct SObjCat
    {
        int CatId;
        int OwnCat;
        int Ord;
        int Lev;
        AnsiString Descr;
    };

    
    SObjCat X;
    std::map < int, TTreeNode*> TNVals;
    
    OQ->SQL->Text = 
      "select T.LEV, S.OWNCAT, S.CATID, S.ORD, S.DESCR "
      "from "
      "( "
      "select OC.OWNCAT, OC.CATID, OC.ORD, OC.DESCR "
      "from "+BaseUserName+".OBJCAT OC, "+BaseUserName+".PRIVTEST PT "
      "where  "
          "OC.OWNCAT = PT.POID and "
          "UNAME = USER and "  
          "PR0 = 1 and " 
          "OC.OWNCAT in " 
              "(select CATID  "
              " from "+BaseUserName+".OBJS O  "
              " where OTYPE = :OTYPE " 
              ") " 
      "union " 
      "select OC.OWNCAT, OC.CATID, OC.ORD,	OC.DESCR "
      "from "+BaseUserName+".OBJCAT OC "
      "where OC.CATID is null  "
      "union  "
      "select OC.OWNCAT, OC.CATID, OC.ORD,	OC.DESCR  " 
      "from "+BaseUserName+".OBJCAT OC  "
      "where 1 = (select PR0 from "+BaseUserName+".PRIVTEST where POID = OC.OWNCAT and UNAME=USER) "    
      "start with OC.OWNCAT in " 
      "(select OC.OWNCAT  " 
      "from "+BaseUserName+".OBJCAT OC, "+BaseUserName+".PRIVTEST PT " 
      "where  "
          "OC.OWNCAT = PT.POID and "
          "UNAME = USER and  "
          "PR0 = 1 and  "
          "OC.OWNCAT in "  
              "(select CATID "
              " from "+BaseUserName+".OBJS O "
              " where OTYPE = :OTYPE "
              ") "  
      ") " 
      "connect by prior CATID = OWNCAT "
      ") S, "
      "(select LEVEL as LEV, OWNCAT "
      " from "+BaseUserName+".OBJCAT  "
      " start with CATID is NULL "
      " connect by PRIOR OWNCAT = CATID "
      ") T where S.OWNCAT = T.OWNCAT order By T.Lev, S.ORD ";
    OQ->DeleteVariables();
    OQ->DeclareVariable("OTYPE", otInteger);
    OQ->SetVariable("OTYPE", CurrOType);
    ExecSQL(OQ, "Категории объектов");
    TTreeNode * TreeNode=NULL;
    ClearTree();
    TNVals.clear();
    
    while (!OQ->Eof)
    {
        X.Lev=OQ->FieldAsInteger(0);
        X.OwnCat=OQ->FieldAsInteger(1);
        X.CatId=OQ->FieldAsInteger(2);
        X.Ord=OQ->FieldAsInteger(3);
        X.Descr=OQ->FieldAsString(4);
        if (OQ->FieldIsNull(2))
        {
            X.CatId = -1;
            TreeNode = TV->Items->AddChild(NULL, X.Descr);
            TNVals[X.OwnCat]= TreeNode;             
            TAmsTreeData*  TD = new TAmsTreeData(X.OwnCat, TreeNode);
            TreeNode->Data = TD;
            TreeData.push_back (TD);
        }
        else
        {
            TreeNode = TNVals[X.CatId];
            if (TreeNode)
            {
                TreeNode = TV->Items->AddChild (TreeNode, X.Descr);
                TNVals[X.OwnCat]=TreeNode;                
                TAmsTreeData*  TD = new TAmsTreeData(X.OwnCat, TreeNode);
                TreeNode->Data = TD;
                TreeData.push_back (TD);
            }
        }
        OQ->Next();
    };
    if (CurrCode != -1)
    {
        TreeNode = TNVals[CurrCode];
        if (! TreeNode )
            CurrCode = -1;
        else
            TV->Selected = TreeNode;
    };
    if (CurrCode == -1)
    {
        TV->Selected = TV->Items->GetFirstNode();
        TAmsTreeData* TD = (TAmsTreeData*) TV->Selected->Data;
        CurrCode = TD->GetCode();
        TV->Selected->Expanded = true; 
    };
    RefreshList();
}
//---------------------------------------------------------------------------
void TFrmChooseObj::Begin(int OType)
{
    CurrOType=OType;
    ClearTree();
    switch (CurrOType)
    {
        case otDoc:
            Caption = "Документы";
        break;
        case otRepForm :
            Caption = "Отчетные формы";
        break;
        case otStdOper :
            Caption = "Стандартные операции";
        break;
        case otDB:
            Caption = "Базы данных";        
        break;
        case otAnal:
            Caption = "Аналитики";        
        break;
        case otConst:
            Caption = "Константы";        
        break;
        case otViewFrm:
            Caption = "Вид формы";        
        break;        
    };
    CurrCode = -1;
    ToolButton1Click(this);    
    Show();
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TFrmChooseObj::RefreshList(void)
{
    //Обновляем вид списка объектов
    //
    OQ->SQL->Text = 
    " select count(*) " 
    " from "+BaseUserName+".OBJS O, "+BaseUserName+".PRIVTEST PT "
    " where "
	    " O.CATID = :CATID and "
	    " O.OTYPE = :OTYPE and " 
	    " O.OID = PT.POID and "
	    " PT.UNAME = USER and "
	    " PR0=1 ";
    OQ->DeleteVariables ();
    OQ->DeclareVariable("CATID", otInteger);    
    OQ->DeclareVariable("OTYPE", otInteger);
    OQ->SetVariable("CATID", CurrCode);
    OQ->SetVariable("OTYPE", CurrOType);
    ExecSQL (OQ, "Количество объектов в категории");
    if(!OQ->FieldAsInteger(0))
    {
        SG->RowCount= 2;
        SG->Cells[0][1]= "";
        SG->Cells[1][1]= "";        
        SG->Cells[2][1]= "";        
        SG->Cells[3][1]= "";        
    }
    else
    {
        SG->RowCount=OQ->FieldAsInteger(0)+1;
        OQ->SQL->Text = 
        " select O.OID, O.NAME, O.DESCR, nvl(O.HIST,0) HIST  " 
        " from "+BaseUserName+".OBJS O, "+BaseUserName+".PRIVTEST PT "
        " where "
            " O.CATID = :CATID and "
            " O.OTYPE = :OTYPE and " 
            " O.OID = PT.POID and "
            " PT.UNAME = USER and "
            " PR0=1 ORDER BY o.ord";
        ExecSQL(OQ, "Объекты категории");
        int i = 1;
        while (!OQ->Eof)
        {
            SG->Cells[0][i]= IntToStr(OQ->FieldAsInteger(0));
            SG->Cells[1][i]= OQ->FieldAsString(1);
            SG->Cells[2][i]= OQ->FieldAsString(2);
            SG->Cells[3][i]= IntToStr(OQ->FieldAsInteger(3));
            i++;
            if (i> SG->RowCount)
                SG->RowCount++;
            OQ->Next();
        }    
    }                          
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void __fastcall TFrmChooseObj::TVChange(TObject *Sender, TTreeNode *Node)
{
    if (!Cleared)
    {
        TAmsTreeData* TD = (TAmsTreeData*)Node->Data;
        if (TD)
        {
            CurrCode= TD->GetCode();
            RefreshList();
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmChooseObj::FormCreate(TObject *Sender)
{
    SG->Cells[0][0]="Код";
    SG->Cells[1][0]="Название";
    SG->Cells[2][0]="Описание";
    SG->Cells[3][0]="История";
    SG->ColWidths[3]= 0;
    SGResize(this);
}
//---------------------------------------------------------------------------

void __fastcall TFrmChooseObj::FormDestroy(TObject *Sender)
{
    ClearTree();
}
//---------------------------------------------------------------------------

void __fastcall TFrmChooseObj::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Action = caFree;
}
//---------------------------------------------------------------------------


void __fastcall TFrmChooseObj::TVEditing(TObject *Sender, TTreeNode *Node,
      bool &AllowEdit)
{
    AllowEdit = false;
}
//---------------------------------------------------------------------------

void __fastcall TFrmChooseObj::TVKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if (Key==32 || Key == 13)
    {
        TV->Selected->Expanded =  !TV->Selected->Expanded;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmChooseObj::SGKeyPress(TObject *Sender, char &Key)
{
    if (Key == 13 || Key == 32)
    {
            MakeSomething();
    }        
}
//---------------------------------------------------------------------------
void TFrmChooseObj::EditDB (int)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmChooseObj::SGDblClick(TObject *Sender)
{
    MakeSomething();
}
//---------------------------------------------------------------------------

void __fastcall TFrmChooseObj::SGResize(TObject *Sender)
{
    //
    int W =SG->ColWidths[0]+SG->ColWidths[1];
    SG->ColWidths[2] = SG->Width-26-W;
    SG->ColWidths[3]= 0;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void TFrmChooseObj::MakeSomething(void)
{
    if (SG->Cells[0][SG->Row]!="")
    {
        int Code = StrToInt(SG->Cells[0][SG->Row]);
        AnsiString Name = SG->Cells[1][SG->Row];  //"Название";
        AnsiString Descr = SG->Cells[2][SG->Row]; //"Описание";



        switch (CurrOType)
        {
            case otDoc, otRepForm:
            break;
            case otStdOper:
            break;
            case otDB:
                EditDB(0);
            break;
            case otAnal:
                TFrmSetAnFldOrder* F;
                Application->CreateForm(__classid(TFrmSetAnFldOrder), &F);

                F->SetAnFldOrder(Code, Name, Descr);
            break;
            case otConst:
            case otViewFrm:
            break;
        }
    }
}

