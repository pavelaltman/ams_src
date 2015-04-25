ALTER SESSION SET nls_date_language=RUSSIAN ;
ALTER SESSION SET NLS_DATE_FORMAT = 'DD-MM-YY' ;

delete turns where beg_date='1-10-98' ;
delete cor_turns where beg_date='1-10-98' ;
update user_options set beg_date=NULL where beg_date='1-10-98' ;
delete cints where beg_date='1-10-98' ;

delete rems where rem_date='1-10-98' ;
delete cps where point_date='1-10-98' ;
