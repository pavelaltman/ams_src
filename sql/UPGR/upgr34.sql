CREATE OR REPLACE PROCEDURE
CreateAndCopyAnSum
        (
            pAnalId in number,
            pNewId in out number,
            pOldId in number,
            pDeleteOld in number default 0 ) AS
BEGIN
-----------------------------------------------------------------------------------------
-- Создает временную сумму, копирует и удаляет старую 
-----------------------------------------------------------------------------------------
  if (pNewId is NULL) or (pNewId = 0) then
    SELECT SumSeq.NEXTVAL INTO pNewId FROM DUAL;
    insert into SUMS
        (SUM_ID, VALUE) values (pNewId, 0); 
    commit;
  end if;

  CopyAnalSum(pAnalId, pNewId, pOldId);
  if (pDeleteOld =1) then
    delete from SUMS where SUM_ID = pOldId;
  end if;
END ;
/
grant execute on CreateAndCopyAnSum to &1.ROLE;
show errors;
exit;
