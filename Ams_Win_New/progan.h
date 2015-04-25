class PosDB_PC;
class CatDB_PC: public Struct {
friend PosDB_PC;
 cursor crsr;
 DBStructInfo DBStr;
 OraView      *cv;
 CatTreeRec crec;
 ObjRec obj;
 int    dbId;
public:
 CatDB_PC(const ::string &dbname,const ::string &set);
 virtual ~CatDB_PC() { delete(cv); }
 virtual StackItem Method(Node &node,::string const &method,int pc,StackItem *stk);
};
//-----------------------------------------------------------------------------
class DbImpExp: public Struct {
    AnsiString ODBC;
    TDatabase * DB;
    enum enLifeCycle {lcEmpty, lcCreated, lcDeleted};
    enLifeCycle CurrState;
    std::vector<TQuery*> Queries;
public:
 DbImpExp(const AnsiString& ODBC_, const char* UName, const char* Pass);
 virtual ~DbImpExp();
 virtual StackItem Method(Node &node,::string const &method,int pc,StackItem *stk);
};
//-----------------------------------------------------------------------------
class PosDB_PC: public Struct {
 cursor crsr;
 cursor adcrsr;
 DBStructInfo *DBStr;
 OraView      *dv;
 OraView      *addv;
 PosAnalBuf   drec,adrec;
 FieldSet     *arg;
 CatDB_PC     *cat;
 ObjRec obj;
 int    dbId;
 int    orphan,extart,outer,SumId;
 ::string query;
 ::string ord;
public:
 PosDB_PC(const ::string &dbname,int ext,const ::string &set,int SumId=0,int outer=0);
 PosDB_PC(Node &node,int ext,int SumId=0,int outer=0);
 void Setup();
 // PosDB_PC(const ::string &dbname);
 virtual ~PosDB_PC() { delete(dv); delete(addv); if(orphan) delete(DBStr); }
 virtual StackItem Method(Node &node,::string const &method,int pc,StackItem *stk);
};

class AnRow: public Struct {
 cursor crsr;
 DBStructInfo DBStr;
 OraView      *dv;
 PosAnalBuf   drec;
 int mode;
// ::string       condition ;
public:
 AnRow() :crsr(),dv(0) {}
 void Init(const ::string &aname,int mode,int SumId,const char *article=0,int ver=0,char *header=NULL,const ::string &cond="");
 void LoadVar(Node *ths);
 virtual ~AnRow() { delete(dv); }
};

