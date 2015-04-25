--select * from PRIVTEST;
update OBJPRIV
set RUNAME='&1.SUPER'
where 
    RUNAME='&2.SUPER';
commit;
exit;
