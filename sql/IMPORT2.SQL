ALTER SESSION SET nls_date_language=RUSSIAN ;
ALTER SESSION SET NLS_DATE_FORMAT = 'DD-MM-YY' ;

DROP SEQUENCE SumSeq ;
CREATE SEQUENCE SumSeq
INCREMENT BY 1
START WITH 200000
CACHE 100 ;

DROP SEQUENCE OperSeq ;
CREATE SEQUENCE OperSeq
INCREMENT BY 1
START WITH 200000
CACHE 20 ;

DROP SEQUENCE ObjSeq ;
CREATE SEQUENCE ObjSeq
INCREMENT BY 1
START WITH 300000
NOCACHE ;

DROP SEQUENCE DBCatSeq ;
CREATE SEQUENCE DBCatSeq
INCREMENT BY 1
START WITH 300000
NOCACHE ;

set serveroutput on ;

alter rollback segment myroll online ;

BEGIN
 dbms_output.put_line('Добавление месяцев') ;
 periods.BeginNextMonth ;
 periods.BeginNextMonth ;
 periods.BeginNextMonth ;
 periods.BeginNextMonth ;
 periods.BeginNextMonth ;
 periods.BeginNextMonth ;
 periods.BeginNextMonth ;
END ;
/

COMMIT ;
set transaction use rollback segment myroll ;
DELETE UNAPP_items ;
DELETE UNAPP_cor_items ;

DELETE op_items WHERE node>0 ;
COMMIT ;

begin
 dbms_output.put_line('Добавление родительских счетов в проводках') ;
 commit ;
 set transaction use rollback segment myroll ;
 bookkern.AddAllParentItems ;
 commit ;
end ;
/

DELETE cor_items ;
COMMIT ;

begin
 dbms_output.put_line('Добавление корреспондирующих элементов') ;
 commit ;
 set transaction use rollback segment myroll ;
 bookkern.AddAllCorItems ;
 commit ;
end ;
/


COMMIT ;
set transaction use rollback segment myroll ;

INSERT INTO unapp_opers
    SELECT op_id,op_subid,0,0
    FROM opers ;

INSERT INTO unapp_items
    SELECT op_id,op_subid,code,point_date
    FROM op_items,cps
    WHERE op_date<point_date ;

INSERT INTO unapp_cor_items
    SELECT i.op_id,i.op_subid,d_code,c_code,point_date
    FROM cor_items i,opers o,cps
    WHERE op_date<point_date AND i.op_id=o.op_id AND i.op_subid=o.op_subid
          AND o.post=0 ;

COMMIT ;

declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/
declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,1000) ;
END ;
/

DECLARE
   CURSOR accounts IS SELECT * FROM accs FOR UPDATE ;
   sub_num NUMBER ;
BEGIN
  FOR A IN accounts LOOP
      SELECT COUNT(*) INTO sub_num FROM accs WHERE parent=A.code ;
      UPDATE accs SET node=sub_num WHERE CURRENT OF accounts ;
  END LOOP ;
END ;
/

BEGIN
 UPDATE dbstr set disp_opt=def_opt ;
END ;
/

commit ;

