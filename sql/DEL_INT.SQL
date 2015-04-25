ALTER SESSION SET nls_date_language=RUSSIAN ;
ALTER SESSION SET NLS_DATE_FORMAT = 'DD-MM-YY' ;
alter rollback segment myroll online ;
commit ;

set transaction use rollback segment myroll ;
delete cor_items where op_id in
  (select op_id from opers where op_date>='&2.' or op_date<'&1.') ;
commit ;

set transaction use rollback segment myroll ;
delete op_items where op_id in
  (select op_id from opers where op_date>='&2.' or op_date<'&1.') ;
commit ;

set transaction use rollback segment myroll ;
delete history where op_id in
  (select op_id from opers where op_date>='&2.' or op_date<'&1.') ;
commit ;

set transaction use rollback segment myroll ;
delete opers where op_date>='&2.' or op_date<'&1.' ;
commit ;

set transaction use rollback segment myroll ;
delete history where op_id in
  (select op_id from del_opers where op_date>='&2.' or op_date<'&1.') ;
commit ;

set transaction use rollback segment myroll ;
delete del_op_items where op_id in
  (select op_id from del_opers where op_date>='&2.' or op_date<'&1.') ;
commit ;

set transaction use rollback segment myroll ;
delete del_opers where op_date>='&2.' or op_date<'&1.' ;
commit ;

set transaction use rollback segment myroll ;
delete turns where end_date>'&2.' or beg_date<'&1.' ;
commit ;

set transaction use rollback segment myroll ;
delete cor_turns where end_date>'&2.' or beg_date<'&1.' ;
commit ;

set transaction use rollback segment myroll ;
update user_options set beg_date=NULL,end_date=NULL
          where end_date>'&2.' or beg_date<'&1.' ;
commit ;

set transaction use rollback segment myroll ;
delete cints where end_date>'&2.' or beg_date<'&1.' ;
commit ;

set transaction use rollback segment myroll ;
delete rems where rem_date>'&2.' or rem_date<'&1.' ;
commit ;

set transaction use rollback segment myroll ;
delete cps where point_date>'&2.' or point_date<'&1.' ;
commit ;

update cps set is_start=1000 where point_date='&1.' ;
commit ;
exit;