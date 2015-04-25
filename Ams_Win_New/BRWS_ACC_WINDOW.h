//---------------------------------------------------------------------------

#ifndef BRWS_ACC_WINDOWH
#define BRWS_ACC_WINDOWH
//---------------------------------------------------------------------------
// Содержится вспомагательный класс для формирования SQL запросов в окне
// Просмотр счета
//---------------------------------------------------------------------------
struct SBrwsAccWindowOptions
{
    enum enDK {dkDebet=1, dkCredit=2, dkBoth=3};
    enDK DK;
    bool ByDate;           //
    bool Analitic;         //
    bool SortByModifDate;  //
    bool DetailedCorresp;  // Развернутая корреспонденция счетов
    int  AccsTreeDepth;    // Какие счета показывать -1 - только листья
                           // (0-1-2) == LEVEL
    bool CheckOps;
    //--------------------------------------------------------------------
    AnsiString FldName;    // Имя поля по которому ставим режим просмотра
    AnsiString AnName;     // Имя аналитики
    AnsiString SetName;    // 
    int AnCode;            // Код аналитики
    int DbCode;            // Код базы данных (по которой ведем аналитику)
    //--------------------------------------------------------------------
    TDate DateBeg;
    TDate DateEnd;
    int AccCode;
    AnsiString BalFld;
    //--------------------------------------------------------------------
    AnsiString AddWhere;
    AnsiString OrderBy;
    //--------------------------------------------------------------------
    bool WithoutVer;
    AnsiString FilterArticle;   // По какому артикулу фильтруем
    int FilterVer;              //
    //--------------------------------------------------------------------
    // Для неровного периода
    bool PerNotPlain;           // Период не ровный
    bool SyntOnly;              // только балансовое поле можем показать
};
//---------------------------------------------------------------------------
class TBrwsAccWindow
{
public:
    struct SAccs
    {
        bool Debet;
        int Code;
        AnsiString Name;
    };
    TBrwsAccWindow():Setted(0){};
    void BuildSqlQuery( SBrwsAccWindowOptions& CurrOpt,
                        DS_Q& DS, AnsiString& Sql, AnsiString& Simple, bool Deep=false);
    void BuildRemQuery(
            DS_Q& DS, AnsiString& Sql,
            int& DtAcc, int& CtAcc);//  Возвращает к-во счетов по Д/Кр
protected:
    static AnsiString Str[2][4];
    SBrwsAccWindowOptions PrevOpt;
    bool Setted;            // Были ли установлены опции
    vector<SAccs> SelAcc;
};
//---------------------------------------------------------------------------
#endif
