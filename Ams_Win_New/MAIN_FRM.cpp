//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "MAIN_FRM.h"
#include <process.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainFrm *MainFrm;
//---------------------------------------------------------------------------
__fastcall TMainFrm::TMainFrm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
extern int def_point,
    def_numdig,
    def_zeros,
    def_apps ;

extern int dbsnum, dbcnum ;

char username[40]="Vlad" , userpass[40]="asdzxc" ;
char ExcelType[20]={0 };

StrList arg_list ;
int cur_arg ;
char doc_path[64]="W:\\CONVERT\\NEWCOM";
int To1251=1;  // Íåîáõîäèìî ïðåîáðàçîâàíèå ôàéëîâ-ïðîãðàìì â 1251



enum { VS=1,VD,HS,HD,VHS,VHD,VHUS,VHUD,VHDS,VHDD,VHLAST };

static void xlat(char *a)
{
 static char *t,*pat="_ºÄÍ",*altpat="|!-=";
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
 static char boxch[]="¦¦-=+++¦TT";
 for(;*a;a++)
  if(*a>=VS && *a<VHLAST) *a=boxch[*a-VS];
 *a++='\n'; *a=0;
};

static void cornlat(char *a)
{
  static char boxch[]  = "+++¦TT";
  static char corn[][7] ={"+¦LL-ã",
             "+¦--¬¬"};
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
    MainFrm->SB->SimpleText = Msg;
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
void __fastcall TMainFrm::Button1Click(TObject *Sender)
{
    ObjRec OR;
    OR.name.Val()= "TEST";
    ProgRun(OR,0,0,0);
};
//---------------------------------------------------------------------------

extern char KbPressed;

void __fastcall TMainFrm::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
//
    if (Key ==27)
    {
        KbPressed = 27;
    }
}
//---------------------------------------------------------------------------

