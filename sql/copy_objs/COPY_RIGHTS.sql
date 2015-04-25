-- Файл для копирования из бухгалтерии в бухгалтерию
-- 2 параметра
--   1 - Имя бухгалтерии откуда копируем
--   2 - Имя бухгалтерии куда копируем
-- Запускать от имени базового пользователя куда копируем !!!
--
--  Пример
-----------------------------------------------------------------
-- sql.bat TO_BUH PASSWORD_TO COPY_RIGHTS.sql FROM_BUH TO_BUH 
-----------------------------------------------------------------

grant &2.SUPER to &1.;
grant execute on &2..InsertObj to &1.;
grant execute on &2..InsertCat to &1.;
grant execute on &2..GetParentAcc to &1.;
grant execute on &2..MakeDBTree to &1.;
grant insert on &2..CONDS to &1.;
grant insert on &2..COND_ITEMS to &1.;
grant insert on &2..CONSTS to &1.;
grant update on &2..OBJS to &1.;
grant insert on &2..DBOPT to &1.;
grant insert on &2..DBOPTOV to &1.;
grant insert on &2..DBADDINFO to &1.;
grant create table to &1.;
grant insert,update on &2..DBSTR to &1.;
grant execute on &2..InsertDBCat to &1.;

exit;
