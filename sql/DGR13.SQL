alter table opers drop CONSTRAINT opers_doc_num ;

alter table opers add
       (
        CONSTRAINT opers_doc_num UNIQUE (status,doc_type,doc_num,doc_num_v)
       ) ;

UPDATE opers SET range_date=NULL ;

drop table temp ;
create table temp as
  select * from opers ;
alter table op_items drop constraint op_items_op_id ;
alter table cor_items drop constraint cor_items_op_id ;
drop table opers ;

create table opers as select op_id,op_subid,status,post,storno,text,
                             doc_type,doc_num,doc_num_v,op_date,cond_id,
                             edited FROM temp ;
alter table opers add
       (
        CONSTRAINT opers_prim PRIMARY KEY (op_id,op_subid),
        CONSTRAINT opers_doc FOREIGN KEY (doc_type) REFERENCES objs (name),
        CONSTRAINT opers_doc_num UNIQUE (status,doc_type,doc_num,doc_num_v),
        CONSTRAINT opers_cond FOREIGN KEY (cond_id,op_subid) REFERENCES conds (cond_id,cond_subid)
       ) ;
alter table op_items add
      (
       CONSTRAINT op_items_op_id FOREIGN KEY (op_id,op_subid) REFERENCES opers (op_id,op_subid)
      ) ;
alter table cor_items add
      (
       CONSTRAINT cor_items_op_id FOREIGN KEY (op_id,op_subid) REFERENCES opers (op_id,op_subid)
      ) ;
grant select,insert,update,delete on opers to &1.role;
