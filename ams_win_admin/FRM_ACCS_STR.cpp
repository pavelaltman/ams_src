//---------------------------------------------------------------------------
#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_ACCS_STR.h"
#include "MAIN_FRM.h"
#include "FRM_ACCS.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Oracle"
#pragma resource "*.dfm"
//TFrmAccsStr *FrmAccsStr;
//---------------------------------------------------------------------------
__fastcall TFrmAccsStr::TFrmAccsStr(TComponent* Owner)
  : TForm(Owner), Ready (false), AccNewEvent (NULL) 
{
}
//---------------------------------------------------------------------------
void TFrmAccsStr::Clear ()
{
  TV->Items->Clear();
    
}
//---------------------------------------------------------------------------
void __fastcall TFrmAccsStr::SelAccsClick(TObject *Sender)
{
  Ready=false;

    if (BeginAcc == "")
    {
        Q->SQL->Text="select CODE, NAME, DESCR, LEVEL from "+Schema+".ACCS " 
         " start with PARENT is NULL connect by PRIOR CODE = PARENT "
         " order  by NAME ";
        Q->DeleteVariables();
    }
    else
    {
        Q->SQL->Text="select CODE, NAME, DESCR, LEVEL from "+Schema+".ACCS " 
         " start with NAME= :BEGIN_ACC connect by PRIOR CODE = PARENT "
         " order  by NAME ";
        Q->DeleteVariables();
        Q->DeclareVariable("BEGIN_ACC", otString);
        Q->SetVariable("BEGIN_ACC", BeginAcc); 
    };

  ExecSQL(Q, "—чета из ACCS");
  TTreeNode* TN= TV->Items->Add(NULL,TAccName::ToUserName(
                                Q->FieldAsString(1))
                                    +" "+Q->Field("DESCR") );
  int CODE = Q->Field("CODE");
  TN->Data = (void*) CODE;
  Q->Next();
  while (!Q->Eof)
  {
    int LEV = Q->Field("LEVEL");
    TTreeNode* NN = TN;
    while ((NN!=NULL) &&(LEV <= NN->Level))
     NN= NN->Parent;
    if (NN)
        TN= TV->Items->AddChild(NN,TAccName::ToUserName(Q->FieldAsString(1))
                                    +" "+Q->Field("DESCR") );
    else
        TN= TV->Items->Add(NULL,TAccName::ToUserName(Q->FieldAsString(1))
                                    +" "+Q->Field("DESCR") );
        
    int CODE = Q->Field("CODE");
    TN->Data = (void*) CODE;
    Q->Next();
  };
  Ready = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmAccsStr::TVChange(TObject *Sender, TTreeNode *Node)
{
 if (Ready && AccNewEvent)
 {
   int i = (int) Node->Data;
   AccNewEvent(this, TAccName(i, Node->Text));
 };
}
//---------------------------------------------------------------------------
void __fastcall TFrmAccsStr::ToolButton2Click(TObject *Sender)
{
//
 //FrmAccs->Show();
}
//---------------------------------------------------------------------------
void TFrmAccsStr::Begin(const AnsiString& SchemaName, 
                        const AnsiString& BeginAcc, 
                        TAccNewEvent AccNewEv )
{
    Schema = SchemaName; 
    AccNewEvent = AccNewEv;
    
    SelAccsClick(this); 
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

