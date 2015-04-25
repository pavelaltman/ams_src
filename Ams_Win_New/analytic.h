#ifndef __ANALYTIC_H
#define __ANALYTIC_H

#include "usedef.h"


#ifndef AMS_ENUMS_H
enum
{
        Addable         =0x1,        // 1
        Totalable       =0x2,        // 2
        CommonVer       =0x4,        // 4
        DebetCredit     =0x8,        // 8
        Hidden          =0x10,       // 16
        SystemFld       =0x100,      // 256
        Frozen          =0x200,      // 512
        Editable        =0x400,      // 1024
        AlwaysFree      =0x800,      // 2048
        EditRecalc      =0x1000,     // 4096
        ZestFld         =0x8000,     // 32768
        RemFld          =0x10000,    // 65536
        ShowFld         =0x20000,    // 131072
        DistrFld        =0x40000,    // 262144
        NoUpdateImp     =0x80000,    // 524288
};
#endif

enum {
        ShowRemFld      =0x1,
        ShowDebetCredit =0x2
};

extern int TotVer;
extern char TotArt[];

enum {
  IR_INIT                 =0x1,
  IR_EDIT                 =0x2,
  IR_NOTBUSY              =0x4,
  IR_QBE                  =0x8,
  IR_NOSYS                =0x10,
  IR_DBONLY               =0x20,
  IR_COMVER               =0x40
};

class FInfoRec;
class DBStructInfo;

struct  FInfo
        {
         int     db;
         ::string  RealName;
         ::string  OrigName;
         VTYPE   vtype;
         int     ref_dbId ;
         ::string  ref_dbname ;

         int     maxlen;
         int     deleted;

         ::string  Alias ;
         int     width;
         int     dec;

         int     ord;
         int     def_opt,opt,disp_opt;
         ::string  DB_SetName,dbset ;

/*         struct OptOver {
           ::string SetName;
           int    opt;
         };
         gcl::vector     <OptOver> overs;
*/
         FInfo() {}
         FInfo(const FInfoRec & FIRec,int _db=1);

         int& Opt() { return opt; }
         int  Align();
         int  DispLen(int use_len=1);
         void OutputHeader(char *str,int use_len=1);
         void OutputFld(FieldSet &rec,char *str,int use_len=1,int flags=GS_NOZERO,int point='-');
         void OutputTotal(FieldSet &rec,char *str);
         int  InputFld(FieldSet &rec,const char *str,char *errmes=NULL);
         int  TestField(int flags);
};

struct  FInfoRec: public FieldSet
        {
         friend FInfo;
         Field<orarowid> row_id;
         Field<int>     dbId;
         Field<::string>  RealName;
         Field<int>     vtype;
         Field<int>     ref_dbId;

         Field<int>     maxlen;
         Field<int>     deleted;

         Field<::string>  Alias ;
         Field<int>     disp_opt;
         Field<int>     width;
         Field<int>     dec;

         Field<int>     ord;
         Field<int>     def_opt;
         Field<int>     opt;
//         PosTab         *ptab ;
//         Code           acode;
         Field<::string>  DB_SetName ;
         Field<::string>  dbset ;
         Field<int>     lines;

         FInfoRec();

 void MakeLink(FieldLink &lnk, int inp, int sn);
 FieldSet *clone() { return new FInfoRec(This); }
         ~FInfoRec();
};

struct  DBOptRec: public FieldSet
        {
         Field<orarowid> row_id;
         Field<int>     dbId;
         Field<::string>  SetName;
 void MakeLink(FieldLink &lnk, int inp, int sn);
 DBOptRec *clone() { return new DBOptRec(This); }
};


struct  DBAddInfoRec: public FieldSet
        {
         Field<orarowid> row_id;
         Field<int>      dbId;
         Field<::string>   SetName;
         Field<::string>   RealName;
         Field<::string>   name;
         Field<int>      code;
         Field<::string>   turntype;
         Field<::string>   alias ;
         Field<int>      sumid ;
         Field<int>      opt ;
         Field<::string>   accname ;
 void MakeLink(FieldLink &lnk, int inp, int sn);
 DBAddInfoRec *clone() { return new DBAddInfoRec(This); }
};


class   DBStructInfo    {
protected:
    int FirstAnalField;
public:
        int GetFirstAnalField() const {return FirstAnalField;};

        gcl::vector     <FInfo>  fis;
        ::string          set;
        gcl::vector     <::string> osets;
        int             dbfldNum;
        ::string          dbname,analname;
        int             dbid,analid;
        Prog            db_prog,an_prog;
        int             show_rem,show_dc, for_docs ;

        char *          bal_fld;
        Field<Sum>  *   synt_sum;

        DBAddInfoRec    arecs[5] ;
        int             arnum ;

        DBStructInfo() {}
        DBStructInfo(cursor *crsr,ObjRec &objRec,const ::string &set,int flags=0) { Init(crsr,objRec,set,flags); }
        DBStructInfo(cursor *crsr,int objId,const ::string &set,int flags=0) { ObjRec r; GetObj(objId,r,crsr); Init(crsr,r,set,flags); }

        void Init(cursor *crsr,ObjRec &objRec,const ::string &set,int flags=0);
//        Finfo * NextField(int &it);
        FInfo &FI(const ::string &rname);
        int  Opt(const ::string &rname)
                { return FI(rname).Opt(); }
        int   NFields() { return fis.size(); }
        FInfo &FI(int i) { return fis[i]; }
        int  Opt(int i)
                { return FI(i).Opt(); }
        OraField *FLD(FieldSet &rec,int i)
              { return rec.FldN(fis[i].RealName.data()); }

        int  DispLen(int use_len=1);
        int  FrozenLen();
        void OutputHeader(char *str,char *delim=0);
        void OutputRec(FieldSet &rec,char *str,char *delim=0,int start=0);
        void OutputTotal(FieldSet &rec,char *str,char *delim=0);
        void OutputInfo(FieldSet &rec,char *str);
        int  CheckRec(FieldSet &rec);
        int  TestField(int i,int flags) { return FI(i).TestField(flags); }
        int  InputRecDlg(FieldSet &rec,unsigned flags,unsigned _db=15);
        int  InputQBEDlg(FieldSet &rec,unsigned flags,unsigned _db,::string &cond,::string &ucond);
        int  SelectField(char *tit,int &fnum,int extra,int _skip,int flags);
        void BeforeInsRec(FieldSet &rec,cursor *crsr) ;

        ::string UpdateSTMT(int db,int only_free=0);
        ::string ClearSTMT();
        ::string InsertSTMT(int db,int hist=0);
        ::string DELSTMT(int db);
        ::string ByArtVer(int db);
        ::string AnalView();
        ::string AnalHeader();
        ::string AnalCond(int cat,int outer,Code SumId,Code RemId);

        ::string DBExHeader(int cat=1);
        ::string DBExGroup(int cat=1);
        ::string DBStructInfo::AnExHeader();

//        ::string AnalExHeader(int _show_rem);
//        ::string AnalExCond(int cat,int outer,Code SumId,Code RemId);

        void   SumBuf(FieldSet &totrec,FieldSet &rec,int sign);
        void   CopyBuf(FieldSet &totrec,FieldSet &rec,int db);
        void   CalcDBLine(FieldSet &rec);
        void   CalcAnLine(FieldSet &rec,FieldSet &totrec,int pre=0);
        void   MakeStruct(Node &nd,FieldSet &rec,int db=3,int clone=0);

        int    Make_AAS();
        int    Make_ExView();
        void   RecalcSum(Code SumId);

        OraView *aux_view;
        FieldSet *aux_rec;
        void SetAux(OraView *at=NULL,FieldSet *accr=NULL)
                { aux_view=at; aux_rec=accr; }
        int    Gen_CreateStmt(::string &shema,FILE *f);

};

class   InputRecordDialog
        {
         DBStructInfo   *St ;
         StrList        FldText , FldShow ;
         StrList        QBE_ops;
         int            qbe;
         char           *recBuf ;
         int            not_busy , hide_second , no_article , show_additional ;
         unsigned       start_from,iflags,db;
//         const ::string   &set;
         FieldSet       *recp;
         ::string         *rescond,*usercond;
        public:
         OraTable       *tab ;

         InputRecordDialog(OraTable *_tab,DBStructInfo *st)
          { tab=_tab ; St=st ; recBuf=NULL ; qbe=0; start_from=0; } ;
         int            work_inputrecord(void *p,int call_par,long *ret_par) ;
         int            InputRecord(FieldSet &rec,unsigned flags,unsigned _db=15) ;
         int            InputQBE(FieldSet &rec,unsigned flags,unsigned _db,::string &cond,::string &ucond) ;
         int            ShowFld(int i)
               { return (St->FI(i).db&db) && St->TestField(i,iflags); }
         #pragma argsused
         virtual int VerifyRecord(FieldSet &rec,int *ctrl) { return 1 ; } ;
        } ;

class PosBuf: public FieldDynaSet {
public:
 Field<orarowid>   row_id ;
 Field<int>        catId;
 Field<::string>     article;
 Field<int>        prVer;
 Field<int>        tord ;

 void MakeLink(FieldLink &lnk, int inp, int sn);
 FieldSet *clone() { return new PosBuf(This); }
};

class PosAnalBuf: public PosBuf {
public:
 Field<Code>       SumId;
 Field<::string>     aarticle;
 Field<int>        aprVer;

 void MakeLink(FieldLink &lnk, int inp, int sn);
 void InitTot(int SumId);
 FieldSet *clone() { return new PosAnalBuf(This); }
};

int DBRootCat(int dbId,cursor *crsr);
/*
class OpVars : public Node {
public:

 DBStructInfo DBStr;
 OraTable *operdb;
 PosBuf opvars;
 int rootCat;
 int no_first ;
 OpVars() {} ;
// OpVars(OpRec &or,OraTable &ot,cursor *crsr);
 int Init(OpRec &or,OraTable &ot,cursor *crsr);
 void OutputInfo(char *str) ;
 void SetKey(OpRec &or,int noedit=0);
 int Edit();
 int Del() ;
 ~OpVars() { delete(operdb); destroy(); }
};
*/

class OpVars : public Node
{
protected:
    FieldSetM Flds;
public:
    DBStructInfo DBStr;
    OraTable *operdb;
    PosBuf opvars;
    int rootCat;
    int no_first ;

    int Init(OpRec &or,OraTable &ot,cursor *crsr);
    void SetKey(OpRec &or,int noedit=0);

    OpVars();
    const FieldSetM& GetFlds()const {return Flds;};
    void MakeCopy (const OpVars& OV);
    void AddVarInt(const AnsiString& Name_, int Val_, bool SetNull=false);
    void AddVarSum(const AnsiString& Name_, Sum Val_, bool SetNull=false);
    void AddVarDate(const AnsiString& Name_, TDateTime Val_, bool SetNull=false);
    void AddVarStr(const AnsiString& Name_, const AnsiString& Val_);
    void AddVar(const AnsiString& Name_, OraField* Fld_);
    virtual ~OpVars() { destroy(); }

    void AddVars (TOracleDataSet* ODS);
    void AddVars (AmsQuery& AQ);

    void DelVar (const AnsiString& VarName);
};

class OraNode : public Node {
public:
 OraNode();
 OraNode(OraView &view,FieldSet &rec,int clone=0)
        { MakeOraRec(view,rec,clone); };
 ~OraNode() { destroy(); }
};


struct DBSubstInfo
       {
        int   dbid ;
        char  prefix[10] ;
       } ;
extern DBSubstInfo DBSI[] ;
extern int dbsnum ;


struct DBChangeInfo
       {
        int   dbid ;
        int   fld1 ;
        int   fld2 ;
       } ;
extern DBChangeInfo DBCI[] ;
extern int dbcnum ;

struct DBKeyInfo
       {
        int   dbid ;
        int   key_fld ;
       } ;
extern DBKeyInfo DBKI[] ;
extern int dbknum ;



extern int  cf_dbid , cf_analid ;
extern ::string cf_article ;
extern int  cf_ver ;


#endif

