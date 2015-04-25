//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_BUILD_BUSY_PROG.h"
#include "MAIN_FRM.h"
#include "PRINT_ERROR.h"  
#include "AMS_UTILS.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Oracle"
#pragma resource "*.dfm"
TFrmBuildBusyProg *FrmBuildBusyProg;
//---------------------------------------------------------------------------
__fastcall TFrmBuildBusyProg::TFrmBuildBusyProg(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFrmBuildBusyProg::FormShow(TObject *Sender)
{
    //
    DBQ->SQL->Text = " select NAME, OID from OBJS " 
                     " where OTYPE = 4 order by ORD ";
    DBQ->Execute();
    MemSql->Lines->Clear();
    AnsiString S = "create or replace function\n IsBusyPos (pDbId number, "
       " pArticle varchar2, pVer number ) return NUMBER as\n";
    S+="    vRes number;\n"
       "    vReq number;\n";
    S+="begin\n"
       "    vReq:=0;\n";
       
    while (!DBQ->Eof)
    {
        S+="    if pDbId = "+IntToStr(DBQ->FieldAsInteger(1))+" then \n";
        
        AnQ->SetVariable("POID", DBQ->FieldAsInteger(1));
        AnQ->Execute();
        while(!AnQ->Eof)
        {
            S+= "       select max(1) into vReq from "+AnQ->FieldAsString(0)+
                " where AARTICLE = pArticle and AVER = pVer and ROWNUM < 2;\n"
                "       if vReq=1 then\n"
                "           return 1;\n"
                "       end if;\n";     
            AnQ->Next();                  
        }
        ODbLinks->SetVariable("DB_ID", DBQ->FieldAsInteger(1));
        ODbLinks->Execute();
        while (!ODbLinks->Eof)
        {
            S+="    select max(1) into vReq from " +ODbLinks->FieldAsString(1)+
                " where "+ODbLinks->FieldAsString(0)+"= pArticle and "+
                ODbLinks->FieldAsString(0)+"_V = pVer and ROWNUM < 2;\n"
                "       if vReq=1 then\n"
                "           return 1;\n"
                "       end if;\n";     
            ODbLinks->Next();
        }
        S+= "       return 0;\n"
            "   end if;\n";
        DBQ->Next();             
    };

    S+= "   RAISE_APPLICATION_ERROR(-20001,  'Для базы не построена функция свободного члена');\n"
        "end;\n";
    MemSql->Lines->Text =S;    
}
//---------------------------------------------------------------------------
void __fastcall TFrmBuildBusyProg::BBApplyClick(TObject *Sender)
{
//
    DBQ->SQL->Text = MemSql->Lines->Text;
    DBQ->Execute();
    DBQ->SQL->Text = "commit";
    DBQ->Execute();
    DBQ->SQL->Text = "grant execute on IsBusyPos to "+BaseUserName+"ROLE";
    DBQ->Execute();
    OkMessage("Функция создана успешно");
}
//---------------------------------------------------------------------------
void __fastcall TFrmBuildBusyProg::BBCancelClick(TObject *Sender)
{
    Release();    
}
//---------------------------------------------------------------------------
void __fastcall TFrmBuildBusyProg::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Action = caFree;    
}
//---------------------------------------------------------------------------
