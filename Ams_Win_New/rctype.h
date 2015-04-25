#ifndef _RCTYPE
#define _RCTYPE

#undef isprint
#undef isgraph

#define isprint(c)  ((c) >= 0x20)
#define isgraph(c)  ((c) >= 0x21)


void	InitCType() ;
int     rus_toupper(int ch) ;
char *	rus_strupr(char *str) ;
void rus_strupra(AnsiString& str) ;


#endif
