#ifndef                      __OrcPP_H
#define __OrcPP_H
#include <stdio.h>
#include <mem.h>
#include <cstring.h>

#include <values.h>

#include "usedef.h"
#include "comisc.h"
#define data c_str

#include "vctr.h"
#include "datetime.h"
#include "sum.h"

#include "AMS_QUERY.h"

extern "C"
{
//#include <oratypes.h>
//#include <ociapr.h>
//#include <ocidfn.h>
}

typedef char r_char[];

/* oparse flags */
#define  DEFER_PARSE        1
#define  NATIVE             1
#define  VERSION_7          2

#define  HDA_SIZE                         256

struct orarowid {
  //char strrep[19];
  AnsiString strrep;
  int operator==(orarowid const &r) const { return strrep==r.strrep; }
};

class OraError:public xmsg
{
public:
//  char *why() { return why_s; }
//  char why_s[1000];
  sword err_code;
  OraError(sword code,const ::string &str);
  OraError(sword code,const char * str);
  void message(int roll=0,int code=1,char *point=NULL);
  //~OraError();
};

/* Class forward declarations */
class connection;
class cursor;
/*
 * This class represents a connection to ORACLE database.
 *
 * NOTE: This connection class is just given as an example and
 * all possible operations on a connection have not been defined.
 */
class connection
{
  /*
  friend class cursor;
  public:
    connection()
      { state = not_connected;
        ClearVar(lda);
        ClearVar(hda);
        log_file=0;
      }
    ~connection();
    sword connect(const text *username, const text *password);
    sword disconnect();

    int  commit()   { int t=ocom(&lda); display_error(); return t; }
    int  rollback() { int t=orol(&lda); display_error(); return t; }

    void log_error(FILE* file) const;
    void display_error() const { log_error(log_file); }
    sword Error() const { return (lda.rc); }
    void ErrMes(int errcode,char *msg,int len)
         { oerhms(&lda, errcode, msg, (short)len); }
    FILE *log_file;
  private:
    Lda_Def lda;
    ub1 hda[HDA_SIZE];
    enum conn_state
    {
      not_connected,
      connected
    };
    conn_state state;
    */
public:
    void commit();
    void rollback();
};

/*
 * This class represents an ORACLE cursor.
 *
 * NOTE: This cursor class is just given as an example and all
 * possible operations on a cursor have not been defined.
 */
class OraDDL;
class OraDML;
class OraView;

class cursor: public AmsQuery
{
public:
    cursor(connection &conn,int databufsize=3000,int indbufsize=200){};
    cursor(int databufsize=3000,int indbufsize=200){};

/*
    void _init(int databufsize,int indbufsize);
  public:
    cursor(int databufsize=3000,int indbufsize=200)
      { _init(databufsize,indbufsize); }
    cursor(connection &conn,int databufsize=3000,int indbufsize=200)
      { _init(databufsize,indbufsize);
        open(&conn);
      }
    ~cursor();
    sword open(connection *conn_param);
    sword close();
    sword parse(const text *stmt)
      {   return (oparse(&cda, (text *)stmt, (sb4)-1,
                       DEFER_PARSE, (ub4) VERSION_7)); }
    // bind an input variable
    sword bind_by_position(sword sqlvnum, ub1 *progvar,
                           sword progvarlen, sword datatype,
                           sword scale, sb2 *indicator)
      { return (obndrn(&cda, sqlvnum, progvar, progvarlen,
                       datatype, scale, indicator, (text *)0,
                       -1, -1)); }
    // bind an input variable
    sword bind_by_name(ub1 *sqlvar, ub1 *progvar,
                           sword progvarlen, sword datatype,
                           sword scale, sb2 *indicator)
      { return (obndrv(&cda, sqlvar,-1, progvar, progvarlen,
                       datatype, scale, indicator, (text *)0,
                       -1, -1));
        return obndra(&cda,sqlvar,-1,progvar, progvarlen, datatype,
                      scale, indicator,
                      0,0, 0, 0,
                      0, -1, -1);
                       }
#if 0
    sword bind_long(ub1 *sqlvar, sword progvarlen, sword datatype,
                           sb2 *indicator , ub1 *context)
      {
         return obindps(&cda, 0, sqlvar,-1,
                 (ub1 *)context, progvarlen,
                 datatype, (sword)0, indicator,
                 0, 0, 0, 0, 0, 0,
                 0, (ub4 *)0, (text *)0, 0, 0);
      }
#endif
    // define an output variable
    sword define_by_position(sword position, ub1 *buf,
                             sword bufl, sword datatype,
                                                                          sword scale, sb2 *indicator,
                             ub2 *rlen=NULL, ub2 *rcode=NULL)
      { return (odefin(&cda, position, buf, bufl, datatype,
                       scale, indicator,
                       (text *)0, -1, -1, rlen, rcode)); }
    sword define_arr(sword position,ub1 *buf,
                             sword bufl, sword datatype,
                                                                          sb2 *indicator, sword rec_size,
                             sword fld_num)
      {
      }
    sword describe(sword position, sb4 *dbsize, sb2 *dbtype,
                   sb1 *cbuf, sb4 *cbufl, sb4 *dsize, sb2 *prec,
                   sb2 *scale, sb2 *nullok)
      { return (odescr(&cda, position, dbsize, dbtype,
                       cbuf, cbufl, dsize,  prec, scale, nullok));
      }
    sword execute()
      { return (oexec(&cda)); }
    sword execute_fetch(int q,sword exact=0)
      { return oexfet(&cda, q, 0, exact); }
    sword fetch()
      { return (ofetch(&cda)); }
    sword multi_fetch(int q)
      { return (ofen(&cda,q)); }
    sword cancel()
      { return (ocan(&cda)); }

    int RecCount() { return cda.rpc; }

    sword get_error_code() const
      { return (cda.rc); }
    sword Error() const { return (cda.rc); }
    void log_error( FILE* file,const char *det) const;
    void display_error(const char *det=0) const { log_error(conn->log_file,det); }
    int  commit()   { return conn->commit(); }
    int  rollback() { return conn->rollback(); }
    Cda_Def *CDA() { return &cda; }
  private:
    Cda_Def cda;
    connection *conn;
    enum cursor_state
    {
      not_opened,
      opened
    };
    cursor_state state;

    int dbufsize,ibufsize;
    ub1 *data_buf,*dbuf,*mdbuf;
    sb2 *ind_buf,*ibuf,*mibuf;
    void ResetBufs() { dbuf=data_buf; ibuf=ind_buf; }
    void SetMark()  { mdbuf=dbuf; mibuf=ibuf; }
    void GotoMark() { dbuf=mdbuf; ibuf=mibuf; }
    ub1 *databuf() { return dbuf; }
    sb2 *indbuf()  { return ibuf; }
    void Step(int size) { dbuf+=size; ibuf++; }
    void RecSkip(int q,int rec_size,int fld_num)
        { dbuf=mdbuf+q*rec_size; ibuf=mibuf+q*fld_num;}
    int  DataCap() { return dbufsize-(dbuf-data_buf); }
    int  IndCap()  { return ibufsize-(ibuf-ind_buf); }

    friend OraDDL;
    friend OraDML;
    friend OraView;
 */
};

enum OraTypes {
   NULL_TYPE      =0,
   VARCHAR2_TYPE  =1,
   NUMBER_TYPE    =2,
   INT_TYPE       =3,
   FLOAT_TYPE     =4,
   STRING_TYPE    =5,
   LONG_TYPE      =8,
   ROWID_TYPE     =11,
   DATE_TYPE      =12,
   LONGRAW_TYPE   =24
};

/*  ORACLE error codes used in demonstration programs */
enum OraErrors {
        VAR_NOT_IN_LIST       =1007,
        NO_DATA_FOUND         =1403,
        NULL_VALUE_RETURNED   =1405,
   EXTRA_DATA            =1422,
   OCI_MORE_INSERT_PIECES=3129,
   OCI_MORE_FETCH_PIECES =3130,
};
/*  some SQL and OCI function codes */
enum OraCodes {
        FT_INSERT               =3,
        FT_SELECT               =4,
        FT_UPDATE               =5,
        FT_DELETE               =9,
        FC_OOPEN                =14,
};

class FieldLink;
class FieldDef
{
public:
    OraTypes        int_type;
    int             int_size;

    sb4             dbsize;
    sb2             dbtype;
    sb1             name[51];
    sb4             buflen;
    sb4             dsize;
    sb2             precision;
    sb2             scale;
    sb2             nullok;
    FieldDef(): int_type(NULL_TYPE) {}
};

class cursor;
class FieldLink;
class TAmsInput;
class AmsQuery;

class OraField
{
protected:
 ub2 null;
public:
 OraField() { null=1; }

 virtual VTYPE vtype() { return gcl::VT_NULL; }
 virtual void Buf2Field(ub1 *buf)=0;
 //virtual void Field2Buf(ub1 *buf) const =0;
 int  isNull() const { return null; }
 void SetNull() { null=1; _SetNull(); }
 void SetNotNull() { null=0; }
 virtual void _SetNull() {}
 virtual OraField *clone()=0;
 virtual ~OraField() {}

 virtual void SetVal(OraField* Fld)=0;

 virtual char *Output(char *buf,int flags=0,int num_dig=-1,int pchar='-') { *buf=0 ; return buf ; }
 virtual char *OutputPval(char *buf,int side,int flags=0,int num_dig=-1,int pchar='-') { *buf=0 ; return buf ; }
 virtual int DefAlign() { return SA_LEFT ; }
 virtual int TSize(int len=0) { return 0 ; }
 virtual int Input(const char *buf,int num_dig=6,char *errmess=NULL) { return 0 ; }
 virtual Sum Number() { return Sum(0) ; }
 // Устанавливает OraField из Датасета, Флд Ном- номер поля внутри датасета
 virtual void SetField(TOracleDataSet* ODS, int FldNo)=0;
 // Уст поля датасета из OraFielda
 virtual void SetDS(TOracleDataSet* ODS, int FldNo)=0;
 //--------------------------------------------------------------
 virtual void Buf2Field (AmsQuery& Curs, FieldLink& FL )=0;
 //virtual void Field2Buf (cursor& Curs, FieldLink& FL )=0;
 //--------------------------------------------------------------
 virtual void DeclVar (AmsQuery& Curs, const AnsiString& Val)=0;
 virtual void GetVar (AmsQuery& Curs, const AnsiString& Val)=0;
 virtual void SetVar (AmsQuery& Curs, const AnsiString& Val)=0;
 //--------------------------------------------------------------
 virtual void SetVal (TAmsInput* AI)=0;
 virtual void GetResult (TAmsInput* AI)=0;
 //   Устанавливает TField из OraField
 virtual void SetVal (TField* Fld)=0;
 //   Устанавливает OraField из TField
 virtual void GetVal (TField* Fld)=0;
 //--------------------------------------------------------------
 // Устанавливат OraField из AmsQuery
 virtual void SetFld(AmsQuery& AQ, int FldNo)=0;
 virtual void SubVal(TField* WhatSub, bool OldVal)=0;
 virtual void AddVal(TField* WhatAdd, bool OldVal)=0;

 virtual AnsiString AsString(const AnsiString& Format="0.00")=0;

 virtual void SetNumber(double Val) = 0;
};


template <class T>
class Field: public OraField
{
public:
  T     val;
public:
 Field() { _SetNull(); }
 //Field(const T& vl){(*this)= vl;};
 //--------------------------------------------------------------
 void Buf2Field(ub1 *buf);
 //void Field2Buf(ub1 *buf) const;
 //--------------------------------------------------------------
 virtual void Buf2Field (AmsQuery& Curs, FieldLink& FL );
 //virtual void Field2Buf (cursor& Curs, FieldLink& FL );
 //--------------------------------------------------------------
 virtual void DeclVar (AmsQuery& Curs, const AnsiString& Val);
 virtual void GetVar (AmsQuery& Curs, const AnsiString& Val);
 virtual void SetVar (AmsQuery& Curs, const AnsiString& Val);
 //--------------------------------------------------------------

 VTYPE vtype();
 void _SetNull();
 const T& Val()const {return val;};
       T& Val() {return val;};

 Field* MakeField(OraField* Fld)
 {
    return dynamic_cast<Field<T>*> (Fld); 
 };
 virtual void SetVal(OraField* Fld){Val()=MakeField(Fld)->Val();};

 OraField * clone() { return new Field(This); }
  operator const T&() const     { return val; }
  Field& operator=(const T &v)      { null=0; val=v; return *this; }
  int operator==(Field const &f)
   { int t=isNull()+f.isNull(); return t?(t==2?1:0):val==f.val; }

 int operator==(const T& v) const{ return isNull()?0:val==v;};

 Field operator+(const T& v)
   { if (isNull()) (*this)=v; else val+=v;return *this;};

 char *Output(char *buf,int flags=0,int num_dig=0,int pchar=0) ;
 char *OutputPval(char *buf,int side,int flags=0,int num_dig=0,int pchar=0) ;
 int DefAlign() ;
 int TSize(int len=0) ;
 int Input(const char *buf,int num_dig=6,char *errmess=NULL) ;
 Sum Number() ;
 // Устанавливает OraField из Датасета, Флд Ном- номер поля внутри датасета
 virtual void SetField(TOracleDataSet* ODS, int FldNo);
 // Уст поля датасета из OraFielda
 virtual void SetDS(TOracleDataSet* ODS, int FldNo);
 virtual void SetVal (TAmsInput* AI);
 virtual void GetResult (TAmsInput* AI);
 virtual void SetVal (TField* Fld);
 //   Устанавливает OraField из TField
 virtual void GetVal (TField* Fld);
 //--------------------------------------------------------------
 // Устанавливат OraField из AmsQuery
 virtual void SetFld(AmsQuery& AQ, int FldNo);
 virtual void SubVal(TField* WhatSub, bool OldVal);
 virtual void AddVal(TField* WhatAdd, bool OldVal);

 virtual AnsiString AsString(const AnsiString& Format="0.00");
 virtual void SetNumber(double Val);
};

//--------------------------------------------------
// Для смычки с новой программой
//--------------------------------------------------
class TFldsDescr;
//class TOracleDataSet;
class Node;
class FieldSetM
{
protected:
    TFldsDescr* FldsDescr;
    vector<OraField*> Flds;
    TOracleDataSet* ODS;
    bool Builded;
public:
    FieldSetM();
    FieldSetM(TFldsDescr* FldsDescr_, TOracleDataSet* ODS_):
        FldsDescr(FldsDescr_), ODS(ODS_), Builded(false){};
    //------------------------------------------------------------
    void BuildFSM();
    void SetDS (TOracleDataSet* ODS_) {ODS=ODS_;};
    void SetFldsDescr(TFldsDescr* FldsDescr_)
    {
        FldsDescr=FldsDescr_;
        Builded=false;
    };
    //------------------------------------------------------------
    // Делает структруру по описанию и заполняет ее значениями из датасета, если TRUE
    void LoadData(Node&, bool SetFlds, bool ClearFlds, bool ClearF4Only = false );
    void SetData();
    int Count () {return Flds.size();};
    OraField* At(int i) {return Flds[i];};

    void Add(OraField*);
    virtual ~FieldSetM ()
    {
        for (unsigned int i = 0; i< Flds.size(); i++)
            delete Flds[i];
    };
    // Вставляет значение поля в элемент ввода
    void SetInputVal(TAmsInput* AI, int FldNo);
};
//----------------------------------------------------------------------------
// Класс для поддержки вычислений итого строки в анал сумме
class FieldSetAnTotal:public FieldSetM
{
protected:
    // Индекс итого-полей
    vector<int> Indx;
    // Sql запрос на получение итого-строки по поддереву и тд
    AnsiString Sql;
    bool SqlFormed;
public:
    FieldSetAnTotal(TFldsDescr* FldsDescr_, TOracleDataSet* ODS_):
        FieldSetM(FldsDescr_,ODS_), SqlFormed(false){};

    // Строим все
    virtual void Build();

    void MakeTotalUpdate (int AnSum);
    //------------------------------------------------------
    //  Для сложения аналитических сумм
    //------------------------------------------------------
    // Установить во всех полях нулевые значения
    void SetZero();
    // Копировать итого строку в FldSet
    void CopyAnalSum (AmsQuery& AQ_);
    // Добавить текущую запись датасета к фиелд сету
    void AddLine (TOracleDataSet* ODS_, bool OldVal= false);
    // Отнять текущую запись датасета от фиелд сета
    void SubLine (TOracleDataSet* ODS_, bool OldVal=false);
    // Может быть понадобится
    void AddLine (TOracleDataSet* ODS_, int Sign, bool OldVal= false)
    {if (Sign==1) AddLine(ODS_, OldVal);else SubLine(ODS_, OldVal);};
    // Возвратит итого-значение по индексу поля из итого строки
    Sum GetResult(AnsiString FldName);
};
//----------------------------------------------------------------------------
// Класс для поддержки вычислений распределения по версиям
class Prog;
class TGenLocalVars;
class FieldSetVerDistr:public FieldSetAnTotal
{
protected:
    std::vector<int> SumFlds;
    std::vector<int> CopyFlds;
    //-------------------------------
    int AnSumEd;
    int AnSumOst;
    AnsiString OstOrder;
    //-------------------------------
    AnsiString SqlIns;
    int DistrFld;
    int DistrFldIndx;
    AnsiString Art;
public:
    FieldSetVerDistr (TFldsDescr* FldsDescr_, TOracleDataSet* ODS_):
        FieldSetAnTotal(FldsDescr_, ODS_){};

    // Строим все
    virtual void Build();

    //
    void SetParams (int AnSumEd_, int AnSumOst_,
                    const AnsiString& OstOrder_ = "D.VER asc ",
                    Prog* AnProg_=NULL, TGenLocalVars* Vars_=NULL)
    {
        AnSumEd=AnSumEd_;
        AnSumOst=AnSumOst_;
        OstOrder=OstOrder_;
    };
    // Какой артикул распределяем
    void SetCurrArt (const AnsiString& Art_) {Art=Art_;};
    const AnsiString& GetCurrArt() const {return Art;};

    // Формирует запрос на получение остатка по полю распределять
    // Возвращает 1 если существует поле распределять
    int PrepareOstQuery (AmsQuery& AQOst);

    // Возвращает количество версий в остатке по Артикулу
    int GetVersQ(const AnsiString& Art);

    // Возвращает количество версий с положительныm остатком в остатке по Артикулу
    int GetVersQPlus(const AnsiString& Art);
    
    // Формирует запрос на вставку (для ускорения)
    void PrepareInsertQuery (AmsQuery& AQIns);

    // Отнимает остаток и генерирует insert запрос
    // Возвращает false если весь остаток распределен
    int HaveOst(AmsQuery& AQOst, AmsQuery& AQIns);

    // Вставляет то что осталось как версию, прописанную
    // Вызывается когда уже не осталось остатка или списывается полностью остаток
    void PutOst(AmsQuery& AQIns, bool NoOst);
    //virtual ~FieldSetVerDistr(){delete FSM;FSM=NULL;};
};
//-----------------------------------------------------------------------------
class FieldSet;

class FieldLink: public FieldDef
{
public:
 int setN;
 OraField FieldSet::*fld;
 short fldNum;              // Номер поля внутри FieldSeta
 int FldNum;                // Номер поля внутри результата SQL запроса
 ::string name_query;
 FieldLink(): fld(NULL),fldNum(-1),FldNum(-1),setN(0) {}
};

struct FieldIntDef
{
 char *name;
 OraTypes type;
 int size;
 OraField FieldSet::*fld;
 char *altname,*altname1;

 int Set(FieldLink &lnk);
};

class OraView;

class FieldSet
{
 OraView *tied;
protected:
 FieldSet *merged;
public:
 FieldSet(): tied(0), merged(0) {}
 virtual void MakeLink(FieldLink &lnk, int inp=0, int sn=0) =0;
         int _MakeLink(FieldLink &lnk,FieldIntDef *flds,int fldnum,int inp, int sn);
 virtual FieldSet *clone() =0;
 virtual void SetNull() {};
 virtual OraField *Fld(FieldLink const &lnk,int sn=0)
        { return lnk.setN==sn?(lnk.fld?&(this->*(lnk.fld)):NULL)
        :(merged?merged->Fld(lnk,sn+1):NULL); }
 virtual void Tie(OraView *ov) { tied=ov; }

 virtual FieldLink* Link(char const *name);
 OraField*  FldN(char const *name)
  { FieldLink *lnk=Link(name);
    if(!lnk) return 0;
    return Fld(*lnk);
  }
 FieldSet &Merge(FieldSet &mrgd) { merged=&mrgd; return This; }
 void      UnMerge() { merged=NULL; }
 virtual ~FieldSet() {}
 OraView *TiedView() { return tied ; } ;
};

class FieldDynaSet: public FieldSet {
public:
 gcl::vector< OraField* > dynaflds;

 virtual void MakeLink(FieldLink &lnk, int inp=0, int sn=0);
 OraField *Fld(FieldLink const &lnk,int sn=0);
 void SetNull();
 ~FieldDynaSet();
 FieldDynaSet &operator =(FieldDynaSet const &c);

virtual FieldSet *clone() { return &(*(new FieldDynaSet)=This); };
};


void err_report(FILE *file, char* *errmsg, sword func_code);

class FieldLinks {
  gcl::vector<FieldLink> flds;
public:
  FieldLink *Link(const char* name);
  void Add(FieldLink &lnk) { flds.push_back(lnk); }
  void Empty() { flds.erase(flds.begin(),flds.end()); }
  int  BufSize() const ;
  int  Num() const { return flds.size(); }
  //void Store(FieldSet const &rec,ub1 *databuf,sb2 *indbuf) const;
  void Store(FieldSet const &rec,cursor& Curs) const;
  //void Get(FieldSet &rec,ub1 *databuf,sb2 *indbuf) const;
  void Get(FieldSet &rec,cursor& Curs) const;
  void SetNull(FieldSet &rec) const;
  int  isNull(FieldSet &rec) const;
  FieldLink & operator[](int i) { return flds[i]; }
  ::string CompExpr(int d);
  ::string OrdExpr(int d,const char *aux=0);
};

class OraDDL {
protected:
  ::string     stmt;
  bool Silent;
public:
  cursor     *curs;
  OraDDL(cursor *crsr) { curs=crsr; if (curs) curs->Clear(); }
  //----------------------------------------------------
  // Чтобы следующий вызов не
  // показывал сообщение об ошибке
  //----------------------------------------------------
  void MakeSilent(){Silent=true;};
  void Parse(const ::string &stmt);
  int  execute();
  int  Do(const ::string _stmt);
  int  DoDDL(char *fmt,...);
};

class OraDML: public OraDDL
{
protected:
  FieldLinks args;
public:
       OraDML(cursor *crsr): OraDDL(crsr) {}
  void Bind(FieldSet const &recb);
  void SetArgs(FieldSet const &recb);
  void GetArgs(FieldSet &recb);
  int  Do(const ::string _stmt,FieldSet &inoutrec);
  int  DoPL(const ::string _stmt,FieldSet &inoutrec);
virtual
  FieldLink* Link(char const *name) { return 0; }

};

class OraView: public OraDML
{
protected:
  ::string                 view;
  FieldLinks flnks;
  void Describe(); // Fill flnks with 'FieldDef's
  int max_fetch,fetched,readed;
public:
  ::string                 group;
  ::string                 header;

  OraView(cursor *cur,const char *vie,const char *head,const char *grou) ;
  OraView(cursor *cur,const char *vie,const char *head=NULL,int row=0) ;
  void AdjView();
  FieldLink* Link(char const *name) { return flnks.Link(name); }
  FieldLinks *Flds() { return &flnks; }
  FieldLinks &FldLinks() { return flnks; }
  void SelectAll();
  void MakeRecord(FieldSet &rec);
  void SetNull(FieldSet &rec) { flnks.SetNull(rec); }
  void Define();
  void SetInOut(FieldSet const &inrec,FieldSet &outrec)
            { MakeRecord(outrec); Bind(inrec); Define(); }
  void SetIn(FieldSet const &inrec) { Bind(inrec); Define(); }
  int  fetch(FieldSet &recb);
  int  ExFetchOne(FieldSet &recb,int exact=1);
  int  ExFetchFirst(FieldSet &recb) { return ExFetchOne(recb,0); }
  int  Do(const ::string _stmt,FieldSet const &inrec);
  int  Do(const ::string cond,const ::string order,FieldSet const &inrec);
  int  Select(const ::string cond,const ::string order,FieldSet const &inrec);
  int  AltSelect(const ::string stmt,FieldSet const &inrec);
  int  RepeatDo(FieldSet const &inrec);
  int  Select(const ::string cond,FieldSet const &inrec,FieldSet &outrec);
  int  AltSelect(const ::string stmt,FieldSet const &inrec,FieldSet &outrec);
  int  Do(const ::string _stmt,FieldSet const &inrec,FieldSet &outrec);
  int  RepeatDo(FieldSet const &inrec,FieldSet &outrec);
  ::string SELSTMT();
  void Cancel() { }
private:
  int  _fetch(FieldSet &recb,int rec_skip);
};

class OraTable: public OraView
{
public:
  OraTable(cursor *cur,const char *vie,const char *head=NULL):
          OraView(cur,vie,head,1) {}
  int InsertRec(FieldSet &recb);
  int UpdateRec_ID(FieldSet &recb,char *tname=NULL);
  int DeleteRec_ID(FieldSet &recb);
  int UpdateRecs(::string _st,FieldSet &inrecb)
   { return OraDML::Do(UPDSTMT()+_st,inrecb);}
  int DeleteRecs(::string _st,FieldSet &inrecb)
   { return OraDML::Do(DELSTMT()+_st,inrecb);}
  ::string UPDSTMT();
  ::string DELSTMT();
  ::string ByROWID();
  ::string InsertSTMT();
};
#if 0
class OraLongTable: public OraTable
{
public:
  OraLongTable(cursor *cur,char *vie,char *head=NULL):
          OraTable(cur,vie,head) {};
  int InsertRec(FieldSet &recb,char *filename);
  int UpdateRec(::string st,FieldSet &recb,char *filename);
  int FetchRec(::string st,FieldSet &recb,char *filename);
};
#endif
#endif
