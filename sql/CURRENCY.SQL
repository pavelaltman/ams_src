-- ��楤�� ��� ����祭�� ���� ������ � 㪠����� ����� �� �㦭�� ����
CREATE OR REPLACE PROCEDURE GetKurs(val_kod VARCHAR2,dat DATE,ret OUT NUMBER) AS
 CURSOR d_rates IS
   SELECT ����,����� FROM VALUTY WHERE article=val_kod ORDER BY ���� ;
BEGIN
 ret:=0 ;

 FOR rate IN d_rates LOOP
   IF rate.����>dat THEN RETURN ;
   ELSE ret:=rate.����� ;
   END IF ;
 END LOOP ;

 RETURN ;
END ;
/
