//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "AmsFooter.h"
#include "AmsDBGrid.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAmsFooter *)
{
    new TAmsFooter(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAmsFooter::TAmsFooter(TComponent* Owner)
    : TStringGrid(Owner)
{
    ScrollBars = ssNone;
    FixedCols = 0;
    FixedRows =0;
    RowCount = 3;
    DefaultRowHeight = 18;
    Height = 3*19;
    Color = FixedColor;
    Enabled = false;
    Options>>goDrawFocusSelected;
    OnDrawCell = DrawCellEvnt;
}
//---------------------------------------------------------------------------
namespace Amsfooter
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TAmsFooter)};
         RegisterComponents("Samples", classes, 0);
    }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Синхронизирует футер с основным гридом
void TAmsFooter::SynChro(TAmsDBGrid* Grid, SSynchoCause& S)
{
    CurrGrid= Grid;
    switch (S.Cause)
    {
        case scBegin:
            MapColor.clear();
        case scSizeChange:
            MakeIndex();
            FixedCols = CurrGrid->FixedCols+1;
            Col=0;
            Row=0;
            SetWidth();
            LeftCol = CurrGrid->LeftCol;
        break;

        case scWidthCol:
            SetWidth();
            LeftCol = CurrGrid->LeftCol;
        break;
        case scScroll:
            LeftCol = CurrGrid->LeftCol;
        break;
        case scColMoved:
            ParentColMoved(S.From, S.To);
        break;
    }
};
//---------------------------------------------------------------------------
void TAmsFooter::MakeIndex()
{
    ColIndx.clear();
    Indx.clear();
    Masks.clear();

    for (int i =0; i< CurrGrid->GetColCount();i++)
    {
        Indx.push_back(i);
        ColIndx.push_back(i);
    }
    ColCount = CurrGrid->GetColCount();
}
//---------------------------------------------------------------------------
void TAmsFooter::SetWidth()
{
    for (int i =0; i< CurrGrid->GetColCount();i++)
    {
        ColWidths[i]= CurrGrid->ColWidths[i];
    }
}
//---------------------------------------------------------------------------
void TAmsFooter::ParentColMoved(int From, int To)
{
    // Меняем местами чего написано в Gride
    if (From > To )
    {
        int Ch= ColIndx[From];
        for (int i= From; i>To; i--)
        {
            ColIndx[i]= ColIndx[i-1];
        }
        ColIndx[To]=Ch;
    }
    else
    {
        int Ch=ColIndx[From];
        for (int i=From;i<To;i++)
        {
            ColIndx[i]= ColIndx[i+1];
        }
        ColIndx[To]=Ch;
    }

    // Строим индекс
    Indx.clear();
    for (unsigned int j=0; j<ColIndx.size();j++)
    {
        unsigned int i = 0;
        while( (i<ColIndx.size()) && (ColIndx[i]!= j)) i++;
        Indx.push_back(i);
    }
    //
    MoveColumn(From,To);
}
//---------------------------------------------------------------------------
AnsiString __fastcall TAmsFooter::GetColIndxTxt ()
{
    AnsiString S ;
    for (unsigned int i = 0; i < ColIndx.size(); i++)
    {
        S+=IntToStr(ColIndx[i])+" ";
    }
    return S;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TAmsFooter::GetIndxTxt ()
{
    AnsiString S ;
    for (unsigned int i = 0; i < Indx.size(); i++)
    {
        S+=IntToStr(Indx[i])+" ";
    }
    return S;
}
//---------------------------------------------------------------------------
void TAmsFooter::SetVal(const AnsiString& Val, int Row_, int Col_)
{
    int C = Col_;
    if (Indx.size()> C)
        C = Indx[Col_];
    Cells[C][Row_]=Val;
};
//---------------------------------------------------------------------------
void TAmsFooter::SetVal(double Val, int Row_, int Col_)
{
    int C = Col_;
    if (Indx.size()> C)
        C = Indx[Col_];
    AnsiString Mask = Masks[C];
    if (Mask!="")
    {
        Cells[C][Row_]= FormatFloat(Mask, Val);
    }
    else
        Cells[C][Row_]= FormatFloat(",0.00", Val);
};
//---------------------------------------------------------------------------
void TAmsFooter::SetVal(TDateTime   Val, int Row_, int Col_)
{
    int C = Col_;
    if (Indx.size()> C)
        C = Indx[Col_];
    Cells[C][Row_]=DateToStr(Val);
};
//---------------------------------------------------------------------------
void TAmsFooter::SetVal(int         Val, int Row_, int Col_)
{
    int C = Col_;
    if (Indx.size()> C)
        C = Indx[Col_];
    Cells[C][Row_]=IntToStr(Val);
};
//---------------------------------------------------------------------------
void TAmsFooter::SetMask (const AnsiString& Mask, int Col_)
{
    Masks[Col_]=Mask;
};
//---------------------------------------------------------------------------
void __fastcall TAmsFooter::DrawCellEvnt(TObject *Sender, int Col, int Row,
    const TRect &Rect, TGridDrawState State)
{
    if (MapColor[Col])
    {
        Canvas->Brush->Color=MapColor[Col];
        Canvas->FillRect(Rect);
    }
    AnsiString WhatWrite = Cells[Col][Row];
    if (WhatWrite!="")
    {
        int Len = Canvas->TextWidth(WhatWrite)+4;
        int BegP =Rect.Left+2;
        if (Rect.Width()>Len)
        {
            BegP = Rect.Right-Len;
        }

        Canvas->TextRect(Rect, BegP, Rect.Top+2, WhatWrite);
    }
}
//---------------------------------------------------------------------------

