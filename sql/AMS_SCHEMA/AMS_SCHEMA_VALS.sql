 set head off;
 set verify off;
 set termout off;

 
 spool W:\SQL_BAT\ADD\AMS_VALS.sql

 select 'insert into SYS_INFO(VAL_TYPE,VAL_DESCR)values('||
        ''''||VAL_TYPE||''','||
        ''''||VAL_DESCR||''') '
 from AMS.SYS_INFO;

 spool off;
 exit;



