//---------------------------------------------------------------------------

#ifndef AMS_CONSTH
#define AMS_CONSTH
//---------------------------------------------------------------------------
// Константы в AMS бухгалтерии

#define CAMS_BadObjId           -1
// Типы объектов
#define CAMS_ObjType_Doc        1 
#define CAMS_ObjType_Frm        2
#define CAMS_ObjType_StdOper    3
#define CAMS_ObjType_DB         4
#define CAMS_ObjType_Anal       5

// Класс преобразования типа параметра в строку и обратно в число
namespace ParamsStr
{
    const int MaxVals=12;
    const AnsiString Vals[MaxVals]={
    "0- строка",
    "1- целое",
    "2- сумма",
    "3- дата",
    "4 - Категория",
    "5 - Группа категорий",
    "6- Субсчет",
    "7- Группа субсчетов",
    "8- Группа строк",
    "9- строка с разделителями",
    "10- позиция базы данных",
    "11- группа позиций базы данных" };

};

void FillStringsByParamsStr (TStrings* S)
{
    for (int i =0; i<ParamsStr::MaxVals; i++)
        S->Add(ParamsStr::Vals[i]);
};





#endif
