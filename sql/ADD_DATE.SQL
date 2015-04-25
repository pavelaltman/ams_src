alter table &1. add (op_date DATE) ;

create unique index d_&1. on &1. (aarticle,aver,op_date,sumid) ;

DECLARE
   CURSOR cu IS SELECT a.sumid,a.aarticle,a.aver,i.op_date,i.code FROM &1. a,op_items i
		WHERE a.sumid=i.item_sum_id ;
BEGIN
  FOR R IN cu LOOP
      UPDATE &1. SET op_date=R.op_date
      WHERE sumid=R.sumid and aarticle=R.aarticle and aver=R.aver ;
      commit ;
  END LOOP ;
END ;
/

exit
