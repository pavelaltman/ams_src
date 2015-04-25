//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_FOOTER_VISIBLE.h"
#include "FOOTER_CNTRL.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmFooterVisible *FrmFooterVisible;
//---------------------------------------------------------------------------
__fastcall TFrmFooterVisible::TFrmFooterVisible(TComponent* Owner)
    : TForm(Owner)
{
    AllRG.push_back(FVAll);
    AllRG.push_back(FVFiltered);
    AllRG.push_back(FVCurrCat);
    AllRG.push_back(FVSubTree);
    AllRG.push_back(FVSelected);
}
//---------------------------------------------------------------------------
void __fastcall TFrmFooterVisible::BitBtn2Click(TObject *Sender)
{
    ModalResult = 1;
}
//---------------------------------------------------------------------------
void __fastcall TFrmFooterVisible::BitBtn1Click(TObject *Sender)
{
    ModalResult = -1;
}
//---------------------------------------------------------------------------
int TFrmFooterVisible::FooterVisible(TFooterCntrl* FooterCntrl)
{
    // Заполняем поля
    for (int i =0; i< FooterCntrl->FI.Count(); i++)
    {
        TFooterInfo::SFooterInfo& FI = FooterCntrl->FI.At(i);
        TRadioGroup* RG = AllRG[i];
        RG->ItemIndex = FI.ShowFooter;
    }
    int Res = 0;
    if (ShowModal()==1)
    {
        for (int i =0; i< FooterCntrl->FI.Count(); i++)
        {
            TFooterInfo::SFooterInfo& FI = FooterCntrl->FI.At(i);
            TRadioGroup* RG = AllRG[i];
            switch (RG->ItemIndex)
            {
                case sfUsual:
                    FI.ShowFooter=sfUsual;
                break;
                case sfAlways:
                    FI.ShowFooter=sfAlways;
                    FI.ShowLine = true;
                break;
                case sfNotShow:
                    FI.ShowFooter=sfNotShow;
                    FI.ShowLine = false;
                break;
            }
        }
        //FooterCntrl->Draw();
        Res = 1;
    }
    return Res;
}
//---------------------------------------------------------------------------

