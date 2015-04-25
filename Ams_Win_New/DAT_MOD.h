//---------------------------------------------------------------------------

#ifndef DAT_MODH
#define DAT_MODH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Oracle.hpp"
#include <ImgList.hpp>
#include "AmsInputHistory.h"
#include "AMS_READ_CNF.h"
#include "AmsLogon.h"
#include "OracleData.hpp"
#include <Db.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TFrmGetDbCat;
//---------------------------------------------------------------------------

class TDatMod : public TDataModule
{
__published:	// IDE-managed Components
    TOracleSession *OSession;
    TOracleQuery *OQPool1;
    TOracleQuery *OQPool2;
    TOracleQuery *OQPool3;
    TOracleQuery *OQPool4;
    TOracleQuery *OQPool5;
    TImageList *ImageList1;
    TImageList *TreeImageList;
    TAmsInputHistory *InpHist;
    TAmsLogon *OLogon;
    TOracleDataSet *ODS1;
    TOracleDataSet *ODS2;
    TOracleDataSet *ODS3;
    TOpenDialog *OD;
    void __fastcall DataModuleCreate(TObject *Sender);
    void __fastcall DataModuleDestroy(TObject *Sender);
private:	// User declarations
    std::list<TOracleQuery*> FreeQuery;
    std::list<TOracleQuery*> AddQuery;

    std::list<TOracleDataSet*> FreeDS;
    std::list<TOracleDataSet*> AddDS;

    TFrmGetDbCat* FrmGetDbCat;
public:		// User declarations
    __fastcall TDatMod(TComponent* Owner);
    int GetDbCat (int DbId, const AnsiString& DbName, int BegCat= -1);
    int Login(bool ReConnect=false);
    TOracleQuery* GetOQ();
    void ReleaseOQ(TOracleQuery* OQ);

    TOracleDataSet* GetDS();
    void ReleaseDS(TOracleDataSet* OQ);
};
//---------------------------------------------------------------------------
extern PACKAGE TDatMod *DatMod;
//---------------------------------------------------------------------------
class TAmsDS
{
    TOracleDataSet* DS;
public:
    TAmsDS(){DS=DatMod->GetDS();};
    ~TAmsDS(){DatMod->ReleaseDS(DS);};
    TOracleDataSet* operator()(void){return DS;};
};
//---------------------------------------------------------------------------
extern TForm* TopMostForm;
//---------------------------------------------------------------------------
#endif
