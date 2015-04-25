//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_AN_VIEW.h"
#include "FRM_BRWS_ACC_WINDOW.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AMS_TREE_VIEW"
#pragma link "AmsDbCatView"
#pragma link "AmsDBGrid"
#pragma link "AmsFooter"
#pragma link "FRM_BROWS_AN"
#pragma link "Oracle"
#pragma link "OracleData"
#pragma link "RXDBCtrl"
#pragma resource "*.dfm"
TFrmAnView *FrmAnView;
//---------------------------------------------------------------------------
__fastcall TFrmAnView::TFrmAnView(TComponent* Owner)
    : TFrmBrowsAn(Owner)
{
}
//---------------------------------------------------------------------------
void TFrmAnView::SumIdChange(bool NeedDelAnSum_,int NewSumId, const AnsiString& SetName_)
{
    if (NeedDelAnSum)
    {
        AmsQuery AQ;
        AQ.SetPLText("DeleteTmpSum(:SUM_ID);commit;");
        AQ.DeclVar("SUM_ID", AnSumEd);
        AQ.Exec("Удаляем сумму времено найденную");
    }
    NeedDelAnSum=NeedDelAnSum_;
    AnSumEd= NewSumId;
    if (SetName_!=CurrSetName)
    {
        Begin_(NeedDelAnSum, ParentF, CurrAnId, CurrAnName, SetName_,
                NewSumId, BalFld, AccCode, NeedDistr);
    }
    else
    {
        do {
            AmsQuery AQ;
            AQ.SetSqlTextReady("select count(*) from "+BaseUserName+"."+CurrAnName+
                    " where SUMID=:SUMID");
            AQ.DeclVar("SUMID", AnSumEd);
            AQ.Exec("Количество позиций в аналитике");
            TotalPos = AQ.GetFldInt(0);
            if (TotalPos)
            {
                TotalPos--;
                if (TotalPos)
                {
                    EdMode = asmSelPos;
                    WithoutCats= true;
                    if (Visible)
                        DBG->SetFocus();
                    else
                        Panel1->TabOrder=0;
                }
            }
            if (EdMode==-1)
                EdMode=asmAllBase;
        } while (0);
        //--------------------------------------------------------------
        CurrCatId = -1;
        ODS->DisableControls();
        BuildSqlQuery(ODS);
        ExecODS(ODS);
        FormatSums();
        ShowMode();
        ODS->EnableControls();
        ViewUpdated();
        if (GC::ViewDebug)
        {
            Caption = Caption + " ["+CurrDBName+"/"+CurrAnName+"]";
        }
        ShowBalSum();
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmAnView::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    if (NeedDelAnSum)
    {
        AmsQuery AQ;
        AQ.SetPLText("*.DeleteTmpSum(:SUM_ID);commit;");
        AQ.DeclVar("SUM_ID", AnSumEd);
        AQ.Exec("Удаляем сумму времено найденную");
    }
    Action=caFree;

    ((TFrmBrwsAccWindow*) ParentF)->AnViewEnd();
}
//---------------------------------------------------------------------------
void TFrmAnView::Begin_(bool NeedDelAnSum_, TForm* Parent_, int AnCode, const AnsiString& AnName,
                    const AnsiString& SetName, int AnSumEd_,
                    const AnsiString& BalFld, int AccCode_, bool NeedDistr)
{
    BegMode BM;
    NeedDelAnSum=NeedDelAnSum_;
    BM.BalFld=BalFld;
    BM.ReadOnly=true;
    BM.AccCode=AccCode_;
    BM.NeedDistr=NeedDistr;
    Begin (Parent_, AnCode, AnName, SetName, "Просмотр аналитики", AnSumEd_, 0,      BM);
    //     Parent_, AnCode, AnName, SetName,   Descr,              AnSumEd_, AnSumOst_, BM
}
//---------------------------------------------------------------------------


