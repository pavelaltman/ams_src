@rem ��� ����⠭������� �� AMS ��壠��ਨ
@if A%1== A goto END_VAL
@if A%2== A goto END_VAL
@if A%3== A goto END_VAL
@if A%4== A goto END_VAL
@if A%5== A goto END_VAL

@rem %1 - ��� �����������, ������ ����⠭��������
@rem %2 - ��஫� ��� ���짮��⥫� %1
@rem %3 - ��஫� ��� SYS
@rem %4 - ���� � SQL - 䠩���
@rem %5 - ��� �� ���ண� ����⠭�������� ��壠����

@call sql sys %3 %4\del_user %1
@call sql sys %3 %4\admin %1 %2

if A%6== A goto FROM_RAR

del creroles_%5.sql
del updprivs_%5.sql
del user_roles_%5.sql
del %5.dmp

move %6\creroles_%5.sql .
move %6\updprivs_%5.sql .
move %6\user_roles_%5.sql .
move %6\%5.dmp .
goto CONT1

:FROM_RAR
rar x %5.rar creroles_%5.sql updprivs_%5.sql %5.dmp user_roles_%5.sql

:CONT1

call sql %1 %2 creroles_%5.sql %1


rem @SET nls_lang=RUSSIAN_CIS.CL8MSWIN1251
@if A%5==A%1 goto ONE2ONE
@imp80 userid=%1/%2@%CONN_STR% file=%5.dmp FULL=y
goto COMM1
:ONE2ONE
@imp80 userid=%1/%2@%CONN_STR% file=%5.dmp
:COMM1
@call sql %1 %2 %4\rights  %1
@call sql %1 %2 %4\crefuncs
@call sql %1 %2 %4\creviews
@call sql %1 %2 %4\grant   %1

@call sql %1 %2 updprivs_%5  %1 %5

@ECHO.
@ECHO.
@ECHO.
@ECHO.
@ECHO ����室��� �������� amsadmin.exe c ��ࠬ��஬ addusers
@ECHO ��᫥ 祣� ����� �������� USER_ROLES.sql 
@ECHO  (�뤠�� ���짮��⥫� ⠪�� �� ஫� ��� ��� ����� ࠭��)
@ECHO.
@ECHO.
@ECHO.
@ECHO.
@del creroles_%5.sql
@del updprivs_%5.sql
@del user_roles_%5.sql
@del %5.dmp
pause
goto END_

:ERROR_USAGE
@Echo �ࠢ���� �맮�
@echo   recreate BASE_USER_NAME BASE_USER_PWD SYS_PWD
 



:END_VAL


