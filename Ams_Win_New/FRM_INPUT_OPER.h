//---------------------------------------------------------------------------

#ifndef FRM_INPUT_OPERH
#define FRM_INPUT_OPERH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "OracleData.hpp"
#include <Db.hpp>
#include <DBCtrls.hpp>
#include <Mask.hpp>
#include "RXDBCtrl.hpp"
#include "ToolEdit.hpp"
#include <Buttons.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include "LOCAL_VARS.h"
#include "RxDBComb.hpp"
#include "Oracle.hpp"

class TFrmBrowsAn ;
class TFrmInputSumNM;
class TFrmEditDbPosOnly;
class TFrmAskAccFirst;
class TFrmAskDK;
//---------------------------------------------------------------------------
typedef struct {
    int COND_SUBID;
    int CODE;
    int NODE;
    AnsiString SETNAME;
    AnsiString SIDE;
    AnsiString FP;
} TAccs;

class TFrmInputOper : public TForm
{
__published:	// IDE-managed Components
    TOracleDataSet *DSet;
    TDataSource *DSrc;
    TLabel *Label1;
    TDBEdit *text_edit;
    TLabel *Label2;
    TDBDateEdit *DBDateEdit1;
    TDBEdit *DBEdit1;
    TLabel *Label3;
    TDBEdit *DBEdit2;
    TLabel *Label4;
    TRxDBGrid *items_grid;
    TOracleDataSet *DSetItems;
    TStringField *DSetItemsASIDE;
    TStringField *DSetItemsANAME;
    TStringField *DSetItemsANALINFO;
    TFloatField *DSetItemsVALUE;
    TDataSource *DSrcItems;
    TDBCheckBox *storno_check;
    TBitBtn *BBOk;
    TBitBtn *BBCancel;
    TBitBtn *BBPost;
    TToolBar *ToolBar1;
    TToolButton *ToolButton2;
    TToolButton *ToolButton3;
    TToolButton *ToolButton8;
    TToolButton *ToolButton9;
    TToolButton *ToolButton10;
    TToolButton *ToolButton11;
    TActionList *ActionList1;
    TAction *ActEdit;
    TAction *ActRead;
    TAction *ActAcc;
    TAction *ActDeb;
    TAction *ActOtl;
    TAction *ActDocType;
    TAction *ActStand;
    TAction *ActStandIz;
    TAction *ActAuto;
    TAction *ActDelNeS;
    TPopupMenu *PopupMenu1;
    TMenuItem *Enter1;
    TMenuItem *N3;
    TMenuItem *Alt1;
    TMenuItem *Alt2;
    TMenuItem *Alt3;
    TMenuItem *Alt4;
    TMenuItem *Alt5;
    TMenuItem *Alt6;
    TMenuItem *N4;
    TMenuItem *N5;
    TMenuItem *Alt7;
    TFloatField *DSetOP_ID;
    TFloatField *DSetOP_SUBID;
    TIntegerField *DSetSTATUS;
    TIntegerField *DSetPOST;
    TIntegerField *DSetSTORNO;
    TStringField *DSetTEXT;
    TStringField *DSetDOC_TYPE;
    TStringField *DSetDOC_NUM;
    TFloatField *DSetDOC_NUM_V;
    TDateTimeField *DSetOP_DATE;
    TFloatField *DSetCOND_ID;
    TFloatField *DSetEDITED;
    TStringField *DSetItemsSIDE;
    TFloatField *DSetItemsCODE;
    TIntegerField *DSetItemsANAL_ID;
    TFloatField *DSetItemsITEM_SUM_ID;
    TFloatField *DSetItemsOP_ID;
    TFloatField *DSetItemsOP_SUBID;
    TDateTimeField *DSetItemsOP_DATE;
    TIntegerField *DSetItemsNODE;
    TIntegerField *DSetItemsFOR_POST;
    TStringField *DSetItemsSETNAME;
    TButton *ButBegin;
    TDBText *DBText1;
    TAction *ActOk;
    TOracleQuery *CondOQ;
    TOracleQuery *QCondItems;
    TPopupMenu *PMenuGrid;
    TMenuItem *NewAcc1;
    TMenuItem *DelAcc2;
    TAction *ActShowTiedDB;
    TButton *ButEnter;
    TAction *ActDocGen;
    TBitBtn *BitBtn1;
    TLabel *LabDbInfo;
    void __fastcall BBCancelClick(TObject *Sender);
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall BBPostClick(TObject *Sender);
    void __fastcall DSetItemsANAMEChange(TField *Sender);
    void __fastcall DSetItemsASIDEChange(TField *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall DSetItemsVALUEChange(TField *Sender);
    void __fastcall DSetItemsNewRecord(TDataSet *DataSet);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall ActEditExecute(TObject *Sender);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall items_gridDblClick(TObject *Sender);
    void __fastcall DSetItemsApplyRecord(TOracleDataSet *Sender,
          char Action, bool &Applied, AnsiString &NewRowId);
    void __fastcall items_gridKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall ActDocTypeExecute(TObject *Sender);
    void __fastcall DBDateEdit1Exit(TObject *Sender);
    void __fastcall DBEdit2Enter(TObject *Sender);
    void __fastcall DBEdit2Exit(TObject *Sender);
    void __fastcall ActOkExecute(TObject *Sender);
    void __fastcall ActStandExecute(TObject *Sender);
    void __fastcall ActStandIzExecute(TObject *Sender);
    void __fastcall ButBeginClick(TObject *Sender);
    void __fastcall NewAcc1Click(TObject *Sender);
    void __fastcall DelAcc2Click(TObject *Sender);
    void __fastcall ActShowTiedDBExecute(TObject *Sender);
    void __fastcall ButEnterClick(TObject *Sender);
    void __fastcall ActDocGenExecute(TObject *Sender);
    void __fastcall DSetApplyRecord(TOracleDataSet *Sender, char Action,
          bool &Applied, AnsiString &NewRowId);
    void __fastcall ActReadExecute(TObject *Sender);
        void __fastcall BBCancelMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall BBCancelMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);

protected:	// User declarations
    TGenLocalVars LV;
    bool LocalVarsFormed;
    //----------------------------------
    // �������������� ��������� �� �����
    int AnAnSumId;
    int AnNewSumId;
    int AnAnId;
    TFrmBrowsAn* FrmEditAn;
    TFrmEditDbPosOnly* FrmEditDbPosOnly;

    TFrmAskAccFirst* FrmAskAccFirst;  // ��� ����� ����� ������

    int CondId;             // ��� CO �� ������ ������� �������� ������� ��������
    int CurrAcc;            // ������� ���� � �����������

    //-----------------------------------------------------------
    bool TiedBaseToOper;    // True - ��������� ���� ����������� � �������� � ��
                            // � ���������
    //-----------------------------------------------------------
    bool IsReadTiedBase;          // ������� �� �������� ��������� ����
    AnsiString TiedBase;        // ��� ��������� ����
    AnsiString TiedBaseArticle; // ������� ��� ��������� ����
    int        TiedBaseVer;     // ������ ��� ��������� ����
    bool NotShowFirstTimeTDb;   // �� ���������� ������ ��� ��������� ��

    AnsiString PredFormula;

    int AutoApply;
    int AutoGen;
    int AutoInput;
    int SetBeginBut;

    std::vector<int> AccCodes;
    std::vector<TAccs> TempAccCodes;//��� ��������(������ �����)
    TFrmAskAccCode* FrmAskAccCode;
    void EditAnalitic(int AnSumId_, int AnId_, int AccCode_,
                            bool Debet,const AnsiString& SetName_ );

    bool SecondStep;            // �� ��� ������� �������� ��� ������ ���
    int TotalSubId;             // ������� � ����������� �������� �����������
    AnsiString DocNum;          // ����� ���������
    TDate OpDate;               // ���� �������� ��� ��������� ��������
    enum EnDocNumOk {notChecked, CheckedOk, CheckedNotOk};
    EnDocNumOk DocNumOk;
    int ChangeOper(bool SaveNumber);
    bool DeepCopy;

    bool NumberOut;

    int CondOrd;
    int BeginAutoInput(void);                // ����� COND_ID


    // ��� ���������� �� �����������
    enum EnBrowsState { enIdle=0, enExecute, enWaitForUserInput, enWaitForUserInputS,
                        enWaitForDbInput};
    EnBrowsState CurrState;
    TFrmBrowsAn* FrmBrowsAn;
    TFrmInputSumNM* FrmInputSumNM;
    void CycleBody();

    bool Dirty;

    //
    struct SInpOper
    {
        int NewSumId;
        int CurrSumId;
        int Acc;
        bool AccNew;
        AnsiString BalFld;
        AnsiString AnalName;
        double Sum_;
    };

    SInpOper InpOper;

    virtual bool __fastcall WantChildKey(Controls::TControl* Child,
                            Messages::TMessage &Message);

    void BeginAutoInput2(void);

    bool InputOperEnd;
    void FormAccv(int Code);
    void ReadTiedBase();
    void ShowTiedDbShortInfo();

    //CH
    int __fastcall MakeTmpAccs(int CondId_,int CondSubId_);
    int __fastcall AddNewAnSums(int CondId_,int CondSubId_);
    bool AddNewAnalSum;//������������� ���������� ������
    //EO CH
    bool MiceOverClose;

public:		// User declarations

    __fastcall TFrmInputOper(TComponent* Owner);

    int op_id, op_subid ;   // ���, ������ ��������
    int old_id, old_subid ; // ���, ������ ���������� ��������
    int del ;               // ������� ��� ������������� ��������� ��������
    int readonly ;          // ������ ��������
    int newop ;             // �������� ����� ��������
    int inputed ;           // ���-�� ������� (���� ��� ���������)
    int ok ;                // ��� ������� �����


    void Init(int _op_id, int _op_subid,int _del,
        int _readonly, int CondId_=0 ) ;
    int  HandleOper(int post) ;
    void CloseForm() ;
    void AnalOk();
    void AnalCancel();
    int GenerateDocument(const AnsiString& DocType);
    void SyntOk(Sum Sum_);
    void SyntCancel(void);
    void BeginTiedDbInput(bool FromAutoInput);
    void TiedDbEditEnd(void);

    // ���������� ��� ������������� ����� �� ��������������
    int GetPredSumId();

    // ��������� �������� �� ��������������:
    //         �����, ���� ��������, ���� ���������
    enum EnOperState {osNew, osWasPosted, osWasCommited};
    EnOperState PrevState;

    // ���������� ���� ��������� (������+SIDE)
    int SignOper();

    int CM_Var;
private:

};
//---------------------------------------------------------------------------
extern PACKAGE TFrmInputOper *FrmInputOper;
//---------------------------------------------------------------------------
#endif
