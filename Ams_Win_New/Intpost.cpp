#include "AMS_HDR.h"
#pragma hdrstop
#include <stdio.h>
#include <string.h>
 
enum { VS=1,VD,HS,HD,VHS,VHD,VHUS,VHUD,VHDS,VHDD,VHLAST };

static void xlat(char *a)
{
 static char *t,*pat="³ºÄÍ",*altpat="|!-=";
 for(;*a;a++)
  if(*a=='\\')
  {
   t=strchr(pat,a[1]);
   if(t) *a=t-pat+VS;
   else
   { t=strchr(altpat,a[1]);
     if(t) *a=t-altpat+VS;
     else continue;
   }
   strcpy(a+1,a+2);
  }
 *(a-1)=0;
}

static void unxlat(char *a)
{
 static char boxch[]="³ºÄÍÅÎÁÊÂË";
 for(;*a;a++)
  if(*a>=VS && *a<VHLAST) *a=boxch[*a-VS];
 *a++='\n'; *a=0;
}

static void cornlat(char *a)
{
  static char boxch[]  = "ÅÎÁÊÂË";
  static char corn[][7] ={"ÃÌÀÈÚÉ",
			 "´¹Ù¼¿»"};
  char *t;
  for(int i=0;i<2;i++)
  {
   if((t=strpbrk(a,boxch))!=0)
   {
    *t=corn[i][int(strchr(boxch,*t)-boxch)];
   }
   strrev(a);
  }
}

/*
extern char    *set_underline_chars ;
extern int     set_underline_num ;

extern char    *clear_underline_chars ;
extern int     clear_underline_num ;

extern char    *set_8lpi_chars ;
extern int     set_8lpi_num ;

extern char    *clear_8lpi_chars ;
extern int     clear_8lpi_num ;

extern char    *set_cond_chars ;
extern int     set_cond_num ;

extern char    *clear_cond_chars ;
extern int     clear_cond_num ;

extern char    *portrait_chars ;
extern int     portrait_num ;

extern char    *landscape_chars ;
extern int     landscape_num ;

extern char    *init_chars;
extern int     init_num;

extern char    *set_micro_chars;
extern int     set_micro_num;

extern char    *clear_micro_chars;
extern int     clear_micro_num;

static void ParsePrintCodes(char *a)
{
 char *t;
 char *code;
 int  codelen;

 for(t=a;*t;t++)
 {
  if(t[0]=='\\' && t[1]=='P')
  {
   code=0;
   switch(t[2])
   {
     case 'U':
         code=set_underline_chars;
         codelen=set_underline_num;
         break;
     case 'u':
         code=clear_underline_chars;
         codelen=clear_underline_num;
         break;
     case 'D':
         code=set_cond_chars;
         codelen=set_cond_num;
         break;
     case 'd':
         code=clear_cond_chars;
         codelen=clear_cond_num;
         break;
     case '8':
         code=set_8lpi_chars;
         codelen=set_8lpi_num;
         break;
     case '6':
         code=clear_8lpi_chars;
         codelen=clear_8lpi_num;
         break;
     case 'P':
         code=portrait_chars;
         codelen=portrait_num;
         break;
     case 'L':
         code=landscape_chars;
         codelen=landscape_num;
         break;
     case 'I':
         code=init_chars;
         codelen=init_num;
         break;
     case 'M':
         code=set_micro_chars;
         codelen=set_micro_num;
         break;
     case 'm':
         code=clear_micro_chars;
         codelen=clear_micro_num;
         break;
   }
   if(code)
   {
     memmove(t+codelen,t+3,strlen(t+3)+1);
     if(codelen) memcpy(t,code,codelen);
   }
  }
 }
}
*/

#define MAXSTR	2048
#define MAXCOLUMN 10

int Skip2Column(FILE *in,char *str,long *apos,int *alines)
{
 int i,col=0;
 *apos=ftell(in);
 for(i=0;!feof(in);i++)
 {
  if (!fgets(str,MAXSTR,in)) break ;
  if (str[0]=='\\' && (str[1]=='C'|| str[1]=='E'))
  {
   alines[col++]=i;
   apos[col]=ftell(in);
   i=-1;
   if(str[1]=='E')   break;
  }
 }
 if(!col)
 {
  alines[0]=i;
  apos[1]=ftell(in);
  col=1;
 }
 return col;
}

int PostProc(char *in,char *out,int convert)
{
 FILE *inf=fopen(in,"rt"),*outf=fopen("_temp","wt");
 if(!inf || !outf) return 1;
 char str[MAXSTR+1],vstr[MAXSTR+1];
 int i,intab=0,vtab=0,htab,hsave;

 long cpos[MAXCOLUMN];
 int lines[MAXCOLUMN],col;

 while(!feof(inf)) // column merging
 {
  if (!fgets(str,MAXSTR,inf)) break ;
  if (str[0]=='\\' && str[1]=='C')
  {
   col=Skip2Column(inf,str,cpos,lines);
   int line,emp=0;
   for(line=0;!emp;line++)
   {
    emp=1;
    str[0]='\n';
    str[1]=0;
    for(i=0;i<col;i++)
    {
     if(line<lines[i])
     {
      emp=0;
      fseek(inf,cpos[i],SEEK_SET);
      fgets(str+strlen(str)-1,MAXSTR-strlen(str),inf);
      cpos[i]=ftell(inf);
     }
    }
    if(!emp) fputs(str,outf);
   }
   fseek(inf,cpos[col],SEEK_SET);
  }
  else
  {
   void My_OemToChar(char const *s,char *d) ;
   if (convert)
      My_OemToChar(str,str) ;
   fputs(str,outf);
  }
 }

 outf=freopen(out,"wt",outf);
 inf=freopen("_temp","rt",inf);

 while(!feof(inf)) // box-drawing
 {
  if (!fgets(str,MAXSTR,inf)) break ;
  xlat(str);

  if(str[0]==HS || str[0]==HD)
  { hsave=str[0];
    if(!intab)
    {
     intab=1; vtab=1; strcpy(vstr,str);
     continue;
    }
    else
    {
     if(vtab) // close table
     {
      for(i=0;vstr[i];i++)
       switch(vstr[i]) {
      	case VHS: vstr[i]=VHUS; break;
	      case VHD: vstr[i]=VHUD; break;
       }
      vtab=0; intab=0;
     }
     else
     {
      char c=str[0];
      for(i=0;vstr[i];i++)
   	str[i]=(vstr[i]==VS)?VHUS:(vstr[i]==VD)?VHUD:c;
      str[i]=0;
      strcpy(vstr,str);
      vtab=1;
     }
     continue;
    }
  }
  htab=0;
  for(i=0;str[i];i++)
     if(str[i]==VS || str[i]==VD)
	if(htab) { strcpy(str+i-1,str+i); i--; }
	else htab=1;
      else htab=0;
   if(vtab)
   {
    for(i=0;vstr[i];i++)
	   switch(str[i]) {
	   case VS: vstr[i]=vstr[i]==VHUS?VHS:vstr[i]==HS?VHDS:vstr[i]; break;
	   case VD: vstr[i]=vstr[i]==VHUD?VHD:vstr[i]==HD?VHDD:vstr[i]; break;
	   }
      for(;str[i];i++)
	   vstr[i]=(str[i]==VS)?VHDS:(str[i]==VD)?VHDD:hsave;
      vstr[i]=0;
      unxlat(vstr);
      cornlat(vstr);
      fputs(vstr,outf);
      vtab=0;
   }
   strcpy(vstr,str);
   unxlat(str);
//   ParsePrintCodes(str);
   fputs(str,outf);
 }
 fclose(inf);
 fclose(outf);
 return 0;
}
/*
void main(int argc,char **argv)
{
 PostProc(argv[1],"!.doc");
} */