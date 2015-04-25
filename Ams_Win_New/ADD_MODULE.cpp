//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop
#include <process.h>
#include "ADD_MODULE.h"
#include "ams_win_c.h"
#include "FRM_EDIT_MEMO.h"
#include "FRM_CHOOSE_DB_POS.h"
#include "analytic.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

#include "amsclnt.h"
GlobalObjects G ;
connection conn;
char doc_path [100];



int CheckNode(void *node)
{
 if (node==NULL) return 0;
#ifndef _Windows
 if(heapchecknode(node)==_USEDENTRY) return 1;
 puts("Heap Error on free!");
 return 0;
#else
 return 1;
#endif
}



void My_OemToChar(char const *s,char *d)
{ OemToChar(s,d); }

void My_CharToOem(char const *s,char *d)
{ CharToOem(s,d); }

int strempty(char *str)
{
 if(!str) return 1;
 for( ; *str!='\n' && *str ; str++) if (*str!=' ' && *str!='\t') return 0 ;
 return 1 ;
}

void stralign(char *buf,char *str,int len,int type)
{
 int i , l=len-strlen(str) ;

 if (l<=0 || type==SA_LEFT)
 {
  strncpy(buf,str,len) ;
  strstlen(buf,len) ;
  return ;
 }

 switch(type)
 {
  case SA_RIGHT :
        for(i=0 ; i<l ; i++) buf[i]=' ' ;
        strcpy(buf+i,str) ;
        break ;
  case SA_CENTER :
        for(i=0 ; i<l/2 ; i++) buf[i]=' ' ;
        strcpy(buf+i,str) ;
        for(i=len-(l-l/2) ; i<len ; i++) buf[i]=' ' ;
        buf[len]=0 ;
        break ;
 }
}

void strstlen(char *str,int len)
{
 int i ;

 str[len]='\0' ;
 for(i=0 ; str[i]!='\0' ; i++) ;
 for( ; i<len ; i++) str[i]=' ' ;
}

void strncatchar(char *str,int n,int ch)
{
 int i , j ;
 for(i=0 ; str[i] ; i++) ;
 for(j=0 ; j<n ; j++ , i++) str[i]=ch ;
 str[i]=0 ;
}

char *strdelchar(char *str,int ch,char *buf)
{
 int i , j ;
 for(i=j=0 ; str[i] ; i++)
  if (str[i]!=ch) buf[j++]=str[i] ;
 buf[j]=0 ;
 return buf ;
}


int GetObj(const ::string &objName,ObjRec &obj,cursor *pcrsr)
{
  cursor *crsr;
  if(!pcrsr) crsr=new cursor(conn,1200,20);
  else crsr=pcrsr;
  OraView ov(crsr,"*.OBJS");
  ov.MakeRecord(obj);
  obj.name=objName;
  int r=ov.Do(" WHERE NAME=:NAME",obj,obj);
  if(!pcrsr) delete crsr;
  return r;
}

int GetDBLine(ObjRec &obj,PosBuf *rec,char *title,int shift,char *istr,const ::string &set,const ::string &QBE_string)
{
 TFrmChooseDbPos *Frm;
 Application ->CreateForm(__classid(TFrmChooseDbPos), &Frm);
 Frm->SetAddCond(ToAnsiStr(QBE_string));
 Frm->Begin(obj.oid.Val(), ToAnsiStr(obj.name.Val()), "Выбор позиции", set.data());
 AnsiString Art = "";
 int Ver = 0;
 int ret=Frm->SelectDbPos(Art, Ver, set.data());
 if (ret)
 {
    rec->article = ToString(Art);
    rec->prVer = Ver;
 }
 Frm->Release();
 return ret ;
}

int GetDBLine(int dbId,PosBuf *rec,char *title,int shift,char *istr,const ::string &set,const ::string &QBE_string)
{
   ObjRec obj;
   cursor crsr(conn);
   if(GetObj(dbId,obj,&crsr)>0)
    return GetDBLine(obj,rec,title,shift,istr,set,QBE_string);
   return 0;
}

void UserOptRec::MakeLink(FieldLink &lnk, int inp, int sn)
{
 static FieldIntDef flds[]={
      {"ROW_ID",STRING_TYPE,19,(OraField FieldSet::*)&UserOptRec::row_id },
      {"BEG_DATE",DATE_TYPE,0,(OraField FieldSet::*)&UserOptRec::beg_date },
      {"END_DATE",DATE_TYPE,0,(OraField FieldSet::*)&UserOptRec::end_date },
      {"USERNAME",STRING_TYPE,0,(OraField FieldSet::*)&UserOptRec::username},
      {"FULL_NAME",STRING_TYPE,0,(OraField FieldSet::*)&UserOptRec::full_name},
      {"SUM_LEN",INT_TYPE,sizeof(int),(OraField FieldSet::*)&UserOptRec::sum_len},
      {"SUM_DIG",INT_TYPE,sizeof(int),(OraField FieldSet::*)&UserOptRec::sum_dig},
      {"APPS",INT_TYPE,sizeof(int),(OraField FieldSet::*)&UserOptRec::apps},
      {"INP_DATE",DATE_TYPE,0,(OraField FieldSet::*)&UserOptRec::inp_date },
      {"CUR_DATES",INT_TYPE,sizeof(int),(OraField FieldSet::*)&UserOptRec::cur_dates},
      {"DIG_INPUT",INT_TYPE,sizeof(int),(OraField FieldSet::*)&UserOptRec::dig_input},
      {"SORT_NUM",INT_TYPE,sizeof(int),(OraField FieldSet::*)&UserOptRec::sort_num},
      {"DEAD_DATE",DATE_TYPE,0,(OraField FieldSet::*)&UserOptRec::dead_date },
      {"READONLY",INT_TYPE,sizeof(int),(OraField FieldSet::*)&UserOptRec::readonly},
      {"OVERWRITE",INT_TYPE,sizeof(int),(OraField FieldSet::*)&UserOptRec::overwrite},
      {"IMP_OPT",INT_TYPE,sizeof(int),(OraField FieldSet::*)&UserOptRec::imp_opt},
      {"SET_TO_END",INT_TYPE,sizeof(int),(OraField FieldSet::*)&UserOptRec::set_to_end},
 };
 const int fldnum=sizeof(flds)/sizeof(flds[0]);
 lnk.fld=0;
 lnk.fldNum=-1;
 for(int i=0;i<fldnum;i++) if(flds[i].Set(lnk)) return;
}

void BookNameRec::MakeLink(FieldLink &lnk, int inp, int sn)
{
 static FieldIntDef flds[]={
      {"USERNAME",STRING_TYPE,0,(OraField FieldSet::*)&BookNameRec::username},
      {"BOOKNAME",STRING_TYPE,0,(OraField FieldSet::*)&BookNameRec::bookname}
 };
 const int fldnum=sizeof(flds)/sizeof(flds[0]);
 lnk.fld=0;
 lnk.fldNum=-1;
 for(int i=0;i<fldnum;i++) if(flds[i].Set(lnk)) return;
}

void OpItemRec::MakeLink(FieldLink &lnk, int inp, int sn)
{
 static FieldIntDef flds[]={
      {"ROW_ID",STRING_TYPE,19,(OraField FieldSet::*)&OpItemRec::row_id },
      {"OP_ID",INT_TYPE,sizeof(int),(OraField FieldSet::*)&OpItemRec::op_id},
      {"OP_SUBID",INT_TYPE,sizeof(int),(OraField FieldSet::*)&OpItemRec::op_subid},
      {"OP_DATE",DATE_TYPE,0,(OraField FieldSet::*)&OpItemRec::op_date},
      {"CODE",INT_TYPE,sizeof(int),(OraField FieldSet::*)&OpItemRec::code},
      {"SIDE",STRING_TYPE,0,(OraField FieldSet::*)&OpItemRec::side},
      {"ITEM_SUM_ID",INT_TYPE,sizeof(int),(OraField FieldSet::*)&OpItemRec::item_sum_id},
      {"NODE",INT_TYPE,sizeof(int),(OraField FieldSet::*)&OpItemRec::node},
      {"BAL",INT_TYPE,sizeof(int),(OraField FieldSet::*)&OpItemRec::bal},
      {"FOR_POST",INT_TYPE,sizeof(int),(OraField FieldSet::*)&OpItemRec::for_post},
      {"VALUE",STRING_TYPE,25,(OraField FieldSet::*)&OpItemRec::value},
      {"NAME",STRING_TYPE,0,(OraField FieldSet::*)&OpItemRec::name},
      {"ANAL_ID",INT_TYPE,sizeof(int),(OraField FieldSet::*)&OpItemRec::anal_id},
      {"ANAL_INFO",STRING_TYPE,0,(OraField FieldSet::*)&OpItemRec::anal_info},
      {"SETNAME",STRING_TYPE,0,(OraField FieldSet::*)&OpItemRec::setname},
 };
 const int fldnum=sizeof(flds)/sizeof(flds[0]);
 lnk.fld=0;
 lnk.fldNum=-1;
 for(int i=0;i<fldnum;i++) if(flds[i].Set(lnk)) return;
}

int SelAcc(cursor &crsr,AccRec &cond)
{
 /*AccSelector B ;
 B.Init(crsr,cond) ;
 if (!B.Browse(selectacctitle)) return 0 ;
 cond.name=B.accr.name;
 cond.descr=B.accr.descr;
 cond.code=B.accr.code;
 return B.accr.code;*/
 return 0;
}

int DBCatTree(cursor &crsr,int start_cat,int dbid,char *title)
{
  /*
  DBCatTreeBrowser ctr_brws;
  ctr_brws.Init(crsr,1,dbid);
  return ctr_brws.Browse(start_cat,title);
  */
  return 0;
}

#include "analytic.h"
int DBStructInfo::SelectField(char *tit,int &fnum,int extra,int _skip,int flags)
{
    return 0;
}

int  cf_dbid , cf_analid ;
::string cf_article ;
int  cf_ver ;
OpVars *opvars ;
OpVars *accvars;
char connect_str[80] , *connect_ptr=NULL ;

int EditAnalSum(AccRec *ar,OraView *accview,char *head,cursor *crsr,ObjRec &or,int sum_id,int rem_id,int flags,Field<Sum> *synt_sum,int readonly,int shift,::string set,int checksub,PosAnalBuf *drec,int emode,int omode)
{
    return 0;
}

long HeapCheck(char *m)
{ return 0;}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
extern int def_point,
    def_numdig,
    def_zeros,
    def_apps ;

extern int dbsnum, dbcnum ;

char username[40] , userpass[40] ;
char ExcelType[20]={0 };

StrList arg_list ;
int cur_arg ;
int To1251=1;  // Необходимо преобразование файлов-программ в 1251



enum { VS=1,VD,HS,HD,VHS,VHD,VHUS,VHUD,VHDS,VHDD,VHLAST };

static void xlat(char *a)
{
 static char *t,*pat="_¦-=",*altpat="|!-=";
 for(;*a;a++)
  if(*a=='\\')
  {
   t=strchr(pat,a[1]);
   if(t) *a=t-pat+VS;
   else
   { t=strchr(altpat,a[1]);
     if(t) *a=t-altpat+VS;
     else continue;
   }
   strcpy(a+1,a+2);
  }
 *(a-1)=0;
};

static void unxlat(char *a)
{
 static char boxch[]="жж-=+++жTT";
 for(;*a;a++)
  if(*a>=VS && *a<VHLAST) *a=boxch[*a-VS];
 *a++='\n'; *a=0;
};

static void cornlat(char *a)
{
  static char boxch[]  = "+++жTT";
  static char corn[][7] ={"+жLL-у",
             "+ж--мм"};
  char *t;
  for(int i=0;i<2;i++)
  {
   if((t=strpbrk(a,boxch))!=0)
   {
    *t=corn[i][int(strchr(boxch,*t)-boxch)];
   }
   strrev(a);
  }
};

void ShortMessage (const AnsiString& Msg)
{
    AmsWinC->SDisplay(3,Msg);
    AmsWinC->Refresh();
};

#define MAXSTR  2048
#define MAXCOLUMN 10

int Skip2Column(FILE *in,char *str,long *apos,int *alines)
{
 int i,col=0;
 *apos=ftell(in);
 for(i=0;!feof(in);i++)
 {
  if (!fgets(str,MAXSTR,in)) break ;
  if (str[0]=='\\' && (str[1]=='C'|| str[1]=='E'))
  {
   alines[col++]=i;
   apos[col]=ftell(in);
   i=-1;
   if(str[1]=='E')   break;
  }
 }
 if(!col)
 {
  alines[0]=i;
  apos[1]=ftell(in);
  col=1;
 }
 return col;
};

void OutputForm(char *title,char *formfile,unsigned flags)
{
 if (flags&DTO_FOREXCEL)
 {
  if (ExcelType[0])
   spawnl(P_WAIT,"ams2xls","ams2xls",formfile,ExcelType,NULL) ;
  else
   spawnl(P_WAIT,"ams2xls","ams2xls",formfile,NULL) ;
  return ;
 }
};


int PostProc(char *in,char *out,int convert)
{
 FILE *inf=fopen(in,"rt"),*outf=fopen("_temp","wt");
 if(!inf || !outf) return 1;
 char str[MAXSTR+1],vstr[MAXSTR+1];
 int i,intab=0,vtab=0,htab,hsave;

 long cpos[MAXCOLUMN];
 int lines[MAXCOLUMN],col;

 while(!feof(inf)) // column merging
 {
  if (!fgets(str,MAXSTR,inf)) break ;
  if (str[0]=='\\' && str[1]=='C')
  {
   col=Skip2Column(inf,str,cpos,lines);
   int line,emp=0;
   for(line=0;!emp;line++)
   {
    emp=1;
    str[0]='\n';
    str[1]=0;
    for(i=0;i<col;i++)
    {
     if(line<lines[i])
     {
      emp=0;
      fseek(inf,cpos[i],SEEK_SET);
      fgets(str+strlen(str)-1,MAXSTR-strlen(str),inf);
      cpos[i]=ftell(inf);
     }
    }
    if(!emp) fputs(str,outf);
   }
   fseek(inf,cpos[col],SEEK_SET);
  }
  else
  {
   void My_OemToChar(char const *s,char *d) ;
   if (convert)
      My_OemToChar(str,str) ;
   fputs(str,outf);
  }
 }

 outf=freopen(out,"wt",outf);
 inf=freopen("_temp","rt",inf);

 while(!feof(inf)) // box-drawing
 {
  if (!fgets(str,MAXSTR,inf)) break ;
  xlat(str);

  if(str[0]==HS || str[0]==HD)
  { hsave=str[0];
    if(!intab)
    {
     intab=1; vtab=1; strcpy(vstr,str);
     continue;
    }
    else
    {
     if(vtab) // close table
     {
      for(i=0;vstr[i];i++)
       switch(vstr[i]) {
        case VHS: vstr[i]=VHUS; break;
          case VHD: vstr[i]=VHUD; break;
       }
      vtab=0; intab=0;
     }
     else
     {
      char c=str[0];
      for(i=0;vstr[i];i++)
    str[i]=(vstr[i]==VS)?VHUS:(vstr[i]==VD)?VHUD:c;
      str[i]=0;
      strcpy(vstr,str);
      vtab=1;
     }
     continue;
    }
  }
  htab=0;
  for(i=0;str[i];i++)
     if(str[i]==VS || str[i]==VD)
    if(htab) { strcpy(str+i-1,str+i); i--; }
    else htab=1;
      else htab=0;
   if(vtab)
   {
    for(i=0;vstr[i];i++)
       switch(str[i]) {
       case VS: vstr[i]=vstr[i]==VHUS?VHS:vstr[i]==HS?VHDS:vstr[i]; break;
       case VD: vstr[i]=vstr[i]==VHUD?VHD:vstr[i]==HD?VHDD:vstr[i]; break;
       }
      for(;str[i];i++)
       vstr[i]=(str[i]==VS)?VHDS:(str[i]==VD)?VHDD:hsave;
      vstr[i]=0;
      unxlat(vstr);
      cornlat(vstr);
      fputs(vstr,outf);
      vtab=0;
   }
   strcpy(vstr,str);
   unxlat(str);
//   ParsePrintCodes(str);
   fputs(str,outf);
 }
 fclose(inf);
 fclose(outf);
 return 0;
} ;



void ProgRun(ObjRec &or,OraView *view,FieldSet *fs,char *output)
{
  int runed=0;
  {
   IP ip;
   ip.Enter();

   if (!output)
   {
    arg_list.Free() ;
    cur_arg=-1 ;
   }

   Prog prog , *oldptr=prog_ptr ;
   prog_ptr=&prog ;

   prog.OpenOut(output ? "alttemp" : "temp");
   ProgFile pf;
   try
   {
      int ok ;
      {
       char fname[256];
       sprintf(fname,"%s\\%s.prg",doc_path,or.name.val.data());
       pf.Init(fname);
       ok=prog.Compile(&pf);
       prog.Link(doc_path);
      }
      if(view && fs) ip.Base.MakeOraRec(*view,*fs,1);
      runed=1;

      def_point='-' ,
      def_numdig=2 ,
      def_zeros=0,
      def_apps=apostrophe ;
      dbsnum=0 ;
      dbcnum=0 ;

      if (ok)
      {
       try
       {
        cursor crsr(conn) ;
        OraDML Q(&crsr) ;
        //Q.Do("ALTER ROLLBACK SEGMENT MYROLL ONLINE",or) ;
       } catch(...) {}
       prog.Run(ip);
      }
   }
   catch(OraError ERR)
   {
       ERR.message(0,0,"After prog error");
   }
   conn.commit() ;
   prog.CloseOut();
   ip.Leave();
   prog_ptr=oldptr ;
  }
  if(runed)
  {
   PostProc(output ? "alttemp" : "temp",output ? output : "temp1",or.popts.val&DTO_FOREXCEL);
   HeapCheck("before file edit");
   if (!output)
     OutputForm((char*)(or.descr.val.data()),"temp1",or.popts.val&DTO_PMASK);
  }
} ;

void RepRun()
{
  ObjRec or;
  if(ChooseObj(OT_FORM,&or,1)<0) return;
  ProgRun(or,0,0);
} ;

void DocRun(OraView *opv,OpRec &op)
{
  ObjRec or;
  if(GetObj(op.doc_type.val,or)<0) return;
  ProgRun(or,opv,&op);
} ;

void ChooseDocRun(OraView *opv,FieldSet &op,OBJ_Type otype)
{
  ObjRec or;
  if(ChooseObj(otype,&or,3)<0) return;
  ProgRun(or,opv,&op);
} ;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void BeginExecReport(const AnsiString& FormName, TGenLocalVars* LV)
{
    int POPT = 0;
    do {
        AmsQuery AQ;
        AQ.SetSqlTextReady("select POPTS from "+BaseUserName+".OBJS "
            "where NAME = :NAME");
        AQ.DeclVar("NAME", FormName);
        AQ.Exec("");
        if (!AQ()->Eof)
        {
            POPT = AQ()->FieldAsInteger(0);
        }
    }
    while (0);


    IP ip;
    if (LV) ip.SetVars(LV);
    ip.Enter();

    Prog doc_prog, *oldptr=prog_ptr ;
    prog_ptr=&doc_prog;

    extern int cur_arg;
    int pre_cur_arg=cur_arg ;
    cur_arg=-1 ;
    extern char DirBuff [200];
    string ResFile=string(DirBuff)+"\\temp";

    try {
        doc_prog.OpenOut(ResFile.data());
        ProgFile pf;
        char fname[256];
        int ok =0 ;
        sprintf(fname,"%s\\%s.prg",GC::DocPath.c_str(),FormName.c_str());
        pf.Init(fname);
        ok=doc_prog.Compile(&pf);
        doc_prog.Link(GC::DocPath.c_str());
        pf.Close();
        //runed=1;

        def_point='-' ,
        def_numdig=2 ,
        def_zeros=0,
        def_apps=apostrophe ;
        dbsnum=0 ;
        dbcnum=0 ;

        try {
            if (ok)
                doc_prog.Run(ip);
        }
        catch(OraError ERR)
        {
            ERR.message(0,0,"Ошибка");
        }
    }
    catch (OraError ERR)
    {
        ERR.message(0,0,"After prog error");
    }

    doc_prog.CloseOut();
    ip.Leave();
    cur_arg=pre_cur_arg ;
    prog_ptr=oldptr;

    if ( POPT & 64)
    {
        char SysStr [250];
        SysStr[0]=0;
        strcat(SysStr, ResFile.c_str());
        strcat(SysStr, " ");
        strcat(SysStr, GC::ExcelType.c_str());
        //system(SysStr);
        ShellExecute(AmsWinC->Handle ,	"open",	"ams2xls.exe",
            SysStr,DirBuff ,SW_HIDE);
    }
    else
    {
        if (doc_prog.GetResFileName()!="")
        {
            char Comm [2000];
            string Command="del "+doc_prog.GetResFileName();
            strcpy(Comm, Command.data());
            system(Comm);
            Command=string("move ")+ResFile+" "+doc_prog.GetResFileName();
            strcpy(Comm, Command.data());
            system(Comm);
            ResFile=doc_prog.GetResFileName();

//---------------------------CH Encoding
          string Sjkl=doc_prog.GetEncodingName();
          if (doc_prog.GetEncodingName()=="DOS") {
              TStringList* M=new TStringList;
              try {
                PChar N;
                M->LoadFromFile(ResFile.data());
                N = M->GetText();
                AnsiToOem(N, N);
                M->Text = StrPas(N);
                M->SaveToFile(ResFile.data());
              }__finally {
                delete M;
              }
          }
//-------------------------EO CH Encoding
        }
        if (doc_prog.GetImpFileName()!="")
        {
            extern void TextOpImport2(const char* FName);
            TextOpImport2(doc_prog.GetImpFileName().data());
        }
        else
        {
            TFrmEditMemo * Frm;
            Application->CreateForm(__classid(TFrmEditMemo), &Frm);
            Frm->ShowFile(ResFile.data(),"Результаты выполнения формы "+ FormName);
        }
    };
};
//---------------------------------------------------------------------------
TDate GetInpDate()
{
    if (G.ur.inp_date.isNull())
    {
        AmsQuery AQ;
        AQ.SetSqlText("select *.GetInpDate() from DUAL");
        AQ.Exec("Дата по умолчанию");
        if (AQ.NotEof())
            G.ur.inp_date=AQ.GetFldDate(0);
    }
    return int (G.ur.inp_date.Val().T());
};
//---------------------------------------------------------------------------

