drop table message_table ;
create table message_table
(
 Constraint_Name varchar2(30) not null,
 Actions         varchar2(3)  not null,
 Parent_Child    varchar2(1)  not null,
 Error_Message   varchar2(2000)
);
grant select on message_table to &1.Role;

insert into message_table values('OPERS_DOC','IU','C','Нет такого документа') ;

exit

