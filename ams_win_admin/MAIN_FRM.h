//---------------------------------------------------------------------------
#ifndef MAIN_FRMH
#define MAIN_FRMH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Oracle.hpp"
#include "OracleData.hpp"
#include <Db.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <DBCtrls.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TMainFrm : public TForm
{
__published:	// IDE-managed Components
  TOracleLogon *OLogon;
  TOracleSession *OSess;
  TMainMenu *MainMenu1;
  TMenuItem *N1;
  TActionList *MainActList;
  TImageList *ImageList1;
  TAction *ActShowAcc;
  TAction *Action2;
  TMenuItem *N2;
    TMenuItem *N4;
    TMenuItem *N5;
    TStatusBar *StatusBar1;
    TMenuItem *N9;
    TMenuItem *N6;
    TMenuItem *N8;
    TMenuItem *N10;
    TMenuItem *N3;
    TMenuItem *N11;
    TMenuItem *N12;
    TMenuItem *N13;
    TMenuItem *N14;
    TMenuItem *N15;
    TMenuItem *N16;
    TMenuItem *Gh1;
    TMenuItem *N17;
    TMenuItem *N18;
    TMenuItem *MenRights;
    TMenuItem *MenGiveRights;
    TMenuItem *MenBuildRights;
    TOracleQuery *OQ1;
    TOracleQuery *OQ2;
    TAction *ActMakeAnOrder;
    TMenuItem *N19;
    TMenuItem *N7;
    TMenuItem *N20;
    TMenuItem *N21;
    TAction *ActSaveBuch;
    TAction *ActRestoreBuch;
    TAction *ActClosePeriod;
    TAction *ActRestoreAs;
    TMenuItem *N22;
    TMenuItem *N23;
    TMenuItem *N24;
    TMenuItem *N25;
    TMenuItem *N26;
    TMenuItem *N27;
    TAction *ActNewBuch;
    TMenuItem *N28;
    TAction *ActEditGlobalSetup;
    TMenuItem *N29;
    TAction *ActEditLocalSets;
    TMenuItem *N30;
  void __fastcall ActShowAccExecute(TObject *Sender);
  void __fastcall N3Click(TObject *Sender);
    void __fastcall N5Click(TObject *Sender);
    void __fastcall N8Click(TObject *Sender);
    void __fastcall N16Click(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall N17Click(TObject *Sender);
    void __fastcall N18Click(TObject *Sender);
    void __fastcall MenBuildRightsClick(TObject *Sender);
    void __fastcall MenGiveRightsClick(TObject *Sender);
    void __fastcall ActMakeAnOrderExecute(TObject *Sender);
    void __fastcall N20Click(TObject *Sender);
    void __fastcall Gh1Click(TObject *Sender);
    void __fastcall N21Click(TObject *Sender);
    void __fastcall ActSaveBuchExecute(TObject *Sender);
    void __fastcall ActEditGlobalSetupExecute(TObject *Sender);
    void __fastcall ActEditLocalSetsExecute(TObject *Sender);
    void __fastcall ActRestoreAsExecute(TObject *Sender);
    void __fastcall ActRestoreBuchExecute(TObject *Sender);
    void __fastcall ActClosePeriodExecute(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TMainFrm(TComponent* Owner);
  int InitConn();
};
//---------------------------------------------------------------------------
extern PACKAGE TMainFrm *MainFrm;
extern AnsiString LastCopyWhere;
extern AnsiString WhereCopyPwd;

//---------------------------------------------------------------------------
#endif
