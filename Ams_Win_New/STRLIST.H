/*****				  strlist.h				*****/

#ifndef	_STRLIST
#define	_STRLIST

#ifdef __cplusplus

#ifndef NULL
#define	NULL	0L
#endif

class	StrList
	{
	protected :
	 char **	ptr ;
	 int		num ;
	 int		alloc_num ;

	 int		_AddStr(char *str,int newnum) ;

	public :

	 StrList() { ptr=NULL ; num=alloc_num=0 ; } ;

	 void		Copy(StrList &r)
			   {
			    ptr=r.ptr ; num=r.num ; alloc_num=r.alloc_num ;
			    r.num=r.alloc_num=0 ; r.ptr=NULL ;
			   } ;
	 char **	GetPtr() { return ptr ; } ;
	 int		GetNum() { return num ; } ;
	 int		Alloc(int n) ;
	 int		AllocStr(int i,int strsize) ;
	 int		AllocAll(int n) ;
	 virtual void	Free(int start=0) ;
	 int		AddSpace(int newnum,int size) ;
	 int		AddStr(int index,char *str) ;
	 int		AddStr(char *str,int &index)
			   {
			    index=num ;
			    return _AddStr(str,num) ;
			   } ;
	 int		AppendStr(char *str)
			   {
			    return _AddStr(str,num) ;
 			   } ;
	 int		InsertStr(char *str,int newnum)
			   {
			    return _AddStr(str,newnum) ;
			   } ;
	 int		AddStr(char *str)
			   {
			    return _AddStr(str,0) ;
			   } ;
	 void		DelStr(int delnum=0) ;
	 char *	GetStr(int i) { return ptr[i] ; } ;
	 int     GetLimit(){return num;}
	 ~StrList() ;
	} ;

#endif

#endif
