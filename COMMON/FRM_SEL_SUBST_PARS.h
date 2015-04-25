//---------------------------------------------------------------------------

#ifndef FRM_SEL_SUBST_PARSH
#define FRM_SEL_SUBST_PARSH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "OracleData.hpp"
#include "OracleNavigator.hpp"
#include <Db.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TFrmSelSubstPars : public TForm
{
__published:	// IDE-managed Components
    TOracleNavigator *OracleNavigator1;
    TDataSource *DataSource1;
    TOracleDataSet *ODS;
    TDBGrid *DBG;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmSelSubstPars(TComponent* Owner);
    void SelSubstPars(int RepId, const AnsiString& SqlQ, const AnsiString& GroupName, bool MultiSel, AnsiString& Result);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmSelSubstPars *FrmSelSubstPars;
//---------------------------------------------------------------------------
#endif
