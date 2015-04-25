create or replace procedure
SetDeadDateToday as
begin
    update USER_OPTIONS set DEAD_DATE= TRUNC(SYSDATE, 'DD')
    where USERNAME=USER;
end;
/
show errors;
grant execute on SetDeadDateToday to &1.ROLE;
exit;

