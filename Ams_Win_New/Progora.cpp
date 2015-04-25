#include "AMS_HDR.h"
#pragma hdrstop
#include "amsclnt.h"
#include "FRM_SEL_LIST.h"
#include "accs.h"
#include "analytic.h"
#include "progan.h"
#include "FRM_ASK_ACC_CODE.h"
#include "FRM_GET_DB_CAT.h"
#include "LOCAL_VARS.h"
#include "AAS_OBJ.h"
#include "ACC_MAN.h"
#include "READ_FILE.h"

extern char       connect_str[41];
extern char       username[41];
extern char       userpass[41];


extern r_char varnameexpectederr,unknownfuncerr,badobjerr,
       nosuchstructerr ;

int SelAcc(cursor &crsr,AccRec &cond);
int DBCatTree(cursor &crsr,int start_cat,int dbid,char *title);

void   Node::MakeOraRec(OraView &view,FieldSet &rec,int clone)
{
 if(!st) MakeStruct();
 Struct &str=Structure();
 FieldLinks &flds=view.FldLinks();                                
 for(int i=0;i<flds.Num();i++)
 {
  OraField *fld=rec.Fld(flds[i]);
  if(clone)
  {
   str.UpdateVar_Clone((char *)flds[i].name,fld);
  }
  else
  {
   str.UpdateVar_Fld((char *)flds[i].name,fld);
  }
 }
}

class AnySel_PC: public Struct {
 cursor crsr;
 OraView      *view;
 FieldDynaSet fs;
 FieldSet   *arg;
 ::string query;
 ::string ord;
public:
 AnySel_PC(const ::string &vn,const char *head,const char *order,const char *group);
 ~AnySel_PC() { delete(view); }
 virtual StackItem Method(Node &node,::string const &method,int pc,StackItem *stk);
};

AnySel_PC::AnySel_PC(const ::string &vn,const char *head,const char *order,
                     const char *group)
:crsr(10000,1000)
{
  //crsr.open(&conn);
  view=new OraView(&crsr,vn.data(),head,group);
  view->MakeRecord(fs);
  query="";
  if(order) { ord=" ORDER BY "; ord+=order; }
  else ord="";
  arg=this;
}

class AnyDML_PC: public Struct {
 cursor crsr;
 OraDML      *dml;
 FieldSet    *arg;
public:
 AnyDML_PC();
 ~AnyDML_PC() { delete(dml); }
 virtual StackItem Method(Node &node,::string const &method,int pc,StackItem *stk);
};

AnyDML_PC::AnyDML_PC()
{
  //crsr.open(&conn);
  dml=new OraDML(&crsr);
  arg=this;
}

StackItem Node::Method(::string const &method,int pc,StackItem *stk)
{
 try {
 if(method=="IS_UNDEF" && pc==0) return VAL(int(vtype()==gcl::VT_NULL));
 if((method=="IS_NULL" || method=="ISNULL") && pc==0)
   return VAL(int(vtype()==gcl::VT_NULL || isNull()));
 if(method=="DAY" && pc==0)    return VAL(int(GetDate().Day()));
 if(method=="MONTH" && pc==0)  return VAL(int(GetDate().Month()));
 if(method=="YEAR" && pc==0)   return VAL(int(GetDate().Year()));
 if(method=="MONNAME" || method=="MONNAME_UKR")
 {
   char buf[100];
   int no_year=1;
   if(pc) no_year=!stk[1-pc].Number();
   return VAL(::string(GetDate().GetMonStr(buf,no_year,method=="MONNAME_UKR")));
 }
 if(method=="SCRIPT" || method=="SCRIPT_UKR")
 {
   char buf[200];
   int date_quote=1;
   if(pc && stk[1-pc].vtype()!=gcl::VT_NULL) date_quote=!stk[1-pc].Number();
   return VAL(::string(GetDate().GetTextStr(buf,method!="SCRIPT",date_quote)));
 }
 if( (method=="NEXTMONTH" || method=="PREVMONTH")&& pc<2)
 {
    DateTime D=GetDate();
    int KM=method=="NEXTMONTH"?1:-1;
    if(pc)
    {
        KM=method=="NEXTMONTH"?stk[0].Number():-stk[0].Number();
    }
    AmsQuery AQ;
    AQ.SetPLTextReady("select add_months(:DAT, :ADD) into :DAT from DUAL;");
    AQ.DeclVar("DAT", D.T());
    AQ.DeclVar("ADD", KM);
    AQ.Exec("Ищем месяцы");
    D=AQ.GetVarDate("DAT");
    return VAL(D);
 }
 if(method=="LEN" && pc==0)    return VAL(int(String().length()));
 if(method=="ARRAY" && pc==1)
 {
   if(st) prog_ptr->RTError(104,varnameexpectederr);
   MakeStruct();
   int r=stk[0].Number();
   for(int i=0;i<r;i++)
   {
    char nm[10];
    sprintf(nm,"el%03d",i);
    st->els.push_back(new Node(nm));
   }
   return This;
 }



 if((method=="ORAVIEW" || method=="VIEW")&& pc>=1)
 {
   if(st) prog_ptr->RTError(104,varnameexpectederr);
   char const *h=0,*o=0,*g=0 ;
   if(pc>1 && stk[2-pc].vtype()==gcl::VT_STRING) h=stk[2-pc].String().data();
   if(pc>2 && stk[3-pc].vtype()==gcl::VT_STRING) o=stk[3-pc].String().data();
   if(pc>3 && stk[4-pc].vtype()==gcl::VT_STRING) g=stk[4-pc].String().data();

   AnySel_PC *PC=new AnySel_PC(stk[1-pc].String(),h,o,g);
   st=PC;
   st->IncC();
   return This;
 }
 if((method=="ORADML" || method=="DML") && pc==0)
 {
   if(st) prog_ptr->RTError(104,varnameexpectederr);
   AnyDML_PC *PC=new AnyDML_PC();
   st=PC;
   st->IncC();
   return This;
  }
  if(method=="DBC" && pc>=1)
  {
   if(st) prog_ptr->RTError(104,varnameexpectederr);
   ::string set="";
   if(pc>1) set=stk[2-pc].String();
   CatDB_PC *PC=new CatDB_PC(stk[1-pc].String(),set);
   st=PC;
   st->IncC();
   return This;
 }
  if(method=="DBEXPIMP" && pc>=1)
  {
   if(st) prog_ptr->RTError(104,varnameexpectederr);
   const char* Pass=NULL;
   const char* UName=NULL;
   if (pc==3)
   {
        UName=stk[2-pc].String().data();
        Pass=stk[3-pc].String().data();
   }
    DbImpExp*PC=new DbImpExp(stk[1-pc].String().data(), UName, Pass);
   st=PC;
   st->IncC();
   return This;
 }
 //------------------------------------------------------------------------
 //  Vlad 04-10-01
 //   Объект для суммирования аналитических сумм
 //------------------------------------------------------------------------
  if(method=="ADDAS" && pc>=1 && pc<5)
  {
    if(st) prog_ptr->RTError(104,varnameexpectederr);
    string AnName;
    int AnCode=0;
    string WhereAdd;
    Struct* S=0;
    if (pc>=1) AnName=stk[1-pc].String();
    if (pc>=2) AnCode=stk[2-pc].Number();
    if (pc>=3) WhereAdd=stk[3-pc].String();
    if (pc>=4) S=stk[4-pc].Stru();

    AasObj* PC=new AasObj(AnCode, AnName, WhereAdd, S);
    st=PC;
    st->IncC();
    return This;
 }
 //------------------------------------------------------------------------
 //  Vlad 05-10-01
 //   Объект для работы с остатками и оборотами по счету
 //------------------------------------------------------------------------
  if(method=="ACCMAN" && pc>1 && pc<5)
  {
    if(st) prog_ptr->RTError(104,varnameexpectederr);
    Struct* S=0;
    DateTime DtBeg;
    DateTime DtEnd;
    string WhereAdd;
    if (pc>=1) DtBeg=stk[1-pc].GetDate();
    if (pc>=2) DtEnd=stk[2-pc].GetDate();
    if (pc>=3) WhereAdd=stk[3-pc].String();
    if (pc>=4) S=stk[4-pc].Stru();

    AccMan* PC=new AccMan(DtBeg, DtEnd, WhereAdd, S);
    st=PC;
    st->IncC();
    return This;
 }
 //------------------------------------------------------------------------
 //  Vlad 12-11-01
 //   Объект для работы с текстовыми файлами
 //------------------------------------------------------------------------
  if(method=="READFILE" && pc==1)
  {
    if(st) prog_ptr->RTError(104,varnameexpectederr);
    VReadFile* PC = new VReadFile(stk[1-pc].String());
    st=PC;
    st->IncC();
    return This;
 }
 //------------------------------------------------------------------------
 if((method=="DB" || method=="DBE") && pc>=1)
 {
   if(st) prog_ptr->RTError(104,varnameexpectederr);
   PosDB_PC *PC;
   int extart=0;
   if(method=="DBE") extart=1;
   if(stk[1-pc].nd && stk[1-pc].nd->st)
   {
    if(pc>1 && stk[2-pc].vtype()!=gcl::VT_NULL) extart=stk[2-pc].Number();
    PC=new PosDB_PC(*stk[1-pc].nd,extart);
   }
   else
   {
    ::string set="";
    if(pc>1) set=stk[2-pc].String();
    if(pc>2 && stk[3-pc].vtype()!=gcl::VT_NULL) extart=stk[3-pc].Number();
    PC=new PosDB_PC(stk[1-pc].String(),extart,set);
   }
   PC->Setup();
   st=PC;
   st->IncC();
   return This;
 }
 if((method=="AN" || method=="ANE") && pc>=2)
 {
   if(st) prog_ptr->RTError(104,varnameexpectederr);
   PosDB_PC *PC=0;
   int SumId;
   int outer=0;
   int extart=0;
   if(method=="ANE") extart=1;
   if(stk[1-pc].nd && stk[1-pc].nd->st)
   {
    SumId=stk[2-pc].Number();
    if(!SumId) prog_ptr->RTError(105,badobjerr);
    if(pc>2 && stk[3-pc].vtype()!=gcl::VT_NULL) outer=stk[3-pc].Number();
    if(pc>3 && stk[4-pc].vtype()!=gcl::VT_NULL) extart=stk[4-pc].Number();
    PC=new PosDB_PC(*stk[1-pc].nd,extart,SumId,outer);
   }
   else
   {
    ::string set="";
    if(pc>1 && stk[2-pc].vtype()!=gcl::VT_NULL) set=stk[2-pc].String();
    SumId=stk[3-pc].Number();
    if(!SumId) prog_ptr->RTError(105,badobjerr);
    if(pc>3 && stk[4-pc].vtype()!=gcl::VT_NULL) outer=stk[4-pc].Number();
    if(pc>4 && stk[5-pc].vtype()!=gcl::VT_NULL) extart=stk[5-pc].Number();
    PC=new PosDB_PC(stk[1-pc].String(),extart,set,SumId,outer);
   }
   PC->Setup();
   st=PC;
   st->IncC();
   return This;
 }
 if((method=="AR"||method=="DBR"||method=="DBAR"||method=="DBAR1") && pc)
 {
   if(st) prog_ptr->RTError(104,varnameexpectederr);
   AnRow *PC;
   const char *article=0;
   int ver=0;
   int mode=(method=="DBR")?1:(method=="AR")?2: (method=="DBAR") ? 3 : 4 ;
   if(mode!=1 && mode!=4)
   {
    if(pc>2 && stk[3-pc].vtype()!=gcl::VT_NULL) article=stk[3-pc].String().data();
    if(pc>3 && stk[4-pc].vtype()!=gcl::VT_NULL) ver=stk[4-pc].Number();
    PC=new AnRow ;
    PC->Init(stk[1-pc].String(),mode,stk[2-pc].Number(),article,ver);
   }
   else if (mode==4)
   {
    if(pc>2 && stk[3-pc].vtype()!=gcl::VT_NULL)
    {
     PC=new AnRow ;
     PC->Init(stk[1-pc].String(),mode,stk[2-pc].Number(),article,ver,(char*)(stk[3-pc].String().data()));
    }
    else
    {
     PC=new AnRow ;
     PC->Init(stk[1-pc].String(),mode,stk[2-pc].Number(),article,ver,"");
    }
   }
   else
   {
    if(pc>1 && stk[2-pc].vtype()!=gcl::VT_NULL) article=stk[2-pc].String().data();
    if(pc>2 && stk[3-pc].vtype()!=gcl::VT_NULL) ver=stk[3-pc].Number();
    PC=new AnRow ;
    ::string cnd="" ;
    if (pc>3)
      cnd=stk[4-pc].String();
    AnsiString S=cnd.data();
    S=ReplaceBaseUser(S);
    cnd=S.c_str();
    PC->Init(stk[1-pc].String(),mode,0,article,ver,NULL,cnd);
   }
   st=PC;
   st->IncC();
   PC->LoadVar(this);
   return This;
 }
 if(st)  return st->Method(This,method,pc,stk);
 else   prog_ptr->RTError(101,::string(unknownfuncerr)+"  "+method);
 }
 catch(OraError Err)
 {
  HeapCheck("inside method error");
  throw;
 }
 return StackItem();
}


StackItem AnySel_PC::Method(Node &node,::string const &method,int pc,StackItem *stk)
{
  if((method=="GET") && pc==1)
  {
   stk[0].getNode().MakeOraRec(*view,fs);
   int ok=view->Do(query+ord,*arg,fs);
   return VAL(ok);
  }
  if(method=="DO" && pc==1)
  {
   stk[0].getNode().MakeOraRec(*view,fs);
   view->Do(query,ord,*arg);
   return StackItem();
  }
  if(method=="FETCH" && pc==0)
  {
   int ok=view->fetch(fs);
   return VAL(ok);
  }
  if(method=="COND" && pc>=1)
  {
   if(pc>=2)
   {
    arg=stk[2-pc].getNode().Stru();
    if (arg==NULL)
       prog_ptr->RTError(301,::string(nosuchstructerr)+"  "+stk[2-pc].nd->name);
   }
   query=" ";
   query+=stk[1-pc].String();
   return node;
  }
  prog_ptr->RTError(101,::string(unknownfuncerr)+"  "+method);
  return StackItem();
}

StackItem AnyDML_PC::Method(Node &node,::string const &method,int pc,StackItem *stk)
{
  if((method=="SQL" || method=="PL") && pc>=1)
  {
   if(pc>=2)
   {
    arg=stk[2-pc].getNode().Stru();
    if (arg==NULL)
       prog_ptr->RTError(301,::string(nosuchstructerr)+"  "+stk[2-pc].nd->name);
   }
   ::string query=stk[1-pc].String();
   if(method!="PL") dml->Do(query,*arg);
   else dml->DoPL(query+";",*arg);
   return node;
  }
  return Struct::Method(node,method,pc,stk);
}

StackItem GlobStruct::Method(Node &node,::string const &method,int pc,StackItem *stk)
{
  if((method=="SQL" || method=="PL") && pc>=1)
  {
   FieldSet *arg;
   if(pc>=2)
   {
    arg=stk[2-pc].getNode().Stru();
    if (arg==NULL)
       prog_ptr->RTError(301,::string(nosuchstructerr)+"  "+stk[2-pc].nd->name);
   }
   else arg=this;

   ::string query=stk[1-pc].String();
   if(method!="PL") Do(query,*arg);
   else DoPL(query+";",*arg);
   return node;
  }
  if(method=="ACCCODE" && pc==1)
  {
   AuxRec aux;
   char b[41];
   strncpy(b,stk[0].String().data(),40);
   b[40]=0;
   AccRec::Normalize(b);
   aux.str1=b;
   DoPL("select code into :int1 from *.accs where name=:str1;",aux);
   return VAL(aux.int1.val);
  }
  if((method=="GETACC") && pc==2)
  {
   int ok;
   AccRec acc;
   OraView av(curs,"*.accs");
   av.MakeRecord(acc);
   if(stk[2-pc].vtype()==gcl::VT_STRING)
   {
    acc.SetName(stk[2-pc].String().data());
    ok=av.Select("name=:name",acc,acc);
   }
   else
   {
    acc.code=stk[2-pc].Number();
    ok=av.Select("code=:code",acc,acc);
   }
   stk[1-pc].getNode().MakeOraRec(av,acc,1);
   return VAL(ok);
  }
  if(method=="OPERAN" && pc>=2)
  {
   AuxRec aux;
   aux.str1=stk[1-pc].String();
   aux.int1=stk[2-pc].Number();
   aux.int2=0;
   if(pc>2) aux.int2=stk[3-pc].Number();
   DoPL("select min(item_sum_id) into :int1 from *.accs a,*.op_items i,*.objs o"
        " where o.name=:str1 and oid=anal_id and a.code=i.code"
        " and op_id=:int1 and op_subid=:int2 ;",aux);
   return VAL(&aux.int1);
  }
  if(method=="OPERDB" && pc>=2)
  {
   AuxRec aux;
   aux.str1=stk[1-pc].String();
   aux.int1=stk[2-pc].Number();
   aux.int2=0;
   if(pc>2) aux.int2=stk[3-pc].Number();
   DoPL("select min(a.code) into :int1 from *.accs a,*.op_items i,*.objs db,*.objs an"
        " where db.name=:str1 and db.oid=an.parent and an.oid=anal_id"
        " and a.code=i.code and op_id=:int1 and op_subid=:int2 ;",aux);
   return VAL(&aux.int1);
  }
  if((method=="CONDSUMID" || method=="CSI") && pc>=1)
  {
   AuxRec aux;
   char b[41];
   strncpy(b,stk[1-pc].String().data(),40);
   b[40]=0;
   AccRec::Normalize(b);
   aux.str1=b;
   if(pc>1 && stk[2-pc].vtype()!=gcl::VT_NULL)
    aux.int1=stk[2-pc].Number();
   else
   {
    Node *opid=FindVar("OP_ID");
    if(opid) aux.int1=opid->Number();
    else aux.int1=0;
   }
   if(pc>2 && stk[3-pc].vtype()!=gcl::VT_NULL)
    aux.int2=stk[3-pc].Number();
   else
   {
    Node *opsubid=FindVar("OP_SUBID");
    if(opsubid) aux.int2=opsubid->Number();
    else aux.int2=0;
   }
   aux.int3=0 ;
   try {
   DoPL("select max(item_sum_id) into :int3 from *.opitems i, *.accs a"
        " where a.name=:str1 and a.code=i.code"
        " and op_id=:int1 and op_subid=:int2 ;",aux);
   } catch(...) {}
   return VAL(&aux.int3);
  }
  //---------------------------------------------------------
  // Модифиц CSI (Возвращает код суммы по
  // № счета в операции, OP_ID , OP_SUBID
  // !!! Необходима функция. Корректно работает при перепроведении счетов
  //---------------------------------------------------------
  if((method=="CSIM") && (pc>=1))
  {
   AuxRec aux;
   aux.int3 = stk[1-pc].Number();
   if(pc>1 && stk[2-pc].vtype()!=gcl::VT_NULL)
    aux.int1=stk[2-pc].Number();
   else
   {
    Node *opid=FindVar("OP_ID");
    if(opid) aux.int1=opid->Number();
    else aux.int1=0;
   }
   if(pc>2 && stk[3-pc].vtype()!=gcl::VT_NULL)
    aux.int2=stk[3-pc].Number();
   else
   {
    Node *opsubid=FindVar("OP_SUBID");
    if(opsubid) aux.int2=opsubid->Number();
    else aux.int2=0;
   }
   try {
   DoPL(":int3 := *.CSIM( :int3, :int1, :int2);",aux);
   } catch(...) {}
   return VAL(&aux.int3);
  }
  //---------------------------------------------------------
  if((method=="CONDSUM" || method=="CS") && pc>=1)
  {
   AuxRec aux;
   char b[41];
   strncpy(b,stk[1-pc].String().data(),40);
   b[40]=0;
   AccRec::Normalize(b);
   //aux.str1=b;
   if(pc>1 && stk[2-pc].vtype()!=gcl::VT_NULL)
    aux.int1=stk[2-pc].Number();
   else
   {
    Node *opid=FindVar("OP_ID");
    if(opid) aux.int1=opid->Number();
    else aux.int1=0;
   }
   if(pc>2 && stk[3-pc].vtype()!=gcl::VT_NULL)
    aux.int2=stk[3-pc].Number();
   else
   {
    Node *opsubid=FindVar("OP_SUBID");
    if(opsubid) aux.int2=opsubid->Number();
    else aux.int2=0;
   }
   AmsQuery AQ ;
   AQ.SetSqlText("select s.value from *.op_items i,*.accs a,*.sums s"
        " where a.name=:str1 and a.code=i.code"
        " and op_id=:int1 and op_subid=:int2 and item_sum_id=sum_id");
   AQ.DeclVar("STR1",(char*)b);
   AQ.DeclVar("INT1",aux.int1.Val());
   AQ.DeclVar("INT2",aux.int2.Val());
   AQ.Exec("Находим синтетическую сумму");
   if (!AQ()->Eof)
   {
        aux.sum1= AQ()->FieldAsFloat(0);
   }
   return VAL(&aux.sum1);
  }
  //---------------------------------------------------------
  // Модифиц CS (Возвращает сумму по
  // № счета в операции, OP_ID , OP_SUBID
  // !!! Необходима функция. Корректно работает при перепроведении счетов
  //---------------------------------------------------------
  if((method=="CSM") && (pc>=1))
  {
   AuxRec aux;
   aux.int3 = stk[1-pc].Number();
   if(pc>1 && stk[2-pc].vtype()!=gcl::VT_NULL)
    aux.int1=stk[2-pc].Number();
   else
   {
    Node *opid=FindVar("OP_ID");
    if(opid) aux.int1=opid->Number();
    else aux.int1=0;
   }
   if(pc>2 && stk[3-pc].vtype()!=gcl::VT_NULL)
    aux.int2=stk[3-pc].Number();
   else
   {
    Node *opsubid=FindVar("OP_SUBID");
    if(opsubid) aux.int2=opsubid->Number();
    else aux.int2=0;
   }
   aux.sum1=0;
   try {
   DoPL(":sum1 := *.CSM( :int3, :int1, :int2);",aux);
   } catch(...) {}
   return VAL(&aux.sum1);
  }

  //---------------------------------------------------------
  if(method=="CST" && pc>=2)
  {
   AuxRec aux;
   char b1[41] , b2[41] ;
   strncpy(b1,stk[1-pc].String().data(),40);
   b1[40]=0;
   AccRec::Normalize(b1);
   aux.str1=b1;
   strncpy(b2,stk[2-pc].String().data(),40);
   b2[40]=0;
   AccRec::Normalize(b2);
   aux.str2=b2;

   if(pc>2 && stk[3-pc].vtype()!=gcl::VT_NULL)
    aux.int1=stk[3-pc].Number();
   else
   {
    Node *opid=FindVar("OP_ID");
    if(opid) aux.int1=opid->Number();
    else aux.int1=0;
   }
   if(pc>3 && stk[4-pc].vtype()!=gcl::VT_NULL)
    aux.int2=stk[4-pc].Number();
   else
   {
    Node *opsubid=FindVar("OP_SUBID");
    if(opsubid) aux.int2=opsubid->Number();
    else aux.int2=0;
   }
   try {
   DoPL("select turn into :sum1 from *.coritems"
        " where dname=:str1 and cname=:str2 AND op_id=:int1 and op_subid=:int2;",aux);
   } catch(...) {}
   return VAL(&aux.sum1);
  }

  if(method=="OPSI_D" && pc==3)
  {
   AuxRec aux;
   char b[41];
   strncpy(b,stk[1-pc].String().data(),40);
   b[40]=0;
   AccRec::Normalize(b);
   aux.str1=b;
   aux.str2=stk[2-pc].String();

   aux.str3=stk[3-pc].String();
   aux.int2=0 ;

   aux.date1=G.ur.beg_date ;
   aux.date2=G.ur.end_date ;
   try
   {
    DoPL("select item_sum_id into :int1 from *.opitems i,*.opers o,*.accs a"
         " where a.name=:str1 and a.code=i.code"
         " and o.doc_type=:str2 and o.doc_num=:str3 and o.op_date>=:date1 and o.op_date<:date2"
         " and o.op_id=i.op_id and o.op_subid=i.op_subid ;",aux);
   } catch(...) {}
   return VAL(&aux.int1);
  }
  if((method=="OPSUMID" || method=="OPSI") && pc>=2)
  {
   AuxRec aux;
   char b[41];
   strncpy(b,stk[1-pc].String().data(),40);
   b[40]=0;
   AccRec::Normalize(b);
   aux.str1=b;
   aux.str2=stk[2-pc].String();

   if(pc>2 && stk[3-pc].vtype()!=gcl::VT_NULL)
    aux.str3=stk[3-pc].String();
   else
   {
    Node *opid=FindVar("DOC_NUM");
    if(opid) aux.str3=opid->String();
    else aux.str3.SetNull();
   }

   if(pc>3 && stk[4-pc].vtype()!=gcl::VT_NULL)
    aux.int2=stk[4-pc].Number();
   else
   {
    Node *opsubid=FindVar("DOC_NUM_V");
    if(opsubid) aux.int2=opsubid->Number();
    else aux.int2=0 ;
   }
   try
   {
    DoPL("select item_sum_id into :int1 from *.opitems i,*.opers o,*.accs a"
         " where a.name=:str1 and a.code=i.code"
         " and o.doc_type=:str2 and o.doc_num=:str3 and o.doc_num_v=:int2"
         " and o.op_id=i.op_id and o.op_subid=i.op_subid ;",aux);
   } // catch(...) {}
   catch(OraError ERR)
   {
    if (ERR.err_code!=1403)
      ERR.message() ;
   }
   return VAL(&aux.int1);
  }
  if((method=="OPSUM" || method=="OPS") && pc>=1)
  {
   AuxRec aux;
   char b[41];
   strncpy(b,stk[1-pc].String().data(),40);
   b[40]=0;
   AccRec::Normalize(b);
   aux.str1=b;
   aux.str2=stk[2-pc].String();

   if(pc>2 && stk[3-pc].vtype()!=gcl::VT_NULL)
    aux.str3=stk[3-pc].String();
   else
   {
    Node *opid=FindVar("DOC_NUM");
    if(opid) aux.str3=opid->String();
    else aux.str3.SetNull();
   }

   if(pc>3 && stk[4-pc].vtype()!=gcl::VT_NULL)
    aux.int2=stk[4-pc].Number();
   else
   {
    Node *opsubid=FindVar("DOC_NUM_V");
    if(opsubid) aux.int2=opsubid->Number();
    else aux.int2=0;
   }
   try {
   DoPL("select s.value into :sum1 from *.opitems i,*.accs a,*.sums s,*.opers o"
        " where a.name=:str1 and a.code=i.code"
        " and o.doc_type=:str2 and o.doc_num=:str3 and o.doc_num_v=:int2"
        " and o.op_id=i.op_id and o.op_subid=i.op_subid"
        " and item_sum_id=sum_id;",aux);
   } catch(...) {}
   return VAL(&aux.sum1);
  }

  if((method=="AC_D" || method=="AC_C") && pc>=1)
  {
   char buf[40] ;
   OpItemRec inr ;
   AccRec outr ;

   int count=stk[1-pc].Number() ;

   if(pc>1 && stk[2-pc].vtype()!=gcl::VT_NULL)
    inr.op_id=stk[2-pc].Number();
   else
   {
    Node *opid=FindVar("OP_ID");
    if(opid) inr.op_id=opid->Number();
    else inr.op_id=0;
   }
   if(pc>2 && stk[3-pc].vtype()!=gcl::VT_NULL)
    inr.op_subid=stk[3-pc].Number();
   else
   {
    Node *opsubid=FindVar("OP_SUBID");
    if(opsubid) inr.op_subid=opsubid->Number();
    else inr.op_subid=0;
   }

   inr.side= (method=="AC_D" ? "D" : "K") ;

   {
    OraView iv(curs,"*.op_items oi,*.accs a","a.*") ;
    iv.MakeRecord(outr) ;

    iv.Do(" where a.code=oi.code and oi.op_id=:op_id and a.node=0 and oi.op_subid=:op_subid and oi.side=:side ORDER BY name",inr);
    for(int i=1 ; iv.fetch(outr) ; i++)
    {
     if (i==count)
       return VAL(outr.UserName(buf)) ;
    }
   }

   return VAL("");
  }


  if((method=="R" && pc>=1) || (method=="BR" && pc==1))
  {
   AuxRec aux;
   char accname[40];
   strcpy(accname,stk[1-pc].String().data());
   AccRec::Normalize(accname);
   aux.str1=accname;
   if(method=="BR") aux.date1=G.BegDate();
   else if(pc>1) aux.date1=stk[2-pc].GetDate();
        else aux.date1=G.EndDate();
   try {
/*
   DoPL("select value into :sum1 from *.rems r,*.accs a,*.sums s "
         "where a.name=:str1 and r.code=a.code and rem_date=:date1"
         " and end_val_id=s.sum_id;",aux);
*/
    DoPL(":sum1:=*.REST(:str1,:date1);",aux);
   }
   catch(OraError Err)
   {
    OraErr(Err);
    aux.sum1.SetNull();
   }
   return VAL(&aux.sum1);
  }
  if((method=="RI" || method=="BRI") && pc>=1)
  {
   AuxRec aux;
   char accname[40];
   strcpy(accname,stk[1-pc].String().data());
   AccRec::Normalize(accname);
   aux.str1=accname;
   if(method=="BRI") aux.date1=G.BegDate();
   else if(pc>1) aux.date1=stk[2-pc].GetDate();
        else aux.date1=G.EndDate();
   try {
   DoPL("select end_val_id into :int1 from *.rems r,*.accs a "
         "where a.name=:str1 and r.code=a.code and rem_date=:date1;",aux);
   }
   catch(OraError Err)
   {
    OraErr(Err);
    aux.int1.SetNull();
   }
//   catch(...) {}
   return VAL(&aux.int1);
  }
  if((method=="R_AS") && pc==2)
  {
   AuxRec aux;
   char accname[40];
   strcpy(accname,stk[1-pc].String().data());
   AccRec::Normalize(accname);
   aux.str1=accname;
   aux.date1=stk[2-pc].GetDate();
   try {
   DoPL(":int1:=*.R_AS(:str1,:date1);",aux);
   }
   catch(OraError Err)
   {
    OraErr(Err);
    aux.int1.SetNull();
   }
   return VAL(&aux.int1);
  }
  if((method=="DT_AS" || method=="CT_AS") && pc==3)
  {
   AuxRec aux;
   char accname[40];
   strcpy(accname,stk[1-pc].String().data());
   AccRec::Normalize(accname);
   aux.str1=accname;
   aux.date1=stk[2-pc].GetDate();
   aux.date2=stk[3-pc].GetDate();
   aux.str2= method=="DT_AS" ? "D" : "K" ;
   try {
    DoPL(":int1:=*.DT_CT_AS(:str1,:date1,:date2,:str2);",aux);
   }
   catch(OraError Err)
   {
    OraErr(Err);
    aux.int1.SetNull();
   }
   return VAL(&aux.int1);
  }
  if((method=="DT_AS_COR" || method=="CT_AS_COR") && pc>3)
  {
   AuxRec aux;
   char accname[40] , corname[40] ;
   strcpy(accname,stk[1-pc].String().data());
   AccRec::Normalize(accname);
   aux.str1=accname;
   aux.date1=stk[2-pc].GetDate();
   aux.date2=stk[3-pc].GetDate();
   aux.str2= (method=="DT_AS_COR" ? "D" : "K") ;

   strcpy(corname,stk[4-pc].String().data());
   AccRec::Normalize(corname);
   aux.str3=corname;

   aux.int2.SetNull() ;
   if (pc>4)
     aux.int2=stk[5-pc].Number() ;

   try {
    DoPL(":int1:=*.DT_CT_AS_COR(:str1,:date1,:date2,:str2,:str3,:int2);",aux);
   }
   catch(OraError Err)
   {
    OraErr(Err);
    aux.int1.SetNull();
   }
   return VAL(&aux.int1);
  }

  if((method=="DT_AS_TEXT" || method=="CT_AS_TEXT") && pc>3)
  {
   AuxRec aux;
   char accname[40] , corname[40] ;
   strcpy(accname,stk[1-pc].String().data());
   AccRec::Normalize(accname);
   aux.str1=accname;
   aux.date1=stk[2-pc].GetDate();
   aux.date2=stk[3-pc].GetDate();
   aux.str2= (method=="DT_AS_TEXT" ? "D" : "K") ;
   aux.str3=stk[4-pc].String().data();

   aux.int2.SetNull() ;
   if (pc>4)
     aux.int2=stk[5-pc].Number() ;

   try {
    DoPL(":int1:=*.DT_CT_AS_TEXT(:str1,:date1,:date2,:str2,:str3,:int2);",aux);
   }
   catch(OraError Err)
   {
    OraErr(Err);
    aux.int1.SetNull();
   }
   return VAL(&aux.int1);
  }

  if((method=="CR" || method=="DR" || method=="BCR" || method=="BDR") && pc>=1)
  {
   AuxRec aux;
   char accname[40];
   strcpy(accname,stk[1-pc].String().data());
   AccRec::Normalize(accname);
   aux.str1=accname;

   if(method=="BCR" || method=="BDR") aux.date1=G.BegDate();
   else if(pc>1) aux.date1=stk[2-pc].GetDate();
   else aux.date1=G.EndDate();

   aux.int2= (method=="DR" || method=="BDR") ;
   try {
    DoPL("select code into :int1 from *.accs where name=:str1; ",aux);
    DoPL(":sum1:=*.ExpandedSaldo(:int1,:date1,:int2);",aux);
   }
   catch(OraError Err)
   {
    OraErr(Err);
    aux.sum1.SetNull();
   }
   return VAL(&aux.sum1);
  }
  if((method=="DT" || method=="CT") && pc>=1)
  {
   AuxRec aux;
   char accname[40];
   strcpy(accname,stk[1-pc].String().data());
   AccRec::Normalize(accname);
   aux.str1=accname;
   if(pc>1 && stk[2-pc].vtype()!=gcl::VT_NULL) aux.date1=stk[2-pc].GetDate();
   else aux.date1=G.BegDate();
   if(pc>2 && stk[3-pc].vtype()!=gcl::VT_NULL) aux.date2=stk[3-pc].GetDate();
   else aux.date2=G.EndDate();
   aux.int1=(method=="DT" ? 0 : 1 ) ;
   try {
/*
    ::string sql="select value into :sum1 from *.turns t,*.accs a,*.sums s "
         "where a.name=:str1 and t.code=a.code and "
         "beg_date=:date1 and end_date=:date2 and s.sum_id=";
    sql+=(method=="DT")?"d":"c";
    sql+="_turn_id;";
    DoPL(sql,aux);
*/
    DoPL(":sum1:=*.DCTURN(:str1,:date1,:date2,:int1);",aux);
   }
   catch(OraError Err)
   {
    OraErr(Err);
    aux.sum1.SetNull();
   }
   return VAL(&aux.sum1);
  }
  if((method=="DTI" || method=="CTI") && pc>=1)
  {
   AuxRec aux;
   char accname[40];
   strcpy(accname,stk[1-pc].String().data());
   AccRec::Normalize(accname);
   aux.str1=accname;
   if(pc>1 && stk[2-pc].vtype()!=gcl::VT_NULL) aux.date1=stk[2-pc].GetDate();
   else aux.date1=G.BegDate();
   if(pc>2 && stk[3-pc].vtype()!=gcl::VT_NULL) aux.date2=stk[3-pc].GetDate();
   else aux.date2=G.EndDate();
   try {
    ::string sql="select ";
    sql+=(method=="DTI")?"d":"c";
    sql+="_turn_id into :int1 from *.turns t,*.accs a "
         "where a.name=:str1 and t.code=a.code and "
         "beg_date=:date1 and end_date=:date2;";
    DoPL(sql,aux);
   }
   catch(OraError Err)
   {
    OraErr(Err);
    aux.int1.SetNull();
   }
   return VAL(&aux.int1);
  }

  if(method=="TURNS" && (pc==2 || pc==4))
  {
   AuxRec aux;
   char daccname[40], caccname[40] ;

   strcpy(daccname,stk[1-pc].String().data());
   AccRec::Normalize(daccname);
   aux.str1=daccname;

   strcpy(caccname,stk[2-pc].String().data());
   AccRec::Normalize(caccname);
   aux.str2=caccname;

   if(pc==4)
   {
    aux.date1=stk[3-pc].GetDate();
    aux.date2=stk[4-pc].GetDate();
   }
   else
   {
    aux.date1=G.BegDate();
    aux.date2=G.EndDate();
   }

   aux.sum1=0 ;
   MakeSilent();
   DoPL(":sum1:=*.DCCORTURN(:str1,:str2,:date1,:date2);",aux);
   return VAL(&aux.sum1);
  }

  if(method=="ASK_ACC" || method=="ASK_ACCN" ||
         method=="ASK_SUBACC" || method=="ASK_SUBACCN" )
  {
    TFrmAskAccCode* FrmAccCode;
    Application->CreateForm(__classid (TFrmAskAccCode), &FrmAccCode);

    if(pc>0 && stk[1-pc].vtype()==gcl::VT_STRING)
    {
        AnsiString BegCode = ToAnsiStr(stk[1-pc].String());
        if (method=="ASK_ACC" || method=="ASK_SUBACC")
        {
            int Res = FrmAccCode->AskAccCode(BaseUserName, BegCode);
            FrmAccCode->Release();
            return VAL(Res);
        }
        else
        {
            AnsiString Res = FrmAccCode->AskAccName(BaseUserName, BegCode);
            FrmAccCode->Release();
            return VAL(ToString(Res));
        }
    }
    else
    {
        if (pc>0 && stk[1-pc].vtype()!=gcl::VT_NULL)
        {
            int BegCode = stk[1-pc].Number();
            if (method=="ASK_ACC" || method=="ASK_SUBACC")
            {
                int Res = FrmAccCode->AskAccCode(BaseUserName, BegCode);
                FrmAccCode->Release();
                return VAL(Res);
            }
            else
            {
                AnsiString Res = FrmAccCode->AskAccName(BaseUserName, BegCode);
                FrmAccCode->Release();
                return VAL(ToString(Res));
            }
        }
        else
        {
            if (method=="ASK_ACC" || method=="ASK_SUBACC")
            {
                int Res = FrmAccCode->AskAccCode(BaseUserName, -1);
                FrmAccCode->Release();
                return VAL(Res);
            }
            else
            {
                AnsiString Res = FrmAccCode->AskAccName(BaseUserName, -1);
                FrmAccCode->Release();
                return VAL(ToString(Res));
            }
        }
    }
  }
  if(method=="ASK_CAT" && pc>=1)
  {
   AnsiString CurrCaption = "";
   AmsQuery AQ;
   AQ.SetSqlTextReady("select OID from "+BaseUserName+".OBJS where NAME = :NAME");
   AnsiString ObjName = ToAnsiStr(stk[1-pc].String());
   AQ.DeclVar("NAME", ObjName);
   AQ.Exec("");
   int oid = 0;
   if (!AQ()->Eof)
   {
        oid = AQ()->FieldAsInteger(0);
   }
   if(pc>1)
        CurrCaption=(char *)stk[2-pc].String().data();

   TFrmGetDbCat* Frm ;
   Application->CreateForm(__classid(TFrmGetDbCat), &Frm);
   if (CurrCaption != "")
        Frm->Caption = CurrCaption;
   int Res = Frm->GetDbCat(oid, ObjName);
   Frm->Release();
   return VAL(Res);
  }

  if(method=="ADD_PREF" && pc==2)
  {
   ObjRec obj;
   GetObj(stk[1-pc].String(),obj,curs);

   DBSI[dbsnum].dbid=obj.oid ;
   strcpy(DBSI[dbsnum].prefix,stk[2-pc].String().data()) ;
   dbsnum++ ;
   return VAL("");
  }

  if(method=="DEL_PREF" && pc==0)
  {
   DBSI[dbsnum].dbid=0 ;
   dbsnum-- ;
   return VAL("");
  }

  if(method=="ADD_CHANGE" && pc==3)
  {
   ObjRec obj;
   GetObj(stk[1-pc].String(),obj,curs);

   DBCI[dbcnum].dbid=obj.oid ;
   DBCI[dbcnum].fld1=stk[2-pc].Number() ;
   DBCI[dbcnum].fld2=stk[3-pc].Number() ;
   dbcnum++ ;
   return VAL("");
  }

  if(method=="ASK_FLD" && pc==2)
  {
   AnsiString OName= stk[1-pc].String().data();
   rus_strupra(OName);
   char *title=(char *)stk[2-pc].String().data();
   AmsQuery AQ;
   AQ.SetSqlText("select S.REALNAME, S.ALIAS, nvl(O.OPT,0) OPT  "
    "from *.DBSTR S, *.DBOPTOV O "
    "where "
	"S.DBID = (select OID from *.OBJS where NAME = :NAME) and "
	"S.DBID = O.DBID (+) and "
	"O.SETNAME (+) is NULL and "
	"S.REALNAME = O.REALNAME (+)");
   AQ.DeclVar("NAME", OName);
   AQ.Exec("Поля базы данных или аналитики");
   TFrmSelList* Frm ;
   Application->CreateForm( __classid(TFrmSelList), &Frm);
   AnsiString Res = Frm->AskFieldStr(AQ(), title);
   Frm->Release();
   return VAL(ToString(Res));
  }
  if(method=="OPV" && pc==0)
  {
    return *(GenLocalVars->OPV());
  }
  if(method=="ACCV" && pc==0)
  {
    return *(GenLocalVars->ACCV());
  }
  // -----------------------------------------------
  // Строим отчет в ReportBuildere
  // -----------------------------------------------
  /* Через DLL
  if((method=="BUILDREP") && (pc==5))
  {
   char SessId[30];
   char RepName[30];
   char FileName[30];
   int BegLine, BegCol;
   strcpy(SessId,stk[1-pc].String().data());
   strcpy(RepName,stk[2-pc].String().data());
   strcpy(FileName,stk[3-pc].String().data());
   BegLine = stk[4-pc].Number();
   BegCol = stk[5-pc].Number();
   BuildRep (SessId, RepName, FileName, BegLine, BegCol);
   return VAL(0);
  }
  // -----------------------------------------------
  // Инициализация RepBuildera
  // -----------------------------------------------
  if(method=="INITREP")
  {
   InitRepBldr  (def_shema.data(), username, userpass );
   return VAL(0);
  }
  */
  // -----------------------------------------------
  // Вызов из командной строки
  // -----------------------------------------------
  if((method=="BUILDREP") && (pc==4))
  {
   char SessId[30];
   char RepName[30];
   char FileName[30];
   int BegLine, BegCol;

   strcpy(RepName,stk[1-pc].String().data());
   strcpy(FileName,stk[2-pc].String().data());
   BegLine = stk[3-pc].Number();
   BegCol = stk[4-pc].Number();
   char Cmd [500];
   char NumbStr [10];
   strcpy(Cmd, "strt_repbldr.bat ");
   strcat(strcat(Cmd, def_shema.data())," ");
   strcat(strcat(Cmd, username)," ");
   strcat(strcat(Cmd, userpass)," ");
   strcat(strcat(Cmd, connect_str)," ");

   AuxRec aux;

   DoPL("select DBMS_SESSION.unique_session_id() into :str1 from DUAL;",aux);
   strcat(strcat(Cmd, FileName)," ");
   strcat(strcat(Cmd, RepName)," ");
   strcat(strcat(Cmd, aux.str1.val.c_str())," ");
   strcat(strcat(Cmd, itoa(BegLine, NumbStr, 10))," ");
   strcat(strcat(Cmd, itoa(BegCol, NumbStr, 10))," ");

   system(Cmd);
   return VAL(0);
  }
  //-----------------------------------------------
  if((method=="GETSESSIONID") && pc==0)
  {
   AuxRec aux;
   char Result[60];
   try {
    DoPL("select DBMS_SESSION.unique_session_id() into :str1 from DUAL;",aux);
    strcpy (Result, aux.str1.val.c_str());
   }
   catch(OraError Err)
   {
    OraErr(Err);
    *Result=0;
   }

   return VAL(Result);
  }
  //-----------------------------------------------
  if((method=="SETREPPARAM") && pc==3)
  {
   AuxRec aux;
   aux.str1 = stk[1-pc].String(); // Название Отчета
   aux.str2 = stk[2-pc].String(); // Название параметра
   aux.str3 = stk[3-pc].String(); // Значение

   int Res=0;
   try {
    DoPL("*.SetRepParam(:str1, :str2, :str3);",aux);
   }
   catch(OraError Err)
   {
    OraErr(Err);
    Res=-1;
   }
   return VAL(Res);
  }
  // -----------------------------------------------
  // -----------------------------------------------
  if((method=="CREATETMPSUM") && pc==0)
  {
   AuxRec aux;
   try {
   DoPL("*.CreateSum(:int1,0);",aux);
   }
   catch(OraError Err)
   {
    OraErr(Err);
    aux.int1.SetNull();
   }
   return VAL(&aux.int1);
  }
  // -----------------------------------------------
  if((method=="DELTMPSUM") && pc==1)
  {
   AuxRec aux;
   aux.int2=stk[1-pc].Number();
   aux.int1=1;
   try {
   DoPL("delete *.SUMS where SUM_ID=:int2;",aux);
   }
   catch(OraError Err)
   {
    OraErr(Err);
    aux.int1.SetNull();
   }
   return VAL(&aux.int1);
  }
  // -----------------------------------------------

  return Struct::Method(node,method,pc,stk);
} ;

