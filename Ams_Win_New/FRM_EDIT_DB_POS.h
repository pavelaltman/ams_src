//---------------------------------------------------------------------------

#ifndef FRM_EDIT_DB_POSH
#define FRM_EDIT_DB_POSH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "AmsInputDate.h"
#include "AmsInputStr.h"
#include "AmsInputInt.h"
#include "AmsInputSum.h"
#include "AmsInputDbPos.h"
#include "AmsInput.h"
#include "AmsInputBool.h"
#include <ActnList.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include "AmsInputAnal.h"
//---------------------------------------------------------------------------
class TFrmBrowsDb;
//---------------------------------------------------------------------------
struct SEditFldsTie
{
    TAmsInput*   InpFld;       //
    SFldOpt*     FldOpt;
    int          FldsDescrIndx;
};
//---------------------------------------------------------------------------
class FieldSetM;
//---------------------------------------------------------------------------
class TFrmEditDbPos : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TBitBtn *BBCancel;
    TBitBtn *BBOk;
    TPopupMenu *PopupMenu1;
    TActionList *ActionList1;
    TAction *Action1;
    TScrollBox *Pan;
    TButton *ButEnter;
    TAction *ActCalc;
    void __fastcall InpDbPosAmsButPressed(TObject *Sender);
    void __fastcall ClrDbPosAmsButPressed(TObject *Sender);
    void __fastcall AmsChanged(TAmsInputAnal *Sender);
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall BBCancelClick(TObject *Sender);
    void __fastcall AmsInpAmsNextElement(TObject *Sender);
    void __fastcall AmsInpAmsPressEnter(TObject *Sender);
    void __fastcall AmsInpAmsPreviosElement(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall ISAmsMultiCopy(TAmsInput *IS,
          const AnsiString &WhatCopy);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall InpAnalClick(TObject *Sender);
    void __fastcall ClrAnalClick(TObject *Sender);
    void __fastcall ButEnterClick(TObject *Sender);
    void __fastcall ActCalcExecute(TObject *Sender);
    void __fastcall IAAmsGetAnalInfo(TAmsInputAnal *Sender);
        void __fastcall ButEnterEnter(TObject *Sender);
protected:	// User declarations
    static int MaxHeight ;
    int     PrevFlags;
    bool    MakeEdit;
    TAmsInput* FirstInputEl; // Какой элемент должен быть активен при
                             // открытии формы
    TAmsInput* LastInputEl;  // Последний видимый элемент

    TAmsInputInt* VerInp;
    TAmsInputStr* ArtInp;
    TAmsInputPoint* AmsInP;

    TFldsDescr* FldsDescr;
    FieldSetM* FSM;
    vector<SEditFldsTie> AllInput;
    TOracleDataSet* ODS;
    void RenPos(void);
    void FillForm(void);
    bool GoBack;

    TForm* ActFrm;
    int AnAnSumId, AnNewSumId, AnAnId;

    virtual void RunProg(bool Post, bool SetFlds, bool ClrFld, bool ClearF4= false, bool UseFsm=true);
    void OkBut();
    virtual void LastElPressed();
    bool Dirty;
    void __fastcall InpElChanged(TObject *Sender);
    bool BeginLookForDirty;

    // Чтобы небыло модальности для ввода аналитики
    TAmsInputAnal* IA;
    bool InputAnalitic;

public:		// User declarations
    bool JKL, bJKL; //EO CH
    bool WithoutCats; //EO CH
    TFrmBrowsDb* FrmBrowsDb;
    TGenLocalVars* LocVar;
    void SetFldsDescr(TFldsDescr* FldsDescr_) {FldsDescr=FldsDescr_;};

    __fastcall TFrmEditDbPos(TComponent* Owner);
    void EditDbPos (TOracleDataSet* ODS_,
                    int Flags, FieldSetM* FSM_,
                    TFldsDescr * FldsDescr=NULL, bool HaveFirstDigit= false,
                    int FirstDigit=0, bool ShModal=true, TGenLocalVars* LocVar=NULL  );
    void AnalOk();
    void AnalCancel();
    int ShtrixAddOne(TOracleDataSet* ODS_,
                     const AnsiString& ShFld_,
                     const AnsiString& ShOrderFld_, int TotPos);
    int ShtrixMakeNewPos(TOracleDataSet* ODS_,
                            const AnsiString& ShFld_,
                            const AnsiString& ShOrderFld_,
                            const AnsiString& ShtrixVal_, int TotPos);
private:
    int jkl, jkldban;
    TComponent *jklcopm;
    AnsiString jklsdb[2];
    void __fastcall DbAnChanged(TObject *Sender);
    void __fastcall ClrPointAmsButPressed(TObject * Sender);
    void __fastcall InpPointAmsButPressed(TObject* Sender);
    AnsiString __fastcall FormArticle(AnsiString str);
    bool FirstChange;
};
//---------------------------------------------------------------------------

extern PACKAGE TFrmEditDbPos *FrmEditDbPos;
//---------------------------------------------------------------------------
#endif

