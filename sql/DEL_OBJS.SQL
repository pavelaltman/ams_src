-- select name,oid,otype from objs ;

declare
 cursor c is
   select * from objs for update ;
 a number ;
begin
  for r in c loop
    begin
      if (r.otype=1 or r.otype=4 or r.otype=5) and r.oid<300000 then
        delete objs where current of c ;
      end if ;
    exception
      when others then a:=1 ;
    end ;
  end loop ;
end ;
/
