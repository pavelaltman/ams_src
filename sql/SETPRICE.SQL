DECLARE
   CURSOR cu IS SELECT * FROM TOVARY ;
   MAX_C NUMBER ;
BEGIN
  FOR R IN cu LOOP
    SELECT MAX(���_�_���) INTO MAX_C FROM TOVARY WHERE ARTICLE=R.ARTICLE ;
    UPDATE TOVARY SET ���_�_���=MAX_C WHERE ARTICLE=R.ARTICLE AND VER=R.VER ;
    commit ;
  END LOOP ;
END ;
/

exit
