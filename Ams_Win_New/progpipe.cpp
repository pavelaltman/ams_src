#include "AMS_HDR.h"
#pragma hdrstop
#include "accs.h"

extern int To1251;

int ProgFile::Open(ProgPos &PP)
{
 tail=0;
 PP.file=fname;
 PP.line=0;
 //void My_OemToChar(char const *,char *);
 //My_OemToChar(PP.file.data(),fname);
 return (ifile=fopen(fname,"rt"))!=NULL;
}

int ProgFile::GetLine(char *line,ProgPos &PP)
{
 *line=0;
 char *t=line;
 //bool Converted = false;
 while(1)
 {
  if(!tail)
  {

    if(fgets(t,MAXSTMTLEN-strlen(line),ifile)==NULL)
    {
        if(!*line) return 0;
    }
    if (To1251)
    {
        My_OemToChar(t,t);
    }
    PP.line++;
  }
  else
  {
   strcpy(line,tail);
   tail=0;
  }
  t=strchr(line,'\n');
  if(t) *t=0;
  t=strstr(line,"//");
  if(t) *t=0;

  t=strstr(line,"/*");
  if(t)
  {
   char *t1=strstr(t,"*/") ;
   if (t1)
      memmove(t,t1+2,strlen(t1+2)+1) ;
   else
   {
    while(1)
    {
     char t2[MAXSTMTLEN+1] ;
     if(fgets(t2,MAXSTMTLEN,ifile)==NULL)
        return 0;
     PP.line++;

     char *t3=strchr(t2,'\n');
     if(t3) *t3=0;

     t1=strstr(t2,"*/") ;
     if (t1)
     {
      memmove(t,t1+2,strlen(t1+2)+1) ;
      break ;
     }
    }
   }
//   *t=0;
  }


  t=strrchr(line,';');

  if(!t)
  {
    if (To1251)
    {
      //My_OemToChar(line, line);
    }
    return 1;
  }

  char *w=t+1;
  for(;*w && isspace(*w);w++);
//  if(*w) { *t=0; tail=t+1; return 1; }
  if(*w) return 1;
 }
}

int ProgMem::GetLine(char *line,ProgPos &PP)
{
 PP.line=count;
 if(count<strs.size())
 {
  strcpy(line,strs[count++].data());
  return 1;
 }
 return 0;
}

int ProgStr::GetLine(char *line,ProgPos &PP)
{
  if(!*c) return 0;
  const char *e=strchr(c,';');
  if(!e)
  {
   strcpy(line,c);
   c+=strlen(c);
  }
  else
  {
    strncpy(line,c,e-c);
    line[e-c]=0;
    c=e+1;
  }
  PP.line++;
  return 1;
}
