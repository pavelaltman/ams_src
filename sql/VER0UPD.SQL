-- Функции для проверки 0-версий

--CREATE OR replace FUNCTION VER0INS_TOVARY(newart VARCHAR2, newver NUMBER)
CREATE OR replace FUNCTION VER0NEW_TOVARY(newart VARCHAR2, newver NUMBER)
 RETURN NUMBER AS 
  it NUMBER;
  vn NUMBER ;

BEGIN
  it:=1;
--  IF newver!=0 OR newver NOT IS NULL THEN 
  IF newver!=0 THEN 
    SELECT COUNT(*) INTO VN FROM tovary WHERE article=newart AND ver=0 ;
    IF vn=0 THEN it:=0;
    END IF ;
  END IF ;
  RETURN it;
END ;
/

CREATE OR replace FUNCTION VER0UPD_TOVARY(newart VARCHAR2, newver NUMBER, oldart VARCHAR2, oldver NUMBER)
 RETURN NUMBER AS 
  it NUMBER;
  vo NUMBER ;
  vn NUMBER ;
BEGIN
  it:=1;
  IF newart=oldart THEN 
--    IF newver!=0 OR newver NOT IS NULL THEN 
    IF newver!=0 THEN 
      SELECT COUNT(*) INTO VN FROM tovary WHERE article=newart AND ver=0 ;
      IF vn=0 THEN it:=0;
      END IF ;
    END IF ;
  ELSE
    SELECT COUNT(*) INTO VN FROM tovary WHERE article=newart AND ver=0 ;
--    SELECT COUNT(*) INTO VO FROM tovary WHERE article=oldart AND ver=0 ;
    SELECT COUNT(*) INTO VO FROM tovary WHERE article=oldart ;
    IF oldver=0 AND vo>1 THEN it:=0;
    ELSE
--      IF newver!=0 OR newver NOT IS NULL THEN 
      IF newver!=0 THEN 
        IF vn=0 THEN it:=0;
        END IF ;
      END IF ;
    END IF ;
  END IF ;
  RETURN it;

END ;
/

CREATE OR replace FUNCTION VER0DEL_TOVARY(oldart VARCHAR2, oldver NUMBER)
 RETURN NUMBER AS 
  it NUMBER;
  vo NUMBER ;

BEGIN
  it:=1;
  IF oldver=0 THEN 
    SELECT COUNT(*) INTO VO FROM tovary WHERE article=oldart AND ver!=0 ;
    IF vo>0 THEN it:=0;
    END IF ;
  END IF ;
  RETURN it;
END ;
/

grant execute on ver0new_tovary to &1.role;
grant execute on ver0upd_tovary to &1.role;
grant execute on ver0del_tovary to &1.role;

show errors

exit
