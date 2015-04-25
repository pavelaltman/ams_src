//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_SET_AN_FLD_ORDER.h"
#include "MAIN_FRM.h"
#include "AMS_QUERY.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Oracle"
#pragma link "AmsStringGrid"
#pragma resource "*.dfm"
TFrmSetAnFldOrder *FrmSetAnFldOrder;
//---------------------------------------------------------------------------
__fastcall TFrmSetAnFldOrder::TFrmSetAnFldOrder(TComponent* Owner)
    : TForm(Owner), Dirty (0)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmSetAnFldOrder::BBApplyClick(TObject *Sender)
{
//
    if (Dirty)
    {
        AmsQuery AQ(OQ);
        AQ.Clear();
        AQ.SetSqlText("delete from *.AN_FLD_ORDER where ANID=:ANID and SETNAME=:SETNAME");
        AQ.DeclVar("ANID", AnalId);
        AQ.DeclVar("SETNAME", CurrSetName);
        AQ.Exec("Удаляем если есть уже");
        AQ.DeclVarStr("NAME");
        AQ.DeclVarStr("REALNAME");
        AQ.DeclVarInt("DB_ID");
        AQ.DeclVarStr("FLD_TYPE");
        AQ.DeclVarInt("FLD_ORD");
        AQ.SetSqlText("insert into *.AN_FLD_ORDER (ANID, SETNAME, REALNAME, NAME, DB_ID, FLD_TYPE, FLD_ORD) "
         "values (:ANID, :SETNAME, :REALNAME, :NAME, :DB_ID, :FLD_TYPE, :FLD_ORD)");

        for (int i = 1; i< SG->RowCount; i++)
        {
            AQ.SetVar("FLD_ORD", i);
            int SS = StrToInt(SG->Cells[0][i])-1;
            SFldOpt& FO = FldsDescr[SS];

            AQ.SetVar("REALNAME", FO.RealName);
            AQ.SetVar("NAME", FO.SqlAlias);
            if (FO.FldAddInfo== aiDb)
                AQ.SetVar("DB_ID", DbId);
            else
                AQ.SetVar("DB_ID", AnalId);
            AnsiString S;
            switch(FO.FldAddInfo)
            {
                case aiDb:
                    S="DB";
                break;
                case aiAn:
                    S="AN";
                break;
                case aiAnR:
                    S="ANR";
                break;
                case aiAdd:
                    S="ADD";
                break;
            }
            AQ.SetVar("FLD_TYPE",S);
            AQ.Exec("");
        }
        AQ.Commit();
    }
    Dirty= false;        
}
//---------------------------------------------------------------------------
void TFrmSetAnFldOrder::SetAnFldOrder(int AnId_, const AnsiString& AnName_,
                    const AnsiString& AnDescr_)
{
    AnalId = AnId_;
    AnalName=AnName_;
    AnalDescr= AnDescr_;

    Caption = "Расставляем порядок полей для аналитики ["+AnalName+"] "+AnalDescr;

    CurrSetId = -1;
    Dirty = 0;

    // Находим имя и ID базы данных
    OQ->Clear();
    OQ->SQL->Text="select NAME, OID from "+BaseUserName+".OBJS where "
            "OID=(select PARENT from "+BaseUserName+".OBJS where OID=:OID)";
    OQ->DeclareVariable("OID", otInteger);
    OQ->SetVariable("OID",AnalId);
    ExecSQL(OQ,"Имя родительской БД");
    if (!OQ->Eof)
    {
        DbId= OQ->FieldAsInteger(1);
        DbName= OQ->FieldAsString(0);
    }


    // Считываем наборы установок для данной аналитики
    CBSetName->Items->Clear();
    OQ->Clear();
    OQ->SQL->Text = "select SETNAME from "+BaseUserName+".DBOPT where DBID=:DBID";
    OQ->DeclareVariable("DBID",otInteger);
    OQ->SetVariable("DBID", AnalId);
    CBSetName->Items->Add("Пустой набор");
    FillStr(CBSetName->Items, OQ);
    CBSetName->ItemIndex = 0;
    CBSetName->Text = "Пустой набор";
    SetChanged();
    ShowModal();
}
//---------------------------------------------------------------------------
void TFrmSetAnFldOrder::SetChanged()
{
    //TODO: Add your source code here
    if (CurrSetId != CBSetName->ItemIndex)
    {
        if (Dirty)
        {
            if (AskMessage("Вы внесли изменения в набор установок "+
                CurrSetName+" и не сохранили их.\r\nЖелаете продолжть без сохранения?")!=ID_YES)
            {
                CBSetName->Text=CurrSetName;
                CBSetName->ItemIndex = CurrSetId;
                return;
            }
        }

        CurrSetId= CBSetName->ItemIndex;
        if (CurrSetId ==0)
            CurrSetName = " ";
        else
            CurrSetName = CBSetName->Text;
        FldsDescr.SetAnDescr(DbName, DbId, AnalName, AnalId, CurrSetName);
        FldsDescr.ReadFldsDescr();
        SG->RowCount=FldsDescr.Count()+1;

        for (int i = 0 ; i< FldsDescr.Count();i++)
        {
            SFldOpt& FO = FldsDescr[i];
            SG->Cells[0][i+1]=IntToStr(i+1);
            SG->Cells[1][i+1]=FO.RealName;
            SG->Cells[2][i+1]=FO.Alias;
            AnsiString S;
            switch (FO.FldAddInfo)
            {
                case aiDb:
                    S="Поле БД";
                break;
                case aiAn:
                    S="Поле аналитики";
                break;
                case aiAnR:
                    S="Остаток";
                break;
                case aiAdd:
                    S="Доп поле";
                break;
            }
            SG->Cells[3][i+1]=S;
        }
    }
}
//---------------------------------------------------------------------------
void TFrmSetAnFldOrder::FillFieldDescr()
{

//

}
//---------------------------------------------------------------------------
void __fastcall TFrmSetAnFldOrder::BBOkClick(TObject *Sender)
{
    BBApplyClick(this);
    ModalResult=1;
}
//---------------------------------------------------------------------------

void __fastcall TFrmSetAnFldOrder::BitBtn1Click(TObject *Sender)
{
    ModalResult=-1;
}
//---------------------------------------------------------------------------

void __fastcall TFrmSetAnFldOrder::CBSetNameChange(TObject *Sender)
{
//
    SetChanged();
}
//---------------------------------------------------------------------------

void __fastcall TFrmSetAnFldOrder::SGRowMoved(TObject *Sender,
      int FromIndex, int ToIndex)
{
    //
    Dirty= 1;    
}
//---------------------------------------------------------------------------

