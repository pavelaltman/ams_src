//---------------------------------------------------------------------------

#ifndef FRM_ACC_LISTH
#define FRM_ACC_LISTH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AMS_TREE_VIEW.h"
#include "AmsAccsView.h"
#include "Oracle.hpp"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ActnList.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------
class TFrmChooseForm;
class TFrmBrowsAn;
//---------------------------------------------------------------------------
class TFrmAccList : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TEdit *EdAccCode;
        TAmsAccsView *AccsView;
        TOracleQuery *OracleQuery1;
        TToolBar *ToolBar1;
        TToolButton *ToolButton1;
        TToolButton *ToolButton2;
        TToolButton *ToolButton3;
        TActionList *ActionList1;
        TAction *ActBegRems;
        TAction *ActUstAnal;
        TAction *ActOtlOper;
        TPopupMenu *PopupMenu2;
        TMenuItem *N2;
        TMenuItem *ActUstAnal1;
        TMenuItem *N3;
    TLabel *Label1;
    TToolButton *ToolButton4;
    TAction *ActExecForm;
    TToolButton *ToolButton5;
    TAction *ActShowRems;
    TToolButton *ToolButton6;
    TAction *ActAccView;
    TMenuItem *N1;
    TButton *Button1;
    TAction *ActRecalcLev;
    TMenuItem *N4;
    TMenuItem *N5;
    TButton *Button2;
    TToolButton *ToolButton7;
    TAction *ActRefresh;
        TToolButton *ToolButton8;
        TToolButton *ToolButton9;
        TToolButton *ToolButton10;
        TAction *ActExpandAll;
        TAction *ActCollapseAll;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall EdAccCodeChange(TObject *Sender);
    void __fastcall ActExecFormExecute(TObject *Sender);
    void __fastcall ActShowRemsExecute(TObject *Sender);
    void __fastcall ActAccViewExecute(TObject *Sender);
    void __fastcall AccsViewDblClick(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall ActOtlOperExecute(TObject *Sender);
    void __fastcall ActUstAnalExecute(TObject *Sender);
    void __fastcall ActBegRemsExecute(TObject *Sender);
    void __fastcall ActRecalcLevExecute(TObject *Sender);
    void __fastcall AccsViewKeyPress(TObject *Sender, char &Key);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall ActRefreshExecute(TObject *Sender);
        void __fastcall ActExpandAllExecute(TObject *Sender);
        void __fastcall ActCollapseAllExecute(TObject *Sender);
protected:	// User declarations
    struct SApplyEditRem
    {
        int AccCode;
        int SumId;
    };
    std::map<TFrmBrowsAn*, SApplyEditRem> WaitForApply;
public:		// User declarations
        TFrmChooseForm* ChooseForm;
        __fastcall TFrmAccList(TComponent* Owner);
    void AnRemInputOk(TFrmBrowsAn* Frm);
    void AnRemInputBad(TFrmBrowsAn* Frm);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmAccList *FrmAccList;
//---------------------------------------------------------------------------
#endif
