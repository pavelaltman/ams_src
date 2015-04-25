//---------------------------------------------------------------------------

#ifndef FRM_FIND_DB_POS2H
#define FRM_FIND_DB_POS2H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <ActnList.hpp>
#include "AmsInpCond.h"
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------
/*
struct SSearchDescr
{
    bool Clear;    // True -- ������� ������ �� �����������
    int Type;      // ��� ����
    int FldNo;     // ����� ���� ������ TFldsDescr
    int EqSign;    // ���� ��������� (�����������)
    bool CaseSens; // ��� ��������� ��������� ������� ����
    AnsiString CmpWhat;
    SSearchDescr(): Clear(true), FldNo(-1), EqSign(0), CaseSens(false){};
};
*/  
//---------------------------------------------------------------------------
struct SFldCondToFldOpt
{
    SFldOpt * FOpt;
    TAmsInpCond* FCond;
};
//---------------------------------------------------------------------------
class TFrmFindDbPos : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TBitBtn *BBCancel;
    TBitBtn *BBOk;
    TActionList *ActionList1;
    TAction *ActDelAllCond;
    TAction *ActInsAndExec;
    TAction *ActAddCond;
    TAction *ActDelCond;
    TToolBar *ToolBar1;
    TToolButton *ToolButton1;
    TScrollBox *SBox;
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall BBCancelClick(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall ActInsAndExecExecute(TObject *Sender);
    void __fastcall ActDelAllCondExecute(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
protected:
    TFldsDescr* FldsDescr;
    std::vector<SFldCondToFldOpt> IC;
    AnsiString DbName;    // �������� ���� ������ � ������� ����
    TAmsInpCond* FirstC;
    AnsiString ChangeUkrI (AnsiString s) {if (GC::EnglI) for (int i=1; i<=s.Length(); i++) if (s[i]=='�') s[i]='I'; return s;}

public:		// User declarations
    // ������� ��� ���������� ������� � ������
    AnsiString AddStr;

    __fastcall TFrmFindDbPos(TComponent* Owner);
    // ����� ����� ������������ ������� 1 ���
    void Prepare(TFldsDescr*, const AnsiString& DbName_);
    // ���������������� ������
    bool FindDbPos();
    int  SetFilter(void);
    // ����� ���� ��� ������������ ������ ����� �����������
    // ������
    bool FormWhereCond (AnsiString& SqlWhere, DS_Q& D, bool FirstAnd);
    // ��� ���� ������ ��� ����������� � ������, � �� � ���������� �������
    bool FormStrWhereCond (AnsiString& SqlWhere, bool FirstAnd);
    // �������� ��� ������ ������������ ��� ����������� � ������-����������
    AnsiString GetDescr(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmFindDbPos *FrmFindDbPos;
//---------------------------------------------------------------------------
#endif
