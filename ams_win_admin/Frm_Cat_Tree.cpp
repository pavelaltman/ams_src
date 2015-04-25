//---------------------------------------------------------------------------
#include "AMS_HDR.h"
#pragma hdrstop

#include "Frm_Cat_Tree.h"
#include "MAIN_FRM.h"
#include "AMS_UTILS.h"
#include <fstream>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Oracle"
#pragma resource "*.dfm"
TFrmCatTree *FrmCatTree;
//---------------------------------------------------------------------------
__fastcall TFrmCatTree::TFrmCatTree(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmCatTree::TVChange(TObject *Sender, TTreeNode *Node)
{
 int i = (int) Node->Data;
 if (CatNewEvent)
    CatNewEvent(this, i);
 //MainFrm->QObjs->SetVariable("CATID", i);
 //MainFrm->QObjs->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TFrmCatTree::SelObjCatClick(TObject *Sender)
{
  try {
  Mem.clear();
  TV->Items->Clear();
  Q->Execute();
  TTreeNode* TN= TV->Items->Add(NULL, Q->Field("DESCR"));
  int OWNCAT = Q->Field("OWNCAT");
  Mem[OWNCAT]=TN;
  TN->Data = (void*) OWNCAT;
  Q->Next();
  while (!Q->Eof)
  {
    int CATID=Q->Field("CATID");
    int OWNCAT = Q->Field("OWNCAT");
    TN=Mem[CATID];
    TN= TV->Items->AddChild(TN,Q->Field("DESCR"));
    TN->Data = (void*) OWNCAT;
    Mem[OWNCAT]=TN;
    Q->Next();
  }
  }
  catch (...)
  {
    std::ofstream F ("err.sql");
    F<<Q->SQL->Text.c_str();
    throw;
  }
}
//---------------------------------------------------------------------------
void TFrmCatTree::ShowCategories (const AnsiString& BaseUserName_, 
                                int OType, TCatNewEvent CatNewEv)
{
    CatNewEvent= CatNewEv;
    //Q->Active = false;
    Caption = "Категории объектов из "+BaseUserName_; 
    if (OType ==-1)
    {
      Q->SQL->Text= "select * from "+BaseUserName_+
       ".CATTREE order by TORD ";
    }
    else
    {
      Q->SQL->Text= "select * from "+BaseUserName_+
       ".CATTREE order by TORD ";
      /*
      Q->SQL->Text= "select * from "+BaseUserName_+
       ".CATTREE CT where exists ( select 1 from "+BaseUserName_+
        ".OBJS O where CT.OWNCAT=O.CATID and O.OTYPE = "+IntToStr(OType)+
        ") or CT.CATID is NULL order by TORD ";
      */
    };
    Width = 250;
    //Height = 200;
    SelObjCatClick(this);        
};
//---------------------------------------------------------------------------

