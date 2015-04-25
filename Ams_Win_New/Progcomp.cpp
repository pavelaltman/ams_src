#include "AMS_HDR.h"
#pragma hdrstop
#include "accs.h"

void LangErr(OraError &ERR);

extern
r_char noclosebrackerr,badcharerr,noclosequoteerr,syntaxerr,
       badtypeerr,varnameexpectederr,commaexpectederr,
       unknownfuncerr,badopertypeerr,ctrlstmterr,fileerr;

Node Const_Node("Constant");


int Prog::Compile(ProgPipe *pipe,int lib)
{
 int ret=0 ;
 Sp=line;
 if(!pipe->Open(PP)) //ParseError(0,fileerr);
    goto q ;
 if(!lib)
 {
  cmds.erase(cmds.begin(),cmds.end());
  main_file=PP.file;
 }

 if(Block(pipe)!=0) { pipe->Close() ; ParseError(5,ctrlstmterr); }

 AddCmd(ICmd(ic_halt));
 ret=1 ;

q:
 pipe->Close();
 return ret ;
}

int Prog::Link(const char *doc_path)
{
 int l;
 for(l=0;l<uses_lib.size();l++)
 {
   ProgFile pf;
   char fname[256];
   sprintf(fname,"%s\\%s",doc_path,uses_lib[l].data());
   pf.Init(fname);
   Compile(&pf,1);
 }
 return 1;
}

int Prog::Block(ProgPipe *pipe)
{
 while(pipe->GetLine(line,PP))
 {
   AddCmd(ICmd(ic_curline,PP.line));
   Sp=line;
   GetLex();
   if(lex.lex==iter_e)  return 1;
   if(lex.lex==asksign) return 2;
   if(lex.lex==calc_b)  PrintBlock(pipe);
   else if(lex.lex==iter_b)
   {
    AddCmd(ICmd(ic_enter));
    GetLex();
    switch (lex.lex) {
     case endexp:
          if(Block(pipe)!=1) ParseError(5,ctrlstmterr);
          break;
     case asksign:
          CaseBlock(pipe);
          break;
     case WHILE: {
          GetLex();
          int p0=cmds.size();
          Expr();
          if(lex.lex!=endexp)
             ParseError(4,syntaxerr);
          AddCmd(ICmd(ic_jz,0x0000));
          int p=cmds.size();
          if(Block(pipe)!=1) ParseError(5,ctrlstmterr);
          AddCmd(ICmd(ic_jmp,p0));
          cmds[p-1].arg=cmds.size();
          } break;
     case UNTIL: {
          GetLex();
          AddCmd(ICmd(ic_jmp,0x0000));
          int p0=cmds.size();
          Expr();
          if(lex.lex!=endexp)
             ParseError(4,syntaxerr);
          AddCmd(ICmd(ic_jz,0x0000));
          int p=cmds.size();
          if(Block(pipe)!=1) ParseError(5,ctrlstmterr);
          AddCmd(ICmd(ic_jmp,p0));
          cmds[p0-1].arg=p;
          cmds[p-1].arg=cmds.size();
          } break;
     case FOR: {
          GetLex();
          if(lex.lex!=o_brack)
          {
            if(lex.lex!=ident)  ParseError(4,syntaxerr);

            int vidx=consts.Add(lex.s);
            AddCmd(ICmd(ic_makevar,vidx));
            ::string var=lex.s,curs="_cursor_";
            int cidx=consts.Add(curs);
            AddCmd(ICmd(ic_makevar,cidx));

            GetLex();
            if(lex.lex!=belong) ParseError(4,syntaxerr);
            lex.lex=ident;
            lex.s=curs;
            while(*Sp==' ') Sp++;
            if(*Sp!='(')
            {
             *(--Sp)='.';
             Expr();
             if(lex.lex!=endexp)  ParseError(4,syntaxerr);

             AddCmd(ICmd(ic_ldef));
             AddCmd(ICmd(ic_resolve,vidx));
             AddCmd(ICmd(ic_loadconst,consts.Add(1)));
             AddCmd(ICmd(ic_call,consts.Add("DO")));
            }
            else
            {
             AddCmd(ICmd(ic_ldef));
             AddCmd(ICmd(ic_resolve,cidx));
             GetLex();
             Expr();
             if(lex.lex!=endexp)  ParseError(4,syntaxerr);
             AddCmd(ICmd(ic_store));

             AddCmd(ICmd(ic_ldef));
             AddCmd(ICmd(ic_resolve,cidx));
             AddCmd(ICmd(ic_ldef));
             AddCmd(ICmd(ic_resolve,vidx));
             AddCmd(ICmd(ic_loadconst,consts.Add(1)));
             AddCmd(ICmd(ic_call,consts.Add("DO")));
            }
            int p0=cmds.size();

            AddCmd(ICmd(ic_ldef));
            AddCmd(ICmd(ic_resolve,cidx));
            AddCmd(ICmd(ic_loadconst,consts.Add(0)));
            AddCmd(ICmd(ic_call,consts.Add("FETCH")));

            int p=cmds.size();
            AddCmd(ICmd(ic_jz,0x0000));

            if(Block(pipe)!=1) ParseError(5,ctrlstmterr);
            AddCmd(ICmd(ic_jmp,p0));

            cmds[p].arg=cmds.size();
          }
          if(lex.lex==o_brack)
          {
             GetLex();
             if(lex.lex!=comma) Stmt();
             int p0=cmds.size();
             if(lex.lex!=comma) ParseError(4,syntaxerr);
             GetLex();
             Expr();
             AddCmd(ICmd(ic_jz,0x0000));
             int p=cmds.size();

             int post=0;
             if(lex.lex!=c_brack)
             {
              post=1;
              AddCmd(ICmd(ic_jmp,0x0000));
              if(lex.lex!=comma)   ParseError(4,syntaxerr);
              GetLex();
              if(lex.lex!=c_brack) Stmt();
              if(lex.lex!=c_brack) ParseError(4,syntaxerr);

              AddCmd(ICmd(ic_jmp,p0));
              cmds[p].arg=cmds.size();
             }
             GetLex();
             if(lex.lex!=endexp)  ParseError(4,syntaxerr);

             if(Block(pipe)!=1) ParseError(5,ctrlstmterr);

             if(post)
              AddCmd(ICmd(ic_jmp,p+1));
             else
              AddCmd(ICmd(ic_jmp,p0));

             cmds[p-1].arg=cmds.size();
          }
          } break;
     case FUNCTION: {
          int p0=cmds.size();
          AddCmd(ICmd(ic_jmp,0x0000));
          GetLex();
          if(lex.lex!=func)
             ParseError(4,syntaxerr);

          UserFunc *UF=new UserFunc(lex.s,cmds.size());

          GetLex(); // Skip '('
          while(1)
          {
           GetLex();
           if(lex.lex==c_brack) break;
           if(lex.lex!=ident)
             ParseError(4,syntaxerr);
           UF->AddArg(lex.s);
           GetLex();
           if(lex.lex==c_brack) break;
           if(lex.lex!=comma)
              ParseError(6,commaexpectederr);
          }
          GetLex();
          if(lex.lex!=endexp)
             ParseError(4,syntaxerr);

          if(Block(pipe)!=1) ParseError(5,ctrlstmterr);
          AddCmd(ICmd(ic_loadnull));
          AddCmd(ICmd(ic_ret));
          cmds[p0].arg=cmds.size();
          uf.push_back(UF);
          } break;
      default:
          ParseError(4,syntaxerr);
    }
    AddCmd(ICmd(ic_leave));
   }
   else
   {
    Stmt();
    if(lex.lex!=endexp)
       ParseError(4,syntaxerr);
   }
 }
 return 0;
}

int Prog::CaseBlock(ProgPipe *pipe)
{
  GetLex();
  Expr();
  if(lex.lex!=endexp)  ParseError(4,syntaxerr);
  AddCmd(ICmd(ic_jz,0x0000));
  int p=cmds.size(),e=0;

  while(Block(pipe)==2)
  {
   if(!p) ParseError(5,ctrlstmterr);
   if(e) cmds[e-1].arg=cmds.size();
   AddCmd(ICmd(ic_jmp,0x0000));
   cmds[p-1].arg=e=cmds.size();
   GetLex();

   if(lex.lex==endexp)  p=0;
   else
   {
    Expr();
    if(lex.lex!=endexp)  ParseError(4,syntaxerr);
    AddCmd(ICmd(ic_jz,0x0000));
    p=cmds.size();
   }
  }

  if(lex.lex==iter_e)
  {
    if(p) cmds[p-1].arg=cmds.size();
    if(e) cmds[e-1].arg=cmds.size();
  }
  else ParseError(5,ctrlstmterr);
  return 1;
}

int Prog::PrintBlock(ProgPipe *pipe)
{
  while(pipe->GetLine(line,PP))
  {
   if(*line=='}') return 1;
   AddCmd(ICmd(ic_curline,PP.line));

   AddCmd(ICmd(ic_ldef));
   int par_c=1;

   int p=cmds.size();
   AddCmd(ICmd(ic_loadconst,0x0000));
   ::string fmt="";
   char *t,*t1;

   for(Sp=line;*Sp;)
   {
    t=strchr(Sp,'{');
    if(t) *t=0;
    fmt+=Sp;
    if(!t) break;
    Sp=t+1;
    fmt+="%%";

    AddCmd(ICmd(ic_ldef)); // For OUTSTR

    GetLex();
    Expr();

    ::string mod="";
    if(lex.lex==comma)
    {
     t1=strchr(Sp,'}');
     if(!t1) ParseError(4,syntaxerr);
     *t1=0;
     mod=Sp;
     Sp=t1+1;
    }
    else if(lex.lex!=calc_e) ParseError(4,syntaxerr);
    int len=Sp-t;
    AddCmd(ICmd(ic_loadconst,consts.Add(len)));

    if(mod!="")
    {
      AddCmd(ICmd(ic_loadconst,consts.Add(mod)));
      AddCmd(ICmd(ic_loadconst,consts.Add(3)));
    }
    else AddCmd(ICmd(ic_loadconst,consts.Add(2)));

    int idx=consts.Add("OUTSTR");
    AddCmd(ICmd(ic_call,idx));

    par_c++;
   }

   int pcidx=consts.Add(par_c);
   AddCmd(ICmd(ic_loadconst,pcidx));

   int idx=consts.Add(fmt);
   cmds[p].arg=idx;
   idx=consts.Add("QOUT");
   AddCmd(ICmd(ic_call,idx));
  }
 ParseError(5,ctrlstmterr);
 return 0;
}


int Prog::Factor()
{
 int idx;
 switch(lex.lex)
 {
  case constring:
   idx=consts.Add(lex.s);
   AddCmd(ICmd(ic_loadconst,idx));
   GetLex();
   break;
  case constant:
   idx=consts.Add(lex.v);
   AddCmd(ICmd(ic_loadconst,idx));
   GetLex();
   break;
  case consint:
   idx=consts.Add(lex.i);
   AddCmd(ICmd(ic_loadconst,idx));
   GetLex();
   break;
  case ident:
   AddCmd(ICmd(ic_ldef));
   idx=consts.Add(lex.s);
   AddCmd(ICmd(ic_resolve,idx));
   GetLex();
   break;
  case member:
   idx=consts.Add(lex.s);
   AddCmd(ICmd(ic_resolve,idx));
   GetLex();
   break;
  case func:
  case method:
   {
    if(lex.lex==func)
     AddCmd(ICmd(ic_ldef));
    idx=consts.Add(lex.s);
    GetLex(); // Skip '('

    int par_c=0;
    while(1)
    {
     GetLex();
     if(lex.lex==c_brack) break;
     if(lex.lex==comma)
      AddCmd(ICmd(ic_loadnull));
     else Expr();
     par_c++;
     if(lex.lex==c_brack) break;
     if(lex.lex!=comma)
        ParseError(6,commaexpectederr);
    }

    int pcidx=consts.Add(par_c);
    AddCmd(ICmd(ic_loadconst,pcidx));

    AddCmd(ICmd(ic_call,idx));
    GetLex();
   }
   break;
  case minus:
   GetLex();
   Factor();
   AddCmd(ICmd(ic_uminus));
   break;
  case not:
   GetLex();
   Factor();
   AddCmd(ICmd(ic_not));
   break;
  case o_brack :
   GetLex();
   Expr();
   if (lex.lex!=c_brack)
        ParseError(3,noclosebrackerr);
   GetLex();
   break ;
  default:
   ParseError(4,syntaxerr);
 ;
 }
 return 1;
}

int Prog::Term()
{
 Lex    temp_lex ;

 Factor();
 while(lex.lex==member || lex.lex==method) Factor();
 while(lex.lex==aster  || lex.lex==slash)// || lex==belong
 {
  temp_lex=lex.lex ;
  GetLex();
  Factor();
  while(lex.lex==member || lex.lex==method) Factor();
  switch(temp_lex) {
   case aster:   AddCmd(ICmd(ic_mul));
   break;
   case slash:   AddCmd(ICmd(ic_div));
   break;
  }
/*  if (temp_lex==belong)if (!AddCommand( _in  )) return 0 ;
*/
 }
 return 1 ;
}

int Prog::SubExpr()
{
 Lex    temp_lex ;
 Term();
 while(lex.lex==plus || lex.lex==minus)
 {
  temp_lex=lex.lex ;
  GetLex();
  Term();
  switch(temp_lex) {
   case plus:   AddCmd(ICmd(ic_add));   break;
   case minus:  AddCmd(ICmd(ic_sub));   break;
  }
 }
 return 1 ;
}

int Prog::CompExpr()
{
 Lex    temp_lex ;
 SubExpr();
 while(lex.lex==lss || lex.lex==lse || lex.lex == equal || lex.lex==noteq
       || lex.lex==gre || lex.lex==grt)
 {
  temp_lex=lex.lex ;
  GetLex();
  SubExpr();
  switch(temp_lex) {
   case lss:     AddCmd(ICmd(ic_lss));    break;
   case lse:     AddCmd(ICmd(ic_lse));    break;
   case equal:   AddCmd(ICmd(ic_eq));     break;
   case noteq:   AddCmd(ICmd(ic_noteq));  break;
   case gre:     AddCmd(ICmd(ic_gre));    break;
   case grt:     AddCmd(ICmd(ic_grt));    break;
  }
 }
 return 1 ;
}

int Prog::LogExpr()
{
 Lex    temp_lex ;
 CompExpr();
 while(lex.lex==or || lex.lex==and)
 {
  temp_lex=lex.lex ;
  GetLex();
  CompExpr();
  switch(temp_lex) {
   case or:   AddCmd(ICmd(ic_or));   break;
   case and:  AddCmd(ICmd(ic_and));  break;
  }
 }
 return 1 ;
}

int Prog::Expr()
{
 Lex    temp_lex ;
 LogExpr();
 while(lex.lex==assgn)
 {
   temp_lex=lex.lex ;
   GetLex();
   LogExpr();
   switch(temp_lex) {
    case assgn:   AddCmd(ICmd(ic_store));   break;
   }
 }
 return 1 ;
}

int Prog::Stmt()
{
 if(lex.lex==endexp) return 1;

 if(lex.lex==LOCAL || lex.lex==STRUCT)
 {
   int s=(lex.lex==STRUCT);
   do {
    GetLex();
    if(lex.lex!=ident) ParseError(11,varnameexpectederr);
    int idx=consts.Add(lex.s);
    AddCmd(ICmd(s?ic_makestruct:ic_makevar,idx));
    GetLex();
   } while(lex.lex==comma);
 }
 else if(lex.lex==USELIB)
 {
   do {
    GetLex();
    if(lex.lex!=constring) ParseError(11,varnameexpectederr);
    uses_lib.push_back(lex.s);
    GetLex();
   } while(lex.lex==comma);
 }
 else if(lex.lex==ret)
 {
   GetLex();
   if(lex.lex==endexp)
   {
    AddCmd(ICmd(ic_loadnull));
    AddCmd(ICmd(ic_ret));
   }
   else
   {
     Expr();
     if(lex.lex!=endexp)
          ParseError(4,syntaxerr);
     AddCmd(ICmd(ic_ret));
   }
 }
 else
 {
  Expr();
  AddCmd(ICmd(ic_drop));
 }
 return 1 ;
}

Prog::~Prog()
{
 for(int i=0;i<uf.size();i++) delete(uf[i]);
}

Prog def_prog ; 
Prog *prog_ptr=&def_prog ;

/*
FILE * Prog::out;
int Prog::out_line;
*/

