DELETE corturns ;

INSERT INTO unapp_cor_items
    SELECT i.op_id,i.op_subid,d_code,c_code,point_date,0
    FROM cor_items i,opers o,cps
    WHERE op_date<point_date AND i.op_id=o.op_id AND i.op_subid=o.op_subid ;

commit ;

BEGIN
 bookkern.ApplyUndoAll ;
END ;
/

