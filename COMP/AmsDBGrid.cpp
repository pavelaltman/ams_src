//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "AmsDBGrid.h"
#pragma link "RXDBCtrl"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAmsDBGrid *)
{
    new TAmsDBGrid(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAmsDBGrid::TAmsDBGrid(TComponent* Owner)
    : TRxDBGrid(Owner)
{
}
//---------------------------------------------------------------------------
namespace Amsdbgrid
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TAmsDBGrid)};
         RegisterComponents("Samples", classes, 0);
    }
}
//---------------------------------------------------------------------------
void __fastcall TAmsDBGrid::MouseMove(Classes::TShiftState Shift, int X, int Y)
{
    TRxDBGrid::MouseMove (Shift, X, Y);
    int LeftSpace = 11;
    if (FixedCols>0 && Y<20)
    {
        if (!FCChange)
        {
            int i = 1;
            int CurrWidth = LeftSpace;
            bool Finded = false;
            while (i<=FixedCols && !Finded )
            {
                CurrWidth += ColWidths[i]+1;
                if ( (CurrWidth - X) < 2 && (CurrWidth - X)>-2)
                {
                    Finded = true;
                    Cursor=crHSplit;
                }
                else i++;
            }
            if (!Finded)
            {
                Cursor=crDefault;
            }
            if (Finded && Shift.Contains(ssLeft))
            {
                // включаем механизм изменени€ ширины колонки
                FixedColsChange = i;
                FCChange= true;
            }
        }
        else
        {
            if (!Shift.Contains(ssLeft))
            {
                FCChange=false;
                Cursor = crDefault;
            }
            else
            {
                int BeginX = LeftSpace;
                int i =1;
                while (i<FixedColsChange)
                {
                    BeginX+= ColWidths[i++];
                }
                int CurrWidth = ColWidths[FixedColsChange];
                //Lab2->Caption = IntToStr (BeginX);
                int Diff = CurrWidth+BeginX - X;
                if (Diff+5 < CurrWidth)
                {
                    ColWidths[FixedColsChange] = CurrWidth - Diff-1;
                }
            }
        }
    }
    else
    {
        FCChange=false;
        Cursor=crDefault;
    }

};
//---------------------------------------------------------------------------
void __fastcall TAmsDBGrid::SetSynchoEnable (int Val)
{
    if (Val)
    {
        if (FOnAmsFooterSyncho)
            if (!ComponentState.Contains(csDesigning) &&
                !ComponentState.Contains(csLoading))
            {
                SSynchoCause S;
                S.Cause = scBegin;
                SynchoEnable = true;
                OldLeft = LeftCol;
                FOnAmsFooterSyncho(this, S);
            }
    }
    else SynchoEnable = Val;
}
//---------------------------------------------------------------------------
void __fastcall TAmsDBGrid::ColumnMoved(int FromIndex, int ToIndex)
{
    TRxDBGrid::ColumnMoved(FromIndex, ToIndex);
    if (SynchoEnable && FOnAmsFooterSyncho)
    {
        SSynchoCause S;
        S.Cause = scColMoved;
        S.From=FromIndex;
        S.To=ToIndex;
        FOnAmsFooterSyncho(this, S);
    }
};
//---------------------------------------------------------------------------
void __fastcall TAmsDBGrid::ColWidthsChanged(void)
{
    TRxDBGrid::ColWidthsChanged();
    if (SynchoEnable && FOnAmsFooterSyncho)
    {
        SSynchoCause S;
        S.Cause = scWidthCol;
        FOnAmsFooterSyncho(this, S);
    }
};
//---------------------------------------------------------------------------
void __fastcall TAmsDBGrid::TopLeftChanged(void)
{
    TRxDBGrid::TopLeftChanged();
    if (SynchoEnable && FOnAmsFooterSyncho && OldLeft != LeftCol)
    {
        SSynchoCause S;
        OldLeft =LeftCol;
        S.Cause = scScroll;
        FOnAmsFooterSyncho(this, S);
    }
};
//---------------------------------------------------------------------------
void __fastcall TAmsDBGrid::SizeChanged(int OldColCount, int OldRowCount)
{
    TRxDBGrid::SizeChanged(OldColCount, OldRowCount);
    if (SynchoEnable && FOnAmsFooterSyncho && OldColCount != ColCount)
    {
        SSynchoCause S;
        S.Cause = scSizeChange;
        FOnAmsFooterSyncho(this, S);
    }
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

