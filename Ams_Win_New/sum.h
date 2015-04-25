/*****				    sum.h				*****/

#ifndef _SUM
#define _SUM

#include <_null.h>

#define DEBET   'D'
#define CREDIT  'K'

#define	GS_SIGN		1
#define	GS_NOAPPS	4
#define	GS_NOZERO	8
#define  GS_ALLDIGS  32
#define  GS_NOFULL   64

#define SS_OK		0
#define SS_BAD		1
#define SS_TOOBIG	2

#define MAXSUMLEN 24

extern	int	SUM_LEN	;
extern	int	SUMLEN	;
extern   int   num_dig ;
extern   int   div_val ;

class	Sum
	{
	public :
	 long double	dlv ;

     int Int()const {return dlv;};

	 void		Zero() { dlv=0 ; } ;
	 Sum() { Zero() ; } ;

	 Sum(long l) { dlv=l ;} ;
	 Sum(unsigned s) { dlv=s ; } ;
	 Sum(int s) { dlv=s ; } ;
	 Sum(long double s) { dlv=s ; } ;
	 Sum(const Sum& s) { dlv=s.dlv ; } ;

	 Sum& operator =(const Sum& s) { dlv=s.dlv ; return *this;} ;
	 Sum& operator =(const long double& s) { dlv=s ; return *this;} ;

	 Sum &operator+=(const Sum& r)
			{
			 dlv+=r.dlv ;
			 return *this ;
			} ;
	 Sum &operator-=(const Sum& r)
			{
			 dlv-=r.dlv ;
			 return *this ;
			} ;
	 Sum &operator*=(const Sum& r)
			{
			 dlv*=r.dlv ;
			 return *this ;
			} ;
	 Sum &operator/=(const Sum& r)
			{
			 dlv/=r.dlv ;
			 return *this ;
			} ;
	 int	operator==(const Sum& r) const
			{ return dlv==r.dlv ; } ;
	 int 	operator>(const Sum& r) const
			{ return dlv>r.dlv ; } ;
	 int		operator<(const Sum& r) const
			{ return dlv<r.dlv ; } ;
    //operator long double&() { return dlv;}
    operator const long double&() const { return dlv;}

	 int		IsPos() { return dlv>0 ; } ;
	 int		IsNeg() { return dlv<0 ; } ;
	 int		IsNonNeg() { return dlv>=0 ; } ;
	 int		IsNonZero() { return dlv!=0 ; } ;
	 int		IsZero() { return dlv==0 ; } ;
	 void		Neg() { dlv=-dlv ; } ;
	 char *	GetStr(char *buf,unsigned flags=0,int showdig=-1,int pchar='-') ;
	 char *	GetStrN(char *buf,unsigned flags=0,int showdig=-1) ;
	 int		SetStr(char *str,int side=DEBET,char **stop=NULL,int nominus=0,
			          int allow_dig=6,int no_rest=0) ,
  			   SetStrMes(char *str,int side=DEBET,char **stop=NULL,
				          int nominus=0,int allow_dig=6,int no_rest=0) ;
	 void		Round(int digits) ;
	 Sum		Pval(int side) ;
	} ;

extern int	SUM_LEN ;
extern int	SUMLEN ;
extern int   show_dig ;
extern int   apostrophe ;

inline int OtherSide(int side) { return side==DEBET ? CREDIT : DEBET ; };
int InputSum(char *prompt,char *sum_str,int init,int inp_dig=show_dig) ;

#endif
