/*****                            comisc.h                              *****/

#ifndef _COMISC
#define _COMISC

#include "usedef.h"

#include <stdio.h>
#include <dir.h>

#define point_in_box(x,y,x1,y1,x2,y2) ((x)>=(x1) && (x)<=(x2) && (y)>=(y1)\
 && (y)<=(y2))

#define MAX(x,y) ( (x) > (y) ? (x) : (y) )
#define MIN(x,y) ( (x) < (y) ? (x) : (y) )

#define lowint(l)	((int)(l))
#define highint(l)	((int)(l>>16))
#define makelong(hi,lo)	((long)(lo)+((long)(hi)<<16))

#define	SA_LEFT		1
#define	SA_CENTER	2
#define	SA_RIGHT	3

extern  char outofmem[] , cantopenmes[] ;
extern	unsigned	long	core ;

int	exist(char *) ,
	getfirstlimit(int num,char *longstr,char *shortstr,int *begpos,
		      char *delims) ,
	getnextlimit(int num,char *longstr,char *shortstr,int *begpos,
		     char *delims) ,
	_copyfile(struct ffblk *finfo,char *,char *) ,
	alloc_mem(int *alloc_num,int num,int poolsize,int itemsize,
		  void **ptr) ,
	is_ident(char *str) ,
	strempty(char *str) ;
void	copyfile(char *,char *) ,
	printcore(char *) ,
	rec_copy(char *s_path,char *d_path,int print) ,
	delext(char *) ,
	strstlen(char *str,int len) ,
	stralign(char *buf,char *str,int len,int type) ,
	copyshortname(char *name,char *fullmame) ,
	copypathname(char *path,char *fullmame) ,
	emp(void) ,
	del_slash(char *path,int del_in_root) ,
	res_slash(char *path) ,
	strblank(char *) ,
	strcharcopy(char *dest,char *source,char stop) ,
	strncatchar(char *str,int n,int ch) ,
	expand_tabs(char *str,int tab_len) ,
//	farfread(void huge *ptr,unsigned size,unsigned long num,FILE *f) ,
	skipstr(FILE *,int) ;

char *	_truncstr(char *str,int len),
     *   truncstr(char *str,char *buf,int len)  ;

//unsigned long farfwrite(void huge *ptr,unsigned size,unsigned long num,FILE *f) ;
long HeapCheck(char *m) ;

#endif

