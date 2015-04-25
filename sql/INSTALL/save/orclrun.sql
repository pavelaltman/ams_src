spool E:\orant\database\spoolmain
set echo on
connect INTERNAL/sys
startup nomount pfile=E:\orant\database\initorcl.ora
CREATE DATABASE orcl
LOGFILE 'E:\orant\database\logorcl1.ora' SIZE 1024K,
    'E:\orant\database\logorcl2.ora' SIZE 1024K
MAXLOGFILES 32 
MAXLOGMEMBERS 2
MAXLOGHISTORY 1
DATAFILE 'E:\orant\database\Sys1orcl.ora' SIZE 50M
MAXDATAFILES 254
MAXINSTANCES 1
CHARACTER SET CL8MSWIN1251
NATIONAL CHARACTER SET CL8MSWIN1251;
spool off
