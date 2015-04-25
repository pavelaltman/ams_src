alter rollback segment myroll online ;

ALTER SESSION SET nls_date_language=RUSSIAN ;
ALTER SESSION SET NLS_DATE_FORMAT = 'DD-MM-YY' ;

commit ;

set transaction use rollback segment myroll ;
delete unapp_items ;
commit ;

set transaction use rollback segment myroll ;
delete unapp_cor_items ;
commit ;

set transaction use rollback segment myroll ;
delete op_items ;
commit ;

set transaction use rollback segment myroll ;
delete cor_items ;
commit ;

set transaction use rollback segment myroll ;
delete opers ;
commit ;

set transaction use rollback segment myroll ;
delete del_op_items ;
commit ;

set transaction use rollback segment myroll ;
delete del_opers ;
commit ;
