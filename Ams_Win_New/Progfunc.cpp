#include "AMS_HDR.h"
#pragma hdrstop
#include "FLDS_DESCR_DISTR.h"
#include "accs.h"
#include "analytic.h"
#include "..\fisc\fiscprn.h"
#include "..\fisc\fisc_msg.h"
#include "analytic.h"
#include "FRM_BROWS_AN.h"
#include "FRM_MSG.h"

#define GOOD 0
#define BAD  1

#define INIT 1
#define NEW  0


//-------------------------------------
// Переопределения типов для функций из fisc.dll
//--------------------------------------

/*typedef int (__stdcall *SetComPort)(int Numb, int Speed=4800);

typedef int (__stdcall *CloseComPort)(int Numb);
typedef int (__stdcall *ResetCashes)(void);
typedef int (__stdcall *IsCash)(int NumCash);
typedef int (__stdcall *StrPrntCash)(int NumCash,char* str1,
                                       char * str2=NULL,
                                       char * str3=NULL,
                                       char * str4=NULL,
                                       char * str5=NULL);
typedef int (__stdcall *SellArtCash)(int NumCash,char * Art);
typedef int (__stdcall *GetMode)(int NumCash);
typedef int (__stdcall *SetMode)(int NumCash, int Mode);
typedef int (__stdcall *CloseReceipt)(int Numb);
typedef char (__stdcall *GetVerDLL)();
typedef int (__stdcall *GetLicence)();  */


//-------------------------------------
// Обычно будем работать с одной кассой
//--------------------------------------
struct SCurrCassaInfo
{

    int CassaNo;
    int ComPort;
    int Speed;
    SCurrCassaInfo():CassaNo(1), ComPort(1), Speed(4800){};
};

SCurrCassaInfo CurrCassa;
//--------------------------------------


//#include "orabrws.h"
//#include "editdb.h"

extern r_char badtypeerr,varnameexpectederr,
              unknownfuncerr,outofrangeerr,userbreakmes,baddatemes,
              uknownobjecterr,nosuchvarerr ;
extern char Name_cnt[],Name_kop[];
extern char *Names_kop[][3];
extern int apostrophe ;

int def_point='-' ,
    def_numdig=2 ,
    def_zeros=0,
    def_apps=apostrophe ;

extern StrList arg_list ;
extern int cur_arg ;

void InputField(OraField &fld,const char *prompt,int size=0)
{
 //int bufsize=fld.TSize(size) ;
 char buf[250] ;
 *buf=0 ;
 int mode=NEW;
 if(!fld.isNull())
 {
  fld.Output(buf);
  mode=INIT;
 }

 if (cur_arg==-1)
 {
  //if (!getstring(bufsize,(char*)prompt,buf,mode)) throw OraError(100,userbreakmes) ;
 }
 else
 {
  if (cur_arg<arg_list.GetNum())
  {
   strcpy(buf,arg_list.GetStr(cur_arg++)) ;
  }
 }

 fld.Input(buf) ;
}



StackItem BaseNode::Method(::string const &method,int pc,StackItem *stk)
{
  long double v,v1;
  int r;
  if(method=="ABS" && pc==1)
  {
   v=stk[0].Number();
   return VAL(fabsl(v));
  }
  if(method=="SET_DIGITS" && pc==1)
  {
   def_numdig=stk[0].Number();
   return VAL(def_numdig);
  }
  if(method=="SET_ZEROS" && pc==1)
  {
   def_zeros=stk[0].Number();
   return VAL(def_zeros);
  }
  if(method=="SET_POINT" && pc==1)
  {
   def_point=stk[0].String().data()[0];
   return VAL(def_point);
  }
  if(method=="APPS_OFF" && pc==0)
  {
   def_apps=0 ;
   return VAL(0);
  }
  if(method=="APPS_ON" && pc==0)
  {
   def_apps=1 ;
   return VAL(0);
  }
  if(method=="SIGN" && pc==1)
  {
   v=stk[0].Number();
   int r;
   if(v<0) r=-1;
   else if(v==0) r=0;
   else if(v>0) r=1;
   return VAL(r);
  }
  if(method=="ROUND" && (pc==1 || pc==2))
  {
   r=0;
   int sign=1 ;
   v=stk[1-pc].Number();
   if (v<0)
   {
    v=-v ;
    sign=-1 ;
   }
   if(pc==2) r=stk[0].Number();

   double RoundLD(long double v,int r) ;
   v=RoundLD(v,r) ;

/*
   long double p10,vp10;
   p10=pow10l(r);
   vp10=v*p10;
   if(fmodl(vp10,1)>=.5) v=ceill(vp10)/p10;
   else v=floorl(vp10)/p10;
*/
   return VAL(v*sign);
  }
  if(method=="FLOOR" && (pc==1 || pc==2))
  {
   r=0;
   v=stk[1-pc].Number();
   if(pc==2) r=stk[0].Number();
   long double p10;
   p10=pow10l(r);
   v=floorl(v*p10)/p10;
   return VAL(v);
  }
  if(method=="CEIL" && (pc==1 || pc==2))
  {
   r=0;
   v=stk[1-pc].Number();
   if(pc==2) r=stk[0].Number();
   long double p10;
   p10=pow10l(r);
   v=ceill(v*p10)/p10;
   return VAL(v);
  }
  if(method=="POS" && pc==1)
  {
   v=stk[0].Number();
   if(v<0) v=0;
   return VAL(v);
  }
  if(method=="NEG" && pc==1)
  {
   v=stk[0].Number();
   if(v>0) v=0;
   return VAL(-v);
  }
  if(method=="MAX" && pc==2)
  {
   v=stk[0].Number();
   v1=stk[-1].Number();
   if(v<v1) v=v1;
   return VAL(v);
  }
  if(method=="MIN" && pc==2)
  {
   v=stk[0].Number();
   v1=stk[-1].Number();
   if(v>v1) v=v1;
   return VAL(v);
  }
  if(method=="DATE")
  {
   DateTime dt;
   if(pc==1 && stk[0].vtype()==gcl::VT_STRING)
   {
    if (!dt.SetStr(stk[0].String().data()))
     prog_ptr->RTError(110,baddatemes) ;
    return VAL(dt);
   }
   if(pc==3)
   {
        int D=stk[1-pc].Number();
        int M=stk[2-pc].Number();
        int Y=stk[3-pc].Number();
        dt.Set(D,M,Y);
        return VAL(dt);
   }

   AmsQuery AQ;
   AQ.SetSqlTextReady ("begin select SYSDATE into :CURR_DATE from DUAL; end;");
   AQ.DeclVarDate("CURR_DATE");
   AQ.Exec("Текущее время");
   TDateTime dd=AQ.GetVarDate("CURR_DATE");
   dd= int(dd);
   dt=dd;
   return StackItem(VAL(dt));
  }
  if(method=="INT" && pc==1)
  {
   if(stk[0].vtype()==gcl::VT_STRING) v=_strtold(stk[0].String().data(),0);
   else v=stk[0].Number();
   return StackItem(VAL((int)v));
  }
  if(method=="NUM" && pc==1)
  {
   if(stk[0].vtype()==gcl::VT_STRING) v=_strtold(stk[0].String().data(),0);
   else v=stk[0].Number();
   return StackItem(VAL(v));
  }
  if(method=="LINE" && pc==2)
  {
   ::string s=stk[-1].String() ;
   char buf[280]; buf[0]=0;
   strncatchar(buf,stk[0].Number(),s.data()[0]) ;
   return StackItem(VAL((const ::string )buf)) ;
  }
  if (method=="CONST" && pc==1)
  {
   AmsQuery AQ;
   AQ.SetSqlText("begin *.GetConst(:name,:value,:const_type);end;");
   AnsiString ConstName = stk[0].String().data();
   AQ.DeclVarInt("CONST_TYPE");
   AQ.DeclVar("NAME", ConstName.UpperCase());
   AQ.DeclVarStr("VALUE");
   AQ.Exec("Получаем константу");
   int ConstType = AQ.GetVarInt("CONST_TYPE");
   AnsiString Val =AQ.GetVarStr("VALUE");
   switch(ConstType)
   {
    case gcl::VT_NUM :
    {
     Sum s ;
     s.SetStr((char *)Val.c_str()) ;
     return StackItem(VAL(s)) ;
    }
    case gcl::VT_INT : return VAL(atoi(Val.c_str())) ;
    case gcl::VT_DATE :
         {
            DateTime dt;
            dt.SetStr(Val.c_str());
            return StackItem(VAL(dt));
         }
    case gcl::VT_STRING: return StackItem(VAL(Val.c_str())) ;
    default : return StackItem(VAL()) ;
   }
  }
  if (method=="GETKURS" && pc==2)
  {
   AmsQuery AQ;
   AQ.SetPLText("*.GetKurs(:str1,:date1,:sum1);");
   AQ.DeclVar("STR1",AnsiString(stk[-1].String().data())) ;
   AQ.DeclVarDate("DATE1",(int)stk[0].GetDate().T());
   AQ.DeclVarSum("SUM1");
   AQ.Exec("Выбор курсов");
   return StackItem(VAL(AQ.GetVarSum("SUM1"))) ;
  }


//-------------------------------------CH RECALC
  /*
    if (method=="RECALC_ACC" && pc==3)
      {
       AmsQuery AQ;
       AQ.SetPLText("*.bookkern.RecalcAccount(:str1,:date1,:date2);");
       AQ.DeclVar("STR1", stk[-2].String().data());
       AQ.DeclVar("DATE1", stk[-1].GetDate().T());
       AQ.DeclVar("DATE2", stk[0].GetDate().T()) ;
       AQ.Exec("Пересчет счета");
       return StackItem() ;
      }
  */

   if (((method=="RECALC_ACC") || (method=="RECALC_ONE") || (method=="RECALC_COR")) && pc==3)
   {
      AmsQuery AQ;

      try {
        //AQ.SetPLText("ALTER ROLLBACK SEGMENT MYROLL ONLINE") ;
        //AQ.Exec("MYROll Online");
      } catch(...) {}

      if (method=="RECALC_ACC")
        AQ.SetPLText("*.bookkern.RecalcAccount(:str1,:date1,:date2);") ;
      else if (method=="RECALC_ONE")
        AQ.SetPLText("*.bookkern.RecalcOneAccount(:str1,:date1,:date2);") ;
      else if (method=="RECALC_COR")
        AQ.SetPLText("*.bookkern.RecalcOneAccountCor(:str1,:date1,:date2);") ;

      AQ.DeclVar("STR1", stk[-2].String().data());
      AQ.DeclVar("DATE1", stk[-1].GetDate().T());
      AQ.DeclVar("DATE2", stk[0].GetDate().T()) ;

      AQ.Exec("Пересчет счета");
      return StackItem() ;

   }




   if (method=="SHELLEXECUTE" && pc==1)
   {
      try {
        ShellExecute(Application->Handle,"open", stk[0].String().data() ,NULL,NULL,0);
      } catch(...) {}  
      return StackItem() ;
   }

   if (method=="RECALC_BEG" && pc==1)
   {
      AmsQuery AQ;

      try {
       //AQ.SetPLText("ALTER ROLLBACK SEGMENT MYROLL ONLINE") ;
       //AQ.Exec("MYROll Online");
      } catch(...) {}

      AQ.SetPLText("*.bookkern.RecalcBegin(:str1);") ;
      AQ.DeclVar("STR1", stk[0].String().data()) ;
      AQ.Exec("Пересчет счета");

      return StackItem() ;
   }

//------------------------------------EO CH RECALC



  if (method=="RUN_FORM" && pc>=2)
  {
   ObjRec or ;

   if (!GetObj(stk[-pc+1].String(),or))
    prog_ptr->RTError(111,uknownobjecterr) ;

   arg_list.Free() ;
   cur_arg=0 ;
   arg_list.Alloc(pc-2) ;

   for(int i=0 ; i<pc-2 ; i++)
     arg_list.AddStr((char*)stk[-pc+3+i].String().data()) ;

   ProgRun(or,NULL,NULL,(char*)stk[-pc+2].String().data()) ;

   arg_list.Free() ;
   cur_arg=0 ;

   return VAL(0) ;
  }
  // Сообщение с двумя кнопками
  if (method=="ASKYESNO" && pc>=1)
  {
   int integ=1;
   if(pc>1) integ=stk[2-pc].Number();
   int Res = AskMessage(stk[1-pc].String().data(), !integ) ;
   Res =  (Res==ID_YES);
   return VAL(Res) ;
  }
  if (method=="ASKINT" && pc>=1)
  {
   int integ=0;
   if(pc>1) integ=stk[2-pc].Number();
   int Res = AskInt(stk[1-pc].String().data(), integ) ;
   if (!Res)
   {
        throw OraError(100,userbreakmes) ;
   };
   return VAL(integ) ;                       
  }
  if (method=="ASKSUM" && pc>=1)
  {
   Sum sum =0;
   if(pc>1) sum=stk[2-pc].Number();
   double S= sum;
   int Res = AskSum(stk[1-pc].String().data(), S) ;
   if (!Res)
   {
        throw OraError(100,userbreakmes) ;
   };
   sum=S;
   return VAL(sum) ;
  }
  if (method=="ASKSTRING" && pc>=2)
  {
   Field<string> s ;
   if(pc>2) s=stk[3-pc].String();
   AnsiString DefVal= ToAnsiStr(s.Val());
   int Res = AskString(stk[1-pc].String().data(),(int)(stk[2-pc].Number()), DefVal) ;

   if (!Res)
   {
        throw OraError(100,userbreakmes) ;
   }
   return VAL(ToString(DefVal)) ;
  }
  if (method=="ASKDATE" && pc>=1)
  {
   Field<DateTime> d ;
   if(pc>1) d=stk[2-pc].GetDate();
   TDateTime Dt = d.Val().T();

   int Res =AskDate(stk[1-pc].String().data(), Dt);

   if (!Res)
   {
        throw OraError(100,userbreakmes) ;
   }
   d.Val()=Dt;
   return VAL(d) ;
  }

  if(method=="STR" && (pc==1 || pc==2))
  {
   char const *fmt;
   char buf[280]; buf[0]=0;
   if(stk[1-pc].vtype()==gcl::VT_INT)
   {
     r=stk[1-pc].Number();
     if(pc==2) fmt=stk[0].String().data();
     else fmt="%d";
     sprintf(buf,fmt,r);
   }
   else  if(stk[1-pc].vtype()==gcl::VT_NUM)
   {
     v=stk[1-pc].Number();
     if(pc==2) fmt=stk[0].String().data();
     else fmt="%Lf";
     sprintf(buf,fmt,v);
   }
   else if(stk[1-pc].vtype()==gcl::VT_STRING)
   {
     char const *st=stk[1-pc].String().data();
     if(pc==2) fmt=stk[0].String().data();
     else fmt="%s";
     sprintf(buf,fmt,st);
   }
   else if(stk[1-pc].vtype()==gcl::VT_DATE)
   {
     DateTime dt=stk[1-pc].GetDate() ;
     if(pc==2) fmt=stk[0].String().data();
     else fmt="%d-%m-%y";
     dt.Out(buf,50,(char*)fmt);
   }
   return VAL((const ::string )buf);
  }
  if(method=="WAIT" || method=="PRINT")
  {
   ::string mes;
   char t[40];
   for(int i=-pc+1;i<=0;i++)
   {
    StackItem &v=stk[i];
    if(v.vtype()==gcl::VT_STRING) mes+=v.String()+" ";
    if(v.vtype()==gcl::VT_NUM) { sprintf(t,"%Lf ",v.Number()); mes+=t; }
    if(v.vtype()==gcl::VT_INT) { sprintf(t,"%d ",(int)v.Number()); mes+=t; }
    if(v.vtype()==gcl::VT_DATE) { v.GetDate().Out(t,40); mes+=t; }
   }
   if(method=="WAIT") Message((char *)mes.data());
   else ShortMessage((char *)mes.data());
   return StackItem();
  }
  // Выдаем сообщение различными шрифтами
  if(method=="MESSAGE" && pc>=1)
  {
   ::string mes;
   char t[40];
   AnsiString Format;
   int i=-pc+1;
   TFrmMsg* Frm;
   Application->CreateForm(__classid(TFrmMsg), (&Frm));
   Frm->Caption=stk[i++].String().data();

   int Height=0;

   while(i<=0)
   {
        mes="";
        StackItem &v=stk[i];
        if(v.vtype()==gcl::VT_STRING) mes=v.String();
        else if(v.vtype()==gcl::VT_NUM) { sprintf(t,"%Lf ",v.Number()); mes=t; }
        else if(v.vtype()==gcl::VT_INT) { sprintf(t,"%d ",(int)v.Number()); mes=t; }
        else if(v.vtype()==gcl::VT_DATE) { v.GetDate().Out(t,40); mes=t; }

        Format="L10";
        i++;
        if (i<=0)
        {
            StackItem &v1=stk[i];
            if(v1.vtype()==gcl::VT_STRING) Format=v1.String().data();
        }
        TLabel* Lab=new TLabel (Frm);
        Lab->Parent=Frm;
        Lab->Top=Height+8;
        Lab->Align=alTop;
        Lab->WordWrap=true;
        if (Format.Pos("C")||Format.Pos("Ц")) Lab->Alignment=taCenter;
        if (Format.Pos("R")||Format.Pos("П")) Lab->Alignment=taRightJustify;

        if (Format.Pos("B")||Format.Pos("Ж"))
            Lab->Font->Style=Lab->Font->Style<<fsBold;
        if (Format.Pos("I")||Format.Pos("Н")) Lab->Font->Style=Lab->Font->Style<<fsItalic;
        if (Format.Pos("U")||Format.Pos("Ч")) Lab->Font->Style=Lab->Font->Style<<fsUnderline;

        int j = 1;
        bool Finded=false;
        char str[2];
        str[1]=0;
        AnsiString Size="";
        while (!Finded && j<= Format.Length())
        {
            if (isdigit(Format[j]))
            {
                Finded= true;
                while (j<= Format.Length() && isdigit(Format[j]))
                {
                    str[0]=Format[j];
                    Size+=str;
                    j++;
                }
            }
            else  j++;
        }
        if (Finded) Lab->Font->Size= Size.ToIntDef(10);

        if (Format.Pos("red")   ||Format.Pos("красн")) Lab->Font->Color=clRed;
        if (Format.Pos("lime")  ||Format.Pos("зел")) Lab->Font->Color=clLime;
        if (Format.Pos("yellow")||Format.Pos("желт")) Lab->Font->Color=clYellow;

        Lab->Caption=mes.data();

        Height+=Lab->Height;
        i++;
   }
   Frm->Height=Frm->Panel1->Height+32+Height;
   Frm->ShowModal();
   Frm->Release();
   return StackItem();
  }
  if((method=="IFF" || method=="IIF") && pc==3)
  {
   v=stk[-2].Number();
   if(v) return stk[-1].Val().clone();
   return stk[0].Val().clone();
  }
  if(method=="LEN" && pc==1)
  {
   return VAL(int(stk[0].String().length()));
  }
  if(method=="REST" && pc==2)
  {
   const char *str=stk[-1].String().data() ;
   return VAL(str+min((int)stk[0].Number(),(int)strlen(str))) ;
  }
  if(method=="WORD" && pc>=2)
  {
   char *str=(char*)stk[1-pc].String().data() ;

   char *s=str ;
   int l=0 ;
   for(int i=0 ; i<pc-1 ; i++)
   {
    s=s+l ;
    l=min((int)stk[i-pc+2].Number(),(int)strlen(s)) ;
    for( ; s[l] && !strchr(" -;.,:",s[l-1]) ; l--) ;
   }

   char substr[256] ;
   strncpy(substr,s,l) ;
   substr[l]=0 ;
   return VAL(substr) ;
  }
  if(method=="TOKEN" && pc==3)
  {
   char *str=(char*)stk[1-pc].String().data() ;
   char *dm=(char*)stk[2-pc].String().data() ;
   int i=0 , n=stk[3-pc].Number() ;

   char substr[256] , *p ;
   strcpy(substr,str) ;

   for(p=strtok(substr,dm) ;  p && (i<n) ; p=strtok(NULL,dm) , i++) ;
   if (!p)
   {
    p="";
   }
   return VAL(p) ;
  }
  if(method=="SUBSTR" && pc==3)
  {
   const char *str=stk[-2].String().data() ;
   if (*str==0)
   {
        return VAL("");
   }

   int start=stk[-1].Number() ;
   int end=stk[0].Number() ;
   int len=max(end-start+1,0) ;
   char substr[512] ;

   strncpy(substr,str+min(start-1,(int)strlen(str)),len) ;
   substr[len]=0 ;
   return VAL(substr) ;
  }
  if(method=="PARTSTR" && pc==3)
  {
   const char *str=stk[-2].String().data() ;
   const char *delim=stk[-1].String().data() ;
   int count=stk[0].Number() ;
   char *s ;
   int i ;

   char substr[512] ;
   strcpy(substr,str) ;

   for(s=strtok(substr,delim), i=0 ; s && i<count ; i++)
       s=strtok(NULL,delim) ;
   if (i<count) s="" ;

   return VAL(s) ;
  }
  if(method=="DELSUB" && pc==3)
  {
   const char *str=stk[-2].String().data() ;
   int start=stk[-1].Number() ;
   int end=stk[0].Number() ;
   int len=max(end-start+1,0) ;
   char substr[256] ;

   strcpy(substr,str) ;
   memmove(substr+start-1,substr+end,strlen(substr+end)) ;
   substr[strlen(str)-len]=0 ;
   return VAL(substr) ;
  }
  // ------------------------------ !!!! Vlad 25-04-00
  // Возвращает название колонки XLS по номеру
  // Напр 1- "A"
  //      2- "B"
  //      26- "Z"
  //      27- "AA"
  // Вызов XLSC(1) вернет "А"
  //       XLSC(2,2)      "B2"

  if( method=="XLSC" && (pc==1 ||pc==2))
  {
   char str[20] ;
   str[1]=0;
   str[2]=0;
   int i=stk[1-pc].Number() ;
   if ((i<1) || (i>256))
    /// ???? Error
    str[1]=0;
   else
   {
      if (i>26)
      {
        str[0]='A'+(i-1)/26-1;
        str[1]='A'+(i-1)%26;
      }
      else str[0]='A'+i-1;
      if (pc==2)
      {
       int j=stk[2-pc].Number() ;
       if (j<0 || j>65500)
       {
        // ??? Error
       }
       else
       {
        char K [10];
        itoa(j,K,10);
        strcat(str,K);
       }
      }
   }
   return VAL((const ::string)str);
  }
  //-----------------------------------------------
  //-----------------------------------------------
  //-----------------------------------------------
  // Работа с кассовым аппаратом
  //     переделаем в отдельный объект
  //-----------------------------------------------
  //-----------------------------------------------
  //  CassaInit (ComPort =1    (1,2),
  //                  LogCassNo =1  (1-5),
  //                  Speed = 4800  (9600, 4800, 2400, 1200)
  //                  Mode= 6       (2,3,5,6)
  //  Возвратит 1 если успешно проинициализирован
  //-----------------------------------------------
  if( method=="C_CASSAINIT" && (pc<5))
  {
      int ComP=stk[1-pc].Number() ;
      int CassNo =1;
      int Speed = 4800;
      int Mode = 8;//6;
      if (pc>1) CassNo =stk[2-pc].Number();
      if (pc>2) Speed = stk[3-pc].Number();
      if (pc>3) Mode = stk[4-pc].Number();
      int Res = 1;
      message(CASS_PROMPT_1 , GOOD);
      if (Res==1) Res=SetComPort(ComP, Speed);
      if (Res==1) Res=SetMode(CassNo, Mode);
      if (Res==Mode) Res=CloseComPort(ComP);
      if (Res==1)
      {
         CurrCassa.CassaNo=CassNo;
         CurrCassa.ComPort=ComP;
         CurrCassa.Speed=Speed;
         message(CASS_INIT_OK , GOOD);
      }
      else
        message(CASS_INIT_BAD , BAD);
      return VAL(Res);
  }
  //-----------------------------------------------
  //  SetActiveCassa (ComPort =1    (1,2),
  //                     LogCassNo =1  (1-5))
  //-----------------------------------------------
  if( method=="C_SETACTIVECASSA" && (pc<3))
  {
        int ComP=1;
      if (pc>0) ComP=stk[1-pc].Number() ;
      int CassNo =1;
      if (pc>1) CassNo =stk[2-pc].Number();
      CurrCassa.CassaNo=CassNo;
      CurrCassa.ComPort=ComP;
      return VAL(0);
  }
  if( method=="C_BEGINCHECK" )
  {
      int Res=SetComPort(CurrCassa.ComPort, CurrCassa.Speed);
      //if (Res!=1);
      //!!!
      return VAL(Res);
  }
  //---------------------------------------------------
  // SellArtCash ( ArtNumber = (0-9999)
  //               ArtName   = char [14]
  //                     Price
  //                     Kol
  //              [DEPT =1 ]
  //                    [TovGroup =1]
  //                    [TaxGroup =1]
  //                    [CassaNo]
  //---------------------------------------------------
  if( method=="C_SELLARTCASH" && pc >3 && pc<6)
  {
        int ArtNo =stk[1-pc].Number();
      const char* ArticleName = stk[2-pc].String().data();
      double Price = stk[3-pc].Number();
      double Kol = stk[4-pc].Number();


      int Dept = 1;
      if (pc>4) Dept = stk[5-pc].Number();
      int TovGroup = 1;
      if (pc>5) TovGroup = stk[6-pc].Number();
      int TaxGroup =1;
      if (pc>6) TaxGroup = stk[7-pc].Number();
      int CassaNo = CurrCassa.CassaNo;
      if (pc>7) CassaNo = stk[8-pc].Number();
      char Result[200];

      itoa (ArtNo, Result, 10);
      char* p = strchr (Result, 0);
      *p= ';'; p++;

      char* c = p;

      strncpy (p, ArticleName, 14);
      p[14]=0;
      while(*c)
      {
        if (*c=='/' || *c==';' || *c=='-' || *c=='+')
        {
            *c='_';
        }
        c++;
      };
      p= strchr(p,0);
      *p=';';p++;
      sprintf(p, "%.2f", Price);
      p= strchr(p,0);
      *p=';';p++;
      sprintf(p, "%.3f", Kol);
      p= strchr(p,0);
      *p=';';p++;
      sprintf(p, "%d", Dept);
      p= strchr(p,0);
      *p=';';p++;
      sprintf(p, "%d", TovGroup);
      p= strchr(p,0);
      *p=';';p++;
      sprintf(p, "%d", TaxGroup);
      strcat(p,";0;0;");


      int Res=SellArtCash(CassaNo,Result);
      //if (Res!= 1)
      //???????????
      return VAL(Res);
  }
  //-----------------------------------------------
  if( method=="C_ENDCHECK" )
  {
      int Res=CloseReceipt(CurrCassa.CassaNo);
      if (Res==1) Res=CloseComPort(CurrCassa.ComPort);
      //if (Res!=1)
      //!!!
      return VAL(Res);
  }
  //-----------------------------------------------
  if( method=="C_SETCOMPORT" && (pc==1 ||pc==2))
  {
    int Numb=stk[1-pc].Number() ;
      int Res = 0;
      if (pc==1)
         Res=SetComPort(Numb, 4800);
      else
      {
        int Speed = stk[2-pc].Number();
         Res = SetComPort (Numb, Speed);
      }

      if (Res<0)
      {
         char * Msg=NULL;
        switch(Res)
         {
            case -1: Msg = CASS_MSG_1; break;
            case -2: Msg = CASS_MSG_2; break;
            case -3: Msg = CASS_MSG_3; break;
            case -4: Msg = CASS_MSG_4; break;
            case -5: Msg = CASS_MSG_5; break;
            case -7: Msg = CASS_MSG_6; break;
            default:
                Msg = CASS_MSG_NO_MSG;
         }
         message(Msg,BAD);
      }

      return VAL(Res);
  }
  if( method=="C_CLOSECOMPORT" && pc==1)
  {
    int Numb=stk[1-pc].Number() ;
      int Res =CloseComPort(Numb);
      if (Res<0)
      {
         char * Msg=NULL;
        switch(Res)
         {
            case -1:    Msg = CASS_MSG_7; break;
            default:
                Msg = CASS_MSG_NO_MSG;
         }
         message(Msg,BAD);
      }
      return VAL(Res);
  }

  if( method=="C_RESETCASHES")
  {
      int Res =ResetCashes();
      if (Res<0)
      {
         char * Msg=NULL;
        switch(Res)
         {
            case -1: Msg = CASS_MSG_50;  break;
            case -2: Msg = CASS_MSG_51;  break;
            default:
                Msg = CASS_MSG_NO_MSG;
         }
         message(Msg,BAD);
      }
      return VAL(Res);
  }

  if( method=="C_ISCASH" && pc==1)
  {
    int Numb=stk[1-pc].Number() ;
      int Res =IsCash(Numb);
      if (Res<0)
      {
         char * Msg=NULL;
        switch(Res)
         {
            case -2:    Msg = CASS_MSG_52; break;
            case -3:    Msg = CASS_MSG_53; break;
            case -6:    Msg = CASS_MSG_54; break;
            case -7:    Msg = CASS_MSG_55; break;
            case -8:    Msg = CASS_MSG_56; break;
            default:
                Msg = CASS_MSG_NO_MSG;
         }
         message(Msg,BAD);
      }
      return VAL(Res);
  }
  if( method=="C_STRPRNTCASH" && pc>1 && pc<7)
  {
    int Numb=stk[1-pc].Number();
      char Msgs[5][24];
      char* Str1=NULL;
      char* Str2=NULL;
      char* Str3=NULL;
      char* Str4=NULL;
      char* Str5=NULL;
      if (pc>1) {Str1=Msgs[0];strncpy (Str1, stk[2-pc].String().data(), 23);}
      if (pc>2) {Str2=Msgs[1];strncpy (Str2, stk[3-pc].String().data(), 23);}
      if (pc>3) {Str3=Msgs[2];strncpy (Str3, stk[4-pc].String().data(), 23);}
      if (pc>4) {Str4=Msgs[3];strncpy (Str4, stk[5-pc].String().data(), 23);}
      if (pc>5) {Str5=Msgs[4];strncpy (Str5, stk[6-pc].String().data(), 23);}
      Numb=StrPrntCash(Numb,Str1,Str2,Str3,Str4,Str5);
      if (Numb<0)
      {
         char * Msg=NULL;
        switch(Numb)
         {
            case -1: Msg = CASS_MSG_8;  break;
            case -2: Msg = CASS_MSG_9;  break;
            case -3: Msg = CASS_MSG_10; break;
            case -6: Msg = CASS_MSG_11; break;
            case -7: Msg = CASS_MSG_6;  break;
            case -8: Msg = CASS_MSG_13; break;
            default:
                Msg = CASS_MSG_NO_MSG;
         }
         message(Msg,BAD);
      }
      return VAL(Numb);
  }
  if( method=="C_SELLARTCASH" && pc>1 && pc<3)
  {
    int Numb=stk[1-pc].Number();
      char* Art=(char*)(stk[2-pc].String().data());
        Numb=SellArtCash(Numb,Art);
      if (Numb<0)
      {
         char * Msg=NULL;
        switch(Numb)
         {
            case -1: Msg = CASS_MSG_14;  break;
            case -2: Msg = CASS_MSG_15;  break;
            case -3: Msg = CASS_MSG_16; break;
            case -4: Msg = CASS_MSG_17; break;
            case -5: Msg = CASS_MSG_18; break;
            case -6: Msg = CASS_MSG_19; break;
            case -7: Msg = CASS_MSG_20;  break;
            case -8: Msg = CASS_MSG_21; break;
            default:
                Msg = CASS_MSG_NO_MSG;
         }
         message(Msg,BAD);
      }
      return VAL(Numb);
  }
  if( method=="C_GETMODE" && pc==1)
  {
        int Numb=stk[1-pc].Number();
      int Res = GetMode(Numb);
      if (Res<0)
      {
         char * Msg=NULL;
        switch(Res)
         {
            case -1: Msg = CASS_MSG_22; break;
            case -2: Msg = CASS_MSG_23; break;
            case -4: Msg = CASS_MSG_24; break;
            case -6: Msg = CASS_MSG_25; break;
            default:
                Msg = CASS_MSG_NO_MSG;
         }
         message(Msg,BAD);
      }
        return VAL(Res);
  }
  if( method=="C_SETMODE" && pc==2)
  {
        int Numb=stk[1-pc].Number();
        int Mode=stk[2-pc].Number();
      int Res = SetMode(Numb, Mode);
      if (Res<0)
      {
         char * Msg=NULL;
        switch(Res)
         {
            case -1: Msg = CASS_MSG_22; break;
            case -2: Msg = CASS_MSG_23; break;
            case -4: Msg = CASS_MSG_24; break;
            case -6: Msg = CASS_MSG_25; break;
            default:
                Msg = CASS_MSG_NO_MSG;
         }
         message(Msg,BAD);
      }
        return VAL(Res);
  }
  if( method=="C_CLOSERECEIPT" && pc==1)
  {
        int Numb=stk[1-pc].Number();
      int Res = CloseReceipt(Numb);
      if (Res<0)
      {
         char * Msg=NULL;
        switch(Res)
         {
            case -2: Msg = CASS_MSG_26; break;
            case -3: Msg = CASS_MSG_27; break;
            case -4: Msg = CASS_MSG_28; break;
            case -6: Msg = CASS_MSG_29; break;
            case -7: Msg = CASS_MSG_30; break;
            case -8: Msg = CASS_MSG_31; break;
            default:
                Msg = CASS_MSG_NO_MSG;
         }
         message(Msg,BAD);
      }
        return VAL(Res);
  }
  if( method=="C_GETVERDLL" && pc==0)
  {
      char* Res = GetVerDLL();
        return VAL(Res);
  }
  if( method=="C_GETLICENCE" && pc==0)
  {
      char* Res = GetLicence();
        return VAL(Res);
  }
  //-----------------------------------------------
  //-----------------------------------------------
  //  Andrei -- 06/02/2002
  //-----------------------------------------------
  //-----------------------------------------------
 if( method=="C_PAYMENT" && (pc==3))
  {
      int NumCash =stk[1-pc].Number();
      int TypeOfMoney = stk[2-pc].Number();
      double Summa = stk[3-pc].Number();
      int Res=1;
      Res=Payment(NumCash, TypeOfMoney, Summa);
      if (Res<0)
      {
        char * Msg=NULL;
        switch(Res)
         {
            case -1: Msg = CASS_MSG_1; break;
            case -2: Msg = CASS_MSG_26; break;
            case -4: Msg = CASS_MSG_54; break;
            case -5: Msg = CASS_MSG_14; break;
            case -7: Msg = CASS_MSG_30; break;
            case -8: Msg = CASS_MSG_22; break;
            default:
                Msg = CASS_MSG_NO_MSG;
         }
         message(Msg,BAD);
      }
      return VAL(Res);
  }
  //-----------------------------------------------
  //-----------------------------------------------
  //-----------------------------------------------
  //        Выдает название -- xls файла
  //-----------------------------------------------
  //-----------------------------------------------
  if( method=="EXCELNAME" && pc==1)
  {
      extern char doc_path[] ;
      string Str =string(doc_path)+"\\"+stk[1-pc].String()+".xls";
    FILE *out=prog_ptr->GetOut();
    if(!out) return StackItem();

      fprintf(out,"%s\n", Str.data());
      prog_ptr->Out_Line()++;
      return StackItem();
  }
  //-----------------------------------------------
  //-----------------------------------------------
  //        Выдает название -- xls файла в поддир XLS
  //-----------------------------------------------
  //-----------------------------------------------
  if( method=="EXCELNAMEXLS" && pc==1)
  {
      extern char doc_path[] ;
      string Str =string(doc_path)+"\\xls\\"+stk[1-pc].String()+".xls";
    FILE *out=prog_ptr->GetOut();
    if(!out) return StackItem();

      fprintf(out,"%s\n", Str.data());
      prog_ptr->Out_Line()++;
      return StackItem();
  }
  //-----------------------------------------------
  if (method=="DOCPATH" && pc==0)
  {
   extern char doc_path[] ;
   return VAL(::string(doc_path)+"\\") ;
  }
  //-------------------------------------------------
  if( method=="SAVERESTO" && pc==1)
  {
        prog_ptr->SetResFileName(stk[1-pc].String());
        return StackItem();
  }
  //-------------------------------------------------

  if( method=="ENCODE" && pc==1)
  {
        prog_ptr->SetEncodingName(stk[1-pc].String());
        return StackItem();
  }
  //-------------------------------------------------

  if( method=="IMPORTFILE" && pc==1)
  {
        prog_ptr->SetImpFileName(stk[1-pc].String());
        return StackItem();
  }
  //-------------------------------------------------
  if((method=="STRPOS" || method=="STRPOSI") && pc==2)
  {
   char str[256] ; strcpy(str,stk[-1].String().data()) ;
   char substr[256] ; strcpy(substr,stk[0].String().data()) ;

   if (method=="STRPOSI")
   {
    rus_strupr(str) ;
    rus_strupr((char*)substr) ;
   }

   const char *p=strstr(str,substr) ;
   if (p) return VAL((int)(p-str)+1) ;
   else return VAL(0) ;
  }
  if(method=="NORMACC" && pc==1)
  {
   char accname[40] ;
   strcpy(accname,stk[0].String().data()) ;
   AccRec::Normalize(accname) ;
   return VAL((const ::string)accname) ;
  }
  if(method=="USERACC" && pc==1)
  {
   char accname[40] ;
   Acc_UserName(accname,stk[0].String().data()) ;
   return VAL((const ::string)accname) ;
  }
  if(method=="NULL")
  {
   return StackItem();
  }
  if(method=="IS_UNDEF" && pc==1)
  {
   if(stk[0].Stru()) return VAL(0);
   return VAL(int(stk[0].vtype()==gcl::VT_NULL));
  }
  if((method=="IS_NULL" || method=="ISNULL") && pc==1)
  {
   return VAL(int(stk[0].vtype()==gcl::VT_NULL || stk[0].Val().isNull()));
  }
  if(method=="QOUT")
  {
   FILE *out=prog_ptr->GetOut();
   if(!out) return StackItem();

   if(!pc)
   {
    fprintf(out,"\n");
    prog_ptr->Out_Line()++;
    return StackItem();
   }

   ::string &fmt=stk[1-pc].Val().String();
   int cpos=0;

   for(int i=-pc+2;i<=0;i++)
   {
    int fpos=fmt.find("%%",cpos);
    if(fpos==-1) break;
    fprintf(out,"%.*s",fpos-cpos,fmt.data()+cpos);

    VAL &v=stk[i].Val();
    if(v.vtype()==gcl::VT_STRING) fprintf(out,"%s",v.String().data());
    if(v.vtype()==gcl::VT_NUM) fprintf(out,"%Lf",v.Number());
    if(v.vtype()==gcl::VT_INT) fprintf(out,"%d",(int)v.Number());
    cpos=fpos+2;
   }
   if(fmt.length() && fmt[fmt.length()-1]=='\\')
    fprintf(out,"%.*s",fmt.length()-cpos-1,fmt.data()+cpos);
   else
   {
    const char *p=fmt.data()+cpos ;
    char *p1 ;

    for( ; p1=(char*)strchr(p,26) ; )
       *p1=' ' ;

    fprintf(out,"%s\n",p);
    prog_ptr->Out_Line()++;
   }
   return StackItem();
  }
  if(method=="QLINE" && pc<=1)
  {
   int ol=prog_ptr->Out_Line();
   if(pc>0) prog_ptr->Out_Line()=stk[1-pc].Number();
   return VAL(ol);
  }
  if(method=="OUTSTR" && (pc==3 || pc==2))
  {
   int align=0,num_dig=def_numdig,zeros=def_zeros;
   char point=def_point ;
   int len=stk[2-pc].Number();
   if(pc==3)
   {
    char const *fmt=stk[0].String().data();

    int l,d;
    const char *ld=strpbrk(fmt,"0123456789.") ;
    if (ld)
    {
     if(sscanf(ld,"%d.%d",&l,&d)==2) { len=l;  num_dig=d; }
     else if(sscanf(ld,".%d",&d)==1) { num_dig=d; }
     else if(sscanf(ld,"%d.",&l)==1 && strchr(fmt,'.')) { len=l; }
     else if(sscanf(ld,"%d",&d)==1)
     {
      if (stk[1-pc].Val().Fld().vtype()==gcl::VT_NUM)
         num_dig=d;
      else
         len=d ;
     }
    }

    if(strpbrk(fmt,"zZ")) zeros=1;
    if(strpbrk(fmt,"eE")) zeros=0;
    if(strpbrk(fmt,"lL«‹")) align=SA_LEFT;
    if(strpbrk(fmt,"cCж–")) align=SA_CENTER;
    if(strpbrk(fmt,"rRЇЏ")) align=SA_RIGHT;
    if(strpbrk(fmt,"pP")) point=strpbrk(fmt,"pP")[1];
   }

   char buf[280];
   buf[0]=0;

   int flags=(zeros ? 0 : GS_NOZERO) | (def_apps ? 0 : GS_NOAPPS) | GS_NOFULL ;
   if(!stk[1-pc].Val().isUndef())
   {
    stk[1-pc].Val().Fld().Output(buf,flags,num_dig,point) ;
    if (align==0) align=stk[1-pc].Val().Fld().DefAlign() ;
   }
   else { strcpy(buf,"<NULL>"); align=SA_LEFT; }
   char rbuf[280];
   stralign(rbuf,buf,len,align);

   return VAL((const ::string)rbuf);
  }
  if((method=="SCRIPT_USD" || method=="SCRIPT_UKR" || method=="SCRIPT" ) && (pc))
  {
     if(stk[1-pc].vtype()==gcl::VT_DATE)
     {
        DateTime dt=stk[1-pc].GetDate();
        char buf[250];
        int date_quote=1;
        if(pc>1 && stk[2-pc].vtype()!=gcl::VT_NULL) date_quote=!stk[2-pc].Number();
        return VAL(::string(dt.GetTextStr(buf,method!="SCRIPT",date_quote)));
     }
     else
     {
        double v=stk[1-pc].Number();

        char sum_str[30] , kop_str[20] ,res_str[200];

        int usd=(method=="SCRIPT_USD");
        int lang=(method=="SCRIPT_UKR");
        int short_nm=1,text_with_num=0,text_kops=1;
        if(v<=0) return VAL("");

        if(pc>1 && stk[2-pc].vtype()!=gcl::VT_NULL) short_nm=stk[2-pc].Number();
        if(pc>2 && stk[3-pc].vtype()!=gcl::VT_NULL) text_kops=stk[3-pc].Number();
        if(pc>3 && stk[4-pc].vtype()!=gcl::VT_NULL) text_with_num=stk[4-pc].Number();

        sprintf(sum_str,"%d",int(v));

        void ConvertSum(char *dst,char *src,int usd,int with_num,int lang,int short_grn);
        ConvertSum(res_str,sum_str,usd?usd+(!short_nm):0,text_with_num,lang,short_nm) ;
        if (text_kops)
        {
         char kops[30] ;
         sprintf(kops,"%02.0lf",(v-int(v))*100) ;
         int low_num=kops[1]-'0' ;
         sprintf(kop_str," %02.0lf %s",(v-int(v))*100,
                           usd ? Name_cnt : short_nm ? Name_kop : Names_kop[lang][low_num==1?0:(low_num && low_num<=4)?1:2]) ;

         strcat(res_str,kop_str) ;
        }
        return VAL((const ::string)res_str);
     }
  }
  if(method=="SCRIPT_EXT" && (pc>=4))
  {
        double v=stk[1-pc].Number();
        if(v<=0) return VAL("");

        char sum_str[30] , kop_str[20] ,res_str[200];

        int gender=stk[2-pc].Number();
        int lang=stk[3-pc].Number();
        char const *cur_name=stk[4-pc].String().data();
        char const *kop_name=0;
        if(pc>4 && stk[5-pc].vtype()!=gcl::VT_NULL) kop_name=stk[5-pc].String().data();

        int text_with_num=0;
        if(pc>5 && stk[6-pc].vtype()!=gcl::VT_NULL) text_with_num=stk[6-pc].Number();

        sprintf(sum_str,"%d",int(v));

       void ConvertSumExt(char *dst,char *src,int with_num,int lang,int gender,char const *cur_name);

       ConvertSumExt(res_str,sum_str,text_with_num,lang,gender,cur_name) ;
       if (kop_name)
       {
          sprintf(kop_str," %02.0lf %s",(v-int(v))*100,kop_name);
          strcat(res_str,kop_str) ;
        }
        return VAL((const ::string)res_str);
  }
  if(method=="DISTR_LINE" && pc==5)
  {
    AnsiString anal_name=ToAnsiStr(stk[1-pc].String());
    AnsiString set=ToAnsiStr(stk[2-pc].String());
    int as=stk[3-pc].Number();
    int ras=stk[4-pc].Number();
    AnsiString article=ToAnsiStr(stk[5-pc].String());

    TFldsDescrAnDestr FD;

    FD.ReadDescr(anal_name, set);
    FD.SetAnSum(ras, as);
    TAmsDS ADS;

    TOracleDataSet* ODS= ADS();

    FD.BuildSqlQuery(ODS);
    ODS->Active= true;
    FD.IndexFields(ODS);
    try
    {
        DistrLine (&FD, as, ras, ODS);
    }
    catch(...)
    {
        DatMod->OSession->Rollback();
        throw;
    }
    DatMod->OSession->Commit();
    return StackItem();
  }
  if((method=="INTERVAL" || method=="INTERVAL_UKR") && pc>=2)
  {
        DateTime dt=stk[1-pc].GetDate();
        DateTime dt1=stk[2-pc].GetDate();
        char buf[200];
        int no_year=1;
        if(pc>2 && stk[3-pc].vtype()!=gcl::VT_NULL) no_year=!stk[3-pc].Number();
        return StackItem(VAL(::string(dt.IntervalStr(buf,dt1,method!="INTERVAL",no_year))));
  }


  return GlobStructure().Method(This,method,pc,stk);
}

#pragma argsused
StackItem Struct::Method(Node &node,::string const &method,int pc,StackItem *stk)
{
  if(method=="DUMP")
  {
   FILE *out=prog_ptr->GetOut();
   if(!out) return StackItem();

   if(pc)
   {
     ::string s=stk[0].Val().String();
     fprintf(out,"%s\n",s.data());
     prog_ptr->Out_Line()++;
   }
   Node **cn;
   for(cn=els.begin();cn<els.end();cn++)
   {
    fprintf(out,"%-15s",(*cn)->name.data());
    if((*cn)->vtype()==gcl::VT_STRING) fprintf(out," %s",(*cn)->String().data());
    if((*cn)->vtype()==gcl::VT_NUM) fprintf(out," %Lf",(*cn)->Number());
    if((*cn)->vtype()==gcl::VT_INT) fprintf(out," %d",(int)(*cn)->Number());
    if((*cn)->vtype()==gcl::VT_DATE)
    {
     char t[40];
     (*cn)->GetDate().Out(t,40);
     fprintf(out," %s",t);
    }
    if((*cn)->st)  fprintf(out," Structure");
    fprintf(out,"\n");
    prog_ptr->Out_Line()++;
   }
   fprintf(out,"\n");
   prog_ptr->Out_Line()++;
   return StackItem();
  }
  if(method=="WDUMP")
  {
   ::string mes;
   if(pc)
   {
     ::string s=stk[0].Val().String();
     mes+=s+'\n';
   }
   char outn[40],out[40];
   Node **cn;
   int i=0 ;
   for(cn=els.begin();cn<els.end();cn++)
   {
    *out=0;
    sprintf(outn,"%-15s",(*cn)->name.data());


    if ((*cn)->fld)
    {
     if((*cn)->fld->isNull() || (*cn)->vtype()==gcl::VT_NULL)
        sprintf(out,"<NULL>") ;
     else if((*cn)->vtype()==gcl::VT_STRING) sprintf(out," %.35s",(*cn)->String().data());
     else if((*cn)->vtype()==gcl::VT_NUM) sprintf(out," %Lf",(*cn)->Number());
     else if((*cn)->vtype()==gcl::VT_INT) sprintf(out," %d",(int)(*cn)->Number());
     else if((*cn)->vtype()==gcl::VT_DATE)
     {
      char t[40];
      (*cn)->GetDate().Out(t,30);
      sprintf(out," %s",t);
     }
    }
    else if((*cn)->st)  sprintf(out," Structure");

    mes+=outn;
    mes+=out;
    mes+='\n';

    i++ ;
    if (i>30)
    {
     Message(mes.data());
     i=0 ;
     mes="" ;
    }
   }
   mes+='\n';
   Message((char *)mes.data());
   return StackItem();
  }
  if(method=="SIZE" && pc==0)
  {
   return VAL(els.size());
  }
  if(method=="AT" && pc==1)
  {
   int r=stk[0].Number();
   if(r<0 || r>=els.size()) prog_ptr->RTError(106,outofrangeerr);
   return *els[r];
  }
  if(method=="FLD" && pc==1)
  {
   char fld[50] ;
   strcpy(fld,stk[0].String().data());
   rus_strupr(fld) ;
   Node *n=FindVar(fld);
   if(!n) prog_ptr->RTError(115,nosuchvarerr);
   return *n ;
  }
  prog_ptr->RTError(101,::string(unknownfuncerr)+"  "+method);
  return StackItem();
}


