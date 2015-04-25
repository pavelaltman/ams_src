declare 
 res number ;
begin
 res:=bookkern.ApplyUndoAllDead(10000) ;
end ;
/
declare 
 res number ;
begin
 res:=bookkern.ApplyUndoAllDead(10000) ;
end ;
/
declare 
 res number ;
begin
 res:=bookkern.ApplyUndoAllDead(10000) ;
end ;
/
declare 
 res number ;
begin
 res:=bookkern.ApplyUndoAllDead(10000) ;
end ;
/
declare 
 res number ;
begin
 res:=bookkern.ApplyUndoAllDead(10000) ;
end ;
/
declare 
 res number ;
begin
 res:=bookkern.ApplyUndoAllDead(10000) ;
end ;
/
declare 
 res number ;
begin
 res:=bookkern.ApplyUndoAllDead(10000) ;
end ;
/
declare 
 res number ;
begin
 res:=bookkern.ApplyUndoAllDead(10000) ;
end ;
/
declare 
 res number ;
begin
 res:=bookkern.ApplyUndoAllDead(10000) ;
end ;
/
declare 
 res number ;
begin
 res:=bookkern.ApplyUndoAllDead(10000) ;
end ;
/
declare 
 res number ;
begin
 res:=bookkern.ApplyUndoAllDead(10000) ;
end ;
/
declare 
 res number ;
begin
 res:=bookkern.ApplyUndoAllDead(10000) ;
end ;
/


delete unapp_items ;
delete unapp_cor_items ;
delete unapp_opers ;
delete from op_items where (op_id,op_subid) in
  (select op_id,op_subid from opers where status!=1 or status is null) ;
delete from cor_items where (op_id,op_subid) in
  (select op_id,op_subid from opers where status!=1 or status is null) ;
delete from opers where status!=1 or status is null ;
update opers set edited=null where edited is not null ;
commit ;
exit
