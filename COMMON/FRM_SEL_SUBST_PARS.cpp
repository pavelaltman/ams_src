//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_SEL_SUBST_PARS.h"
#include "DAT_REP_BLDR.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "OracleData"
#pragma link "OracleNavigator"
#pragma resource "*.dfm"
TFrmSelSubstPars *FrmSelSubstPars;
//---------------------------------------------------------------------------
__fastcall TFrmSelSubstPars::TFrmSelSubstPars(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TFrmSelSubstPars::SelSubstPars(int RepId, 
                                    const AnsiString& SqlQ, 
                                    const AnsiString& GroupName,
                                    bool MultiSel,
                                    AnsiString& Result)
{
    ODS->Active= false;
    ODS->SQL->Text=ReplaceBaseUser(SqlQ);
    DBG->Columns->Clear();
    if (MultiSel)
        DBG->Options<<dgMultiSelect;
    else
        DBG->Options>>dgMultiSelect;
    ODS->Active=true;

    for (int i=0; i<ODS->FieldCount;i++)
    {
        AnsiString S = ODS->Fields->Fields[i]->FieldName;

        if (S[1]=='D')
        {
            TColumn* Col=DBG->Columns->Add();
            Col->Field = ODS->Fields->Fields[i];
            Col->DisplayName = S.SubString(3, S.Length()-3);
        }
    }
    bool NeedSel = true;
    while (NeedSel)
    {
        ShowModal();
        std::vector<AnsiString> FldNames;
        std::vector<int> FldTypes;
        if (DBG->SelectedRows->Count > 0)
        {
            NeedSel=false;
            TOracleQuery* OQ = DatMod->GetOQ();
            try
            {
                OQ->SQL->Text= "select GR_FLD_NAME, PAR_TYPE from "+BaseUserName+
                    ".REP_PARS where REP_ID = :REP_ID and GR_NAME = :GR_NAME";
                OQ->DeleteVariables();
                OQ->DeclareVariable("REP_ID", otInteger);
                OQ->DeclareVariable("GR_NAME", otString);
                OQ->SetVariable("REP_ID", RepId);
                OQ->SetVariable("GR_NAME", GroupName);
                ExecSQL(OQ, "Поля группы параметров");
                while (!OQ->Eof)
                {
                    FldNames.push_back(OQ->FieldAsString(0));
                    FldTypes.push_back(OQ->FieldAsInteger(1));
                    OQ->Next();
                }    
            }
            catch (...)
            {
                DatMod->ReleaseOQ(OQ);
                throw;
            }
            DatMod->ReleaseOQ(OQ);

            Result +="(";
            TDataSet *pDS = DBG->DataSource->DataSet;
            bool ManyFields = FldNames.size()>1;
            for (int i=0; i < DBG->SelectedRows->Count; i++)
            {
                if (i>0) 
                  Result+=", ";
                if (ManyFields)
                  Result+=" (";
                pDS->GotoBookmark((void *)DBG->SelectedRows->Items[i].c_str());
                for (unsigned int j = 0; j < FldNames.size(); j++)
                {
                    if (j>0)
                        Result+=", ";
                    switch (FldTypes[j])
                    {
                        case 0:
                            Result+="'"+pDS->FieldByName(FldNames[j])->AsString+"'";
                        break;    
                        case 1:
                            Result+=IntToStr(pDS->FieldByName(FldNames[j])->AsInteger);
                        break;    
                        case 2:
                            Result+=FloatToStr(pDS->FieldByName(FldNames[j])->AsFloat);
                        break;    
                        case 3:
                            Result+="'"+pDS->FieldByName(FldNames[j])->AsString+"'";
                        break;    
                    }    
                }
                if (ManyFields)
                  Result+=") ";
            }
            Result+=") ";
        } //
    }// While not selected                
}
//---------------------------------------------------------------------------

void __fastcall TFrmSelSubstPars::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    ModalResult = 1;
    Action = caHide;    
}
//---------------------------------------------------------------------------

