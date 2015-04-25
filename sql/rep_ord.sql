
declare
cursor curs is


select catid,oid FROM objs where (catid,ord) IN
 (select catid,ord from
   (select catid,ord,count(*) as num from objs group by catid,ord)
    where num>1) ;

max_ord NUMBER ;
begin
  FOR O IN curs LOOP
    SELECT MAX(ord) INTO max_ord FROM objs WHERE catid=o.catid ;
    UPDATE objs SET ord=max_ord+1 WHERE oid=o.oid ;
  END LOOP ;
end ;
/



