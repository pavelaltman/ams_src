#ifndef __ACCS_H
#define __ACCS_H
#include "orcpp.h"
#include "sum.h"
//#include "orabrws.h"
#include <typeinfo.h>
#include <math.h>
#include "prog.h"

#define DOC_TYPE_LEN 20
#define DOC_NUM_LEN  30
#define TEXT_LEN     250

#define DOC_TYPE_VIS 10
#define DOC_NUM_VIS  12

extern connection conn;
void OraErr(OraError &ERR);

typedef int Code;

#define AF_CHECKPOS   1
#define AF_WARNPOS    2
#define AF_EXPANDED   4
#define AF_ALWAYSONE  8

class AccRec: public FieldSet {
public:
 Field<orarowid>   row_id;
 Field<Code>       code;
 Field<::string>     name;
 Field<Code>       parent;
 Field<int>        bal,node;
 Field<::string>     descr;
 Field<Code>       rev_code;
 Field<::string>     val_code;
 Field<::string>     setname ;
 Field<int>        opts;
 Field<int>        expanded ;
 Field<int>        retc;
 Field<int>        anal_id;
 Field<::string>     bal_fld;
 Field<Sum>        sum ;
 void MakeLink(FieldLink &lnk, int inp, int sn);
 FieldSet *clone() { return new AccRec(This); }
 char *     UserName(char *buf,int pref_out=0);
 void       SetName(const char *buf);
static void NormalizeItem(char *it,int l=2,char *oit=NULL);
static void Normalize(char *nam,char *onam=NULL);

 Sum &GetBalanceSum(cursor *crsr,int sum_id) ;
};

class CpRec: public FieldSet {
public:
 Field<DateTime> point_date ;
 Field<int>    is_start ;
 void MakeLink(FieldLink &lnk, int inp, int sn);
 FieldSet *clone() { return new CpRec(This); }
};

class CintRec: public FieldSet {
public:
 Field<orarowid> row_id ;
 Field<DateTime>   beg_date ;
 Field<DateTime>   end_date ;
 void MakeLink(FieldLink &lnk, int inp, int sn);
 FieldSet *clone() { return new CintRec(This); }
};

class UserOptRec: public FieldSet {
public:
 Field<orarowid>   row_id ;
 Field<DateTime>   beg_date ;
 Field<DateTime>   end_date ;
 Field<::string>     username ;
 Field<::string>     full_name ;
 Field<int>        sum_len ;
 Field<int>        sum_dig ;
 Field<int>        apps ;
 Field<DateTime>   inp_date ;
 Field<int>        cur_dates ;
 Field<int>        dig_input ;
 Field<int>        sort_num ;
 Field<DateTime>   dead_date ;
 Field<int>        readonly ;
 Field<int>        overwrite ;
 Field<int>        set_to_end ;
 Field<int>        imp_opt ;

 void MakeLink(FieldLink &lnk, int inp, int sn);
 FieldSet *clone() { return new UserOptRec(This); }
};

class BookNameRec: public FieldSet {
public:
 Field<::string>     username ;
 Field<::string>     bookname ;
 void MakeLink(FieldLink &lnk, int inp, int sn);
 FieldSet *clone() { return new BookNameRec(This); }
};

class AuxRec: public FieldDynaSet {
public:
 Field<orarowid>   row_id ;
 Field<::string>     str1,str2,str3;
 Field<DateTime>   date1,date2;
 Field<int>        int1,int2,int3;
 Field<Sum>        sum1,sum2;
 void MakeLink(FieldLink &lnk, int inp, int sn);
// FieldSet *clone() { return new AuxRec(This); }
};

class AnalErrRec: public FieldSet {
public:
 Field<int>        sid ;
 Field<int>        code ;
 Field<::string>     aarticle ;
 Field<int>        aver ;
 Field<::string>     name ;
 void MakeLink(FieldLink &lnk, int inp, int sn);
 FieldSet *clone() { return new AnalErrRec(This); }
};

class AccTable: protected OraTable {
public:
      AccTable(cursor *cur,char *name="*.ACCS"): OraTable(cur,name) {}
  void MakeRecord(AccRec &rec) { OraTable::MakeRecord(rec); }
  void SetInOut(FieldSet const &inrec,AccRec &outrec)
         { MakeRecord(outrec); Bind(inrec); Define(); }
  void SetIn(FieldSet const &inrec) { Bind(inrec); Define(); }
  int  fetch(AccRec &recb) { OraTable::fetch(recb); return 0;}
  int  ExFetchOne(AccRec &recb,int exact=1)
         { OraTable::ExFetchOne(recb,exact); return 0;}
  int  ExFetchFirst(AccRec &recb) { return ExFetchOne(recb,0); }
  int  Do(const ::string _stmt,FieldSet const &inrec)
         { OraTable::Do(_stmt,inrec); return 0;}
  int  Do(const ::string _stmt,FieldSet const &inrec,AccRec &outrec)
         { OraTable::Do(_stmt,inrec,outrec); return 0;}
  int  DoPL(const ::string _stmt,FieldSet &inoutrec)
         { OraDML::DoPL(_stmt,inoutrec);return 0; }
  int InsertRec(AccRec &recb) { OraTable::InsertRec(recb);return 0; }
  int UpdateRec_ID(AccRec &recb) { OraTable::UpdateRec_ID(recb);return 0;}
  int DeleteRec_ID(AccRec &recb) { OraTable::DeleteRec_ID(recb); return 0;}
};

enum OBJ_Type {OT_DOC=1,OT_FORM=2,OT_SCOND=3,OT_DB=4,OT_ANAL=5,OT_CONST=6,OT_STRUCT=7,OT_ACC=10,OT_CAT=11};

class Cat_Obj_Rec: public FieldSet {
public:
 Field<orarowid>   row_id;
 Field<Code>       catid;
 Field<int>        ord;
 Field<::string>     descr;
 Field<int>        retc;
 Cat_Obj_Rec();
 virtual void MakeLink(FieldLink &lnk, int inp, int sn);
};

class CatRec: public Cat_Obj_Rec {
public:
 Field<Code>       dbid,owncat;
 Field<int>        otype;
 virtual void MakeLink(FieldLink &lnk, int inp, int sn);
 FieldSet *clone() { return new CatRec(This); }
};

class CatTreeRec: public CatRec {
public:
 Field<int>       lev,tord,last,levm;
 CatTreeRec();
 void MakeLink(FieldLink &lnk, int inp, int sn);
 FieldSet *clone() { return new CatTreeRec(This); }
};

class ObjRec: public Cat_Obj_Rec {
public:
 Field<Code>       oid;
 Field<int>        otype;
 Field<::string>     name;
 Field<::string>     opts;
 Field<int>        dup;
 Field<int>        autonum ;
 Field<int>        popts ;
 Field<int>        range ;
 Field<int>        hist ;
 Field<int>        cats ;
 Field<Code>       parent;
 Field<int>        const_type ;
 Field<::string>     value;
 ObjRec();
 void MakeLink(FieldLink &lnk, int inp, int sn);
 FieldSet *clone() { return new ObjRec(This); }
};

#define DTO_ALLOW    1
#define DTO_ASK      2
#define DTO_AUTONUM  4
#define DTO_FFAFTER  8
#define DTO_CONDENS  16
#define DTO_8LPI     32
#define DTO_FOREXCEL 64
#define DTO_YEAR     128
#define DTO_MONTH    256
#define DTO_LAND     512

#define DTO_PMASK    632

#define AO_HIERSORT    1
#define AO_FIFO        2
#define AO_MOVETORIGHT 4

#define DBO_NOFIRST 1

#define SCO_APPLY   1
#define SCO_FORPOST 2

int ChooseObj(OBJ_Type type,ObjRec *resrec,int up_shft=0,char *tit=NULL) ;
void WorkObj(OBJ_Type type,int up_shft=0) ;


// Conds
class CondRec: public FieldSet {
public:
 Field<orarowid> row_id;
 Field<int>      cond_id ;
 Field<int>      cond_subid ;
 Field<int>      ord ;
 Field<::string>   text ;
 Field<::string>   doc_type ;
 Field<int>      storno ;
 Field<int>      native_doc ;
 Field<int>      auto_apply ;
 Field<int>      for_post ;
 Field<int>      max_id ;
 Field<::string>   pre_formula ;
 void MakeLink(FieldLink &lnk, int inp, int sn);
 FieldSet *clone() { return new CondRec(This); }
};

class CondItemRec: public FieldSet {
public:
 Field<orarowid> row_id;
 Field<int>      cond_id ;
 Field<int>      cond_subid ;
 Field<int>      code ;
 Field<::string>   setname ;
 Field<::string>   side ;
 Field<int>      for_post ;
 Field<int>      item_num ;
 Field<::string>   item_formula ;
 Field<int>      max_item ;
 void MakeLink(FieldLink &lnk, int inp, int sn);
 FieldSet *clone() { return new CondItemRec(This); }
};


// Opers
class OpRec: public FieldSet {
public:
 Field<orarowid> row_id;
 Field<int>      op_id ;
 Field<int>      op_subid ;
 Field<int>      status ;
 Field<int>      post ;
 Field<int>      storno ;
 Field<::string>   text ;
 Field<::string>   doc_type ;
 Field<::string>   doc_num ;
 Field<int>      doc_num_v ;
 Field<DateTime> op_date ;
 Field<int>      cond_id ;
 Field<int>      cond_subid ;
 Field<int>      edited ;
 Field<int>      result ;
 Field<DateTime> sdate ;
 Field<DateTime> edate ;
 Field<int>      old_id ;
 Field<int>      old_subid ;
 Field<int>      next_auto ;
 void MakeLink(FieldLink &lnk, int inp, int sn);
 FieldSet *clone() { return new OpRec(This); }
};

class OpItemRec: public FieldSet {
public:
 Field<orarowid>    row_id;
 Field<int>         op_id ;
 Field<int>         op_subid ;
 Field<DateTime>    op_date ;
 Field<int>         code ;
 Field<::string>      side ;
 Field<int>         item_sum_id ;
 Field<int>         node ;
 Field<int>         bal ;
 Field<int>         for_post ;
 Field<Sum>         value ;
 Field<::string>      name ;
 Field<int>         anal_id ;
 Field<::string>      anal_info ;
 Field<::string>      setname ;
 void MakeLink(FieldLink &lnk, int inp, int sn);
 FieldSet *clone() { return new OpItemRec(This); }

 char *UserName(char *buf) ;
};

class OperationRec: public FieldDynaSet {
public:
 Field<orarowid> row_id;
 Field<int>      op_id ;
 Field<int>      op_subid ;
 Field<int>      status ;
 Field<int>      post ;
 Field<int>      storno ;
 Field<::string>   text ;
 Field<::string>   doc_type ;
 Field<::string>   doc_num ;
 Field<int>      doc_num_v ;
 Field<DateTime> op_date ;
 Field<int>      cond_id ;
 Field<int>      cond_subid ;
 Field<int>      result ;

 Field<int>      code ;
 Field<::string>   side ;
 Field<int>      item_sum_id ;
 Field<Sum>      item_sum ;
 Field<int>      node ;
 Field<int>      bal ;
 Field<int>      for_post ;
 Field<Sum>      value ;
 Field<Sum>      c_value ;
 Field<::string>   setname ;
 Field<::string>   name ;
 Field<::string>   art;
 Field<int>      v;

 Field<::string>   aarticle ;
 Field<int>      aver ;
 Field<int>      sumid ;
 Field<DateTime> rem_date ;

 void MakeLink(FieldLink &lnk, int inp, int sn);
 FieldSet *clone() { return new OperationRec(This); }
};


class SumRec: public FieldSet {
public :
 Field<orarowid>    row_id;
 Field<int>         sum_id ;
 Field<Sum>         value ;
 void MakeLink(FieldLink &lnk, int inp, int sn);
 FieldSet *clone() { return new SumRec(This); }
} ;


class ApplyRec: public FieldSet {
public:
 Field<int>      op_id ;
 Field<int>      op_subid ;
 Field<int>      result ;
 Field<::string>   errm ;
 void MakeLink(FieldLink &lnk, int inp, int sn);
 FieldSet *clone() { return new ApplyRec(This); }
};


class CopyOperRec: public FieldSet {
public:
 Field<int>      old_id ;
 Field<int>      old_subid ;
 Field<int>      new_id ;
 Field<int>      new_subid ;
 Field<int>      result ;
 Field<int>      new_status ;
 void MakeLink(FieldLink &lnk, int inp, int sn);
 FieldSet *clone() { return new CopyOperRec(This); }
};

class CopLogRec: public FieldSet {
public:
 Field<int>      op_id ;
 Field<int>      op_subid ;

 Field<int>      db_id ;
 Field<::string>   article ;
 Field<int>      ver ;

 Field<int>      revision ;
 Field<DateTime> rev_time ;
 Field<::string>   username ;
 Field<::string>   full_name ;
 void MakeLink(FieldLink &lnk, int inp, int sn);
 FieldSet *clone() { return new CopLogRec(This); }
};

class CorTurnRec: public FieldSet {
public:
 Field<int>      d_code ;
 Field<int>      c_code ;
 Field<::string>   dname ;
 Field<::string>   cname ;
 Field<Sum>      turn ;
 Field<int>      dparent ;
 Field<int>      cparent ;
 Field<int>      dnode ;
 Field<int>      cnode ;
 void MakeLink(FieldLink &lnk, int inp, int sn);
 FieldSet *clone() { return new CorTurnRec(This); }
};

class CorItemRec: public FieldSet {
public:
 Field<int>      op_id ;
 Field<int>      op_subid ;
 Field<int>      d_code ;
 Field<int>      c_code ;
 Field<::string>   dname ;
 Field<::string>   cname ;
 Field<Sum>      turn ;
 Field<int>      dparent ;
 Field<int>      cparent ;
 Field<int>      dnode ;
 Field<int>      cnode ;
 Field<DateTime> op_date ;
 void MakeLink(FieldLink &lnk, int inp, int sn);
 FieldSet *clone() { return new CorItemRec(This); }
};

class ObjPrivRec: public FieldSet {
public:
 Field<orarowid> row_id;
 Field<int>     oid;
 Field<::string>  runame;
 Field<::string>  name;
 Field<int>     ru;
 Field<int>     pr0,pr1,pr2,pr3,pr4,pr5;
 void MakeLink(FieldLink &lnk,int inp,int sn);
 FieldSet *clone() { return new ObjPrivRec(This); }
};


//extern ClipBoard SumBuf ;
extern int ACCLEN ;
/*
class AccBrowser: public OraBrowser,  public DLG_Manager {
   char str[100];
   OraView *at;
   cursor *crsr;
   DLG *_dlg ;
   enum { C_EXIT=1000 , C_PERIOD, C_INPOPER} ;
   int *exit_flag ;

public:
   AccRec accr;

   void Init(cursor &_crsr,int *eflag);
   char *WriteStr(int *ext);
   void DrawBorder() { draw_dlg_border(_dlg) ; }
   D_WFUNC work ;
   int  Browse(MENU *m,char *title);
   ~AccBrowser() { delete at; }
} ;

extern AccBrowser * ACCB ;
*/
char *Acc_UserName(char *,const char *) ;
void Calculator(char *dest_str) ;
int SelectAcc(cursor &crsr,char *accname) ;
int InputAccount(cursor &crsr,char *accname) ;
int _SelectPeriod() ;
int _InputOper(int shft=0) ;
int InputOper(int shft=0) ;
int EditOper(OpRec &opr,int readonly=0,int del=0,int shft=0);
int DelOper(int op_id,int op_subid) ;
int DBOptEdit(int dbId,cursor &crsr,::string &set);

#define CHECKPOS 1
class PosAnalBuf ;


#define EM_WITHVERS    0
#define EM_WITHOUTVERS 1

#define OM_SELPOS 0
#define OM_ALLPOS 1
#define OM_ENDREM 2
int EditAnalSum(AccRec *ar,OraView *accview,char *head,cursor *crsr,ObjRec &or,int sum_id,int rem_id,int flags=0,Field<Sum> *sptr=NULL,int readonly=0,int shift=0,::string set="",int checkpos=0,PosAnalBuf *drec=NULL,int emode=-1,int omode=-1) ;
int EditAnalSum(AccRec *ar,OraView *accview,char *head,cursor *crsr,int sum_id,int rem_id,int flags=0,Field<Sum> *sptr=NULL,int readonly=0,int shift=0,::string set="",int checkpos=0,int emode=-1,int omode=-1) ;

class PosBuf ;
int GetDBLine(int dbId,PosBuf *rec=NULL,char *title=NULL,int shift=0,char *istr=NULL,const ::string &set="",const ::string &QBE_string="") ;
int GetDBLine(ObjRec &obj,PosBuf *rec=NULL,char *title=NULL,int shift=0,char *istr=NULL,const ::string &set="",const ::string &QBE_string="") ;
//int GetDBLine(ObjRec &obj,PosBuf *rec,char *title,int shift,char *istr,const ::string &set,const ::string &QBE_::string="")


void CopLog(cursor *crsr,int op_id,int op_subid) ;
void CopLog(cursor *crsr,int db_id,::string article,int ver) ;

int _SetOptions(UserOptRec &ur,int from_admin=0) ;
extern int HI_MainWin ;
extern ::string def_shema ;

int GetObj(int objId,ObjRec &obj,cursor *crsr=0);
int GetObj(const ::string &objName,ObjRec &obj,cursor *crsr=0);
int GetLinkDB(cursor *crsr,ObjRec &obj,OpRec or) ;
void ChooseDocRun(OraView *opv,FieldSet &op,OBJ_Type otype) ;
void OutputForm(char *title,char *formfile,unsigned flags) ;
void ProgRun(ObjRec &or,OraView *view,FieldSet *fs,char *output=NULL) ;
void My_OemToChar(char const *s,char *d) ;
void My_CharToOem(char const *s,char *d) ;
void ConnectedOperList(int base_op_id) ;

void AnalyzeRes(int res,char *text=NULL) ;
#define R_SUMSDC    1 // Суммы не совпадают (дебет с кредитом)
#define R_DIVACC    2 // Счет разбит на субсчета
#define R_BALPOST   3 // Балансовый счет для откладывания
#define R_NORIGHTS  4 // Недостаточно прав
#define R_BADDATE   5 // Дата вне допустимых пределов
#define R_ANALSYNT  6 // Не совпадает синтетика с аналитикой
#define R_ANALITOGO 7 // В аналитике не совпадает итого-строка с позициями по балансов
#define R_OPERBUSY  8 // Операция редактируется другим пользователем
#define R_NEGREMS   9 // Действие ведет к отрицательным остаткам
#define R_ERROR     10 // Внутренняя ошибка
#define R_NEGREMSOK 11 // Действие ведет к отрицательным остаткам, ОК
#define R_NEXTOPERS 12 // Есть следующие операции
#define R_NATIVEDOC 13 // Документ не соответствует стандартной операции
#define R_FIRSTOPER 14 // First operation in group

#endif

