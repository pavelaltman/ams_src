//---------------------------------------------------------------------------
#include "AMS_HDR.H"
#ifndef AMS_TREE_VIEWH
#define AMS_TREE_VIEWH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "oracle.hpp"
#include <vector>
#include <map>
#include <algorithm>
//---------------------------------------------------------------------------
class TAmsTreeData
{
protected:
    TTreeNode* TN;
    int Code;
public:
    TAmsTreeData (int Code_, TTreeNode* TN_):Code(Code_), TN(TN_){};
    virtual ~TAmsTreeData (){};
    virtual int GetCode() const {return Code;};
    virtual const AnsiString& GetDescr()const {return TN->Text;};
    virtual int GetLevel()const {return TN->Level;};
};

// Класс для сохранения последлвательности выделенных объектов
class TAmsSelObjsList
{
    std::vector<int> OL;
public:
    void Clear(){OL.clear();};
    // Значение выбранного элемента
    int operator[](int Index) {return OL[Index];};
    // Всего выбрано элементов
    int Count() {return OL.size();};
    // Выбрать элемент (повторно не выбирается)
    void SelectObj (int OId);
    // Удалить элемент из списка выбранных элементов     
    void DeSelectObj (int OId);
    // Формирует строку - выбранные элементы ч.з запятую
    void GetCommaText (AnsiString& Res);    
};

class PACKAGE TAmsTreeView : public TTreeView
{
private:
protected:
    // Сохраняются данные относительно каждого счета(Категории)
    std::vector<TAmsTreeData*> TreeData;
    std::map<int, TTreeNode*> Track;
    // Список выбранных категорий
    TAmsSelObjsList SelectedCats;
    // Внешнее OQ за счет которого идет выборка значений из базы
    TOracleQuery* OQ;
    bool FQueryShared;
    // Если TRUE то производится считывание из вбазы данных каждый раз 
    // как пользователь переходит на уровень вверх/вниз
    bool FRefreshEveryTime;
    // Название схемы из которой берем данные
    AnsiString FBaseSchema;
    // Сохраняется текст SQL запроса
    AnsiString FQueryText;
    // Точка-начало показа
    int BeginPoint;

    bool Ready;
    
    void __fastcall FillTheTreeView(bool DeepQuerySet=true);
    void Clear ();
    virtual void PrepareSQL(const AnsiString& BeginPoint_)=0;
    virtual void PrepareSQL(int BeginPoint_)=0;
    virtual AnsiString FormNodeText()=0;
    virtual TAmsTreeData* FormTreeData(TTreeNode*)=0;
    virtual int GetParentCode()=0;
    virtual int GetCurrLevel()=0;
public:
    __fastcall TAmsTreeView(TComponent* Owner);
    __fastcall virtual ~TAmsTreeView (){Clear();};
    
    void RefreshView();
    virtual void Begin(const AnsiString& BaseSchema_, const AnsiString& BeginPoint_);
    virtual void Begin(const AnsiString& BaseSchema_, int BeginPoint_);
    // Сделать активной категорию с кодом Code
    void MakeActive (int Code);
    // Возвращает TreeNode по коду категории
    TTreeNode* GetNodeByCode (int Code){ return Track[Code];};
    // Возвращает код Активной категории
    int GetCurrCode () ;
    // Возвращает код категории по TTreeNode*
    int GetCode (TTreeNode* ) ;
    // Выбрать активную категорию и поместить в список выбранных категорий
    void SelectCurr();
    // Убрать активную категорию из списка выбранных категорий
    void DeSelectCurr();
    // Список выбранных категорий
    TAmsSelObjsList* GetSelectedCats () {return &SelectedCats;};
    // Вставка нового листа перед TN, в ветку с корнем Root
    // Название Ветки Txt, код кат Code
    TTreeNode* NewLeaf (TTreeNode* Root, TTreeNode* TN, const AnsiString& Txt, int Code);
    // Двигает TN вверх Up = True или вниз Up=False, 
    // возвращет указатель на TreeNode c которым поменялись местами
    // NULL ни с кем не поменялись
    TTreeNode* MoveNode (TTreeNode* TN, bool Up);
    // Удаляет Node по коду категориии
    void DelNodeByCode( int Code);
    
__published:
    __property bool AmsOraQueryShared = {read =FQueryShared, 
                                        write=FQueryShared, default =true};
    __property TOracleQuery* AmsOraQuery = {read= OQ,  write=OQ};
    __property bool AmsRefreshEveryTime = {read =FRefreshEveryTime, 
                                        write=FRefreshEveryTime, default =false};
    __property AnsiString AmsBaseSchema={read=FBaseSchema, write=FBaseSchema};
};
//---------------------------------------------------------------------------
#endif
