ALTER USER "SYS" IDENTIFIED BY "SYS";
ALTER USER "SYSTEM" IDENTIFIED BY "SYSTEM";

ALTER DATABASE DATAFILE '&1.\DATABASE\USR1ORCL.ORA' 
AUTOEXTEND ON NEXT 50M MAXSIZE UNLIMITED;

ALTER TABLESPACE "USER_DATA" 
DEFAULT STORAGE ( INITIAL 100K NEXT 1M MAXEXTENTS UNLIMITED );

ALTER TABLESPACE "ROLLBACK_DATA" 
DEFAULT STORAGE ( NEXT 1024M MAXEXTENTS UNLIMITED );

ALTER DATABASE DATAFILE '&1.\DATABASE\RBS1ORCL.ORA' 
AUTOEXTEND ON NEXT 10M MAXSIZE UNLIMITED;

ALTER DATABASE DATAFILE '&1.\DATABASE\SYS1ORCL.ORA' 
AUTOEXTEND ON NEXT 10M;

CREATE PUBLIC ROLLBACK SEGMENT myroll TABLESPACE
ROLLBACK_DATA STORAGE (MAXEXTENTS UNLIMITED) ;
ALTER ROLLBACK SEGMENT myroll ONLINE ;

@&1.\DBS\pupbld.sql
