@echo Cохранения БУХГАЛТЕРИИ

@if A%1==A goto ERROR_USAGE
@if A%2==A goto ERROR_USAGE
@set NLS_LANG=.RU8PC866
exp80 userid=%1/%2 file=%1.dmp

goto END_

:ERROR_USAGE
@Echo Правильный вызов 
@echo   import BASE_USER_NAME BASE_USER_PWD

:END_