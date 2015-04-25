REM echo off

REM Запустить этот файл дя подготовки к копированию
REM объектов из одной бухгалтерии в другую
REM c 4 параметрами

REM   ПОДГ_КОПИР.BAT ИМЯ_БУХГАЛТЕРИИ_ИЗ пароль_из ИМЯ_БУХГ_КУДА пароль_куда

if A%1==A goto ERROR_USAGE
if A%2==A goto ERROR_USAGE
if A%3==A goto ERROR_USAGE
if A%4==A goto ERROR_USAGE

call sql.bat %1 %2 UPGR_V_2.sql
call sql.bat %3 %4 COPY_RIGHTS.sql %1 %3
goto END_

:ERROR_USAGE
echo Ошибка запуска правильный запуск
echo   ПОДГ_КОПИР.BAT ИМЯ_БУХГАЛТЕРИИ_ИЗ пароль_из ИМЯ_БУХГ_КУДА пароль_куда
pause

:END_
    
