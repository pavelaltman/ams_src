#include "AMS_HDR.h"
#pragma hdrstop
#include "prog.h"
#include "accs.h"

extern
r_char noclosebrackerr,badcharerr,noclosequoteerr,syntaxerr,
       badtypeerr,varnameexpectederr,commaexpectederr,
       unknownfuncerr;

//      one-character tokens
Prog::CharLex Prog::char_lex[]={                          
   { '(' , o_brack      } ,
   { ')' , c_brack      } ,
   { '=' , assgn        } ,
   { '+' , plus         } ,
   { '-' , minus        } ,
   { ',' , comma        } ,
   { '*' , aster        } ,
   { '/' , slash        } ,
   { '[' ,iter_b        },
   { ']' ,iter_e        },
   { '{' ,calc_b        },
   { '}' ,calc_e        },
   { '<' ,lss           },
   { '>' ,grt           },
   { '&' ,and           },
   { '?' ,asksign       },
   { '|' ,or            },
   { '!' ,not           },
   { '@' ,macro         },
   { '&' ,comand        },
   {  0  ,belong,  "IN"      },
   {  0  ,ret,     "RETURN"  },
   {  0  ,FOR,     "FOR"     },
   {  0  ,WHILE,   "WHILE"   },
   {  0  ,UNTIL,   "UNTIL"   },
   {  0  ,LOCAL,   "LOCAL"   },
   {  0  ,STRUCT,  "STRUCT"  },
   {  0  ,FUNCTION,"FUNCTION"},
   {  0  ,USELIB,  "LIB"     },
   {  1 , dummy        } } ;

long double RoundLD(long double v,int r)
{
 long double p10,vp10;
 p10=pow10l(r);
 vp10=v*p10;
 if(fmodl(vp10,1)>.4999999) v=ceill(vp10) ;
 else v=floorl(vp10) ;
 v=v/p10 ;

/*
 char s[50] ;
 sprintf(s,"%.6lf",v) ;
 v=_atold(s) ;
*/

 return v ;
}


int Prog::GetLex()
{
 int i ;

 while ( !isgraph(*((unsigned char*)Sp)) && *Sp) Sp++ ;

 for(i=0 ; char_lex[i].ch!='\0' ; i++)
  if (char_lex[i].ch==*Sp)
  {
   Sp++ ;
   lex.lex=char_lex[i].lex ;
   if(*Sp=='=')
   {
    if(lex.lex==lss)  { lex.lex=lse;   Sp++; }
    if(lex.lex==grt)  { lex.lex=gre;   Sp++; }
    if(lex.lex==assgn){ lex.lex=equal; Sp++; }
    if(lex.lex==not)  { lex.lex=noteq; Sp++; }
   }
   return 1 ;
  }

 if (isalpha(*((unsigned char*)Sp)) || *Sp=='_'|| *Sp=='.' || *Sp=='"')
 {
  int m=0;
  if(*Sp=='.') { m++; Sp++; }

  lex.s="";//(char *)0;
  if(*Sp=='"')
  {
   for(Sp++;*Sp && *Sp!='"';Sp++) lex.s+=(char)(*Sp) ;
   if (!*Sp) ParseError(1,noclosequoteerr);
   Sp++;
  }
  else for( ; isalnum(*((unsigned char*)Sp)) || *Sp=='_'; Sp++) lex.s+=(char)rus_toupper(*Sp) ;
  if(!m)
  {
   for(i=0 ; char_lex[i].ch!='\0' ; i++);
   for(    ; char_lex[i].ch!=1 ; i++)
    if(lex.s==char_lex[i].txt)
    {
     lex.lex=char_lex[i].lex ;
     return 1 ;
    }
  }
  for(;*Sp==' ';Sp++);
  if(*Sp!='(')
  {
    lex.lex=m?member:ident;
    return 1;
  }
  else
  {
//  for(i=0 ; functions[i]!=NULL ; i++)
//   if (!strcmp(cur_iden,functions[i]))
//     { lex=func ; cur_func_num=i ; return 1 ; }
    lex.lex=m?method:func;
    return 1;
  }
 }

 if (*Sp=='\'')
 {
  lex.lex=constring;
  lex.s="";
  for(i=0 , Sp++ ; *Sp!='\'' && *Sp ; i++ , Sp++)
  {
    if(*Sp=='\\' && Sp[1]=='\'') { lex.s+='\''; Sp++; }
    else lex.s+=*Sp ;
  }
  if (!*Sp) ParseError(1,noclosequoteerr);
  Sp++ ;
  return 1 ;
 }

 if (isdigit(*(unsigned char*)Sp))
 {
  char *t=Sp;
  lex.v=RoundLD(_strtold(Sp,&Sp),6);

  int t1=strcspn(t,".eE");
  if(t1<(Sp-t))   lex.lex=constant ;
  else { lex.i=lex.v; ; lex.lex=consint; }

  return 1 ;
 }

 if (*Sp=='\n' || *Sp=='\0')
 {
  lex.lex=endexp ;
  return 1 ;
 }
 ParseError(2,badcharerr);
 return 0;
}

void Prog::ParseError(int code,::string const &msg)
{
 char b1[10],b2[10];
 throw OraError(code,PP.file+' '+itoa(PP.line,b1,10)+':'+itoa(Sp-line,b2,10)
               +"  "+msg+'\n'+line);
}

/*
int Prog::cur_line=0;
::string *Prog::cur_func;
*/

void Prog::RTError(int code,::string const &msg)  throw(OraError)
{
 char b1[10];
 ::string e="" ;
 if (cur_func)
  e=*cur_func ;
 e+=' ';
 e+=itoa(cur_line,b1,10);
 e+=":__  ";
 e+=msg;
 throw OraError(code,e);
// throw OraError(code,*cur_func+' '+itoa(cur_line,b1,10)+
//                ":__  "+msg);
}

void Prog::RTError(int code,const char *msg)  throw(OraError)
{
 char b1[10];
 ::string e="" ;
 if (cur_func)
   e=*cur_func ;
 e+=' ';
 e+=itoa(cur_line,b1,10);
 e+=":__  ";
 e+=msg;
 throw OraError(code,e.data());
// throw OraError(code,*cur_func+' '+itoa(cur_line,b1,10)+
//                ":__  "+msg);
}
