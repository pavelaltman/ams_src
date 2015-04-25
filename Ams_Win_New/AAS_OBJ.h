//---------------------------------------------------------------------------

#ifndef AAS_OBJH
#define AAS_OBJH
//---------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class AasObj: public Struct {
    string WhereAdd;        // Что добавить в where выражение при суммировании
    int ResSum;
    int AnCode;
    string AnName;
    string DbName;
    Struct* CondValues;
public:
 AasObj(int AnCode_, const string& AnName_, const string& WhereAdd_, Struct* S );
 virtual ~AasObj();
 virtual StackItem Method(Node &node,::string const &method,int pc,StackItem *stk);
};
//-----------------------------------------------------------------------------

#endif
