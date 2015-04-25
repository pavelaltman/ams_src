//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop
#include "FLDS_DESCR_DISTR.h"
#include "utils.h"
#include "ams_utils.h"
#include "FRM_INPUT_OPER.h"
#include "FRM_BROWS_AN.h"
#include "FRM_CHOOSE_OBJ_MODAL.h"
#include "ADD_MODULE.h"
#include "amsclnt.h"
#include "FRM_EDIT_DB_POS_ONLY.h"
#include "FRM_BROWS_AN_MODAL.h"
#include "FRM_CHOOSE_STD_OPER_MODAL.h"
#include "FRM_INPUT_SUM_NM.h"
#include "FRM_ASK_ACC_FIRST.h"
#include "REFRESH_ACCS.h"
#include "FRM_ASK_DK.h"
#include "ams_win_c.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "OracleData"
#pragma link "RXDBCtrl"
#pragma link "ToolEdit"
#pragma link "RxDBComb"
#pragma link "Oracle"
#pragma resource "*.dfm"
#define CodeOperLen 5
#define ShLen 5

TFrmInputOper *FrmInputOper;

bool Test1 = false;
extern int cur_arg ;
//---------------------------------------------------------------------------
__fastcall TFrmInputOper::TFrmInputOper(TComponent* Owner)
    : TForm(Owner), FrmEditAn(0), FrmAskAccCode(0),FrmEditDbPosOnly(0),
        SecondStep(false), DeepCopy(false),FrmAskAccFirst(0), IsReadTiedBase(false),
        CM_Var(-1), CurrAcc(0), MiceOverClose(false)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmInputOper::BBCancelClick(TObject *Sender)
{
    Close() ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmInputOper::BBOkClick(TObject *Sender)
{
 if (readonly)
 {
    Close();       
 }
 else
    HandleOper(0) ;
}
//---------------------------------------------------------------------------
int TFrmInputOper::HandleOper(int post)
{
 DeepCopy=false;
 int res ;
 AnsiString errm ;

 if (DSet->State!=dsEdit)
 {
    DSet->Edit();
 }
 DSetPOST->Value=post;
 DSet->Post() ;
 if (DSetItems->State==dsEdit || DSetItems->State==dsInsert) DSetItems->Post() ;

 ShortMessage("Обрабатывается операция...");

 if (!newop)
 {
  AmsQuery Q ;
  Q.SetPLText("*.bookkern.UpdateOper(:OLD_ID,:OLD_SUBID,:NEW_ID,:NEW_SUBID,:RESULT);") ;
  Q.DeclVar("OLD_ID",old_id) ;
  Q.DeclVar("OLD_SUBID",old_subid) ;
  Q.DeclVar("NEW_ID",op_id) ;
  Q.DeclVar("NEW_SUBID",op_subid) ;
  Q.DeclVarInt("RESULT") ;
  Q.Exec("update oper") ;
  res=Q.GetVarInt("RESULT") ;
 }
 else
 {
  AmsQuery Q ;

  Q.SetPLText("*.bookkern.ApplyOper(:OP_ID,:OP_SUBID,:RESULT,:ERRM);") ;
  Q.DeclVar("OP_ID",op_id) ;
  Q.DeclVar("OP_SUBID",op_subid) ;
  Q.DeclVarInt("RESULT") ;
  Q.DeclVarStr("ERRM") ;
  Q.Exec("apply oper") ;
  res=Q.GetVarInt("RESULT") ;
  errm=Q.GetVarStr("ERRM") ;
 }

 ShortMessage("");

 DatMod->OSession->Commit() ;

 if (res!=0)
 {
  AnalyzeRes(res,(char*)errm.data()) ;
  if (res!=R_NEGREMSOK) return 0 ;
 }

 // Если мы отложили операцию, то необходимо удалить все последующие операции
 // и не проводить далее
 if (post)
 {

    if (!newop && CondId)
    {
        AmsQuery CAQ(CondOQ);
        std::vector<int> MoreSubId;
        CAQ.Next();
        while (CAQ.NotEof())
        {
            MoreSubId.push_back(CAQ.GetFldInt("COND_SUBID"));
            CAQ.Next();
        }
        for (unsigned int i=MoreSubId.size();i!=0;i--)
        {
            try {
                DelOper(old_id, MoreSubId[i-1]);
            }
            catch(...)
            {
            }
        }
        // ОБНОВЛЯЕМ ВСЕ ОКНА С ДАННЫМИ СЧЕТАМИ
        //AccsToWin.Refresh();
    }

    InputOperEnd=true;
    ok=1;
    AccsToWin.Refresh(true);

    Close() ;
    return 1 ;
 }

 ok=1;
 // ОБНОВЛЯЕМ ВСЕ ОКНА С ДАННЫМИ СЧЕТАМИ
 AccsToWin.Refresh();
 do {
    AmsQuery Q ;
     // Проверяем есть ли следующие опрерации и если есть то начинаем вводить их
     Q.SetPLText("*.CANCEL_EDIT_OPER(:OK,:NEWOP,:READONLY,:OP_ID,:OP_SUBID,:OLD_ID,:OLD_SUBID);") ;
     Q.DeclVarInt("OK",ok) ;
     Q.DeclVarInt("NEWOP",newop) ;
     Q.DeclVarInt("READONLY",readonly) ;
     Q.DeclVarInt("OP_ID",op_id) ;
     Q.DeclVarInt("OP_SUBID",op_subid) ;
     Q.DeclVarInt("OLD_ID",old_id) ;
     Q.DeclVarInt("OLD_SUBID",old_subid) ;
     Q.Exec("Cancel oper") ;
 }while (0);

 if (CondId)
 {
    DocNum=DSetDOC_NUM->Value;
    OpDate=(int) DSetOP_DATE->Value;
    SecondStep=true;
    AmsQuery CAQ(CondOQ);
    if (CAQ.NotEof())
    {
        CAQ.Next();
        if (CAQ.NotEof())
        {
            CondOrd=CAQ.GetFldInt("ORD");
            PredFormula=CAQ.GetFldStr("PRE_FORMULA");
            text_edit->SetFocus();
            if (newop)
            {
                old_id=op_id;
            }

            Init(old_id, CAQ.GetFldInt("COND_SUBID"), del, readonly, CondId);
            return 1;
        }
    }
 }
 ok=1;
 InputOperEnd=true;
 Close() ;
 return 1 ;
}
//-----------------------------------------------------------------------
void TFrmInputOper::Init(int _op_id, int _op_subid, int _del, int _readonly,
                         int CondId_ )
{
    // Узнаем состояние проводки до редактирования
    NumberOut=false;
    PrevState=osNew;
    if (_op_id)
    {
        AmsQuery AQ;
        AQ.SetSqlText(
            "select POST from *.OPERS where OP_ID=:OP_ID and OP_SUBID=:OP_SUBID");
        AQ.DeclVar("OP_ID", _op_id);
        AQ.DeclVar("OP_SUBID", _op_subid);
        AQ.Exec("Состояние проводки до редактирования");
        if (AQ.NotEof())
        {
            PrevState=AQ.GetFldInt(0)?osWasPosted:osWasCommited;
        }
    };

    Dirty = false;
 InputOperEnd=false;
 LocalVarsFormed=false;
 del=_del ;
 readonly=_readonly || GC::ReadOnly;
 ok=0;
 op_subid=_op_subid;

 if (readonly)
 {
    text_edit->Enabled=false;
    DBDateEdit1->Enabled=false;
    DBEdit1->Enabled=false;
    DBEdit2->Enabled=false;
    BitBtn1->SetFocus();
 }


 AutoApply=-1;
 AutoGen=-1;
 AutoInput=-1;

 AmsQuery CAQ ( CondOQ);


 if (!SecondStep)
 {
    if (_op_id)
    {
        // Узнаем COND_ID
        AmsQuery AQ;
        AQ.SetSqlText(
            "begin "
            " declare vCondId number (8); "
            "begin "
            "   select max (COND_ID) into vCondId from *.OPERS "
            "   where OP_ID=:OP_ID and OP_SUBID=:OP_SUBID;"
            "   if vCondId is not NULL then "
            "       :COND_ID := vCondId; "
            "       select max (ORD) into :ORD from *.CONDS where "
            "       COND_ID=vCondId and COND_SUBID=:OP_SUBID;"
            "   else "
            "       vCondId:=0; "
            "   end if;"
            "end;"
            "end;");
        AQ.DeclVar("OP_ID", _op_id);
        AQ.DeclVar("OP_SUBID", _op_subid);
        AQ.DeclVar("COND_ID", 0);
        AQ.DeclVar("ORD", 0);
        AQ.Exec("Код станд операции и еe порядок");
        CondOrd=AQ.GetVarInt("ORD");
        CondId=AQ.GetVarInt("COND_ID");
    }
    else
    {
        CondId=CondId_;
        CondOrd=-1;
    }

    newop=0;
    if (CondId)
    {
        CAQ.SetSqlText(
            "select * from *.CONDS where COND_ID=:COND_ID and ORD>=:ORD"
            " order by ORD");
        CAQ.DeclVar("COND_ID", CondId);
        CAQ.DeclVar("ORD", CondOrd);
        CAQ.Exec("Стандартные проводки");
        PredFormula= CAQ.GetFldStr("PRE_FORMULA");
        if (!_op_id)
        {
            if (CAQ.NotEof())
            {
                op_subid=CAQ.GetFldInt("COND_SUBID");
                CondOrd=CAQ.GetFldInt("ORD");
            }
        }
    }
 }
 else // Уже были во вводе
 {
    //Проверяем есть ли операция (может быть новая операция)
    AmsQuery AQ ;
    AQ.SetSqlText("select 1 from *.OPERS where OP_ID=:OP_ID and OP_SUBID=:OP_SUBID");
    AQ.DeclVar("OP_ID", old_id);
    AQ.DeclVar("OP_SUBID", _op_subid);
    AQ.Exec("Определяем была ли введена операция");
    if (AQ.NotEof())
    {
        newop=0;
    }
    else
    {
        newop=1;
        op_id=old_id;
        op_subid=_op_subid;
        old_subid=_op_subid;
    }
 }

 if (_op_id==0 || newop) // Ввод новой операции
 {
    if (!SecondStep)
        TiedBaseArticle="";
    newop=1 ;
    Caption="Ввод операции" ;
 }
 else // Редактирование или просмотр операции
 {
    newop=0 ;
    if (readonly)
    {
        op_id=_op_id ;
        op_subid=_op_subid;

        DSet->ReadOnly=true;
        DSetItems->ReadOnly=true;

        BBOk->Visible=false;
        BBPost->Visible=false;

        DBEdit2->Enabled=false;
        ButBegin->Enabled=false;

        Caption="Просмотр операции" ;
    }
    else   // Редактирование операции
    {
        if (!SecondStep)
        {
            old_id=_op_id ;
            old_subid=_op_subid ;
            op_id=NewOpId() ;
            op_subid=old_subid ;
        }
        else
        {
            old_subid=_op_subid ;
            op_subid=old_subid ;
        }
        // Копирование операции
        AmsQuery Q ;
        Q.DeclVar("OLD_ID",old_id) ;
        Q.DeclVar("OLD_SUBID",old_subid) ;
        Q.DeclVar("NEW_ID",op_id) ;
        Q.DeclVar("NEW_SUBID",op_subid) ;
        Q.SetPLText("*.bookkern.CopyOper(:OLD_ID,:OLD_SUBID,:NEW_ID,:NEW_SUBID,3);") ;
        Q.Exec("Copy oper") ;

        Caption="Редактирование операции" ;
        AddNewAnalSum=false;
    }
 }  // Редактирование или просмотр

 DatMod->OSession->Commit();

 DSet->SetVariable("OP_ID",op_id) ;
 DSet->SetVariable("OP_SUBID",op_subid) ;
 DSetItems->SetVariable("OP_ID",op_id) ;
 DSetItems->SetVariable("OP_SUBID",op_subid) ;

 AnsiString tname= del ? "del_opers" : "opers" ,
            tiname= del ? "del_op_items" : "op_items" ,
            delstr= del ? "Del" : "" ;


 DSet->SQL->Text=" SELECT o.rowid,o.* FROM "+BaseUserName+"."+tname+
                 " o WHERE OP_ID=:OP_ID AND OP_SUBID=:OP_SUBID FOR UPDATE" ;

 DSetItems->SQL->Text="select i.rowid, i.*, "+BaseUserName+".GetDCLetter(side) AS ASIDE,"+
         BaseUserName+".User_Acc(a.name) AS ANAME, "+
         BaseUserName+".AnalInfo(op_id,op_subid,i.code) AS ANALINFO, VALUE, ANAL_ID "
         "from "+BaseUserName+"."+tiname+" i,"+BaseUserName+".accs a, "+BaseUserName+".sums "
         "where op_id = :op_id and op_subid = :op_subid AND i.code=a.code "
         "AND a.node=0 AND sum_id=item_sum_id "
         "ORDER BY ASIDE, A.NAME" ;

 DSet->Active=false ;
 DSetItems->Active=false ;
 DSet->Active=true ;
 DSetItems->Active=true ;
 AnsiString DType;

 if (newop)
 {
    if (!op_id)
        op_id=NewOpId() ;
    //op_subid=_op_subid ;

    DSet->SetVariable("OP_ID",op_id) ;
    DSet->SetVariable("OP_SUBID",op_subid) ;
    DSetItems->SetVariable("OP_ID",op_id) ;
    DSetItems->SetVariable("OP_SUBID",op_subid) ;
    if (SecondStep)
    {
        DSetItems->Refresh();
    }

    TDate OpDate= GetInpDate();
    if (CondId)
    {
        AmsQuery AQ;
        AQ.SetSqlText(
            "select * from *.CONDS "
            "where COND_ID=:COND_ID and "
            "COND_SUBID=:COND_SUBID");
        AQ.DeclVarInt("COND_ID", CondId);
        AQ.DeclVarInt("COND_SUBID", op_subid);
        AQ.Exec("Выбор стандартной операции");

        DSet->Insert() ;
        if (AQ.NotEof())
        {
            DSetTEXT->Value=AQ.GetFldStr("TEXT");
            DSetPOST->Value=AQ.GetFldInt("FOR_POST");
            DSetSTORNO->Value=AQ.GetFldInt("STORNO");
            DType= AQ.GetFldStr("DOC_TYPE");
            PredFormula = AQ.GetFldStr("PRE_FORMULA");
            DSetOP_DATE->Value= (int)GetInpDate();
            DSetOP_ID->Value=op_id ;
            DSetOP_SUBID->Value=op_subid ;
            DSetCOND_ID->Value=CondId;
            AutoApply=AQ.GetFldInt("AUTO_APPLY");
            AutoGen=AQ.GetFldInt("AUTO_GEN");
            AutoInput=AQ.GetFldInt("AUTO_INPUT");
        }
    }
    else
    {
        DSet->Insert() ;
        DSetOP_ID->Value=op_id ;
        DSetOP_SUBID->Value=op_subid ;
        DSetSTORNO->Value=0;
    }
    DSet->Post();
    DatMod->OSession->Commit();
 }
 else
 {
    if (!SecondStep)
    {
        if (!DSetCOND_ID->IsNull)
        {
            CondId =DSetCOND_ID->Value;
        }
    }
 }

 if (!newop && CondId)
 {
    AmsQuery AQ;
    AQ.SetSqlText(
        "select nvl(AUTO_APPLY,0), nvl(AUTO_GEN, 0), nvl(AUTO_INPUT,0), PRE_FORMULA "
        "from *.CONDS where COND_ID=:COND_ID and COND_SUBID=:COND_SUBID");
    AQ.DeclVar ("COND_ID", CondId);
    AQ.DeclVar ("COND_SUBID", op_subid);
    AQ.Exec("Auto выбор");
    if (AQ.NotEof())
    {
        AutoApply   =AQ.GetFldInt(0);
        AutoGen     =AQ.GetFldInt(1);
        AutoInput   =AQ.GetFldInt(2);
        PredFormula =AQ.GetFldStr(3);
    }
 }

 /*
 if (CondId && !SecondStep)
 {
    AmsQuery AQ;
    AQ.SetPLText(
        "select max(COND_SUBID) into :COND_SUBID from *.CONDS "
        "where COND_ID=:COND_ID;");
    AQ.DeclVar("COND_ID", CondId);
    AQ.DeclVar("COND_SUBID",0);
    AQ.Exec("Находим количество подпроводок");
    TotalSubId=AQ.GetVarInt("COND_SUBID");
 }
 */
 if (!readonly) DSet->Edit() ;

 if (DType!="")
    DSetDOC_TYPE->Value=DType;
 if (SecondStep && newop)
 {
     DSetDOC_NUM->Value = DocNum;
     DSetDOC_NUM_V->Value=0;
     DSetOP_DATE->Value=(int) OpDate;
     DBEdit2Enter(this);
 }
 if (GC::ViewDebug)
    Caption=Caption+" OP_ID="+IntToStr(op_id)+" OP_SUBID="+IntToStr(op_subid) ;


    if (SecondStep)
        AutoInput=1;

    if (!newop || readonly)
    {
        AutoInput=0;
    }
    
    if (AutoInput==1)
    {
        int Result =0;
        // Пытаемся сгенерировать номер, если стоит автонумерация
        do {
        AmsQuery AQ;
        AQ.SetSqlText(
            "begin *.bookkern.ProcessDocNum(:OP_ID, :OP_SUBID, :OLD_ID, :OLD_SUBID, "
            ":OP_DATE, :DOC_TYPE, :DOC_NUM, :DOC_NUM_V, :NO_CHECK_DUP, "
            ":RESULT); commit; end;");
        AQ.DeclVarInt("OP_ID", op_id);
        AQ.DeclVarInt("OP_SUBID", op_subid);
        if (newop)
        {
            AQ.DeclVarInt("OLD_ID");
            AQ.DeclVarInt("OLD_SUBID");
        }
        else
        {
            AQ.DeclVarInt("OLD_ID",    old_id);
            AQ.DeclVarInt("OLD_SUBID", old_subid);
        }
        AQ.DeclVarDate("OP_DATE",   DBDateEdit1->Date);
        AQ.DeclVarStr ("DOC_TYPE",  DBEdit1->Text);
        AQ.DeclVarStr ("DOC_NUM",   DBEdit2->Text);
        AQ.DeclVarInt ("DOC_NUM_V", DSetDOC_NUM_V->Value);
        // !!!!!!!!!!! Сложное условие если пользователь не изменил ничего
        AQ.DeclVarInt ("NO_CHECK_DUP", 0);
        AQ.DeclVarInt("RESULT");
        AQ.Exec("");

        int Res=AQ.GetVarInt("RESULT");
        switch (Res)
        {
            case 0:
            case 10:
                Result=1;
                DSet->Edit();
                DSetDOC_NUM->Value=AQ.GetVarStr("DOC_NUM");
                DSetDOC_NUM_V->Value=AQ.GetVarInt("DOC_NUM_V");
                DSet->Post();
            break;
            case 1:
            case 2:
                ErrorMessage("Неправильный тип документа.\r\n Выберите документ ч/з Alt+В");
            break;
            case 3:
                ErrorMessage("Такой номер уже существует, а дублирование запрещено");
            break;
            case 4:
            break;
            default:
                ErrorMessage("Неожиданный результат. Свяжитесь с АМС.");
            break;
        }
        } while (0);
        if (Result)
        {
            ButBeginClick(this);
        }
    }
    if (readonly || !newop)
    {
        // Для просмотра сразу получаем имя связанной БД
        ReadTiedBase();
        if (TiedBase!="")
        {
            ShowTiedDbShortInfo();
        }
    }
}
//---------------------------------------------------------------------------
int NewOpId()
{
 AmsQuery Q ;
 Q.SetPLText("SELECT *.OPERSEQ.NEXTVAL INTO :ID FROM DUAL;") ;
 Q.DeclVarInt("ID") ;
 Q.Exec("Oper seq") ;
 return Q.GetVarInt("ID") ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmInputOper::FormCreate(TObject *Sender)
{
 text_edit->Width=ClientWidth-3 ;
 LabDbInfo->Width=text_edit->Width;
 storno_check->Left=ClientWidth-3-storno_check->Width ;
 items_grid->Width=ClientWidth-3 ;
 BBCancel->Left=ClientWidth-3-BBCancel->Width ;
 BBCancel->Top=ClientHeight-5-BBCancel->Height ;
 BitBtn1->Top=BBCancel->Top;
 BitBtn1->Left=BBCancel->Left-(BitBtn1->Width+10);
 BBPost->Top=BBCancel->Top ;
 BBOk->Top=BBCancel->Top ;
 items_grid->Height=BBOk->Top-items_grid->Top-10;
 FormResize(this);
}
//---------------------------------------------------------------------------
void TFrmInputOper::CloseForm()
{
 AmsQuery Q ;
 Q.SetPLText("*.CANCEL_EDIT_OPER(:OK,:NEWOP,:READONLY,:OP_ID,:OP_SUBID,:OLD_ID,:OLD_SUBID);") ;
 Q.DeclVarInt("OK",ok) ;
 Q.DeclVarInt("NEWOP",newop) ;
 Q.DeclVarInt("READONLY",readonly) ;
 Q.DeclVarInt("OP_ID",op_id) ;
 Q.DeclVarInt("OP_SUBID",op_subid) ;
 Q.DeclVarInt("OLD_ID",old_id) ;
 Q.DeclVarInt("OLD_SUBID",old_subid) ;
 Q.Exec("Cancel oper") ;
}
//---------------------------------------------------------------------------
extern TForm* SaveForm;
//---------------------------------------------------------------------------
void __fastcall TFrmInputOper::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    AmsWinC->FindActiveMDIChild();
    if (readonly)
    {
        Action=caFree;
        return;
    }
    if (InputOperEnd)
    {
        Action = caFree;
    }
    else
    if (readonly || !Dirty||
        AskMessage("Желаете выйти из ввода операции без сохранения?")==ID_YES)
    {
        // Удаляем позицию связанной базы
        if (newop && !SecondStep && TiedBase!="")
        {
            AmsQuery AQ;
            AQ.SetSqlTextReady(
                "begin "
                "delete from "+BaseUserName+"."+TiedBase+
                " where ARTICLE=:ARTICLE and VER=:VER;commit;"
                "exception "
                "   when others then NULL;"
                "end;");
            AQ.DeclVar("ARTICLE", TiedBaseArticle);
            AQ.DeclVar("VER", TiedBaseVer);
            AQ.Exec("Удаляем позицию связанной БД");
        }
        Action = caFree;
    }
    else
        Action =caNone;
    if (Action==caFree)
    {
        bool Finded= false;
        if (SaveForm)
        {
            int i=0;
            while (!Finded && i<AmsWinC->MDIChildCount)
            {
                if (AmsWinC->MDIChildren[i]==SaveForm) Finded=true;
                else i++;
            }
        }
        if (Finded)
        {
            SaveForm->BringToFront();
            SaveForm->SetFocus();
        }
        else
        {
            if (AmsWinC->FrmChooseStdOper)
            {
                AmsWinC->FrmChooseStdOper->BringToFront();
                AmsWinC->FrmChooseStdOper->WindowState=wsNormal;
                AmsWinC->FrmChooseStdOper->SetFocus();
                if (InputOperEnd)
                        AmsWinC->FrmChooseStdOper->EscClear();
            }
        }
      delete FrmEditDbPosOnly;  
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmInputOper::BBPostClick(TObject *Sender)
{
    HandleOper(1) ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmInputOper::DSetItemsANAMEChange(TField *Sender)
{
    Dirty=true;
    TAccName T ;
    AmsQuery Q ;

 T.SetUserAccName(DSetItemsANAME->Value) ;
 try
 {
  int code=T.GetAccCode(Q()) ;
  DSetItemsCODE->Value=code ;
 }
 catch (TAmsException &e)
 {
  ErrorMessage("Нет такого счета") ;
  return;
  /*
  TAccName T1 ;
  T1.SetByCode(DSetItemsCODE->Value,Q()) ;
  DSetItemsANAME->Value=T1.GetUserName() ;
  */
 }
}
//---------------------------------------------------------------------------

void __fastcall TFrmInputOper::DSetItemsASIDEChange(TField *Sender)
{
    Dirty=true;
    DSetItemsSIDE->Value= DSetItemsASIDE->Value=="Д" ? "D" : (DSetItemsASIDE->Value=="К" ? "K" : "" );
}
//---------------------------------------------------------------------------
void __fastcall TFrmInputOper::DSetItemsVALUEChange(TField *Sender)
{
    Dirty=true;
    AmsQuery Q ;
    Q.SetPLText("update *.sums set value=:value where sum_id=:sum_id;") ;
    Q.DeclVar("VALUE",DSetItemsVALUE->Value) ;
    Q.DeclVar("SUM_ID",DSetItemsITEM_SUM_ID->Value) ;
    Q.Exec("update sum") ;
    DatMod->OSession->Commit() ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmInputOper::DSetItemsNewRecord(TDataSet *DataSet)
{
    try {

    Dirty=true;
    if (!FrmAskAccFirst)
    {
        Application->CreateForm(__classid(TFrmAskAccFirst), &FrmAskAccFirst);
    }
    AnsiString S=FrmAskAccFirst->GetAccName();
    if (S=="")
    {
        DSetItems->Cancel();
        return;
    }

    TFrmAskDK * FrmDK;
    Application->CreateForm(__classid(TFrmAskDK), &FrmDK);
    DSetItemsASIDE->Value=FrmDK->AskDK();
    FrmDK->Release();
    if (DSetItemsSIDE->Value=="") {
      DSetItems->Cancel();
      return;
    }
    DSetItemsSIDE->Value= DSetItemsASIDE->Value=="Д" ? "D" : (DSetItemsASIDE->Value=="К" ? "K" : "" );
    DSetItemsANAME->Value= S;
    DSetItemsOP_ID->Value=op_id;
    DSetItemsOP_SUBID->Value=op_subid;
    do {
        AmsQuery AQ;
        AQ.SetSqlText("begin *.CreateSum(:NEW_ID,0); commit; end;");
        AQ.DeclVar("NEW_ID",0);
        AQ.Exec("Находим новую сумму");
        DSetItemsITEM_SUM_ID->Value=AQ.GetVarInt("NEW_ID");
    }while (0);
    DSetItemsOP_DATE->Value=DSetOP_DATE->Value;
    DSetItemsFOR_POST->Value=0;
    DSetItems->Post();
    DatMod->OSession->Commit();
    DSetItems->RefreshRecord();

    } catch(...){
      MessageDlg("В нестандартной операции каждый счет может использоваться только один раз!",
        mtError, TMsgDlgButtons() << mbOK, 0);
      DataSet->Delete();
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmInputOper::FormDestroy(TObject *Sender)
{
    CloseForm() ;
}
//---------------------------------------------------------------------------
void __fastcall TFrmInputOper::ActEditExecute(TObject *Sender)
{
    if (readonly)
    {
        ActRead->Execute();
        return;
    }

    if (!readonly)
    {
        Dirty = true;
        if (DSetItems->State==dsEdit)
        {
            try {
                DSetItems->Post();
                DSetItems->RefreshRecord();
            } catch(...)
            {
                return;
            }
        }
    }
    if (DSetItemsANAME->AsString=="") { DSetItems->Insert(); return; }
    if(DSetItemsANAL_ID->IsNull) // Счет синтетический
    {
        if (!readonly)
        {
            double Sum=DSetItemsVALUE->AsFloat;
            if (AskSum("Введите сумму по "+DSetItemsANAME->AsString+" счету",Sum))
            {
                DSetItems->Edit();
                DSetItemsVALUE->AsFloat=Sum;
                DSetItems->Post();
            }
        }
    }
    else  // счет аналитический
    {
        InpOper.AccNew=false;
        EditAnalitic(
                DSetItemsITEM_SUM_ID->AsInteger,
                DSetItemsANAL_ID->AsInteger,
                DSetItemsCODE->Value,
                DSetItemsSIDE->Value=="D",
                DSetItemsSETNAME->Value);
    }
}
//---------------------------------------------------------------------------
void TFrmInputOper::AnalOk()
{
    if (CurrState!=enWaitForUserInput)
    {
        FrmEditAn = NULL;
        DSetItems->Edit();
        DSetItemsITEM_SUM_ID->Value=AnNewSumId;
        DSetItems->Post();
        DSetItems->RefreshRecord();
        AmsQuery AQ;
        AQ.SetSqlText("begin delete from *.SUMS where SUM_ID=:SUM_ID;commit;end;");
        AQ.DeclVar("SUM_ID" , AnAnSumId);
        AQ.Exec("Удаляем старую аналит сумму");
    }
    else
    {
        FrmEditAn = NULL;
        do {
            AmsQuery A1;
            if (InpOper.Sum_!=0)
            {
                double Sum=InpOper.Sum_;
                A1.SetSqlText(
                    "begin "
                        "update *.SUMS set VALUE = :VALUE where SUM_ID=:ITEM_SUM_ID;"
                        "update *.OP_ITEMS set ITEM_SUM_ID=:ITEM_SUM_ID "
                        "where OP_ID=:OP_ID and OP_SUBID=:OP_SUBID and CODE=:CODE;"
                        "delete from *.SUMS where SUM_ID=:SUM_ID;"
                    "commit;end;");

                A1.DeclVar("VALUE", Sum);
            }
            else
            {
                A1.SetSqlText(
                "begin "
                    "update *.SUMS set VALUE = (select "+InpOper.BalFld+
                    " from *."+InpOper.AnalName+" where SUMID=:ITEM_SUM_ID and "
                    "AARTICLE=' ' and AVER=0) where SUM_ID=:ITEM_SUM_ID;"
                    "update *.OP_ITEMS set ITEM_SUM_ID=:ITEM_SUM_ID "
                    "where OP_ID=:OP_ID and OP_SUBID=:OP_SUBID and CODE=:CODE;"
                    "delete from *.SUMS where SUM_ID=:SUM_ID;"
                "commit;end;");
            }
            A1.DeclVar("OP_ID"      , op_id);
            A1.DeclVar("OP_SUBID"   , op_subid);
            A1.DeclVar("CODE"       , InpOper.Acc);
            A1.DeclVar("ITEM_SUM_ID", InpOper.NewSumId);
            A1.DeclVar("SUM_ID"     , InpOper.CurrSumId);
            A1.Exec("Удаляем старую аналит сумму и вставляем новую");
        } while (0);

        AmsQuery CAQ (QCondItems);
        CAQ.Next();
        CycleBody();
    }
}
//---------------------------------------------------------------------------
void TFrmInputOper::AnalCancel()
{
    if (readonly) return;
    if (InpOper.AccNew)
    {
        AmsQuery AQ;
        AQ.SetPLText(
            "delete from *.OP_ITEMS where OP_ID=:OP_ID and "
                "OP_SUBID=:OP_SUBID and CODE=:CODE; commit;");
        AQ.DeclVar("OP_ID", op_id);
        AQ.DeclVar("OP_SUBID", op_subid);
        AQ.DeclVar("CODE",InpOper.Acc);
        AQ.Exec("Удаляем новый неподтвержденный счет");

    }
    if (CurrState!=enWaitForUserInput)
    {
        FrmEditAn = NULL;
        AmsQuery AQ;
        AQ.SetSqlText("begin delete from *.SUMS where SUM_ID=:SUM_ID;commit;end;");
        AQ.DeclVar("SUM_ID" , AnNewSumId);
        AQ.Exec("Удаляем неподтвержденную аналит сумму");
    }
    else
    {
        // Пользователь НЕ подтвердил ввод
        FrmEditAn = NULL;
        AmsQuery A1;
        A1.SetSqlText(
            "begin "
            "   delete from *.SUMS where SUM_ID=:SUM_ID;"
            "commit;end;");
        A1.DeclVar("SUM_ID" , InpOper.NewSumId);
        A1.Exec("Удаляем неподтвержденную аналит сумму");
        CurrState=enIdle;
        return;
    }
    this->BringToFront();
}
//---------------------------------------------------------------------------
void __fastcall TFrmInputOper::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if (Key == 68 && Shift.Contains(ssAlt))  // -------- Alt+В
    {
        ActDocType->Execute();
        Key=0;
        return;
    }
    else
    if (Key==76 && Shift.Contains(ssAlt)) // Альт+Д
    {
        GenerateDocument(DSetDOC_TYPE->AsString);
        Key=0;
        return;
    }
    else
    if( (Key== 67 || Key==86) && Shift.Contains(ssAlt)) // Альт+M  Alt+C
    {
        ChangeOper(Key!=86);
        Key=0;
        return;
    }
    else
    if (Key==13)
    {
    }
}
//---------------------------------------------------------------------------
int TFrmInputOper::GenerateDocument(const AnsiString& DocType)
{
    do
    {
        Test1=true;
        OpVars* OV = LV.VARS();
        OV->AddVars(DSet);
        OV->st->IncC();
        OV= LV.OPV();
        OV->AddVars(DSet);
        OV->st->IncC();
        //----------------------------------------------------
        // ВПИХИВАЕМ СВЯЗАННУЮ БАЗУ ДАННЫХ в интерпретатор
        //----------------------------------------------------
        ReadTiedBase();
        if (TiedBase!="")
        {
            AmsQuery AQ;
            AQ.SetSqlText("select REALNAME, ALIAS  from *.DBSTR where DBID="
                " (select OID from *.OBJS where NAME=:NAME) and"
                " NOT (REALNAME=ALIAS)");
            AnsiString Sel="D.*";
            AQ.DeclVar("NAME", TiedBase);
            AQ.Exec("Поля, где не совпадает ALIAS и REALNAME");
            while (AQ.NotEof())
            {
                Sel+=", D."+AQ.GetFldStr(0)+" \""+AQ.GetFldStr(1)+"\"";
                AQ.Next();
            }

            AQ.Clear();
            AQ.SetSqlTextReady("select "+Sel+" from "+
                BaseUserName+"."+TiedBase+
                " D where ARTICLE=:ARTICLE and VER=:VER");
            AQ.DeclVar("ARTICLE",DSetDOC_NUM->AsString);
            AQ.DeclVar("VER",DSetDOC_NUM_V->AsInteger);
            AQ.Exec("Выбор полей связанной базы данных");
            if (AQ.NotEof())
            {
                OV->AddVars(AQ);
            }
        }
    }while(0);
    BeginExecReport(DocType, &LV);
    return 1;
}
//---------------------------------------------------------------------------

void __fastcall TFrmInputOper::FormResize(TObject *Sender)
{
 items_grid->Columns->Items[3]->Width= items_grid->Width-35-
                                       items_grid->Columns->Items[0]->Width-
                                       items_grid->Columns->Items[1]->Width-
                                       items_grid->Columns->Items[2]->Width;
}
//---------------------------------------------------------------------------

void __fastcall TFrmInputOper::items_gridDblClick(TObject *Sender)
{
//
    ActEdit->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TFrmInputOper::DSetItemsApplyRecord(TOracleDataSet *Sender,
      char Action, bool &Applied, AnsiString &NewRowId)
{
//
    if (Action =='U')
    {
        /*
        if (DSetItemsANAL_ID->IsNull)
        {
            TField* Fld=DSetItemsVALUE;
            if (Fld->OldValue.IsNull() || Fld->OldValue != Fld->Value)
            {
                AmsQuery AQ;
                AQ.SetSqlTextReady("update "+BaseUserName+".SUMS set VALUE=:VALUE "
                   " where SUM_ID=:SUM_ID ");
                AQ.DeclVar("VALUE", Fld->AsFloat);
                AQ.DeclVar("SUM_ID", DSetItemsITEM_SUM_ID->AsInteger);
                AQ.Exec("Для синт счета вносим новую сумму");
                Applied = true;
            }
        }
        */
    }
    if (Action=='C')
    {
        Applied=true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmInputOper::items_gridKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
    if (Key==67)
    {
        if (Shift.Contains(ssAlt))
        {
            if (!FrmAskAccFirst)
            {
                Application->CreateForm(__classid(TFrmAskAccFirst), &FrmAskAccFirst);
            }

            if (DSetItems->Fields->Fields[0]->AsString=="") {
              //DSetItems->Fields->Fields[0]->AsString="D";
            } else {
              AnsiString S=FrmAskAccFirst->GetAccName();
              if (S!="")
              {
                  DSetItems->Edit();
                  DSetItemsANAME->Value=S;
                  DSetItems->Post();
                  DSetItems->RefreshRecord();
              }
              Key=0;
            }
        }
    }
    FormKeyDown(Sender, Key, Shift);
    if (Key)
    {
        if (Key==13)
        {
            ActEdit->Execute();
            Key=0;
        }
        if (Key==32)
        {
            ActRead->Execute();
            Key=0;
        }

    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmInputOper::ActDocTypeExecute(TObject *Sender)
{
    if (!readonly)
    {
        SCodeAndName CN;
        TFrmChooseObjModal* Frm;
        Application->CreateForm(__classid (TFrmChooseObjModal), &Frm);
        if (Frm->ChooseObjModal(otDoc, CN))
        {
            DSet->Edit();
            DSetDOC_TYPE->AsString=CN.Name;
            DSet->Post();
        };
        Frm->Release();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmInputOper::DBDateEdit1Exit(TObject *Sender)
{
// Проверка даты на входжение в период видимости
    TDateTime Dt = DBDateEdit1->Date;
    if (G.ur.beg_date.val.T()>Dt || G.ur.end_date.val.T()<Dt)
    {
        ErrorMessage ("Внимание!!! Дата вне диапазона периода видимости");
        this->SetFocus();
        text_edit->SetFocus();
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmInputOper::DBEdit2Enter(TObject *Sender)
{
        // Пытаемся сгенерировать номер, если стоит автонумерация
        if (NumberOut)
        {
            NumberOut=false;
            return;
        }
        AmsQuery AQ;
        AQ.SetSqlText(
            "begin *.bookkern.ProcessDocNum(:OP_ID, :OP_SUBID, :OLD_ID, :OLD_SUBID, "
            ":OP_DATE, :DOC_TYPE, :DOC_NUM, :DOC_NUM_V, :NO_CHECK_DUP, "
            ":RESULT); commit; end;");
        AQ.DeclVarInt("OP_ID", op_id);
        AQ.DeclVarInt("OP_SUBID", op_subid);
        if (newop)
        {
            AQ.DeclVarInt("OLD_ID");
            AQ.DeclVarInt("OLD_SUBID");
        }
        else
        {
            AQ.DeclVarInt("OLD_ID",    old_id);
            AQ.DeclVarInt("OLD_SUBID", old_subid);
        }
        AQ.DeclVarDate("OP_DATE",   DBDateEdit1->Date);
        AQ.DeclVarStr ("DOC_TYPE",  DBEdit1->Text);
        AQ.DeclVarStr ("DOC_NUM",   DBEdit2->Text);
        AQ.DeclVarInt ("DOC_NUM_V", DSetDOC_NUM_V->Value);
        // !!!!!!!!!!! Сложное условие если пользователь не изменил ничего
        AQ.DeclVarInt ("NO_CHECK_DUP", 0);
        AQ.DeclVarInt("RESULT");
        AQ.Exec("");

        int Res=AQ.GetVarInt("RESULT");
        switch (Res)
        {
            case 0:
            case 10:
                if (DSet->State!=dsEdit)
                        DSet->Edit();
                DSetDOC_NUM->Value=AQ.GetVarStr("DOC_NUM");
                DSetDOC_NUM_V->Value=AQ.GetVarInt("DOC_NUM_V");
                DSet->Post();
            break;
            case 1:
            case 2:
                ErrorMessage("Неправильный тип документа.\r\n Выберите документ ч/з Alt+В");
            break;
            case 3:
                ErrorMessage("Такой номер уже существует, а дублирование запрещено");
            break;
            case 4:
            break;
            default:
                ErrorMessage("Неожиданный результат. Свяжитесь с АМС.");
            break;
        }
}
//---------------------------------------------------------------------------

void __fastcall TFrmInputOper::DBEdit2Exit(TObject *Sender)
{
    NumberOut=true;

        AmsQuery AQ;
        AQ.SetSqlText(
            "begin *.bookkern.ProcessDocNum(:OP_ID, :OP_SUBID, :OLD_ID, :OLD_SUBID, "
            ":OP_DATE, :DOC_TYPE, :DOC_NUM, :DOC_NUM_V, :NO_CHECK_DUP, "
            ":RESULT); commit; end;");
        AQ.DeclVarInt("OP_ID", op_id);
        AQ.DeclVarInt("OP_SUBID", op_subid);
        if (newop)
        {
            AQ.DeclVarInt("OLD_ID");
            AQ.DeclVarInt("OLD_SUBID");
        }
        else
        {
            AQ.DeclVarInt("OLD_ID",    old_id);
            AQ.DeclVarInt("OLD_SUBID", old_subid);
        }
        AQ.DeclVarDate("OP_DATE",   DBDateEdit1->Date);
        AQ.DeclVarStr ("DOC_TYPE",  DBEdit1->Text);
        AQ.DeclVarStr ("DOC_NUM",   DBEdit2->Text);
        AQ.DeclVarInt ("DOC_NUM_V", DSetDOC_NUM_V->Value);
        // !!!!!!!!!!! Сложное условие если пользователь не изменил ничего
        AQ.DeclVarInt ("NO_CHECK_DUP", 0);
        AQ.DeclVarInt("RESULT");
        AQ.Exec("");

        int Res=AQ.GetVarInt("RESULT");
        DocNumOk=CheckedNotOk;
        switch (Res)
        {
            case 0:
            case 10:
                DSet->Edit();
                DSetDOC_NUM->Value=AQ.GetVarStr("DOC_NUM");
                DSetDOC_NUM_V->Value=AQ.GetVarInt("DOC_NUM_V");
                DSet->Post();

                if (!op_subid && TiedBase!="" && TiedBaseArticle!="" &&
                    TiedBaseArticle!=DSetDOC_NUM->Value)
                {
                    AQ.Clear();
                    AQ.SetSqlTextReady("begin "+BaseUserName+"."+TiedBase+
                    "_RENAME(:OLD_ART, :OLD_VER, :NEW_ART, :NEW_VER); end;");
                    AQ.DeclVarStr("OLD_ART", TiedBaseArticle);
                    AQ.DeclVarStr("NEW_ART", DSetDOC_NUM->Value);
                    AQ.DeclVarInt("OLD_VER", TiedBaseVer);
                    AQ.DeclVarInt("NEW_VER", DSetDOC_NUM_V->Value);
                    AQ.Exec("Сохраняем связанную базу");
                }
                DocNumOk=CheckedOk;
            break;
            case 1:
            case 2:
                if (MiceOverClose) return;
                ErrorMessage("Неправильный тип документа.\r\n Выберите документ ч/з Alt+В");
                DBEdit2->SetFocus();
            break;
            case 3:
                if (MiceOverClose) return;
                ErrorMessage("Такой номер уже существует, а дублирование запрещено");
                DBEdit2->SetFocus();
            break;
            case 4:
                if (MiceOverClose) return;
                ErrorMessage("Введите номер документа");
                DBEdit2->SetFocus();
            break;
            default:
                if (MiceOverClose) return;
                ErrorMessage("Неожиданный результат. Свяжитесь с АМС.");
                DBEdit2->SetFocus();
            break;
        }
}
//---------------------------------------------------------------------------
void TFrmInputOper::EditAnalitic(int AnSumId_,
                        int AnId_, int AccCode_, bool Debet,
                        const AnsiString& SetName_)
{
    AnAnSumId=AnSumId_;
    AnAnId = AnId_;

    AmsQuery AQ;
    AQ.SetSqlText(
        "BEGIN "
        "*.CreateAndCopyAnSum(:ANAL_ID,:NEW_SUM_ID,:SUM_ID);"
        "select max(BAL_FLD), max(NAME), max(DESCR), max(OPTS), max(SETNAME) into "
        ":BAL_FLD, :NAME, :DESCR, :OPTS, :SETNAME from *.ACCS "
        "where CODE=:CODE; :RI:=*.RI(:CODE);commit;END;");
    AQ.DeclVar("NEW_SUM_ID", 0);
    AQ.DeclVar("BAL_FLD", "");
    AQ.DeclVar("NAME", "");
    AQ.DeclVar("DESCR", "");
    AQ.DeclVar("SUM_ID" , AnAnSumId);
    AQ.DeclVar("ANAL_ID", AnAnId);
    AQ.DeclVar("RI", 0);
    AQ.DeclVar("CODE", AccCode_);
    AQ.DeclVar("OPTS", 0);
    AQ.DeclVar("SETNAME", "");

    AQ.Exec("Скопировать аналит сумму");

    AnNewSumId = AQ.GetVarInt("NEW_SUM_ID");
    AnsiString BalFld= AQ.GetVarStr("BAL_FLD");
    int AnOst = AQ.GetVarInt("RI");
    int Opts = AQ.GetVarInt("OPTS");
    AnsiString Descr =TAccName::ToUserName(AQ.GetVarStr("NAME"))+
        (Debet?" Дебет ":" Кредит ")+
        AQ.GetVarStr("DESCR");
    AnsiString ResSetName=SetName_;
    if (SetName_=="" || SetName_==" ")
        ResSetName=AQ.GetVarStr("SETNAME");
    FormAccv(AccCode_);
    TFrmBrowsAn* Frm;
    Application->CreateForm(__classid(TFrmBrowsAn), &Frm);
    FrmEditAn=Frm;
    TFrmBrowsAn::BegMode BM;
    BM.Vars=&LV;
    BM.BalFld=BalFld;
    BM.ReadOnly=readonly;
    if (Opts & aoNegRemWarring)
    {
        BM.AddToRem =  Debet? (!DSetSTORNO->Value):(DSetSTORNO->Value);
        BM.PredSumId= PrevState==osWasCommited?AnAnSumId:0;
        BM.CheckNeg = true;
    }
    BM.ZeroVer=Opts & aoZeroVer;
    BM.newop=newop;
    BM.AddToRem =  Debet? (!DSetSTORNO->Value):(DSetSTORNO->Value);

    Frm->Begin(this,AnAnId, "", ResSetName, Descr,
        AnNewSumId, AnOst, BM);
};
//---------------------------------------------------------------------------
void __fastcall TFrmInputOper::ActOkExecute(TObject *Sender)
{
    //
    BBOkClick(this);
}
//---------------------------------------------------------------------------
int TFrmInputOper::ChangeOper(bool SaveNumber)
{
    if (newop)
    {
        Message("Не могу изменить вновь вводимую операцию\r\n"
                "Сначала отложите ее или выберите другую новую операцию");
        return 1;
    }
    // Изменяем стандартную операцию
    AmsQuery AQ;
    // Проверяем первая ли операция
    if (1)
    {
        AQ.SetSqlText(
            "begin select count(*) into :RES from *.OPERS "
            "where OP_ID=:OP_ID and OP_SUBID in "
            " (select COND_SUBID from *.CONDS "
            "  where COND_ID=:COND_ID and ORD<:ORD); end;");
        AQ.DeclVar("OP_ID", old_id);
        AQ.DeclVar("COND_ID", CondId);
        AQ.DeclVar("ORD", CondOrd);
        AQ.DeclVar("RES", CondOrd);
        AQ.Exec("Проверяем первая ли операция");
        if (AQ.GetVarInt("RES"))
        {
            ErrorMessage("Операция не первая");
            return 1;
        }
    }
    if (AskMessage("Изменить стандартую операцию?", false)==ID_YES)
    {
        AQ.SetSqlText(
            "begin select count(*) into :RES from *.OPERS "
            "where OP_ID=:OP_ID and OP_SUBID in "
            " (select COND_SUBID from *.CONDS "
            "  where COND_ID=:COND_ID and ORD>:ORD); end;");
        AQ.Exec("Сущестуют ли следующие проводки");

        if (AQ.GetVarInt("RES"))
        {
            if (AskMessage("Существуют следующие проводки.\r\nУдалить их?")!=ID_YES)
            {
                return 2;
            }
            AQ.Clear();
            AQ.SetSqlText(
                "begin"
                " *.CANCEL_EDIT_OPER(:OK,:NEWOP,:READONLY,:OP_ID,:OP_SUBID,:OLD_ID,:OLD_SUBID);"
                " *.BookKern.DeleteOperGroup(:OLD_ID, :RES, :ORD);"
                " commit; end;");
            AQ.DeclVarInt("OK",0) ;
            AQ.DeclVarInt("NEWOP",newop) ;
            AQ.DeclVarInt("READONLY",readonly) ;
            AQ.DeclVarInt("OP_ID",op_id) ;
            AQ.DeclVarInt("OP_SUBID",op_subid) ;
            AQ.DeclVarInt("OLD_ID",old_id) ;
            AQ.DeclVarInt("OLD_SUBID",old_subid) ;
            AQ.DeclVar("RES", 0);
            AQ.DeclVar("ORD", CondOrd);
            AQ.Exec("Удаляем все следующие проводки");
            int Res = AQ.GetVarInt("RES");
            if (Res)
            {
                switch (Res)
                {
                    case 4:
                        ErrorMessage("Недостаточно прав");
                    break;
                    case 8:
                        ErrorMessage("Операция редактируется другим пользователем");
                    break;
                    default:
                        ErrorMessage("BookKern.DeleteOperGroup неожиданный результат");
                }
                return 3;
            }
        }  // Сущестуют следующие проводки
        else // Не Сущестуют следующие проводки
        {
            AQ.Clear();
            AQ.SetSqlText(
                "begin"
                " *.CANCEL_EDIT_OPER(:OK,:NEWOP,:READONLY,:OP_ID,:OP_SUBID,:OLD_ID,:OLD_SUBID);"
                " commit; end;");
            AQ.DeclVarInt("OK",0) ;
            AQ.DeclVarInt("NEWOP",newop) ;
            AQ.DeclVarInt("READONLY",readonly) ;
            AQ.DeclVarInt("OP_ID",op_id) ;
            AQ.DeclVarInt("OP_SUBID",op_subid) ;
            AQ.DeclVarInt("OLD_ID",old_id) ;
            AQ.DeclVarInt("OLD_SUBID",old_subid) ;
            AQ.DeclVar("OLD_ID", op_id);
            AQ.Exec("Отмена текущего редактирования");
        }

        // Выбираем новую станд операцию
        TFrmChooseStdOperModal* Frm;
        Application->CreateForm(__classid(TFrmChooseStdOperModal), &Frm);
        int NewCondId=Frm->ChooseStdOper();
        Frm->Release();
        if (NewCondId)
        {
            AmsQuery CAQ(CondOQ);
            if (!CondId)
            {
                CAQ.SetSqlText(
                    "select * from *.CONDS where COND_ID=:COND_ID and ORD>=:ORD"
                    " order by ORD");
                CAQ.DeclVar("COND_ID", 0);
                CAQ.DeclVar("ORD", -1);
            }
            CondId=NewCondId;
            CAQ.SetVar("COND_ID", CondId);
            CAQ.SetVar("ORD", -1);
            CAQ.Exec("Узнаем новую стандартную проводку");
            TiedBase="";
            if (CAQ.NotEof())
                    {
                AmsQuery Q;
                CondOrd= CAQ.GetFldInt("ORD");
                int NewOpSubId=CAQ.GetFldInt("COND_SUBID");
                Q.DeclVar("OLD_ID",old_id) ;
                Q.DeclVar("OLD_SUBID",old_subid) ;
                Q.DeclVar("NEW_ID",op_id) ;
                Q.DeclVar("NEW_SUBID",NewOpSubId);
// CH 10--0-2002
//                Q.DeclVar("COND_ID",NewCondId) ;
                Q.SetPLText(//"update *.OPERS set COND_ID=:COND_ID where OP_ID=:OLD_ID;"
                        "*.bookkern.CopyOper(:OLD_ID,:OLD_SUBID,:NEW_ID,:NEW_SUBID,3);");
//EO CH 10--0-2002
                Q.Exec("Копируем для изменения проводки");

                DeepCopy=true;

                DSet->DisableControls();
                DSetItems->DisableControls();
                DSet->Active=false;
                DSetItems->Active=false;
                op_subid=NewOpSubId;
                DSet->SetVariable("OP_ID", op_id);
                DSetItems->SetVariable("OP_ID", op_id);
                DSet->SetVariable("OP_SUBID", op_subid);
                DSetItems->SetVariable("OP_SUBID", op_subid);
                DSet->Active=true;
                DSetItems->Active=true;
                DSet->EnableControls();
                DSetItems->EnableControls();

                DSet->Edit();
                DSetTEXT->Value=CAQ.GetFldStr("TEXT");
                DSetDOC_TYPE->Value=CAQ.GetFldStr("DOC_TYPE");
                DSetCOND_ID->Value= NewCondId;
                if (!SaveNumber)
                {
                    DSetDOC_NUM->Value="";
                    DSetDOC_NUM_V->Value=0;
                    DSetOP_DATE->Value= (int)GetInpDate();
                }
                DBEdit2->SetFocus();
            }

        }
      IsReadTiedBase= false;
    }
    return 0;
}
//---------------------------------------------------------------------------

void __fastcall TFrmInputOper::ActStandExecute(TObject *Sender)
{
//
    ChangeOper(true);
}
//---------------------------------------------------------------------------

void __fastcall TFrmInputOper::ActStandIzExecute(TObject *Sender)
{
//
    ChangeOper(false);
}
//---------------------------------------------------------------------------
int TFrmInputOper::BeginAutoInput(void)
{
    if (CurrState==enExecute)
    {
        return 1;
    }
    if (CurrState==enWaitForUserInput)
    {
        if (FrmBrowsAn)
        {
            FrmBrowsAn->BringToFront();
            FrmBrowsAn->SetFocus();
        }
        return 2;
    }
    if (CurrState==enWaitForUserInputS)
    {
        if (FrmInputSumNM)
        {
            FrmInputSumNM->BringToFront();
            FrmInputSumNM->SetFocus();
        }
        return 2;
    }
    if (CurrState==enWaitForDbInput)
    {
        if (FrmEditDbPosOnly)
        {
            FrmEditDbPosOnly->BringToFront();
            FrmEditDbPosOnly->SetFocus();
        }
        return 3;
    }
    if (CondId)
    {
        CurrState=enExecute;
        OpVars* OV = LV.VARS();
        OV->AddVars(DSet);
        OV->st->IncC();
        OV= LV.OPV();
        OV->AddVars(DSet);
        OV->st->IncC();
        // Выполняем пред формулу
        if (PredFormula!="")
        {
            IP ip;
            ip.SetVars(&LV);
            ip.Enter();
            Prog prog;

            try
            {
                int ok ;
                ProgStr pf;
                char Str[256];
                strcpy(Str, PredFormula.c_str());
                pf.Init(Str);

                prog.Compile(&pf) ;
                prog.Link(GC::DocPath.c_str());

                cur_arg=-1 ;
                if (ok)
                    prog.Run(ip);
            }
            catch(OraError ERR)
            {
                ERR.message() ;
            }
            ip.Leave();
        }
        ///
        BeginTiedDbInput(true);
    }
    return 0;
}
//---------------------------------------------------------------------------
void TFrmInputOper::BeginTiedDbInput(bool FromAutoInput)
{
    // Связанная база данных
    if (SecondStep)
    {
        BeginAutoInput2();
        return;
    }
    ReadTiedBase();


    if (TiedBase!="")
    {
        // Ввод для связанной базы данных
        if (!FrmEditDbPosOnly)
        {
            Application->CreateForm(__classid(TFrmEditDbPosOnly), &FrmEditDbPosOnly);
            FrmEditDbPosOnly->Init(TiedBase, " ", &LV);
            if (TiedBaseArticle!="")
            {
                AmsQuery AQ;
                TiedBaseArticle=DSetDOC_NUM->Value;
                TiedBaseVer=DSetDOC_NUM_V->Value;

                AQ.SetSqlTextReady("begin "
                "declare vTmp number(1); "
                "begin "
                "select max(1) into vTmp from "+BaseUserName+"."+TiedBase+
                " where ARTICLE=:ARTICLE and VER=:VER; "
                "if vTmp is NULL then "
                "insert into "+BaseUserName+"."+TiedBase+
                " (CATID, ARTICLE, VER) values (:CATID, :ARTICLE, :VER);"
                "end if;"
                "end; commit; end;");
                AQ.DeclVar("CATID", FrmEditDbPosOnly->FDescr().GetRootCat());
                AQ.DeclVar("ARTICLE", TiedBaseArticle);
                AQ.DeclVar("VER", TiedBaseVer);
                AQ.Exec("Вставка новой позиции в связанную базу данных");
            }
            if (TiedBaseToOper)
            {
                FrmEditDbPosOnly->Caption="Связанная с операцией БД ";
            }
            else
            {
                FrmEditDbPosOnly->Caption="Связанная с документом "+DSetDOC_TYPE->Value+" база ";
            }
            if (GC::ViewDebug)
                FrmEditDbPosOnly->Caption=FrmEditDbPosOnly->Caption+
                    TiedBase+"["+TiedBaseArticle+
                    "|"+TiedBaseVer+"]";
        }

        // Формируем OPV для редактирования связанной БД
        OpVars* OV = LV.OPV();
        OV->AddVars(DSet);
    }
    int Flag = eoDubbing|eoDbFlds;
    // Проверяем занятую позицию ?????
    // eoBusyPos       = 0x08,     // Редактируем занятую позицию
    if (readonly) Flag |=eoReadOnly;
    if (FromAutoInput && NotShowFirstTimeTDb)
    {
        if (NotShowFirstTimeTDb)
            FrmEditDbPosOnly->EditPosNMOk(this, TiedBaseArticle, TiedBaseVer, Flag);
        BeginAutoInput2();
    }
    else
    if (TiedBase!="")
    {
        if (FromAutoInput) CurrState=enWaitForDbInput;
        FrmEditDbPosOnly->EditPosNM(this, TiedBaseArticle, TiedBaseVer, Flag);
    }
    else
    {
        if (FromAutoInput)
            BeginAutoInput2();
    }
}
//---------------------------------------------------------------------------
void TFrmInputOper::TiedDbEditEnd(void)
{
    FrmEditDbPosOnly=NULL;
    do {
        AmsQuery AQ;
        AQ.SetSqlText(
            "select * from "+BaseUserName+"."+TiedBase+
            " where ARTICLE=:ARTICLE and VER=:VER");
        AQ.DeclVar("ARTICLE", TiedBaseArticle);
        AQ.DeclVar("VER", TiedBaseVer);
        AQ.Exec("Выбираем строку связанной БД");
        if (AQ.NotEof())
            LV.OPV()->AddVars(AQ);
        ShowTiedDbShortInfo();
    }while (0);
    if (CurrState==enWaitForDbInput)
    {
        CurrState=enExecute;
        BeginAutoInput2();
    }
}
//---------------------------------------------------------------------------
void TFrmInputOper::BeginAutoInput2(void)
{
    //*******************************************************************
    //  Начинаем по счетам ходить
    //*******************************************************************
    AccCodes.clear();
    AmsQuery AQ(QCondItems);
    AQ.SetSqlTextReady(
        "select CI.*, A.NAME, A.ANAL_ID, A.NODE, A.BAL_FLD, A.DESCR ACC_DESCR, A.OPTS "
        "from "+BaseUserName+".COND_ITEMS CI, "+BaseUserName+".ACCS A "
        "where COND_ID=:COND_ID and COND_SUBID=:COND_SUBID and A.CODE=CI.CODE "
        "order by ITEM_NUM");
    AQ.DeclVar("COND_ID", CondId);
    AQ.DeclVar("COND_SUBID", op_subid);
    AQ.Exec("Выбор номеров счетов");

    CycleBody();
};
//----------------------------------------------------------------------------
void TFrmInputOper::CycleBody()
{

    DSetItems->DisableControls();
    DSetItems->Refresh();
    DSetItems->EnableControls();
    AmsQuery AQ(QCondItems);


    bool More = false;

    if (AQ.NotEof())
    {
        AnsiString ItemFormula =AQ.GetFldStr("ITEM_FORMULA");
        AnsiString CurrSetName=AQ.GetFldStr("SETNAME");
        if (CurrSetName=="")
        {
            AmsQuery Q1;
            Q1.SetSqlText("select SETNAME from *.ACCS where CODE=:CODE");
            Q1.DeclVar("CODE", AQ.GetFldStr("CODE"));
            Q1.Exec("Выбираем сохраненный SET");
            if (Q1.NotEof())
            {
                CurrSetName=Q1.GetFldStr(0);
            }
        }

        OpVars* OV = LV.VARS();
        OV->AddVarInt("BREAK_OPER",0);
        OV->AddVarStr("NO_VER_ORD","");   // Имя поzя, по которому выполняется списание
        OV->AddVarInt("NO_VER_FIFO",-1);   //        списание по убыванию
        OV->AddVarInt("CANCEL_AUTO_APPLY",0);
        OV->AddVarInt("SET_AUTO_APPLY",0);
        OV->AddVarInt("SET_BEGIN_BUT",0);
        OV->AddVarInt("A",0);
        OV->AddVarSum("S",0);
        OV->AddVarInt("R",0);
        OV->AddVarInt("D",0);
        OV->AddVarInt("CO",0);
        OV->AddVarInt("OM",-1);
        OV->AddVarInt("VM",-1);
        OV->AddVarInt("E",-1);
        OV->AddVarInt("ACC",0);
        OV->AddVarStr("ART","");
        OV->AddVarInt("V",0);
        OV->AddVarInt("CR",0);
        OV->AddVarStr("SET",CurrSetName);
        OV->AddVarStr("T","");
        OV->AddVarStr("BAR_FLD","");
        OV->AddVarStr("BAR_ORD","");
//------------------------------ CH 26-04-02
        if (AmsWinC->UseShtrih)
        {
           OV->AddVarStr("SH_C", AmsWinC->Shtrih.SubString(CodeOperLen+1, ShLen));
           OV->AddVarStr("FLD_VAL", "");
           OV->AddVarStr("FND_FLD", "");
        }

//------------------------------EO CH 26-04-02
        if (newop)
            AddNewAnSums(CondId, op_subid);
        if (ItemFormula!="")
        {
            IP ip;
            ip.SetVars(&LV);
            ip.Enter();
            Prog prog;

            try
            {
                int ok ;
                ProgStr pf;
                char Str[256];
                strcpy(Str, ItemFormula.c_str());
                pf.Init(Str);

                ok=prog.Compile(&pf) ;
                prog.Link(GC::DocPath.c_str());

                cur_arg=-1 ;
                if (ok)
                    prog.Run(ip);
            }
            catch(OraError ERR)
            {
                ERR.message() ;
            }
            ip.Leave();
        }
        // Если установлен BREAK OPER -- заканчиваем ввод
        if (OV->st->Var("BREAK_OPER").Number()!=0)
        {
            Dirty=false;
            BBCancelClick(this);
            return;
        }

        // Если установлен CANCEL_AUTO_APPLY
        if (OV->st->Var("CANCEL_AUTO_APPLY").Number()!=0)
        {
            AutoApply=0;
        }
        // Если установлен SET_AUTO_APPLY
        if (OV->st->Var("SET_AUTO_APPLY").Number()!=0)
        {
            AutoApply=1;
        }
        // Если установлен SET_BEGIN_BUT
        if (OV->st->Var("SET_BEGIN_BUT").Number()!=0)
        {
            SetBeginBut=1;
        }
        // Определяем код счета, участвующего в операции
        int Acc=OV->st->Var("ACC").Number();
        int AnalId=0;
        int Node=0;
        int ForPost = AQ.GetFldInt("FOR_POST");
        AnsiString AccName = "";
        AnsiString AccDescr = "";
        AnsiString BalFld = "";
        AnsiString SETName = ""; //EO CH 28.03

//----------------------------------CH T=0
        AnsiString T_Equil ="";
        if (ItemFormula!="") {
          int jkl=ItemFormula.Pos("T='")+3;
          if (jkl-3) {
            int hjk=jkl;
            while (ItemFormula[hjk]!=';' && hjk<ItemFormula.Length()) hjk++;
            if (ItemFormula[hjk]==';') hjk--;
            T_Equil=ItemFormula.SubString(jkl, hjk-jkl);
            T_Equil=" ("+T_Equil+") ";
          }
        }
//----------------------------------EO CH T=0

//----------------------------------CH CM =0 без категорий, CM=1 с категориями.
//-------------------------------------------Ежели CM нет - умолчательно
        CM_Var=-1;
        if (ItemFormula!="") {
          int jkl=ItemFormula.Pos("CM=")+3;
          if (jkl-3) {
            int hjk=jkl;
            while (ItemFormula[hjk]!=';' && hjk<ItemFormula.Length()) hjk++;
            if (ItemFormula[hjk]==';') hjk--;
            try { CM_Var=1-StrToInt(ItemFormula.SubString(jkl, hjk-jkl+1)); } catch (...) {;}
          }
        }
//----------------------------------EO CH CM
//----------------------------------------CH R is R_Equil
        int R_Equil=0;
        if (ItemFormula!="") {
          int jkl=ItemFormula.Pos("R=")+2;
          if (jkl-2) {
            int hjk=jkl;
            while (ItemFormula[hjk]!=';' && hjk<ItemFormula.Length()) hjk++;
            if (ItemFormula[hjk]==';') hjk--;
            try { R_Equil=StrToInt(ItemFormula.SubString(jkl, hjk-jkl+1)); } catch (...) {;}
          }
        }
//----------------------------------EO CH R


        int Opts = 0;

        if (Acc)
        {
            AmsQuery A1;
            A1.SetSqlText(
                //       0       1      2       3       4      5
                "select NAME, ANAL_ID, NODE, BAL_FLD, DESCR, OPTS from *.ACCS "
                "where CODE=:CODE");
            A1.DeclVar("CODE", Acc);
            A1.Exec("");
            if (A1.NotEof())
            {
                AccName=A1.GetFldStr(0);
                AnalId=A1.GetFldInt(1);
                Node=A1.GetFldInt(2);
                BalFld=A1.GetFldStr(3);
                AccDescr=A1.GetFldStr(4);
                Opts=A1.GetFldInt(5);
            }
            else Acc=0;
        }

        if (!Acc)
        {
            AnalId=AQ.GetFldInt("ANAL_ID");
            Node=AQ.GetFldInt("NODE");
            Acc=AQ.GetFldInt("CODE");
            AccName=AQ.GetFldStr("NAME");
            AccDescr=AQ.GetFldStr("ACC_DESCR");
            BalFld=AQ.GetFldStr("BAL_FLD");
            Opts=AQ.GetFldInt("OPTS");
        }
//-------------------------------------------------------------------------

        // Если выбран счет с субсчетами
        if (Node)
        {
            if (!FrmAskAccCode)
            {
                Application->CreateForm(__classid(TFrmAskAccCode), &FrmAskAccCode);
            }

            // Пытаемся найти счет если уже был введен
            AmsQuery A1;
            A1.SetSqlText(
                "begin "
                " :RES:=*.GetExistedAcc(:OP_ID, :OP_SUBID, :ACC_CODE, :SIDE);"
                "end;");
            A1.DeclVar("OP_ID", op_id);
            A1.DeclVar("OP_SUBID", op_subid);
            A1.DeclVar("ACC_CODE", Acc);
            A1.DeclVar("SIDE", AQ.GetFldStr("SIDE"));
            A1.DeclVar("RES", 0);
            A1.Exec("Если пользователь уже выбирал счета");
            int Res= A1.GetVarInt("RES");

            Acc = FrmAskAccCode->AskAccCode(BaseUserName, Acc, Res);

            if (!Acc)
            {
                CurrState=enIdle;
                return;
            }
            A1.Clear();
//-----------------------------------CH 28.03

            A1.SetSqlText(
                "select NAME, ANAL_ID, NODE, BAL_FLD, DESCR, OPTS, SETNAME from *.ACCS "
                "where CODE=:CODE");
            A1.DeclVar("CODE", Acc);
            A1.Exec("");
            if (A1.NotEof())
            {
                AccName=A1.GetFldStr(0);
                AccDescr=A1.GetFldStr(4);
                AnalId=A1.GetFldInt(1);
                Opts=A1.GetFldInt(5);
                Node=A1.GetFldInt(2);
                BalFld=A1.GetFldStr(3);
                SETName=A1.GetFldStr(6);

            }
//----------------------------------EO CH 28.03
            else Acc=0;
        }

        // Запоминаем какие номера счетов есть в операции
        AccCodes.push_back(Acc);
        int CurrSumId=0;
        int CopySumId=0;
        bool VarASetted=false;
        if (AnalId)
        {

            int Tmp =OV->st->Var("A").Number();
            if (Tmp!=0)
            {
                CopySumId=OV->st->Var("A").Number();
                VarASetted = true;
            }
            else
            if (Acc && (ItemFormula=="" || DeepCopy))
            {
                // Если у нас существуют счета с такой же аналитикой,
                // и не указана формула нахождения аналитики, то ищем счет
                // с такой же аналитикой
                AmsQuery A1;
                A1.SetSqlTextReady("select "+BaseUserName+"."+
                "GetAnSumByAnId(:ANAL_ID, :OP_ID, :OP_SUBID) from DUAL");
                A1.DeclVar("ANAL_ID", AnalId);
                A1.DeclVar("OP_ID", op_id);
                A1.DeclVar("OP_SUBID", op_subid);

                A1.Exec("Выбираем код аналитической суммы с такой же аналитикой");
                if (A1.NotEof())
                {
                    CopySumId=A1.GetFldInt(0);
                }
            }
        }

        do {
            AmsQuery A1;
            A1.SetSqlText(
                "begin "
                "select max(1) into :NewAcc from *.OP_ITEMS "
                "where OP_ID=:OpId and OP_SUBID=:OpSubId and CODE=:AccCode;"
                "*.GetAccSumId(:AccCode,:OpId,:OpSubId,:AnalSumId,:Copy,"
                ":ItemNum,:SET_NAME);"
                "commit;end;");
            A1.DeclVar("ACCCODE", Acc);
            A1.DeclVar("OPID", op_id);
            A1.DeclVar("OPSUBID", op_subid);
            A1.DeclVar("NEWACC", 0);
            A1.DeclVar("ANALSUMID", CopySumId);
            A1.DeclVarInt("COPY", VarASetted);
            A1.DeclVarInt("ITEMNUM", AQ.GetFldInt("ITEM_NUM"));
            A1.DeclVar("SET_NAME", AQ.GetFldStr("SETNAME"));
            A1.Exec("Копируем и находим SumId по счету ");

            CurrSumId=A1.GetVarInt("ANALSUMID");
            InpOper.CurrSumId=CurrSumId;
            InpOper.AccNew=!A1.GetVarInt("NEWACC");
        }while(0);
//----------------------------------------------------------------------------------------------------------



        // Ввод пользователем аналитики/суммы
        if (AnalId)
        {
            //
            AmsQuery A1;


              if (OV->st->Var("CR").Number()!=0)
              { // Если нужен остаток на след день после операции
                  A1.SetSqlText(
                      "BEGIN "
                      "select NAME into :NAME from *.OBJS where OID=:ANAL_ID;"
                      "*.CreateAndCopyAnSum(:ANAL_ID,:NEW_SUM_ID,:SUM_ID);"
                      "commit;"
                      ":OST:=*.R_AS(:ACC, :REM_DATE);"
                      "commit;END;");
                  A1.DeclVarDate("REM_DATE", (int)DSetOP_DATE->Value+1);
              }
              else
              {
                  A1.SetSqlText(
                      "BEGIN "
                      "select NAME into :NAME from *.OBJS where OID=:ANAL_ID;"
                      "select NAME into :NAME_DB from *.OBJS where OID="
                          "(select PARENT from *.OBJS where OID=:ANAL_ID);"
                      "*.CreateAndCopyAnSum(:ANAL_ID,:NEW_SUM_ID,:SUM_ID);"
                      "commit;"
                      ":OST:=*.RI(:ACC);"
                      "commit;END;");
              };
              A1.DeclVar("OST",0);
              A1.DeclVar("ACC", Acc);
              A1.DeclVar("NEW_SUM_ID", 0);
              A1.DeclVar("SUM_ID" , CurrSumId);
              A1.DeclVar("ANAL_ID", AnalId);
              A1.DeclVar("NAME", "");
              A1.DeclVar("NAME_DB", "");

              A1.Exec("Скопировать аналит сумму и найти аналит сумму остатка");

            int NewSumId = A1.GetVarInt("NEW_SUM_ID");
            int RemSumId = A1.GetVarInt("OST");

     //------------------------------CH
     /*       if (!newop){            //create tmp sumid
              AmsQuery Q;
              Q.SetSqlText(
                    "BEGIN "
                    "select NAME into :NAME from *.OBJS where OID=:ANAL_ID;"
                    "select NAME into :NAME_DB from *.OBJS where OID="
                        "(select PARENT from *.OBJS where OID=:ANAL_ID);"
                    "*.CreateAndCopyAnSum(:ANAL_ID,:TMP_SUM_ID,:SUM_ID);"
                    "commit;"
                    "END;");

                    Q.DeclVar("ANAL_ID", AnalId);
                    Q.DeclVar("TMP_SUM_ID", 0);
                    Q.DeclVar("SUM_ID" , RemSumId);
                    Q.DeclVar("NAME", "");
                    Q.DeclVar("NAME_DB", "");

                    Q.Exec("Создать временную аналит сумму и скопировать в нее аналит сумму остатка");
                    RemSumId = Q.GetVarInt("TMP_SUM_ID");
              Q.DeleteVars();


              AnsiString SIGH="-";
              TOracleQuery* OQN= DatMod->GetOQ();
              TOracleQuery* QH= DatMod->GetOQ();
              AmsQuery OHQ;
              try {

                OQN->DeleteVariables();
                QH->DeleteVariables();
                OQN->SQL->Text= "select SIDE, STORNO from "+BaseUserName+".op_items oi, "+BaseUserName+".opers o "
                   " where item_sum_id="+IntToStr(CurrSumId)+" and o.op_id=oi.op_id and o.op_subid=oi.op_subid";
                OQN->Execute();
                AnsiString SI=OQN->FieldAsString(0);
                int STO=OQN->FieldAsInteger(1);
                if (STO) SI=(SI=="D")?"K":"D";
                if (SI=="K") SIGH="+";

                OQN->SQL->Text= "select NAME from "+BaseUserName+".objs where oid="+IntToStr(AnalId);
                OQN->Execute();
                AnsiString Anname=OQN->FieldAsString(0);
                OQN->SQL->Text= "select REALNAME, DEF_OPT from "+BaseUserName+".dbstr where dbid="+IntToStr(AnalId);
                OQN->Execute();

                while (!OQN->Eof) {
                  int fl=OQN->FieldAsInteger(1);
                  AnsiString RN, VL;
                  if (fl&RemFld ){
                    RN=OQN->FieldAsString(0);
                    QH->SQL->Text="select AARTICLE, AVER,"+RN+" from "+BaseUserName+"."+Anname+" where sumid="+IntToStr(CurrSumId)+" and AARTICLE!=' '";
                    QH->Execute();
                    while (!QH->Eof){
                      VL=QH->FieldAsString(2);
                      OHQ.SetPLText("update "+BaseUserName+"."+Anname+
                      " set "+RN+"="+RN+SIGH+VL+" where sumid="+IntToStr(RemSumId)+
                      " and AARTICLE= '"+QH->FieldAsString(0)+"' and AVER="+QH->FieldAsString(1)+";");
                      OHQ.Exec("");
                      QH->Next();
                    }
                  }
                  OQN->Next();
                }
              } __finally{
                    OQN->Close();
                    QH->Close();
                    DatMod->ReleaseOQ(OQN);
                    DatMod->ReleaseOQ(QH);
              }
            }
     */
     //--------------------------------EO CH при перепроведении показывать остатки, не учитывая саму операцию

            AnsiString AnalName= A1.GetVarStr("NAME");
            AnsiString DbName= A1.GetVarStr("NAME_DB");
            AnsiString SetName = AQ.GetFldStr("SETNAME");
            SetName = SETName; //EO CH 28.03
            // ********************************************************
            // ********************************************************
// CH 07-05-02
            if (AmsWinC->UseShtrih)
            {
              if (OV->st->Var("FND_FLD").String()!="")
              {
                A1.Clear();
                A1.DeclVarStr("VAL", ToAnsiStr(OV->st->Var("FLD_VAL").String()));
                A1.SetSqlText("select Article, Ver from *."+DbName+" where "+ToAnsiStr(OV->st->Var("FND_FLD").String())+"=:VAL");
                A1.Exec("Обрабатываю ART_FLD=");
                if (A1.NotEof())
                {
                    OV->DelVar("ART");
                    OV->DelVar("V");
                    OV->AddVarStr("ART", A1.GetFldStr(0));
                    OV->AddVarInt("V", A1.GetFldInt(1));
                }
              }
            }

// EO CH 07-05-02

            // ====  Если установлено ART и V ====
            if (OV->st->Var("ART").String()!="")
            {
                A1.Clear();
                A1.DeclVar("ARTICLE",ToAnsiStr(OV->st->Var("ART").String()));
                A1.DeclVarInt("VER", OV->st->Var("V").Number());

                A1.SetSqlTextReady(
                    "begin declare Ins number (1);"
                    "begin "
                        "select max(1) into Ins from "+BaseUserName+"."+DbName+
                        " where ARTICLE=:ARTICLE and VER=:VER;"
                        "delete from "+BaseUserName+"."+AnalName+
                        " where SUMID=:SUMID and not(AARTICLE=:ARTICLE and AVER=:VER);"
                        "if Ins = 1 then "
                        "  select max(1) into Ins from "+BaseUserName+"."+AnalName+
                        "  where SUMID=:SUMID and AARTICLE=:ARTICLE and AVER=:VER;"
                        "  if Ins is NULL then "
                        "    insert into "+BaseUserName+"."+AnalName+
                        "    (SUMID, AARTICLE, AVER) values (:SUMID, :ARTICLE,:VER); "
                        "  end if; "
                        "end if;"
                        "insert into "+BaseUserName+"."+AnalName+
                        " (SUMID, AARTICLE, AVER) values (:SUMID,' ',0); "
                    "commit; end;end;");
                A1.DeclVar("SUMID", NewSumId);
                A1.Exec("Обрабатываю ART= V=");


                if (!OV->st->Var("R").Number())
                {
                    OV->st->Var("R").Fld().SetNumber(1);
                }


            }

            int D =OV->st->Var("D").Number();
            int R =OV->st->Var("R").Number();
            InpOper.Sum_=OV->st->Var("S").Number();
            bool NoEdit= OV->st->Var("E").Number()==0;
            int VM = OV->st->Var("VM").Number();
            int OM = OV->st->Var("OM").Number();
            AnsiString ShtrixFld = OV->st->Var("BAR_FLD").String().data();
            AnsiString ShtrixOrd = OV->st->Var("BAR_ORD").String().data();
            AnsiString NoVerOrd = OV->st->Var("NO_VER_ORD").String().data();
            int NoVerFifo=OV->st->Var("NO_VER_FIFO").Number();

            OV->DelVar("BREAK_OPER");
            OV->DelVar("CANCEL_AUTO_APPLY");
            OV->DelVar("SET_AUTO_APPLY");
            OV->DelVar("SET_BEGIN_BUT");
            OV->DelVar("A");
            OV->DelVar("S");
            OV->DelVar("R");
            OV->DelVar("D");
            OV->DelVar("OM");
            OV->DelVar("CO");
            OV->DelVar("OM");
            OV->DelVar("VM");
            OV->DelVar("E");
            OV->DelVar("ACC");
            OV->DelVar("ART");
            OV->DelVar("V");
            OV->DelVar("CR");
            OV->DelVar("BAR_FLD");
            OV->DelVar("BAR_ORD");
            OV->DelVar("NO_VER_ORD");
            OV->DelVar("NO_VER_FIFO");
//CH 26-04-02
            if (AmsWinC->UseShtrih)
            {
                OV->DelVar("FND_FLD");
                OV->DelVar("FLD_VAL");
                OV->DelVar("SH_C");
            };
//EO CH 26-04-02

            FormAccv(Acc);
            // Определяем правило для распределения
            //(порядок по которому будет списываться позиции)
            AnsiString Rules=" D.VER ";
            if (NoVerFifo==-1)
            {
                AmsQuery AQ;
                AQ.SetSqlText("select POPTS from *.OBJS where OID=:OID");
                AQ.DeclVar("OID",AnalId);
                AQ.Exec("Выбираем установку списание FIFO");
                if (AQ.NotEof())
                    if (AQ.GetFldInt(0) & 2)
                    {
                        Rules=" D.VER ";
                    }
                    else
                        Rules=" D.VER desc ";
            }
            else
            if (NoVerOrd!="" )
            {
                Rules=" "+NoVerOrd;
                if (!NoVerFifo) Rules+=" desc ";
            }
            else
            {
                if (!NoVerFifo) Rules+=" desc ";
            }

            // ====  Распределяем если установлено D=1 ====
            if (D || R)
            {
                TFldsDescrAnDestr FD;
                FD.ReadDescr(AnalName, SetName);
                FD.SetAnSum(RemSumId, NewSumId);
                FD.FormAddAnSums( &FD.AddSumList());
                if (D==1)
                {
                    TAmsDS ADS;
                    TOracleDataSet* ODS= ADS();
                    FD.BuildSqlQuery(ODS);
                    ODS->DeclareVariable("ARTICLE", otString);
                    ODS->SetVariable("ARTICLE", " ");
                    ExecODS(ODS);
                    //ODS->Active= true;
                    FD.IndexFields(ODS);
                    A1.Clear();
                    A1.SetSqlTextReady(
                        "select AARTICLE from "+BaseUserName+"."+AnalName+
                        " where SUMID=:SUMID and AARTICLE!=' ' group by AARTICLE");
                    A1.DeclVar("SUMID", NewSumId);
                    A1.Exec("Артикулы для распределения");
                    while(A1.NotEof())
                    {
                        ODS->Active=false;
                        ODS->SetVariable("ARTICLE", A1.GetFldStr(0));
                        ODS->Active=true;
                        DistrLine (&FD, NewSumId, RemSumId, ODS, Rules);
                        A1.Next();
                    }
                    if (!R) R=1;
                }
                //------ Пересчитываем аналитическую сумму
                if (R)
                {
                    TAmsDS D;
                    TOracleDataSet * ODS1 = D();
                    FD.BuildSqlQueryRecalc(ODS1);
                    FD.Recalc(R==2, ODS1, &LV, R_Equil); //EO CH
                }
            }
            // ********************************************************
            // ********************************************************
            // **************************
            // Вводим режим видимости и пр

            if (! NoEdit)
            {
                FrmBrowsAn = new TFrmBrowsAn(this);
                TFrmBrowsAn::BegMode BM;
                BM.Vars=&LV;
                BM.BalFld=BalFld;
                BM.AccCode=Acc;
                BM.newop=newop;
                if (VM!=-1)
                {
                    BM.ShowVer=false;
                }

                if (OM!=-1)
                {
                    BM.EdMode=OM;
                }
                BM.NoVerOrd=Rules;


                AnsiString Descr= TAccName::ToUserName(AccName)+
                    (AQ.GetFldStr("SIDE")=="D"?" Дебет ":" Кредит ")+AccDescr+T_Equil; //EO CH T=0

                CurrState=enWaitForUserInput;
                InpOper.CurrSumId=CurrSumId;
                InpOper.NewSumId=NewSumId;
                InpOper.AnalName=AnalName;
                InpOper.Acc=Acc;
                InpOper.BalFld=BalFld;
                if (Opts & aoNegRemWarring)
                {
                    BM.CheckNeg=true;
                    BM.PredSumId=PrevState==osWasCommited?CurrSumId:0;
                    BM.AddToRem=AQ.GetFldStr("SIDE")=="D"? (!DSetSTORNO->Value):
                        (DSetSTORNO->Value);
                }
                BM.ZeroVer=Opts & aoZeroVer;

                if (ShtrixFld!="")
                {
                    BM.ShtrixFld=ShtrixFld;
                    BM.ShtrixOrderFld=ShtrixOrd;
                }
                AnsiString SetName=AQ.GetFldStr("SETNAME");
                if (SetName==" " || SetName=="")
                   SetName=SETName; //28.03
                if (SetName=="")
                {
                    AmsQuery A1;
                    A1.SetSqlText("select SETNAME from *.ACCS where CODE=:CODE");
                    A1.DeclVar("CODE", Acc);
                    A1.Exec("Находим набор установок для счета");
                    if (A1.NotEof())
                    {
                        SetName=A1.GetFldStr(0);
                    }
                }
                if (CM_Var>=0) BM.WithoutCat=CM_Var;
                FrmBrowsAn->Begin( this,AnalId, "",
                            SetName,
                            Descr,
                            NewSumId, RemSumId, BM );
            }
            else
            {
                CurrState=enWaitForUserInput;
                InpOper.CurrSumId=CurrSumId;
                InpOper.NewSumId=NewSumId;
                InpOper.AnalName=AnalName;
                InpOper.Acc=Acc;
                InpOper.BalFld=BalFld;
                AnalOk();
            }
        }
        else // Ввод синтетической суммы
        {
            double Sum=0;
            bool NotEdit= !OV->st->Var("E").Number();
            if (OV->st->Var("S").Number()!=0)
            {
                Sum=OV->st->Var("S").Number();
            }
            else
            {
                AmsQuery A1;
                A1.SetSqlTextReady(
                    "select VALUE from "+BaseUserName+".SUMS "
                    "where SUM_ID=:SUM_ID");
                A1.DeclVar("SUM_ID", CurrSumId);
                A1.Exec("Находим синтетическую сумму");
                if (A1.NotEof())
                    Sum=A1.GetFldSum(0);
            }

            if (NotEdit)
            {
                SyntOk(Sum);
            }
            else
            {
                Application->CreateForm(__classid(TFrmInputSumNM), &FrmInputSumNM);
                CurrState=enWaitForUserInputS;
                InpOper.CurrSumId=CurrSumId;
                AnsiString Msg=OV->st->Var("T").String().data();
                if (Msg=="")
                {
                    Msg="Введите сумму по "+TAccName::ToUserName(AccName)+" счету";
                }
                FrmInputSumNM->InputSumNM(this, Msg, Sum);
            }
        }// Ввод аналитики/синтетики юзером

    }
    else   // Все стандартные счета закончились
    {
        // Удаляем счета, которых не было в проводке
        CurrState=enIdle;
        if (AccCodes.size())
        {
            AmsQuery AQ;
            AnsiString AccList="";
            for (unsigned int i=0; i<AccCodes.size();i++)
            {
                if (i) AccList+=", ";
                AccList+=":CODE"+IntToStr(i);
                int Acc = AccCodes[i];
                AQ.DeclVar(":CODE"+IntToStr(i), Acc);
            }
            AQ.DeclVar("OP_ID", op_id);
            AQ.DeclVar("OP_SUBID", op_subid);
            AQ.SetPLText(
                "delete from *.OP_ITEMS "
                "where "
                " OP_ID=:OP_ID and "
                " OP_SUBID=:OP_SUBID and "
                " CODE not in ("+AccList+"); commit;");
            AQ.Exec("Удаляем счета, которых нет в проводке");
            DSetItems->DisableControls();
            DSetItems->Refresh();
            DSetItems->EnableControls();
        }
        this->BringToFront();

        if (SetBeginBut)
            ButBegin->SetFocus();
        else
            BBOk->SetFocus();
        if (AutoGen==1)
        {
            GenerateDocument(DSetDOC_TYPE->AsString);
        }
        if (AutoApply==1)
        {
            BBOkClick(this);
        }
    }// Все счета из стадартных проводок завершились
}
//---------------------------------------------------------------------------
void __fastcall TFrmInputOper::ButBeginClick(TObject *Sender)
{
    Dirty = true;
    DBEdit2->SetFocus();  //EO CH 28.03
    SetBeginBut=0;
    if (DSetDOC_NUM->Value=="")
    {
        ErrorMessage("Номер документа не должен быть пустым");
        DBEdit1->SetFocus();
        return;
    }
    BeginAutoInput();
}
//---------------------------------------------------------------------------
void TFrmInputOper::SyntOk(Sum Sum_)
{
    FrmInputSumNM=NULL;
    do {
        AmsQuery A1;
        A1.SetSqlTextReady(
            "begin update "+BaseUserName+".SUMS "
            "set VALUE=:VALUE where SUM_ID=:SUM_ID; commit; end;");
        A1.DeclVar("SUM_ID", InpOper.CurrSumId);
        A1.DeclVar("VALUE", (double)Sum_);
        A1.Exec("Вставляем синтетическую сумму");
    }while(0);
    AmsQuery CAQ (QCondItems);
    CAQ.Next();
    CycleBody();
}
//---------------------------------------------------------------------------
void TFrmInputOper::SyntCancel(void)
{
    //TODO: Add your source code here
    FrmInputSumNM=NULL;
    CurrState=enIdle;
    DSetItems->DisableControls();
    DSetItems->Refresh();
    DSetItems->EnableControls();
    return ;
}
//---------------------------------------------------------------------------
bool __fastcall TFrmInputOper::WantChildKey(Controls::TControl* Child,
                            Messages::TMessage &Message)
{
/*
    bool Ret = false;
    if (Message.Msg==WM_KEYDOWN)
    {
        TWMKey&  KeyMsg = *((TWMKey*)&Message);
        if (KeyMsg.CharCode==13)
        {
            TControl* C = Child;
            if (C==text_edit)
            {
                DBDateEdit1->SetFocus();
                Ret=true;
            }
            else
            if (C==DBDateEdit1)
            {
                Ret=true;
                DBEdit1->SetFocus();
            }
            if (C==DBEdit1)
            {
                DBEdit2->SetFocus();
                Ret=true;
            }
            if (C==DBEdit2)
            {
                DocNumOk=notChecked;
                ButBegin->SetFocus();
                if (DocNumOk==CheckedOk)
                {
                    ButBeginClick(this);
                }
                Ret=true;
            }
        }
    }
    return Ret;
*/
    return false;
};
//---------------------------------------------------------------------------


void __fastcall TFrmInputOper::NewAcc1Click(TObject *Sender)
{
//
    DSetItems->Insert();
}
//---------------------------------------------------------------------------

void __fastcall TFrmInputOper::DelAcc2Click(TObject *Sender)
{
    if (AskMessage("Удалить субсчет "+DSetItemsANAME->Value+" из проводки")==ID_YES)
    {
        DSetItems->Delete();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmInputOper::ActShowTiedDBExecute(TObject *Sender)
{
//

}
//---------------------------------------------------------------------------

void __fastcall TFrmInputOper::ButEnterClick(TObject *Sender)
{
    //
    TControl* C = ActiveControl;
    if (C==text_edit)
    {
        DBDateEdit1->SetFocus();
    }
    else
    if (C==DBDateEdit1)
    {
        DBEdit1->SetFocus();
    }
    if (C==DBEdit1)
    {
        DBEdit2->SetFocus();
    }
    if (C==DBEdit2)
    {
        DocNumOk=notChecked;
        ButBegin->SetFocus();
        if (DocNumOk==CheckedOk)
        {
            ButBeginClick(this);
        }
    }
}
//---------------------------------------------------------------------------
void TFrmInputOper::FormAccv(int Code)
{
    OpVars* Accv= LV.ACCV();
    AmsQuery AQ;
    AQ.SetSqlText("select * from *.ACCS where CODE=:CODE");
    AQ.DeclVar("CODE", Code);
    AQ.Exec("Формируем Accv");
    if (AQ.NotEof())
        Accv->AddVars(AQ);
}
//---------------------------------------------------------------------------
void TFrmInputOper::ReadTiedBase()
{
    if (!IsReadTiedBase)
    {
        IsReadTiedBase= true;
        AmsQuery AQ;
        AQ.SetSqlText(
          "begin "+BaseUserName+".GetLinkDb (:OP_ID,:OP_SUBID,"
          ":DB_NAME,:ARTICLE,:VER,:TIED_TO_DOC); "
          "select max(POPTS) into :POPTS from *.OBJS where NAME=:DB_NAME; end;");

        AQ.DeclVar("OP_ID",   DSetOP_ID->AsInteger);
        AQ.DeclVar("OP_SUBID",DSetOP_SUBID->AsInteger);
        AQ.DeclVar("DB_NAME", "");
        AQ.DeclVar("ARTICLE", "");
        AQ.DeclVar("VER",     0);
        AQ.DeclVar("TIED_TO_DOC",0);
        AQ.DeclVar("POPTS",0);

        AQ.Exec("Находим имя связанной базы данных");
        TiedBase=AQ.GetVarStr("DB_NAME");
        if (TiedBase!="")
        {
            TiedBaseArticle =AQ.GetVarStr("ARTICLE");
            TiedBaseVer     =AQ.GetVarInt("VER");
            TiedBaseToOper  =!AQ.GetVarInt("TIED_TO_DOC");
            NotShowFirstTimeTDb =AQ.GetVarInt("POPTS") & DBO_NOT_FIRST_SHOW;
        }
    }

}
//---------------------------------------------------------------------------
void __fastcall TFrmInputOper::ActDocGenExecute(TObject *Sender)
{
//
    GenerateDocument(DSetDOC_TYPE->AsString);
}
//---------------------------------------------------------------------------
void __fastcall TFrmInputOper::DSetApplyRecord(TOracleDataSet *Sender,
      char Action, bool &Applied, AnsiString &NewRowId)
{
    if (Action=='C')
    {
        Applied= true;
    }
}
//---------------------------------------------------------------------------
void TFrmInputOper::ShowTiedDbShortInfo()
{
    do {
        AmsQuery AQ;
        AQ.SetSqlText("begin :Res:=*.GetDbShortInfo(:DB_NAME, :ART, :VER); end;");
        AQ.DeclVar("DB_NAME", TiedBase);
        AQ.DeclVar("ART", TiedBaseArticle);
        AQ.DeclVar("VER",TiedBaseVer);
        AQ.DeclVar("RES", "");
        AQ.Exec("Находим короткую информацию для связанной БД");
        LabDbInfo->Caption=AQ.GetVarStr("RES");
    } while (0);
}
//---------------------------------------------------------------------------
void __fastcall TFrmInputOper::ActReadExecute(TObject *Sender)
{
//  JustView Items
    if (!readonly)
    {
        if (DSetItems->State==dsEdit)
        {
            try {
                DSetItems->Post();
                DSetItems->RefreshRecord();
            } catch(...)
            {
                return;
            }
        }
    }
    if(!DSetItemsANAL_ID->IsNull) // Счет аналитический
    {

        AnNewSumId = DSetItemsITEM_SUM_ID->Value;
        int AccCode_ = DSetItemsCODE->Value;
        bool Debet = DSetItemsSIDE->Value=="D";
        AnsiString SetName_= DSetItemsSETNAME->Value;
        AnAnId=DSetItemsANAL_ID->Value;

        AnsiString BalFld;
        int AnOst;
        int Opts;
        AnsiString Descr;
        do {
            AmsQuery AQ;
            AQ.SetSqlText(
                "BEGIN "
                "select max(BAL_FLD), max(NAME), max(DESCR), max(OPTS), max(SETNAME) into "
                ":BAL_FLD, :NAME, :DESCR, :OPTS, :SETNAME from *.ACCS "
                "where CODE=:CODE; :RI:=*.RI(:CODE);commit;END;");
            AQ.DeclVar("BAL_FLD", "");
            AQ.DeclVar("NAME", "");
            AQ.DeclVar("DESCR", "");
            AQ.DeclVar("RI", 0);
            AQ.DeclVar("CODE", AccCode_);
            AQ.DeclVar("OPTS", 0);
            AQ.DeclVar("SETNAME", "");
            AQ.Exec("Находим установки");
            BalFld= AQ.GetVarStr("BAL_FLD");
            AnOst = AQ.GetVarInt("RI");
            Opts = AQ.GetVarInt("OPTS");
            if (SetName_=="") SetName_=AQ.GetVarStr("SETNAME");
            Descr =TAccName::ToUserName(AQ.GetVarStr("NAME"))+
                            (Debet?" Дебет ":" Кредит ")+AQ.GetVarStr("DESCR");
        }while (0);

        FormAccv(AccCode_);
        TFrmBrowsAn* Frm;
        Application->CreateForm(__classid(TFrmBrowsAn), &Frm);
        FrmEditAn=Frm;
        TFrmBrowsAn::BegMode BM;
        BM.Vars=&LV;
        BM.BalFld=BalFld;
        BM.ReadOnly=true;
        BM.ZeroVer=Opts & aoZeroVer;
        BM.newop=newop;

        Frm->Begin(this,AnAnId, "", SetName_, Descr,
            AnNewSumId, AnOst, BM);
    }
}
//---------------------------------------------------------------------------
//CH 26-03-02 & later
int __fastcall TFrmInputOper::AddNewAnSums(int CondId_,int CondSubId_)
{
        if (!CurrAcc) MakeTmpAccs(CondId_, CondSubId_);

        if (TempAccCodes.size()>CurrAcc+1)
        {
            if (!TempAccCodes[CurrAcc+1].NODE)
            {
                AmsQuery B2;
                B2.SetSqlText("select  count(*) from *.OP_ITEMS where OP_ID=:op_id and OP_SUBID=:op_subid and CODE=:code");
                B2.DeclVarInt("op_id", op_id);
                B2.DeclVarInt("op_subid", op_subid);
                B2.DeclVarInt("code", TempAccCodes[CurrAcc+1].CODE);
                B2.Exec("Проверяем наличие записи в оп_айтемсах");
                if ((!TempAccCodes[CurrAcc+1].COND_SUBID)&&(!B2.GetFldInt(0)))
                {
                    AmsQuery B1;
                    int wCode=TempAccCodes[CurrAcc+1].CODE;
                    AnsiString wSide=TempAccCodes[CurrAcc+1].SIDE;
                    AnsiString wCodes=IntToStr(wCode);

                    B1.SetPLText("SELECT *.SumSeq.NEXTVAL into :NewSumId FROM DUAL;"
                                 "insert into *.SUMS (SUM_ID, VALUE) values (:NewSumId, 0);"
                                 " INSERT INTO *.op_items VALUES  (:op_id,:op_subid,:OPDATE,:code,:DC,:NewSumId,0,:FP,:SETNAME);");
                    B1.DeclVarInt("NewSumId",0);
                    B1.DeclVarInt("op_id", op_id);
                    B1.DeclVarInt("op_subid", op_subid);
                    B1.DeclVarDate("OPDATE", OpDate);
                    B1.DeclVarInt("code", TempAccCodes[CurrAcc+1].CODE);
                    B1.DeclVarStr("SETNAME", TempAccCodes[CurrAcc+1].SETNAME);
                    B1.DeclVarStr("DC", TempAccCodes[CurrAcc+1].SIDE);
                    B1.DeclVarStr("FP", TempAccCodes[CurrAcc+1].FP);
                    B1.Exec("Вставляем новую аналит. сумму");
                }
            };
        }
        else CurrAcc = -1;
        CurrAcc++;
}
//-----------------------------------------------------------------------

int __fastcall TFrmInputOper::MakeTmpAccs(int CondId_,int CondSubId_)
{
    AmsQuery B;
    B.SetSqlText(//           0             1       2        3       4            5
                  "select C.COND_SUBID, C.CODE, C.SETNAME, C.SIDE, C.FOR_POST, A.NODE "
                  "from   *.COND_ITEMS C, *.ACCS A where COND_ID=:CI and COND_SUBID=:CSI and C.CODE=A.CODE");
    B.DeclVarInt("CI", CondId_);
    B.DeclVarInt("CSI", CondSubId_);
    B.Exec("");

    TempAccCodes.clear();
    while (B.NotEof())
    {   TAccs A;
        A.COND_SUBID = B.GetFldInt(0);
        A.CODE = B.GetFldInt(1);
        A.SETNAME = B.GetFldStr(2);
        A.SIDE = B.GetFldStr(3);
        A.FP = B.GetFldStr(4);
        A.NODE = B.GetFldInt(5);
        TempAccCodes.push_back(A);
        B.Next();
    }
}

// EO CH
void __fastcall TFrmInputOper::BBCancelMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  Close() ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmInputOper::BBCancelMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  MiceOverClose = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmInputOper::FormMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  MiceOverClose = false;
}
//---------------------------------------------------------------------------

