//---------------------------------------------------------------------------

#ifndef copy_cond_iTEMSH
#define copy_cond_iTEMSH

#include "AMS_UTILS.h"
//---------------------------------------------------------------------------
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
class TFrmCopyStdOp;
//---------------------------------------------------------------------------
class TPairs
{
protected:
    //std::vector<SPairs> Pairs;
    int MyOpId;
    int EtOpId;
    AnsiString EtUserName;
public:
    // ������� ���� CondItems � ����-�������
    // � ������� ���� 
    int FindPairs(Oracle:: TOracleQuery* OQ, 
                   const AnsiString& EtUserName_,
                   int CurrOpId,
                   int EtOpId);
                   
    // ������ ��� ������� �����������
    int AskOk(Oracle:: TOracleQuery* OQ, TFrmCopyStdOp* Frm);
    int Copy(Oracle:: TOracleQuery* OQ);
};
//---------------------------------------------------------------------------
#endif
 