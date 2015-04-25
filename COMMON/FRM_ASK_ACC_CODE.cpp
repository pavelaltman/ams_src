//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_ASK_ACC_CODE.h"
//#include "DAT_REP_BLDR.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AMS_TREE_VIEW"
#pragma link "AmsAccsView"
#pragma link "Oracle"
#pragma resource "*.dfm"
TFrmAskAccCode *FrmAskAccCode;
//---------------------------------------------------------------------------
__fastcall TFrmAskAccCode::TFrmAskAccCode(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmAskAccCode::BBCancelClick(TObject *Sender)
{
    ModalResult=-1;
}
//---------------------------------------------------------------------------
void __fastcall TFrmAskAccCode::BBOkClick(TObject *Sender)
{
    ModalResult = 1;
}
//-------------------------------------------------------------------------
int TFrmAskAccCode::AskAccCode(const AnsiString& BaseUser, int BegCode, int FirstNode)
{
    int Res=0;
    AccsView->Begin(BaseUser, BegCode);
    if (FirstNode)
    {
        AccsView->MakeActive(FirstNode);
    }
    if (ShowModal()==1)
    {
        TAmsTreeData* F = (TAmsTreeData*)AccsView->Selected->Data;
        Res = F->GetCode();
    };
    return Res;
};
//-------------------------------------------------------------------------
int TFrmAskAccCode::AskAccCode(const AnsiString& BaseUser,
                        const AnsiString& BegCode, int FirstNode)
{
    int Res=0;
    AccsView->Begin(BaseUser, TAccName::ToSysName(BegCode));
    if (FirstNode)
    {
        AccsView->MakeActive(FirstNode);
    }
    if (ShowModal()==1)
    {
        TAmsTreeData* F = (TAmsTreeData*)AccsView->Selected->Data;
        Res = F->GetCode();
    };
    return Res;
};
//-------------------------------------------------------------------------
AnsiString TFrmAskAccCode::AskAccName(const AnsiString& BaseUser, int BegCode, int FirstNode)
{
    int Res = AskAccCode(BaseUser, BegCode, FirstNode);
    AnsiString ResStr="";
    if (Res)
    {
        AmsQuery AQ;
        AQ.SetSqlTextReady("select NAME from "+BaseUser+".ACCS where CODE = :CODE");
        AQ.DeclVar("CODE", Res);
        AQ.Exec("");
        if (!AQ()->Eof)
        {
            ResStr= AQ()->FieldAsString(0);
        }
    }
    return ResStr;
};
//-------------------------------------------------------------------------
AnsiString TFrmAskAccCode::AskAccName(const AnsiString& BaseUser,
            const AnsiString& BegCode, int FirstNode)
{
    AnsiString NBegCode = TAccName::ToSysName(BegCode);
    int Res = AskAccCode(BaseUser, NBegCode, FirstNode);
    AnsiString ResStr="";
    if (Res)
    {
        AmsQuery AQ;
        AQ.SetSqlTextReady("select NAME from "+BaseUser+".ACCS where CODE = :CODE");
        AQ.DeclVar("CODE", Res);
        AQ.Exec("");
        if (!AQ()->Eof)
        {
            ResStr= AQ()->FieldAsString(0);
        }
    }
    return ResStr;
};
//-------------------------------------------------------------------------
void __fastcall TFrmAskAccCode::EdAccCodeChange(TObject *Sender)
{
    int i = 0;
    while (i< AccsView->Items->Count &&
        AccsView->Items->Item[i]->Text.Pos(EdAccCode->Text)!=1) i++;
    if (i< AccsView->Items->Count)
    {
        AccsView->Selected = AccsView->Items->Item[i];
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmAskAccCode::AccsViewDblClick(TObject *Sender)
{
    MakeSelection();
}
//---------------------------------------------------------------------------
void TFrmAskAccCode::MakeSelection()
{
    TTreeNode* N= AccsView->Selected;
    if (N)
    {
        if (N->HasChildren)
        {
            N->Expanded= !N->Expanded;
            if (N->Expanded)
                AccsView->Selected=N->Item[0];
        }
        else
        {
            BBOkClick(this);
        }
    }
};
//---------------------------------------------------------------------------
bool __fastcall TFrmAskAccCode::WantChildKey(Controls::TControl* Child,
                            Messages::TMessage &Message)
{
    bool Res=false;
    if (Child == AccsView)
    {
        if (Message.Msg==WM_KEYDOWN )
        {
            TWMKey&  KeyMsg = *((TWMKey*)&Message);
            if (KeyMsg.CharCode==13 ||KeyMsg.CharCode==32 || KeyMsg.CharCode==VK_F2)
            {
                MakeSelection();
                Res= true;
            }
        }
        if (Message.Msg==WM_KEYUP )
        {
            TWMKey&  KeyMsg = *((TWMKey*)&Message);
            if (KeyMsg.CharCode==13 ||KeyMsg.CharCode==32 || KeyMsg.CharCode==VK_F2)
            {
                Res= true;
            }
        }
    }
    return Res;
}
