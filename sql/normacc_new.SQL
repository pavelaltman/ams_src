CREATE or REPLACE FUNCTION userACC_new (pAccName VARCHAR2) 
 RETURN VARCHAR2 AS 
 S VARCHAR2(20);
 Res varchar2(20);
 Prom varchar2(2);
 PredI NUMBER;
 i NUMBER;
 Flag NUMBER;
 Val NUMBER;
BEGIN
 -- ѕреобразование нового плана счетов
 Flag := 100;
 Prom := '';
 i:=1;
 Res:='';
 if instr(pAccName,'Z/') != 0 then 
  Res:='Z/';
 end if;
 i:= instr(pAccName, '/');
 if i=0 then
  return ' ';
 end if;
 PredI:= i+1;
 Val := 0;

 while Flag > 0 LOOP
   i:= instr(pAccName, '/', PredI, 1);
   if i=0 then 
     exit;
   end if ;
   S:= substr(pAccName, PredI, I-PredI);
   PredI := i+1;
   S:= TO_CHAR( TO_NUMBER (S));
   Res:= Res || Prom || S;
   Val:= Val +1;
   if Val = 3 then
    Prom:='/';
   end if;
   Flag := Flag -1 ;
 END LOOP;

 
 Res:= Res||Prom|| TO_CHAR(TO_NUMBER(substr (pAccName, PredI, length(pAccName))));

 return Res; 
END;
/
commit;
/
-- select * from USER_ERRORS;
