#include "AMS_HDR.h"
#pragma hdrstop
#include "accs.h"

extern
r_char noclosebrackerr,badcharerr,noclosequoteerr,syntaxerr,
       badtypeerr,varnameexpectederr,commaexpectederr,
       unknownfuncerr,badopertypeerr,invalidreterr,breakgenerate,
       userbreakmes ;

char KbPressed;

int Prog::Run(IP &ip)
{
 int CS_IP;
 call_stck_name.push_back(cur_func=&main_file);
 GetAsyncKeyState(VK_ESCAPE);
 for(CS_IP=0;CS_IP<cmds.size();CS_IP++)
 {
    if (GetAsyncKeyState(VK_ESCAPE) )
    {
        if (AskMessage(breakgenerate)==ID_YES)
            throw OraError(100,userbreakmes) ;
    }
   switch(cmds[CS_IP].cmd) {
    case ic_enter:
         ip.GlobStructure().Enter();
         break;
    case ic_leave:
         ip.GlobStructure().Leave();
         break;
    case ic_makevar:
          ip.GlobStructure().AddVar(consts[cmds[CS_IP].arg].String());
         break;
    case ic_makestruct:
          ip.GlobStructure().AddStruct(consts[cmds[CS_IP].arg].String());
         break;
    case ic_loadnull:
         ip.stck.push(StackItem(VAL()));
         break;
    case ic_loadconst:
         ip.stck.push(StackItem(Const_Node,consts[cmds[CS_IP].arg]));
         break;
    case ic_ldef:
         ip.stck.push(StackItem(ip.Base));
         break;
    case ic_resolve:
         {
          StackItem st=ip.stck.pop();
          Node &n=st.getNode().Structure().Var(consts[cmds[CS_IP].arg].String());
          ip.stck.push(StackItem(n));
         }
         break;
    case ic_add:
    case ic_sub:
    case ic_mul:
    case ic_div:
    case ic_and:
    case ic_or :
    case ic_lss:
    case ic_lse:
    case ic_noteq:
    case ic_eq :
    case ic_grt:
    case ic_gre:
        {
          StackItem a=ip.stck.pop();
          StackItem b=ip.stck.pop();
          ip.stck.push(DoOperator(cmds[CS_IP].cmd,b,a));
          a.destroy();
          b.destroy();
         }
         break;
    case ic_not:
        {
          StackItem a=ip.stck.pop();
          long double av=a.Number();
          ip.stck.push(VAL(int(av==0)));
          a.destroy();
         }
         break;
    case ic_uminus:
        {
          StackItem a=ip.stck.pop();
          long double av=a.Number();
          ip.stck.push(a.vtype()==gcl::VT_INT?VAL(int(-av)):VAL(-av));
          a.destroy();
         }
         break;
    case ic_store:
        {
          StackItem a=ip.stck.pop();
          StackItem b=ip.stck.pop();
          Node &nd=b.getNode();
          nd.assign(a);
          ip.stck.push(StackItem(nd));
         }
         break;
    case ic_drop:
         {
          StackItem a=ip.stck.pop();
          a.destroy();
         }
         break;
    case ic_call:
        {
          int pc=ip.stck.pop().Number();
          StackItem &ob=ip.stck.top(pc);
          ::string &func_name=consts[cmds[CS_IP].arg].String();
          int i;
          for(i=0;i<uf.size();i++)
            if(uf[i]->name==func_name) break;
          if(i<uf.size())
          {
           int upc=uf[i]->args.size();
           ip.GlobStructure().Enter();
           call_stck.push_back(CS_IP);
           call_stck_name.push_back(cur_func=&func_name);
           for(;pc>upc;pc--)
             ip.stck.pop().destroy(); // Drop extra params
           for(;pc<upc;upc--)
             ip.GlobStructure().AddVar(uf[i]->args[upc-1]); // Omitted params
           for(;pc;pc--)
           {
            Node &nd=ip.GlobStructure().AddVar(uf[i]->args[pc-1]);
            nd.assign_(ip.stck.pop());
            //nd.assign(ip.stck.pop().Val());
           }
           Node &nd=ip.GlobStructure().AddVar("self");
           nd.assign(ip.stck.pop().Val());
           CS_IP=uf[i]->entry-1;
          }
          else
          {
            StackItem res=ob.getNode().Method(func_name,pc,&ip.stck.top());
            while(0<=pc--) ip.stck.pop().destroy();
            ip.stck.push(res);
          }
         }
         break;
    case ic_jmp:
         CS_IP=cmds[CS_IP].arg-1;
         break;
    case ic_jz:
    case ic_jnz:
        {
          StackItem a=ip.stck.pop();
          long double av=a.Number();
          a.destroy();
          if((cmds[CS_IP].cmd==ic_jz && !av) || (cmds[CS_IP].cmd==ic_jnz && av))
              CS_IP=cmds[CS_IP].arg-1;
         }
         break;
    case ic_ret:
         {
           StackItem &retval=ip.stck.top();
           if(retval.nd)
              ip.stck.push(ip.stck.pop().val.clone());
           ip.GlobStructure().Leave();
           if(call_stck.size()==0)  RTError(105,invalidreterr);
           CS_IP=call_stck.back();
           call_stck.pop_back();

           call_stck_name.pop_back();
           cur_func=call_stck_name.back();
           break;
         }
    case ic_curline:
         cur_line=cmds[CS_IP].arg;
         break;
    case ic_halt:
         return 0;
   }
 }
 return 1;
}

StackItem Prog::DoOperator(icmd op,StackItem &l,StackItem &r)
{
  if(l.vtype()==gcl::VT_INT && r.vtype()==gcl::VT_INT)
  {
   int lv=l.Number();
   int rv=r.Number();
   switch(op)  {
    case ic_add: return VAL(lv+rv);
    case ic_sub: return VAL(lv-rv);
    case ic_mul: return VAL(lv*rv);
    case ic_div: if(rv)
    {    // деление не целочисленное!!!
         double LVV=lv;
         return VAL(LVV/rv);
    }

         return VAL();
    case ic_and: return VAL(int(lv && rv));
    case ic_or : return VAL(int(lv || rv));
    case ic_lss: return VAL(int(lv <  rv));
    case ic_lse: return VAL(int(lv <= rv));
    case ic_eq : return VAL(int(lv == rv));
    case ic_noteq: return VAL(int(lv != rv));
    case ic_gre: return VAL(int(lv >= rv));
    case ic_grt: return VAL(int(lv >  rv));
   }
  }
  else if(l.vtype()==gcl::VT_NUM || l.vtype()==gcl::VT_INT)
  {
   long double lv=l.Number();
   long double rv=r.Number();
   switch(op)  {
    case ic_add: return VAL(lv+rv);
    case ic_sub: return VAL(lv-rv);
    case ic_mul: return VAL(lv*rv);
    case ic_div: if(rv) return VAL(lv/rv);
                 return VAL();
    case ic_and: return VAL(int(lv && rv));
    case ic_or : return VAL(int(lv || rv));
    case ic_lss: return VAL(int(lv <  rv));
    case ic_lse: return VAL(int(lv <= rv));
    case ic_eq : return VAL(int(lv == rv));
    case ic_noteq:
    {
        long double Res = lv -rv;
        int i = (Res>0.000001)||(Res<-0.000001);
        return VAL(i);
    }
    case ic_gre: return VAL(int(lv >= rv));
    case ic_grt: return VAL(int(lv >  rv));
   }
  }
  else if(l.vtype()==gcl::VT_STRING)
  {
   ::string &lv=l.String();
   ::string &rv=r.String();
   switch(op)  {
    case ic_add: return VAL(lv+rv);
    case ic_lss: return VAL(int(lv <  rv));
    case ic_lse: return VAL(int(lv <= rv));
    case ic_eq : return VAL(int(lv == rv));
    case ic_noteq: return VAL(int(lv != rv));
    case ic_gre: return VAL(int(lv >= rv));
    case ic_grt: return VAL(int(lv >  rv));
   }
  }
  else if(l.vtype()==gcl::VT_DATE)
  {
   DateTime &lv=l.GetDate();
   if(r.vtype()==gcl::VT_DATE)
   {
    DateTime &rv=r.GetDate();
    switch(op)  {
     case ic_lss: return VAL((int)(lv <  rv));
     case ic_lse: return VAL((int)(lv <= rv));
     case ic_eq : return VAL((int)(lv == rv));
     case ic_noteq: return VAL((int)(lv != rv));
     case ic_gre: return VAL((int)(lv >= rv));
     case ic_grt: return VAL((int)(lv >  rv));
    }
   }
   else
   {
    int rv=r.Number();
    switch(op)  {
     case ic_add: return VAL(lv.ChDay(rv));
     case ic_sub: return VAL(lv.ChDay(-rv));
    }
   }
  }
  RTError(106,badopertypeerr);
  return StackItem();
}
