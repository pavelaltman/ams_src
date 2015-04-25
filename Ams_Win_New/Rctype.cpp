#include "AMS_HDR.h"
#pragma hdrstop

//#include <libdefs.h>
#include <ctype.h>

void InitCType()
{
  unsigned char i;
  unsigned char A = 'À';
  unsigned char a = 'à';
  unsigned char YA = 'ß';
  unsigned char ya = 'ÿ';

  for(i=A;i<=YA;i++)
    _chartype[i+1]=_IS_UPP | _IS_ALPHA;

  for(i=a;(i<=ya) && (i!=0);i++)
    _chartype[i+1]=_IS_LOW | _IS_ALPHA;
}

int rus_toupper(int ch)
{
 if (0<=ch && ch<128) return toupper(ch) ;
 if (ch>='à' && ch<='ÿ') return ch-'à'+'À';
 return ch ;
}

char *rus_strupr(char *str)
{
 char *p ;
 for(p=str ; *p ; p++) *p=rus_toupper(*p) ;
 return str ;
}

void rus_strupra(AnsiString& Str)
{
    int K = Str.Length();
    for (int i = 1 ; i<= K ; i++)
        Str[i] = rus_toupper(Str[i]);
}
