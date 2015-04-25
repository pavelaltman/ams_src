//---------------------------------------------------------------------------

#ifndef LOCAL_VARSH
#define LOCAL_VARSH
//---------------------------------------------------------------------------
// Ћокальные переменные, которые ранее были глобальными
//---------------------------------------------------------------------------
#include "analytic.h"
//---------------------------------------------------------------------------
class TGenLocalVars
{
    OpVars* opvars;     // то что доступно ч/з opv
    OpVars* accvar;     // то что доступно ч/з accv
    OpVars* CurrVars;   // то что доступно ч/з DUMP()
public:
    TGenLocalVars():opvars(0), accvar(0), CurrVars(0){};
    void MakeCopy (const TGenLocalVars& CPY);
    ~TGenLocalVars(){delete opvars; delete accvar; delete CurrVars;};
    OpVars* OPV(){if (!opvars) opvars= new OpVars(); return opvars;};
    OpVars* ACCV(){if (!accvar) accvar= new OpVars(); return accvar;};
    OpVars* VARS(){if (!CurrVars) CurrVars= new OpVars(); return CurrVars;};
    void ClearVars();
};
//---------------------------------------------------------------------------
// ¬ыполн€ет вызов выполнени€ отчетной формы
extern void BeginExecReport(const AnsiString& FormName, TGenLocalVars* LV=NULL);
//---------------------------------------------------------------------------
#endif
