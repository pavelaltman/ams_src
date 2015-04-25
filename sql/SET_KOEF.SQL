declare
  cursor curs is
     select ARTICLE,AVG(–…€_Žˆ‡‚) AS KOEF FROM TOVARY 
            WHERE –…€_Žˆ‡‚>0 GROUP BY ARTICLE ;
begin
  for r in curs loop
    UPDATE tovary SET –…€_Žˆ‡‚=r.KOEF WHERE ARTICLE=r.ARTICLE AND (–…€_Žˆ‡‚=0 OR –…€_Žˆ‡‚ IS NULL) ;
  end loop ;
end ;
/
