//---------------------------------------------------------------------------

#ifndef AmsFooterH
#define AmsFooterH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include "AmsDBGrid.h"
//---------------------------------------------------------------------------
class PACKAGE TAmsFooter : public TStringGrid
{
private:
protected:
    // Если пользователь изменит порядок колонок
    std::vector<int> Indx;
    std::vector<int> ColIndx;
    std::map<int,AnsiString> Masks;
    void MakeIndex();
    void SetWidth();
    TAmsDBGrid* CurrGrid;
    void ParentColMoved(int From, int To);

    AnsiString __fastcall GetColIndxTxt ();
    AnsiString __fastcall GetIndxTxt ();

    void __fastcall DrawCellEvnt(TObject *Sender, int Col, int Row,
                        const TRect &Rect, TGridDrawState State);
    std::map<int, TColor> MapColor;
public:
    __fastcall TAmsFooter(TComponent* Owner);

    // Синхронизирует футер с основным гридом
    void SynChro(TAmsDBGrid* Grid, SSynchoCause& S);

    AnsiString  GetStr (int Row_, int Col_);
    int         GetInt (int Row_, int Col_);
    double      GetSum (int Row_, int Col_);
    TDateTime   GetDate(int Row_, int Col_);

    void SetVal(const AnsiString& Val, int Row_, int Col_);
    void SetVal(double      Val, int Row_, int Col_);
    void SetVal(TDateTime   Val, int Row_, int Col_);
    void SetVal(int         Val, int Row_, int Col_);

    void SetMask (const AnsiString& Mask, int Col_);

    // Убрать цвета
    void ClrColors(){MapColor.clear();};
    // Колонку Col выводить цветом Color
    void SetColor(int Col, TColor Color){MapColor[Col]=Color;};

__published:
    __property DefaultRowHeight;
    __property AnsiString AmsColIndxTxt = {read = GetColIndxTxt};
    __property AnsiString AmsIndxTxt = {read = GetIndxTxt};
};
//---------------------------------------------------------------------------
#endif
