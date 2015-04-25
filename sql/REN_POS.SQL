CREATE OR REPLACE
PROCEDURE REN_POS_TOVARY(OLDART VARCHAR2,OLDVER NUMBER,
                         NEWART VARCHAR2,NEWVER NUMBER) AS
CURSOR ANALCURS IS
  SELECT o.* FROM OBJS p,OBJS o WHERE p.name='TOVARY' AND o.PARENT=p.OID AND o.OTYPE=5 ;

CURSOR DBCURS IS
  SELECT o.name,realname FROM OBJS p,OBJS o,DBSTR WHERE p.name='TOVARY' AND RDBID=p.OID AND DBID=o.OID ;

cid      NUMBER ;
sql_res  NUMBER ;
func_str VARCHAR2(300) ;

BEGIN
   cid:=DBMS_SQL.OPEN_CURSOR ;

   INSERT INTO TOVARY
     SELECT CATID,NEWART,NEWVER,çÄàåÖçéÇÄçàÖ,ñÖçÄáÄä,ñÖçÄéíè,äéã_ÅÄí
     FROM TOVARY WHERE ARTICLE=OLDART AND VER=OLDVER ;

   FOR AN IN ANALCURS LOOP
      func_str:='UPDATE ' || AN.name || ' SET AARTICLE=:NEWART, AVER=:NEWVER WHERE AARTICLE=:OLDART AND AVER=:OLDVER' ;
      DBMS_SQL.PARSE(cid,func_str,dbms_sql.v7);
      dbms_sql.bind_variable(cid,':OLDART',OLDART);
      dbms_sql.bind_variable(cid,':OLDVER',OLDVER);
      dbms_sql.bind_variable(cid,':NEWART',NEWART);
      dbms_sql.bind_variable(cid,':NEWVER',NEWVER);
      sql_res:=dbms_sql.execute(cid) ;
   END LOOP ;

   FOR DB IN DBCURS LOOP
      func_str:='UPDATE ' || DB.name || ' SET '+DB.REALNAME+'=:NEWART, '+DB.REALNAME+'_V=:NEWVER WHERE '+DB.REALNAME+'=:OLDART AND '+DB.REALNAME+'=:OLDVER' ;
      DBMS_SQL.PARSE(cid,func_str,dbms_sql.v7);
      dbms_sql.bind_variable(cid,':OLDART',OLDART);
      dbms_sql.bind_variable(cid,':OLDVER',OLDVER);
      dbms_sql.bind_variable(cid,':NEWART',OLDART);
      dbms_sql.bind_variable(cid,':NEWVER',OLDVER);
      sql_res:=dbms_sql.execute(cid) ;
   END LOOP ;

   DELETE TOVARY WHERE ARTICLE=OLDART AND VER=OLDVER ;

   DBMS_SQL.CLOSE_CURSOR(cid);
END;
/

show errors

GRANT execute on REN_POS_TOVARY to MAYROLE ;

exit
