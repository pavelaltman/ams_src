alter rollback segment myroll online ;

ALTER SESSION SET nls_date_language=RUSSIAN ;
ALTER SESSION SET NLS_DATE_FORMAT = 'DD-MM-YY' ;

commit ;
set transaction use rollback segment myroll ;
DELETE UNAPP_items ;
DELETE UNAPP_cor_items ;
DELETE op_items WHERE node>0 ;
DELETE cor_items ;
COMMIT ;

set transaction use rollback segment myroll ;
delete rems ;
delete turns ;
delete cor_turns ;
update user_options set beg_date=NULL, end_date=NULL ;
delete cints ;
delete cps ;
commit ;

delete op_items ;
delete opers ;
delete del_op_items ;
delete del_opers ;
commit ;
