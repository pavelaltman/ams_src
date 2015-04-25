alter table opers drop constraint opers_cond ;
alter table opers add CONSTRAINT opers_cond FOREIGN KEY (cond_id,op_subid) REFERENCES conds (cond_id,cond_subid) ;
