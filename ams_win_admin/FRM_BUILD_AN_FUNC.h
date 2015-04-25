//---------------------------------------------------------------------------

#ifndef FRM_BUILD_AN_FUNCH
#define FRM_BUILD_AN_FUNCH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "Oracle.hpp"
#include <Menus.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TFrmBuildAnFunc : public TForm
{
__published:	// IDE-managed Components
    TMemo *MemSQL;
    TOracleQuery *OQ1;
    TOracleQuery *OQ2;
    TOracleQuery *OQ3;
    TMainMenu *MainMenu1;
    TMenuItem *N1;
    TMenuItem *N2;
    TMenuItem *N3;
    TMenuItem *N4;
    TMemo *MemResult;
    TSplitter *Splitter1;
    TOracleQuery *OQ4;
    TSaveDialog *SD;
    TOpenDialog *OD;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall N2Click(TObject *Sender);
private:	// User declarations
    int WhatBuild;
    AnsiString GrantProc;
public:		// User declarations
    __fastcall TFrmBuildAnFunc(TComponent* Owner);
    virtual int BuildThis(int BuildWhat);
    void BuildGetKolPos();
    void BuildDelDbPos();
    int BuildShortDbInfo();
    int BuildCAS();
protected:
    TForm* SetupFrm;
    void BuildAAS(int AnCode, const AnsiString& AnName, const AnsiString& Opt, AnsiString& Res);
    int CheckAndApply(const AnsiString& FuncName, const AnsiString& FuncText, const AnsiString& PlSqlType);
    int BuildAAS_All();
    void BuildAASW(int AnCode, const AnsiString& AnName, const AnsiString& Opt, AnsiString& Res);
    int BuildAASW_All();
    int BuildHistSupport();
    int BuildRenPos();
    void BuildTextHistSupp (int DbId, const AnsiString& DbName, AnsiString& FuncText1, AnsiString& FuncText);
    void BuildIsBusyPos();
    void BuildRenPos(int DbId, const AnsiString& DbName, int POpts, AnsiString& FuncText);
    int BuildAnInfo(void);
    void BuildAnInfo(int AnId, const AnsiString& AnName,
                     int DbId, const AnsiString& DbName, AnsiString& ResFunc);
    void BuildCheckDbRefToDb (int DbId, const AnsiString& DbName, AnsiString& TxtVer, AnsiString& TNoVer);
    void BuildCheckAnRefToDb (int DbId, const AnsiString& DbName, AnsiString& TxtVer, AnsiString& TNoVer);
    void BuildCheckOpers    (int DbId, const AnsiString& DbName, AnsiString& TxtVer, AnsiString& TNoVer);
    void BuildCheckRems     (int DbId, const AnsiString& DbName, AnsiString& TxtVer, AnsiString& TNoVer);
    void BuildCheckDbRefToAn(int DbId, const AnsiString& DbName, int AnId, const AnsiString& AnName,
                                AnsiString& TxtVer, AnsiString& TNoVer);
    void BuildCheckAnRefToAn(int DbId, const AnsiString& DbName, int AnId, const AnsiString& AnName,
                                AnsiString& TxtVer, AnsiString& TNoVer);
    void BuildShortDbInfo(int DbId, const AnsiString& DbName, AnsiString& ResFunc);
    void BuildCAS(int AnCode, const AnsiString& AnName, const AnsiString& Opt, AnsiString& Res);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmBuildAnFunc *FrmBuildAnFunc;
//---------------------------------------------------------------------------
#endif
