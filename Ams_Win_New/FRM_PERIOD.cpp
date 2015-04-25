//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "amsclnt.h"

#include "FRM_PERIOD.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "OracleData"
#pragma link "RXDBCtrl"
#pragma resource "*.dfm"
TFrmPeriod *FrmPeriod;
//---------------------------------------------------------------------------
__fastcall TFrmPeriod::TFrmPeriod(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void TFrmPeriod::SetInitPeriod(TDateTime beg,TDateTime end)
{
 DT_beg=beg ; DT_end=end ;
 Variant Val= DT_beg,
            Val1=DT_end ;
 TSearchRecordOptions SetSR;

 DSet->SQL->Text="select * from "+BaseUserName+".cps "
                 "where point_date not in (select max(point_date) "
                 "   from "+BaseUserName+".cps) "
                 "   ORDER BY point_date" ;

 DSet1->SQL->Text="select end_date from "+BaseUserName+".cints "
                  "  where beg_date=:point_date " ;

 DSet->Active = true;
 DSet1->Active = true;

 DSet->SearchRecord("POINT_DATE", Val, SetSR);
 DSet1->SearchRecord("END_DATE", Val1, SetSR);
}


void __fastcall TFrmPeriod::FormCreate(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------

void __fastcall TFrmPeriod::BBCancelClick(TObject *Sender)
{
    ModalResult=-1;
}
//---------------------------------------------------------------------------
void __fastcall TFrmPeriod::BBOkClick(TObject *Sender)
{
    ModalResult = 1;
}
//-----------------------------------------------------------------------
int TFrmPeriod::Period(TDateTime& BegP, TDateTime& EndP)
{
    SetInitPeriod(BegP, EndP);
    int Res = 0;
    if (ShowModal()>0)
    {
        BegP=DSet->FieldByName("POINT_DATE")->AsDateTime;
        EndP=DSet1->Fields->Fields[0]->AsDateTime;
        Res = 1;
    }
    return Res;
}
//-----------------------------------------------------------------------

