//---------------------------------------------------------------------------
#include "AMS_HDR.H"
#include <vcl.h>
#pragma hdrstop
#include "FRM_MAP.h"
#include "DAT_MOD.h"                        
#include "utils.h"
#include "Ams_Query.h"
#include "FRM_CREATE_QRY.h"
#include "ams_win_c.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Oracle"
#pragma resource "*.dfm"

#define Epsilon 5
TFrmMap *FrmMap;
//---------------------------------------------------------------------------
__fastcall TFrmMap::TFrmMap(TComponent* Owner)
    : TForm(Owner)
{
    AddPoint=0;
    CurColor=-1;
    ShowStatistic=0;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMap::InitInsertPoint(TAmsInputPoint* IM, int TypeAct)
{
     type=TypeAct;
     AmsIn=IM;
     AddPoint=1;
     if (type)
       FindPoint(type);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMap::LoadMap(AnsiString FileName, AnsiString Name, int id)
{
  try
  {
    im->Picture->LoadFromFile(FileName);
    ClientWidth=pan->Width+im->Width;
    ClientHeight=im->Height+sb->Height+ptb->Height;
    pim->Width=im->Width;
    IdRegion=id;
    sb->Panels->Items[1]->Text=Name;
    int i=2;
    if (sb->Panels->Count==2)
       sb->Panels->Add();
    AnsiString S="";
    if (((TTreeData*)(tv->Selected->Data))->AllSum)
      S=IntToStr(((TTreeData*)(tv->Selected->Data))->AllSum);
    sb->Panels->Items[2]->Text="Всего: "+S;
    sb->Panels->Items[2]->Alignment=taLeftJustify;
  }
  catch (Exception& E)
  {
    ErrorMessage(E.Message);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFrmMap::FormClose(TObject *Sender, TCloseAction &Action)
{
    AmsWinC->FindActiveMDIChild();
    FreeTreeData();
    Action= caFree;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMap::imMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    if (Button!=mbLeft) return;
    AmsQuery SelQ;
    if (InsIntoMainMap)
    {
          SelQ.SetSqlText("BEGIN insert into "+BaseUserName+".point(id, CoordX, CoordY, id_ref) values("+BaseUserName+".PointSeq.nextval, :P1, :P2, :P3);commit;END;");
          SelQ.DeclVarInt("P1", X);
          SelQ.DeclVarInt("P2", Y);
          SelQ.DeclVarInt("P3", IdRegion);
          SelQ.Exec("");
          return;
    }

    if (IdRegion==0)
    {
      if ((TColor)GetPixel(im->Canvas->Handle, X, Y)!=16694802)
      {
          AddToHist=false;
          LoadMap(((TTreeData*)(tv->Selected->Data))->path, ((TTreeData*)(tv->Selected->Data))->name, 0);
          AddToHist=true;
          for(int i=0;i<tv->Items->Count; i++)
          if ((unsigned int)((TTreeData*)(tv->Items->Item[i]->Data))->color==GetPixel(im->Canvas->Handle, X, Y))
          {
             tv->Items->Item[i]->Selected=true;
             return;
          }
      };
    };

    if (ShowStatistic)
    {
       int Id_P=IsNearPoint(X,Y);
       if (Id_P>0)
       {
       string SQLQ="select  P.id as id, Б.НАИМ as Наименование, sum(СУММА) as Сумма "
                   "from *.АН_ПОКУП А, *.point P, *.ПОКУПАТ Б, *.region R, *.op_items O "
                   "where    А.aver=Б.ver and А.aarticle=Б.article "
                   "and Б.М_РАСП=P.id and R.id=P.id_ref and O.item_sum_id=А.sumid group by Б.НАИМ , P.id";
           FillHint(Id_P,SQLQ);
           Application->HintColor=clAqua;
           Application->HintHidePause=900000;
       };
       return;
    };

    if (AddPoint)
    {
        Add(X,Y);
        Close();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmMap::ActGoTopExecute(TObject *Sender)
{
    if (IdRegion)
      (tv->Selected->Parent)->Selected=true;
}
//--------------------------------------------------------------------------

void __fastcall TFrmMap::InputTree(TTreeNode* tnd, int id)
{
   TTreeNode* cur;
   TOracleQuery* OQ=new TOracleQuery(FrmMap);
   try
   {
     OQ->Session=DatMod->OSession;
     OQ->DeclareVariable("P1", otInteger);
     OQ->SetVariable("P1", id);
     OQ->SQL->Add("select id, name, path, color from "+BaseUserName+".region where id_ref=:P1 and id>0");
     OQ->Execute();
     while (!OQ->Eof)
     {
        TTreeData *td=new TTreeData();
        td->id=OQ->Field("id");
        td->path=OQ->Field("path");
        td->color=OQ->Field("color");
        td->name=OQ->Field("name");
        td->AllSum=CalcRegionData(td->color);
        cur=tv->Items->AddChildObject(tnd,OQ->Field("name"),td);
        InputTree(cur,OQ->Field("id"));
        OQ->Next();
     }
    }
    __finally
    {
        OQ->~TOracleQuery();
    }
}
//-------------------------------------------------------------------------

void __fastcall TFrmMap::FormCreate(TObject *Sender)
{
      TTreeNode *Node=new TTreeNode(NULL);
      try
      {
        Caption="Карта регионов Украины";
        TTreeData* td=new TTreeData();
        AmsQuery SelQ;
        SelQ.SetSqlText("select id, name, path, color from *.REGION R where R.id=R.id_ref");
        SelQ.Exec("");
        td->id=SelQ.GetFldInt("id");
        td->path=SelQ.GetFldStr("path");
        td->color=SelQ.GetFldInt("color");
        td->name=SelQ.GetFldStr("name");
        if (!td->id)
          td->AllSum=CalcRegionData(td->color);
        else
          td->AllSum=-1;
        Node=tv->Items->AddObjectFirst(NULL,td->name,td);
        InputTree(Node, td->id);
        ShowStatistic=false;
        AddToHist=true;
        MapHist.clear();
        FillPointsList();
        CurMapHistPos=0;
      }
      __finally
      {
      }
}
//---------------------------------------------------------------------------

void __fastcall TFrmMap::tvChange(TObject *Sender, TTreeNode *Node)
{
  LoadMap(((TTreeData*)(Node->Data))->path, ((TTreeData*)(Node->Data))->name,((TTreeData*)(Node->Data))->id);
  if (AddToHist)
  {
     MapHist[MapHist.size()+1]=Node;
     CurMapHistPos=MapHist.size();
  };
  DrawPoint(((TTreeData*)(Node->Data))->id);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMap::Splitter1Moved(TObject *Sender)
{
      ClientWidth=im->Width+pan->Width;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMap::DrawPoint(int id, int Radius)
{
        Points.clear();
        AmsQuery SelQ;
        SelQ.DeclVarInt("P1", id);
        if (id==0)
           SelQ.SetSqlText("select CoordX, CoordY, Id from *.point, *.ПОКУПАТ where id_ref=:P1");
        else
           SelQ.SetSqlText("select CoordX, CoordY, Id from *.point, *.ПОКУПАТ where id_ref=:P1 and ПОКУПАТ.М_РАСП=Id");
        SelQ.Exec("");

        im->Canvas->Font->Color = clYellow;
        TPnt Point;
        while (SelQ.NotEof())
        {
           Point.X=SelQ.GetFldInt(0);
           Point.Y=SelQ.GetFldInt(1);
           Point.Id=SelQ.GetFldInt(2);
           Points.push_back(Point);
           if (id)
           {
              im->Canvas->Brush->Color = clRed;
              im->Canvas->Ellipse(Point.X-Radius,Point.Y-Radius,Point.X+Radius,Point.Y+Radius);
           }
           SelQ.Next();
        };

        int CurId;
        if (id==0)
        {
           AmsQuery Q;
           Q.SetSqlText("select  sum(СУММА), color "
                       "from *.АН_ПОКУП А, *.point P, *.ПОКУПАТ Б, *.region R, *.op_items O "
                       "where    А.aver=Б.ver and А.aarticle=Б.article "
                       "and Б.М_РАСП=P.id and R.id=P.id_ref and O.item_sum_id=А.sumid group by R.color ");

           Q.Exec("Суммируем показатели по областям");

           while (Q.NotEof())
           {
             if (Q.GetFldStr(0)>" ")
                FindMainPntCoord((TColor)Q.GetFldInt(1), Q.GetFldStr(0));
             else
                FindMainPntCoord((TColor)Q.GetFldInt(1), "0");
             Q.Next();
           };
        }
}
//---------------------------------------------------------------------------

void __fastcall TFrmMap::imMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
   if (!IdRegion)
   {
     if (GetPixel(im->Canvas->Handle, X, Y)!=(unsigned int)CurColor)
        HintShow(X, Y);
     return;
   };

   if ((IsNearPoint(X,Y)==-1)&(ShowStatistic))
   {
     Application->CancelHint();
     Application->HintColor=(TColor)-2147483624;
     Application->HintHidePause=2500;
     im->Hint="";
   }
}
//---------------------------------------------------------------------------

void __fastcall TFrmMap::FreeTreeData()
{
        for(int i=0;i<tv->Items->Count;i++)
        delete(tv->Items->Item[i]->Data);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMap::FillPointsList()
{
    AnsiString SQLStr="select P.id, P.CoordX, P.CoordY from *.Point P";
    AmsQuery Q;
    Q.SetSqlText(SQLStr);
    Q.Exec("Строим список");
    int Id;
    MapQry.clear();
    while (Q.NotEof())
    {
      TMapQuery* MQ=new TMapQuery();
      Id= Q.GetFldInt(0);
      MQ->PX= Q.GetFldInt(1);
      MQ->PY= Q.GetFldInt(2);
      MapQry[Id]=MQ;
      Q.Next();
    };
}
//---------------------------------------------------------------------------

int __fastcall TFrmMap::IsNearPoint(int X, int Y)
{
   int Res=-1;
    for(unsigned int i=0;i<Points.size();i++)
       if ((abs(Points[i].X-X)<Epsilon)&&(abs(Points[i].Y-Y)<Epsilon))
       {
         Res=Points[i].Id;
         return Res;
       };
    return Res;
}
//----------------------------------------------------------------------------

void __fastcall TFrmMap::FillHint(int Id_Point, string sSQL)
{
    ShowQry.SetSqlText(sSQL.data());
    ShowQry.Exec("Строим список");
    CurHintTempl=sSQL.substr(7, sSQL.find("from", 0)-7);
    AnsiString Hint="";
    int begin;
    int end=CurHintTempl.find(",", 0);
    int beg=0;
    while (ShowQry.NotEof())
    {
        if (Id_Point==ShowQry.GetFldInt(0))
        {
           int i=0;
           do
           {
              begin=end+1;
              end=CurHintTempl.find(",", begin);
              beg=CurHintTempl.find("as ", begin)+3;
              Hint+=(AnsiString)(CurHintTempl.substr(max(begin,beg), end-max(begin,beg))).data()+" - "+ShowQry.GetFldStr(++i)+"\n";
           } while (end>0);
           im->Hint=Hint.SetLength(Hint.Length()-1);
           return;
        }
        else ShowQry.Next();
    }
}
//----------------------------------------------------------------------------

void __fastcall TFrmMap::ActShowStatExecute(TObject *Sender)
{
    if (ShowStatistic)
      tbQuery->Down=ShowStatistic=false;
    else
      tbQuery->Down=ShowStatistic=true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMap::Add(int X, int Y)
{
     if (AskQuestion("Вы уверены что расположение \n   указано правильно?")==ID_YES)
     {
      AmsQuery SelQ;
        if (type==0)
        {
          SelQ.SetPLText("select *.PointSeq.nextval into :P4 from DUAL"
                         "insert into *.point(id, CoordX, CoordY, id_ref) values(:P4, :P1, :P2, :P3);commit;");
          SelQ.Exec("");
          SelQ.DeclVarInt("P1", X);
          SelQ.DeclVarInt("P2", Y);
          SelQ.DeclVarInt("P3", IdRegion);
          SelQ.DeclVarInt("P4", PntSeq);
          PntSeq=SelQ.GetVarInt("P4");
          AmsIn->SetDefaultVal(PntSeq);
        }
        else
        {
          SelQ.DeclVarInt("P1", X);
          SelQ.DeclVarInt("P2", Y);
          SelQ.DeclVarInt("P3", type);

          SelQ.SetSqlText("BEGIN update *.point set CoordX=:P1, CoordY=:P2 where id=:P3;commit;END;");
          SelQ.Exec("");
        }
     }
}
//----------------------------------------------------------------------------

void __fastcall TFrmMap::ActGoBackExecute(TObject *Sender)
{
   AddToHist=false;
   CurMapHistPos--;
   if (CurMapHistPos>0)
      tv->Items->Item[tvGetTreeNodeIndex(MapHist[CurMapHistPos])]->Selected=true;
   else
   {
      Message("Достигнут конец списка");
      CurMapHistPos=1;
   };
   AddToHist=true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMap::ActGoStraightExecute(TObject *Sender)
{
   AddToHist=false;
   CurMapHistPos++;
   if (CurMapHistPos<(int)MapHist.size()+1)
      tv->Items->Item[tvGetTreeNodeIndex(MapHist[CurMapHistPos])]->Selected=true;
   else
   {
       Message("Достигнут конец списка");
       CurMapHistPos=MapHist.size();
   };
   AddToHist=true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMap::HintShow(int X, int Y)
{
      CurColor=GetPixel(im->Canvas->Handle, X, Y);
      for(int i=0;i<tv->Items->Count; i++)
      if ((unsigned int)((TTreeData*)(tv->Items->Item[i]->Data))->color==(unsigned int)CurColor)
      {
         sb->Panels->Items[1]->Text=((TTreeData*)(tv->Items->Item[i]->Data))->name;
         return;
      }
      if (CurColor==16694802)
      {
         sb->Panels->Items[1]->Text="Пока еще не Украина";
      };
}
//---------------------------------------------------------------------------

int __fastcall TFrmMap::tvGetTreeNodeIndex(TTreeNode* td)
{
      for(int i=0;i<tv->Items->Count; i++)
        if (tv->Items->Item[i]==td) return i;
}
//---------------------------------------------------------------------------

int __fastcall TFrmMap::FindMainPntCoord(TColor cl, AnsiString s)
{
        for(unsigned int i=0;i<Points.size();i++)
        {
           if ((TColor)GetPixel(im->Canvas->Handle, Points[i].X, Points[i].Y)==cl)
           {
               im->Canvas->Brush->Color = cl;
               im->Canvas->TextOutA(Points[i].X-5, Points[i].Y-5, s);
               return i;
           };
        };
        return -1;
}
//---------------------------------------------------------------------------

//Вычисляем суммарный показатель по региону
int __fastcall TFrmMap::CalcRegionData(int color)
{
   AmsQuery Q;
   Q.SetSqlText("select  sum(СУММА) "
               "from *.АН_ПОКУП А, *.point P, *.ПОКУПАТ Б, *.region R, *.op_items O "
               "where    А.aver=Б.ver and А.aarticle=Б.article "
               "and Б.М_РАСП=P.id and R.id=P.id_ref and O.item_sum_id=А.sumid and R.Color=:Color");
   Q.DeclVarInt("Color", color);
   Q.Exec("Суммируем показатели по областям");
   return Q.GetFldInt(0);
}

//------------------------------------------------------------
int __fastcall TFrmMap::FindPoint(int Id_)
{
   AmsQuery Q;
   Q.SetSqlText("select  R.Id, P.CoordX, P.CoordY "
               "from *.point P, *.region R "
               "where  R.id=P.id_ref and P.id=:ID");
   Q.DeclVarInt("ID", Id_);
   Q.Exec("Ищем область и координаты поставщика");
   int IdRegion=Q.GetFldInt(0);
      for(int i=0;i<tv->Items->Count; i++)
        if (((TTreeData*)(tv->Items->Item[i]->Data))->id==IdRegion)
        {
            tv->Items->Item[i]->Selected=true;
              im->Canvas->Brush->Color = clBlue;
              int Radius=5;
              im->Canvas->Ellipse(Q.GetFldInt(1)-Radius,Q.GetFldInt(2)-Radius,Q.GetFldInt(1)+Radius,Q.GetFldInt(2)+Radius);
            return 1;
        }
   return -1;
}
