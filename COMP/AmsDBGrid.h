//---------------------------------------------------------------------------

#ifndef AmsDBGridH
#define AmsDBGridH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "RXDBCtrl.hpp"
#include <DBGrids.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TAmsDBGrid;
enum EnSynchoCause { scBegin, scSizeChange, scWidthCol, scScroll, scColMoved};
struct SSynchoCause
{
    EnSynchoCause Cause;
    int From ;
    int To;
};
typedef void __fastcall (__closure *TAmsNeedSynchoEvent)(TAmsDBGrid* Sender, SSynchoCause& Cause);

class PACKAGE TAmsDBGrid : public TRxDBGrid
{
private:
protected:
    DYNAMIC void __fastcall MouseMove(Classes::TShiftState Shift, int X, int Y);
    int FixedColsChange;        // номер изменяемого столбца
    bool FCChange;              // true  - изменяем ширину столбца
                                // false - не изменяем
    TAmsNeedSynchoEvent FOnAmsFooterSyncho;
    int SynchoEnable;
    void __fastcall SetSynchoEnable (int Val);
    //---------------------------------------------------
    DYNAMIC void __fastcall ColumnMoved(int FromIndex, int ToIndex);
    DYNAMIC void __fastcall ColWidthsChanged(void);
    DYNAMIC void __fastcall TopLeftChanged(void);
    int OldLeft;
    DYNAMIC void __fastcall SizeChanged(int OldColCount, int OldRowCount);
    //---------------------------------------------------
public:
    __fastcall TAmsDBGrid(TComponent* Owner);
    __property ColWidths;
    int GetColCount() {return ColCount;}; 
__published:
    // Вызывается когда необходима синхронизация с футером
    __property TAmsNeedSynchoEvent OnAmsFooterSyncho = {read=FOnAmsFooterSyncho,
                                                write=FOnAmsFooterSyncho};
    __property DefaultRowHeight;
    __property int AmsSynchoEnable={read=SynchoEnable,write=SetSynchoEnable};
};
//---------------------------------------------------------------------------
#endif
