//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "FRM_EDIT_DB_POS.h"
#include "DAT_MOD.h"
#include "GET_DB_POS.h"
#include "FRM_BROWS_DB.h"
#include "FRM_BROWS_AN_MODAL.h"
#include "ams_win_c.h"
#include "FRM_CALC.h"
#include "FRM_MAP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AmsInputDate"
#pragma link "AmsInputStr"
#pragma link "AmsInputInt"
#pragma link "AmsInputSum"
#pragma link "AmsInputDbPos"
#pragma link "AmsInput"
#pragma link "AmsInputAnal"
#pragma link "AmsInputPoint"
#pragma resource "*.dfm"
TFrmEditDbPos *FrmEditDbPos;

extern TForm* LastForm;
//---------------------------------------------------------------------------
int TFrmEditDbPos::MaxHeight = Screen->Height-100;
//---------------------------------------------------------------------------
__fastcall TFrmEditDbPos::TFrmEditDbPos(TComponent* Owner)
    : TForm(Owner)
{
    PrevFlags= -1;
    FldsDescr=NULL;
    VerInp = NULL;
    ArtInp = NULL;
    LocVar = NULL;
    AmsInP=NULL;
    Left = 50;
    Top=50;
    ActFrm= NULL;
    InputAnalitic=false;
    FirstChange=true;
    JKL=false;
    bJKL=false;
}
//---------------------------------------------------------------------------
void TFrmEditDbPos::EditDbPos (TOracleDataSet* ODS_, int Flags,
                                FieldSetM* FSM_,TFldsDescr* FldsDescr_,
                                bool HaveFirstDigit, int FirstDigit, bool ShModal, TGenLocalVars* LocVar_)
{
    FSM= FSM_;
    TAmsInput* IS;
    SEditFldsTie EFT;
    ODS = ODS_;

    LocVar=LocVar_;
    Dirty=false;
    BeginLookForDirty=false;
    if (PrevFlags==-1)
    {
        FldsDescr=FldsDescr_;
    }

    if (!MakeEdit)
    {
        // Не были ни разу созданы элементы для ввода данных
        // создаем их

//------------------------- CHANGED AS ANSWER ON QUESTION #7 MADE 15.03.02
    jkl=0;
    jkldban=0;
    jklsdb[0]="TAmsInputAnal";
    jklsdb[1]="TAmsInputDbPos";
//-------------------------EO CHANGED AS ANSWER ON QUESTION #7 MADE 15.03.02


        int MaxLen = 0;
        TComponent* P = Pan;
        int CurrTop = 0;
        for (int i = 0; i<FldsDescr->Count(); i++)
        {
            SFldOpt& FO = (*FldsDescr)[i];
            if (FO.FldAddInfo==aiDb || FO.FldAddInfo==aiAn)
            {
                switch ((*FldsDescr)[i].Type)
                {
                    case aftDate:
                        IS = new TAmsInputDate(P);
                    break;
                    case aftSumma:
                        IS = new TAmsInputSum(P);
                    break;
                    case aftInt:
                        if ((*FldsDescr)[i].DBId<=0)
                            IS = new TAmsInputInt(P);
                        else
                        {
                            TAmsInputAnal* IAn = new TAmsInputAnal(P); // Аналитика
                            IAn->OnAmsButPressed =InpAnalClick;
                            IAn->OnAmsClrButPressed =ClrAnalClick;
                            IAn->AmsSumId=0;
                            IAn->AmsAnalId=(*FldsDescr)[i].DBId;
                            IAn->OnAmsGetAnalInfo=IAAmsGetAnalInfo;
                            //---------CH
                               IAn->OnChangeEdit=DbAnChanged;
                            //---------EO CH
                            IS=IAn;
                        }
                    break;
                    case aftStr:
                        if ((*FldsDescr)[i].DBId<=0)
                        {
                            if ((*FldsDescr)[i].Lines>1)
                            {
                                TAmsInputStrML* ML = new TAmsInputStrML(P);
                                ML->AmsLineCount =(*FldsDescr)[i].Lines;
                                IS = ML;
                            }
                            else
                            {
                                IS = new TAmsInputStr(P);
                            }
                        }
                        else
                        {
                            TAmsInputDbPos* IDb = new TAmsInputDbPos(P); // База данных
                            IDb->OnAmsButPressed = InpDbPosAmsButPressed;
                            IDb->OnAmsClrButPressed = ClrDbPosAmsButPressed;
                            IS= IDb;
                        }
                    break;
                    case aftBool:
                        IS = new TAmsInputBool(P);
                    break;
                    case aftPoint:
                         TAmsInputPoint* IP = new TAmsInputPoint(P);  // Карта
                         IP->OnAmsButPressed = InpPointAmsButPressed;
                         IP->OnAmsClrButPressed = ClrPointAmsButPressed;
                         IP->OnAmsGetAnalInfo = AmsChanged;
                         IS = IP;
                         AmsInP = IP;
                }
                IS->Top     = CurrTop;
                CurrTop    += IS->Height;
                EFT.InpFld  = IS;
                EFT.FldOpt  = &(*FldsDescr)[i];
                EFT.FldsDescrIndx = i;
                if (EFT.FldOpt->RealName=="VER")
                {
                    VerInp = dynamic_cast<TAmsInputInt*>(IS);
                }
                else
                if (EFT.FldOpt->RealName=="ARTICLE")
                {
                    ArtInp = dynamic_cast<TAmsInputStr*>(IS);
                }
                AllInput.push_back(EFT);
                IS->Visible=false;
                IS->OnAmsChanged=InpElChanged;
                IS->OnAmsPreviosElement = AmsInpAmsPreviosElement;
                IS->OnAmsNextElement    = AmsInpAmsNextElement;
                IS->OnAmsPressEnter     = AmsInpAmsPressEnter;
                IS->AmsPrompt           = (*FldsDescr)[i].Alias;
                IS->AmsMaxLen           = (*FldsDescr)[i].FldMaxWidth;
                IS->AmsInputHistory     = NULL;// DatMod->InpHist;
                IS->Align               = alTop;
                IS->AmsFldIndex         = i;
                IS->OnAmsMultiCopy= ISAmsMultiCopy;
                if ((*FldsDescr)[i].Hint!= "")
                {
                    IS->Hint = (*FldsDescr)[i].Hint;
                    IS->ShowHint = true;
                }
                if (IS->AmsPromptMaxLen > MaxLen)
                    MaxLen  = IS->AmsPromptMaxLen;
            }
        } // For i

        // Устанавливает ширину подсказки
        for (unsigned int i = 0; i<AllInput.size(); i++)
        {
            AllInput[i].InpFld->AmsPromptLen=MaxLen;
        }
        MakeEdit = true;
    }

    if (Flags != PrevFlags)
    {
        // Необходимо проставить видимость по-новому
        PrevFlags= Flags;
        SEditFldsTie EFT;
        int CurrHeight = 0;

        FirstInputEl = NULL;
        LastInputEl = NULL;
        int MaxWidth=20;
        int PromptWidth=0;
        // Идем по всем полям
        for (unsigned int i = 0; i<AllInput.size(); i++)
        {
            IS = AllInput[i].InpFld;
            PromptWidth=IS->AmsPromptLen;
            bool Ed = false;
            bool Vis = false;
            SFldOpt& FO = *(AllInput[i].FldOpt);
            if (
                (FO.FldAddInfo== aiDb && (PrevFlags & eoDbFlds)) ||
                (FO.FldAddInfo== aiAn && (PrevFlags & eoAnFlds))
               )
            {
                if ((FO.DispOpt & Editable)&&(FO.DefOpt&Editable))
                {
                    Vis = true;
                    if ( ((FO.FldAddInfo== aiAn)|| !(PrevFlags & eoBusyPos) ) || (FO.DefOpt & AlwaysFree))
                    {
                        if ((!(PrevFlags & eoHideVersion))||(FO.DefOpt&CommonVer)|| FO.FldAddInfo==aiAn)
                            Ed= true;
                       // --------------------CH LIGA
                         if (FO.RealName.UpperCase() == "ARTICLE") Ed= true;
                       //---------------------EO CH LIGA
                    }
                }
                if ( PrevFlags & eoReadOnly )
                {
                    Ed= (FO.Type==aftInt && FO.DBId!=-1);
                }
            }
            if (Vis)
            {
                IS->Visible=true;

//------------------------- CHANGED AS ANSWER ON QUESTION #7 MADE 15.03.02
                if ((jklsdb[0]==IS->ClassName()) || (jklsdb[0]==IS->ClassName())) {jkldban++; jklcopm=IS;}
                else jkl++;
//-------------------------EO CHANGED AS ANSWER ON QUESTION #7 MADE 15.03.02


                IS->AmsEnabled= Ed;
                IS->Top= CurrHeight+1;
                CurrHeight += IS->Height;
                if (FO.DiplWidth>MaxWidth)
                    MaxWidth=FO.DiplWidth;

            } // Must edit
            else
            {
                IS->Visible = false;
            }
            if (Vis && Ed)
            {
                if (!FirstInputEl)
                    FirstInputEl = IS;
                if (LastInputEl)
                    LastInputEl->SetPost(IS);
                IS->SetPred (LastInputEl);
                LastInputEl = IS;
            }
        } // For i
        if (LastInputEl)
            LastInputEl->SetPost(BBOk);
        Height = CurrHeight > MaxHeight ? MaxHeight: CurrHeight+60;

        TCanvas* Canv= this->Canvas;
        int W=Canv->TextWidth("W");
        int TotWidth= PromptWidth+W*MaxWidth+20;
        if (AmsWinC->Width-100< TotWidth)
        {
            TotWidth=AmsWinC->Width-100;
        }
        if (Left+Width>AmsWinC->Width)
        {
            Left=50;
        }
        Width=TotWidth;

        PrevFlags = Flags;
    } // PrevFlag != Flag

    BBOk->Visible=!(PrevFlags & eoReadOnly);

    if (PrevFlags &  eoNewPos) Caption="Ввод новой позиции";

    FillForm();
    //if (PrevFlags & eoDubbing)  // Нужно копировать содержимое текущее
    //if (1)
    {
        SFldOpt * Fo;
        int TabStops = 0;
        for (unsigned int i = 0; i<AllInput.size(); i++)
        {
            IS = AllInput[i].InpFld;
            if (IS->Visible || (PrevFlags &  eoNewPos)  )
            {
                IS->TabOrder = TabStops++;
            }
        }

        if ((PrevFlags &  eoNewPos) && (PrevFlags & eoDubbing))
        {// Дублируем позицию => Находим макс версию для артикула
            if (VerInp)
            {
                    int Res = 0;
                    TOracleQuery* OQ = DatMod->GetOQ();
                    try
                    {
                        OQ->SQL->Text = " select max(VER) from "+BaseUserName+
                        "."+FldsDescr->GetDbName()+" where ARTICLE = :ARTICLE ";
                        OQ->DeleteVariables();
                        OQ->DeclareVariable("ARTICLE", otString);
                        OQ->SetVariable("ARTICLE", ODS->FieldByName("ARTICLE")->AsString);
                        ExecSQL(OQ, "Max артикул");
                        if (!OQ->Eof)
                        {
                            Res=OQ->FieldAsInteger(0)+1;
                        }
                        DatMod->ReleaseOQ(OQ);
                        VerInp->SetDefaultVal(Res);
                    }
                    catch (...)
                    {
                        DatMod->ReleaseOQ(OQ);
                        throw;
                    }
            }
            else
            {
                THROW_AMS_EXC("Нет поля для ввода ВЕРСИИ");
            }
        }
    }
    BeginLookForDirty=true;
    if (! (PrevFlags & eoReadOnly) && !FirstInputEl )
    {
        OkMessage("Нет полей для ввода");
        Hide();
    }
    else
    {

        GoBack =HaveFirstDigit;
        if (HaveFirstDigit)
        {
            FirstInputEl->SetDefaultVal(IntToStr(FirstDigit));
        }
        //LastForm=this;

//------------------------- CHANGED AS ANSWER ON QUESTION #7 MADE 15.03.02
        if (!jkl && (jkldban==1)) {
            Show();
           ((TAmsInputDbPos *)jklcopm)->OnAmsButPressed(jklcopm);
           JKL=true;
            return;
        }
//-------------------------EO CHANGED AS ANSWER ON QUESTION #7 MADE 15.03.02


        if (ShModal)
        {
            ShowModal();
            //LastForm=NULL;
        }
        else
            Show();
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmEditDbPos::InpDbPosAmsButPressed(TObject *Sender)
{
    TAmsInputDbPos* IP = dynamic_cast<TAmsInputDbPos*> (Sender);
    AnsiString Art = IP->AmsArticle;
    SRefs* SR = &FldsDescr->GetRefs(IP->AmsFldIndex);
    int Ver=0; bool h=true; TAmsInputInt* InpVer;
    try {
      InpVer =((TAmsInputInt*)AllInput[SR->VerFld].InpFld);
      Ver=InpVer->GetResultInt();
    } catch (...) {
      h=false;
    }
    int DbId = (*FldsDescr)[IP->AmsFldIndex].DBId;
    if (GetDbPos(DbId, Art, Ver, (*FldsDescr)[IP->AmsFldIndex].SetName ))
    {
        if (h) InpVer->SetDefaultVal(Ver);
        IP->SetDefaultVal(Art);
        AmsInpAmsNextElement(IP);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmEditDbPos::ClrDbPosAmsButPressed(TObject *Sender)
{
    TAmsInputDbPos* IP = dynamic_cast<TAmsInputDbPos*> (Sender);
    if (IP)
    {
        if (AskMessage("Желаете обнулить поле "+IP->AmsPrompt+"?", false)==ID_YES)
        {
            IP->Clear();
            SRefs* SR = &FldsDescr->GetRefs(IP->AmsFldIndex);
            TAmsInputInt* InpVer = (TAmsInputInt*) AllInput[SR->VerFld].InpFld;
            InpVer->Clear();
        }
    }
}
//---------------------------------------------------------------------------
void TFrmEditDbPos::OkBut()
{
                    if (PrevFlags & eoReadOnly)
    {
        ModalResult=1;
        return ;
    }
    bool CHartver=false;
    AnsiString s="TEdit";
        if (s==((TEdit*)ActiveControl)->ClassName()) {
          ((TEdit*)ActiveControl)->SelStart=0;
          ((TEdit*)ActiveControl)->SelLength=((TEdit*)ActiveControl)->Text.Length();
        }
//    OpVars* OP=LocVar->VARS();
//    OP->DBStr
    //---------------------------------------------
    // Запускаем еще раз программу для базы данных

    for (unsigned int i =0; i<AllInput.size();i++)
    {
        OraField* OF = FSM->At(AllInput[i].FldsDescrIndx);
        //SFldOpt& FO= * (AllInput[i].FldOpt);
        OF->GetResult(AllInput[i].InpFld);
    }
    RunProg(1, 0,0);
    // Запоминаем результаты работы программы в переменные
    FillForm();
    //----------------------------------------------
    if (VerInp && VerInp->AmsIsNull) // Если не указа версия - ставим нулевую
    {
        VerInp->SetDefaultVal(0);
    }

    if (ArtInp)
      ArtInp->SetDefaultVal(FormArticle(ArtInp->GetResultStr()));


    //int Dubbing = false;
    if (PrevFlags&eoNewPos)
    {
        // Проверяем правило нулевых версий
        if (VerInp && VerInp->GetResultInt()!=0)
        {
            if (FldsDescr->IsNullRule())
            {
                AmsQuery AQ;
                AQ.SetSqlTextReady(
                "select 1 from "+BaseUserName+"."+FldsDescr->GetDbName()+" where "
                "ARTICLE=:ARTICLE and VER=0");
                AQ.DeclVarStr("ARTICLE", ArtInp->GetResultStr());
                AQ.Exec("Есть ли нулевые версии");
                if (!AQ.NotEof())
                {
                    THROW_AMS_EXC("Необходимо сначала ввести нулевую версию");
                }
            }
        }
        int catidjkl;
        catidjkl=ODS->FieldByName("CATID")->AsInteger;

        ODS->Insert();

        if (WithoutCats) ODS->FieldByName("CATID")->AsInteger=catidjkl;
        else ODS->FieldByName("CATID")->AsInteger = FrmBrowsDb->GetCurrDbCat(); //EO CH CATID
        
        if (ODS->FieldByName("CATID")->AsInteger==0)
        {
            AmsQuery AQ;
            AQ.SetSqlText("select OWNCAT from *.DBCAT where "
            "DBID=(select OID from *.OBJS where NAME=:NAME) and CATID is NULL");
            AQ.DeclVar("NAME", FldsDescr->GetDbName());
            AQ.Exec("Находим корневую категорию БД");
            if (AQ.NotEof())
            {
                ODS->FieldByName("CATID")->AsInteger=AQ.GetFldInt(0);
            }
        }
    }
    else
        ODS->Edit();

    if ((PrevFlags&eoNewPos) || (PrevFlags&eoDubbing))
    {
        //Dubbing = true;
    }


    if (!(PrevFlags&eoNewPos))
    {
        // Проверяем если изменился артикул + версия
        if (VerInp && ArtInp)
        {
            if (// (!(PrevFlags & eoHideVersion)) &&
            /*CHLIGA (!(PrevFlags & eoHideVersion)) && EO CHLIGA*/
                (

                    (ArtInp->GetResultStr() != ODS->FieldByName("ARTICLE")->AsString) ||
                    (VerInp->GetResultInt() != ODS->FieldByName("VER")->AsInteger)
                )
            )
            {
                CHartver=true;
                RenPos ();
                int CatId = ODS->FieldByName("CATID")->AsInteger;
                ODS->Refresh();
                FrmBrowsDb->SetPos(ArtInp->GetResultStr(), VerInp->GetResultInt(), CatId);
            }
        }
        ODS->Edit();
    }

    for (unsigned int i = 0; i< AllInput.size(); i++)
    {
        TAmsInput* IS=AllInput[i].InpFld;
        {
            SFldOpt& FO = *(AllInput[i].FldOpt);
 //----------------CH LIGA
            if (!(PrevFlags & eoNewPos)) {
               if (/*(WithoutCats) ||*/(( FO.RealName.UpperCase() != "CATID") && (FO.RealName.UpperCase() != "VER") &&  (FO.RealName.UpperCase() != "ARTICLE") &&
               (FO.RealName.UpperCase() != "SUMID") && (FO.RealName.UpperCase() != "AARTICLE") && (FO.RealName.UpperCase() != "AVER")))
                  IS->SetValToFld(ODS->Fields->Fields[FO.FldNo]);
            } else {
                if (/*(WithoutCats) ||*/ ( FO.RealName.UpperCase() != "CATID"))
                  IS->SetValToFld(ODS->Fields->Fields[FO.FldNo]);

            }
 //----------------EO CH LIGA
        }
    }
    try
    {
        if (FldsDescr->GetAnName()!="" && ODS->State==dsInsert)
        {
            ((TFrmBrowsAn*)FrmBrowsDb)->MakeInsert(ODS);
        }
        else {
            ODS->Post();
        }
    }
    catch (Exception& E)
    {
        Message(E.Message);
        ODS->Cancel();
        if (FormStyle==fsNormal) Hide();
        return;
    }
    if (FrmBrowsDb)
    {
        FrmBrowsDb->EndEdit();
        FrmBrowsDb->EdNewAnal=false;
    }
    if (FormStyle==fsNormal && !bJKL) Hide();
    ModalResult = 1;
    if (this->Name=="FrmEditDbPosOnly") return;
    if ((PrevFlags&eoNewPos) && (FrmBrowsDb->Name=="FrmBrowsAn")) {((TFrmBrowsAn*)FrmBrowsDb)->ActEditAnExecute(this);return;}
    if ((PrevFlags&eoHideVersion) && (!CHartver) && (FrmBrowsDb->Name=="FrmBrowsAn"))
                 ((TFrmBrowsAn*)FrmBrowsDb)->CalcAnSumWithoutVer(ODS->FieldByName("Article")->AsString);

    if ((!(PrevFlags&eoNewPos))&&(FrmBrowsDb->Name=="FrmBrowsAn"))
        {ODS->Next();}

}
//---------------------------------------------------------------------------
void __fastcall TFrmEditDbPos::BBOkClick(TObject *Sender)
{
    OkBut();
    if (AmsInP)
       AmsInP->AmsSumId=0;
}
//---------------------------------------------------------------------------
void __fastcall TFrmEditDbPos::BBCancelClick(TObject *Sender)
{
    //Hide();
    FrmBrowsDb->EdNewAnal=false;
    if ( !Dirty || AskMessage("Не запоминать изменения?",false)==ID_YES)
    {
        ModalResult = -1;
        if (FrmBrowsDb)
        {
            FrmBrowsDb->EndEdit();
        }
        if (FormStyle==fsNormal) Hide();
    }
    if (AmsInP)
       AmsInP->AmsSumId=0;
}
//---------------------------------------------------------------------------
void __fastcall TFrmEditDbPos::AmsInpAmsNextElement(TObject *Sender)
{
    ((TAmsInput*)Sender)->SetActPost();
}
//---------------------------------------------------------------------------
void TFrmEditDbPos::LastElPressed()
{
    BBOkClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TFrmEditDbPos::AmsInpAmsPressEnter(TObject *Sender)
{
    TAmsInput* AI = dynamic_cast<TAmsInput*>(Sender);
    if (AI)
    {
        if (AI ==  LastInputEl)
        {
            LastElPressed();
            return ;
        }
        ((TAmsInput*)Sender)->SetActPost();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmEditDbPos::AmsInpAmsPreviosElement(TObject *Sender)
{
    ((TAmsInput*)Sender)->SetActPred();
}
//---------------------------------------------------------------------------

void __fastcall TFrmEditDbPos::FormActivate(TObject *Sender)
{
    //TopMostForm = this;
    if (ActFrm)
        ActFrm->SetFocus();
    if (JKL) {
      JKL=!JKL; bJKL=true;
      OkBut();
    }

}
//---------------------------------------------------------------------------
void TFrmEditDbPos::RenPos()
{
//---------------------------------CH LIGA

    TOracleQuery* OQ= DatMod->GetOQ();
    TOracleQuery* OQN= DatMod->GetOQ();
    AmsQuery OHQ;
    try
    {
      if (!FrmBrowsDb->WithoutVer) {
        OQ->SQL->Text =  " begin "+BaseUserName+"."+FldsDescr->GetDbName()+"_RENAME "
        "(:OLDART, :OLDVER, :NEWART, :NEWVER);commit;end;";
        OQ->DeleteVariables();
        OQ->DeclareVariable(":NEWART", otString);
        OQ->DeclareVariable(":OLDART", otString);
        OQ->DeclareVariable(":NEWVER", otInteger);
        OQ->DeclareVariable(":OLDVER", otInteger);

        OQ->SetVariable(":OLDART", ODS->FieldByName("ARTICLE")->AsString);
        OQ->SetVariable(":OLDVER", ODS->FieldByName("VER")->AsInteger);

        OQ->SetVariable(":NEWART", ArtInp->GetResultStr());
        OQ->SetVariable(":NEWVER", VerInp->GetResultInt());
        ExecSQL(OQ, "Изменяем Артикул и версию в БД");
      } else {

        OQ->DeleteVariables();
        OQN->DeleteVariables();
        OQN->SQL->Text= "select * from "+BaseUserName+"."+FldsDescr->GetDbName()+
                        " where Article='"+ODS->FieldByName("ARTICLE")->AsString+"'";
        OQN->Execute();
        while (!OQN->Eof) {
           OQ->SQL->Clear();
           OQ->DeleteVariables();
           AnsiString Js="";
           Js="insert into "+BaseUserName+"."+FldsDescr->GetDbName()+" values ('"+
                         OQN->FieldAsString(0)+"', '"+ArtInp->GetResultStr()+"', "+
                         OQN->FieldAsString(2);//+"";
                 for (int j=3; j<OQN->FieldCount(); j++) {
                   Js+=", '"+OQN->FieldAsString(j)+"'";

                 }

           Js+=");";

           OHQ.SetPLText(Js);
           OHQ.Exec("Добавляем новый Артикл со старой версией");
//           ExecSQL(OQ, "Добавляем новый Артикл со старой версией");

           OQ->SQL->Clear();
           OQ->DeleteVariables();
           OQ->SQL->Text="update "+BaseUserName+"."+FldsDescr->GetAnName()+
                         " set AARTICLE='"+ArtInp->GetResultStr()+"', AVER='"+
                         OQN->FieldAsString(2)+"' where AARTICLE='"+
                         ODS->FieldByName("ARTICLE")->AsString+"' and AVER='"+
                         OQN->FieldAsString(2)+"'";
           ExecSQL(OQ, "Изменяем Артикл\версию в аналитике");

           OQN->Next();
        }

           OQ->SQL->Clear();
           OQ->SQL->Text="Delete from " +BaseUserName+"."+FldsDescr->GetDbName()+
                         " where ARTICLE='"+ODS->FieldByName("ARTICLE")->AsString+"'";
           ExecSQL(OQ, "Удаляем старый Артикл");
           OQ->SQL->Clear();
      }

      OQ->Close();
      OQN->Close();
      DatMod->ReleaseOQ(OQ);
      DatMod->ReleaseOQ(OQN);
    }
    catch(...)
    {
        OQ->Close();
        OQN->Close();
        DatMod->ReleaseOQ(OQN);
        DatMod->ReleaseOQ(OQ);
        throw;
    }
//---------------------------------EO CH LIGA
}
//---------------------------------------------------------------------------
void __fastcall TFrmEditDbPos::ISAmsMultiCopy(TAmsInput *IS,
      const AnsiString &WhatCopy)
{
//  Копирование из Excel или др программы
    unsigned int i = 0;
    TAmsInput* NextEl = IS;
    while (i < AllInput.size() && AllInput[i].InpFld != IS) i++;
    if ( i< AllInput.size())
    {
        int FPos = 1;
        for (int j = 1; j< WhatCopy.Length(); j++)
        {
            if (NextEl)
            {
                if (WhatCopy[j]==9 || WhatCopy[j]=='\r')
                {
                    if (NextEl != IS)
                        NextEl->SetDefaultVal( WhatCopy.SubString(FPos, j-FPos));
                    FPos = j+1;
                    i++;
                    while (i < AllInput.size() && !AllInput[i].InpFld->Enabled) i++;
                    if ( i< AllInput.size())
                        NextEl =    AllInput[i].InpFld;
                    else
                        NextEl =    NULL;
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void TFrmEditDbPos::FillForm(void)
{
    SFldOpt * Fo;
    for (unsigned int i = 0; i<AllInput.size(); i++)
    {
        TAmsInput* IS = AllInput[i].InpFld;
        SFldOpt& FO = *(AllInput[i].FldOpt);
        if (FSM)
            FSM->SetInputVal(IS, AllInput[i].FldsDescrIndx);
        else
            IS->SetDefaultVal(ODS->Fields->Fields[FO.FldNo]);
        // Если позиция - сcылка на БД, то необходимо копировать и версию
        /*
        TAmsInputDbPos * F = dynamic_cast<TAmsInputDbPos*>(IS);
        if (F)
        {
            SRefs& SR = FldsDescr->GetRefs(F->AmsFldIndex);
            TAmsInput* VerDbPos = AllInput[SR.VerFld].InpFld;
            if (FSM)
                FSM->SetInputVal(VerDbPos,SR.VerFld);
            else
                VerDbPos->SetDefaultVal(ODS->Fields->Fields[(*FldsDescr)[SR.VerFld].FldNo]);
        }
        */
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmEditDbPos::FormShow(TObject *Sender)
{
    if (FirstInputEl)
    {
        BBCancel->SetFocus();
        FirstInputEl->SetFocus();
        if (GoBack)
            FirstInputEl->GoBack();
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmEditDbPos::InpAnalClick(TObject *Sender)
{
//
    IA = dynamic_cast<TAmsInputAnal*> (Sender);
    AnAnSumId=IA->AmsSumId;
    AnAnId = (*FldsDescr)[IA->AmsFldIndex].DBId;

    AmsQuery AQ;
    AQ.SetSqlText("BEGIN *.CreateAndCopyAnSum(:ANAL_ID,:NEW_SUM_ID,:SUM_ID);commit;END;");
    AQ.DeclVar("NEW_SUM_ID", 0);
    AQ.DeclVar("SUM_ID" , AnAnSumId);
    AQ.DeclVar("ANAL_ID", AnAnId);

    AQ.Exec("Скопировать аналит сумму");

    AnNewSumId = AQ.GetVarInt("NEW_SUM_ID");

    TFrmBrowsAn* Frm = new TFrmBrowsAn(this);
    TFrmBrowsAn::BegMode BM;
    BM.ReadOnly=PrevFlags & eoReadOnly;
    BM.Vars=LocVar;
    FrmBrowsDb->EdNewAnal=true;
    ActFrm=Frm;

    Frm->Begin(this,AnAnId, "", (*FldsDescr)[IA->AmsFldIndex].SetName,
        "Вводим аналитику ", AnNewSumId, 0, BM);
    /*
    if (Frm->BrowsAnModal(this,AnAnId, "", (*FldsDescr)[IA->AmsFldIndex].SetName,
        "Вводим аналитику ", AnNewSumId, 0, BM)>0)
    {
        ActFrm = NULL;
        AQ.Clear();
        if (AnAnSumId == 0)
        {
            IA->AmsSumId = AnNewSumId;
        }
        else
        {
            AQ.SetSqlText("BEGIN *.CreateAndCopyAnSum(:ANAL_ID,:SUM_ID,:NEW_SUM_ID, 1);commit;END;");
            AQ.DeclVar("NEW_SUM_ID", AnNewSumId);
            AQ.DeclVar("SUM_ID" , AnAnSumId);
            AQ.DeclVar("ANAL_ID", AnAnId);

            AQ.Exec("Копируем и удаляем");
        }
    }
    else
    {
        ActFrm = NULL;
        AQ.Clear();
        AQ.SetSqlText("BEGIN delete from *.SUMS where SUM_ID=:SUM_ID;commit;END;");
        AQ.DeclVar("SUM_ID" , AnNewSumId);
        AQ.Exec("Удаляем аналит сумму");
    };*/

}
//---------------------------------------------------------------------------
void __fastcall TFrmEditDbPos::ClrAnalClick(TObject *Sender)
{
//
    if (AskMessage("Обнулить поле?", false)==ID_YES)
    {
        IA = dynamic_cast<TAmsInputAnal*> (Sender);
        AnAnSumId=IA->AmsSumId;
        AnAnId = (*FldsDescr)[IA->AmsFldIndex].DBId;
        if (AskMessage("Удалить аналитическую сумму?")==ID_YES)
        {
            AmsQuery AQ;
            AQ.SetPLText("delete from *.SUMS where SUM_ID=:SUM_ID;commit;");
            AQ.DeclVar("SUM_ID", AnAnSumId);
            AQ.Exec("Удаляем ненужную аналитическую сумму");
        }
        IA->Clear();
    }
}
//---------------------------------------------------------------------------
void TFrmEditDbPos::AnalOk()
{
    TFrmBrowsAn* Frm = dynamic_cast<TFrmBrowsAn*>(ActFrm);
    if(Frm)
    {
        ActFrm = NULL;
        AmsQuery AQ;
        if (AnAnSumId == 0)
        {
            IA->AmsSumId = AnNewSumId;
        }
        else
        {
            AQ.SetSqlText("BEGIN *.CreateAndCopyAnSum(:ANAL_ID,:SUM_ID,:NEW_SUM_ID, 1);commit;END;");
            AQ.DeclVar("NEW_SUM_ID", AnNewSumId);
            AQ.DeclVar("SUM_ID" , AnAnSumId);
            AQ.DeclVar("ANAL_ID", AnAnId);
            AQ.Exec("Копируем и удаляем");
        }
        Frm->Release();
    };
}
//---------------------------------------------------------------------------
void TFrmEditDbPos::AnalCancel()
{
    TFrmBrowsAn* Frm = dynamic_cast<TFrmBrowsAn*>(ActFrm);
    if(Frm)
    {
        ActFrm = NULL;
        AmsQuery AQ;
        AQ.SetSqlText("BEGIN delete from *.SUMS where SUM_ID=:SUM_ID;commit;END;");
        AQ.DeclVar("SUM_ID" , AnNewSumId);
        AQ.Exec("Удаляем аналит сумму");
        Frm->Release();
    }
}
//---------------------------------------------------------------------------
void TFrmEditDbPos::RunProg(bool Post, bool SetFlds, bool ClrFld, bool ClearF4, bool UseFsm)
{
    FrmBrowsDb->RunProg(Post, SetFlds, ClrFld, ClearF4, UseFsm);
};
//---------------------------------------------------------------------------
void __fastcall TFrmEditDbPos::ButEnterClick(TObject *Sender)
{
    AmsInpAmsPressEnter(ActiveControl->Parent);
}
//---------------------------------------------------------------------------
void __fastcall TFrmEditDbPos::InpElChanged(TObject *Sender)
{
    if (BeginLookForDirty)
        Dirty=true;
};
//---------------------------------------------------------------------------


void __fastcall TFrmEditDbPos::ActCalcExecute(TObject *Sender)
{
    //
    TWinControl* AC= this->ActiveControl;
    TAmsInput* AI=dynamic_cast<TAmsInput*>(AC->Parent);

    if (FrmCalc->ShowModal()==1)
    {
        if (AI)
            AI->SetDefaultVal(FrmCalc->EdRes->Text);
    };
}
//---------------------------------------------------------------------------
int TFrmEditDbPos::ShtrixAddOne(TOracleDataSet* ODS_,
                     const AnsiString& ShFld_,
                     const AnsiString& ShOrderFld_, int TotPos)
{
    OpVars* OP=FrmBrowsDb->GetOpVars();
    AnsiString Article=ODS_->FieldByName("ARTICLE")->AsString;
    int Ver=ODS_->FieldByName("VER")->AsInteger;
    // Находим первое редактируемое поле аналитики
    int j=0;
    bool NoFld=true;
    while ( j < FldsDescr->Count())
    {
        SFldOpt& FO= FldsDescr->At(j);
        if (FO.FldAddInfo==aiAn)
        {
            if ((FO.DispOpt & Editable)&&(FO.DefOpt& Editable)&&NoFld)
            {
                if (FO.Type==aftInt || FO.Type==aftSumma)
                {
                    Node* N = OP->Stru()->FindVar(FO.RealName.c_str());
                    if (N)
                    {
                        long double& S=N->GetSum().dlv;
                        S++;
                    }
                    else
                        OP->AddVarSum(FO.RealName, 1);
                }
                else
                {
                    ErrorMessage("Первое редактируемое поле должно быть КОЛИЧЕСТВО");
                    return -2;
                }
                break;
            }
        }
        j++;
    }
    OP->AddVarStr("AARTICLE", Article);
    OP->AddVarInt("AVER", Ver);


    OP->AddVarInt(ShOrderFld_, TotPos+1);
    RunProg(0, 0, 0, 0, 0);
    RunProg(1, 0, 0, 0, 0);
    return 0;
};
//---------------------------------------------------------------------------
int TFrmEditDbPos::ShtrixMakeNewPos( TOracleDataSet* ODS_,
                            const AnsiString& ShFld_,
                            const AnsiString& ShOrderFld_,
                            const AnsiString& ShtrixVal_, int TotPos)
{
    AmsQuery AQ;
    AQ.SetSqlText("select * from *."+FldsDescr->GetDbName()+
        " where "+ShFld_+"=:SH_FLD");
    AQ.DeclVar("SH_FLD", ShtrixVal_);
    AQ.Exec("Находим позицию БД с таким штрихкодом");
    if (!AQ.NotEof())
    {
        ErrorMessage("Такой штрих-код не обнаружен в базе");
        return 1;
    }

    OpVars* OP=FrmBrowsDb->GetOpVars();
    OP->AddVars(AQ);
    AnsiString Article= AQ.GetFldStr("ARTICLE");
    int Ver= AQ.GetFldInt("VER");

    // Находим первое редактируемое поле аналитики
    int j=0;
    while ( j < FldsDescr->Count())
    {
        SFldOpt& FO= FldsDescr->At(j);
        if (FO.FldAddInfo==aiAn)
        {
            if ((FO.DispOpt & Editable)&&(FO.DefOpt& Editable))
            {
                if (FO.Type==aftInt)
                {
                    OP->AddVarInt(FO.RealName,1);
                }
                else
                if (FO.Type==aftSumma)
                {
                    OP->AddVarSum(FO.RealName,1);
                }
                else
                {
                    ErrorMessage("Первое редактируемое поле должно быть КОЛИЧЕСТВО");
                    return -2;
                }
                break;
            }
        }
        j++;
    }

    for (int i=0; i< FldsDescr->Count(); i++)
    {
        SFldOpt& FO= FldsDescr->At(i);

        if (FO.FldAddInfo==aiAn && i!=j)
        {
            if (FO.RealName.UpperCase()=="AARTICLE")
                OP->AddVarStr("AARTICLE", Article);
            else
            if (FO.RealName.UpperCase()=="AVER")
                OP->AddVarInt("AVER", Ver);
            else
            {
                switch(FO.Type)
                {
                    case aftDate:
                        OP->AddVarDate(FO.RealName,0, true);
                    break;
                    case aftInt:
                    case aftBool:
                        OP->AddVarInt(FO.RealName,0, true);
                    break;
                    case aftStr:
                        OP->AddVarStr(FO.RealName,"");
                    break;
                    case aftSumma:
                        OP->AddVarSum(FO.RealName, 0, true);
                    break;
                }
            }
        }
    }
    OP->AddVarInt(ShOrderFld_, TotPos+1);
    RunProg(0, 0, 0, 0, 0);
    RunProg(1, 0, 0, 0, 0);
    return 0;
};
//---------------------------------------------------------------------------
void __fastcall TFrmEditDbPos::IAAmsGetAnalInfo(TAmsInputAnal *Sender)
{
    if (Sender->AmsSumId)
    {
        AmsQuery AQ;
        AQ.SetPLText(":AnalInfo:=*.GetAnalInfo(:SUM_ID, :ANAL_ID);");
        AQ.DeclVar("ANALINFO", " ");
        AQ.DeclVar("SUM_ID", Sender->AmsSumId);
        AQ.DeclVar("ANAL_ID", Sender->AmsAnalId);
        AQ.Exec("Выбираем анал инфо");
        Sender->AmsAnalInfo=AQ.GetVarStr("ANALINFO");
    }
    else
        Sender->AmsAnalInfo="";
}
//---------------------------------------------------------------------------

void __fastcall TFrmEditDbPos::DbAnChanged(TObject *Sender)
{
  if (FirstChange) {FirstChange=!FirstChange; return;}
  if (!jkl && (jkldban==1)) {FirstChange=!FirstChange; OkBut();}
}

//CH---------------------------28-03-02----------------------------------------

void __fastcall TFrmEditDbPos::ClrPointAmsButPressed(TObject * Sender)
{
    TAmsInputPoint* IP = dynamic_cast<TAmsInputPoint*> (Sender);
    if (IP)
    {
        if (AskMessage("Желаете обнулить поле "+IP->AmsPrompt+"?", false)==ID_YES)
            IP->Clear();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmEditDbPos::InpPointAmsButPressed(TObject* Sender)
{
    TAmsInputPoint* IP = dynamic_cast<TAmsInputPoint*> (Sender);
    TFrmMap* F;
    Application->CreateForm(__classid(TFrmMap), &F);
    F->InitInsertPoint(IP, IP->AmsSumId);
    F->Show();
}
//---------------------------------------------------------------------------

void __fastcall TFrmEditDbPos::AmsChanged(TAmsInputAnal* Sender)
{
     TAmsInputPoint* IP = dynamic_cast<TAmsInputPoint*> (Sender);
     if (IP->AmsSumId>0)
       IP->AmsAnalInfo="Определено";
     else IP->AmsAnalInfo="Не определено";
}
//--------------------------------------------------------------------------

AnsiString __fastcall TFrmEditDbPos::FormArticle(AnsiString str)
{
    char* ch=str.AnsiLastChar();
    int i=*ch;
    do
    {
      if (i==32)
      {
        str.SetLength(str.Length()-1);
        ch=str.AnsiLastChar();
        i=*ch;
      };
    } while(i==32);
    return str;
}
//EO CH-------------------------------------28-03-02-------------------------
void __fastcall TFrmEditDbPos::ButEnterEnter(TObject *Sender)
{
   BBCancel->SetFocus();
}
//---------------------------------------------------------------------------



