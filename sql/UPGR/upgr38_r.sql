----------------------------------------------------------------
-- Заглушка для функций, генерируемые ч/з AmsWinAdmin
----------------------------------------------------------------
create or replace function
GetDbShortInfo( pDbName varchar2, pArt varchar2, pVer number) return varchar2 as
begin
    return '...';
end;
/
show errors;
grant execute on GetDbShortInfo to &1.ROLE;
exit;