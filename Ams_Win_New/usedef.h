#ifndef USEDEF_
#define USEDEF_

int CheckNode(void *p) ;

#ifdef __cplusplus
#define Delete_(a) { if(CheckNode(a)) delete a ; a=0 ; }
#define DeleteA(a) { if(CheckNode(a)) delete []a ; a=0 ; }
#define This (*this)
#define ClearVar(a) memset(&a,0,sizeof(a))
#endif /* __cplusplus */

#define _free(a) { if(CheckNode(a)) free(a) ; a=0 ; }

#endif
