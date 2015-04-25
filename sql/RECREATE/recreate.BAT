@echo Для восстановления сохраненной БУХГАЛТЕРИИ

@if A%1==A goto ERROR_USAGE
@if A%2==A goto ERROR_USAGE
@if A%3==A goto ERROR_USAGE

@call sql sys %3 ..\del_user %1
@call sql sys %3 admin %1 %2

@if A%4==A goto NEXT1
@imp80 userid=%1/%2 file=%4.dmp FULL=Y
@goto NEXT2

:NEXT1
@imp80 userid=%1/%2 file=%1.dmp

:NEXT2
@call sql %1 %2 ..\rights  %1
@call sql %1 %2 ..\crefuncs 
@call sql %1 %2 ..\creviews
@call sql %1 %2 ..\grant  %1  

@if A%4==A goto NEXT3
@call sql %1 %2 upd_objpriv %1 %4
:NEXT3



@ECHO please exec AMS_ADMIN WITH addusers parameter 
goto END_

:ERROR_USAGE
@Echo Правильный вызов 
@echo   recreate BASE_USER_NAME BASE_USER_PWD SYS_PWD FROM_USER

:END_