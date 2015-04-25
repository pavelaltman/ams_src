set serveroutput on ;

declare
  res NUMBER ;
  old_par NUMBER ;
  new_par NUMBER ;
  acc_num NUMBER ;
  cursor a(cod number) is
    select * from &1..accs where parent=cod order by name;
begin
 select code into old_par from &1..accs where name='&2' ;
 select code into new_par from accs where name='&2' ;

  FOR aa IN a(old_par) LOOP
   BEGIN
    periods.AddAccount(aa.name,aa.descr,new_par,aa.bal,res) ;
   EXCEPTION
    when others then
      begin
       dbms_output.put_line('Счет ' || aa.name || ' уже существует!') ;
       commit ;
      end ;
   END ;
  END LOOP ;

  SELECT count(*) INTO acc_num from &1..accs where parent=old_par ;
  dbms_output.put_line('Всего ' || acc_num || ' счетов.') ;
  commit ;

end ;
/
exit


