alter rollback segment myroll online ;

commit ;
set transaction use rollback segment myroll ;
DELETE cor_items ;
COMMIT ;

begin
 commit ;
 set transaction use rollback segment myroll ;
 bookkern.AddAllCorItems ;
 commit ;
end ;
/

COMMIT ;
set transaction use rollback segment myroll ;

delete cor_turns ;
COMMIT ;
set transaction use rollback segment myroll ;

INSERT INTO unapp_opers
    SELECT op_id,op_subid,0,0
    FROM opers ;

INSERT INTO unapp_cor_items
    SELECT i.op_id,i.op_subid,d_code,c_code,point_date
    FROM cor_items i,opers o,cps
    WHERE op_date<point_date AND i.op_id=o.op_id AND i.op_subid=o.op_subid
          AND o.post=0 ;

COMMIT ;

declare
 res number ;
BEGIN
 res:=bookkern.ApplyUndoAll(0,100000) ;
END ;
/

exit
