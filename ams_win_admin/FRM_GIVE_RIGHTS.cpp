//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_GIVE_RIGHTS.h"
#include "MAIN_FRM.h"
#include "AMS_CONST.h"
#include "AMS_UTILS.h"
#include "AMS_EXC.h"
#include "print_error.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Oracle"
#pragma resource "*.dfm"
TFrmGiveRights *FrmGiveRights;
//---------------------------------------------------------------------------
void RightsMaker::FillStringList (const AnsiString& UserName_)
{
    UserName = UserName_;
    if (UserName_=="")
    {
        Grid->ColCount=RightCount+ServiceFields;
        Grid->RowCount=2;
    }
    else
    {
        Grid->ColCount=RightCount+ServiceFields;
        for (int i = 0 ; i < ServiceFields; i++)
            Grid->Cells[i][0]=ServColsName[i];

        for (int i = ServiceFields ; i < ServiceFields+RightCount; i++)
            Grid->Cells[i][0]=ColsName[i- ServiceFields];
        int TotalLines = GetCount();

        Grid->FixedRows =1;
        Grid->RowCount=TotalLines?TotalLines + 1:2;
        int CurrRow=1;
        if (TotalLines)
        {
            PrepareSQL();
            ExecSQL(OQ, "");

            while (!OQ->Eof)
            {
                for (int i=0; i< ServiceFields; i++)
                    Grid->Cells[i][CurrRow]=OQ->FieldAsString(i);
                for (int i = ServiceFields ; i < ServiceFields+RightCount; i++)
                    Grid->Cells[i][CurrRow]=IntToStr(OQ->FieldAsInteger(i));
                CurrRow++;
                OQ->Next();
            }
        }
        Filled = true;
    }
};
//---------------------------------------------------------------------------
void RightsMaker::Apply()
{
    for (unsigned int i = 0; i<ChangedLines.size(); i++)
        ApplyOneRow(ChangedLines[i]);
    OQ->Session->Commit();
};
//---------------------------------------------------------------------------
void RightsMaker::Changing (TStringGrid* Gr, int ChLine)
{
    if (Gr==Grid)
    {
        unsigned int i = 0;
        while (i<ChangedLines.size() && ChangedLines[i] != ChLine)
            i++;
        if (i>=ChangedLines.size())
            ChangedLines.push_back(ChLine);
    }
};
//---------------------------------------------------------------------------
void RightsMaker::ApplyOneRow(int  Row)
{
    OQ->DeleteVariables();
    OQ->DeclareVariable("OID", otInteger);
    OQ->DeclareVariable("NAME", otString);
    OQ->SetVariable("OID", StrToInt(Grid->Cells[0][Row]));
    OQ->SetVariable("Name", UserName);
    AnsiString S="";
    for (int i = 0 ; i < RightCount; i++)
    {
        OQ->DeclareVariable("PR"+IntToStr(i), otInteger);
        int Res =Grid->Cells[ServiceFields+i][Row]=="1";
        OQ->SetVariable("PR"+IntToStr(i), Res);
        S+=", :PR"+IntToStr(i);
    }

    OQ->SQL->Text= "begin "+BaseUserName+".SetOBJPRIV( :OID, :NAME"+S+"); end;" ;
    ExecSQL(OQ, "Выдаем права");
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TRMFuncs::PrepareSQL()
{
    OQ->DeleteVariables();
    OQ->DeclareVariable("OWNER_NAME", otString);
    OQ->SQL->Text="select 0, OBJECT_NAME, OBJECT_TYPE, 0  from ALL_OBJECTS "
        "where OWNER = :OWNER_NAME and "
        "OBJECT_TYPE in ('PROCEDURE', 'FUNCTION', 'PACKAGE') "
        "order by OBJECT_NAME";
    OQ->SetVariable("OWNER_NAME", BaseUserName);
};
//---------------------------------------------------------------------------
int TRMFuncs::GetCount()
{
    OQ->DeleteVariables();
    OQ->DeclareVariable("OWNER_NAME", otString);
    OQ->SQL->Text="select count(*)  from ALL_OBJECTS "
        "where OWNER = :OWNER_NAME and "
        "OBJECT_TYPE in ('PROCEDURE', 'FUNCTION', 'PACKAGE') ";
    OQ->SetVariable("OWNER_NAME", BaseUserName);
    int Res= 0;
    ExecSQL(OQ, "Количество функций");
    if (!OQ->Eof)
        Res=OQ->FieldAsInteger(0);
    return Res;
};
//---------------------------------------------------------------------------
void TRMFuncs::ApplyOneRow(int  Row)
{
    if (Grid->Cells[3][Row]=="1")
    {
        OQ->DeleteVariables();
        OQ->SQL->Text="grant execute on "+BaseUserName+"."+
        Grid->Cells[1][Row]+" to "+UserName;
        ExecSQL(OQ, "Выдаем права");
    }
    else
    if (Grid->Cells[3][Row]=="2")
    {
        try {
        OQ->DeleteVariables();
        OQ->SQL->Text="revoke execute on "+BaseUserName+"."+
        Grid->Cells[1][Row]+" from "+UserName;
        ExecSQL(OQ, "Забираем права");
        }
        catch(...)
        {}
    }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TRMDocs::PrepareSQL()
{
    OQ->DeleteVariables();
    OQ->DeclareVariable("OTYPE", otInteger);
    OQ->DeclareVariable("RUNAME", otString);
    AnsiString S="";
    for (int i = 0; i< RightCount; i++)
        S+=", OP.PR"+IntToStr(i);
    
    OQ->SQL->Text="select O.OID, GetCatFullName(O.CATID)||O.NAME, O.DESCR"+S+
        " from "+BaseUserName+".OBJS O, "+BaseUserName+".OBJPRIV OP, "+
         BaseUserName+".CATTREE CT "
        "where O.OID=OP.OID (+) and OP.RUNAME (+)=:RUNAME and O.OTYPE= :OTYPE "
        " and O.CATID=CT.OWNCAT order by CT.TORD";
    OQ->SetVariable("OTYPE", CAMS_ObjType_Doc);
    OQ->SetVariable("RUNAME", UserName);
};
//---------------------------------------------------------------------------
int TRMDocs::GetCount()
{
    OQ->DeleteVariables();
    OQ->DeclareVariable("OTYPE", otInteger);
    OQ->SQL->Text="select count (*) from "+BaseUserName+".OBJS O " 
        "where O.OTYPE= :OTYPE";
    OQ->SetVariable("OTYPE", CAMS_ObjType_Doc);
    ExecSQL(OQ, "Находим количество строк"); 
    int Res= 0;
    if (!OQ->Eof)
        Res=OQ->FieldAsInteger(0);
    return Res;       
};
//---------------------------------------------------------------------------
void TRMDocs::ApplyOneRow(int  Row)
{
    RightsMaker::ApplyOneRow(Row);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TRMFrms::PrepareSQL()
{
    OQ->DeleteVariables();
    OQ->DeclareVariable("OTYPE", otInteger);
    OQ->DeclareVariable("RUNAME", otString);
    AnsiString S="";
    for (int i = 0; i< RightCount; i++)
        S+=", OP.PR"+IntToStr(i);
    
    OQ->SQL->Text="select O.OID, GetCatFullName(O.CATID)||O.NAME, O.DESCR"+S+" from "+
        BaseUserName+".OBJS O, "+BaseUserName+".OBJPRIV OP, "+
        BaseUserName+".CATTREE CT "
        "where O.OID=OP.OID (+) and OP.RUNAME (+)=:RUNAME and "
        "O.OTYPE= :OTYPE and O.CATID=CT.OWNCAT order by CT.TORD";
    OQ->SetVariable("OTYPE", CAMS_ObjType_Frm);
    OQ->SetVariable("RUNAME", UserName);
};
//---------------------------------------------------------------------------
int TRMFrms::GetCount()
{
    OQ->DeleteVariables();
    OQ->DeclareVariable("OTYPE", otInteger);
    OQ->SQL->Text="select count (*) from "+BaseUserName+".OBJS O " 
        "where O.OTYPE= :OTYPE";
    OQ->SetVariable("OTYPE", CAMS_ObjType_Doc);
    ExecSQL(OQ, "Находим количество строк"); 
    int Res= 0;
    if (!OQ->Eof)
        Res=OQ->FieldAsInteger(0);
    return Res;       
};
//---------------------------------------------------------------------------
void TRMFrms::ApplyOneRow(int  Row)
{
    RightsMaker::ApplyOneRow(Row);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TRMAccs::PrepareSQL()
{
    OQ->DeleteVariables();
    OQ->DeclareVariable("RUNAME", otString);
    AnsiString S="";
    for (int i = 0; i< RightCount; i++)
        S+=", OP.PR"+IntToStr(i);
    
    OQ->SQL->Text="select A.CODE, A.NAME, A.DESCR"+S+
    " from "+BaseUserName+".ACCS A, "+BaseUserName+".OBJPRIV OP "
        "where A.CODE=OP.OID (+) and OP.RUNAME (+)=:RUNAME order by NAME";
    OQ->SetVariable("RUNAME", UserName);
};
//---------------------------------------------------------------------------
int TRMAccs::GetCount()
{
    OQ->DeleteVariables();
    OQ->SQL->Text="select count (*) from "+BaseUserName+".ACCS O " ;
    ExecSQL(OQ, "Находим количество строк"); 
    int Res= 0;
    if (!OQ->Eof)
        Res=OQ->FieldAsInteger(0);
    return Res;       
};
//---------------------------------------------------------------------------
void TRMAccs::ApplyOneRow(int  Row)
{
    RightsMaker::ApplyOneRow(Row);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TRMCats::PrepareSQL()
{
    OQ->DeleteVariables();
    OQ->DeclareVariable("RUNAME", otString);
    AnsiString S="";
    for (int i = 0; i< RightCount; i++)
        S+=", OP.PR"+IntToStr(i);
    
    OQ->SQL->Text="select OC.OWNCAT, GetCatFullName(OC.OWNCAT), OC.DESCR"+S+
    " from "+BaseUserName+".CATTREE OC, "+BaseUserName+".OBJPRIV OP "
        "where OC.OWNCAT=OP.OID (+) and OP.RUNAME (+)=:RUNAME order by TORD";
    OQ->SetVariable("RUNAME", UserName);
};
//---------------------------------------------------------------------------
int TRMCats::GetCount()
{
    OQ->DeleteVariables();
    OQ->SQL->Text="select count (*) from "+BaseUserName+".OBJCAT O " ;
    ExecSQL(OQ, "Находим количество строк"); 
    int Res= 0;
    if (!OQ->Eof)
        Res=OQ->FieldAsInteger(0);
    return Res;       
};
//---------------------------------------------------------------------------
void TRMCats::ApplyOneRow(int  Row)
{
    RightsMaker::ApplyOneRow(Row);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TRMBds::PrepareSQL()
{
    OQ->DeleteVariables();
    OQ->DeclareVariable("OTYPE", otInteger);
    OQ->DeclareVariable("RUNAME", otString);
    AnsiString S="";
    for (int i = 0; i< RightCount; i++)
        S+=", OP.PR"+IntToStr(i);
    
    OQ->SQL->Text="select O.OID, O.NAME, O.DESCR"+S+" from "+BaseUserName+".OBJS O, "+BaseUserName+".OBJPRIV OP "
        "where O.OID=OP.OID (+) and OP.RUNAME (+)=:RUNAME and O.OTYPE= :OTYPE";
    OQ->SetVariable("OTYPE", CAMS_ObjType_DB);
    OQ->SetVariable("RUNAME", UserName);
};
//---------------------------------------------------------------------------
int TRMBds::GetCount()
{
    OQ->DeleteVariables();
    OQ->DeclareVariable("OTYPE", otInteger);
    OQ->SQL->Text="select count (*) from "+BaseUserName+".OBJS O " 
        "where O.OTYPE= :OTYPE";
    OQ->SetVariable("OTYPE", CAMS_ObjType_DB);
    ExecSQL(OQ, "Находим количество строк"); 
    int Res= 0;
    if (!OQ->Eof)
        Res=OQ->FieldAsInteger(0);
    return Res;       
};
//---------------------------------------------------------------------------
void TRMBds::ApplyOneRow(int  Row)
{
    OQ->DeleteVariables();
    OQ->DeclareVariable("BASE_USERNAME", otString); 
    OQ->SetVariable("BASE_USERNAME", UserName);
    OQ->DeclareVariable("OID", otInteger); 
    OQ->SetVariable("OID", Grid->Cells[0][Row].ToInt());
    OQ->SQL->Text ="select PRIVILEGE "
          " from ALL_TAB_PRIVS " 
          " where TABLE_SCHEMA = :BASE_USERNAME and "
          " TABLE_NAME = (select NAME from OBJS where OID = :OID) ";

    ExecSQL(OQ,"Выбираем какие привелегии есть на таблицу");
    AnsiString S ="";
    AnsiString TN = "";

    while (!OQ->Eof)
    {
        if (S!="")
            S+=", ";
        S+= OQ->FieldAsString(0);
        OQ->Next();    
    }      

    OQ->DeleteVariable("BASE_USERNAME");
    OQ->SQL->Text = "select NAME from OBJS where OID = :OID";
    ExecSQL(OQ, "");
    if (!OQ->Eof)
        TN = OQ->FieldAsString(0);

    if (S!="")
    {
        OQ->DeleteVariables();
        OQ->SQL->Text="revoke "+S+" on "+TN+" from "+UserName;
        ExecSQL(OQ, "");    
    }

    S="";
    if (Grid->Cells[ServiceFields][Row]=="1")
        S+="select";
    if (Grid->Cells[ServiceFields+1][Row]=="1")
    {
        if (S!="") 
            S+=", ";
        S+="update";
    }    
    if (Grid->Cells[ServiceFields+2][Row]=="1")
    {
        if (S!="") 
            S+=", ";
        S+="insert";
    }    
    if (Grid->Cells[ServiceFields+3][Row]=="1")
    {
        if (S!="") 
            S+=", ";
        S+="delete";
    }    

    if (S!="")
    {
        OQ->DeleteVariables();
        OQ->SQL->Text = "grant "+S+" on "+TN+" to "+UserName;
        ExecSQL(OQ, "Выдаем права");
    }
    RightsMaker::ApplyOneRow(Row);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TRMAns::PrepareSQL()
{
    OQ->DeleteVariables();
    OQ->DeclareVariable("OTYPE", otInteger);
    OQ->DeclareVariable("RUNAME", otString);
    AnsiString S="";
    for (int i = 0; i< RightCount; i++)
        S+=", OP.PR"+IntToStr(i);
    
    OQ->SQL->Text="select O.OID, O.NAME, O.DESCR"+S+" from "+BaseUserName+".OBJS O, "+BaseUserName+".OBJPRIV OP "
        "where O.OID=OP.OID (+) and OP.RUNAME (+)=:RUNAME and O.OTYPE= :OTYPE";
    OQ->SetVariable("OTYPE", CAMS_ObjType_Anal);
    OQ->SetVariable("RUNAME", UserName);
};
//---------------------------------------------------------------------------
int TRMAns::GetCount()
{
    OQ->DeleteVariables();
    OQ->DeclareVariable("OTYPE", otInteger);
    OQ->SQL->Text="select count (*) from "+BaseUserName+".OBJS O " 
        "where O.OTYPE= :OTYPE";
    OQ->SetVariable("OTYPE", CAMS_ObjType_DB);
    ExecSQL(OQ, "Находим количество строк"); 
    int Res= 0;
    if (!OQ->Eof)
        Res=OQ->FieldAsInteger(0);
    return Res;       
};
//---------------------------------------------------------------------------
void TRMAns::ApplyOneRow(int  Row)
{
    OQ->DeleteVariables();
    OQ->DeclareVariable("BASE_USERNAME", otString); 
    OQ->SetVariable("BASE_USERNAME", UserName);
    OQ->DeclareVariable("OID", otInteger); 
    OQ->SetVariable("OID", Grid->Cells[0][Row].ToInt());
    OQ->SQL->Text ="select PRIVILEGE "
          " from ALL_TAB_PRIVS " 
          " where TABLE_SCHEMA = :BASE_USERNAME and "
          " TABLE_NAME = (select NAME from OBJS where OID = :OID) ";

    ExecSQL(OQ,"Выбираем какие привелегии есть на таблицу");
    AnsiString S ="";
    AnsiString TN = "";

    while (!OQ->Eof)
    {
        if (S!="")
            S+=", ";
        S+= OQ->FieldAsString(0);
        OQ->Next();    
    }      

    OQ->DeleteVariable("BASE_USERNAME");
    OQ->SQL->Text = "select NAME from OBJS where OID = :OID";
    ExecSQL(OQ, "");
    if (!OQ->Eof)
        TN = OQ->FieldAsString(0);
    
    if (S!="")
    {
        OQ->DeleteVariables();
        OQ->SQL->Text="revoke "+S+" on "+TN+" from "+UserName;
        ExecSQL(OQ, "");    
    }

    S="";
    if (Grid->Cells[ServiceFields][Row]=="1")
        S+="select";
    if (Grid->Cells[ServiceFields+1][Row]=="1")
    {
        if (S!="") 
            S+=", ";
        S+="update";
    }    
    if (Grid->Cells[ServiceFields+2][Row]=="1")
    {
        if (S!="") 
            S+=", ";
        S+="insert";
    }    
    if (Grid->Cells[ServiceFields+3][Row]=="1")
    {
        if (S!="") 
            S+=", ";
        S+="delete";
    }    

    if (S!="")
    {
        OQ->DeleteVariables();
        OQ->SQL->Text = "grant "+S+" on "+TN+" to "+UserName;
        ExecSQL(OQ, "Выдаем права");
    }
    RightsMaker::ApplyOneRow(Row);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TRMOpers::PrepareSQL()
{
    OQ->DeleteVariables();
    OQ->DeclareVariable("OTYPE", otInteger);
    OQ->DeclareVariable("RUNAME", otString);
    AnsiString S="";
    for (int i = 0; i< RightCount; i++)
        S+=", OP.PR"+IntToStr(i);
    
    OQ->SQL->Text="select O.OID, GetCatFullName(O.CATID), O.DESCR"+S+" from "+
        BaseUserName+".OBJS O, "+BaseUserName+".OBJPRIV OP, "+
        BaseUserName+".CATTREE CT "
        "where O.OID=OP.OID (+) and OP.RUNAME (+)=:RUNAME and O.OTYPE= :OTYPE and"
        " O.CATID = CT.OWNCAT order by CT.TORD";
    OQ->SetVariable("OTYPE", CAMS_ObjType_StdOper);
    OQ->SetVariable("RUNAME", UserName);
};
//---------------------------------------------------------------------------
int TRMOpers::GetCount()
{
    OQ->DeleteVariables();
    OQ->DeclareVariable("OTYPE", otInteger);
    OQ->SQL->Text="select count (*) from "+BaseUserName+".OBJS O " 
        "where O.OTYPE= :OTYPE";
    OQ->SetVariable("OTYPE", CAMS_ObjType_StdOper);
    ExecSQL(OQ, "Находим количество строк"); 
    int Res= 0;
    if (!OQ->Eof)
        Res=OQ->FieldAsInteger(0);
    return Res;       
};
//---------------------------------------------------------------------------
void TRMOpers::ApplyOneRow(int  Row)
{
    RightsMaker::ApplyOneRow(Row);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TFrmGiveRights::TFrmGiveRights(TComponent* Owner)
    : TForm(Owner), Dirty(false)
{
    B1= new Graphics::TBitmap();
    B2= new Graphics::TBitmap();
    B3= new Graphics::TBitmap();
    B4= new Graphics::TBitmap();
    MainFrm -> ImageList1->GetBitmap(30, B1);
    MainFrm -> ImageList1->GetBitmap(29, B2);
    MainFrm -> ImageList1->GetBitmap(32, B3);
    MainFrm -> ImageList1->GetBitmap(31, B4);
    RM[0]=&RCats;
    RM[1]=&RAccs;
    RM[2]=&RDocs;
    RM[3]=&RFrms;
    RM[4]=&RBds;
    RM[5]=&RAns;
    RM[6]=&ROpers;
    RM[7]=&RFuncs;
}
//---------------------------------------------------------------------------
void __fastcall TFrmGiveRights::BBCancelClick(TObject *Sender)
{
    bool Ok = false;
    if (Dirty)
    {
        if (AskMessage("Были сделаны изменения, выйти без сохранения?")==ID_YES)
            Ok = true;
    }
    else
        Ok =true;
    if (Ok)
        Hide();
}
//---------------------------------------------------------------------------
void __fastcall TFrmGiveRights::BBOkClick(TObject *Sender)
{
    for (int i = 0; i<ObjCol; i++)
        RM[i]->Apply();
    Dirty= false;
}
//-------------------------------------------------------------------------

void __fastcall TFrmGiveRights::SGCatDrawCell(TObject *Sender,
      int ACol, int ARow, TRect &Rect, TGridDrawState State)
{

  TStringGrid* Grid = dynamic_cast<TStringGrid*> (Sender);
  if (ACol >=ServiceFields && ARow > 0)
  {
    Graphics::TBitmap* BM= NULL;
    bool WriteIt = Grid->Cells[ACol][ARow]=="1";
    if (WriteIt)
    {
        if (State.Contains(gdSelected))
        {
          Grid->Canvas->Brush->Color = clActiveCaption;
          BM=B1;
        }
        else
        {
          Grid->Canvas->Brush->Color = Grid->Color;
          BM=B2;
        }
    }
    else
    {
        if (State.Contains(gdSelected))
          Grid->Canvas->Brush->Color = clActiveCaption;
        else
          Grid->Canvas->Brush->Color = Grid->Color;
    }
    Grid->Canvas->FillRect(Rect);
    if (BM) Grid->Canvas->Draw( Rect.left, Rect.top, BM);
  }
  //else Grid->DefaultDrawColumnCell(Rect,DataCol,Column,State);
}
//---------------------------------------------------------------------------
void __fastcall TFrmGiveRights::SGCatDblClick(TObject *Sender)
{
    TStringGrid* Grid = dynamic_cast<TStringGrid*> (Sender);
    if (Grid->Col>=ServiceFields && Grid->Row > 0)
    {
        Dirty=true;
        if (Grid->Cells[Grid->Col][Grid->Row] == "1")
            Grid->Cells[Grid->Col][Grid->Row]="0";
        else
            Grid->Cells[Grid->Col][Grid->Row]="1";
        for (int i = 0; i<ObjCol; i++)
            RM[i]->Changing(Grid, Grid->Row);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmGiveRights::FormDestroy(TObject *Sender)
{
    delete B1;
    delete B2;
    delete B3;
    delete B4;
}
//---------------------------------------------------------------------------
void __fastcall TFrmGiveRights::CBUserNameChange(TObject *Sender)
{
    if (Dirty)
    {
        if (!AskMessage("Были сделаны изменения, продолжать без сохранения?")==ID_YES)
        {
            BBOkClick(Sender);
        }
    }
    Dirty=false;
    for (int i = 0; i< ObjCol; i++)
        Loaded [i]=false;
    GrName = CBUserName->Text;
    PCChange(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFrmGiveRights::ChBUsersClick(TObject *Sender)
{
    CBUserName->Items->Clear();
    if (ChBRoles->Checked )
    {
        OQ->DeleteVariables();
        OQ->SQL->Text="select ROLE from DBA_ROLES";
        ExecSQL(OQ, "");
        while(!OQ->Eof)
        {
            CBUserName->Items->Add(OQ->FieldAsString(0));
            OQ->Next();    
        };
    }
    if (ChBUsers->Checked )
    {
        CBUserName->Items->Clear();
        OQ->DeleteVariables();
        OQ->SQL->Text="select USERNAME from DBA_USERS";
        ExecSQL(OQ, "");
        while(!OQ->Eof)
        {
            CBUserName->Items->Add(OQ->FieldAsString(0));
            OQ->Next();    
        };
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmGiveRights::PCChange(TObject *Sender)
{
    int i = 0;
    if (PC->ActivePage == TSCat)
        i=0;
    else     
    if (PC->ActivePage == TSAcc)
        i=1;
    else     
    if (PC->ActivePage == TSDoc)
        i=2;
    else     
    if (PC->ActivePage == TSFrm)
        i=3;
    else     
    if (PC->ActivePage == TSBd)
        i=4;
    else     
    if (PC->ActivePage == TSAn)
        i=5;
    else     
    if (PC->ActivePage == TSOper)
        i=6;
    else
    if (PC->ActivePage == TSFuncs)
        i=7;

    if (!Loaded[i])
    {
        RM[i]->FillStringList (GrName);
        Loaded[i]=true;        
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmGiveRights::FormCreate(TObject *Sender)
{
    RCats.Init(OQ, SGCat);
    RAccs.Init(OQ, SGAcc);
    RDocs.Init(OQ, SGDoc);
    RFrms.Init(OQ, SGFrm);
    RBds.Init(OQ, SGBd);
    RAns.Init(OQ, SGAn);
    ROpers.Init(OQ, SGOpers);
    RFuncs.Init(OQ, SGFuncs);
}
//---------------------------------------------------------------------------



void __fastcall TFrmGiveRights::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Action= caFree;    
}
//---------------------------------------------------------------------------

void __fastcall TFrmGiveRights::SGFuncsDblClick(TObject *Sender)
{
    TStringGrid* Grid = dynamic_cast<TStringGrid*> (Sender);
    if (Grid->Col>=ServiceFields && Grid->Row > 0)
    {
        Dirty=true;
        if (Grid->Cells[Grid->Col][Grid->Row] == "1")
            Grid->Cells[Grid->Col][Grid->Row]="2";
        else
        if (Grid->Cells[Grid->Col][Grid->Row] == "2")
            Grid->Cells[Grid->Col][Grid->Row]="0";
        else
            Grid->Cells[Grid->Col][Grid->Row]="1";
        for (int i = 0; i<ObjCol; i++)
            RM[i]->Changing(Grid, Grid->Row);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmGiveRights::SGFuncsDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
  TStringGrid* Grid = dynamic_cast<TStringGrid*> (Sender);
  if (ACol >=ServiceFields && ARow > 0)
  {
    Graphics::TBitmap* BM= NULL;
    int WriteIt = Grid->Cells[ACol][ARow].ToIntDef(0);
    if (WriteIt==2)
    {
        if (State.Contains(gdSelected))
        {
          Grid->Canvas->Brush->Color = clActiveCaption;
          BM=B3;
        }
        else
        {
          Grid->Canvas->Brush->Color = Grid->Color;
          BM=B4;
        }
    }
    else
    if (WriteIt==1)
    {
        if (State.Contains(gdSelected))
        {
          Grid->Canvas->Brush->Color = clActiveCaption;
          BM=B1;
        }
        else
        {
          Grid->Canvas->Brush->Color = Grid->Color;
          BM=B2;
        }
    }
    else
    {
        if (State.Contains(gdSelected))
          Grid->Canvas->Brush->Color = clActiveCaption;
        else
          Grid->Canvas->Brush->Color = Grid->Color;
    }
    Grid->Canvas->FillRect(Rect);
    if (BM) Grid->Canvas->Draw( Rect.left, Rect.top, BM);
  }
}
//---------------------------------------------------------------------------

