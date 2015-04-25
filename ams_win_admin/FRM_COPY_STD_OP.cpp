//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_COPY_STD_OP.h"
#include "MAIN_FRM.h"
#include "AMS_UTILS.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Oracle"
#pragma resource "*.dfm"
TFrmCopyStdOp *FrmCopyStdOp;
//---------------------------------------------------------------------------
__fastcall TFrmCopyStdOp::TFrmCopyStdOp(TComponent* Owner)
    : TForm(Owner), Inited(false)
{
}
//---------------------------------------------------------------------------

void __fastcall TFrmCopyStdOp::BBCancelClick(TObject *Sender)
{
    ModalResult=-1;    
}
//---------------------------------------------------------------------------
void __fastcall TFrmCopyStdOp::BBOkClick(TObject *Sender)
{
    OQuery ->SQL->Text = "declare"; 
    ModalResult = 1;    
}
//-------------------------------------------------------------------------
void __fastcall TFrmCopyStdOp::FormCreate(TObject *Sender)
{
    if (!Inited)
    {
        if (PrepareListAmsBuch(OQuery, CBSchema->Items)> 0)
        {
            CBSchema->Text = CBSchema->Items->Strings[0];
        }
        else
            CBSchema->Text = "";   
        Inited = true;    
    }    
}
//---------------------------------------------------------------------------
struct SCondItem {
    int OP_ID;
    int OP_SUBID;
    TAccName AccName;
};
//---------------------------------------------------------------------------
struct SPairs
{
    SCondItem Curr;
    SCondItem Ethalon;    
};
//---------------------------------------------------------------------------
class TPairs
{
protected:
    //std::vector<SPairs> Pairs;
public:
    // Находит пары CondItems в базе-эталоне
    // и текущей базе 
    int FindPairs(Oracle:: TOracleQuery* OQ, 
                   const AnsiString& BaseUserName_,
                   const AnsiString& EtUserName,
                   int CurrOpId,
                   int EtOpId);
    
    int AskOk();     
};

//---------------------------------------------------------------------------
int TFrmCopyStdOp::CopyStdOp(int OpId, const AnsiString& OpName)
{
    SelfOpId = OpId;
    LabQuestion->Caption= (AnsiString)"Копировать в \""+OpName+"\" код "+
       IntToStr(OpId);
    if (ShowModal()==1)
    {
        // 1. Создаем таблицу соответствий 
        //OQuery->SQL->Text = 
        //"
    }
    return 0; 
    
}