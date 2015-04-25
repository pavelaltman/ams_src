CREATE OR REPLACE FUNCTION AnalInfo(oper_id NUMBER, oper_subid NUMBER,acode NUMBER) RETURN VARCHAR2 AS
 ret VARCHAR2(100) ;
 anal_id NUMBER ;
 anal_name VARCHAR2(30) ;
 sum_id NUMBER ;
 num_pos NUMBER ;
 temp VARCHAR2(100) ;
 ART  VARCHAR2(30) ;
 V    NUMBER ;
BEGIN
 ret:='' ;

 SELECT anal_id INTO anal_id FROM accs WHERE code=acode ;

 IF anal_id>0 THEN
   SELECT name INTO anal_name FROM objs WHERE oid=anal_id ;
   SELECT item_sum_id INTO sum_id FROM op_items WHERE op_id=oper_id AND op_subid=oper_subid AND code=acode ;

   IF anal_name='AN_POKUP' THEN
      SELECT count(*) INTO num_pos FROM AN_POKUP WHERE sumid=sum_id ;
      IF num_pos>2 THEN
         ret:=TO_CHAR(num_pos-1) || ' наименований ' ;
      ELSIF num_pos=2 THEN
         SELECT AARTICLE,AVER INTO ART,V FROM AN_POKUP
           WHERE SUMID=sum_id AND AARTICLE!=' ' ;
         SELECT НАИМ INTO temp FROM POKUPCI
           WHERE ARTICLE=ART AND VER=V ;
         ret:=ret || temp || ' ' ;
      END IF ;
   ELSIF anal_name='AN_POSTACH' THEN
      SELECT count(*) INTO num_pos FROM AN_POSTACH WHERE sumid=sum_id ;
      IF num_pos>2 THEN
         ret:=TO_CHAR(num_pos-1) || ' наименований ' ;
      ELSIF num_pos=2 THEN
         SELECT AARTICLE,AVER INTO ART,V FROM AN_POSTACH
           WHERE SUMID=sum_id AND AARTICLE!=' ' ;
         SELECT НАИМ INTO temp FROM POSTACH
           WHERE ARTICLE=ART AND VER=V ;
         ret:=ret || temp || ' ' ;
      END IF ;
   ELSIF anal_name='AN_TOV' THEN
      SELECT count(*) INTO num_pos FROM AN_TOV WHERE sumid=sum_id ;
      IF num_pos>2 THEN
         ret:=TO_CHAR(num_pos-1) || ' наименований ' ;
      ELSIF num_pos=2 THEN
         SELECT AARTICLE,AVER INTO ART,V FROM AN_TOV
           WHERE SUMID=sum_id AND AARTICLE!=' ' ;
         SELECT НАИМ INTO temp FROM TOVARY
           WHERE ARTICLE=ART AND VER=V ;
         ret:=ret || temp || ' ' ;
      END IF ;

      -- аналитика
      SELECT ОТП_СУММА INTO temp FROM AN_TOV WHERE SUMID=sum_id AND AARTICLE=' ' ;
      ret:=ret || temp || ' ' ;

   END IF ;
 END IF ;

 return ret ;
END ;
/
show errors
exit

