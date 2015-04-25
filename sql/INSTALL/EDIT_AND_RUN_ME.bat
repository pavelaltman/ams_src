set ORA_PATH=D:\orant

@REM -------------------------------------------------------------
@REM -------------------------------------------------------------
@REM -- ����� �� ।���஢���!!!!!!!
@REM -- ����᪠�� � ��ࠬ��஬ � ��������� ��ப� BEGIN
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
@echo     ��᫥ ⮣� ��� 䠩� ��ࠡ�⠥� ���� �㤥� 
@echo     � ��ப�
@echo.
@echo        SQL> _
@echo.
@echo     ������  EXIT � ������ ENTER
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
@echo  �।�����祭� ��� ᮧ����� ���� ������ ORACLE c sid=ORCL.
@echo  ����᪠�� ��� 䠩� 㦥 ��᫥ ���⠫��樨 ORACLE ��� ���� ������.
@echo  ��᫥ ���⠫�樨 ORACLE � ��। ����᪮� �⮣� 䠩��
@echo       ����室���
@echo           1) ��१���㧨�� ��������
@echo           2) ��������� �� � ��ࢮ� ��ப� �⮨� �ࠢ���� ���� � ORACLE 
@echo   ���஡��� ���ᠭ�� � 䠩�� ���⠫����.doc
@echo.
@echo   �ࠢ���� �맮�:
@echo       EDIT_AND_RUN_ME.bat BEGIN
@echo.
@echo.
@echo.
@echo.
@pause


:ENDFILE