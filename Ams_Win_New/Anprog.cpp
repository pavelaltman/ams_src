#include "AMS_HDR.h"
#pragma hdrstop
#include "accs.h"
#include "analytic.h"
#include "progan.h"
extern r_char
         varnameexpectederr,unknownfuncerr,badtypeerr,
         uknownobjecterr,badobjerr,nosuchstructerr;

extern int def_point, def_numdig, def_zeros, def_apps ;

void   DBStructInfo::MakeStruct(Node &st,FieldSet &rec,int db,int clone)
{
 if(!st.Stru()) st.MakeStruct();
 Struct &str=st.Structure();
 for(int i=0;i<NFields();i++)
 {
  if(!(FI(i).db & db)) continue;
  //FInfo& SS= FI(i);
  char const *name=FI(i).OrigName.data();
  char aname[50];
  strcpy(aname,FI(i).Alias.data());
  rus_strupr(aname);

  OraField *fld=rec.FldN(FI(i).RealName.data());

  if(clone)
    str.UpdateVar_Clone(name,fld);
  else
    str.UpdateVar_Fld(name,fld);

  if(aname[0])
   if(clone)
    str.UpdateVar_Clone(aname,fld);
   else
    str.UpdateVar_Fld(aname,fld);
 }
 if(show_rem && db==3)
 {
   Node *rem=str.FindVar("AR");
   if(!rem) rem=str.AddVar(new Node("AR"));
   MakeStruct(*rem,rec,4,clone);
 }
}

extern int cur_arg ;

void  DBStructInfo::CalcDBLine(FieldSet &rec)
{
  IP ip;
  ip.Enter();

  int pre_cur_arg=cur_arg ;
  cur_arg=-1 ;

  try
  {
   MakeStruct(ip.Base,rec);
   ip.Base.Structure().Var("SET").assign(VAL(set));
   db_prog.Run(ip);
  }
  catch(OraError ERR)
  {
    ERR.message(0,0,"After prog error");
  }
  ip.Leave();
  PosBuf &drec=(PosBuf &)rec;
  if(drec.prVer.isNull()) drec.prVer=0;

  cur_arg=pre_cur_arg ;
}

void  DBStructInfo::CalcAnLine(FieldSet &rec,FieldSet &totrec,int pre)
{
  IP ip;
  ip.Enter();

  int pre_cur_arg=cur_arg ;
  cur_arg=-1 ;

  try
  {
   MakeStruct(ip.Base,rec);
   ip.Base.Structure().Var("SET").assign(VAL(set));
   ip.Base.Structure().Var("PRE").assign(VAL(pre));
   if(aux_view && aux_rec) ip.Base.MakeOraRec(*aux_view,*aux_rec,1);
   an_prog.Run(ip);
  }
  catch(OraError ERR)
  {
    ERR.message(0,0,"After prog error");
  }
  ip.Leave();
  if(bal_fld && synt_sum && !synt_sum->isNull())
  {
   OraField *fld=rec.FldN(bal_fld);
   OraField *tfld=totrec.FldN(bal_fld);
   if(fld && tfld && fld->vtype()==gcl::VT_NUM)
    if(fld->isNull())
      *((Field<Sum> *)fld)=synt_sum->val-((Field<Sum> *)tfld)->val;
  }

  cur_arg=pre_cur_arg ;
}

CatDB_PC::CatDB_PC(const ::string &dbname,const ::string &set)
{
  cv=0;
  //crsr.open(&conn);
  if(!GetObj(dbname,obj,&crsr)) prog_ptr->RTError(101,::string(uknownobjecterr)+dbname);
  DBStr.Init(&crsr,obj,set,0);
  cv=new OraTable(&crsr,"*.DBCATTREE");
  cv->MakeRecord(crec);
//  cv->MakeRecord(excrec);
  crec.dbid=DBStr.dbid;
}
#pragma argsused
StackItem CatDB_PC::Method(Node &node,::string const &method,int pc,StackItem *stk)
{
  if(method=="DO" && pc==1)
  {
   stk[0].getNode().MakeOraRec(*cv,crec);
   stk[0].getNode().Structure().UpdateVar_Fld("CATNAME",&crec.descr);
   cv->Select("DBID=:DBID"," ORDER BY TORD",crec);
   return StackItem();
  }
  if(method=="FETCH" && pc==0)
  {
   int ok=cv->fetch(crec);
   return VAL(ok);
  }
  prog_ptr->RTError(101,::string(unknownfuncerr)+"  "+method);
  return StackItem();
}

//------------------------------------------------------------------------------
std::vector<DbImpExp*> DbList;
DbImpExp::DbImpExp(const AnsiString& ODBC_, const char* UName, const char* Pass)
{
    DB=new TDatabase(Application);
    DB->Connected=false;
    DB->DatabaseName="DBEXPIMP"+IntToStr(DbList.size());
    ODBC=ODBC_;
    DB->AliasName=ODBC;
    CurrState=lcEmpty;
    DbList.push_back(this);
    if (UName && Pass)
    {
        DB->LoginPrompt=false;
        DB->Params->Add(AnsiString("USER NAME=")+UName);
        DB->Params->Add(AnsiString("Password=")+Pass);
    }
    DB->Connected=true;
    CurrState=lcCreated;
}
//------------------------------------------------------------------------------
DbImpExp::~DbImpExp()
{
    DB->Connected=false;
    unsigned int i=0;
    for (i=0;i<Queries.size();i++)
    {
        TQuery* Q = Queries[i];
        Q->Active=false;
        delete Q;
    }
    while (i<DbList.size())
    {
        if (DB== DbList[i])
        {
            DbList[i]=NULL;
            break;
        }
        else i++;
    }
    delete DB;
    CurrState=lcDeleted;
}
//---------------------------------------------------------------------------
AnsiString GetFldType (int FldType)
{
    AnsiString Res;
    switch (FldType)
    {
        case ftString:
        case ftFmtMemo:
        case ftFixedChar:
            Res="STR";
        break;


        case ftAutoInc:
        case ftSmallint:
        case ftInteger:
        case ftWord:
        case ftBoolean:
            Res="INT";
        break;


        case ftFloat:
        case ftCurrency:
        case ftLargeint:
            Res="SUM";
        break;


        case ftDate:
        case ftDateTime:
            Res="DATE";
        break;
    }
    return Res;
}
//------------------------------------------------------------------------------
OraField* BornOraField (TField* Fld)
{
    OraField* Res=NULL;
    AnsiString FT= GetFldType (Fld->DataType);
    if (FT=="STR")
    {
        Res= new Field<::string> ();
        Res->GetVal(Fld);
    }
    else if (FT=="SUM")
    {
        Res= new Field<Sum> ();
        Res->GetVal(Fld);
    }
    else if (FT=="INT")
    {
        Res= new Field<int> ();
        Res->GetVal(Fld);
    }
    else if (FT=="DATE")
    {
        Res= new Field<DateTime> ();
        Res->GetVal(Fld);
    }
    return Res;
}


#pragma argsused
StackItem DbImpExp::Method(Node &node,::string const &method,int pc,StackItem *stk)
{
    if (CurrState!=lcCreated)
    {
        prog_ptr->RTError(101,"Не произошло подключение к БД");
    }
    if( (method=="SQL" || method=="TABLE") && pc<=1)
    {
        AnsiString Sql = "";
        if (pc==1) Sql=stk[1-pc].String().data();
        int CurrIndx = Queries.size();
        TQuery* Q= new TQuery(Application);
        Q->DatabaseName=DB->DatabaseName;
        Q->SQL->Text=method=="TABLE"? AnsiString ("select * from ") +Sql:Sql;
        Queries.push_back(Q);
        if (Sql!="")
            Q->Active=true;
        return StackItem(CurrIndx);
    }
    if(method=="CLOSE" && pc==1)
    {
        int R = stk[1-pc].Number();
        TQuery* Q= Queries.at(R);
        Q->Active=false;
        return StackItem(1);
    }
    if(method=="EXEC" && (pc==2 || pc==3))
    {
        int R = stk[1-pc].Number();
        TQuery* Q= Queries.at(R);
        Q->Active=false;
        Q->SQL->Text=stk[2-pc].String().data();
        TParams* Pars= Q->Params;
        Pars->Clear();
        Pars->ParseSQL(Q->SQL->Text, true);

        if (pc==3 &&  stk[3-pc].getNode().st)
        {
            Struct * Str= stk[3-pc].getNode().st;
            for (int i =0 ; i< Pars->Count; i++)
            {
                TParam* P = Pars->Items[i];
                P->ParamType=ptInputOutput;
                P->DataType=ftString;
                Node* N = Str->FindVar(P->Name.c_str());
                if (N)
                {
                    switch (N->vtype())
                    {
                        case gcl::VT_DATE:
                            P->DataType=ftDateTime;
                            if (N->isNull())
                                P->Clear();
                            else
                            {
                                Field<DateTime>& F= (Field<DateTime>&)(N->Fld());
                                P->AsDateTime=F.Val().T();
                            }
                        break;
                        case gcl::VT_NUM:
                        case gcl::VT_INT:
                            P->DataType=ftFloat;
                            if (N->isNull())
                                P->Clear();
                            else
                            {
                                P->AsFloat=N->Number();
                            }
                        break;
                        case gcl::VT_ROWID:
                        break;
                        case gcl::VT_STRING:
                            P->DataType=ftString;
                            if (N->isNull())
                                P->Clear();
                            else
                            {
                                P->AsString=N->String().data();
                            }
                        break;
                    }
                }

            }
        }
        Q->ExecSQL();
        return StackItem(1);
    }
    if(method=="RESET" && (pc==2 || pc==3))
    {
        int R = stk[1-pc].Number();
        TQuery* Q= Queries.at(R);
        Q->Active=false;
        Q->SQL->Text=stk[2-pc].String().data();

        TParams* Pars= Q->Params;
        Pars->Clear();
        Pars->ParseSQL(Q->SQL->Text, true);

        if (pc==3 &&  stk[3-pc].getNode().st)
        {
            Struct * Str= stk[3-pc].getNode().st;
            for (int i =0 ; i< Pars->Count; i++)
            {
                TParam* P = Pars->Items[i];
                P->ParamType=ptInput;
                P->DataType=ftString;
                Node* N = Str->FindVar(P->Name.c_str());
                if (N)
                {
                    switch (N->vtype())
                    {
                        case gcl::VT_DATE:
                            P->DataType=ftDateTime;
                            if (N->isNull())
                                P->Clear();
                            else
                            {
                                Field<DateTime>& F= (Field<DateTime>&)(N->Fld());
                                P->AsDateTime=F.Val().T();
                            }
                        break;
                        case gcl::VT_NUM:
                        case gcl::VT_INT:
                            P->DataType=ftFloat;
                            if (N->isNull())
                                P->Clear();
                            else
                            {
                                P->AsFloat=N->Number();
                            }
                        break;
                        case gcl::VT_ROWID:
                        break;
                        case gcl::VT_STRING:
                            P->DataType=ftString;
                            if (N->isNull())
                                P->Clear();
                            else
                            {
                                P->AsString=N->String().data();
                            }
                        break;
                    }
                }
            }
        }
        Q->Active=true;
        return StackItem(1);
    }
    if(method=="FLDSCOUNT" && pc==1)
    {
        int R = stk[1-pc].Number();
        TQuery* Q= Queries.at(R);
        int T=Q->FieldCount;
        return StackItem(T);
    }
    if(method=="FLDNAME" && pc==2)
    {
        int R = stk[1-pc].Number();
        TQuery* Q= Queries.at(R);
        int FieldIndx = stk[2-pc].Number();
        TField* F =Q->Fields->Fields[FieldIndx];
        ::string FN= F->FieldName.data();
        return StackItem(FN);
    }
    if(method=="FLDTYPE" && pc==2)
    {
        int R = stk[1-pc].Number();
        TQuery* Q= Queries.at(R);
        int FieldIndx = stk[2-pc].Number();
        TField* F =Q->Fields->Fields[FieldIndx];
        ::string Res= (GetFldType (F->DataType)).c_str();
        return StackItem(Res);
    }
    if(method=="FLDSIZE" && pc==2)
    {
        int R = stk[1-pc].Number();
        TQuery* Q= Queries.at(R);
        int FieldIndx = stk[2-pc].Number();
        TField* F =Q->Fields->Fields[FieldIndx];
        int FN= F->DataSize;
        return StackItem(FN);
    }
    if(method=="NOTEOF" && pc==1)
    {
        int R = stk[1-pc].Number();
        TQuery* Q= Queries.at(R);
        int Res=!Q->Eof;
        return StackItem(Res);
    }
    if(method=="NEXT" && pc==1)
    {
        int R = stk[1-pc].Number();
        TQuery* Q= Queries.at(R);
        Q->Next();
        return StackItem();
    }
    if((method=="GET") && pc==2)
    {
        int R = stk[1-pc].Number();
        TQuery* Q= Queries.at(R);
        Node& N = stk[2-pc].getNode();

        if (!N.st) N.MakeStruct();
        Struct &str=N.Structure();
        for(int i=0;i<Q->FieldCount;i++)
        {
            TField *fld=Q->Fields->Fields[i];
            OraField* OF= BornOraField(fld);
            char FN [500];
            strcpy (FN,fld->FieldName.c_str());
            ::string vname= rus_strupr(FN) ;
            Node *nd=str.FindVar(vname);
            if(!nd)
            {
                str.AddVar(new Node(vname,*OF ));
            }
            else
            {
                nd->destroy();
                *nd=Node(vname,*OF);
            }
        }
        return VAL(1);
    }

    /*
    if(method=="DO" && pc==1)
    {
        stk[0].getNode().MakeOraRec(*cv,crec);
        stk[0].getNode().Structure().UpdateVar_Fld("CATNAME",&crec.descr);
        cv->Select("DBID=:DBID"," ORDER BY TORD",crec);
        return StackItem();
    }
    if(method=="FETCH" && pc==0)
    {
        int ok=cv->fetch(crec);
        return VAL(ok);
    }
    */
    prog_ptr->RTError(101,::string(unknownfuncerr)+"  "+method);
    return StackItem();
}
//------------------------------------------------------------------------------
PosDB_PC::PosDB_PC(const ::string &dbname,int _extart,const ::string &set,
                           int _SumId,int _outer)
          :crsr(conn,30000,3000),dv(0),addv(0),DBStr(0)
{
  orphan=1;
  SumId=_SumId;
  outer=_outer;
  extart=_extart;
  cat=0;
  if(GetObj(dbname,obj,&crsr))
  {
   DBStr=new DBStructInfo(&crsr,obj,set,0);
//   Setup();
  }
  else
  {
   ord=uknownobjecterr;
   ord+=dbname;
  }
}

PosDB_PC::PosDB_PC(Node &node,int _extart,int _SumId,int _outer)
          :crsr(conn,30000,3000),dv(0),addv(0),DBStr(0)
{
  orphan=0;
  SumId=_SumId;
  outer=_outer;
  extart=_extart;
  cat=dynamic_cast<CatDB_PC *>(node.Stru());
  if(!cat)
  {
   ord=badtypeerr;
   return;
  }
   obj=cat->obj;
   dbId=cat->dbId;
   DBStr=&cat->DBStr;
//   Setup();
}

void PosDB_PC::Setup()
{
  if(!DBStr)  prog_ptr->RTError(105,ord);
  if(SumId && DBStr->analid<0) prog_ptr->RTError(105,badobjerr);
  if(!SumId && DBStr->analid>0) prog_ptr->RTError(105,badobjerr);

  ::string dbn;

//  if(!SumId) dbn=::string("*.")+DBStr->dbname;
//  else
    dbn=DBStr->AnalView();

  if(!extart)
   dv=new OraView(&crsr,dbn.data(),DBStr->AnalHeader().data());
  else
   dv=new OraView(&crsr,dbn.data(),
                 (DBStr->DBExHeader(0)).data(),DBStr->DBExGroup(0).data());
  dv->MakeRecord(drec);

  if(SumId)
  {
    //adcrsr.open(&conn);
    dbn=::string("*.")+DBStr->analname;
    if(!extart)
    {
     addv=new OraView(&adcrsr,dbn.data());
     addv->MakeRecord(adrec);
     addv->Select(" SUMID=:SUMID AND AARTICLE=:AARTICLE AND AVER=:AVER","",adrec);
    }
    else
    {
     addv=new OraView(&adcrsr,dbn.data(),(DBStr->AnExHeader()).data(),"SUMID,AARTICLE");
     addv->MakeRecord(adrec);
     addv->Select(" SUMID=:SUMID AND AARTICLE=:AARTICLE","",adrec);
    }
  }

  if(!SumId)
  {
   if(orphan) query="ARTICLE!=' ' AND DB.CATID=C.OWNCAT";
   else       query="DB.CATID=:CATID AND DB.CATID=C.OWNCAT";
  }
  else
   query=DBStr->AnalCond(!orphan,outer,SumId,0);

  ord=" ORDER BY ";
  if (SumId /* && !extart */) ord+=" TORD," ;
  if(outer || !orphan || extart || !SumId)
  {
     if(!extart && !orphan) ord+="DB.CATID,";
     ord+="ARTICLE";
     if(!extart) ord+=",VER";
  }
  else
  {
     ord+="AN.AARTICLE";
     if(!extart) ord+=",AN.AVER";
    }
  arg=&drec;
}

StackItem PosDB_PC::Method(Node &node,::string const &method,int pc,StackItem *stk)
{
  if(method=="DO" && pc==1)
  {
   if(cat) drec.catId=cat->crec.owncat;
   DBStr->MakeStruct(stk[0].getNode(),drec,3);
   dv->Select(query,ord,*arg);
   return StackItem();
  }
  if(method=="FETCH" && pc==0)
  {
   int ok=dv->fetch(drec);
   return VAL(ok);
  }
  if(method=="TOTAL" && pc>=1)
  {
   if(!SumId) return VAL(-1);

   adrec.SumId=SumId;
   adrec.aarticle=TotArt;
   adrec.aprVer=TotVer;

   if(pc>1 && stk[2-pc].vtype()!=gcl::VT_NULL) adrec.SumId=stk[2-pc].Number();

   int ok=addv->RepeatDo(adrec,adrec);
   DBStr->MakeStruct(stk[1-pc].getNode(),adrec,2,1);
   return VAL(ok);
  }
  if(method=="GET" && pc>1)
  {
   DBStr->MakeStruct(stk[1-pc].getNode(),adrec,2,1);

   adrec.SumId=SumId;
   adrec.aarticle=drec.article;
   adrec.aprVer=drec.prVer;

   if(pc>1 && stk[2-pc].vtype()!=gcl::VT_NULL) adrec.SumId=stk[2-pc].Number();
   if(pc>2 && stk[3-pc].vtype()!=gcl::VT_NULL) adrec.aarticle=stk[3-pc].String();
   if(pc>3 && stk[4-pc].vtype()!=gcl::VT_NULL) adrec.aprVer=stk[4-pc].Number();

   int ok=addv->RepeatDo(adrec,adrec);
   DBStr->MakeStruct(stk[1-pc].getNode(),adrec,2,1);
   return VAL(ok);
  }
  if(method=="TOTFLD" && pc>=1)
  {
   if(!SumId) return VAL(-1);

   adrec.SumId=SumId;
   adrec.aarticle=TotArt;
   adrec.aprVer=TotVer;

   if(pc>1 && stk[2-pc].vtype()!=gcl::VT_NULL) adrec.SumId=stk[2-pc].Number();

   addv->RepeatDo(adrec,adrec);
   OraField *fld=adrec.FldN(stk[1-pc].String().data());
   if(!fld)
   {
     int i;
     for(i=0;i<DBStr->NFields();i++)
     {
      if(DBStr->FI(i).Alias==stk[1-pc].String())
      {
       fld=adrec.FldN(DBStr->FI(i).RealName.data());
       break;
      }
     }
     if(!fld) return StackItem();
   }
   return VAL(fld);
  }
  if(method=="LINE" && pc==0)
  {
   char *buf=new char [DBStr->DispLen()+DBStr->NFields()+1];
   DBStr->OutputRec(drec,buf,"\\|");
   ::string res=::string("\\|")+buf;
   delete [] buf;
   return VAL(res);
  }
  if((method=="LINE_EXP") && (pc==0 || pc==1))
  {
   int start=0 ;
   if (pc>0) start=stk[1-pc].Number() ;

   char *buf=new char [DBStr->DispLen(0)+DBStr->NFields()+1];
   DBStr->OutputRec(drec,buf,":",start);
   ::string res=::string(" ")+buf;
   delete [] buf;
   return VAL(res);
  }
  if(method=="LINE" && pc==1)
  {
   int _fld=stk[1-pc].Number() ;

   int fld=0 ;
   for(int i=0;i<DBStr->fis.size() ;i++)
   {
    if(DBStr->fis[i].DispLen())
      fld++ ;
    if (fld==_fld)
    {
     fld=i ;
     break ;
    }
   }

   if (fld>DBStr->NFields())
      return VAL("") ;

   char buf[256] ;
   int flags=(def_zeros ? 0 : GS_NOZERO) | (def_apps ? 0 : GS_NOAPPS) | GS_NOFULL ;
   DBStr->FI(fld).OutputFld(drec,buf,0,flags,def_point);
   return VAL(buf);
  }
  if(method=="HEADER" && pc==0)
  {
   char *buf=new char [DBStr->DispLen()+DBStr->NFields()+1];
   DBStr->OutputHeader(buf,"\\|");
   ::string res=::string("\\|")+buf;
   delete [] buf;
   return VAL(res);
  }
  if(method=="HEADER" && pc==1)
  {
   int _fld=stk[1-pc].Number() ;

   int fld=0 ;
   for(int i=0;i<DBStr->fis.size() ;i++)
   {
    if(DBStr->fis[i].DispLen())
      fld++ ;
    if (fld==_fld)
    {
     fld=i ;
     break ;
    }
   }


   if (fld>DBStr->NFields())
      return VAL("") ;
   return VAL(DBStr->FI(fld).Alias) ;
  }

  if(method=="FLDNUM" && pc==0)
  {
   // return VAL(DBStr->NFields()) ;
   int r=1 ;
   for(int i=0;i<DBStr->fis.size();i++)
   {
    if(DBStr->fis[i].DispLen())
      r++ ;
   }
   return VAL(r) ;
  }

  if(method=="FOOTER" && pc==0)
  {
   if(!SumId) return VAL("");
   adrec.aarticle=TotArt;
   adrec.aprVer=TotVer;
   adrec.SumId=SumId;
   addv->RepeatDo(adrec,adrec);
   char *buf=new char [DBStr->DispLen()+DBStr->NFields()+1];

   DBStr->OutputTotal(adrec,buf,"\\|");

   ::string res=::string("\\|")+buf;
   delete [] buf;
   return VAL(res);
  }
  if(method=="FOOTER" && pc==1)
  {
   int _fld=stk[1-pc].Number() ;

   int fld=0 ;
   for(int i=0;i<DBStr->fis.size() ;i++)
   {
    if(DBStr->fis[i].DispLen())
      fld++ ;
    if (fld==_fld)
    {
     fld=i ;
     break ;
    }
   }

   if (fld>DBStr->NFields() || !((DBStr->FI(fld).Opt()&Totalable) && DBStr->FI(fld).db==2))
      return VAL("") ;

   if(!SumId) return VAL("");
   adrec.aarticle=TotArt;
   adrec.aprVer=TotVer;
   adrec.SumId=SumId;
   addv->RepeatDo(adrec,adrec);

   char buf[256] ;

   int flags=(def_zeros ? 0 : GS_NOZERO) | (def_apps ? 0 : GS_NOAPPS) | GS_NOFULL ;
   DBStr->FI(fld).OutputFld(adrec,buf,0,flags,def_point);

   return VAL(buf);
  }
  if(method=="COND" && pc>=1)
  {
   if(pc>=2)
   {
    arg=stk[2-pc].getNode().Stru();
    if (arg==NULL)
       prog_ptr->RTError(301,::string(nosuchstructerr)+"  "+stk[2-pc].nd->name);
    if(!orphan)
     stk[2-pc].getNode().Structure().UpdateVar_Fld("CATID",&drec.catId);
   }
   if(query!="") query+=" AND ";
   query+=stk[1-pc].String();
   return node;
  }
  if(method=="ORDER" && pc==1)
  {
   ord=" ORDER BY "+stk[1-pc].String();
   return node;
  }
  if(method=="COR" && pc>=1)
  {
   if(query!="") query+=" AND ";
   query+="EXISTS (select * from *.";
   query+=DBStr->analname+" where article = aarticle and ver=aver and (";
   int i;
   char b[10];

   for(i=0;i<pc;i++)
   {
    if(i) query+=" OR ";
    int id=stk[1-pc+i].Number();
    query+="SumId=";
    query+=itoa(id,b,10);
   }
   query+="))";

   return node;
  }
  if (method=="INFO" && pc==1)
  {
   char s[256] ;
   DBStr->MakeStruct(stk[0].getNode(),drec,3);
   DBStr->OutputInfo(*arg,s) ;
   return VAL(s) ;
  }


  prog_ptr->RTError(101,::string(unknownfuncerr)+"  "+method);
  return StackItem();
}

void AnRow::Init(const ::string &aname,int _mode,int SumId,const char *article,int ver,char *header,const ::string &condition)
{
  ObjRec obj;
  if(GetObj(aname,obj,&crsr))
  {
   DBStr.Init(&crsr,obj,"",0);
  }
  else prog_ptr->RTError(101,::string(uknownobjecterr)+aname);
  mode=_mode;
  if(obj.otype==OT_DB) mode=1;
  ::string dbn="*.";
  switch (mode) {
   case 1: dbn+=DBStr.dbname; break;
   case 2: dbn+=DBStr.analname; break;
   case 3: dbn+=DBStr.dbname+",*."+DBStr.analname; break;
   case 4: dbn+=DBStr.dbname+",*."+DBStr.analname; break;
  }
  dv=new OraView(&crsr,dbn.data());
  dv->MakeRecord(drec);
  drec.SumId=SumId;

  if((mode==2 || mode==3) && !article)
  {
   ::string sql="SELECT COUNT(*) into :ver FROM *.";
   sql+=DBStr.analname;
   sql+=" WHERE sumid=:sumid ;" ;
   dv->DoPL(sql,drec);
   drec.SumId=SumId;
   if(drec.prVer==2)
   {
    if (mode==2)
      dv->Select(" SUMID=:SUMID AND AARTICLE!=' '",drec,drec);
    if (mode==3)
      dv->Select(" SUMID=:SUMID AND ARTICLE=AARTICLE AND VER=AVER AND AARTICLE!=' '",drec,drec);
    return;
   }
  }
  if(mode==1 && !article)
  {
   if (obj.oid==cf_dbid)
   {
    drec.article=cf_article ;
    drec.prVer=cf_ver ;
   }
   if(!GetDBLine(obj,&drec,0,3,0,"",condition)) return;
  }
  else if(mode==4)
  {
   if (!EditAnalSum(NULL,NULL,header,&crsr,obj,SumId,0,0,NULL,1,2,"",0,&drec))
       return ;
  }
  else
  {
   drec.article=article?article:TotArt;
   drec.prVer=article?ver:TotVer;
  }
  switch (mode) {
   case 1:
     dv->Select("ARTICLE=:ARTICLE AND VER=:VER",drec,drec);
     break;
   case 2:
     dv->Select(" SUMID=:SUMID AND AARTICLE=:ARTICLE AND AVER=:VER",drec,drec);
     break;
   case 3:
     dv->Select(" SUMID=:SUMID AND AARTICLE=:ARTICLE AND AVER=:VER AND ARTICLE=:ARTICLE AND VER=:VER",drec,drec);
     break;
   case 4:
     dv->Select(" SUMID=:SUMID AND AARTICLE=:ARTICLE AND AVER=:VER AND ARTICLE=:ARTICLE AND VER=:VER",drec,drec);
     break;
  }
}

void AnRow::LoadVar(Node *ths)
{
  DBStr.MakeStruct(*ths,drec,mode==4 ? 3 : mode,0);
}


