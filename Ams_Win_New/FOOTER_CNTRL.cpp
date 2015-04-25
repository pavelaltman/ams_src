//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "AmsDbGrid.h"
#include "FOOTER_CNTRL.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

int TFooterInfo::Add(FieldSetAnTotal* WhatShow, bool Show, AnsiString Prompt)
{
    SFooterInfo S;
    S.ShowLine=Show;
    S.WhatShow = WhatShow;
    if (Prompt== "")
    {
        switch (FI.size())
        {
            case 0:
                S.TotalName="Всего итого строка";
            break;
            case 1:
                S.TotalName="Отфильтрованные позиции";
            break;
            case 2:
                S.TotalName="Текущая категория";
            break;
            case 3:
                S.TotalName="Поддерево категорий";
            break;
            case 4:
                S.TotalName="Выделенные строки";
            break;
        };
    }
    else
    {
        S.TotalName = Prompt;
    }
    S.ShowFooter= sfUsual;
    FI.push_back(S);
    return FI.size()-1;
};
//---------------------------------------------------------------------------
void TFooterInfo::Hide (int i)
{
    if (FI[i].ShowFooter != sfAlways)
        FI[i].ShowLine = false;
};
//---------------------------------------------------------------------------
void TFooterInfo::Show (int i)
{
    if (FI[i].ShowFooter != sfNotShow)
        FI[i].ShowLine = true;
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TFooterCntrl::Draw ()
{
    std::vector<int> Visible;
    for (int i = 0; i< FI.Count(); i++)
    {
        if (FI.Visible(i))
        {
            Visible.push_back(i);
        }
    }

    Footer->RowCount = Visible.size();
    for (unsigned int j = 0; j< Visible.size();j++)
    {
        FieldSetAnTotal& AnTotal = FI[Visible[j]];
        Footer->SetVal(FI.At(Visible[j]).TotalName, j, 1);
        for (unsigned int i = 0; i< ColsMap.size();i++)
        {
            OraField* OF = AnTotal.At(i);
            SFldOpt& FO= FldsDescr->operator [](ColsMap[i].FldsDescrIndx);
            if (OF && ColsMap[i].ColIndx>=0)
            {
                if (NeedDistr && (FO.DispOpt&DebetCredit) && (FO.DefOpt&DebetCredit))
                {
                    int Col = ColsMap[i].ColIndx;
                    AnsiString Res="0.0";
                    if (FO.Type==aftSumma && FO.MaxPoints!=2)
                    {
                        int ii=FO.MaxPoints-1;
                        while (ii>0)
                        {
                            Res+="0";
                            ii--;
                        }
                        if (((double)OF->Number())<0)
                        {
                            long double Res = -OF->Number();
                            Footer->SetVal(FloatToStr(Res), j, Col);
                        }
                        else
                        {
                            Col--;
                            Footer->SetVal(OF->AsString(Res), j, Col);
                        }
                    }
                    else
                    {
                        if (((double)OF->Number())<0)
                        {
                            long double Res = -OF->Number();
                            Footer->SetVal(FloatToStr(Res), j, Col);
                        }
                        else
                        {
                            Col--;
                            Footer->SetVal(OF->AsString(), j, Col);
                        }
                    }
                }
                else
                {
                    if (FO.Type==aftSumma && FO.MaxPoints!=2)
                    {
                        AnsiString Res="0.0";
                        int ii=FO.MaxPoints-1;
                        while (ii>0)
                        {
                            Res+="0";
                            ii--;
                        }
                        Footer->SetVal(OF->AsString(Res), j, ColsMap[i].ColIndx);
                    }
                    else
                        Footer->SetVal(OF->AsString(), j, ColsMap[i].ColIndx);
                }
            }
        }
    }
    Footer->Height=Footer->RowCount* (Footer->DefaultRowHeight+1) + 1;
};
//---------------------------------------------------------------------------
void TFooterCntrl::Tie (TAmsDBGrid* Grid, std::vector<int>& ColIndx)
{
    ColsMap.clear();
    STotalMapping TM;
    bool FirstTime = true;
    Sql="";
    for (int i = 0 ; i< FldsDescr->Count(); i++)
    {
        SFldOpt& FO=(*FldsDescr)[i];
        if (FO.FldAddInfo==aiAn && (FO.DefOpt&Totalable))
        {
            TM.FldsDescrIndx=i;
            TM.ColIndx=ColIndx[i];
            ColsMap.push_back(TM);

            if ( FirstTime)
                FirstTime = false;
            else
                Sql+= ", ";
            if ( FO.DefOpt & Addable)
                Sql+= " sum("+FO.FullName+") "+FO.SqlAlias;
            else
                Sql+= " max("+FO.FullName+") "+FO.SqlAlias;
        }
    }
    Builded= true;
    Grid->AmsSynchoEnable= true;
};
//---------------------------------------------------------------------------
// Добавить текущую запись датасета к видимым футерам
void TFooterCntrl::AddLine (TOracleDataSet* ODS_, bool OldVal)
{
    for (int i = 0 ; i<FI.Count(); i++)
    {
        if (FI.Visible(i))
        {
            FI[i].AddLine(ODS_, OldVal);
        }
    }
};
//---------------------------------------------------------------------------
// Отнять текущую запись датасета от видимых футеров
void TFooterCntrl::SubLine (TOracleDataSet* ODS_, bool OldVal)
{
    for (int i = 0 ; i<FI.Count(); i++)
    {
        if (FI.Visible(i))
        {
            FI[i].SubLine(ODS_, OldVal);
        }
    }
};
//---------------------------------------------------------------------------

