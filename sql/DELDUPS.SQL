DECLARE
   CURSOR cu IS SELECT * FROM TOVARY FOR UPDATE ;
   a NUMBER ;
BEGIN
  FOR R IN cu LOOP
    BEGIN
       DELETE TOVARY WHERE article=R.article AND ver!=
              (select max(ver) from tovary where article=r.article) ;
    EXCEPTION
       WHEN OTHERS THEN
            a:=1 ;
    END ;
    commit ;
  END LOOP ;
END ;
/

exit
