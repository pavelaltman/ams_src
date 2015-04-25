//---------------------------------------------------------------------------

#include "AMS_HDR.H"
#pragma hdrstop

#include "FRM_FIND_DB_POS.h"
#include "DAT_MOD.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Oracle"
#pragma link "AmsInput"
#pragma link "AmsInputDate"
#pragma link "AmsInputInt"
#pragma link "AmsInputStr"
#pragma link "AmsInputSum"
#pragma resource "*.dfm"
TFrmFindDbPos *FrmFindDbPos;
//---------------------------------------------------------------------------
__fastcall TFrmFindDbPos::TFrmFindDbPos(TComponent* Owner)
    : TForm(Owner)
{
    ActInput=NULL;
    SqlFormed = false;
    CurrInd = -1;
    Dirty = false;
    AIStr->AmsPromptMaxLen=0;
    AIDate->AmsPromptMaxLen=0;
    AIInt->AmsPromptMaxLen=0;
    AISum->AmsPromptMaxLen=0;
    AIStr->AmsPromptLen=0;
    AIDate->AmsPromptLen=0;
    AIInt->AmsPromptLen=0;
    AISum->AmsPromptLen=0;
    AIStr->AmsEnabled = false;
    AIDate->AmsEnabled = false;
    AIInt->AmsEnabled = false;
    AISum->AmsEnabled = false;
}
//---------------------------------------------------------------------------
bool TFrmFindDbPos::FindDbPos(bool FindNext, int& ResCatId,
                    AnsiString& ResArt, int& ResVer)
{
    //
    bool Res = false;
    if (FindNext)
    {
        if (SqlFormed)
        {
            if (!OQ->Eof)
            {
                ResCatId = OQ->FieldAsInteger(0);
                ResArt  = OQ->FieldAsString(1);
                ResVer  = OQ->FieldAsInteger(2);
                OQ->Next();
                Res = true; 
            }
            else
            {
                if (AskMessage("Поиск завершен. Начинать заново?")== ID_YES)
                {
                    ExecSQL(OQ,"Поиск позиции БД");
                    Res = FindDbPos(true,ResCatId,ResArt,ResVer); 
                }
            }
        }    
    }
    else
    {
        if (ShowModal()==1)
        {
            FormSqlQuery();
            ExecSQL(OQ, "Поиск позиции БД");
            Res = FindDbPos(true,ResCatId,ResArt,ResVer); 
        }    
            
    }
    return Res;
}
//---------------------------------------------------------------------------
void TFrmFindDbPos::Prepare(TFldsDescr* FldsDescr_, const AnsiString& DbName_)
{
    DbName = DbName_;
    FldsDescr = FldsDescr_;
    SSearchDescr SD;
    LBFieldsNames->Items->Clear();
    WhatLookingFor.clear();
    for (int i = 0 ; i< FldsDescr->Count(); i++)
    {
        if ( !((*FldsDescr)[i].DefOpt & Hidden) && !((*FldsDescr)[i].DispOpt & Hidden))
        {
            SD.FldNo=i;
            SD.Type=(*FldsDescr)[i].Type;
            if (SD.Type==aftStr)
            {
                SD.EqSign = 3;
                SD.CmpWhat="%";
            }
            else
            {
                SD.EqSign = 0;
                SD.CmpWhat="";
            }
            LBFieldsNames->Items->Add((*FldsDescr)[i].Alias);
            WhatLookingFor.push_back(SD);
        }
    }
    CurrInd=-1;
    LBFieldsNames->ItemIndex=0;
    CheckChange();
}
//---------------------------------------------------------------------------
void __fastcall TFrmFindDbPos::CBFieldsNamesChange(TObject *Sender)
{
    if (CurrInd != -1 && Dirty)
    {
        if (AskMessage("Сохранить измения?")==ID_YES)
        {
            SbSaveClick(this);
        }
    }
    int II= LBFieldsNames->ItemIndex;
    CurrInd = II;
    SSearchDescr* SD = &WhatLookingFor[II];
    if  (II !=-1)
    {
        if (WhatLookingFor[II].Type ==aftStr)
        {
            ChBCaseSensive->Visible = true;
            if (RGSign->Items->Count <4)
                RGSign->Items->Add("~ Приблизительно равно");
        }
        else
        {
            ChBCaseSensive->Visible = false;
            if (RGSign->Items->Count >3)
                RGSign->Items->Delete(3);
        }
        TAmsInput* NewInput;

        switch (WhatLookingFor[II].Type)
        {
            case aftInt:
                NewInput = AIInt;
            break;
            case aftSumma:
                NewInput = AISum;
            break;
            case aftStr:
                NewInput = AIStr;
            break;
            case aftDate:
                NewInput = AIDate;
            break;
        }

        if (ActInput != NewInput)
        {
            if (ActInput)
            {
                ActInput->Visible= false;
                ActInput->AmsEnabled= false;
            }
            ActInput = NewInput;
            ActInput->Visible = true;
            ActInput->AmsEnabled= true;
        }

        if (WhatLookingFor[II].Clear)
        {
            ActInput->Clear();
            if (WhatLookingFor[II].Type == aftStr)
                RGSign->ItemIndex = acsAlmost;
            else
                RGSign->ItemIndex = acsEqual;
        }
        else
        {
            ActInput->SetDefaultVal(WhatLookingFor[II].CmpWhat);
            RGSign->ItemIndex = SD->EqSign;
            ChBCaseSensive->Checked = SD->CaseSens;
        }
    }
}
//---------------------------------------------------------------------------
void TFrmFindDbPos::FormSqlQuery()
{
    AnsiString S= " select CATID, ARTICLE, VER from "+BaseUserName+"."+
        DbName+" D where ";
    bool FirstTime = true;
    OQ->DeleteVariables();
    for (unsigned int i = 0; i<WhatLookingFor.size(); i++)
    {
        if (!WhatLookingFor[i].Clear)
        {
            SSearchDescr* SD = & WhatLookingFor[i];
            SFldOpt* FO = & ((*FldsDescr)[SD->FldNo]);
            if (FirstTime)
                FirstTime = false;
            else
                S+=" AND ";

            AnsiString Sign;
            AnsiString CmpWhat = "CMP_WHAT"+IntToStr(i);
            switch (SD->EqSign)
            {
                case acsAlmost:
                    Sign = " LIKE :"+CmpWhat;
                break;
                case acsEqual:
                    Sign = " = :"+CmpWhat;
                break;
                case acsMore:
                    Sign = " > :"+CmpWhat;
                break;
                case acsLess:
                    Sign = " < :"+CmpWhat;
                break;
            }

            switch (SD->Type)
            {
                case aftStr:
                    OQ->DeclareVariable(CmpWhat, otString);

                    if (SD->CaseSens)
                    {
                        S+= FO->FullName+Sign;
                        OQ->SetVariable(CmpWhat, SD->CmpWhat);
                    }
                    else
                    {
                        S+= "UPPER("+FO->FullName+")"+Sign;
                        OQ->SetVariable(CmpWhat, SD->CmpWhat.UpperCase());
                    }
                break;

                case aftInt:         
                    OQ->DeclareVariable(CmpWhat, otInteger);
                    OQ->SetVariable(CmpWhat, SD->CmpWhat.ToIntDef(0));
                break;    
                case aftSumma:         
                    OQ->DeclareVariable(CmpWhat, otFloat);
                    OQ->SetVariable(CmpWhat, SD->CmpWhat.ToDouble());
                break;    
                case aftDate:         
                    OQ->DeclareVariable(CmpWhat, otDate);
                    OQ->SetVariable(CmpWhat, StrToDate(SD->CmpWhat));
                break;    
            }
            if (SD->Type!= aftStr)
                S+= FO->FullName+Sign;
        }
    }
    OQ->SQL->Text = S;
    SqlFormed = true;        
}
//---------------------------------------------------------------------------

void __fastcall TFrmFindDbPos::SbSaveClick(TObject *Sender)
{
    SaveCurrPos();
    RefreshMemo();
}
//---------------------------------------------------------------------------
void TFrmFindDbPos::RefreshMemo()
{
    SSearchDescr* SD;
    SFldOpt * FO;
    int j = 1;     
    for (unsigned int i = 0 ; i<WhatLookingFor.size(); i++)
    {
        if (!WhatLookingFor[i].Clear)
        {
            if (j == SGrid->RowCount)
                SGrid->RowCount=j+1; 
                
            SD = &WhatLookingFor[i];
            FO = & ((*FldsDescr)[SD->FldNo]);
            SGrid->Cells[0][j]= FO->Alias;
            switch (SD->EqSign)
            {
                case acsEqual:
                    SGrid->Cells[1][j]= "=";
                break;    
                case acsMore:
                    SGrid->Cells[1][j]= ">";
                break;    
                case acsLess:
                    SGrid->Cells[1][j]= "<";
                break;    
                case acsAlmost:
                    SGrid->Cells[1][j]= "~";
                break;    
            }
            SGrid->Cells[2][j]= SD->CmpWhat;
            if (SD->CaseSens)
                SGrid->Cells[1][j]= SGrid->Cells[1][j]+"П";
            j++;
        }
    }
    if (j==1)
    {
        SGrid->RowCount=2;
        SGrid->Cells[0][1] = "";    
        SGrid->Cells[1][1] = "";    
        SGrid->Cells[2][1] = "";    
    }
    else
    {
        SGrid->RowCount=j;
    }
}
//---------------------------------------------------------------------------


void TFrmFindDbPos::SaveCurrPos()
{
    if (CurrInd!= -1)
    {
        SSearchDescr* SD= &WhatLookingFor[CurrInd];
        SD->Clear       = false;
        SD->EqSign      = RGSign->ItemIndex;
        SD->CaseSens    = ChBCaseSensive->Visible && ChBCaseSensive->Checked;
        SD->CmpWhat     = ActInput->GetResultStr();
        RefreshMemo();
        Dirty = false;
    }    
}
//---------------------------------------------------------------------------
void __fastcall TFrmFindDbPos::SBDelClick(TObject *Sender)
{
    if (CurrInd != -1)
    {
        if (!WhatLookingFor[CurrInd].Clear && AskMessage("Удалить условие выборки для "+ 
            LBFieldsNames->Items->Strings[CurrInd]) == ID_YES)
        {
            WhatLookingFor[CurrInd].Clear=true;
            CBFieldsNamesChange (this);
            RefreshMemo();
            Dirty = false;    
        }    
    }    
}
//---------------------------------------------------------------------------

void __fastcall TFrmFindDbPos::BBOkClick(TObject *Sender)
{
    SbSaveClick(this);
    ModalResult = 1;
}
//---------------------------------------------------------------------------

void __fastcall TFrmFindDbPos::BBCancelClick(TObject *Sender)
{
    ModalResult = -1;
}
//---------------------------------------------------------------------------

void __fastcall TFrmFindDbPos::FormActivate(TObject *Sender)
{
    TopMostForm = this;
}
//---------------------------------------------------------------------------
AnsiString TFrmFindDbPos::FormWhereClause(TOracleDataSet* ODS, bool DataSetModified)
{
    bool FirstTime = true;
    AnsiString S;
    for (unsigned int i = 0; i<WhatLookingFor.size(); i++)
    {
        if (!WhatLookingFor[i].Clear)
        {
            SSearchDescr* SD = & WhatLookingFor[i];
            SFldOpt* FO = & ((*FldsDescr)[SD->FldNo]);
            if (FirstTime)
                FirstTime = false;
            else
                S+=" AND ";

            AnsiString Sign;
            AnsiString CmpWhat = "CMP_WHAT"+IntToStr(i);
            switch (SD->EqSign)
            {
                case acsAlmost:
                    Sign = " LIKE :"+CmpWhat;
                break;
                case acsEqual:
                    Sign = " = :"+CmpWhat;
                break;
                case acsMore:
                    Sign = " > :"+CmpWhat;
                break;
                case acsLess:
                    Sign = " < :"+CmpWhat;
                break;
            }

            switch (SD->Type)
            {
                case aftStr:
                    if (DataSetModified)
                        ODS->DeclareVariable(CmpWhat, otString);
                    else
                        OQ->DeclareVariable(CmpWhat, otString);
                    if (SD->CaseSens)
                    {
                        S+= FO->FullName+Sign;
                        if (DataSetModified)
                            ODS->SetVariable(CmpWhat, SD->CmpWhat);
                        else
                            OQ->SetVariable(CmpWhat, SD->CmpWhat);
                    }
                    else
                    {
                        S+= "UPPER("+FO->FullName+")"+Sign;
                        if (DataSetModified)
                            ODS->SetVariable(CmpWhat, SD->CmpWhat.UpperCase());
                        else
                            OQ->SetVariable(CmpWhat, SD->CmpWhat.UpperCase());
                    }
                break;

                case aftInt:
                    if (DataSetModified)
                    {
                        ODS->DeclareVariable(CmpWhat, otInteger);
                        ODS->SetVariable(CmpWhat, SD->CmpWhat.ToIntDef(0));
                    }
                    else
                    {
                        OQ->DeclareVariable(CmpWhat, otInteger);
                        OQ->SetVariable(CmpWhat, SD->CmpWhat.ToIntDef(0));
                    }
                break;
                case aftSumma:
                    if (DataSetModified)
                    {
                        ODS->DeclareVariable(CmpWhat, otFloat);
                        ODS->SetVariable(CmpWhat, SD->CmpWhat.ToDouble());
                    }
                    else
                    {
                        OQ->DeclareVariable(CmpWhat, otFloat);
                        OQ->SetVariable(CmpWhat, SD->CmpWhat.ToDouble());
                    }
                break;
                case aftDate:
                    if (DataSetModified)
                    {
                        ODS->DeclareVariable(CmpWhat, otDate);
                        ODS->SetVariable(CmpWhat, StrToDate(SD->CmpWhat));
                    }
                    else
                    {
                        OQ->DeclareVariable(CmpWhat, otDate);
                        OQ->SetVariable(CmpWhat, StrToDate(SD->CmpWhat));
                    }
                break;
            }
            if (SD->Type!= aftStr)
                S+= FO->FullName+Sign;
        }
    }
    if (DataSetModified)
    {
        if (S!="")
            S=" AND "+S+" ";
    }
    return S;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TFrmFindDbPos::FormWhereClause(TOracleQuery* ODS, bool PutAnd)
{
    bool FirstTime = true;
    AnsiString S;
    for (unsigned int i = 0; i<WhatLookingFor.size(); i++)
    {
        if (!WhatLookingFor[i].Clear)
        {
            SSearchDescr* SD = & WhatLookingFor[i];
            SFldOpt* FO = & ((*FldsDescr)[SD->FldNo]);
            if (FirstTime)
                FirstTime = false;
            else
                S+=" AND ";

            AnsiString Sign;
            AnsiString CmpWhat = "CMP_WHAT"+IntToStr(i);
            switch (SD->EqSign)
            {
                case acsAlmost:
                    Sign = " LIKE :"+CmpWhat;
                break;
                case acsEqual:
                    Sign = " = :"+CmpWhat;
                break;
                case acsMore:
                    Sign = " > :"+CmpWhat;
                break;
                case acsLess:
                    Sign = " < :"+CmpWhat;
                break;
            }

            switch (SD->Type)
            {
                case aftStr:
                    ODS->DeclareVariable(CmpWhat, otString);
                    if (SD->CaseSens)
                    {
                        S+= FO->FullName+Sign;
                        ODS->SetVariable(CmpWhat, SD->CmpWhat);
                    }
                    else
                    {
                        S+= "UPPER("+FO->FullName+")"+Sign;
                        ODS->SetVariable(CmpWhat, SD->CmpWhat.UpperCase());
                    }
                break;

                case aftInt:
                    ODS->DeclareVariable(CmpWhat, otInteger);
                    ODS->SetVariable(CmpWhat, SD->CmpWhat.ToIntDef(0));
                break;
                case aftSumma:
                    ODS->DeclareVariable(CmpWhat, otFloat);
                    ODS->SetVariable(CmpWhat, SD->CmpWhat.ToDouble());
                break;
                case aftDate:
                    ODS->DeclareVariable(CmpWhat, otDate);
                    ODS->SetVariable(CmpWhat, StrToDate(SD->CmpWhat));
                break;
            }
            if (SD->Type!= aftStr)
                S+= FO->FullName+Sign;
        }
    }
    if (PutAnd)
        ODS->SQL->Text = ODS->SQL->Text +" AND "+ S;
    else
        ODS->SQL->Text = ODS->SQL->Text + S;
}
//---------------------------------------------------------------------------
int TFrmFindDbPos::SetFilter(void)
{
    int Res = 0;
    if (ShowModal()>0)
    {
        Res=1;
    }
    return Res;
}
//---------------------------------------------------------------------------
AnsiString TFrmFindDbPos::GetDescr(void)
{
    //
    AnsiString S;
    for (unsigned int i = 0; i< WhatLookingFor.size(); i++)
    {
        SSearchDescr* SD = & WhatLookingFor[i];
        if (!SD->Clear)
        {
            if (S!="") S+="; ";
            S+= (*FldsDescr)[SD->FldNo].Alias;
            switch (SD->EqSign)
            {
                case acsAlmost:
                    S += "~"+SD->CmpWhat;
                break;
                case acsEqual:
                    S += "="+SD->CmpWhat;
                break;
                case acsMore:
                    S += ">"+SD->CmpWhat;
                break;
                case acsLess:
                    S += "<"+SD->CmpWhat;
                break;
            }
        }
    }
    return S;
}
//---------------------------------------------------------------------------
void __fastcall TFrmFindDbPos::ActInsAndExecExecute(TObject *Sender)
{
    BBOkClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TFrmFindDbPos::LBFieldsNamesKeyUp(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
    if (Key==39)
    {
        Key=0;
        ActInput->SetFocus();
    }
    else
        CheckChange();
}
//---------------------------------------------------------------------------
void TFrmFindDbPos::CheckChange()
{
    if (LBFieldsNames->ItemIndex != CurrInd)
    {
        CBFieldsNamesChange(this);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmFindDbPos::CBFieldsNamesClick(TObject *Sender)
{
    CheckChange();
}
//---------------------------------------------------------------------------
void __fastcall TFrmFindDbPos::ActDelAllCondExecute(TObject *Sender)
{
    for (unsigned int i = 0; i < WhatLookingFor.size(); i++)
    {
        WhatLookingFor[i].Clear=true;
    }
    RefreshMemo();
    ActInput->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TFrmFindDbPos::LBFieldsNamesClick(TObject *Sender)
{
    CheckChange();
}
//---------------------------------------------------------------------------

