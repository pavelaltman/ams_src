-- Файл для забора прав выданных при копировании
--   из бухгалтерии в бухгалтерию
-- 2 параметра
--   1 - Имя бухгалтерии откуда копируем
--   2 - Имя бухгалтерии куда копируем
-- Запускать от имени базового пользователя куда копировали !!!
--
--  Пример
-----------------------------------------------------------------
-- sql TO_BUH PASSWORD_TO COPY_RI_REVOKE.sql FROM_BUH TO_BUH 
-----------------------------------------------------------------

revoke &2.SUPER from &1.;
revoke execute on InsertObj from &1.;
revoke execute on InsertCat from &1.;
revoke execute on GetParentAcc from &1.;
revoke execute on MakeDBTree from &1.;
revoke insert on CONDS from &1.;
revoke insert on COND_ITEMS from &1.;
revoke insert on CONSTS from &1.;
revoke insert,update on DBSTR from &1.;
revoke update on OBJS from &1.;
revoke insert on DBOPT from &1.;
revoke insert on DBOPTOV from &1.;
revoke insert on DBADDINFO from &1.;
revoke create table from &1.;
exit;
