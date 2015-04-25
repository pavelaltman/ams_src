#include "AMS_HDR.h"
#pragma hdrstop

//#include "pashlib.h"

#define STRLISTPOOL     10

int StrList::Alloc(int n)
{
 Free() ;
 if (n<=0) return 1 ;
 ptr=new char*[n] ;
 if (ptr==NULL) { ErrorMessage(outofmem) ; return 0 ; }
 num=alloc_num=n ;
 for(int i=0 ; i<n ; i++) ptr[i]=NULL ;
 return 1 ;
}

int StrList::AllocStr(int i,int strsize)
{
 if (i<0 || i>=num)
    exit(printf("AllocStr out of range")) ;
 if (strsize<0)
    exit(printf("AllocStr bad size")) ;

 DeleteA(ptr[i]);
 ptr[i]=new char[strsize+1] ;
 if (ptr[i]==NULL) { message(outofmem,BAD) ; return 0 ; }
 *ptr[i]=0 ;
 return 1 ;
}

int StrList::AllocAll(int n)
{
 for(int i=0 ; i<num ; i++)
 {
  if (!AllocStr(i,n)) return 0 ;
 }
 return 1 ;
}

void StrList::Free(int start)
{
 for(int i=start ; i<num ; i++)
 {
  if (ptr[i]!=NULL) DeleteA(ptr[i]) ;
  ptr[i]=NULL ;
 }
 if (alloc_num>0 && start==0){ DeleteA(ptr); alloc_num=0;}
 if(start<num) num=start ;
}

int StrList::AddStr(int index,char *str)
{
 if (!AllocStr(index,strlen(str))) return 0 ;
 if (str[0]) strcpy(ptr[index],str) ;
 else ptr[index][0]=0;
 return 1 ;
}

int StrList::AddSpace(int newnum,int size)
{
 if (newnum>num || newnum<0) exit(printf("AddSpace failed")) ;

 num++ ;
 if (num>alloc_num)
 {
  if (alloc_num>0)
  {
   char **_ptr=new char *[num+STRLISTPOOL];
   if(_ptr)
   {
    memcpy(_ptr,ptr,alloc_num*sizeof(char*));
    delete []ptr;
    ptr=_ptr;
    alloc_num=num+STRLISTPOOL ;
   }
  }
  else
  {
   alloc_num=STRLISTPOOL ;
   ptr=new char*[alloc_num];
  }
  if (ptr==NULL)
     { message(outofmem,BAD) ; return 0 ; }
 }

 for(int i=num-1 ; i>newnum ; i--) ptr[i]=ptr[i-1] ;

 ptr[newnum]=NULL ;
// if (size>0)
   if (!AllocStr(newnum,size)) return 0 ;
 return 1 ;
}

int StrList::_AddStr(char *str,int newnum)
{
//  ---------------------------   MY OPINION  --------------------------
 if (!AddSpace(newnum,strlen(str))) return 0 ;
 if (str[0]) strcpy(ptr[newnum],str) ;
 else ptr[newnum][0]=0;
 return 1 ;
}

void StrList::DelStr(int delnum)
{
 if (delnum<0 || delnum>=num) return ;
 if (ptr[delnum]!=NULL) DeleteA(ptr[delnum]) ;
 for(int i=delnum ; i<num-1 ; i++) ptr[i]=ptr[i+1] ;
 num-- ;
 if (num==0 && ptr) { delete []ptr ; alloc_num=0 ; ptr=0; }
}

StrList::~StrList()
{
 Free() ;
}

