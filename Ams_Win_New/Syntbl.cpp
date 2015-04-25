#include "AMS_HDR.h"
#pragma hdrstop

#include "accs.h"

#pragma argsused
void AccRec::MakeLink(FieldLink &lnk, int inp, int sn)
{
 static FieldIntDef flds[]={
      {"ROW_ID",STRING_TYPE,19,(OraField FieldSet::*)&AccRec::row_id },
      {"CODE",INT_TYPE,sizeof(int),(OraField FieldSet::*)&AccRec::code },
      {"NAME",STRING_TYPE,0,(OraField FieldSet::*)&AccRec::name },
      {"PARENT",INT_TYPE,sizeof(int),(OraField FieldSet::*)&AccRec::parent },
      {"BAL",INT_TYPE,0,(OraField FieldSet::*)&AccRec::bal },
      {"NODE",INT_TYPE,0,(OraField FieldSet::*)&AccRec::node },
      {"DESCR",STRING_TYPE,0,(OraField FieldSet::*)&AccRec::descr },
      {"REV_CODE",INT_TYPE,sizeof(int),(OraField FieldSet::*)&AccRec::rev_code},
      {"VAL_CODE",STRING_TYPE,0,(OraField FieldSet::*)&AccRec::val_code},
      {"SETNAME",STRING_TYPE,0,(OraField FieldSet::*)&AccRec::setname},
      {"OPTS",INT_TYPE,0,(OraField FieldSet::*)&AccRec::opts },
      {"EXPANDED",INT_TYPE,0,(OraField FieldSet::*)&AccRec::expanded },
      {"RETC",INT_TYPE,0,(OraField FieldSet::*)&AccRec::retc },
      {"ANAL_ID",INT_TYPE,0,(OraField FieldSet::*)&AccRec::anal_id },
      {"BAL_FLD",STRING_TYPE,0,(OraField FieldSet::*)&AccRec::bal_fld },
      {"SUMMA",STRING_TYPE,30,(OraField FieldSet::*)&AccRec::sum},
 };
 const int fldnum=sizeof(flds)/sizeof(flds[0]);
 lnk.fld=0;
 lnk.fldNum=-1;
 for(int i=0;i<fldnum;i++) if(flds[i].Set(lnk)) return;
}


extern int new_plan ;

char *Acc_UserName(char *buf,const char *name)
{
 if(*name==0)
   { *buf=0; return NULL; }

 int t_new=0 ;

 // Skipping letter and slash
 {
  const char *t=name;
  if(isalpha(*t))
  {
   if (*t=='B') t_new=1 ;
   t++;
   if(*t=='/') t++;
  }
  strcpy(buf,t);
 }

 // Убираем лишние нули (но не с самого начала)
 {
  char *t;
  for(t=buf;*t && *t!='/';t++);
  for(;*t;)
    if(t[0]=='/' && t[1]=='0' && (t[2]!='/' && t[2])) strcpy(t+1,t+2);
    else t++;
 }

 // Для новых стандартов убираем слэши после 1-го и 2-го уровней
 if (t_new)
 {
  char *t=buf ;
  int  i=0 ;
  for( ; *t ; )
   if (*t=='/')
   {
    strcpy(t,t+1) ;
    i++ ;
    if (i>1) break ;
   }
   else t++ ;
 }

 return buf ;
}

char *AccRec::UserName(char *buf,int pref_out)
{
 *buf=0 ;
 const char *n=name.val.data() ;
 if (n[0]=='Z' && pref_out) strcpy(buf,"Z/") ;
 return Acc_UserName(buf+strlen(buf),n) ;
}

void AccRec::NormalizeItem(char *it,int l,char *oit)
{
 int n=0;
 char *t,r[10];
 if(*it=='0')
   if(oit) strcpy(oit,it);
   else return;
 for(t=it;*t && isdigit(*t);t++) { n*=10; n+=*t-'0'; }
 sprintf(r,"%0*d%s",l,n,t);
 if(oit) strcpy(oit,r);
 else strcpy(it,r);
}

void AccRec::Normalize(char *nam,char *onam)
{
 char *nm=nam;
 char res[40]="A/";
 if (new_plan) strcpy(res,"B/") ;

 int l= new_plan ? 1 : 2,
     l_flag=0 ;

 if(isalpha(*nm)) { res[0]=*nm; nm++; }
 if(*nm=='/') nm++;

 // Добавляем разделители после 1-го и 2-го уровней
 if (new_plan && nm[0]!='0')
 {
  char *p=nm ;
  if (strlen(p)>1 && *(p+1)!='/')
  {
   memmove(p+2,p+1,strlen(p)) ;
   *(p+1)='/' ;
   p=p+2 ;
  }
  if (strlen(p)>1 && *(p+1)!='/')
  {
   memmove(p+2,p+1,strlen(p)) ;
   *(p+1)='/' ;
  }
 }


 char *n;
 int done;
 for(done=0;!done;)
 {
  for(n=nm;*n && *n!='/';n++);
  if(!*n) done=1;
  else *n=0;
  if(!isalpha(*nam) && l<=2)
  {
    char *t=nm;
    for(;*t && isdigit(*t);t++);
    if((t-nm)>2) res[0]='Z';
  }
  if(res[0]=='Z') l=3;
  NormalizeItem(nm,l,res+strlen(res));

  if(l==2) l=3;
  if (l==1)
    if (l_flag)
     l=3 ;
    else
     l_flag=1 ;

  if(!done) { strcat(res,"/"); nm=n+1; }
 }

 if (!strcmp(res,"A/00"))
  strcpy(res,"A") ;

 if (!strcmp(res,"B/00"))
  strcpy(res,"B") ;

 if(onam) strcpy(onam,res);
 else strcpy(nam,res);
}

void AccRec::SetName(const char *nam)
{
 char buf[40];
 strcpy(buf,nam);
 Normalize(buf);
 name=(::string)buf;
}

Cat_Obj_Rec::Cat_Obj_Rec() {}

#pragma argsused
void Cat_Obj_Rec::MakeLink(FieldLink &lnk, int inp, int sn)
{
 static FieldIntDef flds[]={
      {"ROW_ID",STRING_TYPE,19,(OraField FieldSet::*)&Cat_Obj_Rec::row_id },
      {"CATID",INT_TYPE,sizeof(int),(OraField FieldSet::*)&Cat_Obj_Rec::catid },
      {"ORD",INT_TYPE,sizeof(int),(OraField FieldSet::*)&Cat_Obj_Rec::ord },
      {"DESCR",STRING_TYPE,0,(OraField FieldSet::*)&Cat_Obj_Rec::descr },
      {"RETC",INT_TYPE,0,(OraField FieldSet::*)&Cat_Obj_Rec::retc, "NEWCAT" }
 };
 const int fldnum=sizeof(flds)/sizeof(flds[0]);
 lnk.fld=0;
 lnk.fldNum=-1;
 for(int i=0;i<fldnum;i++) if(flds[i].Set(lnk)) return;
}

#pragma argsused
void CatRec::MakeLink(FieldLink &lnk, int inp, int sn)
{
 static FieldIntDef flds[]={
      {"DBID",INT_TYPE,sizeof(int),(OraField FieldSet::*)&CatRec::dbid },
      {"OTYPE",INT_TYPE,sizeof(int),(OraField FieldSet::*)&CatRec::otype },
      {"OWNCAT",INT_TYPE,sizeof(int),(OraField FieldSet::*)&CatRec::owncat }
 };
 const int fldnum=sizeof(flds)/sizeof(flds[0]);
 lnk.fld=0;
 lnk.fldNum=-1;
 for(int i=0;i<fldnum;i++) if(flds[i].Set(lnk)) return;
 Cat_Obj_Rec::MakeLink(lnk,inp,sn);
}

CatTreeRec::CatTreeRec() {}

#pragma argsused
void CatTreeRec::MakeLink(FieldLink &lnk, int inp, int sn)
{
 static FieldIntDef flds[]={
      {"LEV",INT_TYPE,sizeof(int),(OraField FieldSet::*)&CatTreeRec::lev },
      {"TORD",INT_TYPE,sizeof(int),(OraField FieldSet::*)&CatTreeRec::tord },
      {"LAST",INT_TYPE,sizeof(int),(OraField FieldSet::*)&CatTreeRec::last },
      {"LEVM",INT_TYPE,sizeof(int),(OraField FieldSet::*)&CatTreeRec::levm }
 };
 const int fldnum=sizeof(flds)/sizeof(flds[0]);
 lnk.fld=0;
 lnk.fldNum=-1;
 for(int i=0;i<fldnum;i++) if(flds[i].Set(lnk)) return;
 CatRec::MakeLink(lnk,inp,sn);
}

ObjRec::ObjRec() {};
#pragma argsused

void ObjRec::MakeLink(FieldLink &lnk, int inp, int sn)
{
 static FieldIntDef flds[]={
      {"OTYPE",INT_TYPE,sizeof(int),(OraField FieldSet::*)&ObjRec::otype },
      {"OID",INT_TYPE,sizeof(int),(OraField FieldSet::*)&ObjRec::oid },
      {"NAME",STRING_TYPE,0,(OraField FieldSet::*)&ObjRec::name },
      {"OPTS",STRING_TYPE,0,(OraField FieldSet::*)&ObjRec::opts},
      {"DUP",INT_TYPE,sizeof(int),(OraField FieldSet::*)&ObjRec::dup},
      {"AUTON",INT_TYPE,sizeof(int),(OraField FieldSet::*)&ObjRec::autonum},
      {"POPTS",INT_TYPE,sizeof(int),(OraField FieldSet::*)&ObjRec::popts},
      {"RANGE",INT_TYPE,sizeof(int),(OraField FieldSet::*)&ObjRec::range},
      {"HIST",INT_TYPE,sizeof(int),(OraField FieldSet::*)&ObjRec::hist},
      {"CATS",INT_TYPE,sizeof(int),(OraField FieldSet::*)&ObjRec::cats},
      {"PARENT",INT_TYPE,sizeof(int),(OraField FieldSet::*)&ObjRec::parent},
      {"CONST_TYPE",INT_TYPE,sizeof(int),(OraField FieldSet::*)&ObjRec::const_type},
      {"VALUE",STRING_TYPE,0,(OraField FieldSet::*)&ObjRec::value},
 };
 const int fldnum=sizeof(flds)/sizeof(flds[0]);
 lnk.fld=0;
 lnk.fldNum=-1;
 for(int i=0;i<fldnum;i++)
    if(flds[i].Set(lnk)) return;
 Cat_Obj_Rec::MakeLink(lnk,inp,sn);
}


// Conds
#pragma argsused
void CondRec::MakeLink(FieldLink &lnk, int inp, int sn)
{
 static FieldIntDef flds[]={
      {"ROW_ID",STRING_TYPE,19,(OraField FieldSet::*)&CondRec::row_id },
      {"COND_ID",INT_TYPE,sizeof(int),(OraField FieldSet::*)&CondRec::cond_id},
      {"COND_SUBID",INT_TYPE,sizeof(int),(OraField FieldSet::*)&CondRec::cond_subid},
      {"ORD",INT_TYPE,sizeof(int),(OraField FieldSet::*)&CondRec::ord},
      {"TEXT",STRING_TYPE,0,(OraField FieldSet::*)&CondRec::text},
      {"DOC_TYPE",STRING_TYPE,0,(OraField FieldSet::*)&CondRec::doc_type},
      {"STORNO",INT_TYPE,sizeof(int),(OraField FieldSet::*)&CondRec::storno},
      {"NATIVE_DOC",INT_TYPE,sizeof(int),(OraField FieldSet::*)&CondRec::native_doc},
      {"AUTO_APPLY",INT_TYPE,sizeof(int),(OraField FieldSet::*)&CondRec::auto_apply},
      {"FOR_POST",INT_TYPE,sizeof(int),(OraField FieldSet::*)&CondRec::for_post},
      {"MAX_ID",INT_TYPE,sizeof(int),(OraField FieldSet::*)&CondRec::max_id},
      {"PRE_FORMULA",STRING_TYPE,0,(OraField FieldSet::*)&CondRec::pre_formula},
 };
 const int fldnum=sizeof(flds)/sizeof(flds[0]);
 lnk.fld=0;
 lnk.fldNum=-1;
 for(int i=0;i<fldnum;i++) if(flds[i].Set(lnk)) return;
}

#pragma argsused
void CondItemRec::MakeLink(FieldLink &lnk, int inp, int sn)
{
 static FieldIntDef flds[]={
      {"ROW_ID",STRING_TYPE,19,(OraField FieldSet::*)&CondItemRec::row_id },
      {"COND_ID",INT_TYPE,sizeof(int),(OraField FieldSet::*)&CondItemRec::cond_id},
      {"COND_SUBID",INT_TYPE,sizeof(int),(OraField FieldSet::*)&CondItemRec::cond_subid},
      {"CODE",INT_TYPE,sizeof(int),(OraField FieldSet::*)&CondItemRec::code},
      {"SETNAME",STRING_TYPE,0,(OraField FieldSet::*)&CondItemRec::setname},
      {"SIDE",STRING_TYPE,0,(OraField FieldSet::*)&CondItemRec::side},
      {"FOR_POST",INT_TYPE,sizeof(int),(OraField FieldSet::*)&CondItemRec::for_post},
      {"ITEM_NUM",INT_TYPE,sizeof(int),(OraField FieldSet::*)&CondItemRec::item_num},
      {"ITEM_FORMULA",STRING_TYPE,0,(OraField FieldSet::*)&CondItemRec::item_formula},
      {"MAX_ITEM",INT_TYPE,sizeof(int),(OraField FieldSet::*)&CondItemRec::max_item},
 };
 const int fldnum=sizeof(flds)/sizeof(flds[0]);
 lnk.fld=0;
 lnk.fldNum=-1;
 for(int i=0;i<fldnum;i++) if(flds[i].Set(lnk)) return;
}

#pragma argsused
void AuxRec::MakeLink(FieldLink &lnk, int inp, int sn)
{
 static FieldIntDef flds[]={
      {"ROW_ID",STRING_TYPE,19,(OraField FieldSet::*)&AuxRec::row_id },
      {"STR1",STRING_TYPE,0,(OraField FieldSet::*)&AuxRec::str1,"STR" },
      {"STR2",STRING_TYPE,0,(OraField FieldSet::*)&AuxRec::str2 },
      {"STR3",STRING_TYPE,32,(OraField FieldSet::*)&AuxRec::str3 },
      {"INT1",INT_TYPE,0,(OraField FieldSet::*)&AuxRec::int1,"INT" },
      {"INT2",INT_TYPE,0,(OraField FieldSet::*)&AuxRec::int2 },
      {"INT3",INT_TYPE,0,(OraField FieldSet::*)&AuxRec::int3 },
      {"DATE1",DATE_TYPE,0,(OraField FieldSet::*)&AuxRec::date1,"DATE" },
      {"DATE2",DATE_TYPE,0,(OraField FieldSet::*)&AuxRec::date2 },
      {"SUM1",STRING_TYPE,34,(OraField FieldSet::*)&AuxRec::sum1,"SUM" },
      {"SUM2",STRING_TYPE,34,(OraField FieldSet::*)&AuxRec::sum2 }
 };
 const int fldnum=sizeof(flds)/sizeof(flds[0]);
 lnk.fld=0;
 lnk.fldNum=-1;
 for(int i=0;i<fldnum;i++) if(flds[i].Set(lnk)) return;
 FieldDynaSet::MakeLink(lnk,inp,sn);
}

#pragma argsused
void ObjPrivRec::MakeLink(FieldLink &lnk,int inp,int sn)
{
 static FieldIntDef flds[]={
      {"ROW_ID",STRING_TYPE,19,(OraField FieldSet::*)&ObjPrivRec::row_id },
      {"OID",INT_TYPE,0,(OraField FieldSet::*)&ObjPrivRec::oid },
      {"RUNAME",STRING_TYPE,0,(OraField FieldSet::*)&ObjPrivRec::runame ,"UNAME"},
      {"NAME",STRING_TYPE,0,(OraField FieldSet::*)&ObjPrivRec::name },
      {"RU",INT_TYPE,0,(OraField FieldSet::*)&ObjPrivRec::ru },
      {"PR0",INT_TYPE,0,(OraField FieldSet::*)&ObjPrivRec::pr0 },
      {"PR1",INT_TYPE,0,(OraField FieldSet::*)&ObjPrivRec::pr1 },
      {"PR2",INT_TYPE,0,(OraField FieldSet::*)&ObjPrivRec::pr2 },
      {"PR3",INT_TYPE,0,(OraField FieldSet::*)&ObjPrivRec::pr3 },
      {"PR4",INT_TYPE,0,(OraField FieldSet::*)&ObjPrivRec::pr4 },
      {"PR5",INT_TYPE,0,(OraField FieldSet::*)&ObjPrivRec::pr5 }
 };
 const int fldnum=sizeof(flds)/sizeof(flds[0]);
 lnk.fld=0;
 lnk.fldNum=-1;
 for(int i=0;i<fldnum;i++) if(flds[i].Set(lnk)) return;
}

void OraErr(OraError &ERR)
{
   ERR.message(1,1);
}

void OraError::message(int roll,int code,char *point)
{
 HeapCheck(point);
 if(roll)
   conn.rollback();
 if(code)
 {
   char s[1024] ;
   sprintf(s,"%d %.1000s",err_code,why().data());
   ::message(s,BAD) ;
 }
 else
   ::message((char *)why().data(),BAD) ;
}



