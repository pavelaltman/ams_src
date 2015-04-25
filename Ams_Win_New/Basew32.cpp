#include "AMS_HDR.h"
#pragma hdrstop

#include <windows.h>
//#include "pashlib.h"

int ukr_key=0 , no_cyr=0 ;

short maxrow=24 , maxcol=79 , chsizey=1 , chsizex=1 , rus=0 ;
HANDLE hOut , hIn ;
void My_OemToChar(char const *s,char *d)
{ OemToChar(s,d); }

void My_CharToOem(char const *s,char *d)
{ CharToOem(s,d); }
int Copy_File(char const *was,char const *newn,int keep_ex)
{
 return CopyFile(was,newn,keep_ex);
}

int Rename_File(char const *was,char const *newn)
{
 return MoveFile(was,newn);
}

int Delete_File(char const *was)
{
 return DeleteFile(was);
}


// Dummies
int prnready() { return 1 ; }
void sound_beep(int) {}
void sound_click() {}
void ShowHelp(int) {}


