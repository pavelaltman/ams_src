#include "accs.h"

class GlobalObjects
      {
       public:

       UserOptRec  ur ;
       BookNameRec bn ;
       char       main_title[100] ;
       int        monochrome,readonly,audsid ;

       void FormTitle() ;
       int ReadOnly()      { return readonly; }
       DateTime &BegDate() { return ur.beg_date.val ; }
       DateTime &EndDate() { return ur.end_date.val ; }
       DateTime LastDate() { return ur.beg_date.val.PrevDay() ; }
      } ;
extern GlobalObjects G ;
int HaveRole(cursor &crsr,char *role);

