//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_CONDS_EDIT.h"
#include "MAIN_FRM.h"
#include "AMS_CONST.h"
#include "Frm_Cat_Tree.h"
#include "print_error.h"
#include "FRM_INPUT_VALUE.h"
#include "FRM_ACCS_TIES.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "OracleData"
#pragma link "Oracle"
#pragma resource "*.dfm"
TFrmCondsEdit *FrmCondsEdit;
//---------------------------------------------------------------------------
__fastcall TFrmCondsEdit::TFrmCondsEdit(TComponent* Owner)
    : TForm(Owner), Cats(NULL), CloseByUser(false), WCCats(NULL) 
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmCondsEdit::NewCatChoose(TFrmCatTree* Ct, int CatId)
{
    QObjs->SetVariable("CATID", CatId);
    QObjs->Refresh();    
};
//---------------------------------------------------------------------------
TFrmCondsEdit::Begin(const AnsiString& SchemaName)
{
    //TODO: Add your source code here
    Caption = "Стандартные операции из "+SchemaName;
    CurrSchemaName= SchemaName;
    QCondItems->Active =false;
    QCondItems->SQL->Text = (AnsiString)
    "select COND_ITEMS.side, COND_ITEMS.setname, " 
    "COND_ITEMS.item_formula, COND_ITEMS.Item_NUM, useracc_new(A.NAME) as NAME1, COND_ITEMS.COND_ID, COND_ITEMS.COND_SUBID, "
    "COND_ITEMS.ROWID "
    "from "+SchemaName+".COND_ITEMS , "+SchemaName+".ACCS A "
    "where " 
    "COND_ITEMS.COND_ID =   :COND_ID    AND "
    "COND_ITEMS.COND_SUBID= :COND_SUBID AND "
    "A.CODE               =  COND_ITEMS.CODE "
    "order by ITEM_NUM";

    
    QConds->Active=false;
    QConds->SQL->Text = (AnsiString) 
    "select ROWID, TEXT, COND_ID, COND_SUBID, PRE_FORMULA "
    "from "+SchemaName+".CONDS "
    "where COND_ID = :COND_ID "
    "order by ORD ";

    QObjs->Active=false;
    QObjs->SQL->Text= (AnsiString)
    "select ROWID, OID, DESCR from OBJS where CATID= :CATID and OTYPE = " +
    IntToStr(CAMS_ObjType_StdOper) + 
    " order by ORD ";

    QObjs->Active=true;
    QConds->Active=true;
    QCondItems->Active=true;

    if (!Cats)
    {
        Application->CreateForm(__classid(TFrmCatTree),&Cats);
    };

    Cats->ShowCategories(SchemaName, CAMS_ObjType_StdOper, NewCatChoose);
    Show();
    return 1;
};
//---------------------------------------------------------------------------
void __fastcall TFrmCondsEdit::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    CloseByUser = true;
    Action = caFree;    

}
//---------------------------------------------------------------------------
void __fastcall TFrmCondsEdit::FormDestroy(TObject *Sender)
{
    if (Cats && CloseByUser)
        Cats->Release();    
}
//---------------------------------------------------------------------------

void __fastcall TFrmCondsEdit::ToolButton2Click(TObject *Sender)
{
    if (QObjs->State == dsEdit  || QObjs->State ==dsInsert)
        QObjs->Post();    
    if (QConds->State == dsEdit  || QConds->State ==dsInsert)
        QConds->Post();    
    if (QCondItems->State == dsEdit  || QCondItems->State ==dsInsert)
        QCondItems->Post();    
}
//---------------------------------------------------------------------------
void __fastcall TFrmCondsEdit::ToolButton1Click(TObject *Sender)
{
    if (Cats)
        Cats->Show();    
}
//---------------------------------------------------------------------------
void __fastcall TFrmCondsEdit::ToolButton3Click(TObject *Sender)
{
   // Копирование проводок в другую бухгалтерию
    if (!DBGridObjs->SelectedRows->Count)
    {
        ErrorMessage("Сначала выберите какие проводки копировать ([Cntrl+Mouse Click])");
        return;
    }
    
    if (!AskString("В какую бухгалтерию копировать?", 30, WhereCopyConds))
    {
        return;
    };
    if (!WCCats)
    {
        Application->CreateForm (__classid(TFrmCatTree),&WCCats);
    }
    Message("Выберите категорию,\r\nв которую необходимо копировать проводки"); 
    WCCats->ShowCategories(WhereCopyConds, -1, WCNewCatChoose);
    //WCCats->Width = 250;
    //WCCats->Height = 300;
    
}
//---------------------------------------------------------------------------
void __fastcall TFrmCondsEdit::WCNewCatChoose(TFrmCatTree* Ct, int CatId)
{
    if (AskMessage("Копировать выделенные проводки в категорию "+
         Ct->GetSelected()+ "?")==ID_YES)
    {
        AnsiString s = "";
        TDataSet *pDS = DBGridObjs->DataSource->DataSet;
        for (int i=0; i < DBGridObjs->SelectedRows->Count; i++)
        {
          pDS->GotoBookmark((void *)DBGridObjs->SelectedRows->Items[i].c_str());
          OQ->SQL->Text="begin "+WhereCopyConds+
          ".InsertObj(:CAT_ID, "+IntToStr(CAMS_ObjType_StdOper)+
          ", '', :DESCR, :KEY,  :RES); commit; end;";
          OQ->DeleteVariables();
          OQ->DeclareVariable("CAT_ID", otInteger);
          OQ->SetVariable("CAT_ID", CatId);
          OQ->DeclareVariable("DESCR", otString);
          OQ->SetVariable("DESCR", pDS->FieldByName("DESCR")->AsString);
          OQ->DeclareVariable("KEY", otInteger);
          OQ->DeclareVariable("RES", otInteger);
          ExecSQL(OQ, "Вставка новой стандартной операции");
          // COND_ID в новой бухгалтерии
          int Res =OQ->GetVariable("KEY");


          // Определяем каких документов нет в бухгалтерии
          OQ->SQL->Text="select NAME, DESCR from "+CurrSchemaName+".OBJS "
           " where NAME in (select distinct DOC_TYPE from "+CurrSchemaName+
           ".CONDS where COND_ID= :COND_ID minus "
           "select NAME from "+WhereCopyConds+".OBJS where OTYPE = "+
            IntToStr(CAMS_ObjType_Doc)+") ";
          OQ->DeleteVariables();
          OQ->DeclareVariable("COND_ID", otInteger);
          OQ->SetVariable("COND_ID", pDS->FieldByName("OID")->AsInteger);
          ExecSQL(OQ, "Проверка существования всех документов");
          if (!OQ->Eof)
          {
            // Вставляем в бухгалтерию все несуществующие документы
            AnsiString S = "В бухгалтерии "+WhereCopyConds+" нет документов:\r\n";
            while (!OQ->Eof)
            {
                S+= OQ->FieldAsString(0)+"\r\n";
                OQ->Next();
            };
            S+= "Добавить?";
            if ( AskMessage (S)==ID_YES)
            {
                OQ->Execute();
                while (!OQ->Eof)
                {
                    OQ2->SQL->Text="begin "+WhereCopyConds+
                    ".InsertObj(:CAT_ID, "+IntToStr(CAMS_ObjType_Doc)+
                    ", :NAME, :DESCR, :KEY,  :RES); commit; end;";
                    OQ2->DeleteVariables();
                    OQ2->DeclareVariable("CAT_ID", otInteger);
                    OQ2->SetVariable("CAT_ID", CatId);
                    OQ2->DeclareVariable("NAME", otString);
                    OQ2->SetVariable("NAME", OQ->FieldAsString (0) );
                    OQ2->DeclareVariable("DESCR", otString);
                    OQ2->SetVariable("DESCR", OQ->FieldAsString (1) );
                    OQ2->DeclareVariable("KEY", otInteger);
                    OQ2->DeclareVariable("RES", otInteger);
                    ExecSQL(OQ2, "Вставка новых документов ");
                    OQ->Next();
                }
            }
          };
          OQ->SQL->Text="begin insert into "+WhereCopyConds+".CONDS "
          "(COND_ID, COND_SUBID, ORD, TEXT, DOC_TYPE, STORNO, AUTO_APPLY, PRE_FORMULA) "
          " select :KEY , COND_SUBID, ORD, TEXT, DOC_TYPE, STORNO, AUTO_APPLY, PRE_FORMULA "
          " from "+CurrSchemaName+".CONDS where COND_ID= :COND_ID; commit; end;";
          OQ->DeleteVariables();
          OQ->DeclareVariable("KEY", otInteger);
          OQ->DeclareVariable("COND_ID", otInteger);
          OQ->SetVariable("KEY", Res);
          OQ->SetVariable("COND_ID",pDS->FieldByName("OID")->AsInteger);
          ExecSQL(OQ, "Копирование CONDS ");

          TFrmAccsTies::TieAccs(CurrSchemaName, WhereCopyConds,
             pDS->FieldByName("OID")->AsInteger, OQ);

          OQ->SQL->Text="begin insert into "+WhereCopyConds+".COND_ITEMS "
          "(COND_ID, COND_SUBID, CODE, SETNAME, SIDE, FOR_POST, ITEM_NUM, ITEM_FORMULA) "
          " select :KEY, COND_SUBID, A.CODE, CI.SETNAME, SIDE, FOR_POST, ITEM_NUM, ITEM_FORMULA "
          " from "+CurrSchemaName+".COND_ITEMS CI, "+
                   CurrSchemaName+".ACCS_TIES ATies, "+
                   WhereCopyConds+".ACCS A, "+
                   CurrSchemaName+".ACCS CA "
          " where "
              " CI.COND_ID = :COND_ID and "
              " CI.CODE = CA.CODE and ATies.BASEUSERNAME = :WHERE_SCHEMA and "
              " CA.NAME = ATies.ACC_NAME and "
              " ATies.ACC_NAME2 = A.NAME; commit; end;" ;  

          OQ->DeleteVariables();
          OQ->DeclareVariable("KEY", otInteger);
          OQ->DeclareVariable("COND_ID", otInteger);
          OQ->DeclareVariable("WHERE_SCHEMA", otString);
          OQ->SetVariable("KEY", Res);
          OQ->SetVariable("COND_ID",pDS->FieldByName("OID")->AsInteger);
          OQ->SetVariable("WHERE_SCHEMA", WhereCopyConds);
          ExecSQL(OQ, "Копирование COND_ITEMS ");

          
          Message(IntToStr(Res));
          s = "";

          //DBGridObjs->SelectedRows->Items[i]=0;
        }
        WCCats->Release();
        WCCats=NULL;
    }
    else 
    {
        Ct->Show();
    }
};
//---------------------------------------------------------------------------

