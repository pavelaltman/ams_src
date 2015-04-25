//---------------------------------------------------------------------------
#ifndef FLDS_DESCRH
#define FLDS_DESCRH
//---------------------------------------------------------------------------
// Для огранизации просмотра ( Описание полей БД и Аналитик)
struct SFldOptAdd
{
    SFldOptAdd():ParentFldName(), AnSumIndx(-1), SubAccCode(-1), TurnType(){};
    AnsiString ParentFldName;
    int AnSumIndx;            // Индекс аналитической суммы заполняется в FrmBrowsAn
    int SubAccCode;
    AnsiString TurnType;
};
struct SFldOpt
{
    int DispOpt;     // Пользовательские установки
    int DefOpt;      // Администраторские
    int Type;        // Тип Поля
    int DBId;        // Для полей типа ссылка на БД или аналитику
    int FldMaxWidth; // Максимальное количество позиций
    int MaxPoints;   // Количество десятичных знаков
    int DiplWidth;   // Ширина отображаемая
    int FldNo;       // Номер поля в результирующем SQL запросе
    int Lines;       // Количество линий для отображения как мултилайн едит
    AnsiString RealName;    // Название поля в таблице
    AnsiString Alias;       // Подсказка пользователю
    AnsiString Hint;        // Всплывающая подсказка (СOMMENT поля)
    AnsiString FullName;    // Название таблицы в SQL запросе
    AnsiString SqlAlias;    // Alias поля в SQL запросе
    AnsiString SetName;     // Набор установок при выборе поля-ссылки из БД
    int FldAddInfo;         // Какое поле - БД, Ан, Ост, Дополнительное
    SFldOptAdd AddOpt;      // Доп опции - для доп полей
};
//---------------------------------------------------------------------------
struct SRefs
{
    int MainFld;
    int VerFld;
    AnsiString SetName;
};

// Классы для связи с интерпретатором
class Node;
class Struct;
class AmsQuery;

//---------------------------------------------------------------------------
// Описание дополнительных полей
struct SAddAnSumDescr
{
    AnsiString TurnType;
    int SubAccCode;
    int operator == (const SAddAnSumDescr& X )
    { return SubAccCode==X.SubAccCode && TurnType==X.TurnType;};
};
//---------------------------------------------------------------------------
// Список полей
//---------------------------------------------------------------------------
class TFldsDescr
{
protected:
    std::vector<SFldOpt> FOpt;
    std::map<int, SRefs> FRefs; // Пары Номер поля (ссылочного), структура, опр позиц БД
    int DbId;
    AnsiString DbName ;
    int AnId;
    AnsiString AnName;
    AnsiString SetName;

    bool NullVerChecked;
    bool NullVerRule;


    void ReadDbDescr();
    void ReadAnDescr();

    void Add(const SFldOpt& NewElement){FOpt.push_back(NewElement);};
    void Clear() {FOpt.clear();};
    void RebuildDbRef(); // Перестраивает все ссылки на БД и на АНАЛИТИКИ

    void PrepareOQ (AmsQuery& AQ_, bool An=false);
    void FillFO(AmsQuery& AQ_, SFldOpt& FO, const AnsiString& AddFullName="D");

    std::vector<SAddAnSumDescr> AddFldsDescr;
public:
    TFldsDescr():NullVerChecked(false){};
    void SetDbDescr(const AnsiString& DbName_, int DbId_, const AnsiString& SetName_)
    {DbName=DbName_; DbId=DbId_; SetName=SetName_;AnName="";AnId=-1;};
    void SetAnDescr(const AnsiString& DbName_, int DbId_,
                    const AnsiString& AnName_, int AnId_,
                    const AnsiString& SetName_)
    {DbName=DbName_; AnName=AnName_; DbId=DbId_; AnId=AnId_;SetName=SetName_;};

    void ReadFldsDescr();

    const AnsiString& GetDbName(){return DbName;};
    const AnsiString& GetSetName() {return SetName;};
    const AnsiString& GetAnName(){return AnName;};

    SFldOpt& operator [](int Ind) {return FOpt[Ind];};
    SFldOpt& At(int Ind) {return FOpt[Ind];};
    int Count () {return FOpt.size();};
    //---------------------------------------------------
    SRefs& GetRefs(int FldInd)
    {return FRefs[FldInd];};

    // Возвращет перечисленные через запятую данные
    void GetCommaText (AnsiString& Res);

    ~TFldsDescr(){Clear();};
    //---------------------------------------------------
    // Возвращает true если есть правило нулевых версий
    bool IsNullRule();

    AnsiString GetAsString (AmsQuery& AQ, int FldIndx);

    std::vector<SAddAnSumDescr>& GetAddFldsDescr()
    { return AddFldsDescr;};
    const std::vector<SAddAnSumDescr>& GetAddFldsDescr() const
    { return AddFldsDescr;};
    void FormAddAnSums( std::vector<int>*  AddAnSums_);
    void BuildAddSums();
};
#endif
