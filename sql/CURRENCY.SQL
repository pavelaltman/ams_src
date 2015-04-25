-- Процедура для получения курса валюты с указанным кодом на нужную дату
CREATE OR REPLACE PROCEDURE GetKurs(val_kod VARCHAR2,dat DATE,ret OUT NUMBER) AS
 CURSOR d_rates IS
   SELECT ДАТА,БКУРС FROM VALUTY WHERE article=val_kod ORDER BY ДАТА ;
BEGIN
 ret:=0 ;

 FOR rate IN d_rates LOOP
   IF rate.ДАТА>dat THEN RETURN ;
   ELSE ret:=rate.БКУРС ;
   END IF ;
 END LOOP ;

 RETURN ;
END ;
/
