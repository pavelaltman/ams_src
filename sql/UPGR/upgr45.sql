-- Пакет процедур для работы с периодами и планом счетов
-- 17.10.01 Функция вставки новых периодов без длительного пересчета
-- 30.08.01 Добавлены функции для начала бухгалтерии с месяца ранее
CREATE OR REPLACE PACKAGE Periods AS
PROCEDURE CreateCInts(end_date DATE,prev_date DATE) ;
PROCEDURE CreateRemSums(rem_date DATE,prev NUMBER,prev_date DATE) ;
PROCEDURE CreateFirstMonth(start_date DATE) ;
PROCEDURE CreateFirstDate(start_date DATE) ;
PROCEDURE NewControlPoint(next_date DATE,is_start NUMBER) ;
PROCEDURE NextDate(next_date OUT DATE) ;
PROCEDURE BeginNextMonth ;
PROCEDURE BeginPrevMonth ;   --  Добавляет месяцы (перед началом бухгалтерии)
PROCEDURE DelLongTurns;      --  Удаляет длииные обороты, которые образовались после
                             --  перенесения даты начала бухгалтерии (BeginPrevMonth)
PROCEDURE AddNotExistedCInts;--  Добавляет периоды, необходимые по IS_START
                             --  и быстро считает обороты и корр обороты       
PROCEDURE AddAccount(nam VARCHAR2,des VARCHAR2,par NUMBER,bl NUMBER,res OUT NUMBER) ;
PROCEDURE DelAccount(cod NUMBER,res OUT NUMBER) ;
PROCEDURE DelSubs(cod NUMBER,res OUT NUMBER) ;
PROCEDURE DelSubAccounts(cod NUMBER,res OUT NUMBER) ;
END ;
/
CREATE OR REPLACE PACKAGE BODY Periods AS

PROCEDURE AddNotExistedCInts is --  Добавляет периоды, необходимые по IS_START
                                --  и быстро считает обороты и корр обороты       
    cursor CPSC is select * from CPS order by POINT_DATE;

    cursor CINTSC (pDate date, pIsStart number) is
        select * from CPS where POINT_DATE>pDate and
        MONTHS_BETWEEN(POINT_DATE, pDate) between 1 and pIsStart
        order by POINT_DATE;  

    cursor TURNSC (pBeg date, pPrev date, pEnd date) is
        select  T.CODE, T.D_TURN_ID, T.C_TURN_ID,
                T1.D_TURN_ID D1, T1.C_TURN_ID C1, A.ANAL_ID 
        from TURNS T, TURNS T1, ACCS A
        where
            T.BEG_DATE  =pBeg and T.END_DATE=pPrev and
            T1.BEG_DATE =T.END_DATE and T1.END_DATE=pEnd and
            T1.CODE     =T.CODE and
            A.CODE      =T.CODE;

    cursor CORTURNSC (pBeg date, pPrev date, pEnd date) is
        select S.D_CODE, S.C_CODE, T.TURN_ID, T1.TURN_ID T1
        from 
        (select distinct D_CODE, C_CODE from COR_TURNS 
        where
            ((beg_date=pBeg AND END_DATE=pPrev) or 
               (beg_date=pPrev AND END_DATE=pEnd))) S,
            COR_TURNS T, COR_TURNS T1
        where   
            T.beg_date  (+)= pBeg AND T.END_DATE(+)=pPrev and
            T1.beg_date (+)= pPrev AND T1.END_DATE(+)=pEnd and
            T.D_CODE    (+)= S.D_CODE and
            T1.D_CODE   (+)= S.D_CODE AND
            T.C_CODE    (+)= S.C_CODE and
            T1.C_CODE   (+)= S.C_CODE;  

    vTmp number;
    vTmp1 number;
    vBeg DATE;
    vEnd Date;
    vPrev Date;
    vAnalId number;
    vVal1 number;
    vVal2 number;
begin
    for X in CPSC loop
        vBeg:=X.POINT_DATE;
        for Y in CINTSC(X.POINT_DATE, X.IS_START) loop
            -- Проверяем есть ли такой интервал
            vEnd:=Y.POINT_DATE;
            select max(1) into vTmp from CINTS
            where beg_date=vBeg and end_date=vEnd;
            if vTmp is NULL then
                -- Нужно вставлять такой промежуток
                insert into CINTS (BEG_DATE, END_DATE) 
                values (vBeg, vEnd);
                vPrev:=add_months(vEnd, -1);
                -- Вставляем обороты
                for Z in TURNSC(vBeg, vPrev, vEnd) loop
                    select VALUE into vVal1 from SUMS
                    where SUM_ID=Z.d_turn_id;
                    select VALUE into vVal2 from SUMS
                    where SUM_ID=Z.D1;
                    CreateSum(vTmp,vVal1+vVal2);
                
                    select VALUE into vVal1 from SUMS
                    where SUM_ID=Z.C_turn_id;
                    select VALUE into vVal2 from SUMS
                    where SUM_ID=Z.C1;
                    CreateSum(vTmp1,vVal1+vVal2);
                    if Z.ANAL_ID is not NULL then
                        CreateAndCopyAnSum(Z.ANAL_ID, vTmp, Z.D_TURN_ID);
                        AddAnalSum(Z.ANAL_ID, vTmp, Z.D1, 1);
                        CreateAndCopyAnSum(Z.ANAL_ID, vTmp1, Z.C_TURN_ID);
                        AddAnalSum(Z.ANAL_ID, vTmp1, Z.C1, 1);
                    end if;

                    insert into Turns (BEG_DATE, END_DATE, CODE, d_turn_id, C_turn_id)
                    values (vBeg, vEnd, Z.CODE, vTmp, vTmp1);
                end loop;

                -- Вставляем корреспондирующие обороты
                for Z in CORTURNSC(vBeg, vPrev, vEnd) loop
                    vVal1:=0;
                    vVal2:=0;
                    if Z.TURN_ID is not NULL then
                        select VALUE into vVal1 from SUMS
                        where SUM_ID=Z.turn_id;
                    end if;
                    if Z.T1 is not NULL then
                        select VALUE into vVal2 from SUMS
                        where SUM_ID=Z.T1;
                    end if;

                    CreateSum(vTmp,vVal1+vVal2);
                    
                    insert into COR_Turns (BEG_DATE, END_DATE, D_CODE,C_CODE, turn_id)
                    values (vBeg, vEnd, Z.D_CODE, Z.C_CODE, vTmp);
                end loop;
            end if;      
        end loop;
    end loop;
end;



PROCEDURE DelLongTurns is    --  Удаляет длииные обороты, которые образовались после
                             --  перенесения даты начала бухгалтерии (BeginPrevMonth)
cursor ints is
    select BEG_DATE, END_DATE, C.ROWID from CINTS C, CPS
    where   POINT_DATE= BEG_DATE and
            MONTHS_BETWEEN(END_DATE, BEG_DATE)>is_start;
cursor cTurns (pBegDate Date, pEndDate Date) is
    select ROWID, d_turn_id, c_turn_id from TURNS
    where BEG_DATE=pBegDate and END_DATE=pEndDate;
cursor cCor_Turns (pBegDate Date, pEndDate Date) is
    select ROWID, turn_id from COR_TURNS
    where BEG_DATE=pBegDate and END_DATE=pEndDate;
begin
    for X in ints loop
        for T in cTurns(X.BEG_DATE, X.END_DATE) loop
            delete from SUMS where SUM_ID=T.D_TURN_ID;
            delete from SUMS where SUM_ID=T.C_TURN_ID;
            delete from TURNS where ROWID=T.ROWID;
        end loop;
        for T in cCOR_Turns(X.BEG_DATE, X.END_DATE) loop
            delete from SUMS where SUM_ID=T.TURN_ID;
            delete from COR_TURNS where ROWID=T.ROWID;
        end loop;
        delete from CINTS where ROWID=X.ROWID;
    end loop;

end DelLongTurns;


PROCEDURE CreatePrevCInts(pBegDate DATE,pPrevDate DATE) IS
prev_d_id NUMBER ;
prev_c_id NUMBER ;
d_id NUMBER ;
c_id NUMBER ;
d_val NUMBER ;
c_val NUMBER ;
prev NUMBER ;
CURSOR accs_curs IS SELECT * FROM accs ;
CURSOR ints IS
    SELECT pBegDate BEGDATE, point_date
    FROM cps
    WHERE point_date>pBegDate;

CURSOR prev_turns(sdate DATE) IS
    SELECT d_code,c_code,value FROM cor_turns,sums
    WHERE end_date=sdate AND beg_date=pPrevDate AND sum_id=turn_id ;
BEGIN
  FOR int IN ints LOOP
    INSERT INTO cints VALUES (int.BEGDATE, int.point_date) ;

    SELECT count(*) INTO prev FROM cints
     WHERE beg_date=pPrevDate AND end_date=int.point_date ;

    FOR acc IN accs_curs LOOP
      d_val:=0 ;
      c_val:=0 ;
      IF prev=1 THEN
        SELECT d_turn_id, c_turn_id INTO prev_d_id, prev_c_id FROM turns
           WHERE code=acc.code AND beg_date=pPrevDate
           AND end_date=int.point_date ;
        SELECT value INTO d_val FROM sums WHERE sum_id=prev_d_id ;
        SELECT value INTO c_val FROM sums WHERE sum_id=prev_c_id ;
      END IF ;
      CreateSum(d_id,d_val) ;
      CreateSum(c_id,c_val) ;
      INSERT INTO turns VALUES (int.BEGDATE,int.point_date,acc.code,d_id,c_id) ;
      IF prev=1 AND acc.anal_id IS NOT NULL THEN
         CopyAnalSum(acc.anal_id,d_id,prev_d_id) ;
         CopyAnalSum(acc.anal_id,c_id,prev_c_id) ;
      END IF ;
    END LOOP ;

    /* corturns */
    IF prev=1 THEN
      FOR prev_turn IN prev_turns(int.point_date) LOOP
        CreateSum(d_id,prev_turn.value) ;
        INSERT INTO cor_turns VALUES
               (int.BEGDATE,int.point_date, prev_turn.d_code,
                prev_turn.c_code,d_id);
      END LOOP ;
    END IF ;
  END LOOP ;
END CreatePrevCInts ;

PROCEDURE BeginPrevMonth is
    vBegDate date;
    vPrevDate date;
    vBegYear date;
    vMon number (2);
begin
    select min(point_date) into vPrevDate
    from cps;

    vBegDate:=add_months(vPrevDate, -1);
    vBegYear:= TRUNC (vPrevDate, 'YEAR');
    vMon:=Months_BETWEEN(vBegYear, vPrevDate);
    if vMon=0 then
        vMon:=12;
    elsif MOD(vMon, 3)=0 then
        vMon:=3;
    else
        vMon:=1;
    end if;

    insert into cps values (vBegDate, 1000);

    update cps set is_start=vMon where point_date=vPrevDate;
    CreateRemSums(vBegDate,1,vPrevDate);
    CreatePrevCInts(vBegDate,vPrevDate);
end;

PROCEDURE CreateCInts(end_date DATE,prev_date DATE) IS
prev_d_id NUMBER ;
prev_c_id NUMBER ;
d_id NUMBER ;
c_id NUMBER ;
d_val NUMBER ;
c_val NUMBER ;
prev NUMBER ;
CURSOR accs_curs IS SELECT * FROM accs ;
CURSOR ints IS
    SELECT point_date,end_date
    FROM cps
    WHERE  MONTHS_BETWEEN(end_date,point_date)<=is_start AND point_date<end_date ;
CURSOR prev_turns(sdate DATE) IS
    SELECT d_code,c_code,value FROM cor_turns,sums
    WHERE beg_date=sdate AND end_date=prev_date AND sum_id=turn_id ;
BEGIN
  FOR int IN ints LOOP
    INSERT INTO cints VALUES (int.point_date,int.end_date) ;

    SELECT count(*) INTO prev FROM cints
     WHERE beg_date=int.point_date AND end_date=prev_date ;

    FOR acc IN accs_curs LOOP
      d_val:=0 ;
      c_val:=0 ;
      IF prev=1 THEN
        SELECT d_turn_id INTO prev_d_id FROM turns
           WHERE code=acc.code AND beg_date=int.point_date AND end_date=prev_date ;
        SELECT value INTO d_val FROM sums WHERE sum_id=prev_d_id ;
        SELECT c_turn_id INTO prev_c_id FROM turns
           WHERE code=acc.code AND beg_date=int.point_date AND end_date=prev_date ;
        SELECT value INTO c_val FROM sums WHERE sum_id=prev_c_id ;
      END IF ;
      CreateSum(d_id,d_val) ;
      CreateSum(c_id,c_val) ;
      INSERT INTO turns VALUES (int.point_date,int.end_date,acc.code,d_id,c_id) ;
      IF prev=1 AND acc.anal_id IS NOT NULL THEN
         CopyAnalSum(acc.anal_id,d_id,prev_d_id) ;
         CopyAnalSum(acc.anal_id,c_id,prev_c_id) ;
      END IF ;
    END LOOP ;

    /* corturns */
    IF prev=1 THEN
      FOR prev_turn IN prev_turns(int.point_date) LOOP
        CreateSum(d_id,prev_turn.value) ;
        INSERT INTO cor_turns VALUES
               (int.point_date,int.end_date,prev_turn.d_code,prev_turn.c_code,d_id) ;
      END LOOP ;
    END IF ;
  END LOOP ;
END CreateCInts ;

PROCEDURE CreateRemSums(rem_date DATE,prev NUMBER,prev_date DATE) IS
CURSOR accs_curs IS SELECT * FROM accs ;
prev_id NUMBER ;
new_id  NUMBER ;
new_sum NUMBER ;
BEGIN
 FOR acc IN accs_curs LOOP
   new_sum:=0 ;
   IF prev=1 THEN
     SELECT end_val_id INTO prev_id FROM rems
         WHERE rem_date=prev_date AND code=acc.code ;
     SELECT value INTO new_sum FROM sums WHERE sum_id=prev_id ;
   END IF ;
   CreateSum(new_id,new_sum) ;
   INSERT INTO rems VALUES (acc.code,rem_date,new_id) ;
   IF prev=1 THEN
      CopyAnalSum(acc.anal_id,new_id,prev_id) ;
   END IF ;
 END LOOP ;
END ;

PROCEDURE CreateFirstDate(start_date DATE) IS
BEGIN
  INSERT INTO cps VALUES(start_date,1000) ;
END ;

PROCEDURE CreateFirstMonth(start_date DATE) IS
mons number ;
BEGIN
  select count(*) into mons from cps ;
  if mons=0 then
     CreateFirstDate(start_date) ;
     CreateRemSums(start_date,0,start_date) ;
  end if ;
END ;

PROCEDURE NewControlPoint(next_date DATE,is_start NUMBER) IS
last_date DATE ;
BEGIN
  commit ;
  set transaction use rollback segment myroll ;
  SELECT MAX(point_date) INTO last_date FROM cps ;
  BEGIN
    INSERT INTO cps VALUES (next_date,is_start) ;
  EXCEPTION
    WHEN OTHERS THEN
       return ;
  END ;

  CreateRemSums(next_date,1,last_date) ;
  CreateCInts(next_date,last_date) ;
  COMMIT ;
END NewControlPoint ;

PROCEDURE NextDate(next_date OUT DATE) IS
BEGIN
  SELECT ADD_MONTHS(TRUNC(MAX(point_date),'MONTH'),1) INTO next_date FROM cps ;
END ;

PROCEDURE BeginNextMonth IS
next_date DATE ;
BEGIN
  NextDate(next_date) ;
  IF next_date=TRUNC(next_date,'YEAR') THEN
     NewControlPoint(next_date,12) ;
  ELSIF MOD(MONTHS_BETWEEN(next_date,TRUNC(next_date,'YEAR')),3)=0 THEN
     NewControlPoint(next_date,3) ;
  ELSE
     NewControlPoint(next_date,1) ;
  END IF ;
END BeginNextMonth ;

PROCEDURE AddAccount(nam VARCHAR2,des VARCHAR2,par NUMBER,bl NUMBER,res OUT NUMBER) IS
new_code NUMBER ;
d_id     NUMBER ;
c_id     NUMBER ;
pd_id    NUMBER ;
pc_id    NUMBER ;
prec     accs%rowtype;
have_sub NUMBER ;
CURSOR c_ints IS
    SELECT beg_date,end_date FROM cints;
CURSOR c_cps IS
    SELECT point_date from cps;
BEGIN
  res:=0;
  IF par is not NULL THEN
     select * into prec from accs where code=par FOR UPDATE ;
     UPDATE accs SET node=node+1 where code=par;
  END IF ;

  SELECT COUNT(*) INTO have_sub FROM accs WHERE parent=par ;

  SELECT ObjSeq.NEXTVAL INTO new_code FROM DUAL ;
  INSERT INTO accs (code,name,descr,parent,bal,node,anal_id,bal_fld,opts,rev_code)
            VALUES (new_code,nam,des,par,bl,0,prec.anal_id,prec.bal_fld,prec.opts,prec.rev_code) ;

  INIT_RIGHTS(new_code) ;

  FOR int IN c_ints LOOP
      CreateSum(d_id,0) ;
      CreateSum(c_id,0) ;
      IF have_sub=0 AND par is not null THEN
         SELECT d_turn_id INTO pd_id FROM turns
          WHERE code=par AND beg_date=int.beg_date AND end_date=int.end_date ;
         UPDATE sums SET value=(SELECT value FROM sums WHERE sum_id=pd_id)
          WHERE sum_id=d_id ;
         SELECT c_turn_id INTO pc_id FROM turns
          WHERE code=par AND beg_date=int.beg_date AND end_date=int.end_date ;
         UPDATE sums SET value=(SELECT value FROM sums WHERE sum_id=pc_id)
          WHERE sum_id=c_id ;
         IF prec.anal_id IS NOT NULL THEN
           CopyAnalSum(prec.anal_id,d_id,pd_id) ;
           CopyAnalSum(prec.anal_id,c_id,pc_id) ;
         END IF ;
      END IF ;
      INSERT INTO turns VALUES (int.beg_date,int.end_date,new_code,d_id,c_id) ;
  END LOOP ;

  FOR cp IN c_cps LOOP
      CreateSum(d_id,0) ;
      IF have_sub=0 and par is not null THEN
         SELECT end_val_id INTO pd_id FROM rems
          WHERE code=par AND rem_date=cp.point_date ;
         UPDATE sums SET value=(SELECT value FROM sums WHERE sum_id=pd_id)
          WHERE sum_id=d_id ;
         IF prec.anal_id IS NOT NULL THEN
           CopyAnalSum(prec.anal_id,d_id,pd_id) ;
         END IF ;
      END IF ;
      INSERT INTO rems VALUES (new_code,cp.point_date,d_id) ;
  END LOOP ;

  -- Перенос операций
  IF have_sub=0 THEN
    INSERT INTO op_items
      SELECT op_id,op_subid,op_date,new_code,side,item_sum_id,node,for_post,setname
       FROM op_items WHERE code=par ;

    INSERT INTO cor_items
      SELECT op_id,op_subid,new_code,c_code,item_sum_id
       FROM cor_items WHERE d_code=par ;
    INSERT INTO cor_items
      SELECT op_id,op_subid,d_code,new_code,item_sum_id
       FROM cor_items WHERE c_code=par ;

    UPDATE op_items SET node=1 WHERE code=par ;
  END IF ;
END AddAccount;

PROCEDURE DelAccount(cod NUMBER,res OUT NUMBER) IS
 c NUMBER;
 par NUMBER;
BEGIN
  res:=0;

  select count(*) into c from accs where parent=cod;
  IF c>0 THEN
    raise_application_error(-20001,'У счета есть суб-счета') ;
  END IF;

  select count(*) into c from turns,sums where code=cod and d_turn_id=sum_id
                               and value is not null and value<>0 ;
  IF c>0 THEN
    raise_application_error(-20002,'На счету есть остаток') ;
  END IF;

  select count(*) into c from rems,sums where code=cod and end_val_id=sum_id
                               and value is not null and value<>0 ;
  IF c>0 THEN
    raise_application_error(-20002,'На счету есть остаток') ;
  END IF;

  select count(*) into c from op_items where code=cod ;
  IF c>0 THEN
    raise_application_error(-20003,'На счету есть операции') ;
  END IF;

  select parent into par from accs where code=cod;
  UPDATE accs SET node=node-1 where code=par;

  DELETE rems  where code=cod;
  DELETE turns where code=cod;
  DELETE accs  where code=cod;
END DelAccount;

PROCEDURE DelSubs(cod NUMBER,res OUT NUMBER) IS
 c NUMBER;
 par NUMBER;
BEGIN
  res:=0;

  UPDATE accs SET node=0 where code=cod ;

  DELETE rems  WHERE (code in (SELECT code from accs START WITH code=cod CONNECT BY parent= PRIOR code))
                      AND code!=cod ;
  DELETE turns where (code in (SELECT code from accs START WITH code=cod CONNECT BY parent= PRIOR code))
                      AND code!=cod ;
  DELETE cor_turns where ((d_code in (SELECT code from accs START WITH code=cod CONNECT BY parent= PRIOR code))
                          AND d_code!=cod) OR
                         ((c_code in (SELECT code from accs START WITH code=cod CONNECT BY parent= PRIOR code))
                          AND c_code!=cod)  ;

  UPDATE op_items SET item_sum_id=NULL WHERE
         (code in (SELECT code from accs START WITH code=cod CONNECT BY parent= PRIOR code))
          AND code!=cod AND
         (item_sum_id IN (SELECT item_sum_id FROM op_items WHERE code=cod)) ;
  DELETE op_items WHERE (code in (SELECT code from accs START WITH code=cod CONNECT BY parent= PRIOR code))
                         AND code!=cod ;
  UPDATE op_items SET node=0 WHERE code=cod ;


  DELETE cor_items WHERE ((d_code in (SELECT code from accs START WITH code=cod CONNECT BY parent= PRIOR code))
                          AND d_code!=cod) OR
                         ((c_code in (SELECT code from accs START WITH code=cod CONNECT BY parent= PRIOR code))
                          AND c_code!=cod) ;


  DELETE accs  where (code in (SELECT code from accs START WITH code=cod CONNECT BY parent= PRIOR code))
                      AND code!=cod ;

END DelSubs ;


PROCEDURE DelSubAccounts(cod NUMBER,res OUT NUMBER) IS
 c NUMBER;
 par NUMBER;
BEGIN
  res:=0;

  select count(*) into par from accs where parent in (select code from accs where parent=cod);

  if par>0 THEN
    raise_application_error(-20003,'У суб-счетов есть свои суб-счета') ;
  END IF;

  DELETE rems  where code in (select code from accs where parent=cod);
  DELETE turns where code in (select code from accs where parent=cod);
  DELETE accs  where code in (select code from accs where parent=cod);
  UPDATE accs  SET node=0 where code=cod;

END DelSubAccounts;

END;
/
show errors;
exit;

