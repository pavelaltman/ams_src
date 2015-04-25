//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_ASK_FLDS_NAMES.h"
#include "FRM_INS_EXCEL_IMP.h"
#include "FRM_READ_EXCEL_IMP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AmsStringGrid"
#pragma resource "*.dfm"
TFrmAskFldsNames *FrmAskFldsNames;
//---------------------------------------------------------------------------
__fastcall TFrmAskFldsNames::TFrmAskFldsNames(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
// 0 - просто следующее слово
// 1 - конец рядка
// 2 - конец входящей строки
int GetNextWord(const AnsiString& Cpy, int& Pos, AnsiString& Res)
{
    int i = Pos;
    bool Finded = false;
    int Ret=2;
    Res="";
    if (Pos>Cpy.Length())
        return Ret;

    while (i<= Cpy.Length() && !Finded)
    {
        if (Cpy[i]=='\t')
        {
            Finded= true;
            Res = Cpy.SubString(Pos, i-Pos);
            Pos=i+1;
            Ret=0;
        }
        else
        if (Cpy[i]=='\r')
        {
            Finded= true;
            Res = Cpy.SubString(Pos, i-Pos);
            Pos=i+2;
            Ret=1;
        }
        else i++;
    }
    if  (!Finded)
    {
        Res=Cpy.SubString(Pos, Cpy.Length()-Pos+1);
        Pos=Cpy.Length()+1;
        Ret=1;
    }
    return Ret;
}
//---------------------------------------------------------------------------
int TFrmAskFldsNames::AskFldsNames(std::vector<int>& Res_, const AnsiString& Cpy, TFldsDescr& Descr_)
{
    Descr=&Descr_;
    Res=&Res_;
    int Ret=0;
    int j=0;
    Res_.clear();
    char str[3];
    CBColName->Items->Clear();
    CBFldName->Items->Clear();
    for (int i=0; i < Descr->Count(); i++)
    {
        SFldOpt& FO = Descr->At(i);
        if ( (!(FO.DispOpt & Hidden) && !(FO.DefOpt & Hidden)) ||
            ((FO.DispOpt & Editable)&& (FO.DefOpt & Editable)) )
        {
            Tie.push_back(i);
            Res_.push_back(i);
            j++;
            int i=j;
            if (i>26)
            {
              str[0]='A'+(i-1)/26-1;
              str[1]='A'+(i-1)%26;
              str[2]=0;
            }
            else
            {
                str[0]='A'+i-1;
                str[1]=0;
            }
            CBColName->Items->Add(str);
            CBFldName->Items->Add(FO.Alias);
        }
    }

    if (Res_.size())
    {
        CBColName->ItemIndex=0;
        CBColName->Text=CBColName->Items->Strings[0];
        CBFldName->ItemIndex=0;
        CBFldName->Text=CBFldName->Items->Strings[0];

        SG->ColCount=Res_.size();
        for (unsigned int i = 0; i<Res_.size();i++)
        {
            SG->Cells[i][1]=CBFldName->Items->Strings[i];
            SG->Cells[i][0]=CBColName->Items->Strings[i];
        }

        int RowCount= SG->FixedRows;
        int ColCount=0;
        int CurrLetter= 1;
        int Res=0;
        while (RowCount< SG->RowCount && Res==0)
        {
            AnsiString NextWord;
            Res= GetNextWord (Cpy, CurrLetter, NextWord);
            if (Res==0 || Res==1)
            {
                if (NextWord!="")
                {
                    SG->Cells[ColCount][RowCount]=NextWord;
                }
                if (Res)
                {
                    Res=0;
                    ColCount=0;
                    RowCount++;
                }
                else
                {
                    ColCount++;
                    if (ColCount>=SG->ColCount)
                    {
                        ColCount=0;
                        RowCount++;
                    }
                }
            }
        } // Заполнение GRida


        if (ShowModal()==1)
        {
            Ret=1;
        }
    }
    return Ret;
}
//---------------------------------------------------------------------------
void __fastcall TFrmAskFldsNames::BBOkClick(TObject *Sender)
{
    ModalResult=1;
}
//---------------------------------------------------------------------------
void __fastcall TFrmAskFldsNames::BitBtn1Click(TObject *Sender)
{
    ModalResult=-1;
}
//---------------------------------------------------------------------------
void __fastcall TFrmAskFldsNames::ButSetClick(TObject *Sender)
{
    if (CBColName->ItemIndex>=0 && CBFldName->ItemIndex>=0)
    {
        unsigned int i = 0;
        bool NeedReshow= false;
        while (i< Res->size() && Res->at(i)!=Tie[CBFldName->ItemIndex]) i++;
        if (i< Res->size())
        {
            if (CBColName->ItemIndex>(int)i)
            {
                int K = Res->at(i);
                for (int j = i ; j< CBColName->ItemIndex;j++)
                {
                    Res->at(j)=Res->at(j+1);
                }
                Res->at(CBColName->ItemIndex)=K;
                NeedReshow=true;
            }
            else
            if (CBColName->ItemIndex<(int)i)
            {
                int K = Res->at(i);
                for (int j = i ; j>CBColName->ItemIndex;j--)
                {
                    Res->at(j)=Res->at(j-1);
                }
                Res->at(CBColName->ItemIndex)=K;
                NeedReshow=true;
            }
            if (NeedReshow)
            {
                for (unsigned int i=0 ; i< Res->size(); i++)
                {
                    SG->Cells[i][1]= Descr->At(Res->at(i)).Alias;
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmAskFldsNames::SGMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int Column, Row;
    SG->MouseToCell(X, Y, Column, Row);
    if (CBColName->ItemIndex!= Column)
    {
        CBColName->ItemIndex= Column;
        CBColName->Text= CBColName->Items->Strings[Column];
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmAskFldsNames::ActReadFldsExecute(TObject *Sender)
{
//
    TFrmReadExcelImp* Frm;
    Application->CreateForm(__classid(TFrmReadExcelImp), &Frm);
    AmsQuery AQ;
    int DbId =  GetObjId (BaseUserName, Descr->GetDbName(), AQ());
    if (Frm->ReadExcelImp(DbId, Descr, Res)==1)
    {
        for (unsigned int i=0 ; i< Res->size(); i++)
        {
            SG->Cells[i][1]= Descr->At(Res->at(i)).Alias;
        }
        SG->ColCount=Res->size();
    };
}
//---------------------------------------------------------------------------

void __fastcall TFrmAskFldsNames::ActWriteFldsExecute(TObject *Sender)
{
//
    TFrmInsExcelImp* Frm;
    Application->CreateForm(__classid(TFrmInsExcelImp), &Frm);
    AmsQuery AQ;
    int DbId =  GetObjId (BaseUserName, Descr->GetDbName(), AQ());
    Frm->InsExcelImp(DbId, Descr, Res);
}
//---------------------------------------------------------------------------

