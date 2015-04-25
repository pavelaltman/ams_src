#ifndef _PROG_H
#define _PROG_H

#include "progpipe.h"

class Struct;
class Const;
class Node;
class StackItem;
class Prog;
class IP;

class VAL {
friend StackItem;
friend Prog;
protected:
 OraField *fld;
public:
 VAL():fld(0) {}
 VAL(::string const &s)
      { fld=new Field<::string>; *(Field<::string>*)(fld)=s; }
 VAL(int s)
      { fld=new Field<int>; *(Field<int>*)(fld)=s; }
 VAL(long double s)
      { fld=new Field<Sum>; *(Field<Sum>*)(fld)=s; }
 VAL(const Sum &s)
      { fld=new Field<Sum>; *(Field<Sum>*)(fld)=s; }
 VAL(const DateTime &s)
      { fld=new Field<DateTime>; *(Field<DateTime>*)(fld)=s; }
 VAL(OraField *_fld)
      { fld=_fld->clone(); }
 VAL clone() { VAL t; if(fld) t.fld=fld->clone(); return t; }
 ::string &    String();
 long double Number();
 Sum&        GetSum();
 DateTime&   GetDate();
 VTYPE vtype() const { if(!fld) return gcl::VT_NULL; return fld->vtype(); }
 int isNull() const { if(!fld) return 1; return fld->isNull(); }
 int isUndef() const { return fld==NULL; }
 void SetNull() { if(fld) fld->SetNull(); }
 void destroy() { delete(fld); }
 OraField &Fld() { return *fld ; }
};

class Node:public VAL {
friend Struct;
friend IP;
friend Prog;
friend StackItem;
 int    refer;
public:
 Struct * st;
 ::string name;
 Node() { refer=0; st=0; }
 Node(::string const &nam) { name=nam; st=0; refer=0; }
 Node(::string const &nam,OraField &FLD)
  { name=nam; refer=1; st=0; fld=&FLD; }
 Node(::string const &nam,OraField *FLD)
  { name=nam; refer=0; st=0; fld=FLD?FLD->clone():0; }
 Struct *Stru() { return st; }
 Struct& Structure();
 void   MakeStruct();
 void   MakeOraRec(OraView &view,FieldSet &rec,int clone=0);
 void   destroy();
 void   assign(StackItem &si);
 void   assign(const VAL &vl);
 void   assign_(StackItem si);
 void   set(OraField const &FLD);
 void AddStruct(Struct* S);
 virtual StackItem Method(::string const &method,int pc,StackItem *stk);
 virtual ~Node() {}
};

extern Node Const_Node;

class StackItem {
friend Prog;
friend Node;
 VAL    val;

public:
 Node   *nd;

 StackItem()         :nd(0)           {}
 StackItem(Node &n)  :val(n),nd(&n)   {}
 StackItem(VAL &v)   :nd(0),val(v)     {}
 StackItem(const VAL &v);
 StackItem(Node &n,VAL &v)   :nd(&n),val(v) {}
 void       destroy() { if(!nd) val.destroy(); }
 Node& getNode();
 Struct *Stru() { if(nd) return nd->Stru(); else return 0; }
 VAL &Val() { return val; }
 ::string &    String();
 long double Number();
 Sum&        GetSum();
 DateTime&   GetDate();
 VTYPE vtype() const { if(!val.fld) return gcl::VT_NULL; return val.fld->vtype(); }
};

class Stack {
friend Prog;
 gcl::vector<StackItem> stck;
 gcl::vector<int> lims;
 StackItem& top() { return stck.back(); }
 StackItem& top(int d) { return *(stck.end()-d-1); }
 StackItem  pop() { StackItem si=stck.back(); stck.pop_back(); return si; }
 void       push(StackItem const &si) { stck.push_back(si); }
 int        sp()  { return stck.size(); }
public:
 void             Enter();
 void             Leave();
 ~Stack();
};

class Struct: public FieldSet {
friend Node;
protected:
 gcl::vector<Node *> els;
 int count;
public:
 Struct() : count(0) {}
 void IncC() { count++; }
 void DecC() { count--; }
 int  GetCount() { return count; }

 int Size(){return els.size();};
 Node* Var(int i){return els[i];};
 void MakeEmpty();   // Не пытается удалить поля составляющие структуру


 virtual StackItem Method(Node &node,::string const &method,int pc,StackItem *stk);
 Node *AddVar(Node *node);
 Node *FindVar(::string const &name);
 Node &Var(::string const &name);
 void UpdateVar_Fld(const char *vname,OraField *fld);
 void UpdateVar_Clone(const char *vname,OraField *fld);
 void MakeLink(FieldLink &lnk, int inp, int sn);
 FieldSet *clone() { return 0; }
 void SetNull() {}
 OraField *Fld(FieldLink const &lnk, int sn)
  { return lnk.fldNum>=0?els[lnk.fldNum]->fld:0; }
 virtual ~Struct();
 Node* DelVar(const string& VarName);
};

class ProgOraCursor: public Struct {
 cursor crsr;
public:
 ProgOraCursor();
 ~ProgOraCursor() {}
 virtual StackItem Method(Node &node,::string const &method,int pc,StackItem *stk);
};

// "Глобальные" переменные
class TGenLocalVars;

class GlobStruct: public Struct, public OraDML {
 gcl::vector<int> lims;
 TGenLocalVars* GenLocalVars;
public:
 GlobStruct() : OraDML(0), GenLocalVars(0) {}
 void             Enter();
 Node&            AddVar(::string const &name)
                                  { return *Struct::AddVar(new Node(name)); }
 void             AddStruct(::string const &name)
                                  { AddVar(name).MakeStruct(); }
 void             Leave();
 void             GiveCurs(cursor &crsr) { curs=&crsr; }
 void             TakeCurs() { curs=0; }
 void   SetVars(TGenLocalVars* GenLocalVars_)
    {GenLocalVars=GenLocalVars_;};
 StackItem Method(Node &node,::string const &method,int pc,StackItem *stk);
};

class Const_TBL {
 gcl::vector<VAL> Consts;
public:
 int Add(::string const &str);
 int Add(long double v);
 int Add(int i);
 VAL &operator[](int i) { return Consts[i]; }
 ~Const_TBL();
};


class UserFunc {
friend Prog;
 int entry;
 ::string name;
 gcl::vector<::string> args;
public:
 UserFunc() : entry(-1) {}
 UserFunc(::string const &fname,int e): entry(e),name(fname) {}
 void AddArg(::string const &name) {  args.push_back(name); }
 int  Call(int pc,StackItem *stk);
};

class BaseNode: public Node {
public:
 BaseNode(): Node("ROOT") { st=new GlobStruct; st->IncC(); }
 StackItem Method(::string const &method,int pc,StackItem *stk);
 GlobStruct &GlobStructure() { return *(GlobStruct *)st; }
 ~BaseNode() { destroy(); }
};

extern connection conn;
class IP {
public:
 BaseNode   Base;
 Stack  stck;
 cursor curs;
 IP() { GlobStructure().GiveCurs(curs); }
 GlobStruct &GlobStructure() { return Base.GlobStructure(); }
 void SetVars(TGenLocalVars* GenLocalVars_);

 void Enter()
 {
    stck.Enter();
    GlobStructure().Enter();
 };
 void     Leave() { stck.Leave(); GlobStructure().Leave(); }
 ~IP() {}
};

enum icmd {
 ic_halt=0,
 ic_loadconst,
 ic_loadnull,
 ic_ldef,
 ic_resolve,
 ic_store,
 ic_call,
 ic_ret,

 ic_enter,
 ic_leave,

 ic_makevar,
 ic_makestruct,

 ic_label,
 ic_jmp,
 ic_jz,
 ic_jnz,

 ic_uminus,
 ic_not,

 ic_and,
 ic_or,

 ic_lss,
 ic_lse,
 ic_eq,
 ic_noteq,
 ic_gre,
 ic_grt,

 ic_add,
 ic_sub,
 ic_mul,
 ic_div,
 ic_drop,

 ic_curline,

 ic_last
};

struct ICmd {
 icmd cmd;
 int  arg;
public:
 ICmd(icmd _cmd=ic_halt,int _arg=0):cmd(_cmd),arg(_arg) {}
};

class Prog {
//      tokens
    enum Lex {
         o_brack=1      ,       //  (
         c_brack        ,       //  )
         comma          ,       //  ,
         plus           ,       //  +
         aster          ,       //  *
         slash          ,       //  /
         minus          ,       //  -
         equal          ,       //  ==
         belong         ,       //  in
         iter_b         ,       // [
         iter_e         ,       // ]
         calc_b         ,       // {
         calc_e         ,       // }
         prn_b          ,       // <
         prn_e          ,       // >
         assgn          ,       // =
         not            ,       // !
         macro          ,       // @
         comand         ,       // &
         asksign        ,       // ?
         ident          ,
         member         ,
         method         ,
         constant       ,
         consint        ,
         constring      ,
         endexp         ,
         func           ,
         func_declare   ,
         e_func_declare ,
         user_func      ,
         ret            ,      // return
         pri            ,      // abc
         lss            ,
         grt            ,
         lse            ,
         gre            ,
         noteq          ,
         and            ,
         or             ,
         FOR            ,
         WHILE          ,
         UNTIL          ,
         LOCAL          ,
         STRUCT         ,
         FUNCTION       ,
         USELIB         ,
         dummy=0
      };

      struct Lexem {
         Lex lex;
         ::string s;
         long double v;
         int   i;
      };
      struct  CharLex { int ch ; Lex lex ; char *txt; } ;

 static CharLex char_lex[];

 gcl::vector<ICmd>  cmds;
 gcl::vector<UserFunc *> uf;
 gcl::vector<::string> uses_lib;
 gcl::vector<int> call_stck;

 ::string main_file;
 gcl::vector<::string *> call_stck_name;
 /* static */ ::string *cur_func;
 /* static */ int cur_line;

 Const_TBL consts;
 char line[MAXSTMTLEN],*Sp;
 ProgPos PP;
 Lexem lex;

/* static */ FILE *out;
/* static */ int  out_line;
    ::string ResFileName; // Куда записывать результат выполнения формы
    ::string ImpFileName; // Какой файл импортировать
    ::string EncodingName; //кодировка DOS или WIN (умолчательно)

public:
 void AddCmd(ICmd &cmd) { cmds.push_back(cmd); }
 int GetLex();
 int Compile(ProgPipe *pipe,int lib=0);
 int Link(const char *doc_path);
 void ParseError(int code,::string const &msg);
 /* static */ void RTError(int code,::string const &msg) throw(OraError);
 /* static */ void RTError(int code,const char * msg) throw(OraError);

 int Factor();
 int Term();
 int SubExpr();
 int CompExpr();
 int LogExpr();
 int Expr();
 int Stmt();
 int CaseBlock(ProgPipe *pipe);
 int PrintBlock(ProgPipe *pipe);
 int Block(ProgPipe *pipe);
 int Run(IP &ip);
 StackItem DoOperator(icmd op,StackItem &l,StackItem &r);

/* static */ FILE *OpenOut(const char *fname) { out_line=0; return out=fopen(fname,"wt"); }
/* static */ void CloseOut() { if(out) { fclose(out); out=0; } out_line=0; }
/* static */ FILE *GetOut() { return out; }
/* static */ int  &Out_Line() { return out_line; }
    const ::string& GetResFileName(){return ResFileName;};
    void SetResFileName(const ::string& RFN ){ResFileName=RFN;};
    const ::string& GetImpFileName(){return ImpFileName;};
    void SetImpFileName(const ::string& RFN ){ImpFileName=RFN;};
    const ::string& GetEncodingName(){return EncodingName;};
    void SetEncodingName(const ::string& RFN ){EncodingName=RFN;};

 ~Prog();
};

extern Prog *prog_ptr ;
#endif


