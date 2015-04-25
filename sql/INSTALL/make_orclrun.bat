echo spool %ORA_PATH%\database\spoolmain > %1
echo set echo on >> %1
echo connect INTERNAL/sys >> %1
echo startup nomount pfile=%ORA_PATH%\database\initorcl.ora >> %1
echo CREATE DATABASE orcl >> %1
echo LOGFILE '%ORA_PATH%\database\logorcl1.ora' SIZE 1024K, >> %1
echo             '%ORA_PATH%\database\logorcl2.ora' SIZE 1024K >> %1
echo MAXLOGFILES 32  >> %1
echo MAXLOGMEMBERS 2 >> %1
echo MAXLOGHISTORY 1 >> %1
echo DATAFILE '%ORA_PATH%\database\Sys1orcl.ora' SIZE 50M >> %1
echo MAXDATAFILES 254 >> %1
echo MAXINSTANCES 1 >> %1
echo CHARACTER SET CL8MSWIN1251 >> %1
echo NATIONAL CHARACTER SET CL8MSWIN1251; >> %1
echo spool off >> %1
