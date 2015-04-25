//---------------------------------------------------------------------------

#ifndef FRM_GIVE_RIGHTSH
#define FRM_GIVE_RIGHTSH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "Oracle.hpp"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>
#include <vector>
//---------------------------------------------------------------------------
const int MaxRights =6;
const int ServiceFields = 3;
class RightsMaker
{
protected:
    TStringGrid* Grid;
    TOracleQuery* OQ;
    int RightCount;
    bool Filled;

    AnsiString UserName;

    // �������� ������� ����������
    AnsiString ColsName[MaxRights];
    // ��� ��������� ������
    AnsiString ServColsName[ServiceFields];
    // ������ �����, ������� ����������
    std::vector<int> ChangedLines;
    virtual void ApplyOneRow(int  Row );
    //
    virtual void PrepareSQL()=0;
    //
    virtual int GetCount()=0;
public:
    RightsMaker ():Filled(false){};
    void Init (TOracleQuery* OQ_, TStringGrid* SG)
    {
        OQ=OQ_;
        Grid= SG;
    };
    void FillStringList (const AnsiString& UserName_);
    void Apply();
    // ��� ��������� ����������
    void Changing (TStringGrid* Gr,int ChLine);
};

class TRMFuncs:public RightsMaker
{
protected:
    virtual void ApplyOneRow(int  Row );
    virtual void PrepareSQL();
    virtual int GetCount();
public:
    TRMFuncs():RightsMaker()
    {
        RightCount = 1;
        ServColsName[0]="";
        ServColsName[1]="�������� �������";
        ServColsName[2]="�������� �������";
        ColsName[0]="����������";
    };
};
//---------------------------------------------------------------------------
class TRMDocs: public RightsMaker
{
public:
    TRMDocs():RightsMaker()
    {
        RightCount = 1;
        ServColsName[0]="OID";
        ServColsName[1]="����";
        ServColsName[2]="�������� ���������";
        ColsName[0]="��������";
    };
    virtual void ApplyOneRow(int  Row );
    virtual void PrepareSQL();
    virtual int GetCount();

};
//---------------------------------------------------------------------------
class TRMCats: public RightsMaker
{
public:
    TRMCats():RightsMaker()
    {
        RightCount = 1;
        ServColsName[0]="OID";
        ServColsName[1]="";
        ServColsName[2]="���������";
        ColsName[0]="��������";
    };
    virtual void ApplyOneRow(int  Row );
    virtual void PrepareSQL();
    virtual int GetCount();    
};
//---------------------------------------------------------------------------
class TRMFrms: public RightsMaker
{
public:
    TRMFrms():RightsMaker()
    {
        RightCount = 1;
        ServColsName[0]="OID";
        ServColsName[1]="����";
        ServColsName[2]="�������� �����";
        ColsName[0]="��������";
    };
    virtual void ApplyOneRow(int  Row );
    virtual void PrepareSQL();
    virtual int GetCount();    
};
//---------------------------------------------------------------------------
class TRMOpers: public RightsMaker
{
public:
    TRMOpers():RightsMaker()
    {
        RightCount = 4;
        ServColsName[0]="OID";
        ServColsName[1]="";
        ServColsName[2]="�������� ��������";
        ColsName[0]="��������";
        ColsName[1]="���������";
        ColsName[2]="��������";
        ColsName[3]="��������";
    };
    virtual void ApplyOneRow(int  Row );
    virtual void PrepareSQL();
    virtual int GetCount();    
};
//---------------------------------------------------------------------------
class TRMBds: public RightsMaker
{
public:
    TRMBds():RightsMaker()
    {
        RightCount = 4;
        ServColsName[0]="OID";
        ServColsName[1]="����";
        ServColsName[2]="�������� ���� ������";
        ColsName[0]="��������";
        ColsName[1]="���������";
        ColsName[2]="��������";
        ColsName[3]="��������";
    };
    virtual void ApplyOneRow(int  Row );
    virtual void PrepareSQL();
    virtual int GetCount();    
};
//---------------------------------------------------------------------------
class TRMAns: public RightsMaker
{
public:
    TRMAns():RightsMaker()
    {
        RightCount = 4;
        ServColsName[0]="OID";
        ServColsName[1]="����";
        ServColsName[2]="�������� ���������";
        ColsName[0]="��������";
        ColsName[1]="���������";
        ColsName[2]="��������";
        ColsName[3]="��������";
    };
    virtual void ApplyOneRow(int  Row );
    virtual void PrepareSQL();
    virtual int GetCount();    
};
//---------------------------------------------------------------------------
class TRMAccs: public RightsMaker
{
public:
    TRMAccs():RightsMaker()
    {
        RightCount = 2;
        ServColsName[0]="CODE";
        ServColsName[1]="���� ���";
        ServColsName[2]="�������� �����";
        ColsName[0]="��������";
        ColsName[1]="���������";
    };
    virtual void ApplyOneRow(int  Row );
    virtual void PrepareSQL();
    virtual int GetCount();    
};
//---------------------------------------------------------------------------
const int ObjCol = 8;
//---------------------------------------------------------------------------
class TFrmGiveRights : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TComboBox *CBUserName;
    TCheckBox *ChBUsers;
    TCheckBox *ChBRoles;
    TOracleQuery *OQ;
    TPanel *Panel2;
    TBitBtn *BBCancel;
    TBitBtn *BBOk;
    TPageControl *PC;
    TTabSheet *TSCat;
    TTabSheet *TSAcc;
    TTabSheet *TSDoc;
    TTabSheet *TSFrm;
    TTabSheet *TSBd;
    TTabSheet *TSAn;
    TStringGrid *SGCat;
    TStringGrid *SGDoc;
    TStringGrid *SGAcc;
    TStringGrid *SGFrm;
    TStringGrid *SGBd;
    TStringGrid *SGAn;
    TTabSheet *TSOper;
    TStringGrid *SGOpers;
    TTabSheet *TSFuncs;
    TStringGrid *SGFuncs;
    void __fastcall BBCancelClick(TObject *Sender);
    void __fastcall BBOkClick(TObject *Sender);
    void __fastcall SGCatDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall SGCatDblClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall CBUserNameChange(TObject *Sender);
    void __fastcall ChBUsersClick(TObject *Sender);
    void __fastcall PCChange(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall SGFuncsDblClick(TObject *Sender);
    void __fastcall SGFuncsDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
private:	// User declarations
    Graphics::TBitmap* B1;
    Graphics::TBitmap* B2;
    Graphics::TBitmap* B3;
    Graphics::TBitmap* B4;
public:		// User declarations
    __fastcall TFrmGiveRights(TComponent* Owner);
protected:
    // �������� �� �-�
    bool Dirty;
    // ��� ������������� ������ ��� ������������
    AnsiString GrName;
    // True - ������, false - ������������
    bool Group;
    // �������� �� ��� �������
    bool Loaded [ObjCol];
    RightsMaker* RM[ObjCol];
    //
    TRMCats     RCats;
    TRMAccs     RAccs;
    TRMDocs     RDocs;
    TRMFrms     RFrms;
    TRMBds      RBds;
    TRMAns      RAns;
    TRMOpers    ROpers;
    TRMFuncs    RFuncs;


};
//---------------------------------------------------------------------------
extern PACKAGE TFrmGiveRights *FrmGiveRights;
//---------------------------------------------------------------------------
#endif
