/*****                            sumtext.c                             *****/

#define SUMBUFLEN    25

extern  char
                 *Names_l2[][2][4][2] ,
                 *Names_l20[][17] ,
                 *Names_l100[][8] ,
                 *Names_l1000[][9] ,
                 *Names_m[][3][3] ,
                 Name_usd[] ,
                 *Names_usd[] ,
                 *Short_names_grn[] ,
                 *Names_grv[][3] ,
                 *Names_krb[] ;

int  rus_toupper(int) ;
char *strdelchar(char *str,int ch,char *buf) ;
    
extern "C" {
void sprintf(char *,...) ;
char *strcat(char *dest, const char *src);
int  strlen(char*) ;
void *malloc(long size) ;
void free(void *) ;
}

static char *Result,memorized,lastnum,low_num;

void AddNumber(int n,int p,int order,int gender,int lang)
{
  int addspace=0;
  switch(p) {
   case 0: if(n) { addspace=1; strcat(Result,Names_l1000[lang][n-1]);}
           lastnum=0 ;
           break;
   case 1: if(n>=2)
             { strcat(Result,Names_l100[lang][n-2]); addspace=1; memorized=0;}
           else  {memorized=n;}
           break;
   case 2: if(memorized || n)
           {
            addspace=1;
            if(!memorized && n<=2)
              { strcat(Result,Names_l2[lang][gender][order][n-1]); lastnum=n; break; }
            lastnum=memorized*10+n;
            strcat(Result,Names_l20[lang][lastnum-3]);
           }
  }
  if(addspace) strcat(Result," ");
}

char *Convert1000(char *String,int order,int gender,int lang)
{
  int i=0;
  *Result=0;

  for(i=0;i<3;i++) AddNumber(String[i]-'0',i,order,gender,lang);

  if(*Result)
  { if(order)
      {
         strcat(Result,Names_m[lang][order-1][lastnum==1?0:
                               (lastnum && lastnum<=4)?1:2]);
         strcat(Result," ");
      }
    else low_num=lastnum ;
  }
  return Result;
} /* Convert1000 */


void ConvertSum(char *dst,char *src,int usd,int with_num,int lang,int short_grn)
{
 int i;
 char str[SUMBUFLEN],buf[SUMBUFLEN],*s;

 Result=(char*)malloc(500) ;
 *dst=0;
 low_num=5 ;
 strdelchar(src,'\'',buf) ;
 sprintf(str,"%12s",buf);
 for(s=str;*s==' ';s++) *s='0';
 if (with_num) sprintf(dst,"%s (",buf) ;
 for(i=0;i<4;i++)
   strcat(dst,Convert1000(str+i*3,3-i,!usd,lang));
 *dst=rus_toupper(*dst) ;
 if (with_num)
 {
  int i=strlen(dst)-1 ;
  for( ; dst[i]==' ' ; i-- ) dst[i]=0 ;
  strcat(dst,") ") ;
 }
 if (usd==1) strcat(dst,Name_usd) ;
 else if (usd==2)
 {
  strcat(dst,Names_usd[low_num==1?0:(low_num && low_num<=4)?1:2]) ;
 }
 else if (short_grn) strcat(dst,Short_names_grn[lang]) ;
 else strcat(dst,Names_grv[lang][low_num==1?0:(low_num && low_num<=4)?1:2]) ;
 free(Result) ;
}

void ConvertSumExt(char *dst,char *src,int with_num,int lang,int gender
  ,char const *cur_name)
{
 int i;
 char str[SUMBUFLEN],buf[SUMBUFLEN],*s;

 Result=(char*)malloc(500) ;
 *dst=0;
 low_num=5 ;
 strdelchar(src,'\'',buf) ;
 sprintf(str,"%12s",buf);
 for(s=str;*s==' ';s++) *s='0';
 if (with_num) sprintf(dst,"%s (",buf) ;
 for(i=0;i<4;i++)
   strcat(dst,Convert1000(str+i*3,3-i,gender,lang));
 *dst=rus_toupper(*dst) ;
 if (with_num)
 {
  int i=strlen(dst)-1 ;
  for( ; dst[i]==' ' ; i-- ) dst[i]=0 ;
  strcat(dst,") ") ;
 }
 strcat(dst,cur_name) ;
 free(Result) ;
}
