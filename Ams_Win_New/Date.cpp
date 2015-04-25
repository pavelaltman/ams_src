#include "AMS_HDR.h"
#pragma hdrstop
#include <stdio.h>
#include <string.h>
#include <dos.h>
#include "datetime.h"
#include "comisc.h"
//#include "libdefs.h"

extern	char	*month_names[][12] , *month_names_im[][12] ;
extern   char  *yearstr[] , *yearstr_im[];

typedef char r_char[] ;
extern r_char kvart_cap , polugod_cap , ninemonths_cap ;

void DateTime::Set(int d,int m,int y)
{
 Zero();
 unsigned short D,M,Y;
 D=d>31?31:d;
 M=m>12?12:m;
 Y=y;
 if (Y< 117)
 {
    if (Y<17) Y+=2000;
    else
        Y+=1900;
 }
 DTVal=EncodeDate (Y,M,D);
}

char  *DateTime::Out(char *buf,int bs,char *fmt) const
{
 if ( (int)DTVal == 0 ) *buf=0 ;
 else if (FYear()==0 || Day()==0) *buf=0 ;
 else
 {
    AnsiString Form= fmt;
    int i = Form.Pos("%d");
    if (i)
    {
        Form[i]='d';
        Form[i+1]='d';
    }
    i = Form.Pos("%m");
    if (i)
    {
        Form[i]='m';
        Form[i+1]='m';
    }
    i = Form.Pos("%y");
    if (i)
    {
        Form[i]='y';
        Form[i+1]='y';
    }
    i = Form.Pos("%Y");
    if (i)
    {
        Form[i]='y';
        Form[i+1]='y';
        Form.Insert("yy", i+1);
    }
    i = Form.Pos("%H");
    if (i)
    {
        Form[i]='h';
        Form[i+1]='h';
    }
    i = Form.Pos("%M");
    if (i)
    {
        Form[i]='n';
        Form[i+1]='n';
    }

    AnsiString S = FormatDateTime(Form,DTVal);
    strcpy (buf, S.c_str());
 }
 return buf;
}

int DateTime::SetStr(const char *str)
{
 Zero() ;
 if (strempty((char *)str)) return 1 ;

 unsigned d,m,y ;
 if (sscanf(str,"%u-%u-%u",&d,&m,&y)!=3)
  if (sscanf(str,"%u.%u.%u",&d,&m,&y)!=3) return 0 ;

 y%=100;

 if (y>99) return 0 ;
 if (y<17) y+=2000 ;
 else
    y+= 1900;

 if (d<1 || d>31) return 0 ;
 if (m<1 || m>12) return 0 ;
 //if (y>117) return 0 ;

 Set(d,m,y) ;

 return 1 ;
}

int DateTime::SetStr(const char *str,int y)
{
 unsigned d,m ;
 if (sscanf(str,"%u-%u",&d,&m)!=2) return 0 ;

 if (y<70) y+=2000 ;
 else
    y+=1900;

 if (d<1 || d>31) return 0 ;
 if (m<1 || m>12) return 0 ;

 Set(d,m,y) ;

 return 1 ;
}

int DateTime::SetStr(const char *str,int m,int y)
{
 unsigned d ;
 if (sscanf(str,"%u",&d)!=1) return 0 ;

 if (y<70) y+=2000 ;
 else
    y+=1900;

 if (d<1 || d>31) return 0 ;
 if (m<1 || m>12) return 0 ;

 Set(d,m,y) ;
 return 1 ;
}

/*
char *DateTime::GetStr(char *buf)
{
 sprintf(buf,"%u-%u-%u",tm_day,tm_month,year+80) ;
 return buf ;
}

char *Date::GetStr2(char *buf)
{
 sprintf(buf,"%u-%u",day,month) ;
 return buf ;
}
*/
char *DateTime::GetTextStr(char *buf,int lang,int date_quote)
{
 sprintf(buf,date_quote ? "\"%d\" %s %d %s" : "%d %s %d %s",Day(),month_names[lang][Month()-1],FYear(),yearstr[lang]) ;
 return buf ;
}

char *DateTime::GetMonStr(char *buf,int no_year,int lang)
{
 if (no_year) sprintf(buf,"%s",month_names_im[lang][Month()-1]) ;
 else sprintf(buf,"%s %d %s",month_names_im[lang][Month()-1],FYear(),yearstr[lang]) ;
 return buf ;
}

char* ninemonths_arr[]  = { "9 мес€цев", "9 мiс€цiв"    };
char* polugod_arr[]     = { "полугодие", "пiврiчч€"     };


char *DateTime::IntervalStr(char *buf,DateTime const &to,int lang,int no_year)
{
 if(Month()==1 && to.Month()==1 && Day()==1 && to.Day()==1)
 {
   if(FYear()==to.FYear()-1) sprintf(buf,"%d %s",FYear(),yearstr_im[lang]) ;
   else sprintf(buf,"%02d-%02d %s",Year(),(to.Year()-1)%100,yearstr_im[lang]) ;
   return buf;
 }
 DateTime to1=to.PrevDay();
 if(FYear()==to1.FYear())
 {
   if(Day()==1 && to.Day()==1)
   {
      if(Month()==to1.Month())
        sprintf(buf,"%s",month_names_im[lang][Month()-1]) ;
      else if (Month()==1 && to1.Month()==3) sprintf(buf,"1 %s",kvart_cap) ;
      else if (Month()==4 && to1.Month()==6) sprintf(buf,"2 %s",kvart_cap) ;
      else if (Month()==7 && to1.Month()==9) sprintf(buf,"3 %s",kvart_cap) ;
      else if (Month()==10 && to1.Month()==12) sprintf(buf,"4 %s",kvart_cap) ;
      else if (Month()==1 && to1.Month()==6) sprintf(buf,"1 %s",polugod_arr[lang]) ;
      else if (Month()==1 && to1.Month()==9) sprintf(buf,"%s",ninemonths_arr[lang]) ;
      else
        sprintf(buf,"%s-%s",month_names_im[lang][Month()-1],month_names_im[lang][to1.Month()-1]) ;
   }
   else
   {
      if(Month()==to.Month()-1)
        sprintf(buf,"%d-%d %s",Day(),to.Day(),month_names[lang][Month()-1]) ;
      else
        sprintf(buf,"%d %s - %d %s",Day(),month_names[lang][Month()-1],to.Day(),month_names[lang][to.Month()-1]) ;
   }
   if(!no_year)
    sprintf(buf+strlen(buf)," %02d %s",FYear(),yearstr[lang]) ;
 }
 else
 {
   if(Day()==1 && to.Day()==1)
     sprintf(buf,"%s %d - %s %d %s",month_names_im[lang][Month()-1],FYear(),month_names_im[lang][to1.Month()-1],to1.FYear(),yearstr[lang]) ;
   else
	 sprintf(buf,"%d %s %d - %d %s %d %s",Day(),month_names[lang][Month()-1],FYear(),to.Day(),month_names[lang][to.Month()-1],to.FYear(),yearstr[lang]) ;
 }
 return buf;
}

DateTime DateTime::ChMonth(int pmn)
{
 unsigned short y,m,d;
 DTVal.DecodeDate(&y,&m,&d);
 int Y = y;
 int D = d;
 int M = m;
 M=M-1;
 M+=pmn;
 if (M<0)
 {
    Y-=(-M)/12+1 ;
    M+=((-M)/12+1)*12 ;
 }
 DateTime SS ;
 SS.Set(D,M+1,Y);
 return SS;
}

