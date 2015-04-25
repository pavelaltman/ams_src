delete op_items where (op_id,op_subid,code) in
 (select op_id,op_subid,code from
   (select * from op_items,sums
           where item_sum_id=sum_id(+))
    where sum_id is NULL) ;

delete cor_items where (op_id,op_subid,d_code,c_code) in
 (select op_id,op_subid,d_code,c_code from
   (select * from cor_items,sums
           where item_sum_id=sum_id(+))
    where sum_id is NULL) ;

exit
