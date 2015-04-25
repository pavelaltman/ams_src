#include "AMS_HDR.h"
#pragma hdrstop

#include "orcpp.h"
#include <stdarg.h>
#include <ctype.h>

::string def_shema="NEWCOM";

void connection::rollback()
{
    DatMod->OSession->Rollback();
}

void connection::commit()
{
    DatMod->OSession->Commit();
}

/*
void FieldLinks::Store(FieldSet const &rec,ub1 *databuf,sb2 *indbuf) const
{
 for(int i=0;i<Num();i++)
 {
  if(flds[i].int_type==LONG_TYPE && flds[i].int_type==LONGRAW_TYPE) continue;
  OraField const *fld=const_cast<FieldSet &>(rec).Fld(flds[i]);
  if(!fld) *indbuf=-1;
  else if (fld->isNull()) *indbuf=-1;
  else
  {
   memset(databuf,0,flds[i].int_size) ;
   fld->Field2Buf(databuf);
   *indbuf=0;
  }
  indbuf++;
  databuf+=flds[i].int_size;
 }
}

void FieldLinks::Get(FieldSet &rec,ub1 *databuf,sb2 *indbuf) const
{
 for(int i=0;i<Num();i++)
 {
  if(flds[i].int_type==LONG_TYPE && flds[i].int_type==LONGRAW_TYPE) continue;
  FieldLink& FL = flds[i];
  OraField *fld=rec.Fld(flds[i]);
  if(fld)
   if(*indbuf>=0)
    fld->Buf2Field(databuf);
   else fld->SetNull();
  indbuf++;
  databuf+=flds[i].int_size;
 }
}
*/
void FieldLinks::Get(FieldSet &rec,cursor& Curs) const
{
 for(int i=0;i<Num();i++)
 {
  if(flds[i].int_type==LONG_TYPE && flds[i].int_type==LONGRAW_TYPE) continue;
  FieldLink& FL = flds[i];
  OraField *fld=rec.Fld(flds[i]);
  if(fld)
    fld->Buf2Field(Curs,FL);
   else
    //fld->SetNull()
    ;
 }
}

void FieldLinks::SetNull(FieldSet &rec) const
{
 for(int i=0;i<Num();i++)
 {
  OraField *fld=rec.Fld(flds[i]);
  if(fld)  fld->SetNull();
 }
 rec.SetNull();
}

int FieldLinks::isNull(FieldSet &rec) const
{
 for(int i=0;i<Num();i++)
 {
  OraField *fld=rec.Fld(flds[i]);
  if(fld)  if(!fld->isNull()) return 0;
 }
 return 1;
}

void OraDDL::Parse(const ::string &_stmt)
{
 stmt=_stmt;
 if(!curs)
   throw(OraError(-1,::string("CURSOR is UNDEFINED.\n")+stmt));
// int pos;
 const char *p;
// while((pos=stmt.find("*."))!=NPOS)
//  stmt.replace(pos,1,def_shema);
 //while((p=strstr(stmt.data(),"*."))!=NULL)
 // stmt.replace(p-stmt.data(),1,def_shema);

 curs->SetSqlText(ToAnsiStr(stmt));
 //curs->parse(stmt.data());
 //curs->display_error(stmt.data());
 //curs->ResetBufs();
 //curs->SetMark();
}


int OraDDL::execute()
{
 //if(!curs)
 //  throw(OraError(-1,::string("CURSOR is UNDEFINED.\n")+stmt));
 //sword er=curs->execute();
 //curs->display_error(stmt.data());
 int result = 0;
 if (Silent)
 {
    Silent = false;
    result=curs->ExecSilent();
 }
 else
    curs->Exec("");
 return result;
}

void OraView::AdjView()
{
 int pos;
 while((pos=view.find("*."))!=-1) view.replace(pos,1,def_shema);
 while((pos=header.find("*."))!=-1) header.replace(pos,1,def_shema);
}

::string OraView::SELSTMT()
{
 ::string sqls="SELECT "+header+" FROM "+view;
 return sqls;
}

void OraView::Describe()
{
 FieldLink def;
 curs->SetSqlText(ToAnsiStr(SELSTMT()+group));
 TOracleQuery * OQ= (*curs)();
 DescribeSQL(OQ, "");

 for (int i = 0; i<OQ->FieldCount(); i++)
 {
    strcpy ((char*)def.name, OQ->FieldName(i).c_str());
    def.FldNum=i;
    def.nullok=1;
    switch (OQ->FieldType(i))
    {
        case otString:
            def.dbtype= VARCHAR2_TYPE;
        break;
        case otFloat:
            def.precision=OQ->FieldPrecision(i);
            def.scale=OQ->FieldScale(i);
            def.dbtype= NUMBER_TYPE;
        break;
        case otInteger:
            def.dbtype= NUMBER_TYPE;
            def.precision=8;
            def.scale=0;
        break;
        case otDate:
            def.dbtype= DATE_TYPE;
            def.precision=0;
            def.scale=0;
        break;
    }
    flnks.Add(def);
  }
}

void OraView::MakeRecord(FieldSet &rec)
{
 for(int i=0;i<flnks.Num();i++)
    rec.MakeLink(flnks[i]);
 rec.Tie(this);
}

void OraView::SelectAll()
{
 //Parse(SELSTMT()+group);
 Define();
}

void OraDML::Bind(FieldSet const &recb)
{
 char varname[30];
 unsigned char *s,*e;
 sword in_quote;
 (*curs)()->DeleteVariables();
 args.Empty();
 for(s=(unsigned char *)stmt.data(),in_quote=0;*s;s++)
 {
   if(*s=='\'') in_quote=!in_quote;
   if(*s==':' && !in_quote)
   {
     for(e=s+1;*e && (isalnum(*e) || *e=='_');e++);
     if(e-s==1) continue;

     strncpy(varname,(char*)s,e-s);
     varname[e-s]=0;
     char *rus_strupr(char *);
     rus_strupr(varname);

     FieldLink def;
     char *var=varname+1;
     if(*var=='J' && isdigit(var[1])) { def.setN=var[1]-'0'; var+=3; }
     strcpy((char*)def.name,var);

     FieldLink *lnk=0;
     if(!def.setN) lnk=const_cast<FieldSet &>(recb).Link(var);
     if(lnk && lnk->int_type!=NULL_TYPE) def=*lnk;
     else
     {
      lnk=Link(var);
      if(lnk) def.dbsize=lnk->dbsize;
      else def.dbsize=251;
      const_cast<FieldSet &>(recb).MakeLink(def,1);
     }

     if(def.int_type!=LONG_TYPE && def.int_type!=LONGRAW_TYPE)
     {
      OraField* OF = (recb).Fld(def);
      if(const_cast<FieldSet &>(recb).Fld(def)==0)
       throw(OraError(-1,::string("Input arg ")+varname+" undefined.\n"+stmt));
      args.Add(def);
      OF->DeclVar(*curs, (char*)def.name);
      OF->SetVar(*curs, (char*)def.name);
     }
     if(*e) s=e;
     else break;
   }
 }
 //curs->SetMark();
}

void OraDML::SetArgs(FieldSet const &recb)
{
 //curs->ResetBufs();
 //args.Store(recb,curs->databuf(),curs->indbuf());
 (*curs)()->ClearVariables();
 for (int i = 0; i<args.Num(); i++)
 {
    FieldLink& FL = args[i];
    OraField* OF = recb.Fld(FL);
    if (!OF)
        THROW_AMS_EXC("Не нашел поля");
    OF->SetVar(*curs, (char*) FL.name);
 }
}

void OraDML::GetArgs(FieldSet &recb)
{
 for (int i = 0; i<args.Num(); i++)
 {
    FieldLink& FL = args[i];
    OraField * OF = recb.Fld(FL);
    if (!OF)
        THROW_AMS_EXC("Не нашел поля");
    OF->GetVar(*curs, (char*) FL.name);
 }
}

OraView::OraView(cursor *cur,const char *vie,const char *head,int row):
          OraDML(cur),view(vie)
{
 group="";
 AdjView();
 if(!head) { if(row) header="ROWID as ROW_ID,"+view+".*";
             else header="*"; }
 else header=head;
 Describe();
}

OraView::OraView(cursor *cur,const char *vie,const char *head,const char *grou):
          OraDML(cur),view(vie)
{
 if (!head) header="*" ;
 else header = head;

 if(grou && *grou) group=::string(" GROUP BY ")+grou;
 else group="";

 AdjView();
 Describe();
}

void OraView::Define()
{

 //int i,tsize=flnks.BufSize(),fld_num=flnks.Num();

 //curs->GotoMark();

 //fetched=0;
 //readed=0;

 //if(!fld_num || tsize<=0)
 //    throw(OraError(-1,::string("No defined fields\n")+stmt));

 //max_fetch=min(curs->IndCap()/fld_num,curs->DataCap()/tsize);

 //if(max_fetch<=0)
 //    throw(OraError(-1,::string("Buffer too small\n")+stmt));

 for(int i=0;i<flnks.Num();i++)
 {
   //FieldLink& FL = flnks[i];
   //FL.fldNum=-1;
   //if(type==LONG_TYPE && type==LONGRAW_TYPE) continue;
   //if(type==NULL_TYPE) type=STRING_TYPE;
   //curs->define_by_position(i+1,curs->databuf(),size,flnks[i].int_type,0,
   //       curs->indbuf());

   //curs->define_arr(i+1,curs->databuf(),size,type,curs->indbuf(),
   //                 tsize,fld_num);
   //curs->display_error(stmt.data());
   //curs->Step(size);
 }
}

int OraView::fetch(FieldSet &recb)
{
 /*
 if(fetched==readed)
 {
  if(fetched && fetched!=max_fetch) return 0;
  readed=0;
  fetched=0;
  int t=curs->RecCount();

  if(curs->multi_fetch(max_fetch) && curs->Error() != NULL_VALUE_RETURNED)
  {
   SetNull(recb);
   if (curs->Error() == NO_DATA_FOUND) { if(curs->RecCount()==t) return 0; }
   else  { curs->display_error(stmt.data()); return 0; }
  }
  fetched=curs->RecCount()-t;
 }
 */
 return _fetch(recb,readed++);
}

int OraView::ExFetchOne(FieldSet &recb,int exact)
{
 /*
 if(curs->execute_fetch(1,exact))
 {
    SetNull(recb);
    if (curs->Error() == NO_DATA_FOUND) return 0;
    if (curs->Error() != NULL_VALUE_RETURNED)
               { curs->display_error(stmt.data()); return 0; }
 }
 fetched=1;
 readed=0;
 */
 SetNull(recb);
 curs->Exec("");
 if ( (*curs)()->Eof) return 0;
 else
    return _fetch(recb,readed++);
}

int OraView::_fetch(FieldSet &recb,int skip_rec)
{
 //curs->RecSkip(skip_rec,flnks.BufSize(),flnks.Num());
 int Res = 0;
 if (!(*curs)()->Eof)
 {
    flnks.Get(recb,*curs);
    (*curs)()->Next();
    Res = 1;
 }
 return Res;
}

::string OraTable::InsertSTMT()
{
  ::string sqls="INSERT INTO ";
  sqls+=view+"(";
  int i,k;
  for(k=i=0;i<flnks.Num();i++)
  {
   if(strcmp((char*)flnks[i].name,"ROW_ID"))
   {
    if(k) sqls+=",";
    sqls+=(char *)flnks[i].name;
    k++;
   }
  }
  sqls+=") VALUES (";
  for(k=i=0;i<flnks.Num();i++)
  {
   if(strcmp((char*)flnks[i].name,"ROW_ID"))
   {
    if(k) sqls+=",";
    sqls+=":";
    sqls+=(char *)flnks[i].name;
    k++;
   }
  }
  sqls+=")";
  return sqls;
}

int OraTable::InsertRec(FieldSet &recb)
{
  Parse(InsertSTMT());
  Bind(recb);
  SetArgs(recb);
  return execute();
}

::string OraTable::UPDSTMT()
{
  return ::string("UPDATE ")+view+" SET ";
}

::string OraTable::DELSTMT()
{
  return ::string("DELETE FROM ")+view;
}

::string OraTable::ByROWID()
{
  return ::string(" WHERE ROWID=:ROW_ID");
}

int OraTable::UpdateRec_ID(FieldSet &recb,char *tname)
{
  ::string sqls ;
  if (tname)
    sqls=::string("UPDATE ")+tname+" SET " ;
  else
    sqls=UPDSTMT();

  int i,k;
  for(k=i=0;i<flnks.Num();i++)
  {
   if(strcmp((char*)flnks[i].name,"ROW_ID"))
   {
    if(k) sqls+=", ";
    sqls+=(char *)flnks[i].name;
    sqls+="=:";
    sqls+=(char *)flnks[i].name;
    k++;
   }
  }

  sqls+=ByROWID();
  Parse(sqls);
  Bind(recb);
  SetArgs(recb);
  return execute();
}

int OraTable::DeleteRec_ID(FieldSet &recb)
{
  ::string sqls=DELSTMT();
// here lives bug sqls=DELSTMT()+ByROWID()
// causes mem error if exception is thrown
  sqls+=ByROWID();
  Parse(sqls);
  Bind(recb);
  SetArgs(recb);
  return execute();
}

int OraDDL::Do(const ::string _stmt)
{
 Parse(_stmt);
 return execute();
}

int OraDDL::DoDDL(char *fmt,...)
{
  char sql[1024];
  va_list argptr;

  va_start(argptr, fmt);
  vsprintf(sql, fmt, argptr);
  va_end(argptr);
  return Do(sql);
}

int OraDML::Do(const ::string _stmt,FieldSet &inoutrec)
{
  Parse(::string(_stmt));
  Bind(inoutrec);
  SetArgs(inoutrec);
  int er=execute();
  GetArgs(inoutrec);
  return er;
}

int OraDML::DoPL(const ::string _stmt,FieldSet &inoutrec)
{
 return  Do(::string("begin ")+_stmt+" end;",inoutrec);
}

int OraView::Do(const ::string _stmt,FieldSet const &inrec)
{
  Parse(SELSTMT()+_stmt+group);
  Bind(inrec);
  SetArgs(inrec);
  Define();
  return execute();
}

int OraView::Do(const ::string cond,const ::string order,FieldSet const &inrec)
{
  Parse(SELSTMT()+cond+group+order);
  Bind(inrec);
  SetArgs(inrec);
  Define();
  return execute();
}

int OraView::Select(const ::string cond,const ::string order,FieldSet const &inrec)
{
  Parse(SELSTMT()+(cond>""?" WHERE ":"")+cond+group+order);
  Bind(inrec);
  SetArgs(inrec);
  Define();
  return execute();
}

int OraView::AltSelect(const ::string stmt,FieldSet const &inrec)
{
  Parse(stmt);
  Bind(inrec);
  SetArgs(inrec);
  Define();
  return execute();
}

int OraView::Select(const ::string cond,FieldSet const &inrec,FieldSet &outrec)
{
  Parse(SELSTMT()+(cond>""?" WHERE ":"")+cond+group);
  Bind(inrec);
  SetArgs(inrec);
  Define();
  return ExFetchOne(outrec);
}

int OraView::AltSelect(const ::string stmt,FieldSet const &inrec,FieldSet &outrec)
{
  Parse(stmt);
  Bind(inrec);
  SetArgs(inrec);
  Define();
  return ExFetchOne(outrec);
}

int OraView::RepeatDo(FieldSet const &inrec)
{
  SetArgs(inrec);
  fetched=readed=0;
  return execute();
}

int OraView::Do(const ::string _stmt,FieldSet const &inrec,FieldSet &outrec)
{
  Parse(SELSTMT()+_stmt+group);
  SetInOut(inrec,outrec);
  Bind(inrec);
  SetArgs(inrec);
  Define();
  return ExFetchOne(outrec);
}

int OraView::RepeatDo(FieldSet const &inrec,FieldSet &outrec)
{
  SetArgs(inrec);
  return ExFetchOne(outrec);
}

::string FieldLinks::CompExpr(int d)
{
 int i;
 ::string res;
 if(!d)
  for(i=0;i<Num();i++)
  {
   if(i) res+=" AND ";
   if(flds[i].name_query=="")
    res+=(char *)flds[i].name;
   else res+=flds[i].name_query;
   res+="=:";
   res+=(char *)flds[i].name;
  }
 else
 {
#if 0      // a<a || (a=a && (b<b || b=b && (c<c)))
  char *op=d<0?"<:":">:";
  for(i=0;i<Num();i++)
  {
//   if(i) res+=" AND ";
   if(flds[i].name_query.is_null())
    res+=(char *)flds[i].name;
   else res+=flds[i].name_query;
//   res+=(char *)flds[i].name;
   res+=op;
   res+=(char *)flds[i].name;
   if(i<Num()-1)
   {
    res+=" OR (";
    res+=(char *)flds[i].name;
    res+="=:";
    res+=(char *)flds[i].name;
    res+=" AND (";
   }
  }
  for(i=0;i<(Num()-1)*2;i++) res+=")";
#else // a<=a && (a<a || (b<=b && (b<b || c<c)))
  char *op=d<0?"<:":">:";
  char *op1=d<0?"<=:":">=:";
  for(i=0;i<Num();i++)
  {
   if(i==Num()-1)
   {
    if(flds[i].name_query=="")
     res+=(char *)flds[i].name;
    else res+=flds[i].name_query;
//    res+=(char *)flds[i].name;
    res+=op;
    res+=(char *)flds[i].name;
    continue;
   }
   if(i) res+="(";
   if(flds[i].name_query=="")
     res+=(char *)flds[i].name;
   else res+=flds[i].name_query;
//   res+=(char *)flds[i].name;
   res+=op1;
   res+=(char *)flds[i].name;
   res+=" AND (";
   if(flds[i].name_query=="")
     res+=(char *)flds[i].name;
   else res+=flds[i].name_query;
//   res+=(char *)flds[i].name;
   res+=op;
   res+=(char *)flds[i].name;
   res+=" OR ";
  }
  for(i=0;i<(Num()-1)*2-1;i++) res+=")";
#endif
 }
 return res;
}

::string FieldLinks::OrdExpr(int d,const char *aux)
{
 int i;
 ::string res=" ORDER BY ";
 if(aux)
 {
  res+=aux;
  if(d<0) res+=" DESC";
  res+=", ";
 }
 for(i=0;i<Num();i++)
 {
   if(i) res+=", ";
   if(flds[i].name_query=="")
    res+=(char *)flds[i].name;
   else res+=flds[i].name_query;
//   res+=(char *)flds[i].name;
   if(d<0) res+=" DESC";
 }
 return res;
}
#if 0
int OraLongTable::InsertRec(FieldSet &recb,char *filename)
{
  Parse(InsertSTMT());
  Bind(recb);
  SetArgs(recb);

  int i;
  for(i=0;i<flnks.Num();i++)
   if(flnks[i].int_type==LONG_TYPE || flnks[i].int_type==LONGRAW_TYPE) break;
  if(i==flnks.Num()) return -1;

  ::string fldname=':';
  fldname+=(char *)flnks[i].name;

  sb2 ind=0;
  char *context="ZZZ";
  ub1 piece;
  ub4 iteration;
  ub4 plsqltable;
  ub1 cont = (ub1)1;
  char buf[4096];
  ub4 readed;
  FILE *file;

  curs->bind_long((char *)fldname.data(), 4096, flnks[i].int_type,
                           &ind , context);

  file = fopen(filename,"rb");
  if (file==NULL) return -2;

  while (cont)
  {
   curs->execute();
   switch (curs->Error())
   {
     case 0: /* operation is finished */
       cont = 0;
       break;
     case OCI_MORE_INSERT_PIECES: /* ORA–03129 was returned */
       readed = fread(buf, 4096,1,file);
       ogetpi(curs->CDA(), &piece, (dvoid **)&context, &iteration,
               &plsqltable);
       if (readed < 4096) /* last piece? */
         piece = OCI_LAST_PIECE;
       osetpi(curs->CDA(), piece, buf, &readed);
       break;
   default:
       curs->display_error(stmt.data());
       break;
   }
  }
  fclose(file);
  if(ind==-1) return 0;
  return 1;
}

int OraLongTable::FetchRec(::string st,FieldSet &recb,char *filename)
{
  int i;
  for(i=0;i<flnks.Num();i++)
   if(flnks[i].int_type==LONG_TYPE || flnks[i].int_type==LONGRAW_TYPE) break;
  if(i==flnks.Num()) return -1;

  ::string sqls;
  sqls="SELECT ";
  sqls+=(char *)flnks[i].name;
  sqls+=" FROM "+view;
  sqls+=st;

  Parse(sqls);
  Bind(recb);
  SetArgs(recb);


  sb2 ind;
  char *context="ZZZ";
  ub1 piece;
  ub4 iteration;
  ub4 plsqltable;
  ub1 cont = (ub1)1;
  char buf[4096];
  ub4  len_buf=4096;
  FILE *file;

    if (odefinps(curs->CDA(), 0, 1, (ub1 *)&context, (ub4) 1000000l,
                 flnks[i].int_type, 0, (sb2 *)ind, (text *)0, 0, 0,
                 0, 0, 0, 0, 0, 0))
    curs->display_error();

  file = fopen(filename,"wb");
  if (file==NULL) return -2;

  curs->execute();
  while (cont)
  {
   curs->fetch();
   switch (curs->Error())
   {
     case 0: /* operation is finished */
       cont = 0;
       break;
     case OCI_MORE_FETCH_PIECES: /* ORA–03130 was returned */
       ogetpi(curs->CDA(), &piece, (dvoid **)&context, &iteration,
               &plsqltable);
       if (piece!=OCI_FIRST_PIECE) /* can’t write on first fetch */
        fwrite(buf, len_buf,1,file);
       osetpi(curs->CDA(), piece, buf, &len_buf);
       break;
     default:
       curs->display_error(stmt.data());
       break;
   }
  }
  fclose(file);
  if(ind==-1) return 0;
  return 1;
}
#endif
OraError::OraError(sword code,const ::string &str):err_code(code),xmsg(str.data()) {}
OraError::OraError(sword code,const char *str):err_code(code),xmsg(str) {}
//OraError::~OraError() {}

//-----------------------------------------------------------------------------
//---------------------------------------------------------------------------
OraField* BornFld(TOracleDataSet* ODS, SFldOpt& FO )
{
    OraField* OraFld = 0;
    if (FO.FldNo!=-1)
    {
        TField* Fld = 0;
        if (ODS) Fld=ODS->Fields->Fields[FO.FldNo];
        switch (FO.Type)
        {
            case    aftDate:
            {
                Field<DateTime>* FD= new Field<DateTime>();
                OraFld = FD;
                if (Fld)
                {
                    DateTime TTT;
                    TTT=(Fld->AsDateTime);
                    *FD= TTT;
                }
            }
            break;
            case aftSumma:
            {
                Field<Sum>* FD= new Field<Sum>();
                OraFld = FD;
                if (Fld) *FD=(Fld->AsFloat);
            }
            break;
            case aftInt:
            {
                Field<int>* FD= new Field<int>();
                OraFld = FD;
                if (Fld) *FD=(Fld->AsInteger);
            }
            break;
            case aftStr:
            {
                Field<::string>* FD= new Field<::string>();
                if (Fld) *FD= (ToString(Fld->AsString));
                OraFld = FD;
            }
            break;
            case aftBool:
            {
                Field<int>* FD= new Field<int>();
                if (Fld) *FD=(Fld->AsInteger);
                OraFld = FD;
            }
            break;
        }
    }
    return OraFld;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
FieldSetM::FieldSetM():Builded(false), ODS(NULL), FldsDescr(NULL){};
//---------------------------------------------------------------------------
void FieldSetM::BuildFSM()
{
        for (int i = 0 ; i< FldsDescr->Count(); i++)
        {
            SFldOpt& FO=(*FldsDescr)[i];
            OraField *fld=BornFld(ODS, FO);
            Add(fld);
        }
        Builded= true;
};
//---------------------------------------------------------------------------
void FieldSetM::LoadData(Node& st, bool SetFlds, bool ClearFlds, bool ClearF4Only)
{
    int clone = 0;
    if (!Builded)
    {
        BuildFSM();
    }
    if(!st.Stru()) st.MakeStruct();
    Struct &str=st.Structure();
    for (unsigned int i = 0 ; i< Flds.size(); i++)
    {
        OraField *fld=Flds[i];
        SFldOpt& FO=(*FldsDescr)[i];
        if (fld)
        {
            // Устанавливаем значение из дата сета
            if (SetFlds)
            {
                if (ClearF4Only && (FO.DefOpt & EditRecalc))
                {
                    fld->SetNull();
                }
                else
                if (FO.FldNo!=-1)
                    fld->SetField(ODS, FO.FldNo);
                else
                    fld->SetNull();
            }
            else
            if (ClearFlds)
            {
                    fld->SetNull();
            }
            if(clone)
                str.UpdateVar_Clone(FO.RealName.c_str(),fld);
            else
                str.UpdateVar_Fld(FO.RealName.c_str(),fld);

            if(FO.Alias!="")
                if(clone)
                    str.UpdateVar_Clone(FO.Alias.c_str(),fld);
                else
                    str.UpdateVar_Fld(FO.Alias.c_str(),fld);
        }
    };
};
//-----------------------------------------------------------------------------
void FieldSetM::SetData()
{
    for (unsigned int i = 0 ; i< Flds.size(); i++)
    {
        OraField *fld=Flds[i];
        if(fld) fld->SetDS(ODS, (*FldsDescr)[i].FldNo);
    }
};
//-----------------------------------------------------------------------------
void FieldSetM::Add(OraField* OF)
{
    Flds.push_back(OF);
};
//-----------------------------------------------------------------------------
void FieldSetM::SetInputVal(TAmsInput* AI, int FldNo)
{
    if (FldNo != -1 && FldNo<int (Flds.size()))
    {
        OraField* OF = Flds[FldNo];
        if (OF) OF->SetVal(AI);
    }
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Строим все
void FieldSetAnTotal::Build()
{
    bool FirstTime = true;
    Sql = "select ";
    Indx.clear();
    for (int i = 0 ; i< FldsDescr->Count(); i++)
    {
        SFldOpt& FO=(*FldsDescr)[i];
        if (FO.FldAddInfo==aiAn && FO.DefOpt & Totalable)
        {
            if (FirstTime )
                FirstTime = false;
            else
                Sql+=", ";
            OraField *fld=BornFld(ODS, FO);
            Add(fld);
            Indx.push_back(i);
            if ( FO.DefOpt & Addable)
            {
                Sql+= " sum("+FO.FullName+") "+FO.SqlAlias;
            }
            else
            {
                Sql+= " max("+FO.FullName+") "+FO.SqlAlias;
            }
        }
    }
    Builded= true;
    SqlFormed = !FirstTime;
};
//-----------------------------------------------------------------------------
//  Для сложения аналитических сумм
//-----------------------------------------------------------------------------
// Установить во всех полях нулевые значения
void FieldSetAnTotal::SetZero()
{
    for (unsigned int i =0; i< Flds.size(); i++)
    {
        Flds[i]->_SetNull();
    }
};
//-----------------------------------------------------------------------------
// Копировать итого строку в FldSet
void FieldSetAnTotal::CopyAnalSum (AmsQuery& AQ_)
{
    for (unsigned int i =0; i< Flds.size(); i++)
    {
        Flds[i]->SetFld(AQ_, i);
    }
};
//-----------------------------------------------------------------------------
// Добавить текущую запись датасета к фиелд сету
void FieldSetAnTotal::AddLine (TOracleDataSet* ODS_, bool OldVal)
{
    for (unsigned int i =0; i< Flds.size(); i++)
    {
        SFldOpt& FO = (*FldsDescr)[Indx[i]];
        Flds[i]->AddVal(ODS_->Fields->Fields[FO.FldNo], OldVal);
    }
};
//-----------------------------------------------------------------------------
// Отнять текущую запись датасета от фиелд сета
void FieldSetAnTotal::SubLine (TOracleDataSet* ODS_, bool OldVal)
{
    for (unsigned int i =0; i< Flds.size(); i++)
    {
        SFldOpt& FO = (*FldsDescr)[Indx[i]];
        Flds[i]->SubVal(ODS_->Fields->Fields[FO.FldNo], OldVal);
    }
};
//-----------------------------------------------------------------------------
void FieldSetAnTotal::MakeTotalUpdate (int AnSum)
{
    AnsiString S = "update "+BaseUserName+"."+FldsDescr->GetAnName()+" set ";
    AmsQuery AQ;
    for (unsigned int i =0; i< Flds.size(); i++)
    {
        SFldOpt& FO = (*FldsDescr)[Indx[i]];
        if (i)
            S+= ", ";
        S+= FO.RealName+"=:"+FO.RealName;
        Flds[i]->DeclVar(AQ, FO.RealName);
        Flds[i]->SetVar(AQ, FO.RealName);
    }
    AQ.SetSqlText(S+" where SUMID=:SUMID and AARTICLE=' ' and AVER=0");
    AQ.DeclVar("SUMID", AnSum);
    AQ.Exec("Устанавливаем итого строку");
};
//-----------------------------------------------------------------------------
Sum FieldSetAnTotal::GetResult(AnsiString FldName)
{
    for (unsigned int i =0; i< Flds.size(); i++)
    {
        SFldOpt& FO = (*FldsDescr)[Indx[i]];
        if (FO.RealName==FldName)
        {
            return Flds[i]->Number();
        }
    }
    return 0;
};
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Строим все
void FieldSetVerDistr::Build()
{
    bool Finded = false;
    int i = 0 ;
    while (!Finded && i< FldsDescr->Count())
    {
        SFldOpt& FO = (*FldsDescr)[i];
        if ((FO.FldAddInfo==aiAn) && (FO.DefOpt & Addable) && (FO.DefOpt & ::DistrFld))
        {
            Finded = true;
        }
        else
            i++;
    }
    DistrFld = -1;
    if (Finded)
    {
        DistrFld = i;
        SFldOpt& FO = (*FldsDescr)[i];

        Sql="select "+FO.FullName+", AN.AVER from "+BaseUserName+"."+
            FldsDescr->GetAnName()+" AN, "+BaseUserName+"."+
            FldsDescr->GetDbName()+" D where AN.SUMID=:SUMID and "
            "AN.AARTICLE=:AARTICLE and AN.AARTICLE=D.ARTICLE and AN.AVER=D.VER ";
//            " and "+FO.FullName+">0";
    }
    else
    {
        ErrorMessage("Не нашел поля с установкой Распределять");
    }
    SqlIns = "insert into "+BaseUserName+"."+FldsDescr->GetAnName()+
        "(SUMID, AARTICLE, AVER";
    AnsiString FldsList="(:SUMID, :AARTICLE, :AVER";

    // Узнаем какие поля копировать, какие разделять
    for (int i=0; i<FldsDescr->Count();i++)
    {
        SFldOpt& F1= (*FldsDescr)[i];
        if (i==DistrFld)
            DistrFldIndx = SumFlds.size();
        if ( F1.FldAddInfo==aiAn &&
             F1.RealName!="SUMID" &&
             F1.RealName!="AARTICLE" &&
             F1.RealName!="AVER" )
        {
            if (F1.DefOpt & Addable)
                SumFlds.push_back(i);
            else
                CopyFlds.push_back(i);
            FldsList+=", :"+F1.RealName;
            SqlIns+=", "+F1.RealName;
        }
    }
    for (unsigned int i =0; i< SumFlds.size();i++)
    {
        SFldOpt& F1= (*FldsDescr)[SumFlds[i]];
        OraField *fld=BornFld(ODS, F1);
        Add(fld);
        Indx.push_back(SumFlds[i]);
    }

    for (unsigned int i =0; i< CopyFlds.size();i++)
    {
        SFldOpt& F1= (*FldsDescr)[CopyFlds[i]];
        OraField *fld=BornFld(ODS, F1);
        Add(fld);
        Indx.push_back(CopyFlds[i]);
    }
    SqlIns+=") values "+FldsList+")";
    Builded= true;
}
//-----------------------------------------------------------------------------
// Формирует запрос на получение остатка по полю распределять
// Возвращает 1 если существует поле распределять
int FieldSetVerDistr::PrepareOstQuery (AmsQuery& AQ)
{
    int Res = 0;
    if (DistrFld!= -1)
    {
        AQ.Clear();
        AQ.SetSqlText(Sql+" order by " + OstOrder);
        AQ.DeclVar("SUMID", AnSumOst);
        Res=1;
    }
    return Res;
};
//-----------------------------------------------------------------------------
// Формирует запрос на вставку (для ускорения)
void FieldSetVerDistr::PrepareInsertQuery (AmsQuery& AQ)
{
    AQ.Clear();
    AQ.SetSqlText(SqlIns);
    AQ.DeclVar("SUMID", AnSumEd);
    AQ.DeclVarStr("AARTICLE");
    AQ.DeclVarInt("AVER");

    for (unsigned int i = 0 ; i< SumFlds.size();i++)
    {
        SFldOpt& F1 = (*FldsDescr)[SumFlds[i]];
        switch(F1.Type)
        {
            case aftDate:
                AQ.DeclVarDate(F1.RealName);
            break;
            case aftSumma:
                AQ.DeclVarSum(F1.RealName);
            break;
            case aftInt:
            case aftBool:
                AQ.DeclVarInt(F1.RealName);
            break;
            case aftStr:
                AQ.DeclVarStr(F1.RealName);
            break;
        };
    }
    for (unsigned int i = 0 ; i< CopyFlds.size();i++)
    {
        SFldOpt& F1 = (*FldsDescr)[CopyFlds[i]];
        switch(F1.Type)
        {
            case aftDate:
                AQ.DeclVarDate(F1.RealName);
            break;
            case aftSumma:
                AQ.DeclVarSum(F1.RealName);
            break;
            case aftInt:
            case aftBool:
                AQ.DeclVarInt(F1.RealName);
            break;
            case aftStr:
                AQ.DeclVarStr(F1.RealName);
            break;
        };
    }
};
//-----------------------------------------------------------------------------
// Отнимает остаток и генерирует insert запрос
// Возвращает false если весь остаток распределен
int FieldSetVerDistr::HaveOst(AmsQuery& AQOst, AmsQuery& AQIns)
{
    double OstK=AQOst.GetFldSum(0);
    double RaspK=Flds[DistrFldIndx]->Number();
    int Res = 1;
    AQIns.SetVar("AARTICLE", AQOst.GetVarStr("AARTICLE"));
    AQIns.SetVar("AVER", AQOst.GetFldInt(1));
    if (OstK<=0) return Res;      //EO CH
    if (OstK<RaspK)
    {
        double Tran =0;
        double Rest =0;
        AQIns.SetVar("AARTICLE", AQOst.GetVarStr("AARTICLE"));
        AQIns.SetVar("AVER", AQOst.GetFldInt(1));
        for (unsigned int i=0; i<SumFlds.size();i++)
        {
            SFldOpt& FO = (*FldsDescr)[SumFlds[i]];
            if ((int)i == DistrFldIndx)
            {
                Tran = OstK;
                Rest = RaspK-OstK;
            }
            else
            {
                Tran = (Flds[i]->Number()) * OstK/ RaspK;
                Rest = Flds[i]->Number()- Tran;
            }
            Flds[i]->SetNumber(Rest);
            AQIns.SetVar(FO.RealName, Tran);
        }
        unsigned int Offs = SumFlds.size();
        for (unsigned int i=0; i<CopyFlds.size();i++)
        {
            SFldOpt& FO = (*FldsDescr)[CopyFlds[i]];
            Flds[i+Offs]->SetVar(AQIns, FO.RealName);
        }
        AQIns.Exec("Вставка распределения");
    }
    else
    {
        PutOst(AQIns, false);
        Res=0;
    }
    return Res;
};
//-----------------------------------------------------------------------------
// Вставляет то что осталось как версию, прописанную
// Вызывается когда уже не осталось остатка
void FieldSetVerDistr::PutOst(AmsQuery& AQIns, bool NoOst)
{
    if (NoOst)
        AQIns.SetVar("AVER", ODS->FieldByName("VER")->AsInteger);
    for (unsigned int i=0; i<SumFlds.size();i++)
    {
        SFldOpt& FO = (*FldsDescr)[SumFlds[i]];
        Flds[i]->SetVar(AQIns, FO.RealName);
    }
    unsigned int Offs = SumFlds.size();
    for (unsigned int i=0; i<CopyFlds.size();i++)
    {
        SFldOpt& FO = (*FldsDescr)[CopyFlds[i]];
        Flds[i+Offs]->SetVar(AQIns, FO.RealName);
    }
    AQIns.Exec("Вставка распределения");
};
//-----------------------------------------------------------------------------
int FieldSetVerDistr::GetVersQ(const AnsiString& Art)
{
    AmsQuery AQ;
    AnsiString S="select count(*) from "+BaseUserName+"."+
        FldsDescr->GetAnName()+" AN where AN.SUMID=:SUMID and "
        "AN.AARTICLE=:AARTICLE ";
    AQ.DeclVar("AARTICLE", Art);
    AQ.DeclVar("SUMID", AnSumOst);
    AQ.SetSqlTextReady(S);
    AQ.Exec("Колическво версий в остатке");
    int Res = 0;
    if (AQ.NotEof())
        Res = AQ.GetFldInt(0);
    AQ.DeleteVars();
    return Res;
};

int FieldSetVerDistr::GetVersQPlus(const AnsiString& Art)
{
    AmsQuery AQ;
        bool Finded = false;
    int i = 0 ;
    while (!Finded && i< FldsDescr->Count())
    {
        SFldOpt& FO = (*FldsDescr)[i];
        if ((FO.FldAddInfo==aiAn) && (FO.DefOpt & Addable) && (FO.DefOpt & ::DistrFld))
        {
            Finded = true;
        }
        else
            i++;
    }
    AnsiString S="select count(*) from "+BaseUserName+"."+
        FldsDescr->GetAnName()+" AN where AN.SUMID=:SUMID and "
        "AN.AARTICLE=:AARTICLE ";
    if (Finded)
    {
      SFldOpt& FO = (*FldsDescr)[i];
      S+=" and "+FO.FullName+">0";
    }
    AQ.SetSqlTextReady(S);
    AQ.DeclVar("AARTICLE", Art);
    AQ.DeclVar("SUMID", AnSumOst);
    AQ.Exec("Колическво версий с положительным остатком в остатке");
    int Res = 0;
    if (AQ.NotEof())
        Res = AQ.GetFldInt(0);
    AQ.DeleteVars();
    return Res;
};

//-----------------------------------------------------------------------------



