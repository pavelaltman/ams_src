DECLARE
   CURSOR cu IS SELECT op_id,op_subid,doc_type,doc_num,doc_num_v FROM opers ;
BEGIN
  FOR R IN cu LOOP
      UPDATE op_items SET doc_type=R.doc_type, doc_num=R.doc_num, doc_num_v=R.doc_num_v
      WHERE op_id=R.op_id and op_subid=R.op_subid ;
      commit ;
  END LOOP ;
END ;
/

drop index items_code_ind ;
CREATE INDEX items_code_ind ON op_items(code,op_date,doc_type) ;
