#include "AMS_HDR.h"
#pragma hdrstop
#include "accs.h"
#include "analytic.h"

extern char doc_path[];
char drv_file[100]="" ;

int PostProc(char *in,char *out,int convert=0);

int split_file(char *in_file,char *out_file,int width,int l_width) ;
char *tempfile="temp";
char formoutfile[]="output.txt" , outfile[80]="lastform" ;
int sheet_width=129 , l_sheet_width=179 ;

extern StrList arg_list ;
extern int cur_arg ;
extern int def_point , def_numdig, def_zeros, def_apps ;

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
        Q.Do("ALTER ROLLBACK SEGMENT MYROLL ONLINE",or) ;
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
}

void RepRun()
{
  ObjRec or;
  if(ChooseObj(OT_FORM,&or,1)<0) return;
  ProgRun(or,0,0);
}

void DocRun(OraView *opv,OpRec &op)
{
  ObjRec or;
  if(GetObj(op.doc_type.val,or)<0) return;
  ProgRun(or,opv,&op);
}

void ChooseDocRun(OraView *opv,FieldSet &op,OBJ_Type otype)
{
  ObjRec or;
  if(ChooseObj(otype,&or,3)<0) return;
  ProgRun(or,opv,&op);
}

// Calculator

#define C_NUM   8
#define C_COPY  101
#define C_HIST  102
#define C_BUF   103

int     replace_string ;

extern  r_char  calcbutton , calctitle , exprprompt , resultprompt ,
                fromcalcmes , nothingtocopymes , nowheretopaste ,
                resultcopiedmes , nothingmes , replaceresprompt ;

char    expstr[255] , resultstr[30] ;

//ClipBoard       HList(nothingmes,"") ;

//EINFO   EI={expstr,0,0,0,60,0} ;
/*
int work_calculator(DLG *D,int call_par,long *ret_par)
{
 Sum sum ;
 char str[255] ;

 switch(call_par)
 {
  case D_OK :
     if (!strempty(expstr))
     {
        Field<string> res;
        {
         IP ip;
         ip.Enter();
         Prog prog;
         ip.Base.Structure().UpdateVar_Fld("__RES",&res);
         try
         {
          {
           ProgStr pf;
           pf.Init((string("__RES=STR(")+expstr+")").data());
           prog.Compile(&pf);
          }
          prog.Run(ip);
         }
         catch(OraError ERR)
         {
           ERR.message(0,0) ;
         }
         ip.Leave();
        }
        HList.Copy(expstr) ;
        if (replace_string)
        {
          char *p ;
          strcpy(expstr,res.val.data()) ;
          if ((p=strrchr(expstr,'-'))!=NULL && p!=expstr) *p='.' ;
          strblank(expstr) ;
          EI.pos=strlen(expstr) ;
          init_control(D,0) ;
       }
       strncpy(resultstr,res.val.data(),25) ;
     }
     else *resultstr=0 ;
     strstlen(resultstr,28) ;
     draw_static(D,3) ;
     break ;
  case C_COPY :
        if (strempty(resultstr)) { message(nothingtocopymes,BAD) ; break ; }
        SumBuf.CopyCom(resultstr,fromcalcmes) ;
        bottom_string.DrawStr(resultcopiedmes) ;
        pause(5) ;
        bottom_string.RemoveStr() ;
        break ;
  case C_HIST :
        if (HList.Paste(expstr)) init_control(D,0) ;
        break ;
  case C_BUF :
        if (!SumBuf.Paste(str)) break ;
        char *p ;
        if ((p=strchr(str,'-'))!=NULL) *p='.' ;
        strblank(str) ;
        int l=strlen(str) ;
        if (l>60-EI.len)
           { message(nowheretopaste,BAD) ; break ; }
        strcpy(expstr+EI.pos+l,expstr+EI.pos) ;
        memcpy(expstr+EI.pos,str,l) ;
        init_control(D,0) ;
        break ;
 }
 *ret_par=makelong(D_DEF,0) ;
 return D_CALL ;
}

void Calculator(char *dest_str)
{
 extern r_char  saveresbutton , bufbutton ;

 *expstr=0 ;
 *resultstr=0 ;

 HOTENTRY calc_hot_table[]={
         { alt_p , D_CALL , makelong(C_COPY,C_NUM) },
         { alt_b , D_CALL , makelong(C_BUF,C_NUM) } ,
         { key_ff2  , D_OK , 0 } ,
         { key_down , D_CALL , makelong(C_HIST,C_NUM) } ,
         { 0 , 0 , 0 } } ;

 CINFO  UI={&replace_string,replaceresprompt} ;

 CTRL _cl[]=
          {
           {2,3,62,3,&EI,D_CALL,makelong(D_OK,C_NUM),work_editp,init_editp},
           {46,6,70,6,&UI,D_CALL,makelong(D_DEF,0),work_check,init_check},
           {2,9,11,9,okbutton,D_OK,0,work_button,init_button},
           {14,9,24,9,calcbutton,D_CALL,makelong(D_OK,C_NUM),work_button,init_button},
           {27,9,47,9,saveresbutton,D_CALL,makelong(C_COPY,C_NUM),work_button,init_button},
           {50,9,59,9,bufbutton,D_CALL,makelong(C_BUF,C_NUM),work_button,init_button},
           {62,9,71,9,exitbutton,D_CANCEL,0L,work_button,init_button},
           {64,3,66,3,"  ",D_CALL,makelong(C_HIST,C_NUM),work_button,init_button},
           {0,0,0,0,NULL,0,0L,work_calculator,init_emp}
          } ;
 STAT _sl[]=
          {
           {2,2,exprprompt},
           {2,6,resultprompt},
           {13,5,(char *)makelong(30,2),ST_BOX},
           {15,6,resultstr},
          } ;
 DLG dlg={
          3,7,77,18,
          8,0,_cl,
          4,_sl,
          calctitle,
          0,NULL,0,NULL,NULL,
          calc_hot_table,
          0
         } ;

 int r=workdialog(&dlg) ;

 if (r && dest_str!=NULL)
 {
  strblank(resultstr) ;
  Sum s ;
  s.SetStr(resultstr) ;
  s.GetStr(dest_str) ;
//  strcpy(dest_str,resultstr) ;
 }
}

void Calculator()
{
 Calculator(NULL) ;
}
*/
