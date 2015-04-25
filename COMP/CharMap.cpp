//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "CharMap.h"
#include <printers.hpp>
#include <stdio.h>
#include <math.h>
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TCharMap *)
{
  new TCharMap(NULL);
}
//---------------------------------------------------------------------------
__fastcall TCharMap::TCharMap(TComponent* Owner)
  : TCustomControl(Owner)
{
  Str=NULL;
  MustRedraw=false;
  MustRecalc=true;
  MaxLineWidth=0;
}
//---------------------------------------------------------------------------
namespace Charmap
{
  void __fastcall PACKAGE Register()
  {
     //TComponentClass classes[1] = {__classid(TCharMap)};
     //RegisterComponents("Samples", classes, 0);
  }
}
//---------------------------------------------------------------------------
void __fastcall TCharMap::SetFontSize(int I)
{
  Canvas->Font->Size=I;
  MustRecalc=true;
  Refresh();
};
//---------------------------------------------------------------------------
void TCharMap::SetFile(const char* FileName)
{

  MustRedraw=false;
  if (Str) delete Str;
  Str=new TStringList;
  FILE* F= fopen(FileName, "rt");
  if (!F) return;
  char Buf[5000];
  char Buf1[5000];
  char* p="";
  EndPage.clear();
  MaxLineWidth=0;
  while (p)
  {
    p=fgets(Buf, 4999, F);
    if (p)
    {
      int K=strlen(p);
      if (p[K-1]=='\n')
      {
       p[K-1]=0;
       K--;
      };

      // Замена символов табуляции на 8 пробелов
      char *S="";
      char *S1=Buf1;
      *Buf1=0;
      while (S)
      {
        S=strpbrk(p, "\t");
        if (S)
        {
          *S=0;
          strcpy(S1, p);
          int L=S-p;
          while ((L+1)%8 !=0)
           S1[L++]=' ';
          S1[L]=0;
          S1=&S1[L];
          p=S+1;
        }
      }
      char* EndP=&p[K];
      if ( *Buf1!=0)
      {
        strcpy(S1,p);
        int XX= strlen(p);
        p=Buf1;
        EndP=S1+XX;
      }

      while (p<EndP)
      {
        // Есть ли в строке символ прогона формата или табуляции
        char* FF=strpbrk(p, "\f");
        if (FF)
        {
          if (FF[0]=='\t')
          {
            FF[0]=0;
            Str->Add(p);

            if ((FF-p)>MaxLineWidth) MaxLineWidth=FF-p;
            p=FF+1;
            EndPage.push_back(Str->Count);
          }
          else
          {
            FF[0]=0;
            Str->Add(p);
            if ((FF-p)>MaxLineWidth) MaxLineWidth=FF-p;
            p=FF+1;
            EndPage.push_back(Str->Count);
          }
        }
        else
        {
          Str->Add(p);
          int XX= EndP-p;
          if (XX>MaxLineWidth)
            MaxLineWidth=XX;
          p=EndP;
        };
      }
    } // Считали строку из файла
  }// Цикл пo строкам файла
  fclose(F);
  MustRecalc=true;
  MustRedraw=true;
  Recalc();
  Refresh();
};
//---------------------------------------------------------------------------
void TCharMap::Print(const AnsiString& JobTitle)
{
  if (MustRedraw && Str)
  {
    if (MustRecalc) Recalc();
    TPrinter* Prn=Printer();
    Prn->Title="AMS "+JobTitle;
    Prn->BeginDoc();
    Prn->Canvas->Font->Name="Lucida Console";
    Prn->Canvas->Font->Charset=255;
    Prn->Canvas->Font->Size=Canvas->Font->Size;
    int TH= Prn->Canvas->Font->Height;
    if (TH<0) TH= -TH+1;

    for (int LASY=0; LASY<CurrWidth; LASY++)
    {

    int LineInPage=0;   // Номер строки внутри страницы
    int Y=pSpaceTop;
    unsigned int CurrHrdPgBrk=0; // Номер обработанного конца страницы
    bool PgBrk=false;

    // Прохожу по всем строкам
    for (int i =0; i< Str->Count;i++)
    {
      int X=pSpaceLeft;
      // Для всех страниц
      int PrevStop=0;
      int NextStop=CharPerLine;
      for (int j =0;j<CurrWidth;j++)
      {
        if (LASY==j)
        {
           AnsiString A = Str->Strings[i].SubString(PrevStop,NextStop-PrevStop);
           Prn->Canvas->TextOut(X,Y, A);
        }
        PrevStop=NextStop;
        NextStop+=CharPerLine;
        //X+=pPageWidth;
      }
      Y+=TH;
      LineInPage++;
      // Проверка на окончание страницы в высоту
      if (LineInPage>=LinePerPage) PgBrk=true;
      if ((CurrHrdPgBrk<EndPage.size())&&(EndPage[CurrHrdPgBrk]==LineInPage))
      {
        CurrHrdPgBrk++;
        PgBrk=true;
      };
      if (PgBrk)
      {
        Y=pSpaceTop;
        LineInPage=0;
        PgBrk=false;
        Prn->NewPage();
      };
    }// Все строки из Str
  } //LASY
  Prn->EndDoc();
  }// Если отрисовываем

};
//---------------------------------------------------------------------------
void TCharMap::Recalc()
{
  Canvas->Font->Charset=255;
  Canvas->Font->Name="Lucida Console";
  LineH=Canvas->Font->Height;// Высота Строки в пикселах
  if (LineH<0) LineH=-LineH;
  int W= Canvas->TextWidth("W");

  TPrinter* P= Printer();
  P->Canvas->Font->Charset=255;
  P->Canvas->Font->Name="Lucida Console";
  P->Canvas->Font->Size = Canvas->Font->Size;
  int LineHP=P->Canvas->Font->Height;// Высота Строки в пикселах
  if (LineHP<0) LineHP=-LineHP+1;
  int WP= P->Canvas->TextWidth("W");

  pPageWidth= P->PageWidth;  // Ширина страницы в пикселах (Экранных)
  pPageHeight=P->PageHeight; // Высота страницы в пикселах (Экранных)
  int PPIX = GetDeviceCaps(P->Handle,LOGPIXELSX);
  int PPIY = GetDeviceCaps(P->Handle,LOGPIXELSY);

  pSpaceTop=pSpaceTopMM*PPIY/25.4;       // Отступ сверху
  pSpaceBottom=pSpaceBottomMM*PPIY/25.4; // Отступ снизу
  pSpaceLeft=pSpaceLeftMM*PPIX/25.4;     // Отступ слева
  pSpaceRight=pSpaceRightMM*PPIX/25.4;   // Отступ справа

  CharPerLine= (pPageWidth-pSpaceLeft-pSpaceRight)/WP;      // Ширина страницы в символах
  LinePerPage= (pPageHeight-pSpaceTop-pSpaceBottom)/LineHP; // Количество строк в 1 странице

  float KoefHor = W;
  KoefHor/=WP;
  float KoefVert= LineH;
  KoefVert/=LineHP;
  pSpaceTop=pSpaceTop * KoefVert;        // Отступ сверху
  pSpaceBottom=pSpaceBottom * KoefVert;  // Отступ снизу
  pSpaceLeft=pSpaceLeft * KoefHor;       // Отступ слева
  pSpaceRight=pSpaceRight * KoefHor;     // Отступ справа


  pPageWidth = pPageWidth * W/WP;
  pPageHeight = pPageHeight * LineH/LineHP;
  CurrWidth  = ceil(((double)MaxLineWidth)/CharPerLine);   // Кво склеенных страниц по ширине
  MustRecalc=false;
}
//---------------------------------------------------------------------------
void __fastcall TCharMap::Paint(void)
{
 //
  if (MustRedraw && Str)
  {
    if (MustRecalc) Recalc();
    int Pages_=1;        // К-во страниц в высоту
    int Y=pSpaceTop;
    int PageShift=Y;    // Cмещение отн начала страницы
    int LineInPage=0;   // Номер строки внутри страницы
    unsigned int CurrHrdPgBrk=0; // Номер обработанного конца страницы
    bool PgBrk=false;

    // Прохожу по всем строкам
    for (int i =0; i< Str->Count;i++)
    {
      int X=pSpaceLeft;
      // Для всех страниц
      int PrevStop=0;
      int NextStop=CharPerLine;
      for (int j =0;j<CurrWidth;j++)
      {
        AnsiString A = Str->Strings[i].SubString(PrevStop,NextStop-PrevStop);
        Canvas->TextOut(X,Y, A);
        PrevStop=NextStop;
        NextStop+=CharPerLine;
        X+=pPageWidth;
      }
      Y+=LineH;
      LineInPage++;
      // Проверка на окончание страницы в высоту
      if (LineInPage>=LinePerPage) PgBrk=true;
      if ((CurrHrdPgBrk<EndPage.size())&&(EndPage[CurrHrdPgBrk]==LineInPage))
      {
        CurrHrdPgBrk++;
        PgBrk=true;
      };
      if (PgBrk)
      {
        PageShift+=pPageHeight;
        Y=PageShift;
        LineInPage=0;
        PgBrk=false;
        Pages_++;
      };
    }// Все строки из Str

    Canvas->Pen->Width=1;
    Canvas->Pen->Color=clGray;

    // Рисуем линии страниц
    int TotalWidth=pPageWidth*CurrWidth;
    int TotalHeight=pPageHeight*Pages_;
    Pages=Pages_;
    for (int i=pPageHeight; i<=TotalHeight; i+=pPageHeight)
    {
      Canvas->MoveTo(0,i);
      Canvas->LineTo(TotalWidth, i);
    }
    for (int i=pPageWidth; i<=TotalWidth; i+=pPageWidth)
    {
      Canvas->MoveTo(i,0);
      Canvas->LineTo(i,TotalHeight);
    }
  }// Если отрисовываем
};
//---------------------------------------------------------------------------
