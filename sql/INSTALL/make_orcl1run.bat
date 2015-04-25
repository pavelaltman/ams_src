echo spool %ORA_PATH%\database\spoolmain > %1
echo set echo on >> %1
echo connect INTERNAL/sys  >> %1
echo ALTER DATABASE DATAFILE '%ORA_PATH%\database\Sys1orcl.ora' AUTOEXTEND ON;  >> %1
echo CREATE ROLLBACK SEGMENT SYSROL TABLESPACE "SYSTEM" STORAGE (INITIAL 100K NEXT 100K); >> %1
echo ALTER ROLLBACK SEGMENT "SYSROL" ONLINE; >> %1
echo @%ORA_PATH%\Rdbms80\admin\catalog.sql; >> %1
echo @%ORA_PATH%\Rdbms80\admin\catproc.sql >> %1
echo @%ORA_PATH%\Rdbms80\admin\caths.sql >> %1

echo REM **************TABLESPACE FOR ROLLBACK***************** >> %1
echo CREATE TABLESPACE ROLLBACK_DATA DATAFILE '%ORA_PATH%\database\Rbs1orcl.ora' SIZE 50M >> %1
echo DEFAULT STORAGE ( INITIAL 1024K NEXT 1024K MINEXTENTS 2 MAXEXTENTS 121 PCTINCREASE 0); >> %1
echo ALTER DATABASE DATAFILE '%ORA_PATH%\database\Rbs1orcl.ora' AUTOEXTEND ON; >> %1

echo REM ***********Alter system tablespace ******************** >> %1
echo ALTER TABLESPACE SYSTEM >> %1
echo DEFAULT STORAGE ( INITIAL 100K NEXT 100K MINEXTENTS 1 MAXEXTENTS 300 PCTINCREASE 1); >> %1

echo REM **************TABLESPACE FOR USER********************* >> %1
echo CREATE TABLESPACE USER_DATA DATAFILE '%ORA_PATH%\database\Usr1orcl.ora' SIZE 500M >> %1
echo DEFAULT STORAGE ( INITIAL 50K NEXT 50K MINEXTENTS 1 MAXEXTENTS 121 PCTINCREASE 1); >> %1
echo ALTER DATABASE DATAFILE '%ORA_PATH%\database\Usr1orcl.ora' AUTOEXTEND ON; >> %1

echo REM **************TABLESPACE FOR TEMPORARY***************** >> %1
echo CREATE TABLESPACE TEMPORARY_DATA DATAFILE '%ORA_PATH%\database\Tmp1orcl.ora' SIZE 20M >> %1
echo DEFAULT STORAGE ( INITIAL 100K NEXT 100K MINEXTENTS 1 MAXEXTENTS 121 PCTINCREASE 0) TEMPORARY; >> %1
echo ALTER DATABASE DATAFILE '%ORA_PATH%\database\Tmp1orcl.ora' AUTOEXTEND ON; >> %1

echo REM **************TABLESPACE FOR INDEX********************* >> %1
echo CREATE TABLESPACE INDX DATAFILE '%ORA_PATH%\database\Indx1orcl.ora' SIZE 10M >> %1
echo DEFAULT STORAGE ( INITIAL 50K NEXT 50K MINEXTENTS 1 MAXEXTENTS 121 PCTINCREASE 1); >> %1
echo ALTER DATABASE DATAFILE '%ORA_PATH%\database\Indx1orcl.ora' AUTOEXTEND ON; >> %1

echo REM **** Creating two rollback segments **************** >> %1
echo CREATE PUBLIC ROLLBACK SEGMENT RB0 TABLESPACE "ROLLBACK_DATA" >> %1
echo STORAGE ( INITIAL 50K NEXT 50K MINEXTENTS 2 MAXEXTENTS 121 ); >> %1
echo CREATE PUBLIC ROLLBACK SEGMENT RB1 TABLESPACE "ROLLBACK_DATA" >> %1
echo STORAGE ( INITIAL 50K NEXT 50K MINEXTENTS 2 MAXEXTENTS 121 ); >> %1
echo ALTER ROLLBACK SEGMENT "RB0" ONLINE; >> %1
echo ALTER ROLLBACK SEGMENT "RB1" ONLINE; >> %1
echo alter user sys temporary tablespace TEMPORARY_DATA; >> %1
echo alter user system default tablespace USER_DATA; >> %1
echo alter rollback segment "SYSROL" offline; >> %1
echo spool off >> %1
