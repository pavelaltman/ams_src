#include "AMS_HDR.h"
#pragma hdrstop
#include "sum.h"
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

extern r_char badsummes,toomuchsummes,nothingtocopymes,resultcopiedmes,
       storebutton,calculbutton,bufbutton,bufemptymes,sumbuftitle;


int	SUM_LEN=14 ;
int	SUMLEN=13 ;
int   show_dig=2 ;
int   apostrophe=1 ;

Sum Sum::Pval(int side)
{
 Sum retsum ;
 if (side==DEBET)
   { if (IsPos()) retsum=*this ; }
 else
   { if (IsNeg()) { retsum=*this ; retsum.Neg() ; } }
 return retsum ;
}

void Sum::Round(int r)
{
   int sign=1 ;
   long double v=dlv ;
   if (v<0)
   {
    v=-v ;
    sign=-1 ;
   }
   long double p10,vp10;
   p10=pow10l(r);
   vp10=v*p10;
   if(fmodl(vp10,1)>=.5) v=ceill(vp10)/p10;
   else v=floorl(vp10)/p10;
   *this=v*sign ;
}

#pragma argsused
int Sum::SetStr(char *str,int side,char **stop,int nominus,int allow_dig,int no_rest)
{
 int t=0;
 char *s;
 dlv=0 ;

 s=str ;
 if (*s=='-') { s++; t++; }
 for(; isdigit(*s) || *s=='\'' ;s++)
 {
  if (*s=='\'') continue ;
  dlv*=10;
  dlv+=(*s-'0');
 }

 if (dlv>1e13) return SS_TOOBIG ;

 if ((nominus ? 0 : *s=='-') || *s=='.')
 {
  s++ ;
  if (isdigit(*s))
  {
   long double div=10 ;
   int d ;
   for(d=0 ; isdigit(*s) ; s++ , div*=10 , d++)
   {
    dlv+=(*s-'0')/div ;
   }
   if (d>allow_dig) return SS_BAD ;
  }
  else s-- ;
 }

 if (stop) *stop=s ;

 if (no_rest && *s)
 {
  if (*s==' ')
    return SS_OK ;
  return SS_BAD ;
 }

 if(t) Neg();
 if (side==CREDIT) Neg() ;
 return SS_OK ;
}

char *Sum::GetStr(char *str,unsigned flags,int showdig,int pchar)
{
 if (showdig==-1) showdig=show_dig ;

 char full_str[30] ;
 long double floo=floorl(dlv) ;
 long double rest=dlv-floo ;
 sprintf(full_str,"%Lg",rest) ;
 if (!(flags&GS_NOFULL))
 {
  int full_digs=strlen(full_str)-2 ;
  if (showdig<full_digs) showdig=full_digs ;
 }

 sprintf(str,"%.*Lf",showdig,dlv) ;

 if ((flags&GS_NOZERO) && dlv==0)
 {
  *str=0 ;
 }

 if (pchar!='.')
  if (char *p=strchr(str,'.')) *p=pchar ;

 int    l=strlen(str) ;

 if (apostrophe && !(flags&GS_NOAPPS))
  for(int apos=l-(showdig ? showdig : -1)-4 ; apos>(dlv<0) ; apos-=3)
  {
   memmove(str+apos+1,str+apos,l-apos+1) ;
   str[apos]='\'' ;
   l++ ;
  }

 return str ;
}

int Sum::SetStrMes(char *str,int side,char **stop,int nominus,int allow_dig,int no_rest)
{
 switch(SetStr(str,side,stop,nominus,allow_dig,no_rest))
 {
  case SS_BAD :
         message(badsummes,BAD) ;
         return 0 ;
  case SS_TOOBIG :
         message(toomuchsummes,BAD) ;
         return 0 ;
 }
 return 1 ;
}


