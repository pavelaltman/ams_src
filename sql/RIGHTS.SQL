/* Создание функций и процедур, текст которых зависит от схемы */

CREATE OR REPLACE FUNCTION GETSHEMA RETURN VARCHAR2 AS
BEGIN
 return '&1.' ;
END ;
/

CREATE OR replace VIEW PRIV_T as
 (select oid,runame uname,pr0,pr1,pr2,pr3,pr4,pr5 from objpriv where ru=0)
   union all
 (select oid,grantee uname,pr0,pr1,pr2,pr3,pr4,pr5
      from objpriv,dba_role_privs  where runame=granted_role);

CREATE OR replace PROCEDURE PRIV_OBJ(pid NUMBER) as
begin
 delete privtest where poid=pid;
 insert into privtest (
      select oid,uname,max(pr0),max(pr1),max(pr2),max(pr3),max(pr4),max(pr5)
             from PRIV_T where oid=pid group by oid,uname );
end;
/
show errors

CREATE OR REPLACE PROCEDURE INIT_RIGHTS(key NUMBER) AS
BEGIN
  declare
   t number ;
  begin
    insert into objpriv(runame,ru,oid,pr0,pr1,pr2,pr3,pr4,pr5) values ('&1.SUPER',1,key,1,1,1,1,1,1) ;
  exception
    when others then t:=0 ;
  end ;
  PRIV_OBJ(key) ;
END ;
/

CREATE OR replace FUNCTION HAVEROLE_(TROLE VARCHAR2) RETURN NUMBER AS
c number;
r VARCHAR2(30) ;
begin
 if trole!='&1.ROLE' THEN
    r:='&1._' || trole ;
 else
    r:=trole ;
 end if ;
 select count(*) into c from dba_role_privs where granted_role=r and grantee=USER;
 return c;
end;
/

CREATE OR replace FUNCTION HAVEROLE(TROLE VARCHAR2) RETURN NUMBER AS
c number;
r VARCHAR2(30) ;
begin
 select count(*) into c from dba_role_privs where granted_role='&1.SUPER' and grantee=USER;
 if c=0 THEN
    if trole!='&1.ROLE' THEN
       r:='&1._' || trole ;
    else
       r:=trole ;
    end if ;
    select count(*) into c from dba_role_privs where granted_role=r and grantee=USER;
 END IF ;
 return c;
end;
/
exit
