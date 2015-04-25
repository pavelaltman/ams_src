/* ᮧ����� ���짮��⥫� � �।��।������� ஫�� */

GRANT CONNECT,RESOURCE,UNLIMITED TABLESPACE TO &1. IDENTIFIED BY &2. ;
ALTER USER &1. DEFAULT TABLESPACE USER_DATA ;
ALTER USER &1. TEMPORARY TABLESPACE TEMPORARY_DATA ;

-- ���� ��� ࠡ��� � AMS �奬��
grant AMS_BUCH_ROLE to &1.;

grant create user to &1.;
grant create role to &1.;
grant drop any role to &1.;
grant drop user to &1.;
grant select any table to &1.;
grant create session to &1. with admin option;
grant create table to &1. with admin option;
grant create trigger to &1. with admin option;
grant create procedure to &1. with admin option;
grant alter rollback segment to &1. with admin option ;
grant select on SYS.V_$SESSION to &1. with grant option ;
create role &1.role;
create role &1.super;
create role &1._InputOper;
create role &1._EXIM;
create role &1._InpRems;
create role &1._OperList;
create role &1._Unapp;
create role &1._History;
create role &1._Periods;
create role &1._DeadDate;
create role &1._ViewDate;
create role &1._NegRems;
grant &1.role to &1. with admin option ;
grant &1.super to &1. with admin option;
grant &1._InputOper to &1. with admin option ;
grant &1._EXIM to &1. with admin option ;
grant &1._InpRems to &1. with admin option ;
grant &1._OperList to &1. with admin option ;
grant &1._Unapp to &1. with admin option ;
grant &1._History to &1. with admin option ;
grant &1._Periods to &1. with admin option ;
grant &1._DeadDate to &1. with admin option ;
grant &1._ViewDate to &1. with admin option ;
grant &1._NegRems to &1. with admin option ;
revoke &1.role from SYS  ;
revoke &1.super from SYS ;
revoke &1._InputOper from SYS  ;
revoke &1._EXIM from SYS  ;
revoke &1._InpRems from SYS  ;
revoke &1._OperList from SYS  ;
revoke &1._Unapp from SYS  ;
revoke &1._History from SYS  ;
revoke &1._Periods from SYS  ;
revoke &1._DeadDate from SYS  ;
revoke &1._ViewDate from SYS  ;
revoke &1._NegRems from SYS  ;
exit