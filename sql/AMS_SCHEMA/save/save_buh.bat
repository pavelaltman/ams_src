@if A%1==A GOTO END_VAL
@if A%2==A GOTO END_VAL
@if A%3==A GOTO END_VAL
@if A%4==A GOTO END_VAL
@REM Сохранение AMS бухгалтерии 
rem %1 - Имя бухгалтерии 
rem %2 - пароль 
rem %3 - строка связи 
rem %4 - Путь по которому лежат *.prg файлы 

@set nls_lang=.RU8PC866
@exp80 %1/%2@%3 FILE=%1.dmp

del %1_4.rar
move %1_3.rar %1_4.rar
move %1_2.rar %1_3.rar
move %1_1.rar %1_2.rar
move %1.rar %1_1.rar

rar a %1.rar %1.dmp creroles_%1.sql updprivs_%1.sql user_roles_%1.sql
rar a %1.rar %4 -r
pause

:END_VAL