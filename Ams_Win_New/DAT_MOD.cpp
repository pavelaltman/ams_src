//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "DAT_MOD.h"
#include "ams_win_c.h"
#include "FRM_GET_DB_CAT.h"
#include "amsclnt.H"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Oracle"          
#pragma link "AmsInputHistory"
#pragma link "AmsLogon"
#pragma link "OracleData"
#pragma resource "*.dfm"
TDatMod *DatMod;
TForm* TopMostForm = NULL;
//---------------------------------------------------------------------------
TOracleQuery* TDatMod::GetOQ()
{
    // Для создания многопоточного отчета
    TOracleQuery* OQ=NULL;
    if (FreeQuery.size()!=0)
    {
        OQ= FreeQuery.front();
        FreeQuery.pop_front();
    }
    else
    {
        // добавляем запросик
        OQ = new TOracleQuery (this);
        OQ->Session= OSession;
        OQ->Cursor=crSQLWait;
        AddQuery.push_back(OQ);
    }
    return OQ;
};
//---------------------------------------------------------------------------
void TDatMod::ReleaseOQ(TOracleQuery* OQ)
{
    // Для создания многопоточного отчета
    FreeQuery.push_front(OQ);
};
//---------------------------------------------------------------------------
TOracleDataSet* TDatMod::GetDS()
{
    TOracleDataSet* DS=NULL;
    if (FreeDS.size()!=0)
    {
        DS= FreeDS.front();
        FreeDS.pop_front();
    }
    else
    {
        // добавляем запросик
        DS = new TOracleDataSet (this);
        DS->Session= OSession;
        DS->Cursor=crSQLWait;
        AddDS.push_back(DS);
    }
    return DS;
}
//---------------------------------------------------------------------------
void TDatMod::ReleaseDS(TOracleDataSet* OQ)
{
    OQ->Active=false;
    FreeDS.push_front(OQ);
}
//---------------------------------------------------------------------------
__fastcall TDatMod::TDatMod(TComponent* Owner)
    : TDataModule(Owner), FrmGetDbCat(NULL)
{
    FreeQuery.push_back(OQPool1);
    FreeQuery.push_back(OQPool2);
    FreeQuery.push_back(OQPool3);
    FreeQuery.push_back(OQPool4);
    FreeQuery.push_back(OQPool5);

    FreeDS.push_back(ODS1);
    FreeDS.push_back(ODS2);
    FreeDS.push_back(ODS3);
}
//---------------------------------------------------------------------------
extern ::string def_shema;
extern void MakeRoleDepend();

bool FirstLogin = true;

int TDatMod::Login(bool ReConnect)
{
    OLogon->AmsBuchName = BaseUserName;
    if (ReConnect)
        OSession->Connected=false;
    else
        if (OSession->Connected)
            return true;
    bool Res = OLogon->AmsExecute();
    BaseUserName = OLogon->AmsBuchName.UpperCase();
    def_shema= ToString(BaseUserName);
    if (OSession->Connected)
    {

     if (UpperCase(OSession->LogonUsername)==UpperCase(OLogon->AmsBuchName)) {
       ShowMessage("Под базовым пользователем входить не положено!");
       Application->Terminate();
     }                          

     AmsWinC->SDisplay(0, OSession->LogonUsername);
     AmsQuery Q ;
     Q.SetSqlText("SELECT * FROM *.user_options WHERE username=USER") ;
     Q.Exec("получение установок пользователя") ;
     DateTime DT;
     if (!Q.NotEof())
     {
        ErrorMessage("Пользователь не зарегистрирован в бухгалтерии");
        OSession->Connected=false;
        Application->Terminate();
        return -1; 
     }
     DT=Q()->FieldAsDate(Q()->FieldIndex("BEG_DATE")) ;
     G.ur.beg_date=DT ;
     DT=Q()->FieldAsDate(Q()->FieldIndex("END_DATE")) ;
     G.ur.end_date=DT ;
     G.ur.imp_opt=Q.GetFldInt("IMP_OPT");
     G.ur.sum_len    =Q.GetFldInt("SUM_LEN");
     G.ur.sum_dig    =Q.GetFldInt("SUM_DIG");
     G.ur.apps   = Q.GetFldInt("APPS");
     G.ur.inp_date.SetNull();
     G.ur.cur_dates=Q.GetFldInt("CUR_DATES");
     G.ur.sort_num=Q.GetFldInt("SORT_NUM");
     G.ur.readonly=Q.GetFldInt("READONLY") || GC::ReadOnly;
     G.ur.set_to_end=Q.GetFldInt("SET_TO_END");
     GC::ReadOnly=G.ur.readonly;
     if (FirstLogin)
     {
        FirstLogin= false;
        if( GC::AutoRun!= "")
        {
            BeginExecReport(GC::AutoRun, NULL);
            if (GC::AutoExit)
            {
                Application->Terminate();
            }
        }
     }
     MakeRoleDepend();


//     char buf[20] ;
//     Message(DT.Out(buf,10)) ;
    }
    else
    {
        Application->Terminate();
    }
    return Res;
}
//---------------------------------------------------------------------------
void __fastcall TDatMod::DataModuleCreate(TObject *Sender)
{
    OSession->LogonUsername=GC::UserName;
    OSession->LogonDatabase=GC::ConnStr;
    OSession->LogonPassword=GC::UserPwd;
    if (!Login(true))
        Application->Terminate();
    else
    {
        ;
        AmsWinC->SDisplay(1, BaseUserName);
        AmsQuery Q;
        Q.SetSqlTextReady(
            "begin DBMS_APPLICATION_INFO.SET_CLIENT_INFO('V000.0002');"
            "select userenv('SESSIONID') into :MYSID from DUAL;"
            "commit;end;");
        Q.DeclVar("MYSID",0);
        Q.Exec("Установка версии клиента");
        G.audsid=Q.GetVarInt("MYSID");
        Q.Clear();
        Q.SetSqlTextReady(
            " select FULL_NAME from "+BaseUserName+".USER_OPTIONS "
            "where USERNAME= :USERNAME");
        Q.DeclVar("USERNAME", BaseUserName);
        Q.Exec("Полное название бухгалтерии");
        if (Q.NotEof())
        {
            AmsWinC->SDisplay(2, Q.GetFldStr(0));
            Application->Title="AMS-"+Q.GetFldStr(0)+" ["+G.ur.beg_date.val.T().DateString()+" - "+G.ur.end_date.val.T().DateString()+"]";
            AmsWinC->Caption = Application ->Title;
            G.bn.bookname=(string)(Q.GetFldStr(0).data()) ;
        }
    }
    OSession->MessageTable=BaseUserName+".message_table" ;

    
//---------------------------------------CH
#ifndef _DEBUG
    try {
      AmsQuery Q;
      Q.SetSqlTextReady("ALTER ROLLBACK SEGMENT MYROLL ONLINE");
      Q.Exec("Set Rollback segment Online");
    } catch(...) {}
#endif
//--------------------------------------EO CH
}
//---------------------------------------------------------------------------
int TDatMod::GetDbCat (int DbId, const AnsiString& DbName, int BegCat)
{
    if (!FrmGetDbCat)
    {
        Application->CreateForm(__classid(TFrmGetDbCat), &FrmGetDbCat);
    }
    return FrmGetDbCat->GetDbCat(DbId, DbName, BegCat);
};
//---------------------------------------------------------------------------

void __fastcall TDatMod::DataModuleDestroy(TObject *Sender)
{
    std::list<TOracleQuery*>::iterator i = AddQuery.begin();
    while (i != AddQuery.end())
    {
        delete *i;
        i++;
    }

    std::list<TOracleDataSet*>::iterator j = AddDS.begin();
    while (j != AddDS.end())
    {
        delete *j;
        j++;
    }
}
//---------------------------------------------------------------------------


