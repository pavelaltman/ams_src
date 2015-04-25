set ORA_PATH=D:\orant

@REM -------------------------------------------------------------
@REM -------------------------------------------------------------
@REM -- Дальше не редактировать!!!!!!!
@REM -- Запускать с параметром в командной строке BEGIN
@REM -------------------------------------------------------------
@REM -------------------------------------------------------------
@if not A%1==ABEGIN goto ERROR_USAGE
@set ORACLE_SID=orcl
@set nls_lang=.RU8PC866
echo off
call make_ora_init.bat
call make_orcl1run.bat orcl1run.sql
call make_orclrun.bat orclrun.sql
copy orc1.ora %ORA_PATH%\database\initorcl.ora
del orc1.ora
echo on

%ORA_PATH%\bin\oradim80 -new -sid orcl -intpwd sys -startmode auto -pfile %ORA_PATH%\database\initorcl.ora
%ORA_PATH%\bin\oradim80 -startup -sid orcl -starttype srvc,inst -usrpwd sys -pfile %ORA_PATH%\database\initorcl.ora
%ORA_PATH%\bin\svrmgr30 @orclrun.sql
%ORA_PATH%\bin\svrmgr30 @orcl1run.sql
@echo.
@echo.
@echo.
@echo.
@echo     После того как файл отработает надо будет 
@echo     в строке
@echo.
@echo        SQL> _
@echo.
@echo     набрать  EXIT и нажать ENTER
@echo.
@echo.
@pause
plus80 system/manager @setp.sql %ORA_PATH%

GOTO ENDFILE

:ERROR_USAGE
@echo.
@echo.
@echo.
@echo.
@echo.
@echo.
@echo.
@echo  Предназначено для создания базы данных ORACLE c sid=ORCL.
@echo  Запускать этот файл уже после инсталляции ORACLE без базы данных.
@echo  После инсталяции ORACLE и перед запуском этого файла
@echo       необходимо
@echo           1) Перезагрузить компьютер
@echo           2) Убедиться что в первой строке стоит правильный путь к ORACLE 
@echo   Подробное описание в файле Инсталляция.doc
@echo.
@echo   Правильный вызов:
@echo       EDIT_AND_RUN_ME.bat BEGIN
@echo.
@echo.
@echo.
@echo.
@pause


:ENDFILE