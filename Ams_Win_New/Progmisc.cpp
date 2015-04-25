#include "AMS_HDR.h"
#pragma hdrstop
#include "accs.h"
#include "LOCAL_VARS.h"
extern
r_char noclosebrackerr,badcharerr,noclosequoteerr,syntaxerr,
       badtypeerr,varnameexpectederr,commaexpectederr,
       unknownfuncerr;

int Const_TBL::Add(::string const &s)
{
 for(int i=0;i<Consts.size();i++)
  if(Consts[i].vtype()==gcl::VT_STRING && Consts[i].String()==s) return i;
 Consts.push_back(VAL(s));
 return Consts.size()-1;
}

int Const_TBL::Add(long double v)                               
{
 for(int i=0;i<Consts.size();i++)
  if(Consts[i].vtype()==gcl::VT_NUM && Consts[i].Number()==v) return i;
 Consts.push_back(VAL(v));
 return Consts.size()-1;
}

int Const_TBL::Add(int v)
{
 for(int i=0;i<Consts.size();i++)
  if(Consts[i].vtype()==gcl::VT_INT && Consts[i].Number()==v) return i;
 Consts.push_back(VAL(v));
 return Consts.size()-1;
}

Const_TBL::~Const_TBL()
{
 for(int i=0;i<Consts.size();i++)
  if(Consts[i].vtype()!=gcl::VT_NULL) Consts[i].destroy();
}

::string & VAL::String()
{
  if(!fld || fld->vtype()!=gcl::VT_STRING)
        prog_ptr->RTError(100,badtypeerr);
  return dynamic_cast< Field<::string>* >(fld)->val;
}

long double VAL::Number()
{
  if(!fld || (fld->vtype()!=gcl::VT_NUM && fld->vtype()!=gcl::VT_INT))
    prog_ptr->RTError(100,badtypeerr);
 if(fld->vtype()==gcl::VT_NUM)
  return dynamic_cast< Field<Sum>* >(fld)->val;
 else
  return dynamic_cast< Field<int>* >(fld)->val;
}

Sum &VAL::GetSum()
{
  if(!fld || fld->vtype()!=gcl::VT_NUM ) prog_ptr->RTError(100,badtypeerr);
  return dynamic_cast< Field<Sum>* >(fld)->val;
}

DateTime& VAL::GetDate()
{
  if(!fld || fld->vtype()!=gcl::VT_DATE ) prog_ptr->RTError(100,badtypeerr);
  return dynamic_cast< Field<DateTime>* >(fld)->val;
}

StackItem::StackItem(const VAL &v):nd(0)
{
    val = v;
};

::string & StackItem::String()
{
  if(!val.fld || val.fld->vtype()!=gcl::VT_STRING)
    if(nd) prog_ptr->RTError(100,nd->name+"  "+badtypeerr);
    else prog_ptr->RTError(100,badtypeerr);
  return dynamic_cast< Field<::string>* >(val.fld)->val;
}

long double StackItem::Number()
{
  if(!val.fld || (val.fld->vtype()!=gcl::VT_NUM && val.fld->vtype()!=gcl::VT_INT))
    if(nd) prog_ptr->RTError(100,nd->name+"  "+badtypeerr);
    else prog_ptr->RTError(100,badtypeerr);
  if(val.fld->vtype()==gcl::VT_NUM)
   return dynamic_cast< Field<Sum>* >(val.fld)->val;
  else
   return dynamic_cast< Field<int>* >(val.fld)->val;
}

Sum &StackItem::GetSum()
{
  if(!val.fld || val.fld->vtype()!=gcl::VT_NUM )
    if(nd) prog_ptr->RTError(100,nd->name+"  "+badtypeerr);
    else prog_ptr->RTError(100,badtypeerr);
  return dynamic_cast< Field<Sum>* >(val.fld)->val;
}

DateTime& StackItem::GetDate()
{
  if(!val.fld || val.fld->vtype()!=gcl::VT_DATE )
    if(nd) prog_ptr->RTError(100,nd->name+"  "+badtypeerr);
    else prog_ptr->RTError(100,badtypeerr);
  return dynamic_cast< Field<DateTime>* >(val.fld)->val;
}


Struct& Node::Structure()
{
  if(!st) prog_ptr->RTError(101,name+"  "+badtypeerr);
  return *st;
}

void   Node::MakeStruct()
{
 if(st) return;
 st=new Struct;
 st->IncC();
}

void   Node::destroy()
{
  if(st)
  {
   st->DecC();
   if(st->GetCount()<=0) { delete(st); }
   else st=0;
  }
  if(!refer) VAL::destroy();
  else fld=0;
}

void   Node::assign(StackItem &si)
{
 if(si.nd==this) return;
 VTYPE vt=vtype();
// VAL::destroy();
 // ≈сли к SI прикреплена стуктура
 if(si.nd && si.nd->st)
 {
   if(st) // ѕровер€ем нужно ли удалить текущую структуру
   {
    st->DecC();
    if(st->GetCount()<=0) { delete(st); }
   }
//   destroy();
   st=si.nd->st; // присваиваем значение дл€ текущей структуры
   st->IncC();
 }

 if(si.vtype()==gcl::VT_NULL)
 {
  if(vt!=gcl::VT_NULL) fld->SetNull();
  return;
 }

 switch(vt) {
  case gcl::VT_NULL:
       if(si.nd)  *(VAL *)this=si.val.clone();
       else       *(VAL *)this=si.val;
       return;
  case gcl::VT_INT:
     *dynamic_cast< Field<int>* >(fld)=si.Number();
     si.destroy();
     return;
  case gcl::VT_NUM:
     *dynamic_cast< Field<Sum>* >(fld)=si.Number();
     si.destroy();
     return;
  case gcl::VT_STRING:
     {
      ::string &s=si.String();
      *dynamic_cast< Field<::string>* >(fld)=s;//si.String();
      si.destroy();
     }
     return;
  case gcl::VT_DATE:
     {
      DateTime &d=si.GetDate();
      *dynamic_cast< Field<DateTime>* >(fld)=d;//si.String();
      si.destroy();
     }
     return;
  default:
     prog_ptr->RTError(100,name+"  "+badtypeerr);
  }
}

//------------------------------------------------------------
void   Node::assign_(StackItem si)
{
 if(si.nd==this) return;
 VTYPE vt=vtype();
// VAL::destroy();
 // ≈сли к SI прикреплена стуктура
 if(si.nd && si.nd->st)
 {
   if(st) // ѕровер€ем нужно ли удалить текущую структуру
   {
    st->DecC();
    if(st->GetCount()<=0) { delete(st); }
   }
//   destroy();
   st=si.nd->st; // присваиваем значение дл€ текущей структуры
   st->IncC();
 }

 if(si.vtype()==gcl::VT_NULL)
 {
  if(vt!=gcl::VT_NULL) fld->SetNull();
  return;
 }

 switch(vt) {
  case gcl::VT_NULL:
       if(si.nd)  *(VAL *)this=si.val.clone();
       else       *(VAL *)this=si.val;
       return;
  case gcl::VT_INT:
     *dynamic_cast< Field<int>* >(fld)=si.Number();
     si.destroy();
     return;
  case gcl::VT_NUM:
     *dynamic_cast< Field<Sum>* >(fld)=si.Number();
     si.destroy();
     return;
  case gcl::VT_STRING:
     {
      ::string &s=si.String();
      *dynamic_cast< Field<::string>* >(fld)=s;//si.String();
      si.destroy();
     }
     return;
  case gcl::VT_DATE:
     {
      DateTime &d=si.GetDate();
      *dynamic_cast< Field<DateTime>* >(fld)=d;//si.String();
      si.destroy();
     }
     return;
  default:
     prog_ptr->RTError(100,name+"  "+badtypeerr);
  }
}
//------------------------------------------------------------
void   Node::assign(const VAL& vl)
{
 VTYPE vt=vtype();
 // ≈сли к SI прикреплена стуктура
 if(vl.vtype()==gcl::VT_NULL)
 {
  if(vt!=gcl::VT_NULL) fld->SetNull();
  return;
 }

 switch(vt) {
  case gcl::VT_NULL:
    *(VAL *)this=vl;
    return;
  case gcl::VT_INT:
     *dynamic_cast< Field<int>* >(fld)=vl.Number();
     return;
  case gcl::VT_NUM:
     *dynamic_cast< Field<Sum>* >(fld)=vl.Number();
     return;
  case gcl::VT_STRING:
     {
      ::string &s=vl.String();
      *dynamic_cast< Field<::string>* >(fld)=s;//si.String();
     }
     return;
  case gcl::VT_DATE:
     {
      DateTime &d=vl.GetDate();
      *dynamic_cast< Field<DateTime>* >(fld)=d;//si.String();
      vl.destroy();
     }
     return;
  default:
     prog_ptr->RTError(100,name+"  "+badtypeerr);
  }
}
//------------------------------------------------------------
void Node::AddStruct(Struct* S)
{
    for (int i = 0; i< S->Size(); i++)
    {
        Node* N = S->Var(i);
        st->UpdateVar_Fld(N->name.data(), N->fld);
    }
};
//------------------------------------------------------------
void   Node::set(OraField const &FLD)
{
 if(fld==&FLD) return;
 VAL::destroy();
 fld=(OraField *)&FLD;
}


Node& StackItem::getNode()
{
  if(!nd) prog_ptr->RTError(100,varnameexpectederr);
  return *nd;
}

Struct::~Struct()
{
 while(els.size())
 {
  els.back()->destroy();
  delete(els.back());
  els.pop_back();
 }
}

Node &Struct::Var(::string const &name)
{
 Node *cn=FindVar(name);
 if(cn) return *cn;
 els.push_back(new Node(name));
 return *els.back();
}

Node *Struct::FindVar(::string const &name)
{
 Node **cn;
 for(cn=els.end();cn && cn>els.begin();)
 {
  cn--;
  if((*cn)->name==name) return *cn;
 }
 return 0;
}

Node* Struct::DelVar(const string& VarName)
{
    Node **cn;
    Node* Ret =0;
    for(cn=els.end();cn && cn>els.begin();)
    {
        cn--;
        if((*cn)->name==VarName)
        {
            Ret= *cn;
            els.erase(cn);
            return Ret;
        }
    }
    return Ret;
}

#pragma argsused
void Struct::MakeLink(FieldLink &lnk, int inp, int sn)
{
 lnk.fld=NULL;
 lnk.fldNum=-1;
 Node **cn;
 for(cn=els.end();cn && cn>els.begin();)
 {
  cn--;
  if(!strcmp((*cn)->name.data(),(char*)lnk.name))
  {
   lnk.fldNum=cn-els.begin();
   VTYPE type=(*cn)->vtype();
   int size=0;
   switch(type) {
     case gcl::VT_INT:    lnk.int_type=INT_TYPE;  size=sizeof(int); break;
     case gcl::VT_DATE:   lnk.int_type=DATE_TYPE; size=7; break;
     case gcl::VT_STRING: lnk.int_type=STRING_TYPE; size=255; break;
     case gcl::VT_NUM:    lnk.int_type=STRING_TYPE; size=34; break;
     case gcl::VT_ROWID:  lnk.int_type=STRING_TYPE; size=19; break;
     default: lnk.int_type=NULL_TYPE;
   }
   lnk.int_size=size;
   return;
  }
 }
 return;
}

TGenLocalVars GlobalOpVars;

void IP::SetVars(TGenLocalVars* GenLocalVars_)
{
    if (GenLocalVars_)
    {
        GlobStructure().SetVars(GenLocalVars_);
        OpVars* V=GenLocalVars_->VARS();
        Base.AddStruct(V->st);
    };
};


void  GlobStruct::Enter()
{
    if (!GenLocalVars)
    {
        GenLocalVars=&GlobalOpVars;
    }
    lims.push_back(els.size());
}


void  Stack::Enter()
{
  lims.push_back(sp());
}

void Struct::MakeEmpty()
{
    while (els.size())
    {
        els.pop_back();
    }
};


Node* Struct::AddVar(Node *node)
{
 els.push_back(node);
 return els.back();
}

void  GlobStruct::Leave()
{
  int l=lims.back();
  lims.pop_back();
  while(els.size()>l)
  {
   els.back()->destroy();
   delete(els.back());
   els.pop_back();
  }
}

void  Stack::Leave()
{
  int l=lims.back();
  lims.pop_back();
  while(sp()>l) pop().destroy();
}

Stack::~Stack()
{
 while(sp()) pop().destroy();
}

void   Struct::UpdateVar_Fld(const char *vname,OraField *fld)
{
  Node *nd=FindVar(vname);
  if(!nd)
    AddVar(new Node(  vname, *fld  ));
  else
  {
    nd->destroy();
    *nd=Node( vname, *fld );
  }
}

void   Struct::UpdateVar_Clone(const char *vname,OraField *fld)
{
  Node *nd=FindVar(vname);
  if(!nd)
    AddVar(new Node(vname,fld));
  else
  {
    nd->destroy();
    *nd=Node(vname,fld);
  }
}

