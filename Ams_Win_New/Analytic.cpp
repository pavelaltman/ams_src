#include "AMS_HDR.h"
#pragma hdrstop

#include "amsclnt.h"
#include "analytic.h"

extern r_char itogo_cap,debet_cap,credit_cap ;

int TotVer=0;
char TotArt[]=" " ;
DateTime TotRDate=DateTime(1,1,97) ;

FInfoRec::FInfoRec() {}
FInfoRec::~FInfoRec() {}

#pragma argsused
void FInfoRec::MakeLink(FieldLink &lnk, int inp, int sn)
{
 static FieldIntDef flds[]={
      {"ROW_ID",STRING_TYPE,19,(OraField FieldSet::*)&FInfoRec::row_id },
      {"DBID",INT_TYPE,0,(OraField FieldSet::*)&FInfoRec::dbId },
      {"REALNAME",STRING_TYPE,0,(OraField FieldSet::*)&FInfoRec::RealName },
      {"VTYPE",INT_TYPE,0,(OraField FieldSet::*)&FInfoRec::vtype },
      {"RDBID",INT_TYPE,0,(OraField FieldSet::*)&FInfoRec::ref_dbId },
      {"MAXLEN",INT_TYPE,0,(OraField FieldSet::*)&FInfoRec::maxlen },
      {"DELETED",INT_TYPE,0,(OraField FieldSet::*)&FInfoRec::deleted },
      {"ALIAS",STRING_TYPE,0,(OraField FieldSet::*)&FInfoRec::Alias },
      {"DISP_OPT",INT_TYPE,0,(OraField FieldSet::*)&FInfoRec::disp_opt },
      {"WIDTH",INT_TYPE,0,(OraField FieldSet::*)&FInfoRec::width },
      {"DEC",INT_TYPE,0,(OraField FieldSet::*)&FInfoRec::dec },
      {"ORD",INT_TYPE,0,(OraField FieldSet::*)&FInfoRec::ord },
      {"DEF_OPT",INT_TYPE,0,(OraField FieldSet::*)&FInfoRec::def_opt },
      {"OPT",INT_TYPE,0,(OraField FieldSet::*)&FInfoRec::opt },
      {"DB_SETNAME",STRING_TYPE,0,(OraField FieldSet::*)&FInfoRec::DB_SetName},
      {"DBSET",STRING_TYPE,0,(OraField FieldSet::*)&FInfoRec::dbset},
      {"LINES",INT_TYPE,0,(OraField FieldSet::*)&FInfoRec::lines },
 };
 const int fldnum=sizeof(flds)/sizeof(flds[0]);
 lnk.fld=0;
 lnk.fldNum=-1;
 for(int i=0;i<fldnum;i++) if(flds[i].Set(lnk)) return;
}

#pragma argsused
void DBOptRec::MakeLink(FieldLink &lnk, int inp, int sn)
{
 static FieldIntDef flds[]={
      {"ROW_ID",STRING_TYPE,19,(OraField FieldSet::*)&DBOptRec::row_id },
      {"DBID",INT_TYPE,0,(OraField FieldSet::*)&DBOptRec::dbId },
      {"SETNAME",STRING_TYPE,0,(OraField FieldSet::*)&DBOptRec::SetName }
 };
 const int fldnum=sizeof(flds)/sizeof(flds[0]);
 lnk.fld=0;
 lnk.fldNum=-1;
 for(int i=0;i<fldnum;i++) if(flds[i].Set(lnk)) return;
}

#pragma argsused
void DBAddInfoRec::MakeLink(FieldLink &lnk, int inp, int sn)
{
 static FieldIntDef flds[]={
      {"ROW_ID",STRING_TYPE,19,(OraField FieldSet::*)&DBAddInfoRec::row_id },
      {"DBID",INT_TYPE,0,(OraField FieldSet::*)&DBAddInfoRec::dbId },
      {"SETNAME",STRING_TYPE,0,(OraField FieldSet::*)&DBAddInfoRec::SetName },
      {"REALNAME",STRING_TYPE,0,(OraField FieldSet::*)&DBAddInfoRec::RealName },
      {"NAME",STRING_TYPE,0,(OraField FieldSet::*)&DBAddInfoRec::name },
      {"CODE",INT_TYPE,0,(OraField FieldSet::*)&DBAddInfoRec::code },
      {"TURNTYPE",STRING_TYPE,0,(OraField FieldSet::*)&DBAddInfoRec::turntype},
      {"ALIAS",STRING_TYPE,0,(OraField FieldSet::*)&DBAddInfoRec::alias},
      {"SUMID",INT_TYPE,0,(OraField FieldSet::*)&DBAddInfoRec::sumid },
      {"ACCNAME",STRING_TYPE,0,(OraField FieldSet::*)&DBAddInfoRec::accname},
 };
 const int fldnum=sizeof(flds)/sizeof(flds[0]);
 lnk.fld=0;
 lnk.fldNum=-1;
 for(int i=0;i<fldnum;i++) if(flds[i].Set(lnk)) return;
}


FInfo::FInfo(const FInfoRec & FIRec,int _db)
{
 db=_db;
 RealName=FIRec.RealName;
 vtype=(gcl::VTYPE)FIRec.vtype.Val();
 ref_dbId=FIRec.ref_dbId;
// if(ref_dbId) ref=new DBStructInfo(ref_dbId);

 maxlen=FIRec.maxlen;
 deleted=FIRec.deleted;
 Alias=FIRec.Alias ;
 disp_opt=FIRec.disp_opt;
 width=FIRec.width;
// if(disp_opt>width) disp_opt=width;
 dec=FIRec.dec;
 ord=FIRec.ord;
 def_opt=FIRec.def_opt;
 DB_SetName=FIRec.DB_SetName ;
 opt=FIRec.opt.isNull()?def_opt:(int)FIRec.opt;
 dbset=FIRec.dbset.isNull()?DB_SetName:FIRec.dbset.val;
}
/*
int FInfo::Opt()
{
  for(int i=0;i<overs.size();i++)
   if(overs[i].SetName==set) return overs[i].opt;
  return def_opt;
}
*/
int  FInfo::Align()
{
  if(vtype==gcl::VT_NUM || vtype==gcl::VT_INT) return SA_RIGHT;
  return SA_LEFT;
}

int def_num_width=MAXSUMLEN ;

int  FInfo::DispLen(int use_len)
{
 if(use_len)
 {
   if((Opt()|disp_opt)&Hidden) return 0;
   return ((disp_opt&DebetCredit)?2:1)*width;
//   return disp_opt;
 }

/*
 if(Opt()&SystemFld)
 {
  return 0;
 }
*/

 switch(vtype) {
  case gcl::VT_INT: return 8;
  case gcl::VT_NUM: return def_num_width;
  case gcl::VT_STRING: return maxlen;
  case gcl::VT_DATE: return 8;
 }
 return MAXSUMLEN ;
}

void FInfo::OutputHeader(char *str,int use_len)
{
 if(!use_len)
 {
   strcpy(str,Alias.data());
   return;
 }
 if((Opt()|disp_opt)&Hidden)
 {
  *str=0;
  return;
 }
 if(disp_opt&DebetCredit)
 {
   ::string h;
   h=Alias+debet_cap;
   stralign(str,(char *)h.data(),width,Align());
   h=Alias+credit_cap;
   stralign(str+width,(char *)h.data(),width,Align());
 }
 else
  stralign(str,(char *)Alias.data(),width,Align());
// stralign(str,(char *)Alias.data(),disp_opt,Align());
 return;
}

void FInfo::OutputFld(FieldSet &rec,char *str,int use_len,int flags,int point)
{
 if(use_len && ((Opt()|disp_opt)&Hidden))
 {
  *str=0;
  return;
 }
 OraField *fld=rec.FldN(RealName.data());
 if(!fld) throw(OraError(0,::string(" Undefined field ")+RealName));

 int len=DispLen(use_len);
 int align=Align();

 char buf[280];
 buf[0]=0;

 if(fld->isNull()) *buf=0;
 else
   fld->Output(buf,flags,dec,point) ;
 if(use_len)
  if(disp_opt&DebetCredit)
  {
   fld->OutputPval(buf,DEBET,flags,dec);
   stralign(str,buf,width,align);
   fld->OutputPval(buf,CREDIT,flags,dec);
   stralign(str+width,buf,width,align);
  }
  else stralign(str,buf,len,align);
 else strcpy(str,buf);
}

void FInfo::OutputTotal(FieldSet &rec,char *str)
{
 if((Opt()|disp_opt)&Hidden) { *str=0; return; }

 if(!(Opt()&Totalable) || db!=2)
 {
  sprintf(str,"%-*s",DispLen(1),"");
 }
 else OutputFld(rec,str,1);
}

int FInfo::InputFld(FieldSet &rec,const char *str,char *errmes)
{
 OraField *fld=rec.FldN(RealName.data());
 if (!fld) return 0;
 if(*str) return fld->Input(str,dec,errmes);
 else fld->SetNull();
 return 1;
}

FInfo NullFInfo;
FInfo &DBStructInfo::FI(const ::string &rname)
{
  for(int i=0;i<fis.size();i++)
   if(fis[i].RealName==rname) return fis[i];
  return NullFInfo;
}

extern char doc_path[];

void DBStructInfo::Init(cursor *crsr,ObjRec &obj,const ::string &_set,int flags)
{
  try
  {
   SetAux();
   synt_sum=0;
   bal_fld=0;
   set=_set;
   if(obj.otype==OT_DB || obj.otype==OT_STRUCT)
    { dbid=obj.oid; analid=-1; dbname=obj.name; }
   if(obj.otype==OT_ANAL)
   {
    analid=obj.oid;
    dbid=obj.parent;
    analname=obj.name;
    OraDML DML(crsr);
    AuxRec aux;
    aux.int1=dbid;
    DML.DoPL("select name into :str1 from *.objs where oid=:INT1;",aux);
    dbname=aux.str1;
   }
   FInfoRec FIRec;
   OraView FI(crsr,"*.DBSTR DBS,*.DBOPTOV OV","DBS.*,OV.OPT AS OPT,OV.DB_SETNAME AS DBSET");
   FI.MakeRecord(FIRec);
   FIRec.dbId=dbid;
   FIRec.Alias=set;
   FI.Do(" WHERE DBS.DBID=:DBID AND OV.DBID(+)=DBS.DBID AND "
         "OV.REALNAME(+)=DBS.REALNAME AND OV.SETNAME(+)=:ALIAS"
         " AND DELETED=0 ORDER BY ORD",FIRec);
   while(FI.fetch(FIRec))
   {
     FInfo f(FIRec,1);
     rus_strupr((char*)(f.RealName.data())) ;
     f.OrigName=f.RealName;
     fis.push_back(f);
   }
   dbfldNum=fis.size();

   {
    ProgFile pf;
    char fname[256];
    sprintf(fname,"%s\\%s.prg",doc_path,dbname.data());
    pf.Init(fname);
    try {
    db_prog.Compile(&pf);
    db_prog.Link(doc_path);
    } catch(...) {}
   }

   show_dc=(analid>=0 && (flags&ShowDebetCredit));
   show_rem=0;
   if(analid>=0 && (flags&ShowRemFld))
   {
    FIRec.dbId=analid;
    FIRec.Alias=set;
    FI.RepeatDo(FIRec);
    while(FI.fetch(FIRec))
    {
     FInfo f(FIRec,4);
     if(f.Opt()&RemFld)
     {
      show_rem=1;
      f.OrigName=f.RealName;
      f.RealName=::string("R_")+f.RealName;
      fis.push_back(f);
     }
    }
   }

   arnum=0 ;
   if(analid>=0)
   {
    // Additional info
    try
    {
     if (set>"")
     {
      cursor crsr1(conn) ;
      OraView ait(&crsr1,"*.DBADDINFO") ;
      DBAddInfoRec airec ;

      cursor scrsr(conn) ;
      OraDML Q(&scrsr) ;
      AuxRec aux ;
      aux.date1=G.ur.beg_date ;
      aux.date2=G.ur.end_date ;

      ait.MakeRecord(airec) ;
      airec.dbId=analid ;
      airec.SetName=set ;
      ait.Do(" WHERE dbid=:dbid AND setname=:setname",airec) ;
      for(arnum=0 ; ait.fetch(airec) ; arnum++)
      {
       arecs[arnum]=airec ;

       aux.int1=airec.code ;
       if (airec.turntype=="BR")
       {
        Q.DoPL(" SELECT end_val_id INTO :int2 FROM *.rems "
               "   WHERE rem_date=:date1 AND code=:int1;",aux) ;
       }
       if (airec.turntype=="R")
       {
        Q.DoPL(" SELECT end_val_id INTO :int2 FROM *.rems "
               "   WHERE rem_date=:date2 AND code=:int1;",aux) ;
       }
       if (airec.turntype=="DT")
       {
        Q.DoPL(" SELECT d_turn_id INTO :int2 FROM *.turns "
               "   WHERE beg_date=:date1 AND end_date=:date2 "
               "   AND code=:int1;",aux) ;
       }
       if (airec.turntype=="CT")
       {
        Q.DoPL(" SELECT c_turn_id INTO :int2 FROM *.turns "
               "   WHERE beg_date=:date1 AND end_date=:date2 "
               "   AND code=:int1;",aux) ;
       }
       arecs[arnum].sumid=aux.int2 ;

       {
        FIRec.dbId=analid;
        FIRec.Alias=set;
        FI.RepeatDo(FIRec);
        while(FI.fetch(FIRec))
        {
         FInfo f(FIRec,8);
         if(FIRec.RealName==airec.RealName)
         {
          f.OrigName=f.RealName;
          f.RealName=airec.name+::string("_")+f.RealName;
          f.Alias=airec.alias ;
          arecs[arnum].opt=f.opt ;
          fis.push_back(f);
         }
        }
       }

      }
     }
    }
    catch(OraError err)
    {
     // err.message() ;
    }

    // Analytic fieds
    FIRec.dbId=analid;
    FIRec.Alias=set;
    FI.RepeatDo(FIRec);
    FirstAnalField=fis.size();
    while(FI.fetch(FIRec))
    {
     FInfo f(FIRec,2);
     f.OrigName=f.RealName;
     if((f.disp_opt&DebetCredit) && !show_dc) f.disp_opt&=~DebetCredit;
     fis.push_back(f);
    }


    {
     ProgFile pf;
     char fname[256];
     sprintf(fname,"%s\\%s.prg",doc_path,analname.data());
     pf.Init(fname);
     try {
      if (an_prog.Compile(&pf))
        an_prog.Link(doc_path);
     } catch(...) {}
    }
   }
  }
  catch(OraError err)
  {
   err.message() ;
  }
  {
    AuxRec aux;
    DBOptRec or;
    or.dbId=dbid;

    OraView SET(crsr,"*.DBOPT");
    SET.MakeRecord(or);
    SET.Do(" WHERE DBID=:DBID",or);
    while(SET.fetch(or)) osets.push_back(or.SetName);
/*
    OraView OOV(&crsr,"*.DBOPTOV","REALNAME as str2,OPT as int1");
    OOV.MakeRecord(aux);
    aux.int1=dbid;
    aux.str1=set;
    OOV.Do(" WHERE DBID=:INT1 AND SETNAME=:STR1",aux);
    while(OOV.fetch(aux))
    {
     OptOver oo;
     oo.SetName=aux.str1;
     oo.opt=aux.int1;
     FI(aux.str2).overs.push_back(oo);
    }
*/
  }
}

::string DBStructInfo::AnalView()
{
 ::string ret ;

 if (analid!=-1)
 {
  ret=::string("*.")+analname+" AN,*."+dbname+" DB" ;

  if (show_rem)
    ret+=",*."+analname+" REM" ;

  for(int i=0 ; i<arnum ; i++)
    ret+=",*."+analname+" "+arecs[i].name.Val() ;
 }
 else
 {
  ret=::string("*.")+dbname+" DB" ;
 }

 ret+=",*.DBCATTREE C" ;
 return ret ;
}

::string DBStructInfo::AnalHeader()
{
 ::string ret=""; //" /*+ ORDERED */ ";
// if(!show_rem) return ret+"*";
// else

 {
  ret+="DB.*";//::string("*.")+dbname+".*";
  if (show_rem)
  {
   int i;
   for(i=0;i<NFields();i++)
   {
     if(fis[i].db==4)
     {
      ret+=::string(", REM.")+fis[i].OrigName+" "+fis[i].RealName;
     }
   }
  }

  if (analid!=-1)
  {
   ret+=", AN.*";//::string(",*.")+analname+".*";

   for(int i=0 ; i<arnum ; i++)
     ret+=::string(", ")+arecs[i].name.Val()+"."+arecs[i].RealName.Val()+" "+
            arecs[i].name.Val()+"_"+arecs[i].RealName.Val() ;
  }

  ret+=", C.TORD" ;
 }

 return ret;
}

::string DBStructInfo::DBExHeader(int cat)
{
  ::string ret;
  int i;

  if(cat)
    ret="TORD,ARTICLE,MAX(DB.CATID) AS CATID";
  else
    ret="MAX(DB.CATID) AS CATID,MAX(TORD) AS TORD,ARTICLE";

  for(i=2;i<NFields();i++)
  {
    switch(fis[i].db) {
     case 1:
       ret+=::string(",MAX(")+fis[i].RealName+") as "+fis[i].RealName;
       break;
     case 2:
       if(fis[i].Opt()&Addable)
        ret+=::string(",SUM(AN.")+fis[i].RealName+") as "+fis[i].RealName;
       else
        ret+=::string(",MAX(AN.")+fis[i].RealName+") as "+fis[i].RealName;
        break;
     case 4:
       if(fis[i].Opt()&Addable)
        ret+=::string(",SUM(REM.")+fis[i].OrigName+") as "+fis[i].RealName;
       else
        ret+=::string(",MAX(REM.")+fis[i].OrigName+") as "+fis[i].RealName;
       break;
    }
  }

  for(int i=0 ; i<arnum ; i++)
  {
   if(arecs[i].opt&Addable)
        ret+=::string(",SUM(")+arecs[i].name.Val()+"."+arecs[i].RealName.Val()+
            ") as "+arecs[i].name.Val()+"_"+arecs[i].RealName.Val() ;
   else
        ret+=::string(",MAX(")+arecs[i].name.Val()+"."+arecs[i].RealName.Val()+
            ") as "+arecs[i].name.Val()+"_"+arecs[i].RealName.Val() ;
  }

  return ret;
}


::string DBStructInfo::AnExHeader()
{
  ::string ret;
  int i;
  ret="SUMID,AARTICLE,MAX(AVER)";
  for(i=2;i<NFields();i++)
  {
    switch(fis[i].db) {
     case 2:
       if(fis[i].Opt()&SystemFld) break;
       if(fis[i].Opt()&Addable)
        ret+=::string(",SUM(")+fis[i].RealName+") as "+fis[i].RealName;
       else
        ret+=::string(",MAX(")+fis[i].RealName+") as "+fis[i].RealName;
        break;
    }
  }
//  ret+=", MAX(C.TORD) AS TORD" ;
  return ret;
}

::string DBStructInfo::DBExGroup(int cat)
{
  if(cat)
   return "TORD,article";
  else
   return "article";
}

::string DBStructInfo::AnalCond(int cat,int outer,Code SumId,Code RemId)
{
 char b[20];
 ::string eid,rid;
 itoa(SumId,b,10);
 eid=b;
 itoa(RemId,b,10);
 rid=b;

 ::string res="ARTICLE<>' ' AND ";
 if(cat) res+="DB.CATID=:CATID AND ";
 res+="C.OWNCAT=DB.CATID AND " ;

 for(int i=0 ; i<arnum ; i++)
 {
  char bb[20];
  ::string sid;
  memset(bb,0,19) ;
  itoa(arecs[i].sumid,bb,10);
  sid=bb ;
  res+="ARTICLE="+arecs[i].name.Val()+".AARTICLE(+) AND VER="+arecs[i].name.Val()+
    ".AVER(+) AND "+arecs[i].name.Val()+".SUMID(+)="+sid+" AND " ;
 }

 if(!show_rem || !RemId)
  if(outer)
  return res+
   "ARTICLE = AN.AARTICLE(+) AND VER = AN.AVER(+) AND AN.SUMID (+)="+eid;
  else
  return res+
   "ARTICLE = AN.AARTICLE AND VER = AN.AVER AND AN.SUMID="+eid;

 if(outer==1)
  return res+
  "ARTICLE = AN.AARTICLE(+) AND VER = AN.AVER(+) AND AN.SUMID (+)="+eid+
  " AND ARTICLE = REM.AARTICLE(+) AND VER = REM.AVER(+) AND REM.SUMID(+)="+rid;
 if(!outer)
  return res+
  "ARTICLE = AN.AARTICLE AND VER = AN.AVER AND AN.SUMID="+eid+
  " AND ARTICLE = REM.AARTICLE(+) AND VER = REM.AVER(+) AND REM.SUMID(+)="+rid;

  return res+"ARTICLE = AN.AARTICLE(+) AND VER = AN.AVER(+)"
        " AND ARTICLE = REM.AARTICLE(+) AND VER = REM.AVER(+)"
        " AND AN.SUMID(+)="+eid+"AND REM.SUMID(+)="+rid+
        " AND (AN.SUMID IS NOT NULL OR REM.SUMID IS NOT NULL)";
}

/*
Finfo * DBStructInfo::NextField(int &it)
{
  int minord=1000,fld=-1;
  for(int i=0;i<fis.size();i++)
  {
    if(!fis[i].deleted && fis[i].ord>it && fis[i].ord<minord)
    {
     fld=i;
     minord=fis[i].ord;
    }
  }
  if(fld>=0) { it=fis[fld].ord; return *fis[fld]; }
  return 0;
}
*/
void DBStructInfo::SumBuf(FieldSet &totrec,FieldSet &rec,int sign)
{
 for(int i=dbfldNum;i<fis.size();i++)
 {
  if(fis[i].db!=2) continue;
  if(!(fis[i].Opt() & SystemFld))
  {
   char const *fName=fis[i].RealName.data();
   OraField *fld=rec.FldN(fName);
   OraField *tfld=totrec.FldN(fName);
   if(!fld || !tfld)
     throw OraError(101,::string("Undefined field in SumBuf ")+fName);

   tfld->SetNotNull();
   switch(fis[i].vtype) {
    case gcl::VT_INT:
     {
      Field<int> *fldi,*tfldi;
      fldi=dynamic_cast< Field<int>* >(fld);
      tfldi=dynamic_cast< Field<int>* >(tfld);
      if(fldi && tfldi)
      {
       if(fis[i].Opt() & Addable)
          tfldi->val+=fldi->val*sign;
       else tfldi->val=fldi->val ;
       break;
      }
      if(tfldi)
      {
       if(fis[i].Opt() & Addable)
          tfldi->val+=dynamic_cast< Field<Sum>* >(fld)->val.dlv*sign;
       else
          tfldi->val=dynamic_cast< Field<Sum>* >(fld)->val.dlv ;
       break;
      }
      if(fldi)
      {
       if(fis[i].Opt() & Addable)
       {
        long double& LD =dynamic_cast< Field<Sum>* >(tfld)->val.dlv;
        LD +=fldi->val*sign;
       }
       else
         dynamic_cast< Field<Sum>* >(tfld)->val=fldi->val ;
      }
     }
    case gcl::VT_NUM:
       if(fis[i].Opt() & Addable)
       {
         long double& LD =dynamic_cast< Field<Sum>* >(tfld)->val.dlv;
         LD+=dynamic_cast< Field<Sum>* >(fld)->val.dlv*sign;
       }
       else
       {
         long double& LD =dynamic_cast< Field<Sum>* >(tfld)->val.dlv;
         LD=dynamic_cast< Field<Sum>* >(fld)->val.dlv*sign;
       }
    break;
   }
  }
 }
}

void DBStructInfo::CopyBuf(FieldSet &totrec,FieldSet &rec,int db)
{
 for(int i=0;i<dbfldNum ;i++)
 {
  if(fis[i].db==db)
  {
   char const *fName=fis[i].RealName.data();
   OraField *fld=rec.FldN(fName);
   OraField *tfld=totrec.FldN(fName);
   if(!fld || !tfld)
     throw OraError(101,::string("Undefined field in CopyBuf ")+fName);
   tfld->SetNotNull();
//   *tfld=*fld ;


   switch(fis[i].vtype) {
    case gcl::VT_INT:
     {
      Field<int> *fldi,*tfldi;
      fldi=dynamic_cast< Field<int>* >(fld);
      tfldi=dynamic_cast< Field<int>* >(tfld);
      if(fldi && tfldi)
      {
       tfldi->val=fldi->val ;
       break;
      }
      if(tfldi)
      {
       tfldi->val=dynamic_cast< Field<Sum>* >(fld)->val.dlv ;
       break;
      }
      if(fldi)
      {
       dynamic_cast< Field<Sum>* >(tfld)->val=fldi->val ;
      }
     }
    case gcl::VT_NUM:
        dynamic_cast< Field<Sum>* >(tfld)->val=
            dynamic_cast< Field<Sum>* >(fld)->val.dlv;

        break;
    case gcl::VT_STRING:
        dynamic_cast< Field<::string>* >(tfld)->val=
            dynamic_cast< Field<::string>* >(fld)->val;
        break;
   }

  }
 }
}


int DBStructInfo::DispLen(int use_len)
{
 int tlen=0;
 for(int i=0;i<fis.size();i++)
 {
//  if((fis[i].db & 4) && !_show_rem ) continue;
  int len=fis[i].DispLen(use_len);
  if(len) tlen+=len+1;
 }
 return tlen;
}

int DBStructInfo::FrozenLen()
{
 int tlen=0,flen=0;
 for(int i=0;i<fis.size();i++)
  if (fis[i].db==1)
  {
   int len=fis[i].DispLen();
   if(len) tlen+=len+1;
   if(fis[i].disp_opt&Frozen) flen=tlen;
  }
 return flen;
}

void DBStructInfo::OutputHeader(char *str,char *delim)
{
 *str=0;
 if(!delim) delim=" ";
 for(int i=0;i<fis.size();i++)
 {
//  if((fis[i].db & 4) && !_show_rem ) continue;
  if(fis[i].DispLen())
  {
   fis[i].OutputHeader(str+strlen(str));
   strcat(str,delim);
  }
 }
}



DBSubstInfo DBSI[10] ;
int dbsnum ;

DBChangeInfo DBCI[10] ;
int dbcnum ;

DBKeyInfo DBKI[10] ;
int dbknum ;

void DBStructInfo::OutputRec(FieldSet &rec,char *str,char *delim,int start)
{
 *str=0;
 if(!delim) delim=" ";
 int lastdb=0 , first=1 ;
 for(int _i=start;_i<fis.size();_i++)
 {
  int i=_i ;

  // Подстановка одного поля вместо другого
  for(int j=0 ; j<dbcnum ; j++)
    if (DBCI[j].dbid==dbid)
      if (DBCI[j].fld1==_i)
       i=DBCI[j].fld2 ;


  if(fis[i].DispLen() || (delim[0]==':' && (fis[i].RealName!="CATID") && (fis[i].RealName!="SUMID")&& (fis[i].RealName!="AARTICLE") && (fis[i].RealName!="AVER") && (fis[i].RealName!="AArticle") && (fis[i].RealName!="AVer")))
  {
    if (delim[0]==':' && !first)
       strcat(str,(lastdb==1) && (fis[_i].db==2) ? "|" : delim) ;

    char tstr[256] ;
    fis[i].OutputFld(rec,tstr,delim[0]!=':',delim[0]==':' ? 0 : GS_NOZERO);
    if (delim[0]==':')
    {
     while(char *p=strchr(tstr,':'))
       *p=' ' ;
    }

    if (fis[i].RealName=="ARTICLE")
      for(int j=0 ; j<dbsnum ; j++)
        if (DBSI[j].dbid==dbid)
         strcat(str,DBSI[j].prefix) ;
    if (fis[i].ref_dbId)
      for(int j=0 ; j<dbsnum ; j++)
        if (DBSI[j].dbid==fis[i].ref_dbId)
         strcat(str,DBSI[j].prefix) ;

    strcat(str,tstr) ;

    if (delim[0]!=':')
        strcat(str,delim);
    lastdb=fis[_i].db ;
    first=0 ;
  }
 }
}

void DBStructInfo::OutputTotal(FieldSet &rec,char *str,char *delim)
{
 *str=0;
 if(!delim) delim=" ";
 for(int i=0;i<fis.size();i++)
 {
//  if((fis[i].db & 4) && !_show_rem ) continue;
  if(fis[i].DispLen())
  {
   fis[i].OutputTotal(rec,str+strlen(str));
   strcat(str,delim);
  }
 }
}

void DBStructInfo::OutputInfo(FieldSet &rec,char *str)
{
 *str=0;
 for(int i=0;i<fis.size();i++)
 {
  if((fis[i].Opt()&ShowFld) && rec.FldN(fis[i].RealName.data()))
  {
   fis[i].OutputFld(rec,str+strlen(str),0);
   strcat(str," ");
  }
 }
}

int DBStructInfo::CheckRec(FieldSet &rec)
{
 for(int i=0;i<fis.size();i++)
 {
   if(fis[i].db==2 && fis[i].Opt()&ZestFld)
   {
    OraField *fld=rec.FldN(fis[i].RealName.data());
    if(!fld) throw(OraError(0,::string(" Undefined field ")+fis[i].RealName));
    if(!fld->isNull()) return 1;
   }
 }
 return 0;
}

::string DBStructInfo::InsertSTMT(int db,int hist)
{
  ::string sqls="INSERT INTO *.";
  ::string view=(db==1?dbname:analname);

  sqls+=view ;
  if (hist)
    sqls+="_H" ;
  sqls+="(";

  int i,k;
  for(k=i=0;i<NFields();i++)
  {
   if(FI(i).db&db)
   {
    if(k) sqls+=",";
    sqls+=FI(i).RealName;
    k++;
   }
  }
  sqls+=") VALUES (";
  for(k=i=0;i<NFields();i++)
  {
   if(FI(i).db&db)
   {
    if(k) sqls+=",";
    sqls+=":";
    sqls+=FI(i).RealName;
    k++;
   }
  }
  sqls+=")";
  return sqls;
}

::string DBStructInfo::DELSTMT(int db)
{
  return ::string("DELETE FROM *.")+(db==1?dbname:analname)+ByArtVer(db);
}

::string DBStructInfo::ByArtVer(int db)
{
  return (db==1)?" WHERE ARTICLE=:ARTICLE AND VER=:VER":
                 " WHERE AARTICLE=:ARTICLE AND AVER=:VER AND SUMID=:SUMID";
}

::string DBStructInfo::UpdateSTMT(int db,int only_free)
{
  ::string sqls="UPDATE *.";
  ::string &view=(db==1?dbname:analname);
  sqls+=view+" SET ";
  int i,k;
  for(k=i=0;i<NFields();i++)
  {
   if(FI(i).db&db && (!only_free || !(Opt(i)&NoUpdateImp)))
   {
    if(k) sqls+=", ";
    sqls+=FI(i).RealName;
    sqls+="=:";
    sqls+=FI(i).RealName;
    k++;
   }
  }
 sqls+=ByArtVer(db);
 return sqls;
}

::string DBStructInfo::ClearSTMT()
{
  ::string sqls="UPDATE *.";
  ::string &view=analname ;
  sqls+=view+" SET " ;
  int i,k;
  for(k=i=0;i<NFields();i++)
  {
   if(FI(i).db==2 && FI(i).opt&EditRecalc)
   {
    if(k) sqls+=", ";
    sqls+=FI(i).RealName;
    sqls+="=NULL";
    k++;
   }
  }
 sqls+=" WHERE SUMID=:SUMID";
 return sqls;
}

void DBStructInfo::RecalcSum(Code SumId)
{
 cursor crsr(conn);
 PosAnalBuf totbuf,rec;
 OraTable totv(&crsr,::string(::string("*.")+analname).data());
 totv.MakeRecord(totbuf) ;

 if(!totv.Do(" WHERE SUMID=:SUMID AND AARTICLE=:AARTICLE AND AVER=:AVER",
              totbuf,totbuf))
 {
   totbuf.SumId=SumId;
   totbuf.aarticle=TotArt;
   totbuf.aprVer=TotVer;
   totv.InsertRec(totbuf);
   totv.Do(" WHERE SUMID=:SUMID AND AARTICLE=:AARTICLE AND AVER=:AVER",
              totbuf,totbuf);
 }

 totv.MakeRecord(rec) ;
 rec.SumId=SumId;
 totbuf.SetNull();

 totv.Do(" WHERE SUMID=:SUMID AND AARTICLE>' '",rec);
 while(totv.fetch(rec))
 {
   SumBuf(totbuf,rec,1);
 }
 totv.UpdateRec_ID(totbuf);
}

void DBStructInfo::BeforeInsRec(FieldSet &rec,cursor *crsr)
{
 for(int i=0 ; i<NFields() ; i++)
 {
  if (FI(i).ref_dbId && FI(i).vtype==gcl::VT_INT)
  {
   if (!FLD(rec,i)->isNull())
   {
    OraDML Q(crsr) ;
    Q.DoPL("*.CREATESUM(:"+FI(i).RealName+",0);",rec) ;
   }
  }
 }
}

void PosBuf::MakeLink(FieldLink &lnk, int inp, int sn)
{
 static FieldIntDef flds[]={
      {"ROW_ID",STRING_TYPE,19,(OraField FieldSet::*)&PosBuf::row_id },
      {"CATID",INT_TYPE,0,(OraField FieldSet::*)&PosBuf::catId },
      {"TORD",INT_TYPE,0,(OraField FieldSet::*)&PosBuf::tord },
      {"ARTICLE",STRING_TYPE,0,(OraField FieldSet::*)&PosBuf::article },
      {"VER",INT_TYPE,0,(OraField FieldSet::*)&PosBuf::prVer }
 };
 const int fldnum=sizeof(flds)/sizeof(flds[0]);
 if(!_MakeLink(lnk,flds,fldnum,inp,sn)) FieldDynaSet::MakeLink(lnk);
}

void PosAnalBuf::MakeLink(FieldLink &lnk, int inp, int sn)
{
 static FieldIntDef flds[]={
      {"SUMID",INT_TYPE,0,(OraField FieldSet::*)&PosAnalBuf::SumId },
      {"AARTICLE",STRING_TYPE,0,(OraField FieldSet::*)&PosAnalBuf::aarticle },
      {"AVER",INT_TYPE,0,(OraField FieldSet::*)&PosAnalBuf::aprVer }
 };
 const int fldnum=sizeof(flds)/sizeof(flds[0]);
 if(!_MakeLink(lnk,flds,fldnum,inp,sn)) PosBuf::MakeLink(lnk,inp,sn);
}

void PosAnalBuf::InitTot(int _SumId)
{
 SetNull();
 SumId=_SumId;
 aarticle=TotArt;
 aprVer=TotVer;
}

int DBRootCat(int dbId,cursor *crsr)
{
  OraDML cv(crsr);
  CatRec cr;
  cr.dbid=dbId;
  cv.DoPL("SELECT owncat into :owncat FROM *.DBCAT WHERE DBID=:DBID AND CATID IS NULL; ",cr);
  return cr.owncat;
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
OpVars::OpVars(){MakeStruct();};
//-------------------------------------------------------------------
void OpVars::MakeCopy(const OpVars& OV)
{
    Struct* S1=OV.st;
    for (int i=0; i<S1->Size();i++)
    {
        Node* N= S1->Var(i);
        st->UpdateVar_Clone(N->name.data(), &(N->Fld()));
        st->IncC();
    }
};
//-------------------------------------------------------------------
void OpVars::AddVarInt(const AnsiString& Name_, int Val_, bool SetNull)
{
    Field<int>* OF = new Field<int>();
    if (!SetNull) (*OF)=Val_;
    Flds.Add(OF);
    st->UpdateVar_Fld(Name_.c_str(),OF);
    st->IncC();
};
//-------------------------------------------------------------------
void OpVars::AddVarSum(const AnsiString& Name_, Sum Val_, bool SetNull)
{
    Field<Sum>* OF = new Field<Sum>();
    if (!SetNull) (*OF)=Val_;
    Flds.Add(OF);
    st->UpdateVar_Fld(Name_.c_str(),OF);
    st->IncC();
};
//-------------------------------------------------------------------
void OpVars::AddVarDate(const AnsiString& Name_, TDateTime Val_, bool SetNull)
{
    Field<DateTime>* OF = new Field<DateTime>();
    if (!SetNull)(*OF)=Val_;
    Flds.Add(OF);
    st->UpdateVar_Fld(Name_.c_str(),OF);
    st->IncC();
};
//-------------------------------------------------------------------
void OpVars::AddVar(const AnsiString& Name_, OraField* Fld_)
{
    st->UpdateVar_Fld(Name_.c_str(),Fld_);
    st->IncC();
};
//-------------------------------------------------------------------
void OpVars::AddVarStr(const AnsiString& Name_, const AnsiString& Val_)
{
    Field<string>* OF = new Field<string>();
    (*OF)=ToString(Val_);
    Flds.Add(OF);
    st->UpdateVar_Fld(Name_.c_str(),OF);
    st->IncC();
};
//-------------------------------------------------------------------
void OpVars::AddVars (TOracleDataSet* ODS)
{
    for (int i= 0; i< ODS->Fields->Count;i++)
    {
        TField* Fld = ODS->Fields->Fields[i];
        switch (Fld->DataType)
        {
            case ftString:
                AddVarStr(Fld->FieldName, Fld->AsString);
            break;
            case ftInteger:
                AddVarInt(Fld->FieldName, Fld->AsInteger);
            break;
            case ftFloat:
                AddVarSum(Fld->FieldName, Fld->AsFloat);
            break;
            case ftDateTime:
                AddVarDate(Fld->FieldName, Fld->AsDateTime);
            break;
        }
    }
};
//-------------------------------------------------------------------
void OpVars::AddVars (AmsQuery& AQ)
{
    for (int i= 0; i< AQ()->FieldCount();i++)
    {
        switch (AQ()->FieldType(i))
        {
            case otInteger:
                AddVarInt(AQ()->FieldName(i), AQ.GetFldInt(i));
            break;
            case otFloat:
                if (!AQ()->FieldScale(i))
                    AddVarInt(AQ()->FieldName(i), AQ.GetFldInt(i));
                else
                    AddVarSum(AQ()->FieldName(i), AQ.GetFldSum(i));
            break;
            case otString:
                AddVarStr(AQ()->FieldName(i), AQ.GetFldStr(i));
            break;
            case otDate:
                AddVarDate(AQ()->FieldName(i), AQ.GetFldInt(i));
            break;
        }
    }
};
//-------------------------------------------------------------------
void OpVars::DelVar (const AnsiString& VarName)
{
    st->DelVar(VarName.c_str());
};
//-------------------------------------------------------------------
int OpVars::Init(OpRec &or,OraTable &ot,cursor *crsr)
{
  operdb=0;
  MakeOraRec(ot,or);
//  if(!or.cond_id) return;
  ObjRec obj;
  if (!GetLinkDB(crsr,obj,or))
     return 0 ;
  no_first= (obj.popts & DBO_NOFIRST) ;

  rootCat=DBRootCat(obj.oid,crsr);
  string dbn=string("*.")+obj.name.Val();
  DBStr.Init(crsr,obj,"",0);
  try
  {
   operdb=new OraTable(crsr,dbn.data());
  }
  catch(...)
    { return 0 ; }
  operdb->MakeRecord(opvars);
  DBStr.MakeStruct(This,opvars,1,0);
  return 1 ;
}

void OpVars::SetKey(OpRec &or,int norecalc)
{
  if(!operdb) return;
  opvars.article=or.doc_num;
  opvars.prVer=or.doc_num_v;
  if(opvars.prVer.isNull()) opvars.prVer=0;
  if(!operdb->Select("article=:article and ver=:ver",opvars,opvars))
  {
   opvars.SetNull() ;
   opvars.catId=rootCat;
   opvars.article=or.doc_num;
   opvars.prVer=or.doc_num_v;
   if(opvars.prVer.isNull()) opvars.prVer=0;

   if (!norecalc)
      DBStr.CalcDBLine(opvars);

   operdb->InsertRec(opvars);
   conn.commit() ;
   operdb->Select("article=:article and ver=:ver",opvars,opvars);
  }
}

