UPDATE OBJPRIV SET RUNAME='&1.ROLE'       WHERE RUNAME='&2.ROLE';
UPDATE OBJPRIV SET RUNAME='&1.SUPER'      WHERE RUNAME='&2.SUPER';
UPDATE OBJPRIV SET RUNAME='&1._INPUTOPER' WHERE RUNAME='&2._INPUTOPER';
UPDATE OBJPRIV SET RUNAME='&1._EXIM'      WHERE RUNAME='&2._EXIM';
UPDATE OBJPRIV SET RUNAME='&1._INPREMS'   WHERE RUNAME='&2._INPREMS';
UPDATE OBJPRIV SET RUNAME='&1._OPERLIST'  WHERE RUNAME='&2._OPERLIST';
UPDATE OBJPRIV SET RUNAME='&1._UNAPP'     WHERE RUNAME='&2._UNAPP';
UPDATE OBJPRIV SET RUNAME='&1._HISTORY'   WHERE RUNAME='&2._HISTORY';
UPDATE OBJPRIV SET RUNAME='&1._PERIODS'   WHERE RUNAME='&2._PERIODS';
UPDATE OBJPRIV SET RUNAME='&1._DEADDATE'  WHERE RUNAME='&2._DEADDATE';
UPDATE OBJPRIV SET RUNAME='&1._VIEWDATE'  WHERE RUNAME='&2._VIEWDATE';
UPDATE OBJPRIV SET RUNAME='&1._NEGREMS'   WHERE RUNAME='&2._NEGREMS';

set serveroutput on ;

DECLARE
 cid NUMBER ;
 sql_res NUMBER ;
 anal_name VARCHAR2(30) ;
 func_str  VARCHAR2(100) ;
 CURSOR curs IS
    select * from dba_role_privs where INSTR(granted_role,'&2.')>0 
                                       AND ADMIN_OPTION='NO' ;
BEGIN
  FOR r IN curs LOOP
     cid:=DBMS_SQL.OPEN_CURSOR;

     func_str:='GRANT ' || REPLACE(r.granted_role,'&2.','&1.') || ' TO ' || r.grantee ;
     DBMS_SQL.PARSE(cid,func_str,dbms_sql.v7);
     sql_res:=dbms_sql.execute(cid);

     func_str:='UPDATE OBJPRIV SET RUNAME=''' || REPLACE(r.granted_role,'&2.','&1.') || ''' WHERE RUNAME=''' || r.granted_role || '''' ;

     dbms_output.put_line(func_str) ;
     commit ;

     DBMS_SQL.PARSE(cid,func_str,dbms_sql.v7);
     sql_res:=dbms_sql.execute(cid);

     DBMS_SQL.CLOSE_CURSOR(cid);
  END LOOP ;
END ;
/
