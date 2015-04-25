//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "FRM_CHOOSE_OBJ.h"
#include "ams_win_c.h"
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
      "select OC.OWNCAT, OC.CATID, OC.ORD,      OC.DESCR "
      "from "+BaseUserName+".OBJCAT OC "
      "where OC.CATID is null  "
      "union  "
      "select OC.OWNCAT, OC.CATID, OC.ORD,      OC.DESCR  "
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

//------------------------- CHANGED AS ANSWER QUESTION #6 MADE 15.03.02
        if (CurrOType==otConst || CurrOType==otDoc || CurrOType==otRepForm)
        SG->ColWidths[1]=100;
        SGResize(this);
//------------------------- EO CHANGED AS ANSWER QUESTION #6 MADE 15.03.02
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
            HelpContext = 111;
        break;
        case otRepForm :
            Caption = "Отчетные формы";
            HelpContext = 25;
        break;
        case otStdOper :
            Caption = "Стандартные операции";
            HelpContext = 3;
        break;
        case otDB:
            Caption = "Базы данных";
            HelpContext = 7;
            ActDelPos->Enabled=true;
            ActEditInSet->Enabled=true;

        break;
        case otAnal:
            Caption = "Аналитики";
        break;
        case otConst:
            Caption = "Константы";
            HelpContext = 99;
        break;
        case otViewFrm:
            Caption = "Вид формы";
        break;
    };
    CurrCode = -1;
    ToolButton1Click(this);
    SGResize(this);
    Show();
};

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TFrmChooseObj::RefreshList(void)
{
    if (CurrOType!=otConst)
       SG->ColWidths[4]= 0;

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
        SG->Cells[4][1]= "";
        SG->Cells[5][1]= "";
    }
    else
    {
        SG->RowCount=OQ->FieldAsInteger(0)+1;

        AnsiString s_add= CurrOType==otConst ? ", VALUE, const_type " : "" ;
        AnsiString f_add= CurrOType==otConst ? ","+BaseUserName+".consts c" : AnsiString("") ;
        AnsiString w_add= CurrOType==otConst ? " AND c.oid=o.oid " : "" ;

        OQ->SQL->Text =
        " select O.OID, O.NAME, O.DESCR, nvl(O.HIST,0) HIST "+s_add+
        " from "+BaseUserName+".OBJS O, "+BaseUserName+".PRIVTEST PT "+f_add+
        " where "
            " O.CATID = :CATID and "
            " O.OTYPE = :OTYPE and "
            " O.OID = PT.POID and "
            " PT.UNAME = USER and "
            " PR0=1 "+w_add+
            " ORDER BY o.ord" ;

        ExecSQL(OQ, "Объекты категории");
        int i = 1;
        while (!OQ->Eof)
        {
            SG->Cells[0][i]= IntToStr(OQ->FieldAsInteger(0));
            SG->Cells[1][i]= OQ->FieldAsString(1);
            SG->Cells[2][i]= OQ->FieldAsString(2);
            SG->Cells[3][i]= IntToStr(OQ->FieldAsInteger(3));
            SG->Cells[4][i]= CurrOType==otConst ? OQ->FieldAsString(4) : AnsiString("") ;
            SG->Cells[5][i]= CurrOType==otConst ? IntToStr(OQ->FieldAsInteger(5)) : AnsiString("") ; // const type
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
    SG->Cells[4][0]="Значение";
    SG->Cells[5][0]="Тип";
    SG->ColWidths[3]= 0;
    if ( !GC::ViewDebug)
    {
        SG->ColWidths[0]=-1;
        SG->ColWidths[1]=-1;
        SG->ColWidths[5]=-1;
//        SG->Options>>goColSizing;
//        SG->Options>>goVertLine;
    }
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
    if (AmsWinC->FrmChooseDb==this)
        AmsWinC->FrmChooseDb=0;
    if (AmsWinC->FrmChooseConst==this)
        AmsWinC->FrmChooseConst=0;
    if (AmsWinC->FrmChooseForms==this)
        AmsWinC->FrmChooseForms=0;
    if (AmsWinC->FrmChooseStdOper==this)
        AmsWinC->FrmChooseStdOper=0;
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
    FCKeyDown(Key, Shift);
    if (Key==32)
    {
        TV->Selected->Expanded =  !TV->Selected->Expanded;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmChooseObj::SGKeyPress(TObject *Sender, char &Key)
{

    unsigned short T = Key;
    FCKeyDown(T, TShiftState());
    Key=T;
    if (Key == 13 || Key == 32)
    {
            MakeSomething();
    }
}
//---------------------------------------------------------------------------
void TFrmChooseObj::EditDB (int id)
{
    int DbId = id;
    if (!id)
       DbId = StrToInt(SG->Cells[0][SG->Row]);
    TFrmBrowsDb* F;
    Application->CreateForm (__classid(TFrmBrowsDb), &F);

    F->Begin(DbId, SG->Cells[1][SG->Row], SG->Cells[2][SG->Row], "");
}
//---------------------------------------------------------------------------
void __fastcall TFrmChooseObj::SGDblClick(TObject *Sender)
{
    MakeSomething();
}
//---------------------------------------------------------------------------

void __fastcall TFrmChooseObj::FormActivate(TObject *Sender)
{
    TopMostForm = this;
}
//---------------------------------------------------------------------------
void __fastcall TFrmChooseObj::SGResize(TObject *Sender)
{
    //
    int W =SG->ColWidths[0]+SG->ColWidths[1];
    int wc= CurrOType==otConst ? 100 : -1 ;
    SG->ColWidths[4]= wc ;
    SG->ColWidths[2] = SG->Width-26-W-wc ;
    SG->ColWidths[3]= -1;
}
//---------------------------------------------------------------------------


void __fastcall TFrmChooseObj::ActDelPosExecute(TObject *Sender)
{
    // Удаленные позиции для баз данных, у которых включена история
    TFrmHistBrws* FrmHistBrws = NULL;
    bool HaveHist=false;
    do {
        AmsQuery AQ;
        AQ.SetSqlText( "select HIST from *.OBJS where OID=:OID");
        AQ.DeclVar("OID",StrToInt(SG->Cells[0][SG->Row]));
        AQ.Exec("Выясняем есть ли история для данной таблицы");
        if (AQ.NotEof())
        {
            if (AQ.GetFldInt(0)==1)
            {
                HaveHist=true;
            }
        }
    }
    while (0);

    if (!HaveHist)
    {
        ErrorMessage("Для таблицы не вкючено хранение истории");

    }
    else
    {
        Application->CreateForm(__classid(TFrmHistBrws), &FrmHistBrws);
        FrmHistBrws->Begin(StrToInt(SG->Cells[0][SG->Row]), SG->Cells[1][SG->Row],
            SG->Cells[1][SG->Row], "" );
    }
}
//---------------------------------------------------------------------------

void TFrmChooseObj::MakeSomething(void)
{
    if (SG->Cells[0][SG->Row]!="")
    {
        switch (CurrOType)
        {
            case otDoc, otRepForm:
                BeginExecReport(SG->Cells[1][SG->Row]);
            break;
            case otStdOper:
            break;
            case otDB:
                EditDB(0);
            break;
            case otAnal:
            {
                int ASEd =-1;
                AskInt("Введите какую аналитическую сумму"
                            " редактируем\r\n(-1)-Новая", ASEd);

                if (ASEd ==-1)
                {
                    AmsQuery AQ;
                    AQ.SetSqlText("begin *.CreateTmpSum (:TMPSUM, 0, 'Test An SUM Edit'); end;");
                    AQ.DeclVar("TMPSUM", 0);
                    AQ.Exec ("Создаем врем сумму");
                    ASEd= AQ.GetVarInt("TMPSUM");
                }

                int ASOst = 0;
                AskInt("Введите аналитическую сумму остатка", ASOst);
                int AnId = StrToInt(SG->Cells[0][SG->Row]);
                TFrmBrowsAn* F;
                Application->CreateForm (__classid(TFrmBrowsAn), &F);
                TFrmGetSetName* FS;
                Application->CreateForm (__classid(TFrmGetSetName), &FS);

                AnsiString SetName = FS->GetSetName(AnId, "В каком наборе установок");
                FS->Release();
                extern OpVars* opvars;
                delete opvars;
                opvars= new OpVars;
                opvars->AddVarInt("OP_ID",0);
                opvars->AddVarInt("OP_SUBID",0);
                AmsQuery AQ;
                AQ.SetSqlText("select * from *.ACCS where NAME='B/2/8/001'");
                AQ.Exec("");
                if (AQ.NotEof())
                {
                    opvars->AddVars(AQ);
                }

                F->Begin(0, AnId, SG->Cells[1][SG->Row], SetName, SG->Cells[2][SG->Row], ASEd, ASOst);
            }
            break;
            case otConst:
                 EditConst() ;
            case otViewFrm:
            break;
        }
    }
}

void TFrmChooseObj::EditConst()
{
    int type = StrToInt(SG->Cells[5][SG->Row]);
    AnsiString str;

    switch (type)
    {
        case aftInt:
        case aftBool:
        {
            int integ=0;
            str=SG->Cells[4][SG->Row] ;
            if (str.IsEmpty()) integ=0 ;
            else integ=StrToInt(str) ;
            int Res = AskInt(SG->Cells[2][SG->Row], integ) ;
            if (!Res) return;
            str=IntToStr(integ);
        }
        break;
        case aftSumma:
        {
            double integ=0;
            str=SG->Cells[4][SG->Row] ;
            if (str.IsEmpty()) integ=0 ;
            else
            {
                int i =1;
                while (i<=str.Length())
                {
                    if (str[i]=='\'')
                        str.Delete(i,1);
                    else i++;
                }
                for (int i = 1; i<= str.Length();i++)
                {
                    if (str[i]=='-')
                        str[i]=DecimalSeparator;
                }
                integ=StrToFloat(str) ;
            }
            int Res = AskSum(SG->Cells[2][SG->Row], integ) ;
            if (!Res) return;
            str=FloatToStr(integ);
            for (int i = 1; i<= str.Length();i++)
            {
                if (str[i]==DecimalSeparator)
                    str[i]='-';
            }
        }
        break;
        case aftDate:
        {
            TDateTime DT=0;
            str=SG->Cells[4][SG->Row] ;
            if (!str.IsEmpty())
            {
                for (int i = 1; i<= str.Length();i++)
                {
                    if (str[i]=='-')
                        str[i]=DateSeparator;
                }
                DT=StrToDate(str) ;
            }
            int Res = AskDate(SG->Cells[2][SG->Row], DT) ;
            if (!Res) return;
            str=DateToStr(DT);
            for (int i = 1; i<= str.Length();i++)
            {
                if (str[i]==DateSeparator)
                    str[i]='-';
            }
        }
        break;
        case aftStr:
        {
            str=SG->Cells[4][SG->Row];
            int Res = AskString(SG->Cells[2][SG->Row], 250, str) ;
            if (!Res) return;
        }
        break;
    }
    AmsQuery Q ;
    Q.SetPLText(" update *.consts set value=:value where OID=:OID; commit;") ;
    Q.DeclVar("OID",StrToInt(SG->Cells[0][SG->Row]));
    Q.DeclVar("VALUE", str);
    Q.Exec("Изменяем константу");
    SG->Cells[4][SG->Row]=str;
}
//---------------------------------------------------------------------------
void __fastcall TFrmChooseObj::ButEnterClick(TObject *Sender)
{
    //
    if (ActiveControl==TV)
    {
        if (TV->Selected->HasChildren)
            TV->Selected->Expanded != TV->Selected->Expanded;
        else
            SG->SetFocus();
    }
    else
    if (ActiveControl==SG)
    {
        MakeSomething();
    }

}
//---------------------------------------------------------------------------
void TFrmChooseObj::FCKeyDown (WORD &Key, TShiftState Shift)
{
};
//---------------------------------------------------------------------------

void __fastcall TFrmChooseObj::ActEditInSetExecute(TObject *Sender)
{
//
    int DbId = StrToInt(SG->Cells[0][SG->Row]);
    if (DbId)
    {
        TFrmGetSetName* Frm;
        Application->CreateForm(__classid(TFrmGetSetName), &Frm);
        AnsiString Set = Frm->GetSetName(DbId, "В каком наборе установок редактировать");
        Frm->Release();
        TFrmBrowsDb* F;
        Application->CreateForm (__classid(TFrmBrowsDb), &F);

        F->Begin(DbId, SG->Cells[1][SG->Row], SG->Cells[2][SG->Row], Set);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmChooseObj::ActEscExitExecute(TObject *Sender)
{
   this->Close();
}
//---------------------------------------------------------------------------

