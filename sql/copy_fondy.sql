alter table an_fondy add (op_date DATE) ;
insert into fondy select owncat,article,ver,€ˆŒ from init.fondy,dbcat,objs where article!=' ' and objs.name='FONDY' AND objs.oid=dbcat.dbid ;


