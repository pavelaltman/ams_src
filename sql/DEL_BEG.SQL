ALTER SESSION SET nls_date_language=RUSSIAN ;
ALTER SESSION SET NLS_DATE_FORMAT = 'DD-MM-YY' ;
alter rollback segment myroll online ;
commit ;


set transaction use rollback segment myroll ;
delete cor_items where op_id in (select op_id from opers where op_date<'1-1-98') ;
commit ;

set transaction use rollback segment myroll ;
delete op_items where op_date<'1-1-98' ;
commit ;

set transaction use rollback segment myroll ;
delete opers where op_date<'1-1-98' ;
commit ;

set transaction use rollback segment myroll ;
delete turns where beg_date<'1-1-98' ;
commit ;

set transaction use rollback segment myroll ;
delete cor_turns where beg_date<'1-1-98' ;
commit ;

set transaction use rollback segment myroll ;
update user_options set beg_date=NULL,end_date=NULL where beg_date<'1-1-98' ;
commit ;

set transaction use rollback segment myroll ;
delete cints where beg_date<'1-1-98' ;
commit ;

set transaction use rollback segment myroll ;
delete rems where rem_date<'1-1-98' ;
commit ;

set transaction use rollback segment myroll ;
delete cps where point_date<'1-1-98' ;
commit ;

update cps set is_start=1000 where point_date='1-1-98' ;
commit ;
