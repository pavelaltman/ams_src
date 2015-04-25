//--------------------------------------------------------------------------
#ifndef ACC_MANH
#define ACC_MANH
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Acc Manager работа с Остатками, оборотами счета/счетов
//-----------------------------------------------------------------------------

class AccMan: public Struct {
    string WhereAdd;        // Что добавить в where выражение при суммировании
    int AnCode;
    string AnName;
    string DbName;
    Struct* CondValues;

    int MapCount;
    map<string, string> NamesToAccs;


    DateTime DateBeg;
    DateTime DateEnd;

    int ResSumBR;
    std::vector<string>BRNames;

    int ResSumER;
    std::vector<string>ERNames;

    int ResSumDT;
    std::vector<string>DTNames;
    string CondDt;

    int ResSumCT;
    std::vector<string>CTNames;
    string CondCt;

    int ResSumCorDT;
    std::vector<string>CorDTNames;
    std::vector<string>CorDTNamesC;
    std::vector<string>CorDTNamesNot;
    string CondCorDt;

    int ResSumCorCT;
    std::vector<string> CorCTNames;
    std::vector<string> CorCTNamesD;
    std::vector<string> CorCTNamesNot;
    string CondCorCt;
public:
    AccMan(     const DateTime& DateBeg_, const DateTime& DateEnd_,
                const string& WhereAdd_, Struct* S );
    virtual ~AccMan();
    virtual StackItem Method(Node &node,::string const &method,int pc,StackItem *stk);
protected:
    void ReadAccs (int pc,StackItem *stk, std::vector<string>& Vals, int& CurrPos);
    void ReadText (int pc,StackItem *stk, string& Val, int& CurrPos);
    void ReadNotText (int pc,StackItem *stk, string& Val, int& CurrPos);
    void BuildSql(string& Sql, Struct* Pars);

    void BuildRemSql(string& Sql, string& Sql2, int RemType);
    void BuildRemAccSql(string& Sql,string& Sql2, const string& AccCode, const string& AS);
    void BuildTurnSql(string& Sql, string& Sql2, int TurnType);
    void BuildCorTurnSql(string& Sql, string& Sql2, int TurnType);

    bool NeedCount (const string& What);
    void FormAccList (string& AccsList, vector<string>* Accs);
    void FindDbDescr();
    void SaveResults(Struct* S);
};
//-----------------------------------------------------------------------------


#endif
