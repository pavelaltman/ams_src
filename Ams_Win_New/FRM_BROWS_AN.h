//---------------------------------------------------------------------------

#ifndef FRM_BROWS_ANH
#define FRM_BROWS_ANH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AMS_TREE_VIEW.h"
#include "AmsDbCatView.h"
#include "AmsDBGrid.h"
#include "FRM_BROWS_DB.h"
#include "Oracle.hpp"
#include "OracleData.hpp"
#include "RXDBCtrl.hpp"
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <Db.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include "AmsFooter.h"
#include "FOOTER_CNTRL.h"
#include <Buttons.hpp>

#define MAX_CALC 40
#define CALCUNIQUE "CALC_"
//---------------------------------------------------------------------------
class Prog;
class TGenLocalVars;
extern void DistrLine (TFldsDescr* FldsDescr,
                int AnSumEd, int AnSumOst,
                TOracleDataSet* ODS_,
                const AnsiString& SpisOrder = AnsiString("D.VER ASC"));
//---------------------------------------------------------------------------
class TFrmFilterForm;
class OpVars;
//---------------------------------------------------------------------------
class TFrmBrowsAn : public TFrmBrowsDb
{
__published:    // IDE-managed Components
    TAmsFooter *Footer;
    TAction *ActFooter;
    TMenuItem *N10;
    TMenuItem *N11;
    TMenuItem *N12;
    TTimer *TimSelected;
    TAction *ActDelPos;
    TMenuItem *N13;
    TAction *ActClearAn;
    TAction *ActTakeAnSum;
    TAction *ActDestr;
    TAction *ActCalcWithClear;
    TAction *ActCalcVisible;
    TAction *ActClaclVisibleWithClear;
    TMenuItem *N14;
    TMenuItem *N15;
    TMenuItem *N16;
    TMenuItem *N17;
    TMenuItem *N18;
    TMenuItem *N19;
    TPanel *Panel21;
    TBitBtn *BBOk;
    TBitBtn *BBCancel;
    TMenuItem *N20;
    TAction *ActOk_;
    TAction *ActDocGen;
    TAction *ActFormGen;
    TMenuItem *N21;
    TMenuItem *AltM1;
    TMenuItem *AltM2;
    TLabel *LabTotal;
    TAction *ActOperVars;
    TMenuItem *Alt2;
    TAction *ActFilter;
    TMenuItem *Alt3;
    TMenuItem *N22;
    TMenuItem *N23;
    TMenuItem *N24;
    TEdit *EdShtrixCode;
    TToolButton *ToolButton5;
    TAction *ActShtrixModeOnOff;
    TMenuItem *N25;
    TMenuItem *N26;
    void __fastcall ActEditAnExecute(TObject *Sender);
    void __fastcall ActEdDbPosExecute(TObject *Sender);
    void __fastcall ActVisibilityModeExecute(TObject *Sender);
    void __fastcall ODSApplyRecord(TOracleDataSet *Sender, char Action,
          bool &Applied, AnsiString &NewRowId);
    void __fastcall ActHistExecute(TObject *Sender);
    void __fastcall DBGAmsFooterSyncho(TAmsDBGrid *Sender,
          SSynchoCause &Cause);
    void __fastcall ActFooterExecute(TObject *Sender);
    void __fastcall TimSelectedTimer(TObject *Sender);
    void __fastcall ActDelExecute(TObject *Sender);
    void __fastcall ActClearAnExecute(TObject *Sender);
    void __fastcall ActTakeAnSumExecute(TObject *Sender);
    void __fastcall ActDelPosExecute(TObject *Sender);
    void __fastcall ActDestrExecute(TObject *Sender);
    void __fastcall ActCalcExecute(TObject *Sender);
    void __fastcall ActCalcWithClearExecute(TObject *Sender);
    void __fastcall ActCalcVisibleExecute(TObject *Sender);
    void __fastcall ActClaclVisibleWithClearExecute(TObject *Sender);
    void __fastcall DBGFrosenDblClick(TObject *Sender);
    void __fastcall BBCancelClick(TObject *Sender);
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall ActOk_Execute(TObject *Sender);
    void __fastcall ActDocGenExecute(TObject *Sender);
    void __fastcall ActFormGenExecute(TObject *Sender);
    void __fastcall ActOperVarsExecute(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall ActFilterExecute(TObject *Sender);
    void __fastcall ActShtrixModeOnOffExecute(TObject *Sender);
    void __fastcall ButEnterClick(TObject *Sender);
private:        // User declarations
public:         // User declarations
    //------------------------------------------------------
    // ��� ������� ��������� ���������� ��������������
    struct BegMode
    {
        BegMode():
                ReadOnly(false),EdMode(-1),
                BegVer(0), BegCatId(0), ShowVer(true),
                WithoutCat(false), AccCode(0), Vars(0),NeedDistr(false),
                CheckNeg(false), ShtrixFld(), ZeroVer(false), NoVerOrd(){};
        bool ReadOnly;      // ����������� � ������ ������ ��� ������
        int EdMode;         // ����� ����� ��� ����, ������� �� �����, ���� ���
        AnsiString BegArt;  // �� ����� ������� �����������������
        int BegVer;         //           ����
        int BegCatId;       // ��� ��������� ���������
        bool ShowVer;       // ���������� �� ������
        bool WithoutCat;    // ��� ���������
        TGenLocalVars* Vars; // OPV() � ACCV()
        int AccCode;
        AnsiString BalFld;      // ���������� ����

        bool NeedDistr;         // ����� �� �������������
        bool    AddToRem;       // �� ������ ��� ������� ��������
        int     PredSumId;      // ����� ������� ���� �����
        bool    CheckNeg;       // ��������� �� ��������������� �������?
        bool    ZeroVer;        // �������� ����� ������� ������
        AnsiString ShtrixFld;   // ����� ����� �� ���������
        AnsiString ShtrixOrderFld; // �� ������ ���� ���������
        AnsiString NoVerOrd; // �� ������ ���� ������� ������������� �� �������

        bool newop; //����� ��� ��������������
    } ;
    BegMode BM1;
      TOracleDataSet * ODS1; 

    //------------------------------------------------------

    __fastcall TFrmBrowsAn(TComponent* Owner);
    void Begin(TForm* Parent_, int AnCode, const AnsiString& AnName,
                const AnsiString& SetName, const AnsiString& Descr,
                int AnSumEd_, int AnSumOst_, const BegMode& BM= BegMode());
    void OperSelectedOk(int SumId);
    void MakeInsert(TOracleDataSet* ODS);
    virtual OpVars* GetOpVars();

    void CalcAnSumWithoutVer(AnsiString art_);
protected:
    bool Dirty;
    AnsiString NewRowId_; // ��� �������� ������ RowId
    AnsiString BalMask; // ������ ��� ���������� �����


    void PrepareForm (TForm* Parent_, int AnCode, const AnsiString& AnName,
                const AnsiString& SetName, const AnsiString& Descr,
                int AnSumEd_, int AnSumOst_, const BegMode& BM= BegMode());

    void GenerateDoc(int ObjType);


    AnsiString NoVerOrd; // �� ������ ���� ������� ������������� �� �������

    bool FreeExecuted;
    TForm* ParentF;
    //---------------------------------------------------------------------------

    virtual AnsiString GetOrderBy(void);

    std::vector<int> AddAnSums;  // ����� ������������� ����� ��� �������������� �����

    virtual void BuildCols(void);
    void BuildSqlQuery(TOracleDataSet* ODS_);
    virtual void ShowMode();
    virtual void GetFromAndWhere (AnsiString& Beg, DS_Q D);
    void GetAllAddAnSum (); // ��������� AllAddAnSum (���� ������ ���� ��� ���� ��� �����)

    int EdMode;    // ����� ����������� (��� ����, ������� �� �����, ��������� ���)
    int EditWhat ; // ��� �����������
    int CurrAnId;
    int AnSumEd;
    int AnSumOst;
    int ReadOnly;
    int TotalPos;
    int AccCode;
    AnsiString BalFld; // ���������� ����
    bool NeedDistr;
    bool AddToRem;       // �� ������ ��� ������� ��������
    int PredSumId;       // ����� ������� ���� �����
    bool CheckNeg;       // ��������� �� ��������������� �������?
    bool ShtrixCodeMode; // ������� �� ����� ����� ������ �� �����-����
    AnsiString ShtrixFld;// ����� ����� �� ���������
    AnsiString ShtrixOrderFld;
    int ShtrixCodeCounter;

    int SxSaveEdMode;        // ��� �������� ������� ��� �������� �� ������
    bool SxSaveWithoutCats;  // ����� �� ����-����


    TFrmFilterForm* FilterForm;

    AnsiString ArtSel; // ��� ����������� ������ ����� ART= V=
    int VerSel;
    int CatIdSel;
    bool ArtVerSel;

    bool ZeroVer;

    bool ExistFooterChecked; // ��� �������� ������������� ����� ������ � ������� ������

    FieldSetAnTotal TotalAll;       // 0- ����� ������ ��� ���� ������ �����
    FieldSetAnTotal TotalShowLines; // 1- ����� ������ ��� ������������ �������
    FieldSetAnTotal TotalCurrCat;   // 2- ����� ������ ��� ������� ���������
    FieldSetAnTotal TotalCatTree;   // 3- ����� ������ ��� ������������ ��������� � �����
    FieldSetAnTotal TotalSelected;  // 4- ����� ������ ��� ��������� �����

    // ������ ������������ ����� ���� � DataSet - ����� �������
    std::vector<int> ColsIndx;
    // ������ ������� ����� ���� "�������� � �����"
    std::vector<int> ZestFlds;
    bool IncludeInSum();

    TFooterCntrl FooterCntrl;
    // ���� == -1 �� ��� ����� ������ ���������
    void RecalcFooter(int Indx = -1, bool Redraw = true);

    void ChangeFooter (int Sign);   // ������������ ������� ������ ��� �����������
                                    // ������ ����� ����������� ������

    virtual void CatChanged ();     // ��������� ����������

    virtual void BeginSelection();  // ������������ ������� ����� ����� ������
    virtual void EndSelection();    // ����������� ����� ��������� �����

    virtual void SetFilterOn();     // ���������� ������ �� F5
    virtual void SetFilterOff();    // ������� ������

    virtual void ViewUpdated();

    // ������������� ������� ��� �������� (��� �������� ����)
    void Recalc (bool ClearFlds, TOracleDataSet* ODS_);
    bool SetTotalNow;

    bool DeletePosAn;

    virtual void TakeAnSum();
    virtual void BeginEdit(int Key);
    virtual void EditOperVars();

    void ShowBalSum();
    void MakeNewTotal();
    void SetNewSumId(int NewSumId);
    void OperSelectedFalse();
    virtual void SetFilter();
    virtual void VirtKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);

    void DistrLine2();
    Prog* GetAnProg();
    void RecalcOneArticle(const AnsiString& Art);
    AnsiString RecalcArticle;

    int jkl; SFldOpt CalcFO[MAX_CALC];
    //AnsiString CalcNames[MAX_CALC]; int CalcWidth[MAX_CALC];
    void RefreshView(void);
    bool ChangeShtrixMode;
    void PrepareODS1(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmBrowsAn *FrmBrowsAn;
//---------------------------------------------------------------------------
void EditAnalSum(TForm* Parent_,int AnId,AnsiString AnName,AnsiString SetName,AnsiString Descr,int ASEd,int ASOst) ;
#endif
