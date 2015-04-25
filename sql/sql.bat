@set CONN_STR=S
@REM ------------------------------------------------------------
@REM Выполнение файла в среде с установленным nls_lang = .RU8PC866
@title EXEC file %3 from user %1
@if A%1==A goto ERROR_USAGE
@if A%2==A goto ERROR_USAGE
@if A%3==A goto ERROR_USAGE
@set nls_lang=.RU8PC866
@echo EXEC file %3 from user %1 
pause
@plus80 %1/%2@%CONN_STR% @%3 %4 %5 %6 %7 %8 %9
pause
goto END_

:ERROR_USAGE
    @ECHO Error:
    @ECHO Usage: SQL.BAT BASE_USER_NAME BASE_USER_PWD EXEC_file.SQL

:END_
