REM echo off

REM �������� ��� 䠩� �� �����⮢�� � ����஢����
REM ��ꥪ⮢ �� ����� ��壠��ਨ � �����
REM c 4 ��ࠬ��ࠬ�

REM   ����_�����.BAT ���_�����������_�� ��஫�_�� ���_����_���� ��஫�_�㤠

if A%1==A goto ERROR_USAGE
if A%2==A goto ERROR_USAGE
if A%3==A goto ERROR_USAGE
if A%4==A goto ERROR_USAGE

call sql.bat %1 %2 UPGR_V_2.sql
call sql.bat %3 %4 COPY_RIGHTS.sql %1 %3
goto END_

:ERROR_USAGE
echo �訡�� ����᪠ �ࠢ���� �����
echo   ����_�����.BAT ���_�����������_�� ��஫�_�� ���_����_���� ��஫�_�㤠
pause

:END_
    
