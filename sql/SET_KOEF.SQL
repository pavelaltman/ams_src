declare
  cursor curs is
     select ARTICLE,AVG(����_������) AS KOEF FROM TOVARY 
            WHERE ����_������>0 GROUP BY ARTICLE ;
begin
  for r in curs loop
    UPDATE tovary SET ����_������=r.KOEF WHERE ARTICLE=r.ARTICLE AND (����_������=0 OR ����_������ IS NULL) ;
  end loop ;
end ;
/
