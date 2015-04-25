//---------------------------------------------------------------------------

#ifndef AmsStringGridH
#define AmsStringGridH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class PACKAGE TAmsStringGrid : public TStringGrid
{
private:
protected:
public:
    __fastcall TAmsStringGrid(TComponent* Owner);
__published:
    __property OnResize;
};
//---------------------------------------------------------------------------
#endif
