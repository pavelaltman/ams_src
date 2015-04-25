#include "AMS_HDR.h"
#pragma hdrstop

#include "amsclnt.h"
#include "analytic.h"
#include "FRM_EDIT_OPT.h"
#define SLEN 1000

extern char ImpPath[] ;
extern r_char impfiletitle , textimpopttitle ,
              prohnewartopt,prohsamedocopt , artnotexistmes ,
              continuemes , docalreadyexistmesbad , docalreadyexistmes ,
              totopersmes , importedopersmes , applyopersprompt ,
              substringartprompt , nosuchdberr, donotaskopt,
              nosuchdoctype, docexistmes, emptydocnum, docexist_dup,
              sumsnotmatchmes, operbynodemes, importingmes, prohupdateopt ;

int     OpImpFile(FILE *f);
int FileInputItem(OpRec &or,CondItemRec &ci,char *str,Sum &cursum,FILE *f);
int FileInputOper(char *essent,int ctype,FILE *f);
int FileInputAnalSum(bool HaveCats, cursor &crsr,int &sumid,int anal_id,string setname,
        char *str,FILE *f,int skip_num,char *bal_fld=NULL,Sum *sum=NULL) ;
int FileInputAnalSum1( cursor &crsr,int &sumid,int anal_id,string setname,
        char *str,FILE *f,int skip_num,char *bal_fld=NULL,Sum *sum=NULL, char* fname=NULL);
int GetLastVer(AnsiString dbname, AnsiString dbart);

extern OpVars *opvars;
bool AmsExp1251=false;

#define PROH_NEWART  1
#define PROH_SAMEDOC 2
#define APPLY_OPERS  4
#define SUB_STRING   8
#define PROH_UPDATE  16
#define DONOT_ASK    32
// Осуществлять перекодировку в WIN1251
#define AMSTO1251    64

static char key_str[80] ;

void TextOpImport2(const char* FName);

void TextOpImport()
{

  TOpenDialog* OD=DatMod->OD;
  OD->DefaultExt="*.ams";
  OD->Filter="Файлы импорта (*.ams)|*.ams|Текстовые файлы (*.txt)|*.txt|Все файлы(*.*)|*.*";
  if (!OD->Execute())
  {
    return;
  }
  TextOpImport2(OD->FileName.c_str());
}

void TextOpImport2(const char* FName)
{
  char fname[200];
  FILE *f;
  strcpy(fname, FName);
  f=fopen(fname,"rt");
  ShortMessage(importingmes) ;
  if(f)
  {
   char t[80];
   sprintf(t,"%s %s",importingmes,fname);
   ShortMessage(t) ;
   OpImpFile(f);
   fclose(f);
  }
  ShortMessage("") ;
}


char* strdelws(char *str)
{
 int i;
 while(isspace(*str)) *str++=0;
 for(i=strlen(str)-1;i>=0 && isspace(str[i]);i--) str[i]=0;
 i=strlen(str)-1;
 if(   (*str=='\"' && str[i]=='\"') || (*str=='\'' && str[i]=='\'')
    || (*str=='<'  && str[i]=='>' )  )
 { str[i]=0; str++; }
 return str;
}

enum { STD_Cond, Debet_Cond, Credit_Cond };



int FillCats(char* str, FILE*F);
int ImpDb1(char* str, FILE* f, char* ObjName, bool NeedCatId, AnsiString Art, int Ver);

int OpImpFile(FILE *f)
{
    AmsExp1251=G.ur.imp_opt & AMSTO1251;
 char str[SLEN];
 int tot_opers=0 , imp_opers=0 ;

 while(!feof(f) && fgets(str,SLEN,f))
 {
  if (AmsExp1251) My_OemToChar(str,str);
  if(!strncmpi(str,"[SC]",4))
  {
   int r=0;
   try {
    r=FileInputOper(str+4,STD_Cond,f) ;
    delete opvars ;
    opvars=NULL;
   }
   catch(OraError ERR)
   {
      ERR.message();
   }
   if(r) conn.commit();
   else conn.rollback();
   tot_opers++ ;
   if (r==0) continue ;
   imp_opers++ ;
  }
  if(!strncmpi(str,"[CAT]",4))
  {
   FillCats(str, f);
  }
  if(!strncmpi(str,"[DB1]",5))
  {
    char ObjName[30];
    strcpy(ObjName, strdelws(str+5));
    fgets(str,SLEN,f);
    if (AmsExp1251) My_OemToChar(str,str);
    ImpDb1(str, f, ObjName, true, "", 0);
  }
  if(!strncmpi(str,"[DB]",4))
  {
   ObjRec obj ;
   cursor crsr(conn) ;

   if (!GetObj(strdelws(str+4),obj,&crsr))
      throw OraError(0,nosuchdberr) ;
   fgets(str,SLEN,f);
   if (AmsExp1251) My_OemToChar(str,str);

   DBStructInfo DBS(&crsr,obj.oid,"",0);
   int rootCat=DBRootCat(DBS.dbid,&crsr);

   OraTable dt(&crsr,("*."+DBS.dbname).data());

   PosBuf drec,exdrec;

   dt.MakeRecord(drec);
   dt.MakeRecord(exdrec);

   char *s=strdelws(str);

   do
   {
    int field,stop;
    drec.SetNull();
    exdrec.SetNull();

    char *s1;
    for(field=1,stop=0;*s && !stop;field++)
    {
     s1=s;
     for(;*s && *s!='\n' && *s!='|' && *s!=':';s++);
     if(!*s || *s=='|')  stop=1;
     if(*s) *s++=0;
     DBS.FI(field).InputFld(exdrec,strdelws(s1));
    }
    if (exdrec.prVer.isNull())
        exdrec.prVer=0 ;

    fgets(s=str,SLEN,f);
    if (AmsExp1251) My_OemToChar(str,str);
    s=strdelws(s);

    // импорт полей типа "Аналитическая сумма"
    if(!strncmpi(str,"[ASUM]",6))
    {
     char fname[80] ;
     strcpy(fname,strdelws(str+6)) ;

     int sum_id ;
     FileInputAnalSum(false, crsr,sum_id,DBS.FI(fname).ref_dbId,"",str,f,6) ;
     char buffer[50] ;
     itoa(sum_id,buffer,10) ;
     DBS.FI(fname).InputFld(exdrec,buffer);
    }

    dt.Do(" WHERE article=:article and ver=:ver",exdrec);
    int found=dt.fetch(drec);

    try
    {
    if (!found)
    {
     char mes[80] ;
     if (G.ur.imp_opt&PROH_NEWART)
     {
      sprintf(mes,"%s %s %d",artnotexistmes,exdrec.article.val.data(),exdrec.prVer.val) ;
      throw OraError(0,mes);
     }
     else
     {
      sprintf(mes,"%s %s %d. %s",artnotexistmes,exdrec.article.val.data(),exdrec.prVer.val
             ,continuemes) ;
      if (!(G.ur.imp_opt&DONOT_ASK))
        if (AskMessage(mes, false)!=ID_YES)
        {
            return 0;
        }
     }
     exdrec.catId=rootCat;
     DBS.CalcDBLine(exdrec);
     dt.InsertRec(exdrec);
    }
    else if (!(G.ur.imp_opt & PROH_UPDATE))
    {
     // Updating database fields
     exdrec.catId=drec.catId ;
     exdrec.row_id=drec.row_id ;
     // dt.UpdateRec_ID(exdrec);

     if (obj.hist)
     {
//      DBS.BeforeInsRec(exdrec,&crsr,1) ;

      AuxRec rec ;
      rec.int1=obj.oid ;
      rec.str1=drec.article ;
      rec.int2=drec.prVer ;
      rec.int3.SetNull() ;
      dt.DoPL("*.DBRevision(:int1,:str1,:int2,:int3);",rec) ;

      drec.catId=rec.int3 ;

      dt.OraDML::Do(DBS.InsertSTMT(1,1),drec);
     }

     dt.OraDML::Do(DBS.UpdateSTMT(1,1),exdrec);
    }
    }
    catch(OraError ERR)
    {
     message((char*)(exdrec.article.val.data()),BAD) ;
     ERR.message();
    }

    {
     char t[80];
     sprintf(t,"%s%s",importingmes,exdrec.article.val.data());
     ShortMessage(t) ;
    }
    conn.commit() ;

   } while(s[0]!='[');
  }
 }

 char s1[80] , s2[80] ;
 sprintf(s1,"%-30s %d\r\n",totopersmes,tot_opers) ;
 sprintf(s2,"%-30s %d",importedopersmes,imp_opers) ;
 Message(AnsiString(s1)+s2) ;

 return 1;
}


void ReadOpv(char *str,FILE *f)
{
 if(!strncmpi(str,"[OPV]",5))
 {
  int field,stop;
  char *s,*s1;
  s=strdelws(str+5) ;

  for(field=3,stop=0;*s && !stop;field++)
  {
   s1=s;
   for(;*s && *s!='\n' && *s!='|' && *s!=':';s++);
   if(!*s || *s=='|')  stop=1;
   if(*s) *s++=0;
   opvars->DBStr.FI(field).InputFld(opvars->opvars,strdelws(s1));
  }

  //if(!opvars->DBStr.InputRecDlg(opvars->opvars,IR_NOTBUSY|IR_INIT|IR_NOSYS)) return;

  opvars->operdb->UpdateRec_ID(opvars->opvars);
  conn.commit() ;

  fgets(str,SLEN,f);
  if (AmsExp1251) My_OemToChar(str,str);
 }
}


void ReadHist(char *str,FILE *f,int op_id,int op_subid)
{
 if(!strncmpi(str,"[HIST]",6))
 {
  char *s,*s1;
  s=strdelws(str+6) ;

  do
  {
   char uname[30] ;
   AuxRec aux ;

   aux.int1=op_id ;
   aux.int2=op_subid ;
   s1=s;
   for(;*s && *s!='\n' && *s!=':';s++);
   if(*s)*s++=0;
   strcpy(uname,strdelws(s1)) ; // имя пользователя
   aux.str1=uname ; // имя пользователя

   s=strdelws(s);


   tm dt ;
   sscanf(s,"%02d-%02d-%02d %02d:%02d",&dt.tm_mday,&dt.tm_mon,&dt.tm_year,&dt.tm_hour,&dt.tm_min);
   if (dt.tm_year<17) dt.tm_year+=100 ;
   dt.tm_year+=1900;
   TDateTime Dt = EncodeDate (dt.tm_year, dt.tm_mon, dt.tm_mday);
   TDateTime Tm = EncodeTime (dt.tm_hour, dt.tm_min, 0, 0);
   DateTime DT = Dt+Tm;
   aux.date1=DT ;

   cursor crsr(conn) ;
   OraDML Q(&crsr) ;
   Q.DoPL("*.NEW_HIST_RECORD(:int1,:int2,:date1,:str1);",aux) ;
   conn.commit() ;

   fgets(str,SLEN,f);
   if (AmsExp1251) My_OemToChar(str,str);
   s=strdelws(str);
  } while(s[0]!='[');

  conn.commit() ;
 }
}


int FileInputOper(char *essent,int ctype,FILE *f)
{
 OpRec or;
 or.post=(G.ur.imp_opt&APPLY_OPERS) ? 0 : 1;
 or.status=0;
 or.storno=0 ;
 {
  int ok=1;
  char *s;
  for(s=essent;*s && *s!=':';s++);
  if(!*s) ok=0;
  else *s=0;

  //Получаем тип документа
  or.doc_type=strdelws(essent);
  
  if(ok)
  {
   for(essent=++s;*s && *s!=':';s++);
   if(!*s) ok=0;
   else *s=0;
  //Получаем номер документа
   or.doc_num=strdelws(essent);
  }
  DateTime date;
  if(ok)  date.SetStr(s+1);
  //Получаем дату
  or.op_date=date;
  if(date<G.BegDate() || date>=G.EndDate())
        throw OraError(0,"Дата операции вне пределов периода!");
 }

 CondRec cond;
 cursor crsr(conn);
 OraTable ot(&crsr,"*.opers");
 ot.DoPL("SELECT *.OperSeq.NEXTVAL INTO :OP_ID FROM DUAL;",or) ;
 or.op_subid=0 ;
 Sum cursum=0;
 //Ищем операцию  с таким шаблоном
 if(ctype==STD_Cond)
 {
  OraView scds(&crsr,"*.conds c,*.objs o, *.cattree t","c.*");
  scds.MakeRecord(cond);
  cond.doc_type=or.doc_type;
  scds.Do(" WHERE c.cond_id=o.oid AND DOC_TYPE=:doc_type AND o.catid=t.owncat ORDER BY TORD,o.ORD",cond) ;
  if(!scds.fetch(cond))
  {
        char mes[70];
        sprintf(mes," Неопределена операция %s! ",or.doc_type.val.data());
        throw OraError(0,mes);
  }
  //Получаем название  
  or.text=cond.text;
  or.cond_id=cond.cond_id;
  or.cond_subid=cond.cond_subid;
  or.op_subid=cond.cond_subid;
 }
 char str[SLEN+1];

 fgets(str,SLEN,f);
 if (AmsExp1251) My_OemToChar(str,str);

 if(!strncmpi(str,"[TIT]",5))
 {
  or.text=strdelws(str+5);
  fgets(str,SLEN,f);
  if (AmsExp1251) My_OemToChar(str,str);
 }

  OpRec oprec ;
  oprec.op_id=or.op_id ;
  oprec.op_subid=or.op_subid ;
  oprec.doc_type=or.doc_type ;
  oprec.doc_num=or.doc_num ;
  oprec.status=0 ;
  oprec.storno=0 ;
  oprec.op_date=or.op_date ;
  oprec.old_id.SetNull() ;
  oprec.old_subid.SetNull() ;
  ot.DoPL("*.bookkern.ProcessDocNum(:op_id,:op_subid,:old_id,:old_subid,:op_date,:doc_type,:doc_num,:doc_num_v,:status,:result);",oprec) ;

  if (oprec.result==1 || oprec.result==2)
       { message(nosuchdoctype,BAD) ; return 0; }
  if (oprec.result==3  || (oprec.doc_num_v>0 && (G.ur.imp_opt&PROH_SAMEDOC)))
  {
        ErrorMessage(AnsiString(docexistmes)+(oprec.doc_type.val+oprec.doc_num.val).data());
        return 0;
  }
  if (oprec.result==4)
       { message(emptydocnum,BAD) ; return 0; }

  if (oprec.result==10)
       if (AskMessage((char*)(string(docexist_dup)+" "+oprec.doc_type.Val()+" "+oprec.doc_num.Val()).data(), false)!=ID_OK)
       return 0;

 or.doc_num=oprec.doc_num;
 or.doc_num_v=oprec.doc_num_v ;

 ot.InsertRec(or);

 delete opvars ;
 opvars=new OpVars ;
 opvars->Init(or,ot,&crsr) ;
 opvars->SetKey(or) ;

 ReadOpv(str,f) ;

 ReadHist(str,f,or.op_id,or.op_subid) ;

 if(ctype==STD_Cond)
 {
  CondItemRec ci;
  OraView cdi(&crsr,"*.cond_items");
  cdi.MakeRecord(ci);
  ci.cond_id=cond.cond_id;
  ci.cond_subid=cond.cond_subid;
  cdi.Select("cond_id=:cond_id and cond_subid=:cond_subid"," ORDER BY item_num",ci);
  while(cdi.fetch(ci))
  {
     if (!FileInputItem(or,ci,str,cursum,f)) return 0;
  }
 }

 ReadOpv(str,f) ;

 while(strnicmp(str,"[END]",5))
 {
    fgets(str,SLEN,f);
    if (AmsExp1251) My_OemToChar(str,str);
 }
 {
  ApplyRec rec ;
  rec.op_id=or.op_id ;
  rec.op_subid=or.op_subid ;
  ot.DoPL("*.bookkern.ApplyOper(:OP_ID,:OP_SUBID,:RESULT,:ERRM);",rec) ;
  int res=rec.result ;
  if (res!=0)
  {
   AnalyzeRes(res) ;
   if (res!=R_NEGREMSOK) return 0 ;
  }
 }

 return 1 ;
}


int FileInputAnalSum(bool HaveCats, cursor &crsr,int &sumid,int anal_id,string setname,char *str,FILE *f,int skip_num,char *bal_fld,Sum *sum)
{
   char *s ;
   AuxRec rec ;
   int sfld ;
   int LastVer=0;

   rec.int1=0 ;
   OraDML Q(&crsr) ;
   Q.DoPL("*.CreateSum(:INT1,0);",rec) ;
   sumid=rec.int1 ;

   DBStructInfo DBS(&crsr,anal_id,setname,0);
   int rootCat=DBRootCat(DBS.dbid,&crsr);
   OraView dv(&crsr,DBS.AnalView().data(),DBS.AnalHeader().data());
   OraTable totv(&crsr,string(string("*.")+DBS.analname).data());
   PosAnalBuf drec,dexr,totbuf,exdrec;

   dv.MakeRecord(drec);
   dv.MakeRecord(exdrec);
   totv.MakeRecord(totbuf);
   totbuf.InitTot(sumid);

   
   AmsQuery AQ;
   if (HaveCats)
   {
       AQ.SetSqlText(
            "select OCAT_HERE from *.CATS_MAP where DB_ID=:DB_ID and OCAT_THERE=:THERE");
       AQ.DeclVarInt("DB_ID", DBS.dbid);
       AQ.DeclVar("THERE", 0);
   }

      s=strdelws(str+skip_num+HaveCats);
   int prfld=0 ;
   std::vector<int> prfld_lst;
   while (*s!='\0')
   {
       if (!strncmpi(s,"[PRFLD=",7))
       {
       prfld=strtol(s+7,&s,10) ;
       prfld_lst.push_back(prfld);
       s=strdelws(s+1);
       }

       if (!strncmpi(s,"[SFLD=",6))
       {
        int _sfld=strtol(s+6,&s,10) ;
        itoa(_sfld,key_str,10) ;
        s=strdelws(s+1);
       }
    }
   if(!*s) { fgets(s=str,SLEN,f); if (AmsExp1251) My_OemToChar(str,str);s=strdelws(s); }

   while(s[0]!='[')
   {
   int field,stop,skip=0;
   drec.SetNull();

   // Database fields
   char *s1;

   for(field=(!HaveCats),stop=0;*s && !stop;field++)
   {
    s1=s;
    for(;*s && *s!='\n' && *s!='|' && *s!=':';s++);
    if(!*s || *s=='|')  stop=1;
    if(*s) *s++=0;
    DBS.FI(field).InputFld(exdrec,strdelws(s1));
   }
   int CurrCatId = rootCat;
   if (HaveCats)
   {
        CurrCatId=exdrec.catId;
        AQ.SetVar("THERE", CurrCatId);
        AQ.Exec("Находим свою категорию");
        if (AQ.NotEof())
        {
            CurrCatId=AQ.GetFldInt(0);
        }
        else CurrCatId= rootCat;
   }

   string cnd=DBS.AnalCond(0,1,sumid,0);

   LastVer=GetLastVer((AnsiString)DBS.dbname.data(), (AnsiString)exdrec.article.val.data());

   sfld=atoi(key_str)+1 ;
   if (!strempty(key_str))
   {
    string fn=DBS.FI(sfld).RealName ;

    cnd+=" and "+fn+"=:"+fn+" ORDER BY TORD" ;
   }
   else if (G.ur.imp_opt&SUB_STRING)
   {
    char srch_str[80] ;
    strcpy(srch_str,exdrec.article.val.data()) ;
    rus_strupr(srch_str) ;

    cnd+=string(" and INSTR(UPPER(ARTICLE),'")+srch_str+"')>0";
   }
   else
   {
    cnd+=" and article=:article" ;
    cnd+=" and ver=:ver";
    if (exdrec.prVer.isNull())
        exdrec.prVer=0;
   }
    int found=0;
   for(int i=LastVer;i>=0;i--)
   {
     exdrec.prVer=i;
     dv.Select(cnd,"",exdrec);
     if (dv.fetch(drec))
     {
        found=1;
        break;
     };
   };
   if (!strempty(key_str) && found==0)
   {
    char srch_str[80] ;
    DBS.FI(sfld).OutputFld(exdrec,srch_str,0);
    drec.article=srch_str ;
    drec.prVer=0 ;
    exdrec.article=srch_str ;
    exdrec.prVer=0 ;
   }

   if (!found)
   {
    char mes[80] ;
    if (G.ur.imp_opt&PROH_NEWART)
    {
     sprintf(mes,"%s %s %d",artnotexistmes,exdrec.article.val.data(),exdrec.prVer.val) ;
     throw OraError(0,mes);
    }
    else
    {
     sprintf(mes,"%s(%s) %s %d. %s",artnotexistmes,DBS.dbname.data(),exdrec.article.val.data(),exdrec.prVer.val
          ,continuemes) ;
     int res=1 ;
     if (!(G.ur.imp_opt&DONOT_ASK))
       res=AskMessage(mes) ;
//     if (res==ID_YES)
//        return 0;
     if (res==ID_NO)
       skip=1 ;
    }
    if (!skip)
    {
     exdrec.catId=CurrCatId;
     DBS.CalcDBLine(exdrec);
     try
     {
      dv.OraDML::Do(DBS.InsertSTMT(1),exdrec);
     }
     catch(OraError ERR)
     {

      ERR.message();
      return 0 ;
     }
     dv.Select(cnd,"",exdrec);
     dv.fetch(drec);
    }
   }
   else
   {
    // Updating database fields
    //exdrec.catId=drec.catId ;
    //dv.OraDML::Do(DBS.UpdateSTMT(1),exdrec);
    bool newver=false;
    char str1[256] , str2[256] ;
    for(unsigned int i=0;i<prfld_lst.size();i++)
    {
     DBS.FI(prfld_lst[i]).OutputFld(exdrec,str1,0);
     DBS.FI(prfld_lst[i]).OutputFld(drec,str2,0);
     newver=((newver)||(strcmp(str1,str2)));
    }
    prfld_lst.clear();
     if(newver)
     {
      exdrec.catId=drec.catId ;

      DBS.FI(prfld).InputFld(drec,str1);

      DBS.CalcDBLine(exdrec);

      while(1)
      {
       try
       {
//        exdrec.prVer.val++ ;
        exdrec.prVer=LastVer+1;
        drec.prVer=LastVer+1;
        dv.OraDML::MakeSilent();
        int r=dv.OraDML::Do(DBS.InsertSTMT(1),exdrec);
        if (r==0)  break ;
       }
       catch(OraError ERR)
       {
         ERR.message();
         return 0 ;
       }
//      }
      drec.prVer=exdrec.prVer ;
   //         dv.Select(cnd,"",exdrec);
   //         dv.fetch(drec);
     }
    }
   }
   drec.aarticle=drec.article;
   drec.aprVer=drec.prVer;
   drec.SumId=sumid ;
   /*
   AnsiString AS="";
   for (int i = 0 ; i< DBS.fis.size();i++)
   {
        FInfo &DD = DBS.FI(field);
        AS+=IntToStr(i)+":"+IntToStr(DD.db)+","+DD.RealName.data()+"| ";
   }
   */
   for(field=DBS.GetFirstAnalField()+3;*s;field++)
   {
    s1=s;
    for(;*s && *s!='\n' && *s!='|' && *s!=':';s++);
    if(*s) *s++=0;


    // Поддержка ','
    enum VTYPE { VT_NULL=0,VT_DATE,VT_NUM,VT_INT,VT_ROWID,VT_STRING };
    if (DBS.FI(field).vtype==VT_NUM)
    {
     char *p=strchr(s1,',') ;
     if (p)
     *p='.' ;
    }

    DBS.FI(field).InputFld(drec,strdelws(s1));
   }

   if (!skip)
   {
    DBS.CalcAnLine(drec,totbuf);
    if(DBS.CheckRec(drec))
    {
     dv.OraDML::Do(DBS.InsertSTMT(2),drec);
     DBS.SumBuf(totbuf,drec,1);
    }
   }
   fgets(s=str,SLEN,f);
   if (AmsExp1251) My_OemToChar(str,str);
   s=strdelws(s);
   }

   totv.InsertRec(totbuf);   

   if (bal_fld)
   {
    char sustr[30] ;
    DBS.FI(bal_fld).OutputFld(totbuf,sustr,0);
    sum->SetStr(sustr);
   }

 return 1 ;
}

int FileInputItem(OpRec &or,CondItemRec &ci,char *str,Sum &cursum,FILE *f)
{
 cursor crsr(conn);
 OpItemRec oi;
 oi.op_id=or.op_id;
 oi.op_subid=or.op_subid;
 oi.op_date=or.op_date;
 oi.code=ci.code;
 oi.side=ci.side;
 oi.for_post=ci.for_post ;
 oi.setname=ci.setname ;
 oi.node=0;

 AccRec ar;
 {
  OraView av(&crsr,"*.accs");
  av.MakeRecord(ar);
  av.Select("code=:code",oi,ar);
 }
 OraTable st(&crsr,"*.sums") ;
 SumRec sr;
 st.MakeRecord(sr);
 OraTable opi(&crsr,"*.op_items") ;

 char *s;
 *key_str=0 ;

 if (ar.anal_id)
 {
     bool HaveCats=false;
     if( strncmpi(str,"[AA]",4) && strncmpi(str,"[AA1]",5))
     {
        OpItemRec soi;
        opi.MakeRecord(soi);
        soi.op_id=oi.op_id;
        soi.op_subid=oi.op_subid;
        soi.code=ar.anal_id;
        if(opi.Select("op_id=:op_id and op_subid=:op_subid and code in "
                 "(select code from *.accs where anal_id=:code)",soi,soi)) //And
        {
          sr.sum_id=soi.item_sum_id;
          st.Select("SUM_ID=:SUM_ID",sr,sr) ;
          oi.value=sr.value ;
          st.DoPL("*.CreateSum(:SUM_ID,:VALUE);",sr) ;
          AuxRec aux;
          aux.int1=soi.item_sum_id;
          aux.int2=oi.item_sum_id=sr.sum_id;
          aux.sum1=ar.anal_id.val;
          st.DoPL("*.CopyAnalSum(:sum1,:int2,:int1);",aux) ;
          opi.InsertRec(oi);
          if(oi.side=="D") cursum-=oi.value;
          else cursum+=oi.value;
          return 1;
        }
        else return 1 ; // throw OraError(0,"[AA] expected!");
     }

     if (!strncmpi(str,"[AA1]",5))
     {
        HaveCats=true;
     }

     Sum sum ;
     int sum_id ;
     FileInputAnalSum(HaveCats, crsr,sum_id,ar.anal_id,ci.setname.Val()>"" ? ci.setname : ar.setname,str,f,4,(char*)(ar.bal_fld.val.data()),&sum) ;
     oi.item_sum_id=sum_id ;

     sr.value=sum;
     sr.sum_id=oi.item_sum_id;
     st.UpdateRecs("VALUE=:VALUE WHERE SUM_ID=:SUM_ID",sr) ;

     opi.InsertRec(oi);

     if (ar.bal)
     {
      if(oi.side=="D") cursum-=sum;
      else cursum+=sum;
     }

     return 1;
 }
 else  // Synthetic account
 {
   Sum D; D.Zero() ;

   if(!strncmpi(str,"[SS]",4))
   {
    s=strdelws(str+4);
    sr.value.Input(s,2) ;
    if (ar.bal)
    {
     if(oi.side=="D") cursum-=sr.value ;
     else cursum+=sr.value ;
    }

    fgets(s=str,SLEN,f);
    if (AmsExp1251) My_OemToChar(str,str);
   }
   else
   {
    sr.value= (oi.side=="D") ? cursum : D-=cursum ;
    cursum=0;
   }

   st.DoPL("*.CreateSum(:SUM_ID,:VALUE);",sr) ;
   oi.item_sum_id=sr.sum_id;
   opi.InsertRec(oi);
   return 1;
 }
}
#if 0
int PosTab::FileInput(FILE *f,char *str)
{
  TPos_Buffer  *pos_buf=CreateBuf() ;
  long catId,lcatId;

  {
   gotoLast();
   if(getRec(pos_buf)) catId=CATSPACE;
   else catId=lcatId=pos_buf->catId;
  }

  while(!feof(f) && fgets(str,SLEN,f))
  {
    if (AmsExp1251) My_OemToChar(str,str);
    ClearBuf(pos_buf);

    if(!strncmpi(str,"[CAT]",5))
    {
        FillCatId (str, f);

/*
     char catname[120];
     int found=0;
     strcpy(catname,strdelws(str+5));
     SetArticle();
     for(gotoBegin();!gotoNext();)
     {
      getRec(pos_buf);
      if(pos_buf->article[0]) break;
      char cname[51];
      stru->OutputFld(pos_buf,CATFld(),cname) ;
      if(!strcmp(cname,catname))
      {
       found=1;
       catId=pos_buf->catId;
      }
     }
     SetCategory();

     if(!found)
     {
       pos_buf->article[0]=0;
       pos_buf->prVer=0;
       stru->InputFld(pos_buf,CATFld(),catname);
#ifdef HIERINP
       if(flags&DBF_HIER)
       {
           lcatId &= MaskVal(0) ;
           lcatId += IncVal(0) ;
           pos_buf->catId = catId = lcatId ;
       }
       else
#endif
         pos_buf->catId= catId = (lcatId+=CATSPACE);
       insertRec(pos_buf);
     }
     continue;
*/
    }

    if(!strncmpi(str,"[END]",5)) break;

    if(str[0]=='[')
    {
      Delete(pos_buf);
      return Error("Неизвестная комманда");
    }

    int field;
    char *s=strdelws(str),*s1;
    for(field=0;*s && *s!='\n';field++)
    {
     s1=s;
     for(;*s && *s!='\n' && *s!=':';s++);
     if(*s)  *s++=0;
     stru->InputFld(pos_buf,field,strdelws(s1));
    }
    if(searchFPN(pos_buf,1))
    {
     pos_buf->catId=catId;
     insertRec(pos_buf);
    }
//    else getRec(pos_buf);
  }
  Delete(pos_buf);
  return 1;
}
#endif

//---------------------------------------------------------------------
int GetInt(char* Str, char* & Res)
{
    char* C = strchr(Str, ':');
    if (C)
    {
        *C=0;
        C++;
    }
    Res=C;
    C=Str;
    while ( *C==' ' || *C=='\n') C++;
    char* end=strchr(C, '\n');
    if (end) *end=0;

    int Res1=*C?StrToInt(C):0;
    return Res1;
}
//---------------------------------------------------------------------
double GetSum(char* Str, char* & Res)
{
    char* C = strchr(Str, ':');
    if (C)
    {
        *C=0;
        C++;
    }
    char* F= Str;
    while (*F)
    {
        if (*F=='-' || *F=='.' || *F==',')
        {
            *F=DecimalSeparator;
        }
        F++;
    }
    Res=C;
    C=Str;
    while ( *C==' ' || *C=='\n') C++;
    char* end=strchr(C, '\n');
    if (end) *end=0;
    double Res1=*C?StrToFloat(C):0;
    return Res1;
}
//---------------------------------------------------------------------
AnsiString GetStr(char* Str, char* & Res)
{
    char* C = strchr(Str, ':');
    if (C)
    {
        *C=0;
        C++;
    }

    Res=C;
    strdelws(Str);
    return Str;
}

int FindEnd(char* str, FILE* f)
{

    while (!feof(f))
    {
        fgets(str,SLEN,f);
        if (!strncmpi(str, "[END]", 5))
        {
            return 0;
        }

    }
    ErrorMessage("Отсутствует [END]");
    return 1;
}
//---------------------------------------------------------------------
// Импорт категорий
// Формат
/*
[CAT]DB_NAME
OWNCAT:CATID:ORD:Название категории
1::5:/
3:1:6:Adidas
[END]
*/
int FillCats(char* str, FILE*f)
{
    char* S= str+5;
    AmsQuery AQ;
    AQ.SetSqlText("select OID from *.OBJS where NAME=:NAME and OTYPE=4");
    char *end =strchr(S, '\n');
    if (end) *end=0;
    AQ.DeclVar("NAME",S);
    AQ.Exec("Код базы данных");
    int DbCode=0;
    int RootCat=0;
    if (!AQ.NotEof())
    {
        FindEnd(str,f);
        ErrorMessage(AnsiString("Не найдена база данных с именем ")+S);
        return 0;
    }
    else
        DbCode= AQ.GetFldInt(0);
    AQ.Clear();
    AQ.SetSqlText("begin select OWNCAT into :RES from *.DBCAT where DBID=:DB_ID and "
            "CATID is NULL; end;");
    AQ.DeclVar("DB_ID", DbCode);
    AQ.DeclVar("RES", 0);
    AQ.Exec("Находим корневую категорию");
    RootCat=AQ.GetVarInt("RES");

    //-----------------------------------------------------------------------
    // Удаляем все категории уже удаленные в БД
    //-----------------------------------------------------------------------
    AQ.Clear();
    AQ.SetPLText("delete from *.CATS_MAP where DB_ID=:DBID and "
	    "not exists( select 1 from *.DBCAT where DBID=:DBID and "
        "OWNCAT=OCAT_HERE);commit;");
    AQ.DeclVarInt("DBID", DbCode);
    AQ.Exec("Удаляем все категории уже удаленные в БД");

    AnsiString Errs;
    while (!feof(f))
    {
        fgets(S=str,SLEN,f);
        if (!strncmpi(str, "[END]", 5))
        {
            if (Errs!="")
            {
                ErrorMessage("Проблемы при вставке категорий\r\n"+Errs);
            }
            AQ.Clear();
            AQ.SetPLText("*.MakeDBTree(:DBID); commit;");
            AQ.DeclVar("DBID", DbCode);
            AQ.Exec("Перестраиваем категории БД");
            return 1;
        }
        if (AmsExp1251) My_OemToChar(str,str);
        char* S = str;
        int OwnCat = GetInt(S,S);
        int CatId = GetInt(S,S);
        int Ord=GetInt (S,S);
        int MyCatId=0;
        int MyOwnCat=0;
        bool NoChangeCat = CatId==-1;
        bool NoChangeOrd = Ord==-1;
        AnsiString CatName= GetStr(S,S);

        if (CatId==0) // Предполагаем что корневая категория
        {
            AQ.Clear();
            AQ.SetPLText(
            "select max(1) into :RES from *.CATS_MAP where DB_ID=:DB_ID and OCAT_THERE=:THERE;"
            "if :RES is NULL then "
            "select OWNCAT into :RES from *.DBCAT where DBID=:DB_ID and CATID is NULL;"
            "insert into *.CATS_MAP (DB_ID, OCAT_THERE, OCAT_HERE) values (:DB_ID, :THERE, :RES);"
            "end if;");
            AQ.DeclVar("RES",0);
            AQ.DeclVar("DB_ID", DbCode);
            AQ.DeclVar("THERE", OwnCat);
            AQ.Exec("");
        }
        else
        {
            // Проверяем есть ли уже такая категория
            AQ.Clear();
            AQ.SetSqlText(
                "select OCAT_HERE from *.CATS_MAP where DB_ID=:DB_ID and OCAT_THERE=:THERE");
            AQ.DeclVar("DB_ID", DbCode);
            AQ.DeclVar("THERE", OwnCat);
            AQ.Exec("Находим соответств категорию");
            if (AQ.NotEof())
            {
                MyOwnCat=AQ.GetFldInt(0);
            }
            if (!NoChangeCat)
            {
                AQ.SetVar("THERE", CatId);
                AQ.Exec("Находим соответств категорию");
                if (AQ.NotEof())
                {
                    MyCatId=AQ.GetFldInt(0);
                }
                else
                {
                    Errs+="Не вставил категорию "+CatName+". Сначала должна идти родительская категория!!!\n";
                }
            }
            if (MyCatId || NoChangeCat)
            {
                if (MyOwnCat)
                {
                    AQ.Clear();
                    AnsiString S="update *.DBCAT set DESCR=:DESCR";
                    if (!NoChangeCat)
                    {
                        S+=", CATID=:CATID";
                        AQ.DeclVar("CATID", MyCatId);
                    }
                    if (!NoChangeOrd)
                    {
                        S+=", ORD=:ORD";
                        AQ.DeclVar("ORD",Ord);
                    }

                    AQ.SetPLText(S+" where DBID=:DBID and OWNCAT=:OWNCAT;commit;");
                    AQ.DeclVar("DESCR",CatName);
                    AQ.DeclVar("OWNCAT", MyOwnCat);
                    AQ.DeclVar("DBID", DbCode);
                    AQ.Exec("Выполняем обновление категории");
                }
                else
                {

                    AQ.Clear();
                    if (NoChangeCat) MyCatId=RootCat;
                    AnsiString S;
                    if (NoChangeOrd)
                        S="select max(ORD) into :ORD from *.DBCAT "
                            "where DBID=:DBID; :ORD:=:ORD+16; ";
                    AQ.SetPLText( S+
                    "*.InsertDBCat(:DBID,:CATID,:DESCR,:KEY,:RES);"
                    "update *.DBCAT set ORD=:ORD where DBID=:DBID and OWNCAT=:KEY;"
                    "insert into *.CATS_MAP (DB_ID, OCAT_THERE, OCAT_HERE) values "
                                                      "(:DBID, :OCAT_THERE, :KEY);"
                    "commit;");
                    AQ.DeclVar("ORD",Ord);
                    AQ.DeclVar("DESCR",CatName);
                    AQ.DeclVarInt("KEY");
                    AQ.DeclVar("RES", 0);
                    AQ.DeclVar("OCAT_THERE", OwnCat);
                    AQ.DeclVar("CATID", MyCatId);
                    AQ.DeclVar("DBID", DbCode);
                    AQ.Exec("Выполняем вставку категории");
                }
            }
        } // If not root cat
    } // while not eof
    ErrorMessage ("Ожидался [END]");
    return 0;
};
//---------------------------------------------------------------------
void ImportOptions()
{
 unsigned int imp_opt=G.ur.imp_opt ;
 TOptList OL;
    OL.Add (prohnewartopt,PROH_NEWART);
    OL.Add (prohsamedocopt,PROH_SAMEDOC);
    OL.Add (applyopersprompt,APPLY_OPERS);
    OL.Add (substringartprompt,SUB_STRING);
    OL.Add (prohupdateopt,PROH_UPDATE);
    OL.Add (donotaskopt,DONOT_ASK);
    OL.Add ("Осуществлять перекодировку в WIN1251",AMSTO1251);

 TFrmEditOpt* Frm;
 Application->CreateForm (__classid(TFrmEditOpt), & Frm);
 if (Frm->EditOpt(textimpopttitle,imp_opt,OL))
 {
    G.ur.imp_opt=imp_opt ;
    AmsExp1251=imp_opt & AMSTO1251;
    AmsQuery AQ ;
    AQ.SetPLText("UPDATE *.user_options SET IMP_OPT=:IMP_OPT WHERE USERNAME=USER;commit;");
    AQ.DeclVarSum("IMP_OPT", imp_opt) ;
    AQ.Exec("Пользовательские установки для импорта");
 }
 Frm->Release();
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void OpRec::MakeLink(FieldLink &lnk, int inp, int sn)
{
 static FieldIntDef flds[]={
      {"ROW_ID",STRING_TYPE,19,(OraField FieldSet::*)&OpRec::row_id },
      {"OP_ID",INT_TYPE,sizeof(int),(OraField FieldSet::*)&OpRec::op_id},
      {"OP_SUBID",INT_TYPE,sizeof(int),(OraField FieldSet::*)&OpRec::op_subid},
      {"STATUS",INT_TYPE,sizeof(int),(OraField FieldSet::*)&OpRec::status},
      {"POST",INT_TYPE,sizeof(int),(OraField FieldSet::*)&OpRec::post},
      {"STORNO",INT_TYPE,sizeof(int),(OraField FieldSet::*)&OpRec::storno},
      {"TEXT",STRING_TYPE,0,(OraField FieldSet::*)&OpRec::text},
      {"DOC_TYPE",STRING_TYPE,0,(OraField FieldSet::*)&OpRec::doc_type},
      {"DOC_NUM",STRING_TYPE,0,(OraField FieldSet::*)&OpRec::doc_num},
      {"DOC_NUM_V",INT_TYPE,sizeof(int),(OraField FieldSet::*)&OpRec::doc_num_v},
      {"OP_DATE",DATE_TYPE,0,(OraField FieldSet::*)&OpRec::op_date},
      {"COND_ID",INT_TYPE,sizeof(int),(OraField FieldSet::*)&OpRec::cond_id},
      {"COND_SUBID",INT_TYPE,sizeof(int),(OraField FieldSet::*)&OpRec::cond_subid},
      {"EDITED",INT_TYPE,sizeof(int),(OraField FieldSet::*)&OpRec::edited},
      {"RESULT",INT_TYPE,sizeof(int),(OraField FieldSet::*)&OpRec::result},
      {"SDATE",DATE_TYPE,0,(OraField FieldSet::*)&OpRec::sdate},
      {"EDATE",DATE_TYPE,0,(OraField FieldSet::*)&OpRec::edate},
      {"OLD_ID",INT_TYPE,sizeof(int),(OraField FieldSet::*)&OpRec::old_id},
      {"OLD_SUBID",INT_TYPE,sizeof(int),(OraField FieldSet::*)&OpRec::old_subid},
 };
 const int fldnum=sizeof(flds)/sizeof(flds[0]);
 lnk.fld=0;
 lnk.fldNum=-1;
 for(int i=0;i<fldnum;i++) if(flds[i].Set(lnk)) return;
}
//-----------------------------------------------------------------------------
void SumRec::MakeLink(FieldLink &lnk, int inp, int sn)
{
 static FieldIntDef flds[]={
      {"ROW_ID",STRING_TYPE,19,(OraField FieldSet::*)&SumRec::row_id },
      {"SUM_ID",INT_TYPE,sizeof(int),(OraField FieldSet::*)&SumRec::sum_id},
      {"VALUE",STRING_TYPE,25,(OraField FieldSet::*)&SumRec::value},
 };
 const int fldnum=sizeof(flds)/sizeof(flds[0]);
 lnk.fld=0;
 lnk.fldNum=-1;
 for(int i=0;i<fldnum;i++) if(flds[i].Set(lnk)) return;
}
//-----------------------------------------------------------------------------
void ApplyRec::MakeLink(FieldLink &lnk, int inp, int sn)
{
 static FieldIntDef flds[]={
      {"OP_ID",INT_TYPE,sizeof(int),(OraField FieldSet::*)&ApplyRec::op_id},
      {"OP_SUBID",INT_TYPE,sizeof(int),(OraField FieldSet::*)&ApplyRec::op_subid},
      {"RESULT",INT_TYPE,sizeof(int),(OraField FieldSet::*)&ApplyRec::result},
      {"ERRM",STRING_TYPE,0,(OraField FieldSet::*)&ApplyRec::errm},
 };
 const int fldnum=sizeof(flds)/sizeof(flds[0]);
 lnk.fld=0;
 lnk.fldNum=-1;
 for(int i=0;i<fldnum;i++) if(flds[i].Set(lnk)) return;
}
//-----------------------------------------------------------------------------
int GetObj(int objId,ObjRec &obj,cursor *pcrsr)
{
  cursor *crsr;
  if(!pcrsr) crsr=new cursor(conn,1200,20);
  else crsr=pcrsr;
  OraView ov(crsr,"*.OBJS");
  ov.MakeRecord(obj);
  obj.oid=objId;
  int r=ov.Do(" WHERE OID=:OID",obj,obj);
  if(!pcrsr) delete crsr;
  return r;
}
//-----------------------------------------------------------------------------
int ImpDb1(char* str, FILE* f, char* ObjName, bool NeedCatId, AnsiString Art, int Ver)
{
   ObjRec obj ;
   cursor crsr(conn) ;


   if (!GetObj(ObjName,obj,&crsr))
      THROW_AMS_EXC(AnsiString(nosuchdberr)+" "+ObjName) ;

   DBStructInfo DBS(&crsr,obj.oid,"",0);
   int rootCat=DBRootCat(DBS.dbid,&crsr);
   int RootCat_=rootCat;
   OraTable dt(&crsr,("*."+DBS.dbname).data());

   PosBuf drec,exdrec;
   dt.MakeRecord(drec);
   dt.MakeRecord(exdrec);

   char *s=strdelws(str);
   if ( !strncmpi(s,"[END]",5))
    return 1;

   AmsQuery AQ;
   if (NeedCatId)
   {
       AQ.SetSqlText(
            "select OCAT_HERE from *.CATS_MAP where DB_ID=:DB_ID and OCAT_THERE=:THERE");
       AQ.DeclVarInt("DB_ID", obj.oid);
       AQ.DeclVar("THERE", 0);
   }

   do
   {
    int field,stop;
    drec.SetNull();
    exdrec.SetNull();

    if (NeedCatId)
    {
        int Cat=GetInt(s,s);
        AQ.SetVar("THERE",Cat);
        AQ.Exec("Находим соответств категорию");
        if (AQ.NotEof())
            rootCat=AQ.GetFldInt(0);
        else
            rootCat=RootCat_;
    }
    char *s1;
    for(field=1,stop=0;*s && !stop;field++)
    {
     s1=s;
     for(;*s && *s!='\n' && *s!='|' && *s!=':';s++);
     if(!*s || *s=='|')  stop=1;
     if(*s) *s++=0;
     DBS.FI(field).InputFld(exdrec,strdelws(s1));
    }
    if (exdrec.prVer.isNull())
        exdrec.prVer=0;
    if (Art!="")
    {
        exdrec.article.Val()=Art.data();
        exdrec.prVer.Val()=Ver;
    }

    fgets(s=str,SLEN,f);
    if (AmsExp1251) My_OemToChar(str,str);
    s=strdelws(s);

    // импорт полей типа "Аналитическая сумма"
    if(!strncmpi(str,"[ASUM]",6))
    {
     char fname[80] ;
     strcpy(fname,strdelws(str+6)) ;

     int sum_id ;
     FileInputAnalSum1(crsr,sum_id,DBS.FI(fname).ref_dbId,"",str,f,6,NULL,NULL,fname);
     char buffer[50] ;
     itoa(sum_id,buffer,10) ;
     DBS.FI(fname).InputFld(exdrec,buffer);

     fgets(s=str,SLEN,f);
     if (AmsExp1251) My_OemToChar(str,str);
    }

    dt.Do(" WHERE article=:article and ver=:ver",exdrec);
    int found=dt.fetch(drec);

    try
    {
    if (!found)
    {
     char mes[80] ;
     if (G.ur.imp_opt&PROH_NEWART)
     {
      sprintf(mes,"%s %s %d",artnotexistmes,exdrec.article.val.data(),exdrec.prVer.val) ;
      throw OraError(0,mes);
     }
     else
     {
      sprintf(mes,"%s %s %d. %s",artnotexistmes,exdrec.article.val.data(),exdrec.prVer.val
             ,continuemes) ;
      if (!(G.ur.imp_opt&DONOT_ASK))
        if (AskMessage(mes, false)!=ID_YES)
        {
            return 0;
        }
     }
     exdrec.catId=rootCat;
     DBS.CalcDBLine(exdrec);
     dt.InsertRec(exdrec);
    }
    else if (!(G.ur.imp_opt & PROH_UPDATE))
    {
     // Updating database fields
     exdrec.catId=rootCat ;
     exdrec.row_id=drec.row_id ;
     // dt.UpdateRec_ID(exdrec);
     /*
     if (obj.hist)
     {
//      DBS.BeforeInsRec(exdrec,&crsr,1) ;

      AuxRec rec ;
      rec.int1=obj.oid ;
      rec.str1=drec.article ;
      rec.int2=drec.prVer ;
      rec.int3.SetNull() ;
      dt.DoPL("*.DBRevision(:int1,:str1,:int2,:int3);",rec) ;

      drec.catId=rec.int3 ;

      dt.OraDML::Do(DBS.InsertSTMT(1,1),drec);
     }
     */
     dt.OraDML::Do(DBS.UpdateSTMT(1,1),exdrec);
    }
    }
    catch(OraError ERR)
    {
     message((char*)(exdrec.article.val.data()),BAD) ;
     ERR.message();
    }

    {
     char t[80];
     sprintf(t,"%s%s",importingmes,exdrec.article.val.data());
     ShortMessage(t) ;
    }
    conn.commit() ;

   } while(s[0]!='[');
   return 0;
};


int FileInputAnalSum1(cursor &crsr,int &sumid,int anal_id,string setname,char *str,FILE *f,int skip_num,char *bal_fld,Sum *sum, char* fname)
{
   char *s ;
   AuxRec rec ;

   rec.int1=0 ;
   OraDML Q(&crsr) ;
   Q.DoPL("*.CreateSum(:INT1,0);",rec) ;


   sumid=rec.int1 ;

   DBStructInfo DBS(&crsr,anal_id,setname,0);
   int rootCat=DBRootCat(DBS.dbid,&crsr);

   OraView dv(&crsr,DBS.AnalView().data(),DBS.AnalHeader().data());
   OraTable totv(&crsr,string(string("*.")+DBS.analname).data());
   PosAnalBuf drec,dexr,totbuf,exdrec;

   dv.MakeRecord(drec);
   dv.MakeRecord(exdrec);
   totv.MakeRecord(totbuf);
   totbuf.InitTot(sumid);

   s=strdelws(str+skip_num);

   int prfld=0 ;
   if (!strncmpi(s,"[PRFLD=",7))
   {
   prfld=strtol(s+7,&s,10) ;
   s=strdelws(s+1);
   }

   fgets(s=str,SLEN,f);
   if (AmsExp1251) My_OemToChar(str,str);
   s=strdelws(s);

   while(s[0]!='[')
   {
   int field,stop,skip=0;
   drec.SetNull();

   // Database fields
   char *s1;
   for(field=1,stop=0;*s && !stop;field++)
   {
    s1=s;
    for(;*s && *s!='\n' && *s!='|' && *s!=':';s++);
    if(!*s || *s=='|')  stop=1;
    if(*s) *s++=0;
    DBS.FI(field).InputFld(exdrec,strdelws(s1));
   }

   string cnd=DBS.AnalCond(0,1,sumid,0);

   int sfld=atoi(key_str)+1 ;
   if (!strempty(key_str))
   {
    string fn=DBS.FI(sfld).RealName ;

    cnd+=" and "+fn+"=:"+fn+" ORDER BY TORD" ;
   }
   else if (G.ur.imp_opt&SUB_STRING)
   {
    char srch_str[80] ;
    strcpy(srch_str,exdrec.article.val.data()) ;
    rus_strupr(srch_str) ;

    cnd+=string(" and INSTR(UPPER(ARTICLE),'")+srch_str+"')>0";
   }
   else
   {
    cnd+=" and article=:article" ;
    cnd+=" and ver=:ver";
    if (exdrec.prVer.isNull())
        exdrec.prVer=0 ;
   }
   dv.Select(cnd,"",exdrec);
   int found=dv.fetch(drec);

   if (!strempty(key_str) && found==0)
   {
    char srch_str[80] ;
    DBS.FI(sfld).OutputFld(exdrec,srch_str,0);
    drec.article=srch_str ;
    drec.prVer=0 ;
    exdrec.article=srch_str ;
    exdrec.prVer=0 ;
   }

   if (!found)
   {
    char mes[80] ;
    if (G.ur.imp_opt&PROH_NEWART)
    {
     sprintf(mes,"%s %s %d",artnotexistmes,exdrec.article.val.data(),exdrec.prVer.val) ;
     throw OraError(0,mes);
    }
    else
    {
     sprintf(mes,"%s(%s) %s %d. %s",artnotexistmes,DBS.dbname.data(),exdrec.article.val.data(),exdrec.prVer.val
          ,continuemes) ;
     int res=1 ;
     if (!(G.ur.imp_opt&DONOT_ASK))
       res=AskMessage(mes) ;
     if (res==ID_YES)
        return 0;
     if (res==ID_NO)
       skip=1 ;
    }
    if (!skip)
    {
     exdrec.catId=rootCat;
     DBS.CalcDBLine(exdrec);
     try
     {
      dv.OraDML::Do(DBS.InsertSTMT(1),exdrec);
     }
     catch(OraError ERR)
     {

      ERR.message();
      return 0 ;
     }
     dv.Select(cnd,"",exdrec);
     dv.fetch(drec);
    }
   }
   else
   {
    // Updating database fields
   //       exdrec.catId=drec.catId ;
   //       dv.OraDML::Do(DBS.UpdateSTMT(1),exdrec);

    if (prfld)
    {
     char str1[256] , str2[256] ;
     DBS.FI(prfld).OutputFld(exdrec,str1,0);
     DBS.FI(prfld).OutputFld(drec,str2,0);
     if (strcmp(str1,str2))
     {
      exdrec.catId=drec.catId ;

      DBS.FI(prfld).InputFld(drec,str1);

      DBS.CalcDBLine(exdrec);

      while(1)
      {
       try
       {
        dv.OraDML::Do(DBS.InsertSTMT(1),exdrec);
        break ;
       }
       catch(OraError ERR)
       {
        if (ERR.err_code==1)
        {
         exdrec.prVer.val++ ;
        }
        else
        {
         ERR.message();
         return 0 ;
        }
       }
      }

      drec.prVer=exdrec.prVer ;
   //         dv.Select(cnd,"",exdrec);
   //         dv.fetch(drec);
     }
    }
   }


   drec.aarticle=drec.article;
   drec.aprVer=drec.prVer;
   drec.SumId=sumid ;

   for(field=DBS.dbfldNum+3;*s;field++)
   {
    s1=s;
    for(;*s && *s!='\n' && *s!='|' && *s!=':';s++);
    if(*s) *s++=0;


    // Поддержка ','
    enum VTYPE { VT_NULL=0,VT_DATE,VT_NUM,VT_INT,VT_ROWID,VT_STRING };
    if (DBS.FI(field).vtype==VT_NUM)
    {
     char *p=strchr(s1,',') ;
     if (p)
     *p='.' ;
    }

    DBS.FI(field).InputFld(drec,strdelws(s1));
   }

   if (!skip)
   {
    DBS.CalcAnLine(drec,totbuf);
    if(DBS.CheckRec(drec))
    {
     dv.OraDML::Do(DBS.InsertSTMT(2),drec);
     DBS.SumBuf(totbuf,drec,1);
    }
   }
   fgets(s=str,SLEN,f);
   if (AmsExp1251) My_OemToChar(str,str);
   s=strdelws(s);
   }

   totv.InsertRec(totbuf);

   if (bal_fld)
   {
    char sustr[30] ;
    DBS.FI(bal_fld).OutputFld(totbuf,sustr,0);
    sum->SetStr(sustr);
   }

 return 1 ;
}

int GetLinkDB(cursor *crsr,ObjRec &obj,OpRec or)
{
 if(!GetObj(or.doc_type,obj,crsr)) return 0 ;
 if(!obj.parent)
 {
  if(!or.cond_id) return 0 ;
  if(!GetObj(or.cond_id,obj,crsr)) return 0 ;
  if(!obj.parent) return 0 ;
 }
 if(!GetObj(obj.parent,obj,crsr)) return 0 ;
 return 1 ;
}

//--------------------------------------------------------
int GetLastVer(AnsiString dbname, AnsiString dbart)
{
       AmsQuery AQ;
       AQ.SetSqlText(
            "select max(VER) from *."+dbname+" where ARTICLE=:ART");
       AQ.DeclVarStr("ART", dbart);
       AQ.Exec("Ищем макс версию");
       return AQ.GetFldInt(0);
}

