//---------------------------------------------------------------------------

#ifndef AMS_QUERYH
#define AMS_QUERYH
//---------------------------------------------------------------------------
class Struct;

// Класс помощник для "Наследования" Датасета от Куэри
class DS_Q
{
protected:
    TOracleDataSet* ODS;
    TOracleQuery*   OQ;
public:
    //DS_Q():ODS(0), OQ(0){};
    DS_Q(TOracleDataSet* ODS_):ODS(ODS_),OQ(0){};
    DS_Q(TOracleQuery* OQ_):ODS(0),OQ(OQ_){};

    void DeclVar (const AnsiString& VarName, int Var);
    void DeclVar (const AnsiString& VarName, const AnsiString& Var);
    void DeclVar (const AnsiString& VarName, TDateTime Var);
    void DeclVar (const AnsiString& VarName, double Var);

    void DelVars();
    void SetSqlText(const AnsiString& SqlText);
};
//---------------------------------------------------------------------------
// Класс- помощник для работы с пулом OQ
class AmsQuery
{
protected:
    TOracleQuery* OQ;
    bool FromPool;
public:
    AmsQuery(TOracleQuery* OQ_):FromPool(false){OQ=OQ_;};
    AmsQuery();
    ~AmsQuery();
    TOracleQuery* operator ()(void){return OQ;};
    void Exec(const char* Msg);
    //-----------------------------------------------------------------
    // Возвращает 0 если не было эксцепшена
    //            -1 если был но не показывает сообщений об ошибках
    int  ExecSilent();
    bool NotEof () {return !OQ->Eof;};
    void Next () {OQ->Next();};

    void SetSqlTextReady(const AnsiString& Sql)
    {OQ->SQL->Text = Sql;};
    void SetSqlText(const AnsiString& Sql)
    {OQ->SQL->Text = ReplaceBaseUser(Sql);};

    void SetPLText(const AnsiString& PL)
    {OQ->SQL->Text ="begin "+ReplaceBaseUser(PL)+" end;";};
    void SetPLTextReady(const AnsiString& PL)
    {OQ->SQL->Text ="begin "+PL+" end;";};

    void Clear(){if (OQ) OQ->Clear();};
    void ClearVars(){if (OQ) OQ->ClearVariables();};
    void DeleteVars(){if (OQ) OQ->DeleteVariables();};
    //------------------------------------------------------
    void DeclVarDate (const AnsiString& VarName) {OQ->DeclareVariable(VarName, otDate);};
    void DeclVarInt  (const AnsiString& VarName) {OQ->DeclareVariable(VarName, otInteger);};
    void DeclVarSum  (const AnsiString& VarName) {OQ->DeclareVariable(VarName, otFloat);};
    void DeclVarStr  (const AnsiString& VarName) {OQ->DeclareVariable(VarName, otString);};
    //------------------------------------------------------
    void DeclVar(const AnsiString& VarName, TDateTime Val)
        {OQ->DeclareVariable(VarName, otDate);OQ->SetVariable(VarName, Val);};
    void DeclVar(const AnsiString& VarName, int Val)
        {OQ->DeclareVariable(VarName, otInteger);OQ->SetVariable(VarName, Val);};
    void DeclVar(const AnsiString& VarName, double Val)
        {OQ->DeclareVariable(VarName, otFloat);OQ->SetVariable(VarName, Val);};
    void DeclVar(const AnsiString& VarName, const AnsiString& Val)
        {OQ->DeclareVariable(VarName, otString);OQ->SetVariable(VarName, Val);};
    //-------------------------------------------------------
    void DeclVarDate(const AnsiString& VarName, TDateTime Val)
        {OQ->DeclareVariable(VarName, otDate);OQ->SetVariable(VarName, Val);};
    void DeclVarInt(const AnsiString& VarName, int Val)
        {OQ->DeclareVariable(VarName, otInteger);OQ->SetVariable(VarName, Val);};
    void DeclVarSum(const AnsiString& VarName, double Val)
        {OQ->DeclareVariable(VarName, otFloat);OQ->SetVariable(VarName, Val);};
    void DeclVarStr(const AnsiString& VarName, const AnsiString& Val)
        {OQ->DeclareVariable(VarName, otString);OQ->SetVariable(VarName, Val);};
    //-------------------------------------------------------

    void SetVar (const AnsiString& VarName, const AnsiString& Val)
        {OQ->SetVariable(VarName,Val);};
    void SetVar (const AnsiString& VarName, int Val)
        {OQ->SetVariable(VarName,Val);};
    void SetVar (const AnsiString& VarName, double Val)
        {OQ->SetVariable(VarName, Val);};
    void SetVar (const AnsiString& VarName, TDateTime Val)
        {OQ->SetVariable(VarName,Val);};
    //-------------------------------------------------------
    AnsiString GetVarStr (const AnsiString& VarName)
        {return OQ->GetVariable(VarName);};
    int GetVarInt (const AnsiString& VarName)
        {return OQ->GetVariable(VarName);};
    double GetVarSum (const AnsiString& VarName)
        {return (double)OQ->GetVariable(VarName);};
    TDateTime GetVarDate (const AnsiString& VarName)
        {return OQ->GetVariable(VarName);};
    //-------------------------------------------------------
    int GetFldInt (int FldNo) {return OQ->FieldAsInteger(FldNo);};
    double GetFldSum (int FldNo) {return OQ->FieldAsFloat(FldNo);};
    AnsiString GetFldStr (int FldNo) {return OQ->FieldAsString(FldNo);};
    TDateTime GetFldDate (int FldNo) {return OQ->FieldAsDate(FldNo);};
    //-------------------------------------------------------
    int GetFldInt (const AnsiString& FldName) {return OQ->Field(FldName);};
    double GetFldSum (const AnsiString& FldName) {return OQ->Field(FldName);};
    AnsiString GetFldStr (const AnsiString& FldName) {return OQ->Field(FldName);};
    TDateTime GetFldDate (const AnsiString& FldName) {return OQ->Field(FldName);};
    //-------------------------------------------------------
    bool IsFldNull (int FldNo=0) {return OQ->FieldIsNull(FldNo);};
    //-------------------------------------------------------
    void Commit() {OQ->Session->Commit();};
    //-------------------------------------------------------
    void BindVars (Struct* Str);
    void GetVars (Struct* Str);

    void DoPL (const AnsiString& SQL, Struct* Str)
    {
        SetPLText(SQL);
        BindVars(Str);
        Exec("Выполняем PL block");
        GetVars(Str);
    };
};
//---------------------------------------------------------------------------
#endif
