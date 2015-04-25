/*   Создание хранимых функций, процедур и пакетов        */
/*   Запускать после cretabs.sql и rights.sql             */

-- Маленькие подзапросы
create or replace function
    GetObjCode ( pObjName OBJS.NAME%TYPE) return NUMBER
AS
    vObjCode    number;
-- Возвращает код объекта по системному имени
begin
    select OID into vObjCode from OBJS where NAME = pObjName;
    return vObjCode;
exception
    when NO_DATA_FOUND then
        RAISE_APPLICATION_ERROR( -20001, 'Нет такого объекта '|| pObjName);
end;
/

create or replace function
    GetAccCode ( pAccName ACCS.NAME%TYPE) return NUMBER
AS
    vAccCode    number;
-- Возвращает код Счета по системному имени
begin
    select Code into vAccCode from ACCS where NAME = pAccName;
    return vAccCode;
exception
    when NO_DATA_FOUND then
        RAISE_APPLICATION_ERROR( -20001, 'Нет такого счета '|| pAccName);
end;
/
show errors;

-- Функции и процедуры для работы с пользователями и правами

CREATE OR replace PROCEDURE PRIV_USER(PUSER VARCHAR2) as
begin
 delete privtest where uname=puser;
 insert into privtest
  ( select oid,uname,max(pr0),max(pr1),max(pr2),max(pr3),max(pr4),max(pr5) from
     PRIV_T where uname=puser group by oid,uname );
end;
/

CREATE OR replace FUNCTION HAVEPRIV(OBID NUMBER,NP VARCHAR2 DEFAULT NULL)
    RETURN NUMBER AS
 c number;
begin
  select pr0 into c from privtest where poid=obid and uname=USER;
  return c;
end;
/

CREATE OR replace PROCEDURE COPY_USER(OUSER VARCHAR2,NUSER VARCHAR2) as
 CURSOR OP IS
  select * from objpriv where runame=ouser;
 CURSOR ORL IS
  select * from dba_role_privs where grantee=ouser;
 cid INTEGER;
 D NUMBER ;
begin
 for o in OP loop
  insert into objpriv values
    (o.oid,nuser,o.ru,o.pr0,o.pr1,o.pr2,o.pr3,o.pr4,o.pr5);
 end loop;
 cid:=DBMS_SQL.OPEN_CURSOR;
 for o in ORL loop
   BEGIN
   /* IF INSTR(o.granted_role,GETSHEMA)!=0 THEN */
     DBMS_SQL.PARSE(cid,'grant '|| o.granted_role ||' to '||nuser,dbms_sql.v7);
   /*  END IF ; */
   EXCEPTION
     WHEN OTHERS THEN
       D:=1 ;
   END ;
 end loop;
 DBMS_SQL.CLOSE_CURSOR(cid);
end;
/
show errors

create or replace procedure
    SetObjPriv ( pOID number,
                 pRuName VARCHAR2,
                 pPr0 number,
                 pPr1 number default 0,
                 pPr2 number default 0,
                 pPr3 number default 0,
                 pPr4 number default 0,
                 pPr5 number default 0) as
   vRu number;
begin
    select max(1) into vRu from DBA_ROLES
    where ROLE = pRuName;
    if vRu is NULL then
        vRu := 0;
    end if;
    delete from OBJPRIV where OID=pOID and RUNAME= pRUNAME;
    insert into OBJPRIV
        (OID,    RUNAME, RU,   PR0, PR1, PR2, PR3, PR4, PR5)
    values
        (pOID, pRUNAME, vRu, pPR0, pPR1, pPR2, pPR3, pPR4, pPR5);
end;
/


-- Создать сумму
CREATE OR REPLACE PROCEDURE CreateSum(newid OUT NUMBER,val IN NUMBER) AS
BEGIN
  SELECT SumSeq.NEXTVAL INTO newid FROM DUAL ;
  INSERT INTO sums VALUES (newid,val) ;
END ;
/

-- Вернуть код счета по имени
CREATE OR REPLACE FUNCTION GetCode(nam VARCHAR2) RETURN NUMBER AS
ret NUMBER:=NULL ;
BEGIN
  SELECT code INTO ret FROM accs WHERE name=nam ;
  RETURN ret ;
END ;
/

-- Вернуть противоположную аргументу сторону
CREATE OR REPLACE FUNCTION OtherSide(side CHAR) RETURN CHAR AS
BEGIN
  IF side='D' THEN
     RETURN 'K' ;
  ELSE
     RETURN 'D' ;
  END IF ;
END ;
/

-- Вернуть выровненный номер документа
CREATE OR REPLACE FUNCTION NormChar(str VARCHAR2,width NUMBER) RETURN VARCHAR2 AS
 len NUMBER ;
 str1 VARCHAR2(100) ;
BEGIN
 str1:=LTRIM(str,'0 ') ;
 len:=LENGTH(LTRIM(str1,'0123456789')) ;
 IF len is NULL THEN len:=0 ; END IF ;
 RETURN LPAD(str1,width+len) ;
END ;
/
show errors

-- Триггер для контроля вводимого типа и номера документа
CREATE OR REPLACE TRIGGER OpersTrg
BEFORE INSERT OR UPDATE ON opers
FOR EACH ROW
WHEN (NEW.status=0)
DECLARE
 N NUMBER ;
 T NUMBER ;
BEGIN
  SELECT otype INTO T FROM objs WHERE name=:NEW.doc_type ;
  IF T!=1 THEN raise_application_error(-20001,'Нет такого вида документа') ;
  END IF ;

  SELECT COUNT(*) INTO N FROM opers,objs
  WHERE doc_type=:NEW.DOC_TYPE AND doc_num=:NEW.DOC_NUM AND status=1 AND doc_type=name AND dup=0 ;
  IF N>0 THEN raise_application_error(-20002,'Такой документ уже существует') ;
  END IF ;
END ;
/


-- Группа функций для работы со стандартными проводками
create or replace procedure CloneCond(oid number,newid number) as
begin
 insert into conds(cond_id,cond_subid,ord,text,doc_type,storno,native_doc,auto_apply,pre_formula)
  (select newid,cond_subid,ord,text,doc_type,storno,native_doc,auto_apply,pre_formula
     from conds where cond_id=oid);
 insert into cond_items (cond_id,cond_subid,code,setname,side,for_post,item_num,item_formula)
  (select newid,cond_subid,code,setname,side,for_post,item_num,item_formula
     from cond_items where cond_id=oid);
end;
/
show errors

create or replace procedure ReOrd_Cond(cid number) as
 t NUMBER;
 CURSOR ct(cid number) IS
  SELECT * FROM conds
  WHERE cond_id=cid
  ORDER BY ord
  FOR UPDATE;

begin
 t:=0;
 FOR i IN ct(cid) LOOP
  t:=t+16;
  update conds set ord=t where CURRENT OF ct;
 END LOOP;
end;
/

create or replace procedure
InsertCond(cid number,csid IN OUT number,des varchar2,res out number) as
 t NUMBER;
 nsid NUMBER;
 neword number;
begin
 res:=0;
 if csid is NULL then
   select max(ord) into t FROM conds WHERE cond_id=cid;
   if t is null then t:=0; end if;
   neword:=t+16;
 else
--
  select ord into neword from conds WHERE cond_id=cid AND cond_subid=csid;
  select max(ord) into t FROM conds WHERE cond_id=cid and ord<neword;
--
  if neword-t>1 then neword:=t+(neword-t)/2;
  else
   if t is NULL then
      if neword>2 then neword:=neword/2;
      else
          ReOrd_Cond(cid);
          neword:=8;
      end if;
   else
      ReOrd_Cond(cid);
      select ord into neword from conds WHERE cond_id=cid AND cond_subid=csid;
      neword:=neword-8;
   end if;
  end if;
 end if;
 select max(cond_subid) into csid FROM conds WHERE cond_id=cid;

 if csid is null then csid:=0;
 else csid:=csid+1; end if;

 insert into Conds(cond_id,cond_subid,ord,text) values (cid,csid,neword,des);
 return;
end;
/

create or replace procedure MoveSubCond(dir number,cid number,csid number) as
 t      NUMBER;
 neword NUMBER;
begin
 select ord into neword from conds where cond_id=cid and cond_subid=csid;
 if dir = 0 then
  select max(ord) into t FROM conds
      WHERE cond_id=cid and ord<neword;
 else
  select min(ord) into t FROM conds
      WHERE cond_id=cid and ord>neword;
 end if;
--
 if t is NULL then
   return;
 end if;
--
 update conds set ord=333333
     WHERE cond_id=cid and ord=t;
 update conds set ord=t
     WHERE cond_id=cid and cond_subid=csid;
 update conds set ord=neword
     WHERE cond_id=cid and ord=333333;
 return;
end;
/
show errors

-- Процедуры и функции для работы с объектами и их категориями
create or replace procedure MakeSubTree(cid number) as
 t NUMBER;
 l NUMBER;
 pcid NUMBER;

 CURSOR ct(cid in number) IS
  SELECT * FROM cattree
  WHERE catid=cid AND tord=0
  ORDER BY ORD
  FOR UPDATE;

begin
 FOR i IN ct(cid) LOOP
  select TreeSeq.NextVal into t from dual;
  update cattree set tord=t where CURRENT OF ct;
  MakeSubTree(i.owncat);
  pcid:=i.owncat;
  l:=i.lev;
 END LOOP;
 update cattree set levm=levm-power(2,l)
   where owncat in (select owncat from cattree start with owncat=pcid
   connect by catid = prior owncat);
 update cattree set last=1,levm=levm+power(2,l)
   where owncat=pcid;
end;
/

create or replace procedure MakeTree as
 cid NUMBER ;
begin
 DELETE CATTREE;
 insert into cattree (select level as lev,0 as tord,0 as last,65535,objcat.* from objcat
   start with catid is null
   connect by catid = prior owncat);
 select owncat into cid from objcat where descr='/' ;
 MakeSubTree(cid);
 COMMIT;
end;
/

create or replace procedure ReOrd_Cat(cid number) as
 t NUMBER;
 CURSOR ct(cid in number) IS
  SELECT * FROM objcat
  WHERE catid=cid
  ORDER BY ord
  FOR UPDATE;

begin
 t:=0;
 FOR i IN ct(cid) LOOP
  t:=t+16;
  update objcat set ord=t where CURRENT OF ct;
 END LOOP;
end;
/

create or replace procedure
InsertCat(cid number,des varchar2,key in out number,res out number) as
 t NUMBER;
 neword number;
begin
 res:=0;
 if key is NULL then
   select max(ord) into t FROM objcat WHERE catid=cid;
   if t is null then t:=0; end if;
   neword:=t+16;
 else
--
  select ord into neword from objcat where owncat=key;
  select max(ord) into t FROM objcat WHERE catid=cid and ord<neword;
--
  if neword-t>1 then neword:=t+(neword-t)/2;
  else
   if t is NULL then
      if neword>2 then neword:=neword/2;
      else
          ReOrd_Cat(cid);
          neword:=8;
      end if;
   else
      ReOrd_Cat(cid);
      select ord into neword from objcat where owncat=key;
      neword:=neword-8;
   end if;
  end if;
 end if;
 select ObjSeq.NextVal into key from dual;
 insert into objcat(owncat,catid,ord,descr) values (key,cid,neword,des);

 INIT_RIGHTS(key) ;

 return;
end;
/

create or replace procedure MoveCat(dir number,key number) as
 cid    NUMBER;
 t      NUMBER;
 neword NUMBER;
begin
 select ord,catid into neword,cid from objcat where owncat=key;
 if dir = 0 then
  select max(ord) into t FROM objcat
      WHERE catid=cid and ord<neword;
 else
  select min(ord) into t FROM objcat
      WHERE catid=cid and ord>neword;
 end if;
--
 if t is NULL then
   return;
 end if;
--
 update objcat set ord=neword
     WHERE catid=cid and ord=t;
 update objcat set ord=t
     WHERE owncat=key;
 return;
end;
/

create or replace procedure ReOrd_Obj(cid number) as
 t NUMBER;
 CURSOR ct(cid in number) IS
  SELECT * FROM objs
  WHERE catid=cid
  ORDER BY ord desc
  FOR UPDATE;

begin
 select count(*) into t from objs where catid=cid;
 t:=(t+1)*16;
 FOR i IN ct(cid) LOOP
  update objs set ord=t where CURRENT OF ct;
  t:=t-16;
 END LOOP;
end;
/
show errors

create or replace procedure
InsertObj(cid number,typ number,nam varchar2,des varchar2,key in out number,res out number) as
 t NUMBER;
 neword number;
begin
 res:=0;
 if key is NULL then
   select max(ord) into t FROM objs WHERE catid=cid;
   if t is null then t:=0; end if;
   neword:=t+16;
 else
--
  select ord into neword from objs where oid=key;
  select max(ord) into t FROM objs WHERE catid=cid and ord<neword;
--
  if neword-t>1 then neword:=t+(neword-t)/2;
  else
   if t is NULL then
      if neword>2 then neword:=neword/2;
      else
          ReOrd_Obj(cid);
          neword:=8;
      end if;
   else
      ReOrd_Obj(cid);
      select ord into neword from objs where oid=key;
      neword:=neword-8;
   end if;
  end if;
 end if;
 select ObjSeq.NextVal into key from dual;

 begin
  insert into objs(oid,otype,catid,ord,name,descr) values (key,typ,cid,neword,nam,des);
 exception
  when others then
     select max(ord) into t FROM objs WHERE catid=cid;
     if t is null then t:=0; end if;
     neword:=t+16;
     insert into objs(oid,otype,catid,ord,name,descr) values (key,typ,cid,neword,nam,des);
 end ;

 INIT_RIGHTS(key) ;

 return;

end;
/

create or replace procedure MoveObj(dir number,key number) as
 cid    NUMBER;
 t      NUMBER;
 neword NUMBER;
 temp   NUMBER;
 key2   NUMBER;
begin
 select ord,catid into neword,cid from objs where oid=key;
 if dir = 0 then
  select max(ord) into t FROM objs
      WHERE catid=cid and ord<neword;
 else
  select min(ord) into t FROM objs
      WHERE catid=cid and ord>neword;
 end if;
--
 if t is NULL then
   return;
 end if;
--

 select max(ord)+1 into temp from objs ;
 select oid into key2 from objs
     WHERE catid=cid and ord=t;

 update objs set ord=temp
     WHERE oid=key2;
 update objs set ord=t
     WHERE oid=key;
 update objs set ord=neword
     WHERE oid=key2;
 return;
end;
/

CREATE OR REPLACE PROCEDURE GetConst(cname VARCHAR2,val OUT VARCHAR2,type OUT NUMBER) AS
BEGIN
  SELECT value,const_type INTO val,type FROM CONSTS,OBJS WHERE CONSTS.OID=OBJS.OID AND NAME=CNAME ;
EXCEPTION
  WHEN OTHERS THEN
    return ;
END ;
/

CREATE OR REPLACE FUNCTION ContainObjs(p_owncat NUMBER,p_otype NUMBER) RETURN NUMBER AS
c NUMBER ;
o NUMBER ;
CURSOR cats IS
  SELECT OWNCAT FROM objcat
  START WITH OWNCAT=p_owncat CONNECT BY CATID=PRIOR OWNCAT ;
BEGIN
 c:=0 ;
 FOR cat IN cats LOOP
   SELECT COUNT(*) INTO o FROM objs WHERE catid=cat.owncat AND otype=p_otype ;
   c:=c+o ;
 END LOOP ;
 RETURN c ;
END ;
/


-- Функции и процедуры для работы с базами для аналитического учета
create or replace procedure ReOrd_Str(cid number) as
 t NUMBER;
 CURSOR ct(cid in number) IS
  SELECT * FROM dbstr
  WHERE dbid=cid
  ORDER BY ord
  FOR UPDATE;

begin
 t:=0;
 FOR i IN ct(cid) LOOP
  t:=t+16;
  update dbstr set ord=t where CURRENT OF ct;
 END LOOP;
end;
/

create or replace procedure
GetNewOrd_Str(cid number,key varchar2,neword in out number) as
 t NUMBER;
begin
 if key is NULL then
   select max(ord) into t FROM dbstr WHERE dbid=cid;
   if t is null then t:=0; end if;
   neword:=t+16;
 else
--
  select ord into neword from dbstr where RealName=key and dbid=cid ;
  select max(ord) into t FROM dbstr WHERE dbid=cid and ord<neword;
--
  if neword-t>1 then neword:=t+(neword-t)/2;
  else
   if t is NULL then
      if neword>2 then neword:=neword/2;
      else
          ReOrd_Str(cid);
          neword:=8;
      end if;
   else
      ReOrd_Str(cid);
      select ord into neword from dbstr where RealName=key and dbid=cid ;
      neword:=neword-8;
   end if;
  end if;
 end if;
 return;
end;
/


create or replace procedure MoveStr(dir number,cid number,key varchar2) as
 t      NUMBER;
 neword NUMBER;
 opt    NUMBER;
begin
 select ord into neword from dbstr where RealName=key and DBID=cid;
 if dir = 0 then
  select max(ord) into t FROM dbstr
      WHERE dbid=cid and ord<neword;
 else
  select min(ord) into t FROM dbstr
      WHERE dbid=cid and ord>neword;
 end if;
--
 if t is NULL then
   return;
 end if;
--
 select def_opt into opt from dbstr WHERE dbid=cid and ord=t;
 if mod(opt,512)>255 then return; end if;
--
 update dbstr set ord=neword
     WHERE dbid=cid and ord=t;
 update dbstr set ord=t
     WHERE dbid=cid and RealName=key;
 return;
end;
/

create or replace procedure ReOrd_DBCat(did number,cid number) as
 t NUMBER;
 CURSOR ct(did number,cid in number) IS
  SELECT * FROM dbcat
  WHERE dbid=did and catid=cid
  ORDER BY ord
  FOR UPDATE;

begin
 t:=0;
 FOR i IN ct(did,cid) LOOP
  t:=t+16;
  update dbcat set ord=t where CURRENT OF ct;
 END LOOP;
end;
/

create or replace procedure
InsertDBCat(did number,cid number,des varchar2,key in out number,res out number) as
 t NUMBER;
 neword number;
begin
 res:=0;
 if key is NULL then
   select max(ord) into t FROM dbcat WHERE dbid=did and catid=cid;
   if t is null then t:=0; end if;
   neword:=t+16;
 else
--
  select ord into neword from dbcat where dbid=did and owncat=key;
  select max(ord) into t FROM dbcat WHERE dbid=did and catid=cid and ord<neword;
--
  if neword-t>1 then neword:=t+(neword-t)/2;
  else
   if t is NULL then
      if neword>2 then neword:=neword/2;
      else
          ReOrd_DBCat(did,cid);
          neword:=8;
      end if;
   else
      ReOrd_DBCat(did,cid);
      select ord into neword from dbcat where dbid=did and owncat=key;
      neword:=neword-8;
   end if;
  end if;
 end if;
 select DBCatSeq.NextVal into key from dual;
 insert into dbcat(dbid,owncat,catid,ord,descr) values (did,key,cid,neword,des);
 return;
end;
/

create or replace procedure MoveDBCat(dir number,did number,key number) as
 cid    NUMBER;
 t      NUMBER;
 neword NUMBER;
begin
 select ord,catid into neword,cid from dbcat where dbid=did and owncat=key;
 if dir = 0 then
  select max(ord) into t FROM dbcat
      WHERE dbid=did and catid=cid and ord<neword;
 else
  select min(ord) into t FROM dbcat
      WHERE dbid=did and catid=cid and ord>neword;
 end if;
--
 if t is NULL then
   return;
 end if;
--
 update dbcat set ord=neword
     WHERE dbid=did and catid=cid and ord=t;
 update dbcat set ord=t
     WHERE dbid=did and owncat=key;
 return;
end;
/

create or replace procedure MakeDBSubTree(did number,cid number,o in out number) as
 l NUMBER;
 pcid NUMBER;

 CURSOR ct(did in number,cid in number) IS
  SELECT * FROM dbcattree
  WHERE dbid=did and catid=cid AND tord=0
  ORDER BY ORD
  FOR UPDATE;

begin
 FOR i IN ct(did,cid) LOOP
  o:=o+1;
  update dbcattree set tord=o where CURRENT OF ct;
  MakeDBSubTree(did,i.owncat,o);
  pcid:=i.owncat;
  l:=i.lev;
 END LOOP;
 update dbcattree set levm=levm-power(2,l)
   where dbid=did and owncat in (select owncat from dbcattree start with owncat=pcid
   connect by catid = prior owncat);
 update dbcattree set last=1,levm=levm+power(2,l)
   where dbid=did and owncat=pcid;
end;
/

create or replace procedure MakeDBTree(did number) as
 o number;
 root number ;
begin
 o:=0;
 DELETE DBCATTREE where dbid=did;
 insert into dbcattree (select level as lev,0 as tord,0 as last,65535,dbcat.* from dbcat
   where dbid=did
   start with catid is null
   connect by catid = prior owncat);
 select owncat into root from dbcat where dbid=did and catid is null ;
 MakeDBSubTree(did,root,o);
 COMMIT;
end;
/


create or replace procedure
ChangeDbCats(pDbId number,pCatId1 number, pCatId2 number) as
    vOrd1 number;
    vOrd2 number;
------------------------- Меняет местами категории (Shift+Up   +Down)
begin
    select ORD into vOrd1 from DBCAT
    where DBID= pDbId and OWNCAT=pCatId1;

    select ORD into vOrd2 from DBCAT
    where DBID= pDbId and OWNCAT=pCatId2;

    update DBCAT set ORD = vOrd2
    where DBID= pDbId and OWNCAT=pCatId1;

    update DBCAT set ORD = vOrd1
    where DBID= pDbId and OWNCAT=pCatId2;
exception
    when NO_DATA_FOUND then
    NULL;
end;
/
create or replace function
DelDbCat(pDbId number,pCatId number) return NUMBER as
------------------------- Удаляет категорию
------------------------- возвращает 1 - удалено успешно
-------------------------            0 - не удалено
begin
    delete from DBCAT
    where
        DBID= pDbId and OWNCAT=pCatId and CATID is not NULL;
    return 1;
exception
    when others then
    return 0;
end;
/
create or replace procedure
MoveDbCatTo(pDbId number, pCatId number, pCatIdTo number) as
------------------------- Перемещает категорию
------------------------- с pCatId в категорию pCatIdTo
-------------------------
    pNewOrd number;
begin
    select max(1) into pNewOrd
    from DUAL
    where
        pCatIdTo in (select OWNCAT
                    from DBCAT
                    start with DBID=pDBID and OWNCAT=pCatId
                    connect by DBID=pDBID and prior OWNCAT = CATID
                    );

    if pNewOrd=1 then
        raise_application_error (-20012, 'Не могу переместить категорию в свою же подкатегорию');
    end if;

    select max(ord) into pNewOrd
    from DBCAT
    where
        DBID    = pDbId     and
        CATID   = pCatIdTo;

    if pNewOrd is NULL then
        pNewOrd := 16;
    else
        pNewOrd := pNewOrd + 16;
    end if;

    update DBCAT
    set CATID = pCatIdTo , ORD = pNewOrd
    where
        DBID = pDbId and
        OWNCAT = pCatId;
end;
/
show errors;
/
create or replace procedure
MoveAllDbCatTo(pDbId number, pCatId number, pCatIdTo number) as
------------------------- Перемещает все подкатегории из категории
------------------------- с pCatId в категорию pCatIdTo
-------------------------
    pNewOrd number;
begin
    select max(ord) into pNewOrd
    from DBCAT
    where
        DBID    = pDbId     and
        CATID   = pCatIdTo;

    if pNewOrd is NULL then
        pNewOrd := 16;
    else
        pNewOrd := pNewOrd + 16;
    end if;

    update DBCAT
    set CATID = pCatIdTo , ORD = pNewOrd+ORD
    where
        DBID = pDbId and
        CATID = pCatId;
end;
/
show errors;

create or replace function
    GetCatFullName(pOwnCat number) return varchar2
    -- Возвращает полное имя категории по ее коду
as
    vRes varchar2(250);
    vTmp varchar2 (250);
    cursor s is
        select DESCR
        from OBJCAT
        start with
            OWNCAT = pOwnCat
        connect by
            OWNCAT= prior CATID;
begin
    vRes :='';
    for X in S LOOP
        vTmp:=vRes;
        if X.DESCR != '/' then
            vRes:=X.Descr||'/'||vTmp;
        else
            vRes:='/'||vTmp;
        end if;

    end LOOP;

    return vRes;
end;
/
create or replace function
    GetObjCatFullName(pObjId number) return varchar2
    -- Возвращает полное имя категории объекта по его
    -- коду
as
    vCatId number;
begin
    select CATID into vCatId from OBJS
    where
        OID=pObjId  ;
    return GetCatFullName(vCatId);
end;
/
show errors;


-- Процедуры для работы с аналитическими суммами

-- Копирование аналитической суммы
CREATE OR REPLACE
PROCEDURE CopyAnalSum(anal_id NUMBER,     -- Код объекта аналитики
                      new_sum_id NUMBER,  -- код суммы куда копировать
                      old_sum_id NUMBER)  -- код суммы откуда копировать
                      AS
cid NUMBER ;
sign NUMBER ;
sql_res NUMBER ;
anal_name VARCHAR2(30) ;
func_str  VARCHAR2(100) ;
BEGIN
  IF anal_id IS NOT NULL THEN
     SELECT name INTO anal_name FROM objs WHERE oid=anal_id ;

     cid:=DBMS_SQL.OPEN_CURSOR;
     func_str:='DELETE FROM ' || anal_name || ' WHERE sumid=:new_sum_id' ;
     DBMS_SQL.PARSE(cid,func_str,dbms_sql.v7);
     dbms_sql.bind_variable(cid,':new_sum_id',new_sum_id);
     sql_res:=dbms_sql.execute(cid);

     func_str:='BEGIN ' || anal_name || '_CAS(:new_sum_id,:old_sum_id) ; END ;' ;
     DBMS_SQL.PARSE(cid,func_str,dbms_sql.v7);
     dbms_sql.bind_variable(cid,':new_sum_id',new_sum_id);
     dbms_sql.bind_variable(cid,':old_sum_id',old_sum_id);
     sql_res:=dbms_sql.execute(cid);
     DBMS_SQL.CLOSE_CURSOR(cid);

  END IF ;
END ;
/

-- Удаление аналитичексой суммы
CREATE OR REPLACE
PROCEDURE DeleteAnalSum(anal_id NUMBER,  -- Код объекта аналитики
                        sum_id NUMBER)   -- Код удаляемой суммы
                        AS
cid NUMBER ;
sql_res NUMBER ;
anal_name VARCHAR2(30) ;
func_str  VARCHAR2(100) ;
BEGIN
  IF anal_id IS NOT NULL THEN
     SELECT name INTO anal_name FROM objs WHERE oid=anal_id ;

     cid:=DBMS_SQL.OPEN_CURSOR;
     func_str:='DELETE FROM ' || anal_name || ' WHERE sumid=:sum_id' ;
     DBMS_SQL.PARSE(cid,func_str,dbms_sql.v7);
     dbms_sql.bind_variable(cid,':sum_id',sum_id);
     sql_res:=dbms_sql.execute(cid);
     DBMS_SQL.CLOSE_CURSOR(cid);

  END IF ;
END ;
/

-- Перемещение аналитической суммы
CREATE OR REPLACE
PROCEDURE MoveAnalSum(anal_id NUMBER,   -- Код объекта аналитики
                      new_id NUMBER,    -- Код суммы откуда копировать
                      old_id NUMBER)    -- Код суммы куда копировать
                      AS
cid NUMBER ;
sql_res NUMBER ;
anal_name VARCHAR2(30) ;
func_str  VARCHAR2(100) ;
BEGIN
  IF anal_id IS NOT NULL THEN
     SELECT name INTO anal_name FROM objs WHERE oid=anal_id ;

     cid:=DBMS_SQL.OPEN_CURSOR;

     func_str:='DELETE FROM ' || anal_name || ' WHERE sumid=:new_id' ;
     DBMS_SQL.PARSE(cid,func_str,dbms_sql.v7);
     dbms_sql.bind_variable(cid,':new_id',new_id);
     sql_res:=dbms_sql.execute(cid);

     func_str:='UPDATE ' || anal_name || ' SET sumid=:new_id WHERE sumid=:old_id' ;
     DBMS_SQL.PARSE(cid,func_str,dbms_sql.v7);
     dbms_sql.bind_variable(cid,':new_id',new_id);
     dbms_sql.bind_variable(cid,':old_id',old_id);
     sql_res:=dbms_sql.execute(cid);
     DBMS_SQL.CLOSE_CURSOR(cid);

  END IF ;
END ;
/

-- Сложение (вычитание) аналитических сумм
CREATE OR REPLACE
PROCEDURE AddAnalSumSet(anal_id NUMBER,         -- Код объекта аналитики
                     new_sum_id NUMBER,      -- Код суммы к которой прибавляем (отнимаем)
                     old_sum_id NUMBER,      -- Код суммы которую прибавляем (отнимаем)
                     sign NUMBER,            -- +1 - прибавление, -1 - отнимание
                     setname VARCHAR2)
                     AS
cid NUMBER ;
sql_res NUMBER ;
anal_name VARCHAR2(30) ;
func_str  VARCHAR2(100) ;
BEGIN
  IF anal_id IS NOT NULL THEN
     SELECT name INTO anal_name FROM objs WHERE oid=anal_id ;

     cid:=DBMS_SQL.OPEN_CURSOR;
     func_str:='BEGIN ' || anal_name || '_' || setname || '_AAS(:new_sum_id,:old_sum_id,:sign) ; END ;' ;
     DBMS_SQL.PARSE(cid,func_str,dbms_sql.v7);
     dbms_sql.bind_variable(cid,':new_sum_id',new_sum_id);
     dbms_sql.bind_variable(cid,':old_sum_id',old_sum_id);
     dbms_sql.bind_variable(cid,':sign',sign);
     sql_res:=dbms_sql.execute(cid);
     DBMS_SQL.CLOSE_CURSOR(cid);

  END IF ;
END ;
/

CREATE OR REPLACE
PROCEDURE AddAnalSum(anal_id NUMBER,         -- Код объекта аналитики
                     new_sum_id NUMBER,      -- Код суммы к которой прибавляем (отнимаем)
                     old_sum_id NUMBER,      -- Код суммы которую прибавляем (отнимаем)
                     sign NUMBER)            -- +1 - прибавление, -1 - отнимание
                     AS
BEGIN
 AddAnalSumSet(anal_id,new_sum_id,old_sum_id,sign,'') ;
END ;
/

-- Функция, возвращающая "Развернутое сальдо" по указанному счету
CREATE OR REPLACE FUNCTION
ExpandedSaldo(acode NUMBER,cp DATE,pos NUMBER) RETURN NUMBER AS
acc accs%ROWTYPE ;
sumid NUMBER;
res NUMBER ;
CURSOR accs_cursor(acode NUMBER) IS
  SELECT * FROM accs WHERE parent=acode ;
cid NUMBER ;
sql_res NUMBER ;
t NUMBER ;
anal_name VARCHAR2(30) ;
func_str  VARCHAR2(300) ;
BEGIN
 SELECT * INTO acc FROM accs WHERE code=acode;

 IF acc.expanded=0 THEN
    select end_val_id into sumid from rems where code=acode and rem_date=cp;
    select value into res from sums where sum_id=sumid;
    if pos>0 and res>0 then return  res; end if;
    if pos=0 and res<0 then return -res; end if;
    return 0;
 END IF ;

 IF acc.node = 0 THEN
  select end_val_id into sumid from rems where code=acode and rem_date=cp;
  IF acc.anal_id is NULL THEN
    select value into res from sums where sum_id=sumid;
    if pos>0 and res>0 then return  res; end if;
    if pos=0 and res<0 then return -res; end if;
    return 0;
  ELSE
    cid:=DBMS_SQL.OPEN_CURSOR ;
    SELECT name INTO anal_name FROM objs WHERE oid=acc.anal_id ;
    func_str:='BEGIN SELECT SUM(' || acc.bal_fld || ') INTO :res FROM ' || anal_name
                 ||' WHERE sumid=:sumid AND AARTICLE!='' '' AND ' || acc.bal_fld ;
    if pos > 0 then func_str:=func_str || '>0 ; END;';
    else func_str:=func_str || '<0 ; END;';
    end if;
    DBMS_SQL.PARSE(cid,func_str,dbms_sql.v7);
    dbms_sql.bind_variable(cid,':sumid',sumid);
    dbms_sql.bind_variable(cid,':res',res);
    sql_res:=dbms_sql.execute(cid);
    dbms_sql.variable_value(cid,':res',res);
    DBMS_SQL.CLOSE_CURSOR(cid);
    if pos <= 0 then res:=-res; end if;
  END IF ;
 ELSE
  res:=0;
  FOR ac IN accs_cursor(acode) LOOP
--   dbms_output.put(ac.name ||'  '|| ac.code);
   t:=ExpandedSaldo(ac.code,cp,pos);
   if t is not null then res:=res+t; end if;
--   dbms_output.put_line('tot:'|| res);
  END LOOP;
 End IF ;
-- dbms_output.put_line('result for '|| acode ||' is ' || res);
 return res;
End;
/

CREATE OR REPLACE FUNCTION Rest(aname VARCHAR2,rdate DATE) RETURN NUMBER AS
 acode NUMBER ;
 sdate DATE ;
 val NUMBER ;
 tmp NUMBER ;
BEGIN
 SELECT MAX(point_date) into sdate FROM cps WHERE point_date<=rdate ;
 SELECT code INTO acode FROM accs WHERE name=aname ;
 SELECT value INTO val FROM sums,rems r
    WHERE sum_id=end_val_id AND rem_date=sdate AND r.code=acode ;

 SELECT SUM(value) INTO tmp FROM sums,op_items i,opers o
   WHERE sum_id=item_sum_id AND i.op_id=o.op_id and i.op_subid=o.op_subid AND
         o.op_date>=sdate AND o.op_date<rdate AND post=for_post AND status=1 AND
         (storno=0 AND side='D' OR storno=1 AND side='K')  AND i.code=acode ;
 IF tmp IS NOT NULL THEN
   val:=val+tmp ;
 END IF ;

 SELECT SUM(value) INTO tmp FROM sums,op_items i,opers o
   WHERE sum_id=item_sum_id AND i.op_id=o.op_id and i.op_subid=o.op_subid AND
         o.op_date>=sdate AND o.op_date<rdate AND post=for_post AND status=1 AND
         (storno=1 AND side='D' OR storno=0 AND side='K')  AND i.code=acode ;
 IF tmp IS NOT NULL THEN
   val:=val-tmp ;
 END IF ;

 RETURN val ;
END ;
/

CREATE OR REPLACE FUNCTION DCTurn(aname VARCHAR2,tdate1 DATE,tdate2 DATE,cred NUMBER) RETURN NUMBER AS
 acode NUMBER ;
 val NUMBER ;
 tmp NUMBER ;
BEGIN
 SELECT code INTO acode FROM accs WHERE name=aname ;

 SELECT COUNT(*) INTO val FROM cints
    WHERE beg_date=tdate1 AND end_date=tdate2 ;

 IF val>0 THEN
   IF cred=0 THEN
     SELECT value INTO val FROM sums,turns
       WHERE sum_id=d_turn_id AND beg_date=tdate1 AND end_date=tdate2 AND
              code=acode ;
   ELSE
     SELECT value INTO val FROM sums,turns
       WHERE sum_id=c_turn_id AND beg_date=tdate1 AND end_date=tdate2 AND
              code=acode ;
   END IF ;
 ELSE
   IF cred=0 THEN
      SELECT SUM(value) INTO tmp FROM sums,op_items i,opers o
        WHERE sum_id=item_sum_id AND i.op_id=o.op_id and i.op_subid=o.op_subid AND
              o.op_date>=tdate1 AND o.op_date<tdate2 AND post=for_post AND status=1 AND
              storno=0 AND side='D' AND i.code=acode ;
      IF tmp IS NOT NULL THEN
        val:=val+tmp ;
      END IF ;

      SELECT SUM(value) INTO tmp FROM sums,op_items i,opers o
        WHERE sum_id=item_sum_id AND i.op_id=o.op_id and i.op_subid=o.op_subid AND
              o.op_date>=tdate1 AND o.op_date<tdate2 AND post=for_post AND status=1 AND
              storno=1 AND side='D' AND i.code=acode ;
      IF tmp IS NOT NULL THEN
        val:=val-tmp ;
      END IF ;
   ELSE
      SELECT SUM(value) INTO tmp FROM sums,op_items i,opers o
        WHERE sum_id=item_sum_id AND i.op_id=o.op_id and i.op_subid=o.op_subid AND
              o.op_date>=tdate1 AND o.op_date<tdate2 AND post=for_post AND status=1 AND
              storno=0 AND side='K' AND i.code=acode ;
      IF tmp IS NOT NULL THEN
        val:=val+tmp ;
      END IF ;

      SELECT SUM(value) INTO tmp FROM sums,op_items i,opers o
        WHERE sum_id=item_sum_id AND i.op_id=o.op_id and i.op_subid=o.op_subid AND
              o.op_date>=tdate1 AND o.op_date<tdate2 AND post=for_post AND status=1 AND
              storno=1 AND side='K' AND i.code=acode ;
      IF tmp IS NOT NULL THEN
        val:=val-tmp ;
      END IF ;
   END IF ;
 END IF ;

 RETURN val ;
END ;
/

CREATE OR REPLACE FUNCTION DCCorTurn(daname VARCHAR2,caname VARCHAR2,tdate1 DATE,tdate2 DATE) RETURN NUMBER AS
 dacode NUMBER ;
 cacode NUMBER ;
 val NUMBER ;
 tmp NUMBER ;
BEGIN
 SELECT code INTO dacode FROM accs WHERE name=daname ;
 SELECT code INTO cacode FROM accs WHERE name=caname ;

 SELECT COUNT(*) INTO val FROM cints
    WHERE beg_date=tdate1 AND end_date=tdate2 ;

 IF val>0 THEN
     SELECT value INTO val FROM sums,cor_turns
       WHERE sum_id=turn_id AND beg_date=tdate1 AND end_date=tdate2 AND
              d_code=dacode ANd c_code=cacode ;
 ELSE
      SELECT SUM(value) INTO tmp FROM sums,cor_items i,opers o
        WHERE sum_id=item_sum_id AND i.op_id=o.op_id and i.op_subid=o.op_subid AND
              o.op_date>=tdate1 AND o.op_date<tdate2 AND post=0 AND status=1 AND
              storno=0 AND d_code=dacode AND c_code=cacode ;
      IF tmp IS NOT NULL THEN
        val:=val+tmp ;
      END IF ;

      SELECT SUM(value) INTO tmp FROM sums,cor_items i,opers o
        WHERE sum_id=item_sum_id AND i.op_id=o.op_id and i.op_subid=o.op_subid AND
              o.op_date>=tdate1 AND o.op_date<tdate2 AND post=0 AND status=1 AND
              storno=1 AND d_code=dacode AND c_code=cacode ;
      IF tmp IS NOT NULL THEN
        val:=val-tmp ;
      END IF ;
 END IF ;

 RETURN val ;
END ;
/

CREATE OR REPLACE FUNCTION R_AS(aname VARCHAR2,rdate DATE) RETURN NUMBER AS
 analid NUMBER ;
 acode NUMBER ;
 sdate DATE ;
 val NUMBER ;
 tmp NUMBER ;
 sign NUMBER ;
 CURSOR ops(ocode NUMBER,d1 DATE,d2 DATE) IS
   SELECT i.item_sum_id,i.side,o.storno FROM opers o,op_items i
     WHERE o.op_id=i.op_id AND o.op_subid=i.op_subid AND
           o.op_date>=d1 AND o.op_date<d2 AND
           code=ocode AND post=for_post AND status=1 ;

BEGIN
 SELECT anal_id  INTO analid FROM accs WHERE name=aname ;
 IF analid IS NULL THEN RETURN 0 ;
 ELSE
  SELECT MAX(point_date) into sdate FROM cps WHERE point_date<=rdate ;
  SELECT code INTO acode FROM accs WHERE name=aname ;

  CREATESUM(val,0) ;
  DeleteAnalSum(analid,val) ;

  SELECT end_val_id INTO tmp FROM rems WHERE rem_date=sdate AND code=acode ;
  AddAnalSum(analid,val,tmp,1) ;

  FOR O IN ops(acode,sdate,rdate) LOOP
    IF O.side='D' AND O.storno=0 THEN sign:=1 ; ELSE sign:=-1 ; END IF ;
    AddAnalSum(analid,val,O.item_sum_id,sign) ;
  END LOOP ;
 END IF ;
 RETURN val ;
END ;
/

CREATE OR REPLACE FUNCTION DT_CT_AS(aname VARCHAR2,tdate1 DATE,tdate2 DATE,tside VARCHAR2) RETURN NUMBER AS
 analid NUMBER ;
 acode NUMBER ;
 val NUMBER ;
 tmp NUMBER ;
 sign NUMBER ;
 CURSOR ops(ocode NUMBER) IS
   SELECT i.item_sum_id,i.side,o.storno FROM opers o,op_items i
     WHERE o.op_id=i.op_id AND o.op_subid=i.op_subid AND
           o.op_date>=tdate1 AND o.op_date<tdate2 AND
           code=ocode AND post=for_post AND status=1 AND side=tside ;

BEGIN
 SELECT anal_id  INTO analid FROM accs WHERE name=aname ;
 IF analid IS NULL THEN RETURN 0 ;
 ELSE
  SELECT code INTO acode FROM accs WHERE name=aname ;

  CREATESUM(val,0) ;
  DeleteAnalSum(analid,val) ;

  FOR O IN ops(acode) LOOP
    IF O.storno=0 THEN sign:=1 ; ELSE sign:=-1 ; END IF ;
    AddAnalSum(analid,val,O.item_sum_id,sign) ;
  END LOOP ;
 END IF ;
 RETURN val ;
END ;
/

CREATE OR REPLACE FUNCTION DT_CT_AS_Cor(aname VARCHAR2,tdate1 DATE,tdate2 DATE,tside VARCHAR2,cor_aname VARCHAR2,pre_val NUMBER) RETURN NUMBER AS
 analid NUMBER ;
 acode NUMBER ;
 acor_code NUMBER ;
 val NUMBER ;
 tmp NUMBER ;
 sign NUMBER ;
 CURSOR d_ops(icode NUMBER,cor_code NUMBER) IS
   SELECT i.item_sum_id,i.side,o.storno FROM opers o,op_items i,cor_items c
     WHERE o.op_id=i.op_id AND o.op_subid=i.op_subid AND
           o.op_id=c.op_id AND o.op_subid=c.op_subid AND
           o.op_date>=tdate1 AND o.op_date<tdate2 AND
           i.code=icode AND i.side='D' AND d_code=i.code AND c_code=cor_code AND
           post=for_post AND status=1 ;
 CURSOR c_ops(icode NUMBER,cor_code NUMBER) IS
   SELECT i.item_sum_id,i.side,o.storno FROM opers o,op_items i,cor_items c
     WHERE o.op_id=i.op_id AND o.op_subid=i.op_subid AND
           o.op_id=c.op_id AND o.op_subid=c.op_subid AND
           o.op_date>=tdate1 AND o.op_date<tdate2 AND
           i.code=icode AND i.side='K' AND c_code=i.code AND d_code=cor_code AND
           post=for_post AND status=1 ;

BEGIN
 SELECT anal_id  INTO analid FROM accs WHERE name=aname ;
 IF analid IS NULL THEN RETURN 0 ;
 ELSE
  SELECT code INTO acode FROM accs WHERE name=aname ;
  SELECT code INTO acor_code FROM accs WHERE name=cor_aname ;

  IF pre_val IS NULL THEN
    CREATESUM(val,0) ;
    DeleteAnalSum(analid,val) ;
  ELSE
    val:=pre_val ;
  END IF ;

  IF tside='D' THEN
    FOR O IN d_ops(acode,acor_code) LOOP
      IF O.storno=0 THEN sign:=1 ; ELSE sign:=-1 ; END IF ;
      AddAnalSum(analid,val,O.item_sum_id,sign) ;
    END LOOP ;
  ELSE
    FOR O IN c_ops(acode,acor_code) LOOP
      IF O.storno=0 THEN sign:=1 ; ELSE sign:=-1 ; END IF ;
      AddAnalSum(analid,val,O.item_sum_id,sign) ;
    END LOOP ;
  END IF ;
 END IF ;
 RETURN val ;
END ;
/

CREATE OR REPLACE FUNCTION DT_CT_AS_TEXT(aname VARCHAR2,tdate1 DATE,tdate2 DATE,tside VARCHAR2,oper_text VARCHAR2,pre_val NUMBER) RETURN NUMBER AS
 analid NUMBER ;
 acode NUMBER ;
 acor_code NUMBER ;
 val NUMBER ;
 tmp NUMBER ;
 sign NUMBER ;
 CURSOR d_ops(icode NUMBER) IS
   SELECT i.item_sum_id,i.side,o.storno FROM opers o,op_items i
     WHERE o.op_id=i.op_id AND o.op_subid=i.op_subid AND
           o.op_date>=tdate1 AND o.op_date<tdate2 AND
           i.code=icode AND i.side='D' AND INSTR(text,oper_text)>0 AND
           post=for_post AND status=1 ;
 CURSOR c_ops(icode NUMBER) IS
   SELECT i.item_sum_id,i.side,o.storno FROM opers o,op_items i
     WHERE o.op_id=i.op_id AND o.op_subid=i.op_subid AND
           o.op_date>=tdate1 AND o.op_date<tdate2 AND
           i.code=icode AND i.side='K' AND INSTR(text,oper_text)>0 AND
           post=for_post AND status=1 ;

BEGIN
 SELECT anal_id  INTO analid FROM accs WHERE name=aname ;
 IF analid IS NULL THEN RETURN 0 ;
 ELSE
  SELECT code INTO acode FROM accs WHERE name=aname ;

  IF pre_val IS NULL THEN
    CREATESUM(val,0) ;
    DeleteAnalSum(analid,val) ;
  ELSE
    val:=pre_val ;
  END IF ;

  IF tside='D' THEN
    FOR O IN d_ops(acode) LOOP
      IF O.storno=0 THEN sign:=1 ; ELSE sign:=-1 ; END IF ;
      AddAnalSum(analid,val,O.item_sum_id,sign) ;
    END LOOP ;
  ELSE
    FOR O IN c_ops(acode) LOOP
      IF O.storno=0 THEN sign:=1 ; ELSE sign:=-1 ; END IF ;
      AddAnalSum(analid,val,O.item_sum_id,sign) ;
    END LOOP ;
  END IF ;
 END IF ;
 RETURN val ;
END ;
/
show errors

CREATE OR REPLACE PROCEDURE StopAnal(acode NUMBER) AS
cid NUMBER ;
sql_res NUMBER ;
aid NUMBER;
parc NUMBER;
anal_name VARCHAR2(30) ;
func_str  VARCHAR2(400) ;
BEGIN
  select anal_id into aid from accs where code=acode;
  if aid is null then
   return;
  end if;

  update accs set anal_id=NULL,bal_fld=NULL,VAL_CODE=NULL,REV_CODE=NULL
     where code=acode;

  SELECT name INTO anal_name FROM objs WHERE oid=aid ;
  cid:=DBMS_SQL.OPEN_CURSOR;
  func_str:='DELETE FROM ' || anal_name || ' WHERE sumid in ' ||
  '((select item_sum_id from op_items where code=:acode and node!=1) UNION ALL ' ||
  ' (select end_val_id  from rems     where code=:acode) UNION ALL ' ||
  ' (select d_turn_id   from turns    where code=:acode) UNION ALL ' ||
  ' (select c_turn_id   from turns    where code=:acode))';

  DBMS_SQL.PARSE(cid,func_str,dbms_sql.v7);
  dbms_sql.bind_variable(cid,':acode',acode);
  sql_res:=dbms_sql.execute(cid);
  DBMS_SQL.CLOSE_CURSOR(cid);
  select parent into parc from accs where code=acode;
  StopAnal(parc);
END ;
/
show errors


-- Получить интервал нумерации
CREATE OR REPLACE
PROCEDURE GetRangeDates(doc_type VARCHAR2,op_date DATE,
                        sdate OUT DATE,edate OUT DATE) AS
  doc objs%ROWTYPE ;
BEGIN
  SELECT * INTO doc FROM objs WHERE name=doc_type ;

  IF doc.range=0 OR doc.range IS NULL THEN
     sdate:=TO_DATE('1-1-1997','DD-MM-YYYY') ;
     edate:=TO_DATE('1-1-2047','DD-MM-YYYY') ;
  ELSIF doc.range=1 THEN
     sdate:=TRUNC(op_date,'YEAR') ;
     edate:=ADD_MONTHS(TRUNC(op_date,'YEAR'),12) ;
  ELSIF doc.range=2 THEN
     sdate:=TRUNC(op_date,'MONTH') ;
     edate:=ADD_MONTHS(TRUNC(op_date,'MONTH'),1) ;
  END IF ;
END ;
/
show errors

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

show errors


CREATE OR REPLACE FUNCTION GET_LINK_DB(OP_ID_ NUMBER, OP_SUBID_ NUMBER) RETURN NUMBER AS
  doc VARCHAR2(30) ;
  cond NUMBER ;
  par NUMBER ;
BEGIN
  SELECT parent INTO par FROM objs WHERE name=(SELECT doc_type FROM opers
                                  WHERE OP_ID=OP_ID_ AND OP_SUBID=OP_SUBID_) ;
  IF par IS NULL THEN
    SELECT parent INTO par FROM objs WHERE oid=(SELECT cond_id FROM opers
                                  WHERE OP_ID=OP_ID_ AND OP_SUBID=OP_SUBID_) ;
  END IF ;
  return par ;
EXCEPTION
  WHEN NO_DATA_FOUND THEN return par ;
END ;
/

CREATE OR REPLACE PROCEDURE DEL_LINK_INFO(OP_ID_ NUMBER, OP_SUBID_ NUMBER) AS
dbid NUMBER ;
cid NUMBER ;
sql_res NUMBER ;
name VARCHAR2(30) ;
func_str VARCHAR2(200) ;
BEGIN
  dbid:=GET_LINK_DB(OP_ID_,OP_SUBID_) ;
  IF dbid IS NOT NULL THEN
     SELECT name INTO name FROM objs WHERE oid=dbid ;

     cid:=DBMS_SQL.OPEN_CURSOR;
     func_str:='DELETE ' || name || ' WHERE (ARTICLE,VER) = (SELECT DOC_NUM,DOC_NUM_V FROM opers WHERE op_id=:op_id AND op_subid=:op_subid)' ;
     DBMS_SQL.PARSE(cid,func_str,dbms_sql.v7);
     dbms_sql.bind_variable(cid,':op_id',OP_ID_);
     dbms_sql.bind_variable(cid,':op_subid',OP_SUBID_);
     sql_res:=dbms_sql.execute(cid);
  END IF ;
END ;
/

CREATE OR REPLACE PROCEDURE CANCEL_EDIT_OPER(OK NUMBER,newop NUMBER,readonly NUMBER,
                                             OP_ID_ NUMBER, OP_SUBID_ NUMBER,
                                             OLD_ID NUMBER, OLD_SUBID NUMBER) AS
BEGIN
     if OK=1 then
      if newop=0 then
         update op_items set item_sum_id=NULL WHERE OP_ID=OP_ID_ ;
         delete op_items WHERE OP_ID=OP_ID_ ;
         delete opers WHERE OP_ID=OP_ID_ ;
      end if ;
     else
       if newop=0 then
        if readonly=0 then
         update op_items set item_sum_id=NULL WHERE OP_ID=OP_ID_ AND OP_SUBID!=OP_SUBID_ ;
         delete op_items WHERE OP_ID=OP_ID_ ;
         delete opers WHERE OP_ID=OP_ID_ ;
         update opers set EDITED=NULL WHERE OP_ID=OLD_ID AND OP_SUBID=OLD_SUBID ;
        end if ;
       else
         delete op_items WHERE OP_ID=OP_ID_ AND OP_SUBID=OP_SUBID_ ;
         delete opers WHERE OP_ID=OP_ID_ AND OP_SUBID=OP_SUBID_ ;
         DEL_LINK_INFO(OP_ID_,OP_SUBID_) ;
       end if ;
     end if ;
     commit ;
end ;
/
show errors


-- Пакет процедур для проведения операций по счетам
CREATE OR REPLACE PACKAGE bookkern AS

 -- Проводит все непроведенные операции с указанным идентификатором сессии
 FUNCTION ApplyUndoAll(session_id NUMBER,max_count NUMBER) RETURN NUMBER ;

 -- Проводит все непроведенные операции с идентификаторами сессиий, которые
 -- уже закончились
 FUNCTION ApplyUndoAllDead(max_count NUMBER) RETURN NUMBER ;

 PROCEDURE ApplyUndoOper(oper_id NUMBER,oper_subid NUMBER) ;

 -- Ввести заголовок новой операции
 PROCEDURE InputOperHeader(oper_id OUT NUMBER,oper_subid NUMBER,post NUMBER,
                           storno NUMBER,oper_text VARCHAR2,
                           doc_type VARCHAR2,doc_num VARCHAR2,doc_num_v NUMBER,
                           oper_date DATE,cond_id NUMBER) ;

 -- Ввести элемент новой операции
 PROCEDURE InputOperItem(oper_id NUMBER,oper_subid NUMBER,code VARCHAR2,
                         side VARCHAR2,item_sum_id OUT NUMBER,
                         item_sum NUMBER,for_post NUMBER,set_name VARCHAR2) ;

 -- Провести новую операцию
 -- result=0 - успех
 --        1 - Суммы не совпадают (дебет с кредитом)
 --        2 - Счет разбит на субсчета
 --        3 - Балансовый счет для откладывания
 --        4 - Недостаточно прав
 --        5 - Дата вне допустимых пределов
 --        6 - Не совпадает синтетика с аналитикой
 --        7 - В аналитике не совпадает итого-строка с позициями по балансовому полю
 --        9 - Действие ведет к отрицательным остаткам
 --       10 - Внутренняя ошибка
 --       11 - Действие привело к отрицательным остаткам, но было произведено,
 --            так как у пользователя была роль NEGREMS
 --       13 - "Неродной" тип документа
 PROCEDURE ApplyOper(oper_id NUMBER,oper_subid NUMBER,result OUT NUMBER,message OUT VARCHAR2) ;

 -- Провести новую операцию вместо старой
 PROCEDURE UpdateOper(old_id NUMBER,old_subid NUMBER,
                      oper_id NUMBER,oper_subid NUMBER,result OUT NUMBER) ;

 -- Удалить операцию
 -- result=8 - Операция редактируется другим пользователем
 --        4 - Недостаточно прав
 --       12 - Есть следующие операции
 --       14 - Есть следующие операции, эта операция первая в группе
 PROCEDURE DeleteOper(oper_id NUMBER,oper_subid NUMBER,allow_del_next NUMBER,result OUT NUMBER,USERNAME OUT VARCHAR2) ;
 PROCEDURE DeleteOperGroup(oper_id NUMBER,result OUT NUMBER,start_sub NUMBER DEFAULT -1) ;


 -- Сформировать номер документа,
 -- учитывая установки автонумерации и дублирования номеров
 -- result=0-успех, 1,2-Неправильный тип документа
 -- 3-такой номер уже существует, а дублирование запрещено
 -- 4-не задан номер, а автонумерация запрещена
 -- 10 - успех, документ был сдублирован версией
 PROCEDURE ProcessDocNum(OPER_ID NUMBER,OPER_SUBID NUMBER,OLD_ID NUMBER,OLD_SUBID NUMBER,opdate DATE,doctype VARCHAR2,docnum IN OUT VARCHAR2,
                         docnum_v IN OUT NUMBER,no_check_dup NUMBER,
                         result OUT NUMBER) ;

 -- Изменить остаток на начало
 PROCEDURE ChangeBegVal(rem_code NUMBER,new_sum_id NUMBER) ;

 -- Пересчет одного счета без кор. оборотов
 PROCEDURE RecalcOneAccount(acc_name VARCHAR2,begin_date DATE,ending_date DATE) ;

 -- Пересчет кор. оборотов по одному счету
 PROCEDURE RecalcOneAccountCor(acc_name VARCHAR2,begin_date DATE,ending_date DATE) ;

 -- Полный пересчет поддерева
 PROCEDURE RecalcAccount_(acc_code NUMBER,begin_date DATE,ending_date DATE) ;
 PROCEDURE RecalcAccount(acc_code NUMBER,begin_date DATE,ending_date DATE) ;
 PROCEDURE RecalcAccount(acc_name VARCHAR2,begin_date DATE,ending_date DATE) ;

 -- Пересчет остатка начало
 PROCEDURE RecalcBegin(acc_code NUMBER) ;
 PROCEDURE RecalcBegin(acc_name VARCHAR2) ;

 -- Для внутреннего использования
 PROCEDURE CopyOper(old_id NUMBER,old_subid NUMBER,
                    new_id NUMBER,new_subid NUMBER,new_status NUMBER) ;
 PROCEDURE AddAllParentItems ;
 PROCEDURE AddAllCorItems ;

END ;
/

CREATE OR REPLACE PACKAGE BODY bookkern AS


PROCEDURE ProcessDocNum(OPER_ID NUMBER,OPER_SUBID NUMBER,OLD_ID NUMBER,OLD_SUBID NUMBER,opdate DATE,doctype VARCHAR2,docnum IN OUT VARCHAR2,
                        docnum_v IN OUT NUMBER,no_check_dup NUMBER,
                        result OUT NUMBER) IS
 T NUMBER ;
 D NUMBER ;
 A NUMBER ;
 exist NUMBER ;
 range_exist NUMBER ;
 max_num VARCHAR2(50) ;
 sdate DATE ;
 edate DATE ;
BEGIN
  -- Документ ли это
  SELECT otype INTO T FROM objs WHERE name=doctype ;
  IF T!=1 THEN result:=1 ; RETURN ; END IF ;

  -- интервал автонумерации
  GetRangeDates(doctype,opdate,sdate,edate) ;

  -- Автонумерация
  IF docnum IS NULL THEN
     SELECT auton INTO A FROM objs WHERE name=doctype ;
     IF A IS NULL THEN A:=0 ; END IF ;
     IF A>0 THEN
        SELECT MAX(NormChar(doc_num,30)) INTO max_num FROM opers
          WHERE doc_type=doctype AND op_date>=sdate AND op_date<edate ;
        max_num:=LTRIM(max_num) ;
        docnum:=TO_CHAR(TO_NUMBER(SUBSTR(max_num,1,LENGTH(max_num)-LENGTH(LTRIM(max_num,'1234567890'))))+1) ;
        IF docnum IS NULL THEN docnum:='1' ; END IF ;
     ELSE result:=4 ; RETURN ;
     END IF ;
  END IF ;

  IF docnum_v IS NULL THEN docnum_v:=0 ; END IF ;

  -- Дублирование номера
  IF no_check_dup=1 THEN
     exist:=0 ;
     range_exist:=0 ;
  ELSE
    SELECT COUNT(*) INTO exist FROM opers
       WHERE doc_type=doctype AND doc_num=docnum AND doc_num_v=docnum_v
             AND NOT (OP_ID=OPER_ID AND OP_SUBID=OPER_SUBID)
             AND NOT (OP_ID=NVL(OLD_ID,0) AND OP_SUBID=NVL(OLD_SUBID,0)) ;
    SELECT COUNT(*) INTO range_exist FROM opers
       WHERE doc_type=doctype AND doc_num=docnum
          AND status=1 AND op_date>=sdate AND op_date<edate
             AND NOT (OP_ID=NVL(OLD_ID,0) AND OP_SUBID=NVL(OLD_SUBID,0)) ;
  END IF ;

  IF exist>0 THEN
    SELECT dup INTO D FROM objs WHERE name=doctype ;
    IF D IS NULL THEN D:=0 ; END IF ;
    IF D=0 AND range_exist>0 THEN
      result:=3 ;
      RETURN ;
    ELSE
       SELECT MAX(doc_num_v)+1 INTO docnum_v FROM opers
          WHERE doc_type=doctype AND doc_num=docnum
            AND NOT (OP_ID=OPER_ID AND OP_SUBID=OPER_SUBID) ;

       -- Сдублировали версию
       IF D>1 AND range_exist>0 THEN
         result:=10 ; RETURN ;
       END IF ;
    END IF ;
  END IF ;

  result:=0 ;
EXCEPTION
  WHEN NO_DATA_FOUND THEN result:=2 ; RETURN ;
END ;

PROCEDURE ApplyUndoItem(undo NUMBER,oper opers%ROWTYPE,cpdate DATE,
                        item op_items%ROWTYPE,need_check NUMBER) IS
itemsum    NUMBER ;
endval     NUMBER ;
turn       NUMBER ;
citemsum   NUMBER ;
turn_exist NUMBER ;
corturnid  NUMBER ;
corturnsum NUMBER ;
par_code   NUMBER ;
sumid      NUMBER ;
undo_sign  NUMBER ;

anal_id    NUMBER ;
anal_name  VARCHAR2(30) ;
setname    VARCHAR2(20) ;
func_str   VARCHAR2(200) ;
cid        NUMBER ;
sql_res    NUMBER ;
operid     NUMBER ;
sign       NUMBER ;

CURSOR c_items_cursor IS
  SELECT * FROM op_items
  WHERE op_id=oper.op_id AND op_subid=oper.op_subid AND side='K' AND
        node=0 AND for_post=0 ;

CURSOR cints_cursor IS
  SELECT * FROM cints
  WHERE end_date=cpdate AND oper.op_date>=beg_date ;

BEGIN
 IF oper.post=item.for_post THEN

    IF item.setname IS NULL THEN setname:='' ;
    ELSE setname:=item.setname ;
    END IF ;

    SELECT value INTO itemsum FROM sums WHERE sum_id=item.item_sum_id ;

    undo_sign:=1 ;
    IF (undo!=0 AND oper.storno=0) OR
       (undo=0 AND oper.storno!=0) THEN
       itemsum:=-itemsum ;
       undo_sign:=-1 ;
    END IF ;

    /* updating endval */
    SELECT end_val_id INTO sumid FROM rems WHERE code=item.code AND rem_date=cpdate ;
    IF item.side='D' THEN
      UPDATE sums SET value=value+itemsum WHERE sum_id=sumid ;
      sign:=undo_sign ;
    ELSE
      UPDATE sums SET value=value-itemsum WHERE sum_id=sumid ;
      sign:=-undo_sign ;
    END IF ;

    /* Analytic */
    SELECT anal_id INTO anal_id FROM accs WHERE code=item.code ;
    IF anal_id IS NOT NULL THEN
       SELECT name INTO anal_name FROM objs WHERE oid=anal_id ;
       func_str:='BEGIN ' || anal_name || '_' || setname || '_AAS(:sumid,:operid,:sign) ; END ;' ;

       cid:=DBMS_SQL.OPEN_CURSOR;
       DBMS_SQL.PARSE(cid,func_str,dbms_sql.v7);
       dbms_sql.bind_variable(cid,':sumid',sumid);
       operid:=item.item_sum_id ;
       dbms_sql.bind_variable(cid,':operid',operid);
       dbms_sql.bind_variable(cid,':sign',sign);
       sql_res:=dbms_sql.execute(cid);

    END IF ;

    /* updating intervals */
    FOR cint IN cints_cursor LOOP
      IF item.side='D' THEN
         /* updating debet turn */
         SELECT d_turn_id INTO sumid FROM turns
         WHERE code=item.code AND beg_date=cint.beg_date AND end_date=cint.end_date ;
         UPDATE sums SET value=value+itemsum WHERE sum_id=sumid ;
      ELSE
         /* updating credit turn */
         SELECT c_turn_id INTO sumid FROM turns
         WHERE code=item.code AND beg_date=cint.beg_date AND end_date=cint.end_date ;
         UPDATE sums SET value=value+itemsum WHERE sum_id=sumid ;
      END IF ;

      IF anal_id IS NOT NULL THEN
         dbms_sql.bind_variable(cid,':sumid',sumid);
         dbms_sql.bind_variable(cid,':operid',operid);
         dbms_sql.bind_variable(cid,':sign',undo_sign);
         sql_res:=dbms_sql.execute(cid);
      END IF ;

    END LOOP ;

    IF anal_id IS NOT NULL THEN
       DBMS_SQL.CLOSE_CURSOR(cid);
    END IF ;
 END IF ;

 DELETE FROM unapp_items WHERE op_id=oper.op_id AND op_subid=oper.op_subid
                               AND code=item.code AND cp_date=cpdate ;
 COMMIT ;
END ;

PROCEDURE ApplyUndoCorItem(undo NUMBER,oper opers%ROWTYPE,cpdate DATE,
                           item cor_items%ROWTYPE) IS
CURSOR cints_cursor IS
  SELECT * FROM cints
  WHERE end_date=cpdate AND oper.op_date>=beg_date ;
itemsum    NUMBER ;
turn_exist NUMBER ;
turnid     NUMBER ;
turnsum    NUMBER ;
BEGIN
    SELECT value INTO itemsum FROM sums WHERE sum_id=item.item_sum_id ;

    IF (undo!=0 AND oper.storno=0) OR
       (undo=0 AND oper.storno!=0) THEN
       itemsum:=-itemsum ;
    END IF ;

    FOR cint IN cints_cursor LOOP
      SELECT COUNT(*) INTO turn_exist FROM cor_turns
      WHERE beg_date=cint.beg_date AND end_date=cint.end_date AND d_code=item.d_code AND c_code=item.c_code ;

      IF turn_exist=1 THEN
        SELECT turn_id INTO turnid FROM cor_turns
        WHERE beg_date=cint.beg_date AND end_date=cint.end_date AND d_code=item.d_code AND c_code=item.c_code ;
        SELECT value INTO turnsum FROM sums WHERE sum_id=turnid ;
        IF (turnsum+itemsum=0) AND undo=1 AND itemsum!=0 THEN
          DELETE FROM cor_turns
            WHERE beg_date=cint.beg_date AND end_date=cint.end_date AND
                  d_code=item.d_code AND c_code=item.c_code ;
          DELETE FROM sums WHERE sum_id=turnid ;
        ELSE
          UPDATE sums SET value=value+itemsum
          WHERE sum_id=turnid ;
        END IF ;
      ELSE
        CreateSum(turnid,itemsum) ;
        INSERT INTO cor_turns VALUES
           (cint.beg_date,cint.end_date,item.d_code,item.c_code,turnid) ;
      END IF ;
    END LOOP ;

    DELETE FROM unapp_cor_items
      WHERE op_id=oper.op_id AND op_subid=oper.op_subid AND
            d_code=item.d_code AND c_code=item.c_code AND cp_date=cpdate ;
    COMMIT ;
END ;


PROCEDURE CheckPositive(oper_id NUMBER,oper_subid NUMBER,neg OUT NUMBER,undo NUMBER) IS
opts       NUMBER ;
fstr       VARCHAR2(500) ;
check_fld  VARCHAR2(30) ;
session_id VARCHAR2(100) ;
cursid     NUMBER ;
sqlres     NUMBER ;
operid     NUMBER ;
tneg       NUMBER ;
CURSOR items_cursor IS
     SELECT i.*,a.anal_id,o.name as anal_name FROM op_items i,accs a,objs o
        WHERE op_id=oper_id AND op_subid=oper_subid AND
              i.code=a.code AND o.oid=a.anal_id;
CURSOR opt_curs_null(anal_id NUMBER) IS
     SELECT def_opt,realname FROM dbstr WHERE dbid=anal_id
     ORDER BY ord DESC ;
CURSOR opt_curs_set(anal_id NUMBER,setname VARCHAR2) IS
     SELECT opt,o.realname FROM dboptov o,dbstr s
     WHERE o.dbid=anal_id AND o.dbid=s.dbid AND o.setname=setname
           AND o.realname=s.realname
     ORDER BY ord DESC ;
BEGIN
 neg:=0 ;
 FOR item IN items_cursor LOOP
   SELECT opts INTO opts FROM ACCS WHERE code=item.code ;
   IF MOD(opts,2)=1 THEN -- Включена проверка положительности
      -- Находим поле для проверки
      IF item.setname IS NOT NULL then
         FOR anal_opt IN opt_curs_set(item.anal_id,item.setname) LOOP
             IF anal_opt.opt>=262144 THEN
                check_fld:=anal_opt.realname ;
             END IF ;
         END LOOP ;
      end if;

      if check_fld is NULL then
         FOR anal_opt IN opt_curs_null(item.anal_id) LOOP
             IF anal_opt.def_opt>=262144 THEN
                check_fld:=anal_opt.realname ;
             END IF ;
         END LOOP ;
      end if;

      if check_fld is NULL then
        RAISE_APPLICATION_ERROR(-20001,'Для аналитики'||item.anal_name||
          ' не указано поле РАСПРЕДЕЛЯТЬ');
      end if;


      session_id:=USERENV('SESSIONID') ;
      DELETE anal_errs WHERE sid=session_id AND code=item.code ;

      fstr:='INSERT INTO anal_errs '||
            'SELECT '||session_id||','||TO_CHAR(item.code)||',O.AARTICLE, O.AVER,'||TO_CHAR(undo)||
            ' FROM '||item.anal_name||' T,'||item.anal_name||' O'||
            ' WHERE O.SUMID=:sumid AND T.SUMID=:operid AND O.AARTICLE!='' '' AND '||
                   'O.AARTICLE=T.AARTICLE AND O.AVER=T.AVER AND '||
                   'O.'||check_fld||'<0' ;

      SELECT end_val_id INTO operid FROM rems WHERE rem_date=
             (select max(point_date) from cps) AND code=item.code ;

      cursid:=DBMS_SQL.OPEN_CURSOR;
      DBMS_SQL.PARSE(cursid,fstr,dbms_sql.v7);
      dbms_sql.bind_variable(cursid,':operid',item.item_sum_id);
      dbms_sql.bind_variable(cursid,':sumid',operid);
      sqlres:=dbms_sql.execute(cursid);
      DBMS_SQL.CLOSE_CURSOR(cursid);

      SELECT COUNT(*) INTO tneg FROM anal_errs
         WHERE SID=session_id AND code=item.code ;

      IF tneg>0 THEN
         neg:=9 ;
         IF HAVEROLE('NEGREMS')>0 THEN
            neg:=11 ;
         END IF ;
      END IF ;

   END IF ;
 END LOOP ;
END ;


PROCEDURE ApplyUndoOper(oper_id NUMBER,oper_subid NUMBER) IS
oper opers%ROWTYPE ;
item op_items%ROWTYPE ;
cor_item cor_items%ROWTYPE ;
CURSOR items_cursor IS
  SELECT i.op_id,i.op_subid,i.code,i.cp_date,o.undo FROM unapp_items i,unapp_opers o
  WHERE o.op_id=oper_id AND o.op_subid=oper_subid AND
        i.op_id=o.op_id AND i.op_subid=o.op_subid ;
CURSOR cor_items_cursor IS
  SELECT i.op_id,i.op_subid,i.d_code,i.c_code,i.cp_date,o.undo FROM unapp_cor_items i,unapp_opers o
  WHERE o.op_id=oper_id AND o.op_subid=oper_subid AND
        i.op_id=o.op_id AND i.op_subid=o.op_subid ;
BEGIN
 SELECT * INTO oper FROM opers WHERE op_id=oper_id AND op_subid=oper_subid ;

 FOR ua IN items_cursor LOOP
   SELECT * INTO item FROM op_items
      WHERE op_id=ua.op_id AND op_subid=ua.op_subid AND code=ua.code ;
   ApplyUndoItem(ua.undo,oper,ua.cp_date,item,1) ;
 END LOOP ;

-- IF oper.post=0 THEN
   FOR ua IN cor_items_cursor LOOP
       SELECT * INTO cor_item FROM cor_items WHERE
            op_id=ua.op_id AND op_subid=ua.op_subid AND
            d_code=ua.d_code AND c_code=ua.c_code ;
       ApplyUndoCorItem(ua.undo,oper,ua.cp_date,cor_item) ;
   END LOOP ;
-- END IF ;

 DELETE unapp_opers WHERE op_id=oper_id AND op_subid=oper_subid ;
 COMMIT ;
END ;

FUNCTION ApplyUndoAll(session_id NUMBER,max_count NUMBER) RETURN NUMBER IS
oper opers%ROWTYPE ;
ret  number ;
CURSOR unapp_cursor IS
    SELECT op_id,op_subid FROM unapp_opers WHERE SID=session_id ;
BEGIN
  ret:=0 ;
  FOR ua IN unapp_cursor LOOP
     ApplyUndoOper(ua.op_id,ua.op_subid) ;
     ret:=ret+1 ;
     IF ret>=max_count THEN EXIT ; END IF ;
  END LOOP ;
  return ret ;
END ;

FUNCTION ApplyUndoAllDead(max_count NUMBER) RETURN NUMBER IS
oper opers%ROWTYPE ;
ret  number ;
CURSOR unapp_cursor IS
    SELECT op_id,op_subid FROM unapp_opers
      WHERE SID NOT IN
        (SELECT audsid from V$SESSION) ;
BEGIN
  ret:=0 ;
  FOR ua IN unapp_cursor LOOP
     ApplyUndoOper(ua.op_id,ua.op_subid) ;
     ret:=ret+1 ;
     IF ret>=max_count THEN EXIT ; END IF ;
  END LOOP ;
  return ret ;
END ;

PROCEDURE InputOperHeader(oper_id OUT NUMBER,oper_subid NUMBER,post NUMBER,
                          storno NUMBER,oper_text VARCHAR2,
                          doc_type VARCHAR2,doc_num VARCHAR2,doc_num_v NUMBER,
                          oper_date DATE,cond_id NUMBER) IS
BEGIN
 SELECT OperSeq.NEXTVAL INTO oper_id FROM DUAL ;
 INSERT INTO opers   (op_id  ,op_subid  ,status,post,storno,text     ,doc_type,doc_num,doc_num_v,op_date  ,cond_id)
             VALUES  (oper_id,oper_subid,0     ,post,storno,oper_text,doc_type,doc_num,doc_num_v,oper_date,cond_id) ;
END ;

PROCEDURE InputOperItem(oper_id NUMBER,oper_subid NUMBER,code VARCHAR2,
                        side VARCHAR2,item_sum_id OUT NUMBER,item_sum NUMBER,
                        for_post NUMBER,set_name VARCHAR2) IS
BEGIN
 CreateSum(item_sum_id,item_sum) ;
 INSERT INTO op_items VALUES
  (oper_id,oper_subid,NULL,GetCode(code),side,item_sum_id,0,for_post,set_name) ;
END ;

PROCEDURE AddParentItem(item op_items%ROWTYPE) IS
acc accs%ROWTYPE ;
par accs%ROWTYPE ;
par_item op_items%ROWTYPE ;
exist NUMBER ;
itemsum NUMBER ;
prevsum NUMBER ;
new_id NUMBER ;
anal NUMBER ;
BEGIN
  SELECT * INTO acc FROM accs WHERE item.code=accs.code ;
  IF acc.parent IS NOT NULL THEN
    SELECT value INTO itemsum FROM sums WHERE sum_id=item.item_sum_id ;

    SELECT * INTO par FROM accs WHERE code=acc.parent ;
    IF acc.anal_id IS NOT NULL AND acc.anal_id=par.anal_id THEN anal:=1 ;
    ELSE anal:=0 ; END IF ;

    /* Does parent item already exist */
    SELECT count(*) INTO exist FROM op_items
    WHERE op_id=item.op_id AND op_subid=item.op_subid AND code=acc.parent ;

    IF exist=1 THEN
      SELECT * INTO par_item FROM op_items
      WHERE op_id=item.op_id AND op_subid=item.op_subid AND code=acc.parent ;
      SELECT value INTO prevsum FROM sums WHERE sum_id=par_item.item_sum_id ;

      IF par_item.side=item.side THEN
         IF par_item.node=1 THEN /* was reference */
            CreateSum(new_id,itemsum+prevsum) ;
            UPDATE op_items SET item_sum_id=new_id,node=2
               WHERE op_id=item.op_id AND op_subid=item.op_subid AND code=acc.parent ;
            IF anal=1 THEN
               CopyAnalSum(acc.anal_id,new_id,par_item.item_sum_id) ;
               AddAnalSum(acc.anal_id,new_id,item.item_sum_id,1) ;
            END IF ;
         ELSE  /* was own sum */
            UPDATE sums SET value=value+itemsum
            WHERE sum_id=par_item.item_sum_id ;
            IF anal=1 THEN
               AddAnalSum(acc.anal_id,par_item.item_sum_id,item.item_sum_id,1) ;
            END IF ;
         END IF ;
      ELSE /* par_item.side!=item.side */
         IF itemsum=prevsum THEN
            DELETE FROM op_items
               WHERE op_id=item.op_id AND op_subid=item.op_subid AND code=acc.parent ;
            IF par_item.node=2 THEN /* was own sum */
               DELETE FROM sums WHERE sum_id=par_item.item_sum_id ;
            END IF ;
         ELSIF itemsum<prevsum THEN
            IF par_item.node=1 THEN /* was reference */
               CreateSum(new_id,prevsum-itemsum) ;
               UPDATE op_items SET item_sum_id=new_id,node=2
                  WHERE op_id=item.op_id AND op_subid=item.op_subid AND code=acc.parent ;
               IF anal=1 THEN
                  CopyAnalSum(acc.anal_id,new_id,par_item.item_sum_id) ;
                  AddAnalSum(acc.anal_id,new_id,item.item_sum_id,-1) ;
               END IF ;
            ELSE  /* was own sum */
               UPDATE sums SET value=value-itemsum
                  WHERE sum_id=par_item.item_sum_id ;
               IF anal=1 THEN
                  AddAnalSum(acc.anal_id,par_item.item_sum_id,item.item_sum_id,-1) ;
               END IF ;
            END IF ;
         ELSE  /* need to change side */
            UPDATE op_items SET side=OtherSide(side)
            WHERE op_id=item.op_id AND op_subid=item.op_subid AND code=acc.parent ;

            IF par_item.node=1 THEN /* was reference */
               CreateSum(new_id,itemsum-prevsum) ;
               UPDATE op_items SET item_sum_id=new_id,node=2
                  WHERE op_id=item.op_id AND op_subid=item.op_subid AND code=acc.parent ;
               IF anal=1 THEN
                  CopyAnalSum(acc.anal_id,new_id,item.item_sum_id) ;
                  AddAnalSum(acc.anal_id,new_id,par_item.item_sum_id,-1) ;
               END IF ;
            ELSE  /* was own sum */
               UPDATE sums SET value=itemsum-value
                  WHERE sum_id=par_item.item_sum_id ;
               IF anal=1 THEN
                  AddAnalSum(acc.anal_id,item.item_sum_id,par_item.item_sum_id,-1) ;
               END IF ;
            END IF ;
         END IF ;
      END IF ;
    ELSE /* parent item not created yet */
      IF (itemsum!=0) OR (par.parent IS NOT NULL) THEN
         INSERT INTO op_items VALUES
            (item.op_id,item.op_subid,item.op_date,acc.parent,item.side,item.item_sum_id,1,
             item.for_post,item.setname) ;
      END IF ;
    END IF ;

    par_item:=item ;
    par_item.code:=acc.parent ;
    AddParentItem(par_item) ;
  END IF ;
END ;

PROCEDURE AddCorItems(oper opers%ROWTYPE) IS
CURSOR d_items IS
  SELECT op_items.* FROM op_items,accs
  WHERE op_id=oper.op_id AND op_subid=oper.op_subid AND side='D' AND
        for_post=0 AND accs.code=op_items.code AND bal=1 ;
CURSOR c_items(dn VARCHAR2) IS
  SELECT op_items.* FROM op_items,accs
  WHERE op_id=oper.op_id AND op_subid=oper.op_subid AND side='K' AND
        for_post=0 AND op_items.code=accs.code AND bal=1 AND
        INSTR(name,dn)=0 AND INSTR(dn,name)=0 ;
ditemsum  NUMBER ;
citemsum  NUMBER ;
opersum   NUMBER ;
opsum     NUMBER ;
redsum    NUMBER ;
corturnid NUMBER ;
turnsum   NUMBER ;
dname     VARCHAR2(20) ;
cname     VARCHAR2(20) ;
BEGIN
  FOR d_item IN d_items LOOP
     SELECT value INTO ditemsum FROM sums WHERE sum_id=d_item.item_sum_id ;
     SELECT name INTO dname FROM accs WHERE code=d_item.code ;

     SELECT SUM(value) INTO opsum
     FROM sums , op_items, accs
     WHERE op_id=oper.op_id AND op_subid=oper.op_subid AND
           side='K' AND op_items.node=0 AND for_post=0 AND sum_id=item_sum_id AND
           op_items.code=accs.code AND INSTR(name,dname)=0 AND INSTR(dname,name)=0 AND
           bal=1 ;

     FOR c_item IN c_items(dname) LOOP
       SELECT value INTO citemsum FROM sums WHERE sum_id=c_item.item_sum_id ;
       SELECT name INTO cname FROM accs WHERE code=c_item.code ;

       SELECT SUM(value) INTO redsum
       FROM sums , op_items , accs
       WHERE op_id=oper.op_id AND op_subid=oper.op_subid AND
             side='D' AND op_items.node=0 AND for_post=0 AND sum_id=item_sum_id AND
             op_items.code=accs.code AND INSTR(name,cname)!=0 ;
       IF redsum IS NOT NULL THEN
          opersum:=opsum-redsum ;
       ELSE
          opersum:=opsum ;
       END IF ;

       IF opersum=0 THEN
          IF ditemsum=0 THEN
            turnsum:=citemsum ;
          ELSIF citemsum=0 THEN
            turnsum:=ditemsum ;
          ELSE
            turnsum:=0 ;
          END IF ;
       ELSE
          turnsum:=ROUND(ditemsum*citemsum/opersum,2) ;
       END IF ;

       CreateSum(corturnid,turnsum) ;
       INSERT INTO cor_items VALUES
               (oper.op_id,oper.op_subid,d_item.code,c_item.code,corturnid) ;
     END LOOP ;
  END LOOP ;
END ;


PROCEDURE ValidateOper(oper_id NUMBER,oper_subid NUMBER,new_status NUMBER,
                       old_id NUMBER,old_subid NUMBER,result OUT NUMBER) IS
oper opers%ROWTYPE ;
old_oper opers%ROWTYPE ;
cond conds%ROWTYPE ;
main_item NUMBER ;
node_items NUMBER ;
res NUMBER ;
rights privtest%ROWTYPE ;
all_r  NUMBER ;
can_modify NUMBER ;
rootsum NUMBER ;
min_date DATE ;
max_date DATE ;
check_this_accs NUMBER ;
check_old_accs NUMBER ;
CURSOR items_cursor IS
  SELECT * FROM op_items
  WHERE op_id=oper_id AND op_subid=oper_subid AND node=0 ;
BEGIN

 SELECT * INTO oper FROM opers WHERE op_id=oper_id AND op_subid=oper_subid ;
 IF old_id IS NOT NULL THEN
    SELECT * INTO old_oper FROM opers WHERE op_id=old_id AND op_subid=old_subid ;
 END IF ;

 UPDATE op_items SET op_date=oper.op_date WHERE op_id=oper_id AND op_subid=oper_subid ;

 /* adding all parent items */
 FOR item IN items_cursor LOOP
    AddParentItem(item) ;
 END LOOP ;

 /* adding all cor_items */
 AddCorItems(oper) ;

 /* verification */
 res:=0 ;

 -- Проверка даты
 SELECT MIN(point_date) INTO min_date FROM cps ;
 IF oper.op_date<min_date THEN res:=5 ; END IF ;

 SELECT MAX(point_date) INTO max_date FROM cps ;
 IF oper.op_date>=max_date THEN res:=5 ; END IF ;

 IF HaveRole_('DEADDATE')=0 THEN
    SELECT dead_date INTO min_date FROM user_options WHERE username=USER ;
    IF min_date IS NOT NULL AND oper.op_date<min_date THEN res:=5 ; END IF ;
 END IF ;

 -- Проверка на "родной" тип документа
 IF oper.cond_id IS NOT NULL THEN
   SELECT * INTO cond FROM conds
     WHERE cond_id=oper.cond_id AND cond_subid=oper.op_subid ;
   IF cond.native_doc>0 AND cond.doc_type!=oper.doc_type THEN
     res:=13 ;
   END IF ;
 END IF ;

 -- Проверки только для проводимых по балансу операций

 IF oper.post=0 THEN
    -- Суммы не совпадают (дебет с кредитом)
    SELECT COUNT(*) INTO main_item FROM op_items,accs
      WHERE op_id=oper_id AND op_subid=oper_subid AND op_items.code=accs.code
        AND (accs.name='A' OR accs.name='B');
    IF main_item>0 THEN
       SELECT VALUE INTO rootsum FROM op_items,accs,sums
         WHERE op_id=oper_id AND op_subid=oper_subid AND op_items.code=accs.code
           AND (accs.name='A' OR accs.name='B') AND item_sum_id=sum_id ;
       IF rootsum!=0 THEN
          res:=1 ;
       END IF ;
    END IF ;
  END IF ;

    -- Совпадение аналитики и синтетики
    DECLARE
      cid NUMBER ;
      sql_res NUMBER ;
      anal_name VARCHAR2(30) ;
      func_str  VARCHAR2(300) ;
      itogo_val NUMBER ;
      pos_itogo_val NUMBER ;
      sum_id NUMBER ;
      set_exist NUMBER ;
      CURSOR anal_info IS
        SELECT a.code,name,anal_id,bal_fld,value,sum_id,i.setname FROM op_items i,accs a,sums
        WHERE op_id=oper_id AND op_subid=oper_subid AND a.code=i.code
              AND item_sum_id=sum_id AND anal_id IS NOT NULL ;
    BEGIN
      cid:=DBMS_SQL.OPEN_CURSOR ;
      FOR AI in anal_info LOOP
          SELECT name INTO anal_name FROM objs WHERE oid=AI.anal_id ;

          /* Verifying setname */
          SELECT COUNT(*) INTO set_exist FROM dbopt
            WHERE dbid=AI.anal_id AND setname=AI.setname ;
          IF set_exist=0 THEN
            UPDATE op_items SET setname=''
              WHERE op_id=oper_id and op_subid=oper_subid AND code=AI.code ;
          END IF ;

          /* Updating op_date fld */
          func_str:='UPDATE ' || anal_name || ' SET op_date=:op_date WHERE sumid=:sum_id' ;
          DBMS_SQL.PARSE(cid,func_str,dbms_sql.v7);
          dbms_sql.bind_variable(cid,':sum_id',AI.sum_id);
          dbms_sql.bind_variable(cid,':op_date',oper.op_date);
          sql_res:=dbms_sql.execute(cid) ;

          IF oper.post=0 THEN
           func_str:='BEGIN SELECT ' || AI.bal_fld || ' INTO :itogo_val FROM ' ||
                     anal_name || ' WHERE sumid=:sum_id AND AARTICLE='' '' AND AVER=0 ; END ;' ;
           DBMS_SQL.PARSE(cid,func_str,dbms_sql.v7);
           dbms_sql.bind_variable(cid,':sum_id',AI.sum_id);
           dbms_sql.bind_variable(cid,':itogo_val',itogo_val);
           sql_res:=dbms_sql.execute(cid);
           dbms_sql.variable_value(cid,':itogo_val',itogo_val);
           IF AI.value!=itogo_val THEN res:=6 ; END IF ;

           func_str:='BEGIN SELECT SUM(' || AI.bal_fld || ') INTO :pos_itogo_val FROM ' ||
                     anal_name || ' WHERE sumid=:sum_id AND AARTICLE!='' '' ; END ;' ;
           DBMS_SQL.PARSE(cid,func_str,dbms_sql.v7);
           dbms_sql.bind_variable(cid,':sum_id',AI.sum_id);
           dbms_sql.bind_variable(cid,':pos_itogo_val',pos_itogo_val);
           sql_res:=dbms_sql.execute(cid);
           dbms_sql.variable_value(cid,':pos_itogo_val',pos_itogo_val);
           IF pos_itogo_val!=itogo_val THEN res:=7 ; END IF ;
          END IF ;
      END LOOP ;
      DBMS_SQL.CLOSE_CURSOR(cid);
    EXCEPTION
      WHEN NO_DATA_FOUND THEN res:=6 ;
    END ;

 IF oper.post=0 THEN
    -- Операция не по листу
    SELECT COUNT(*) INTO node_items
    FROM op_items i,accs a
    WHERE op_id=oper_id AND op_subid=oper_subid AND i.code=a.code AND
          i.node=0 AND a.node>0  ;
    IF node_items>0 THEN res:=2 ; END IF ;
 END IF ;

 -- Балансовый счет для откладывания
 SELECT COUNT(*) INTO node_items
 FROM op_items i,accs a
 WHERE op_id=oper_id AND op_subid=oper_subid AND i.code=a.code AND
       i.for_post=1 AND a.bal=1 ;
 IF node_items>0 THEN res:=3 ; END IF ;

 -- Проверка прав
 IF oper.cond_id IS NOT NULL THEN
    BEGIN
      SELECT * INTO rights FROM privtest WHERE poid=oper.cond_id AND uname=USER ;
      IF rights.pr0=0 AND rights.pr1=0 THEN res:=4 ; END IF ;
      can_modify:=rights.pr1 ;
      all_r:=0 ;
    EXCEPTION
      WHEN NO_DATA_FOUND THEN res:=4 ;
    END ;
 ELSE
    all_r:=HaveRole('INPUTOPER') ;
    IF all_r=0 THEN res:=4 ; END IF ;
    can_modify:=all_r ;
 END IF ;

 check_this_accs:=0 ;
 IF oper.post=0 THEN check_this_accs:=1 ; END IF ;
 IF old_id IS NOT NULL THEN
   IF old_oper.post=0 AND oper.post=1 THEN check_this_accs:=1 ; END IF ;
 END IF ;

 check_old_accs:=0 ;
 IF old_id IS NOT NULL THEN
   IF old_oper.post=0 THEN check_old_accs:=1 ; END IF ;
 END IF ;

 IF check_this_accs=1 THEN
    IF can_modify=0 THEN res:=4 ; END IF ;

    DECLARE
     num_good_accs NUMBER ;
     num_accs NUMBER ;
    BEGIN
      SELECT COUNT(*) INTO num_good_accs FROM op_items,privtest
        WHERE op_id=oper_id AND op_subid=oper_subid AND code=poid AND uname=USER AND pr1=1 ;
      SELECT COUNT(*) INTO num_accs FROM op_items
        WHERE op_id=oper_id AND op_subid=oper_subid ;
      IF num_accs!=num_good_accs THEN res:=4 ; END IF ;

      IF check_old_accs=1 THEN
        SELECT COUNT(*) INTO num_good_accs FROM op_items,privtest
          WHERE op_id=old_id AND op_subid=old_subid AND code=poid AND uname=USER AND pr1=1 ;
        SELECT COUNT(*) INTO num_accs FROM op_items
          WHERE op_id=old_id AND op_subid=old_subid ;
        IF num_accs!=num_good_accs THEN res:=4 ; END IF ;
      END IF ;
    END ;
 END IF ;

 IF res=0 THEN
      UPDATE opers SET status=new_status
        WHERE op_id=oper_id AND op_subid=oper_subid ;
 ELSE
    DELETE op_items WHERE op_id=oper_id AND op_subid=oper_subid AND node>0 ;
    DELETE cor_items WHERE op_id=oper_id AND op_subid=oper_subid ;
 END IF ;
 result:=res ;
END ;

PROCEDURE PrepareToApply(oper_id NUMBER,oper_subid NUMBER,oper_date DATE,
                         undo NUMBER) IS
post NUMBER ;
session_id NUMBER ;
BEGIN
 session_id:=USERENV('SESSIONID') ;
 INSERT INTO unapp_opers (OP_ID,OP_SUBID,UNDO,SID)
                   VALUES(oper_id,oper_subid,undo,session_id) ;

 INSERT INTO unapp_items (OP_ID,OP_SUBID,CODE,CP_DATE)
    SELECT oper_id,oper_subid,code,point_date FROM cps,op_items
    WHERE oper_date<point_date AND op_id=oper_id AND op_subid=oper_subid ;

 SELECT post INTO post FROM opers WHERE op_id=oper_id AND op_subid=oper_subid ;
 IF post=0 THEN
   INSERT INTO unapp_cor_items
      SELECT oper_id,oper_subid,d_code,c_code,point_date FROM cps,cor_items
      WHERE oper_date<point_date AND op_id=oper_id AND op_subid=oper_subid ;
 END IF ;
END;

PROCEDURE CalcDiffOper(new_id NUMBER,new_subid NUMBER,
                       old_id NUMBER,old_subid NUMBER,
                       dif_id OUT NUMBER,diff_fail OUT NUMBER) IS
CURSOR new_items IS
  SELECT * FROM op_items WHERE op_id=new_id AND op_subid=new_subid ;
CURSOR old_items IS
  SELECT * FROM op_items WHERE op_id=old_id AND op_subid=old_subid ;
sid       NUMBER ;
isum      NUMBER ;
old_exist NUMBER ;
diff_id   NUMBER ;
new_date  DATE ;
anal_id   NUMBER ;
oside     VARCHAR2(1) ;
BEGIN
  SELECT OperSeq.NEXTVAL INTO diff_id FROM DUAL ;
  dif_id:=diff_id ;
  SELECT op_date INTO new_date FROM opers WHERE op_id=new_id AND op_subid=new_subid ;
  INSERT INTO opers (op_id  ,op_subid,status,post,op_date ,text     ,storno) VALUES
                    (diff_id,0       ,2     ,0   ,new_date,'Diff'   ,0) ;

  /* Пишем в разницу все старые item'ы с минусом */
  FOR oitem IN old_items LOOP
     SELECT value INTO isum FROM sums WHERE sum_id=oitem.item_sum_id ;
     CreateSum(sid,-isum) ;
     INSERT INTO op_items VALUES (diff_id,0,new_date,oitem.code,oitem.side,sid,
                                  oitem.node,oitem.for_post,oitem.setname) ;
     /* аналитика */
     SELECT anal_id INTO anal_id FROM accs WHERE code=oitem.code ;
     AddAnalSum(anal_id,sid,oitem.item_sum_id,-1) ;
  END LOOP ;

  /* перебираем все новые item'ы */
  FOR nitem IN new_items LOOP
     SELECT COUNT(*) INTO old_exist FROM op_items
       WHERE op_id=diff_id AND op_subid=0 AND code=nitem.code ;
     SELECT anal_id INTO anal_id FROM accs WHERE code=nitem.code ;
     IF old_exist=1 THEN
        SELECT side INTO oside FROM op_items
           WHERE op_id=diff_id AND op_subid=0 AND code=nitem.code ;
        SELECT value INTO isum FROM sums WHERE sum_id=nitem.item_sum_id ;
        SELECT item_sum_id INTO sid FROM op_items
           WHERE op_id=diff_id AND op_subid=0 AND code=nitem.code ;
        IF oside!=nitem.side THEN
           diff_fail:=1 ; RETURN ;
        END IF ;
        UPDATE sums SET value=value+isum WHERE sum_id=sid ;
        /* аналитика */
        AddAnalSum(anal_id,sid,nitem.item_sum_id,1) ;
     ELSIF old_exist=0 THEN
        SELECT value INTO isum FROM sums WHERE sum_id=nitem.item_sum_id ;
        CreateSum(sid,isum) ;
        INSERT INTO op_items VALUES (diff_id,0,new_date,nitem.code,nitem.side,sid,
                                     nitem.node,nitem.for_post,nitem.setname) ;
        /* аналитика */
        CopyAnalSum(anal_id,sid,nitem.item_sum_id) ;
     END IF ;
  END LOOP ;
  diff_fail:=0 ;
END ;

PROCEDURE ApplyOper(oper_id NUMBER,oper_subid NUMBER,result OUT NUMBER,message OUT VARCHAR2) IS
res NUMBER ;
oper_date DATE ;
BEGIN
 message:=NULL ;
 ValidateOper(oper_id,oper_subid,1,NULL,NULL,res) ;
 IF res=0 THEN
    SELECT op_date INTO oper_date FROM opers WHERE op_id=oper_id AND op_subid=oper_subid ;

    PrepareToApply(oper_id,oper_subid,oper_date,0) ;

    DECLARE
      max_rev NUMBER ;
    BEGIN
      SELECT MAX(revision)+1 INTO max_rev FROM HISTORY
                WHERE op_id=oper_id AND op_subid=oper_subid ;
      IF max_rev IS NULL THEN max_rev:=-1 ; END IF ;
      INSERT INTO HISTORY VALUES (oper_id,oper_subid,max_rev,SYSDATE,USER) ;
      COMMIT ;
    EXCEPTION
      WHEN OTHERS THEN
         SELECT MAX(revision) INTO max_rev FROM HISTORY
                WHERE op_id=oper_id AND op_subid=oper_subid ;
         IF max_rev IS NULL THEN max_rev:=0 ; END IF ;
         INSERT INTO HISTORY VALUES (oper_id,oper_subid,max_rev+1,SYSDATE,USER) ;
    END ;


    ApplyUndoOper(oper_id,oper_subid) ;
    CheckPositive(oper_id,oper_subid,res,0) ;

    IF (res!=0) AND (res!=11) THEN
       PrepareToApply(oper_id,oper_subid,oper_date,1) ;
       DELETE HISTORY WHERE op_id=oper_id and op_subid=oper_subid and revision=-1 ;
       COMMIT ;
       ApplyUndoOper(oper_id,oper_subid) ;
       DELETE cor_items WHERE op_id=oper_id AND op_subid=oper_subid ;
       DELETE op_items WHERE op_id=oper_id AND op_subid=oper_subid AND node!=0 ;
       COMMIT ;
    END IF ;

 END IF ;
 result:=res ;
EXCEPTION
 WHEN OTHERS THEN
      DELETE unapp_items WHERE op_id=oper_id AND op_subid=oper_subid ;
      DELETE unapp_cor_items WHERE op_id=oper_id AND op_subid=oper_subid ;
      DELETE unapp_opers WHERE op_id=oper_id AND op_subid=oper_subid ;
      DELETE cor_items WHERE op_id=oper_id AND op_subid=oper_subid ;
      DELETE op_items WHERE op_id=oper_id AND op_subid=oper_subid AND node!=0 ;
      result:=10 ;
      message:=SQLERRM ;
END ;

PROCEDURE UpdateOper(old_id NUMBER,old_subid NUMBER,
                     oper_id NUMBER,oper_subid NUMBER,result OUT NUMBER) IS
diff_id NUMBER ;
no_diff NUMBER ;
old_oper opers%ROWTYPE ;
new_oper opers%ROWTYPE ;
res  NUMBER ;
res1 NUMBER ;
res2 NUMBER ;
max_rev NUMBER ;
diff_fail NUMBER ;
BEGIN

 SELECT * INTO old_oper FROM opers WHERE op_id=old_id AND op_subid=old_subid ;
 SELECT * INTO new_oper FROM opers WHERE op_id=oper_id AND op_subid=oper_subid ;

 ValidateOper(oper_id,oper_subid,3,old_oper.op_id,old_oper.op_subid,res) ;

 IF res=0 THEN

    /* проверка на принадлежность дат одному интервалу */
    IF old_oper.op_date=new_oper.op_date THEN
       no_diff:=0 ;
    ELSIF old_oper.op_date>new_oper.op_date THEN
       SELECT COUNT(*) INTO no_diff FROM cps
          WHERE point_date>=new_oper.op_date AND point_date<old_oper.op_date ;
    ELSE /* old_date<new_date */
       SELECT COUNT(*) INTO no_diff FROM cps
          WHERE point_date>=old_oper.op_date AND point_date<new_oper.op_date ;
    END IF ;

    IF old_oper.storno!=new_oper.storno OR old_oper.post=1 OR new_oper.post=1 THEN
       no_diff:=1 ;
    END IF ;

    -- History
    SELECT MAX(revision) INTO max_rev FROM HISTORY
           WHERE op_id=old_id AND op_subid=old_subid ;
    IF max_rev IS NULL THEN max_rev:=0 ; END IF ;
    INSERT INTO HISTORY VALUES (old_id,old_subid,max_rev+1,SYSDATE,USER) ;

    no_diff:=1 ;

    IF no_diff=0 THEN
      CalcDiffOper(oper_id,oper_subid,old_id,old_subid,diff_id,diff_fail) ;

      IF diff_fail=1 THEN
         no_diff:=1 ;
         DELETE op_items WHERE op_id=diff_id AND op_subid=0 ;
         DELETE cor_items WHERE op_id=diff_id AND op_subid=0 ;
         DELETE opers WHERE op_id=diff_id AND op_subid=0 ;
      ELSE
         PrepareToApply(diff_id,0,new_oper.op_date,0) ;
         COMMIT ;
         ApplyUndoOper(diff_id,0) ;
      END IF ;
    END IF ;

    IF no_diff=1 THEN
      /* откатываем старую и проводим новую */
      PrepareToApply(old_id,old_subid,old_oper.op_date,1) ;
      PrepareToApply(oper_id,oper_subid,new_oper.op_date,0) ;
      COMMIT ;
      ApplyUndoOper(old_id,old_subid) ;
      ApplyUndoOper(oper_id,oper_subid) ;

      CheckPositive(old_id,old_subid,res1,1) ;
      CheckPositive(oper_id,oper_subid,res2,0) ;
      IF (res1!=0 AND res1!=11) OR (res2!=0 AND res2!=11) THEN
         PrepareToApply(old_id,old_subid,old_oper.op_date,0) ;
         PrepareToApply(oper_id,oper_subid,new_oper.op_date,1) ;
         DELETE HISTORY WHERE op_id=old_id and op_subid=old_subid and revision=max_rev+1 ;
         COMMIT ;
         ApplyUndoOper(old_id,old_subid) ;
         ApplyUndoOper(oper_id,oper_subid) ;
         DELETE cor_items WHERE op_id=oper_id AND op_subid=oper_subid ;
         DELETE op_items WHERE op_id=oper_id AND op_subid=oper_subid AND node!=0 ;
         COMMIT ;
         res:=9 ;
      END IF ;
    END IF ;

    /* в случае успеха */
    IF res=0 THEN
       /* удаляем старый вариант */
       DELETE op_items WHERE op_id=old_id AND op_subid=old_subid ;
       DELETE cor_items WHERE op_id=old_id AND op_subid=old_subid ;
       DELETE opers WHERE op_id=old_id AND op_subid=old_subid ;

       /* переписываем новый на место старого */
       INSERT INTO opers (op_id  ,op_subid  ,status,post,storno,text     ,doc_type,doc_num,doc_num_v,op_date,cond_id)
           SELECT old_id,old_subid,1,post,storno,text,doc_type,doc_num,doc_num_v,op_date,cond_id
           FROM opers
           WHERE op_id=oper_id AND op_subid=oper_subid ;
       UPDATE op_items SET op_id=old_id WHERE op_id=oper_id  AND op_subid=oper_subid ;
       UPDATE cor_items SET op_id=old_id WHERE op_id=oper_id AND op_subid=oper_subid  ;
       DELETE FROM OPERS WHERE op_id=oper_id AND op_subid=oper_subid ;

       COMMIT ;
    END IF ;
 END IF ;
 result:=res ;
END ;

PROCEDURE DeleteOperGroup(oper_id NUMBER,result OUT NUMBER,start_sub NUMBER) IS
CURSOR ops IS
  SELECT op_subid FROM opers o,conds c WHERE op_id=oper_id AND
         o.cond_id=c.cond_id AND o.op_subid=c.cond_subid AND
         c.ord>start_sub ;
username VARCHAR2(30) ;
BEGIN
  FOR O IN ops LOOP
    DeleteOper(oper_id,O.op_subid,1,result,username) ;
  END LOOP ;
END ;


PROCEDURE DeleteOper(oper_id NUMBER,oper_subid NUMBER,allow_del_next NUMBER,result OUT NUMBER,USERNAME OUT VARCHAR2) IS
oper opers%ROWTYPE ;
cond conds%ROWTYPE ;
CURSOR items IS
  SELECT * FROM op_items
  WHERE op_id=oper_id AND op_subid=oper_subid AND node=0 ;
item_sum NUMBER ;
item_sum_id NUMBER ;
rights privtest%ROWTYPE ;
res NUMBER ;
all_r NUMBER ;
num_next NUMBER ;
num_prev NUMBER ;
can_modify NUMBER ;
doper opers%ROWTYPE ;
min_date DATE ;
BEGIN
 SELECT * INTO oper FROM opers WHERE op_id=oper_id AND op_subid=oper_subid ;

 -- Проверка прав
 res:=0 ;

 IF oper.edited IS NOT NULL THEN
    BEGIN
      SELECT USERNAME INTO USERNAME FROM V$SESSION WHERE AUDSID=oper.edited ;
      res:=8 ;
    EXCEPTION
      WHEN NO_DATA_FOUND THEN
         SELECT * INTO doper FROM opers WHERE edited=oper.edited AND status=3 ;
         UPDATE op_items SET item_sum_id=NULL WHERE op_id=doper.op_id AND op_subid!=doper.op_subid ;
         DELETE op_items WHERE op_id=doper.op_id ;
         DELETE opers WHERE op_id=doper.op_id ;
    END ;

 ELSE
   IF oper.cond_id IS NOT NULL THEN
      BEGIN
        SELECT * INTO rights FROM privtest WHERE poid=oper.cond_id AND uname=USER ;
        IF rights.pr3=0 THEN res:=4 ; END IF ;
        can_modify:=rights.pr1 ;
        all_r:=0 ;
      EXCEPTION
        WHEN NO_DATA_FOUND THEN res:=4 ;
      END ;
   ELSE
      all_r:=HaveRole('INPUTOPER') ;
      IF all_r=0 THEN res:=4 ; END IF ;
      can_modify:=all_r ;
   END IF ;

   IF oper.post=0 AND can_modify=0 THEN res:=4 ; END IF ;
 END IF ;

 IF HaveRole_('DEADDATE')=0 THEN
    SELECT dead_date INTO min_date FROM user_options WHERE username=USER ;
    IF min_date IS NOT NULL AND oper.op_date<min_date THEN res:=5 ; END IF ;
 END IF ;

 IF res=0 THEN
    -- Проверка на следующие операции
    IF (oper.cond_id IS NOT NULL) AND (allow_del_next=0) THEN
      SELECT * INTO cond FROM conds
        WHERE cond_id=oper.cond_id AND cond_subid=oper_subid ;

      SELECT COUNT(*) INTO num_next FROM opers o,conds c
         WHERE op_id=oper_id AND
               o.cond_id=c.cond_id AND o.op_subid=c.cond_subid AND
               c.ord>cond.ord ;

      SELECT COUNT(*) INTO num_prev FROM opers o,conds c
         WHERE op_id=oper_id AND
               o.cond_id=c.cond_id AND o.op_subid=c.cond_subid AND
               c.ord<cond.ord ;

      IF num_next>0 THEN
         IF num_prev>0 THEN
           res:=12 ;
         ELSE
           res:=14 ;
         END IF ;
      END IF ;
    END IF ;
 END IF ;

 IF res=0 THEN
    PrepareToApply(oper_id,oper_subid,oper.op_date,1) ;
    UPDATE opers SET status=2
    WHERE op_id=oper_id AND op_subid=oper_subid ;
    COMMIT ;
    ApplyUndoOper(oper_id,oper_subid) ;

    CheckPositive(oper_id,oper_subid,res,1) ;
    IF res!=0 AND res!=11 THEN
       PrepareToApply(oper_id,oper_subid,oper.op_date,0) ;
       UPDATE opers SET status=1
       WHERE op_id=oper_id AND op_subid=oper_subid ;
       COMMIT ;
       ApplyUndoOper(oper_id,oper_subid) ;
    END IF ;

    IF res=0 THEN
       -- History and Grave
       BEGIN
          -- на всякий случай очищаем место в списке удаленных операций
          DELETE del_op_items WHERE op_id=oper_id AND op_subid=oper_subid ;
          DELETE del_opers WHERE op_id=oper_id AND op_subid=oper_subid ;

          INSERT INTO del_opers (op_id,op_subid,status,post,storno,text,doc_type,
                                 doc_num,doc_num_v,op_date,cond_id,edited)
                         SELECT  op_id,op_subid,1,post,storno,text,doc_type,
                                 doc_num,doc_num_v,op_date,cond_id,edited
                         FROM opers WHERE op_id=oper_id AND op_subid=oper_subid ;

          FOR item IN items LOOP
          /*
            SELECT value INTO item_sum FROM sums WHERE sum_id=item.item_sum_id ;
            CreateSum(item_sum_id,item_sum) ;
            INSERT INTO del_op_items VALUES (item.op_id,item.op_subid,item.op_date,item.code,item.side,item_sum_id,0,item.for_post) ;
          */
          /* Для хранения аналитики в удаленных операциях */
            INSERT INTO del_op_items VALUES (item.op_id,item.op_subid,item.op_date,item.code,item.side,item.item_sum_id,0,item.for_post) ;
            UPDATE op_items SET item_sum_id=NULL where op_id=oper_id and op_subid=oper_subid ;

          END LOOP ;

          DELETE history
            WHERE op_id=oper_id AND op_subid=oper_subid AND revision=32766 ;
          INSERT INTO history VALUES (oper_id,oper_subid,32766,SYSDATE,USER) ;

       EXCEPTION
          WHEN NO_DATA_FOUND THEN res:=res ;
       END ;

       DELETE FROM op_items WHERE op_id=oper.op_id AND op_subid=oper.op_subid ;
       DELETE FROM cor_items WHERE op_id=oper.op_id AND op_subid=oper.op_subid ;
       DELETE FROM opers WHERE op_id=oper.op_id AND op_subid=oper.op_subid ;

       DEL_LINK_INFO(oper.op_id,oper.op_subid) ;

       COMMIT ;

    END IF ;
 END IF ;
 result:=res ;
END ;

PROCEDURE CopyOper(old_id NUMBER,old_subid NUMBER,
                   new_id NUMBER,new_subid NUMBER,new_status NUMBER) IS
CURSOR items IS
  SELECT * FROM op_items
  WHERE op_id=old_id AND node=0 ;
item_sum NUMBER ;
new_sum_id NUMBER ;
old_sum_id NUMBER ;

anal_id    NUMBER ;
anal_name  VARCHAR2(30) ;
func_str   VARCHAR2(100) ;
cid        NUMBER ;
sql_res    NUMBER ;
sign       NUMBER ;

BEGIN
  INSERT INTO opers (op_id  ,op_subid  ,status,post,storno,text     ,doc_type,doc_num,doc_num_v,op_date,cond_id,edited)
     SELECT new_id,op_subid,new_status,post,storno,text,doc_type,doc_num,doc_num_v,op_date,cond_id,edited
     FROM opers WHERE op_id=old_id ;

  FOR item IN items LOOP
    IF item.op_subid=old_subid THEN
       SELECT value INTO item_sum FROM sums WHERE sum_id=item.item_sum_id ;
       CreateSum(new_sum_id,item_sum) ;
    ELSE
       new_sum_id:=item.item_sum_id ;
    END IF ;

    INSERT INTO op_items VALUES (new_id,item.op_subid,item.op_date,item.code,item.side,new_sum_id,0,item.for_post,item.setname) ;

    /* Analytic */
    IF item.op_subid=old_subid THEN
       SELECT anal_id INTO anal_id FROM accs WHERE code=item.code ;
       CopyAnalSum(anal_id,new_sum_id,item.item_sum_id) ;
    END IF ;

  END LOOP ;
END ;

PROCEDURE AddAllParentItems IS
CURSOR opers_cursor IS
  SELECT * FROM opers ;
CURSOR items_cursor(oper_id NUMBER,oper_subid NUMBER) IS
  SELECT * FROM op_items
  WHERE op_id=oper_id AND op_subid=oper_subid AND node=0 ;
BEGIN
 FOR oper IN opers_cursor LOOP
  FOR item IN items_cursor(oper.op_id,oper.op_subid) LOOP
     AddParentItem(item) ;
  END LOOP ;
 END LOOP ;
END ;

PROCEDURE AddAllCorItems IS
CURSOR opers_cursor IS
  SELECT * FROM opers ;
BEGIN
 FOR oper IN opers_cursor LOOP
  AddCorItems(oper) ;
 END LOOP ;
END ;

PROCEDURE ChangeBegVal(rem_code NUMBER,new_sum_id NUMBER) IS
diff_id  NUMBER ;
diff_sum_id NUMBER ;
rem_sum NUMBER ;
new_sum NUMBER ;
old_rem rems%ROWTYPE ;
item op_items%ROWTYPE ;
anal_id NUMBER ;
acc accs%ROWTYPE ;
BEGIN
  SELECT * INTO acc FROM accs where code=rem_code ;
  SELECT OperSeq.NEXTVAL INTO diff_id FROM DUAL ;
  SELECT * INTO old_rem FROM rems WHERE code=rem_code AND rem_date=
           (SELECT MIN(point_date) FROM cps) ;
  INSERT INTO opers (op_id  ,op_subid,status,post,op_date           ,text     ,storno) VALUES
                    (diff_id,0       ,2     ,0   ,old_rem.rem_date-1,'Diff'   ,0) ;
  SELECT value INTO rem_sum FROM sums WHERE sum_id=old_rem.end_val_id ;
  SELECT value INTO new_sum FROM sums WHERE sum_id=new_sum_id ;
  CreateSum(diff_sum_id,new_sum-rem_sum) ;

  SELECT anal_id INTO anal_id FROM accs WHERE code=rem_code ;
  IF anal_id!=0 THEN
     CopyAnalSum(anal_id,diff_sum_id,new_sum_id) ;
     AddAnalSumSet(anal_id,diff_sum_id,old_rem.end_val_id,-1,acc.setname) ;
  END IF ;

  INSERT INTO op_items VALUES (diff_id,0,old_rem.rem_date-1,rem_code,'D',diff_sum_id,0,0,acc.SETNAME) ;
  SELECT * INTO item FROM op_items WHERE op_id=diff_id AND op_subid=0 AND code=rem_code ;
  AddParentItem(item) ;
  PrepareToApply(diff_id,0,old_rem.rem_date-1,0) ;
  COMMIT ;

  ApplyUndoOper(diff_id,0) ;

  DELETE FROM op_items WHERE op_id=diff_id AND op_subid=0 ;
  DELETE FROM cor_items WHERE op_id=diff_id AND op_subid=0 ;
  DELETE FROM opers WHERE op_id=diff_id AND op_subid=0 ;
  COMMIT ;

END ;

PROCEDURE RecalcAccount(acc_name VARCHAR2,begin_date DATE,ending_date DATE) IS
acc_code NUMBER ;
BEGIN
  BEGIN
     SELECT code INTO acc_code FROM accs WHERE name=acc_name ;
  EXCEPTION
    WHEN NO_DATA_FOUND THEN RETURN ;
  END ;
  RecalcAccount(acc_code,begin_date,ending_date) ;
END ;

PROCEDURE RecalcAccount(acc_code NUMBER,begin_date DATE,ending_date DATE) IS
BEGIN
  RecalcAccount_(acc_code,begin_date,ending_date) ;
END ;

PROCEDURE RecalcOneAccount(acc_name VARCHAR2,begin_date DATE,ending_date DATE) IS
acc_code NUMBER ;
ret NUMBER ;
beg_id  NUMBER ;
beg_sum NUMBER ;
account accs%ROWTYPE ;
pre_turn turns%ROWTYPE ;
pre_dturn NUMBER ;
pre_cturn NUMBER ;
valid NUMBER ;
session_id NUMBER ;

CURSOR recalc_cps(rec_code NUMBER) IS
  SELECT * FROM rems
  WHERE rem_date>begin_date AND rem_date<=ending_date AND code=rec_code ;

CURSOR cints_crsr(rec_code NUMBER,e_date DATE) IS
  SELECT * FROM turns
  WHERE end_date=e_date AND code=rec_code ;

BEGIN
  BEGIN
     SELECT code INTO acc_code FROM accs WHERE name=acc_name ;
  EXCEPTION
    WHEN NO_DATA_FOUND THEN RETURN ;
  END ;

 SELECT count(*) INTO valid FROM cints
   WHERE beg_date=begin_date AND end_date=ending_date ;
 IF valid=0 THEN
    RETURN ;
 END IF ;

 session_id:=USERENV('SESSIONID') ;

 INSERT INTO unapp_opers
   (SELECT DISTINCT op_id,op_subid,0,session_id
    FROM op_items
    WHERE op_date>=begin_date AND op_date<ending_date AND code=acc_code
   ) ;

 COMMIT ;


 BEGIN
    SELECT * INTO account FROM accs WHERE code=acc_code ;
    SELECT end_val_id INTO beg_id FROM rems
       WHERE rem_date=begin_date AND code=acc_code ;
    SELECT value INTO beg_sum FROM sums
       WHERE sum_id=beg_id ;

    FOR cp IN recalc_cps(acc_code) LOOP

       commit ;
       set transaction use rollback segment myroll ;

       -- Записывается остаток на начало во все остатки
       UPDATE sums SET value=beg_sum WHERE sum_id=cp.end_val_id ;
       CopyAnalSum(account.anal_id,cp.end_val_id,beg_id) ;

       -- Инициализация интервалов
       FOR ci IN cints_crsr(acc_code,cp.rem_date) LOOP
         IF ci.beg_date>=begin_date THEN
           -- Обнуление интервала
           UPDATE sums SET value=0
              WHERE sum_id=ci.d_turn_id OR sum_id=ci.c_turn_id ;
           DeleteAnalSum(account.anal_id,ci.d_turn_id) ;
           DeleteAnalSum(account.anal_id,ci.c_turn_id) ;

         ELSE
           -- Копирование предыдущего интервала
           SELECT * INTO pre_turn FROM turns
             WHERE beg_date=ci.beg_date AND end_date=begin_date AND code=acc_code ;
           SELECT value INTO pre_dturn FROM sums WHERE sum_id=pre_turn.d_turn_id ;
           SELECT value INTO pre_cturn FROM sums WHERE sum_id=pre_turn.c_turn_id ;
           UPDATE sums SET value=pre_dturn WHERE sum_id=ci.d_turn_id ;
           UPDATE sums SET value=pre_cturn WHERE sum_id=ci.c_turn_id ;
           CopyAnalSum(account.anal_id,ci.d_turn_id,pre_turn.d_turn_id) ;
           CopyAnalSum(account.anal_id,ci.c_turn_id,pre_turn.c_turn_id) ;

         END IF ;
       END LOOP ;

       INSERT INTO unapp_items (OP_ID,OP_SUBID,CODE,CP_DATE)
         SELECT op_id,op_subid,acc_code,cp.rem_date
         FROM op_items
         WHERE code=acc_code AND op_date>=begin_date AND op_date<cp.rem_date ;

       COMMIT ;

    END LOOP ;
 END ;

 ret:=ApplyUndoAll(session_id,50) ;

END ;

PROCEDURE RecalcOneAccountCor(acc_name VARCHAR2,begin_date DATE,ending_date DATE) IS
acc_code NUMBER ;
ret NUMBER ;
beg_id  NUMBER ;
beg_sum NUMBER ;
account accs%ROWTYPE ;
pre_turn turns%ROWTYPE ;
pre_dturn NUMBER ;
pre_cturn NUMBER ;
valid NUMBER ;
session_id NUMBER ;

CURSOR recalc_cps(rec_code NUMBER) IS
  SELECT * FROM rems
  WHERE rem_date>begin_date AND rem_date<=ending_date AND code=rec_code ;

CURSOR cints_crsr(rec_code NUMBER,e_date DATE) IS
  SELECT * FROM turns
  WHERE end_date=e_date AND code=rec_code ;

CURSOR cor_dturns_crsr(code_ NUMBER,beg_date_ DATE,end_date_ DATE) IS
   SELECT * FROM cor_turns
      WHERE beg_date=beg_date_ AND end_date=end_date_ AND d_code=code_ ;

CURSOR cor_cturns_crsr(code_ NUMBER,beg_date_ DATE,end_date_ DATE) IS
   SELECT * FROM cor_turns
      WHERE beg_date=beg_date_ AND end_date=end_date_ AND c_code=code_ ;

CURSOR opers_cursor is
   select * from opers where (op_id,op_subid) in
   (
    SELECT DISTINCT o.op_id,o.op_subid
    FROM cor_items i,opers o
    WHERE o.op_id=i.op_id AND o.op_subid=i.op_subid AND
          op_date>=begin_date AND op_date<ending_date AND
          d_code=acc_code OR c_code=acc_code
   ) ;

BEGIN
  BEGIN
     SELECT code INTO acc_code FROM accs WHERE name=acc_name ;
  EXCEPTION
    WHEN NO_DATA_FOUND THEN RETURN ;
  END ;

 SELECT count(*) INTO valid FROM cints
   WHERE beg_date=begin_date AND end_date=ending_date ;
 IF valid=0 THEN
    RETURN ;
 END IF ;

 session_id:=USERENV('SESSIONID') ;

 delete cor_items where (op_id,op_subid) in
   (
    SELECT DISTINCT o.op_id,o.op_subid
    FROM cor_items i,opers o
    WHERE o.op_id=i.op_id AND o.op_subid=i.op_subid AND
          op_date>=begin_date AND op_date<ending_date AND
          d_code=acc_code OR c_code=acc_code
   ) ;

 FOR oper IN opers_cursor LOOP
  AddCorItems(oper) ;
 END LOOP ;


 INSERT INTO unapp_opers
   (
    SELECT DISTINCT o.op_id,o.op_subid,0,session_id
    FROM cor_items i,opers o
    WHERE o.op_id=i.op_id AND o.op_subid=i.op_subid AND
          op_date>=begin_date AND op_date<ending_date AND
          d_code=acc_code OR c_code=acc_code
   ) ;

 COMMIT ;

 BEGIN
    SELECT * INTO account FROM accs WHERE code=acc_code ;
    SELECT end_val_id INTO beg_id FROM rems
       WHERE rem_date=begin_date AND code=acc_code ;
    SELECT value INTO beg_sum FROM sums
       WHERE sum_id=beg_id ;

    FOR cp IN recalc_cps(acc_code) LOOP

       commit ;
       set transaction use rollback segment myroll ;

       -- Инициализация интервалов
       FOR ci IN cints_crsr(acc_code,cp.rem_date) LOOP
         IF ci.beg_date>=begin_date THEN

           -- Корр. обороты
           UPDATE sums SET value=0
              WHERE sum_id in
                 (SELECT turn_id FROM cor_turns WHERE (d_code=acc_code
                     OR c_code=acc_code) AND beg_date=ci.beg_date AND
                     end_date=cp.rem_date) ;

         ELSE
           -- Корр. обороты

           -- Дебет
           FOR pre_cor_turn IN cor_dturns_crsr(acc_code,ci.beg_date,begin_date) LOOP
             SELECT value INTO pre_dturn FROM sums WHERE sum_id=pre_cor_turn.turn_id ;
             UPDATE sums SET value=pre_dturn WHERE sum_id=
                 (SELECT turn_id FROM cor_turns WHERE beg_date=ci.beg_date
                    AND end_date=ci.end_date AND d_code=pre_cor_turn.d_code
                                             AND c_code=pre_cor_turn.c_code) ;
           END LOOP ;

           -- Кредит
           FOR pre_cor_turn IN cor_cturns_crsr(acc_code,ci.beg_date,begin_date) LOOP
             SELECT value INTO pre_cturn FROM sums WHERE sum_id=pre_cor_turn.turn_id ;
             UPDATE sums SET value=pre_cturn WHERE sum_id=
                 (SELECT turn_id FROM cor_turns WHERE beg_date=ci.beg_date
                    AND end_date=ci.end_date AND d_code=pre_cor_turn.d_code
                                             AND c_code=pre_cor_turn.c_code) ;
           END LOOP ;

         END IF ;
       END LOOP ;

       -- Корр. обороты

       -- Дебет
       INSERT INTO unapp_cor_items (OP_ID,OP_SUBID,D_CODE,C_CODE,CP_DATE)
         SELECT i.op_id,i.op_subid,d_code,c_code,cp.rem_date
         FROM cor_items i,opers o
         WHERE i.op_id=o.op_id AND i.op_subid=o.op_subid AND post=0 AND
               d_code=acc_code AND
               op_date>=begin_date AND op_date<cp.rem_date ;

       -- Кредит
       INSERT INTO unapp_cor_items (OP_ID,OP_SUBID,D_CODE,C_CODE,CP_DATE)
         SELECT i.op_id,i.op_subid,d_code,c_code,cp.rem_date
         FROM cor_items i,opers o
         WHERE i.op_id=o.op_id AND i.op_subid=o.op_subid AND post=0 AND
               c_code=acc_code AND
               op_date>=begin_date AND op_date<cp.rem_date AND
               d_code NOT IN (SELECT code FROM accs START WITH code=acc_code
                              CONNECT BY parent=PRIOR code) ;

       COMMIT ;

    END LOOP ;
 END ;

 ret:=ApplyUndoAll(session_id,50) ;

END ;

PROCEDURE RecalcAccount_(acc_code NUMBER,begin_date DATE,ending_date DATE) IS
ret NUMBER ;
beg_id  NUMBER ;
beg_sum NUMBER ;
account accs%ROWTYPE ;
pre_turn turns%ROWTYPE ;
pre_dturn NUMBER ;
pre_cturn NUMBER ;
valid NUMBER ;
session_id NUMBER ;

CURSOR rec_accs IS
  SELECT code FROM accs
  START WITH code=acc_code CONNECT BY parent=PRIOR code ;

CURSOR recalc_cps(rec_code NUMBER) IS
  SELECT * FROM rems
  WHERE rem_date>begin_date AND rem_date<=ending_date AND code=rec_code ;

CURSOR cints_crsr(rec_code NUMBER,e_date DATE) IS
  SELECT * FROM turns
  WHERE end_date=e_date AND code=rec_code ;

CURSOR cor_dturns_crsr(code_ NUMBER,beg_date_ DATE,end_date_ DATE) IS
   SELECT * FROM cor_turns
      WHERE beg_date=beg_date_ AND end_date=end_date_ AND d_code=code_ ;

CURSOR cor_cturns_crsr(code_ NUMBER,beg_date_ DATE,end_date_ DATE) IS
   SELECT * FROM cor_turns
      WHERE beg_date=beg_date_ AND end_date=end_date_ AND c_code=code_ ;

BEGIN

 session_id:=USERENV('SESSIONID') ;

 INSERT INTO unapp_opers
   (SELECT DISTINCT op_id,op_subid,0,session_id
    FROM op_items
    WHERE op_date>=begin_date AND op_date<ending_date AND code IN
      (SELECT code FROM accs
       START WITH code=acc_code CONNECT BY parent=PRIOR code)
   )
   union
   (
    SELECT DISTINCT o.op_id,o.op_subid,0,session_id
    FROM cor_items i,opers o
    WHERE o.op_id=i.op_id AND o.op_subid=i.op_subid AND
          op_date>=begin_date AND op_date<ending_date AND
          ((d_code IN (SELECT code FROM accs
                       START WITH code=acc_code CONNECT BY parent=PRIOR code))
           OR
           (c_code IN (SELECT code FROM accs
                       START WITH code=acc_code CONNECT BY parent=PRIOR code))
          )
   ) ;

 COMMIT ;

 SELECT count(*) INTO valid FROM cints
   WHERE beg_date=begin_date AND end_date=ending_date ;
 IF valid=0 THEN
    RETURN ;
 END IF ;

 FOR recalc IN rec_accs LOOP
    SELECT * INTO account FROM accs WHERE code=recalc.code ;
    SELECT end_val_id INTO beg_id FROM rems
       WHERE rem_date=begin_date AND code=recalc.code ;
    SELECT value INTO beg_sum FROM sums
       WHERE sum_id=beg_id ;

    FOR cp IN recalc_cps(recalc.code) LOOP

       commit ;
       set transaction use rollback segment myroll ;

       -- Записывается остаток на начало во все остатки
       UPDATE sums SET value=beg_sum WHERE sum_id=cp.end_val_id ;
       CopyAnalSum(account.anal_id,cp.end_val_id,beg_id) ;

       -- Инициализация интервалов
       FOR ci IN cints_crsr(recalc.code,cp.rem_date) LOOP
         IF ci.beg_date>=begin_date THEN
           -- Обнуление интервала
           UPDATE sums SET value=0
              WHERE sum_id=ci.d_turn_id OR sum_id=ci.c_turn_id ;
           DeleteAnalSum(account.anal_id,ci.d_turn_id) ;
           DeleteAnalSum(account.anal_id,ci.c_turn_id) ;

           -- Корр. обороты
           UPDATE sums SET value=0
              WHERE sum_id in
                 (SELECT turn_id FROM cor_turns WHERE (d_code=recalc.code
                     OR c_code=recalc.code) AND beg_date=ci.beg_date AND
                     end_date=cp.rem_date) ;

         ELSE
           -- Копирование предыдущего интервала
           SELECT * INTO pre_turn FROM turns
             WHERE beg_date=ci.beg_date AND end_date=begin_date AND code=recalc.code ;
           SELECT value INTO pre_dturn FROM sums WHERE sum_id=pre_turn.d_turn_id ;
           SELECT value INTO pre_cturn FROM sums WHERE sum_id=pre_turn.c_turn_id ;
           UPDATE sums SET value=pre_dturn WHERE sum_id=ci.d_turn_id ;
           UPDATE sums SET value=pre_cturn WHERE sum_id=ci.c_turn_id ;
           CopyAnalSum(account.anal_id,ci.d_turn_id,pre_turn.d_turn_id) ;
           CopyAnalSum(account.anal_id,ci.c_turn_id,pre_turn.c_turn_id) ;

           -- Корр. обороты

           -- Дебет
           FOR pre_cor_turn IN cor_dturns_crsr(recalc.code,ci.beg_date,begin_date) LOOP
             SELECT value INTO pre_dturn FROM sums WHERE sum_id=pre_cor_turn.turn_id ;
             UPDATE sums SET value=pre_dturn WHERE sum_id=
                 (SELECT turn_id FROM cor_turns WHERE beg_date=ci.beg_date
                    AND end_date=ci.end_date AND d_code=pre_cor_turn.d_code
                                             AND c_code=pre_cor_turn.c_code) ;
           END LOOP ;

           -- Кредит
           FOR pre_cor_turn IN cor_cturns_crsr(recalc.code,ci.beg_date,begin_date) LOOP
             SELECT value INTO pre_cturn FROM sums WHERE sum_id=pre_cor_turn.turn_id ;
             UPDATE sums SET value=pre_cturn WHERE sum_id=
                 (SELECT turn_id FROM cor_turns WHERE beg_date=ci.beg_date
                    AND end_date=ci.end_date AND d_code=pre_cor_turn.d_code
                                             AND c_code=pre_cor_turn.c_code) ;
           END LOOP ;

         END IF ;
       END LOOP ;

       INSERT INTO unapp_items (OP_ID,OP_SUBID,CODE,CP_DATE)
         SELECT op_id,op_subid,recalc.code,cp.rem_date
         FROM op_items
         WHERE code=recalc.code AND op_date>=begin_date AND op_date<cp.rem_date ;

       -- Корр. обороты

       -- Дебет
       INSERT INTO unapp_cor_items (OP_ID,OP_SUBID,D_CODE,C_CODE,CP_DATE)
         SELECT i.op_id,i.op_subid,d_code,c_code,cp.rem_date
         FROM cor_items i,opers o
         WHERE i.op_id=o.op_id AND i.op_subid=o.op_subid AND post=0 AND
               d_code=recalc.code AND
               op_date>=begin_date AND op_date<cp.rem_date ;

       -- Кредит
       INSERT INTO unapp_cor_items (OP_ID,OP_SUBID,D_CODE,C_CODE,CP_DATE)
         SELECT i.op_id,i.op_subid,d_code,c_code,cp.rem_date
         FROM cor_items i,opers o
         WHERE i.op_id=o.op_id AND i.op_subid=o.op_subid AND post=0 AND
               c_code=recalc.code AND
               op_date>=begin_date AND op_date<cp.rem_date AND
               d_code NOT IN (SELECT code FROM accs START WITH code=acc_code
                              CONNECT BY parent=PRIOR code) ;

       COMMIT ;

    END LOOP ;
 END LOOP ;

 ret:=ApplyUndoAll(session_id,50) ;

END ;


PROCEDURE RecalcBegin(acc_name VARCHAR2) IS
acc_code NUMBER ;
BEGIN
  BEGIN
     SELECT code INTO acc_code FROM accs WHERE name=acc_name ;
  EXCEPTION
    WHEN NO_DATA_FOUND THEN RETURN ;
  END ;
  RecalcBegin(acc_code) ;
END ;


PROCEDURE RecalcBegin(acc_code NUMBER) IS
beg_id  NUMBER ;
acc_sum NUMBER ;
acc_id NUMBER ;
account accs%ROWTYPE ;
pre_turn turns%ROWTYPE ;
pre_dturn NUMBER ;
pre_cturn NUMBER ;
begin_date DATE ;
CURSOR rec_accs IS
  SELECT code FROM accs
  WHERE node=0
  START WITH code=acc_code CONNECT BY parent=PRIOR code ;
BEGIN
    commit ;

    set transaction use rollback segment myroll ;

    SELECT MIN(point_date) INTO begin_date FROM cps ;

    SELECT * INTO account FROM accs WHERE code=acc_code ;

    IF account.node=0 THEN
       RETURN ;
    END IF ;

    SELECT end_val_id INTO beg_id FROM rems
       WHERE rem_date=begin_date AND code=acc_code ;

    -- Обнуление синт. суммы
    UPDATE sums SET value=0 WHERE sum_id=beg_id ;

    -- Обнуление анал. суммы
    IF account.anal_id IS NOT NULL THEN
      DeleteAnalSum(account.anal_id,beg_id) ;
    END IF ;

    FOR acc IN rec_accs LOOP
      IF acc.code!=acc_code THEN
         SELECT end_val_id INTO acc_id FROM rems
            WHERE rem_date=begin_date AND code=acc.code ;
         SELECT value INTO acc_sum FROM sums WHERE sum_id=acc_id ;

         UPDATE sums SET value=value+acc_sum WHERE sum_id=beg_id ;

         IF account.anal_id IS NOT NULL THEN
            AddAnalSum(account.anal_id,beg_id,acc_id,1) ;
         END IF ;
      END IF ;
    END LOOP ;

    COMMIT ;
END ;

END ;
/
show errors

CREATE OR REPLACE
PROCEDURE DBRevision(dbid_ NUMBER,article_ VARCHAR2,ver_ NUMBER,
                     revision IN OUT NUMBER) AS
max_rev NUMBER ;
temp_rev NUMBER ;
BEGIN
  SELECT MAX(revision)+1 INTO max_rev FROM dbhistory
   WHERE db_id=dbid_ AND article=article_ AND ver=ver_ ;
  IF max_rev IS NULL THEN max_rev:=0 ; END IF ;

  if revision is null then
   revision:=max_rev ;
  else
   select count(*) into temp_rev FROM dbhistory
     where db_id=dbid_ and article=article_ and ver=ver_ ;
   if temp_rev>0 then
     revision:=max_rev ;
   end if ;
  end if ;

  INSERT INTO dbhistory VALUES (dbid_,article_,ver_,revision,SYSDATE,USER) ;
END ;
/
show errors

-- Возвращает сумму по № счета в проводке OP_ID, OP_SUB_ID
create or replace
        function CSM (pNum NUMBER, pOP_ID number, pOP_SUBID number)
 return NUMBER as
 K number :=0;
 Res NUMBER;
 ACC_NAME varchar2(20);
begin
 begin
   -- Находим код счета
   select CI.CODE into K from OPERS O, COND_ITEMS CI
   where O.OP_ID= pOP_ID
            and O.OP_SUBID = pOP_SUBID
            and CI.COND_ID= O.COND_ID
            and CI.COND_SUBID=O.OP_SUBID
            and CI.ITEM_NUM= pNum;

   -- Находим собственно сумму
   select VALUE into RES from OP_ITEMS O, SUMS S
   where OP_ID=pOP_ID
            and OP_SUBID = pOP_SUBID
            and code= K and S.SUM_ID=O.ITEM_SUM_ID;

   return RES;
 exception
   when NO_DATA_FOUND then
    if K=0 then
      RETURN 0;
    else
     -- Если сумма не нашлась то пытаемся найти сумму по субсчету
     select NAME into ACC_NAME from ACCS where K = CODE;
     ACC_NAME := ACC_NAME || '/%';
     select VALUE into RES from OP_ITEMS O, SUMS S
     where
         OP_ID=pOP_ID  and OP_SUBID = pOP_SUBID and
         code in (select CODE from ACCS where NAME LIKE ACC_NAME) and
         S.SUM_ID=O.ITEM_SUM_ID;
    end if;
   return RES;
 end;
end;
/

-- Возвращает код аналитической суммы по № счета в проводке OP_ID, OP_SUB_ID
create or replace
        function CSIM (pNum NUMBER, pOP_ID number, pOP_SUBID number)
 return NUMBER as
 K number :=0;
 Res NUMBER;
 ACC_NAME varchar2(20);
begin
 begin
   -- Находим код счета
   select CI.CODE into K from OPERS O, COND_ITEMS CI
   where O.OP_ID= pOP_ID
            and O.OP_SUBID = pOP_SUBID
            and CI.COND_ID= O.COND_ID
            and CI.COND_SUBID=O.OP_SUBID
            and CI.ITEM_NUM= pNum;

   -- Находим собственно сумму
   select ITEM_SUM_ID into RES from OP_ITEMS O
   where
          OP_ID=pOP_ID and
          OP_SUBID = pOP_SUBID and
          code= K;

   return RES;
 exception
   when NO_DATA_FOUND then
    if K=0 then
      RETURN 0;
    else
     -- Если сумма не нашлась то пытаемся найти сумму по субсчету
     select NAME into ACC_NAME from ACCS where K = CODE;
     ACC_NAME := ACC_NAME || '/%';
     select ITEM_SUM_ID into RES from OP_ITEMS O
     where
          OP_ID=pOP_ID and
          OP_SUBID = pOP_SUBID and
          code in (select CODE from ACCS where NAME LIKE ACC_NAME);
    end if;
   return RES;
 end;
end;
/

CREATE OR REPLACE
 FUNCTION GetObjName (pObjId NUMBER) return OBJS.NAME%TYPE as
    vName OBJS.NAME%TYPE;
BEGIN
 select Name into vName from OBJS
 where OID = pObjId;
 return vName;
exception WHEN NO_DATA_FOUND then
 return '';
END ;
/
show errors;

CREATE OR REPLACE FUNCTION GetOperSum(OPER_ID NUMBER,OPER_SUBID NUMBER) RETURN NUMBER AS
 ret NUMBER ;
BEGIN
  SELECT SUM(value) INTO ret FROM op_items i, sums, accs a
    WHERE sum_id=item_sum_id AND op_id=oper_id AND op_subid=oper_subid AND
          SIDE='D' AND bal=1 AND a.node=0 AND i.code=a.code ;

  IF ret=0 OR ret IS NULL THEN
     SELECT SUM(value) INTO ret FROM op_items i, sums, accs a
       WHERE sum_id=item_sum_id AND op_id=oper_id AND op_subid=oper_subid AND
          SIDE='D' AND bal=0 AND a.node=0 AND i.code=a.code ;
  END IF ;
  IF ret=0 OR ret IS NULL THEN
     SELECT SUM(value) INTO ret FROM op_items i, sums, accs a
       WHERE sum_id=item_sum_id AND op_id=oper_id AND op_subid=oper_subid AND
          SIDE='K' AND bal=0 AND a.node=0 AND i.code=a.code ;
  END IF ;
  return ret ;
END ;
/

CREATE OR REPLACE FUNCTION GetDelOperSum(OPER_ID NUMBER,OPER_SUBID NUMBER) RETURN NUMBER AS
 ret NUMBER ;
BEGIN
  SELECT SUM(value) INTO ret FROM del_op_items i, sums, accs a
    WHERE sum_id=item_sum_id AND op_id=oper_id AND op_subid=oper_subid AND
          SIDE='D' AND bal=1 AND a.node=0 AND i.code=a.code ;

  IF ret=0 OR ret IS NULL THEN
     SELECT SUM(value) INTO ret FROM del_op_items i, sums, accs a
       WHERE sum_id=item_sum_id AND op_id=oper_id AND op_subid=oper_subid AND
          SIDE='D' AND bal=0 AND a.node=0 AND i.code=a.code ;
  END IF ;
  IF ret=0 OR ret IS NULL THEN
     SELECT SUM(value) INTO ret FROM del_op_items i, sums, accs a
       WHERE sum_id=item_sum_id AND op_id=oper_id AND op_subid=oper_subid AND
          SIDE='K' AND bal=0 AND a.node=0 AND i.code=a.code ;
  END IF ;
  return ret ;
END ;
/


CREATE OR REPLACE FUNCTION USER_ACC(acc VARCHAR2) RETURN VARCHAR2 AS
 nacc VARCHAR2(30) ;
 new_plan NUMBER ;
 p1 VARCHAR2(30) ;
 p2 VARCHAR2(30) ;
BEGIN
 new_plan:=0 ;
 IF SUBSTR(acc,1,1)='B' THEN new_plan:=1 ; END IF ;

 nacc:=LTRIM(acc,'ABCZ/') ;
 nacc:=REPLACE(nacc,'/00','/') ;
-- nacc:=REPLACE(nacc,'/0','/') ;

 IF new_plan=1 THEN
   IF LENGTH(nacc)<6 THEN
    nacc:=REPLACE(nacc,'/','') ;
   ELSE
    p1:=SUBSTR(nacc,1,5) ;
    P2:=SUBSTR(nacc,6) ;
    p1:=REPLACE(p1,'/','') ;
    nacc:=p1 || p2 ;
   END IF ;
 END IF ;

 return nacc ;
END ;
/
show errors ;

CREATE OR REPLACE FUNCTION GetOperAcc(ASIDE VARCHAR2,OPER_ID NUMBER,OPER_SUBID NUMBER) RETURN VARCHAR2 AS
 cursor items is
  select * from op_items where op_id=oper_id AND op_subid=oper_subid and
           node=0 AND side=aside ;
 acc VARCHAR2(30) ;
 daccs VARCHAR2(250) ;
BEGIN
  FOR I IN items LOOP
    SELECT name INTO acc FROM accs WHERE code=I.code ;
    acc:=USER_ACC(acc) ;
    IF daccs IS NULL THEN daccs:=acc ;
    ELSE daccs:=daccs || ',' || acc ;
    END IF ;
  END LOOP ;
  return daccs ;
END ;
/

CREATE OR REPLACE FUNCTION GetDelOperAcc(ASIDE VARCHAR2,OPER_ID NUMBER,OPER_SUBID NUMBER) RETURN VARCHAR2 AS
 cursor items is
  select * from del_op_items where op_id=oper_id AND op_subid=oper_subid and
           node=0 AND side=aside ;
 acc VARCHAR2(30) ;
 daccs VARCHAR2(250) ;
BEGIN
  FOR I IN items LOOP
    SELECT name INTO acc FROM accs WHERE code=I.code ;
    acc:=USER_ACC(acc) ;
   
    IF daccs IS NULL THEN daccs:=acc ;
    ELSE daccs:=daccs || ',' || acc ;

    END IF ;
  END LOOP ;
  return daccs ;
END ;
/

CREATE OR REPLACE FUNCTION GetDCLetter(ASIDE VARCHAR2) RETURN VARCHAR2 AS
 ret VARCHAR2(10) ;
BEGIN
 IF ASIDE='D' THEN ret:='Д' ; ELSE ret:='К'; END IF ;
 return ret ;
END ;
/

CREATE OR REPLACE PROCEDURE
CreateTmpSum
        (   newid OUT NUMBER,
            val IN NUMBER,
            pDescr in varchar2 default NULL) AS
BEGIN
-----------------------------------------------------------------------------------------
---------------------- Создает временную сумму
-----------------------------------------------------------------------------------------
  SELECT SumSeq.NEXTVAL INTO newid FROM DUAL ;
  INSERT INTO sums VALUES (newid,val) ;
  insert into TMP_SUMS
    (SUM_ID, CREATE_DATE, CREATOR,DESCR)
  values
    (newid,   SYSDATE(),   USER(),pDescr);
END ;
/
CREATE OR REPLACE PROCEDURE
    DeleteTmpSum
        (pSumId NUMBER) AS
    vExists number ;
BEGIN
-----------------------------------------------------------------------------------------
---------------------- Удаляет сумму если она является временной,
---------------------- иначе сообщение об ошибке
-----------------------------------------------------------------------------------------
    select max(1) into vExists
    from TMP_SUMS
    where
        SUM_ID = pSumId;

    if vExists=1 then
        delete from TMP_SUMS
        where SUM_ID = pSumId;
        delete from SUMS
        where SUM_ID = pSumId;
    else
        raise_application_error (-20001,'Временная сумма с кодом '||To_Char(pSumId)||' не существует');
    end if;
end;
/

CREATE OR REPLACE PROCEDURE
    DeleteIfTmpSum
        (pSumId NUMBER) AS
    vExists number ;
BEGIN
-----------------------------------------------------------------------------------------
---------------------- Удаляет сумму если она является временной,
---------------------- иначе ничего не делает
-----------------------------------------------------------------------------------------
    select max(1) into vExists
    from TMP_SUMS
    where
        SUM_ID = pSumId;

    if vExists=1 then
        delete from TMP_SUMS
        where SUM_ID = pSumId;
        delete from SUMS
        where SUM_ID = pSumId;
    end if;
end;
/
show errors;


CREATE OR REPLACE FUNCTION R_ASM(pAccCode NUMBER, rdate DATE) RETURN NUMBER AS
 analid NUMBER ;
 acode NUMBER ;
 sdate DATE ;
 val NUMBER ;
 tmp NUMBER ;
 sign NUMBER ;
 CURSOR ops(ocode NUMBER,d1 DATE,d2 DATE) IS
   SELECT i.item_sum_id,i.side,o.storno FROM opers o,op_items i
     WHERE o.op_id=i.op_id AND o.op_subid=i.op_subid AND
           o.op_date>=d1 AND o.op_date<d2 AND
           code=ocode AND post=for_post AND status=1 ;

BEGIN
  acode :=pAccCode;
  SELECT anal_id  INTO analid FROM accs WHERE code=acode ;
  IF analid IS NULL THEN
    RETURN 0 ;
  end if;


  SELECT MAX(point_date) into sdate FROM cps WHERE point_date<=rdate ;

  CreateTmpSum(val,0, 'Функция R_ASM код '||to_char(pAccCode)||' '||to_char(rdate)) ;
  DeleteAnalSum(analid,val) ;

  SELECT end_val_id INTO tmp FROM rems WHERE rem_date=sdate AND code=acode ;
  AddAnalSum(analid,val,tmp,1) ;

  FOR O IN ops(acode,sdate,rdate) LOOP
    IF O.side='D' AND O.storno=0 THEN sign:=1 ; ELSE sign:=-1 ; END IF ;
    AddAnalSum(analid,val,O.item_sum_id,sign) ;
  END LOOP ;
 RETURN val ;
END ;
/
create or replace function
RIM ( pAccCode number, pDate date default NULL)
return number
as
    vTemp number;
    vDate date;
begin
    ------------------- Находим дату конца периода видимости
    vDate := pDate;
    if vDate is NULL then
        select max(END_DATE) into vDate
        from USER_OPTIONS
        where USERNAME = USER;

        if vDate is NULL then
            select max (POINT_DATE) into vDate
            from CPS;
        end if;
    end if;

    ------------------- Определяем дата ровная?
    select max(1) into vTemp
    from CPS
    where POINT_DATE=vDate;


    if vTemp = 1 then  ------------------- Ровная дата
        select max (end_val_id) into vTemp
        from REMS
        where
            CODE     = pAccCode and
            REM_DATE = vDate;
    else
        vTemp:=R_ASM(pAccCode, vDate);
    end if;

    if vTemp is null then
        vTemp := 0;
    end if;

    return vTemp;
end;
/

show errors

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
 -- Преобразование нового плана счетов
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

show errors

CREATE OR REPLACE FUNCTION AnalInfo(oper_id NUMBER, oper_subid NUMBER,acode NUMBER) RETURN VARCHAR2 AS
ret VARCHAR2(100) ;
begin
 return ret ;
end ;
/


CREATE OR REPLACE PROCEDURE PREPARE_EDIT_OPER(OP_ID_ NUMBER, OP_SUBID_ NUMBER,
                                        RES OUT NUMBER,UNAME OUT VARCHAR2) AS
ed NUMBER ;
live NUMBER ;
dead_exist NUMBER ;
dead_op opers%rowtype ;
BEGIN
  SELECT edited INTO ed FROM opers WHERE OP_ID=OP_ID_ AND OP_SUBID=OP_SUBID_ ;
  IF ed=0 THEN
     ed:=USERENV('SESSIONID') ;
     UPDATE opers SET edited=ed WHERE OP_ID=OP_ID_ AND OP_SUBID=OP_SUBID_ ;
     RES:=1 ; -- Ok
  ELSE
     SELECT count(*) INTO live FROM V$SESSION WHERE AUDSID=ed ;
     IF live=1 THEN
       SELECT USERNAME INTO uname FROM V$SESSION WHERE AUDSID=ed ;
       RES:=2 ;
     ELSE -- dead user
       SELECT count(*) INTO dead_exist FROM opers WHERE edited=ed AND status=3 ;
       IF dead_exist=1 THEN
         SELECT * INTO dead_op FROM opers WHERE edited=ed AND status=3 ;
         bookkern.ApplyUndoOper(dead_op.op_id,dead_op.op_subid) ;
         UPDATE op_items SET item_sum_id=NULL WHERE OP_ID=dead_op.OP_ID AND OP_SUBID!=dead_op.OP_SUBID ;
         DELETE cor_items WHERE OP_ID=dead_op.OP_ID AND OP_SUBID=dead_op.OP_SUBID ;
         DELETE op_items WHERE OP_ID=dead_op.OP_ID ;
         DELETE opers WHERE OP_ID=dead_op.OP_ID ;
       END IF ;
       ed:=USERENV('SESSIONID') ;
       UPDATE opers SET edited=ed WHERE OP_ID=OP_ID_ AND OP_SUBID=OP_SUBID_ ;
       RES:=1 ; -- Ok
     END IF ;
  END IF ;
  COMMIT ;
end ;
/


CREATE OR REPLACE PROCEDURE NEW_HIST_RECORD(OP_ID_ NUMBER, OP_SUBID_ NUMBER,
                                            RDATE DATE,UNAME VARCHAR2) AS
max_rev NUMBER ;
BEGIN
    SELECT MAX(revision) INTO max_rev FROM HISTORY
           WHERE op_id=op_id_ AND op_subid=op_subid_ ;
    IF max_rev IS NULL THEN max_rev:=0 ; END IF ;
    INSERT INTO HISTORY VALUES (op_id_,op_subid_,max_rev+1,RDATE,UNAME) ;
END ;
/

CREATE OR REPLACE PROCEDURE
CreateAndCopyAnSum
        (
            pAnalId in number,
            pNewId in out number,
            pOldId in number,
            pDeleteOld in number default 0 ) AS
BEGIN
-----------------------------------------------------------------------------------------
-- Создает временную сумму, копирует и удаляет старую 
-----------------------------------------------------------------------------------------
  if (pNewId is NULL) or (pNewId = 0) then
    SELECT SumSeq.NEXTVAL INTO pNewId FROM DUAL;
    insert into SUMS
        (SUM_ID, VALUE) values (pNewId, 0); 
    commit;
  end if;

  CopyAnalSum(pAnalId, pNewId, pOldId);
  if (pDeleteOld =1) then
    delete from SUMS where SUM_ID = pOldId;
  end if;
END ;
/
show errors

CREATE OR REPLACE FUNCTION 
    RI (pName varchar2, pDate Date) return NUMBER AS
-- ВОЗВРАЩАЕТ код ан суммы остатка на дату (исп в RI, BRI)
 vSumId NUMBER;
begin
 select END_VAL_ID into vSumId  from REMS R, ACCS A 
 where 
  A.NAME = pNAME and A.CODE = R.CODE and REM_DATE = pDate ; 
 return vSumId;
 exception
  when NO_DATA_FOUND then 
    raise_application_error (-20002, 'Date '||pDate||' not is plain period for account '|| pName);  
end;
/
CREATE OR REPLACE FUNCTION 
    RI (pAccCode number, pDate Date default NULL) return NUMBER AS
-- ВОЗВРАЩАЕТ код ан суммы остатка на дату (исп в RI, BRI)
 vSumId NUMBER;
 pDate1 date;
begin
 pDate1:=pDate;   
 if pDate1 is NULL then 
    select max(END_DATE) into pDATE1
    from USER_OPTIONS
    where
        USERNAME=USER;
 end if;
 select END_VAL_ID into vSumId  from REMS R 
 where 
  R.CODE = pAccCode and REM_DATE = pDate1 ; 
 return vSumId;
 exception
  when NO_DATA_FOUND then 
    raise_application_error (-20002, 'Date '||pDate||' not is plain period for account '|| pAccCode);  
end;
/
CREATE OR REPLACE FUNCTION 
   GetInpDate return DATE AS
-- ВОЗВРАЩАЕТ дату для ввода по умолчанию
    vDate date;
    vCurrDates number;
begin
    select INP_DATE, CUR_DATES into vDate, vCurrDates
    from USER_OPTIONS
    where USERNAME=USER;

    if vCurrDates=1 or vDate is NULL then
        select SYSDATE into vDate from DUAL;
    end if;
    return vDate;      
end;
/
CREATE OR REPLACE PROCEDURE
    GetLinkDb(
    pOP_ID number, pOP_SUBID number,
    pDbName   out varchar2 , ---- Имя связанной БД
    pArticle  out varchar2,  ---- Арт+Вер позиции БД
    pVer       out number,   ----     связанной с данной операцией
    pTiedToDoc out number    ---- 1 - если база данных связана с документом
                             ----     0 - связана с операцией
    ) AS
    ---- Ищет связанную базу данных с операцией или документом
    ---- возвращает название связанной БД, артикул и версию с которыми
    ---- ассоциируется данная БД
  doc VARCHAR2(30) ;
  vcond NUMBER ;
  par NUMBER ;
  vOrd number;
BEGIN
  pTiedToDoc:=0;
  pDbName:='';
  SELECT max(parent) INTO par FROM objs WHERE name=(SELECT doc_type FROM opers
                                  WHERE OP_ID=pOP_ID AND OP_SUBID=pOP_SUBID) ;
  IF par IS Not NULL THEN

    pTiedToDoc:=1;
    select NAME into pDbName from OBJS where OID=par;
    select DOC_NUM, DOC_NUM_V into pArticle, pVer
    from OPERS
    where OP_ID=pOP_ID AND OP_SUBID=pOP_SUBID;

  else
    SELECT max(cond_id) into vCOND FROM opers
            WHERE OP_ID=pOP_ID AND OP_SUBID=pOP_SUBID;
    SELECT max(parent) INTO par FROM objs WHERE oid=vCond;
    if par is not NULL then
        select NAME into pDbName from OBJS where OID=par;

        select MIN (ord) into vOrd from CONDS where COND_ID=vCond;

        select DOC_NUM, DOC_NUM_V into pArticle, pVer
        from OPERS
        where OP_ID=pOP_ID AND OP_SUBID=
                (select COND_SUBID from CONDS
                 where COND_ID=vCond and ORD=vORD);
    end if;
  END IF ;
END ;
/

--------------------------------------
-- 23.11.01 Исправление ошибки FOR_POST
--  для OP_ITEMS в GetAccSumId --
--------------------------------------
CREATE OR REPLACE PROCEDURE
    GetAccSumId(
        pAccCode    number,     ---- Код счета
        pOpId       number,     ---- Код операции
        pOpSubId    number,     ---- SubCod
        pAnalSumId  in out number,--- на входе -какую сумму копировать
                                  --- на выходе в какую сумму скопировали
        pCopy   number,         ---- Копировать аналитическую сумму даже 
                                ----  если есть ч-л в аналит сумме
        pItemNum number,         ---- Номер счета в станд опер
        pSetName varchar2 default NULL

        ) AS 
                                                                   
    ---- Копирует Аналитическую сумму по счету в случае A=... 
    ---- если указать pAnalSumId ==0 то не будет пытаться копировать
    vSumId number;
    vSide varchar2(1);
    vPost number(1);
    vOpDate date;
    vNode number(8);
    vAnalId number(8);
    vNew number(1) :=0;

BEGIN
    select max(ITEM_SUM_ID) into vSumId
    from OP_ITEMS
    where OP_ID=pOpId and OP_SUBID=pOpSubId and CODE=pAccCode;

    select ANAL_ID, NODE into vAnalId, vNode
    from ACCS
    where CODE=pAccCode;

    if vNode != 0 then
        raise_application_error(-20012, 'Копирование только для счетов без субсчетов');
    end if;

 

    ---- если еще не было вставлена запись в OP_ITEMS 
    if vSumId is NULL then
        vNew:=1;

        select SIDE, FOR_POST into vSide, vPost
        from COND_ITEMS
        where
            COND_ID=(select COND_ID from OPERS
                     where OP_ID=pOpId and OP_SUBID=pOpSubId) and
            COND_SUBID = pOpSubId and
            ITEM_NUM=pItemNum;
                       
        CreateSum(vSumId,0);

        select OP_DATE into vOpDate
        from OPERS
        where OP_ID=pOpId and OP_SUBID=pOpSubId;

        insert into OP_ITEMS
            (OP_ID, OP_SUBID, OP_DATE, CODE,      SIDE, ITEM_SUM_ID, NODE, FOR_POST, SETNAME)
        values
            (pOpId, pOpSubId, vOpDate, pAccCode,  vSide,   vSumId,     0,   vPost,  pSetName);
    end if;

    if pAnalSumId!=0 then
        if vNew=1 or pCopy=1 or GetKolPos(vSumId, vAnalId)=0 then
            CopyAnalSum(vAnalId, vSumId, pAnalSumId);
        end if;
    end if;
    pAnalSumId:=vSumId;
    return;
END ;
/
CREATE OR REPLACE function
    GetAnSumByAnId(
        pAnalId     number,     ---- Код аналитики
        pOpId       number,     ---- Код операции
        pOpSubId    number)     ---- SubCod
        return number AS
    ---- возвращает код Аналитической суммы  
    cursor S is select OI.ITEM_SUM_ID
                from OP_ITEMS OI, ACCS A
                where
                    OI.OP_ID    = pOpId     and
                    OI.OP_SUBID = pOpSubId  and
                    OI.CODE     = A.CODE    and
                    A.ANAL_ID   = pAnalId
                order by A.NAME;
    vSumId number;
begin
    for X in S loop
        vSumId:= X.ITEM_SUM_ID;
        return vSumId;    
    end loop;
    return 0;
end;
/

create or replace function
    GetParentAcc (pAccCode number) return number as
    ------- Возвращает код родительского счета с учетом прв доступа
    ------- для построения окна просмотра счетов
    cursor S is
                select A.CODE 
                from 
                    (select CODE, NAME from ACCS
                     start with CODE=pAccCode
                     connect by CODE = prior PARENT)  A, PRIVTEST P 
                where 
                    A.CODE !=pAccCode and
                    P.POID=A.CODE and 
                    P.UNAME=USER and
                    P.PR0>0
                order by A.NAME desc;
begin
    for X in S loop
        return X.CODE;
    end loop;
    return NULL;
end;
/
show errors

CREATE OR REPLACE function
    GetExistedAcc( 
        pOpId       number,     ---- Код операции
        pOpSubId    number,     ---- SubCod
        pAccCode    number,     ---- Код счета с субчетами
        pSide       varchar2 )  ---- Сторона  
        return number AS
    ---- возвращает код субсчета если уже было введено ранее
    ---- пользователем 
    vAcc number;
begin
    begin
        select CODE into vAcc
        from OP_ITEMS
        where
            OP_ID   = pOpId     and
            OP_SUBID= pOpSubId  and
            CODE    in ( select CODE from ACCS
                         where CODE != pAccCode 
                         start with CODE = pAccCode
                         connect by prior CODE = PARENT) and
            SIDE    = pSide;
    exception
        when NO_DATA_FOUND then
            vAcc:=0;
    end;
    return vAcc;
end;
/
show errors

CREATE OR REPLACE FUNCTION 
    BRI (pAccCode number, pDate Date default NULL) return NUMBER AS
-- ВОЗВРАЩАЕТ код ан суммы остатка на дату (исп в RI, BRI)
 vSumId NUMBER;
 pDate1 date;
begin
 pDate1:=pDate;   
 if pDate1 is NULL then 
    select max(BEG_DATE) into pDATE1
    from USER_OPTIONS
    where
        USERNAME=USER;
 end if;
 select END_VAL_ID into vSumId  from REMS R 
 where 
  R.CODE = pAccCode and REM_DATE = pDate1 ; 
 return vSumId;
 exception
  when NO_DATA_FOUND then 
    raise_application_error (-20002, 'Date '||pDate||' not is plain period for account '|| pAccCode);  
end;
/
CREATE OR REPLACE FUNCTION
    CTI (pAccCode number,
         pDateBeg Date default NULL,
         pDateEnd Date default NULL) return NUMBER AS
-- ВОЗВРАЩАЕТ код ан суммы остатка на дату (исп в RI, BRI)
 vSumId NUMBER;
 pDate1 date;
 pDate2 date;
begin
 pDate1:=pDateBeg;
 pDate2:=pDateEnd;
 if pDateBeg is NULL then
    select max(BEG_DATE) into pDATE1
    from USER_OPTIONS
    where
        USERNAME=USER;
 end if;
 if pDateEnd is NULL then
    select max(END_DATE) into pDATE2
    from USER_OPTIONS
    where
        USERNAME=USER;
 end if;

 select C_TURN_ID into vSumId  from TURNS R
 where
  R.CODE = pAccCode and BEG_DATE = pDate1 and END_DATE=pDate2 ;
 return vSumId;
 exception
  when NO_DATA_FOUND then
    return 0;
end;

/
CREATE OR REPLACE FUNCTION
    DTI (pAccCode number,
         pDateBeg Date default NULL,
         pDateEnd Date default NULL) return NUMBER AS
-- ВОЗВРАЩАЕТ код ан суммы остатка на дату (исп в RI, BRI)
 vSumId NUMBER;
 pDate1 date;
 pDate2 date;
begin
 pDate1:=pDateBeg;
 pDate2:=pDateEnd;
 if pDateBeg is NULL then
    select max(BEG_DATE) into pDATE1
    from USER_OPTIONS
    where
        USERNAME=USER;
 end if;
 if pDateEnd is NULL then
    select max(END_DATE) into pDATE2
    from USER_OPTIONS
    where
        USERNAME=USER;
 end if;

 select D_TURN_ID into vSumId  from TURNS R
 where
  R.CODE = pAccCode and BEG_DATE = pDate1 and END_DATE=pDate2 ;
 return vSumId;
 exception
  when NO_DATA_FOUND then
    return 0;
end;
/

create or replace function
    GetAccCorValue (pAccCodeTempl number, 
                    pAccCodeCurr  number,
                    pSideTempl    OP_ITEMS.SIDE%TYPE,
                    pSideCurr     OP_ITEMS.SIDE%TYPE,
                    pStorno       number,
                    pVal          number) return SUMS.VALUE%TYPE
as
    vRes SUMS.VALUE%TYPE :=0;
begin
    if pAccCodeTempl=pAccCodeCurr and pSideTempl=pSideCurr then
        if pStorno = 0 then
            vRes:=pVal;
        else
            vRes:=-pVal;
        end if;
    end if;
    return vRes;
end;
/
create or replace function
    GetAccCorTotValue (
                    pSideTempl    OP_ITEMS.SIDE%TYPE,
                    pSideCurr     OP_ITEMS.SIDE%TYPE,
                    pStorno       number,
                    pVal          number) return SUMS.VALUE%TYPE
as
    vRes SUMS.VALUE%TYPE :=0;
begin
    if pSideTempl=pSideCurr then
        if pStorno = 0 then
            vRes:=pVal;
        else
            vRes:=-pVal;
        end if;
    end if;
    return vRes;
end;
/
create or replace procedure RefreshAccs as  --  Для вычисления уровня счетов
    cursor S is
        select LEVEL, CODE, ROWID R, nvl(LEV, -1 ) LEV from ACCS
        start with PARENT is NULL connect by prior CODE=PARENT ;
    vCurrLevel number(4);

begin
    for X in S loop
        if X.LEVEL!=X.LEV then
            update ACCS set LEV=X.LEVEL
            where ROWID=X.R;
            commit;
        end if;        
    end loop;
end; 
/
create or replace function
    GetAccDKValue (
                    pSideTempl    OP_ITEMS.SIDE%TYPE,
                    pSideCurr     OP_ITEMS.SIDE%TYPE,
                    pVal          number) return SUMS.VALUE%TYPE
as
    vRes SUMS.VALUE%TYPE :=NULL;
begin
    if pSideTempl=pSideCurr then
        vRes:=pVal;
    end if;
    return vRes;
end;
/

create or replace function
    GetDKValue (    pSideCurr     OP_ITEMS.SIDE%TYPE,
                    pVal          number) return SUMS.VALUE%TYPE
as
    vRes SUMS.VALUE%TYPE :=NULL;
begin
    if pSideCurr='D' and pVal>0 then
        vRes:=pVal;
    elsif pSideCurr!='D' and pVal<0 then
        vRes:=-pVal;
    end if;
    return vRes;
end;
/

create or replace procedure
    V_RT ( pBR out number, pDT out number, pCT out number, pER out number,
           pAccCode number, pDtBeg date, pDtEnd date) as

    vBeg date;
    vEnd date;
    vTmp1 number;
    vTmp2 number;
    vTmp0 number;

    vDT number:=0;
    vCT number:=0;

    vBeg1 date;
    vEnd1 date;
begin
    -- Находим остаток на начало
    select max(BEG_DATE), max(END_DATE) into vBeg, vEnd
    from CINTS
    where
        (END_DATE-BEG_DATE)<35 and
        pDtBeg between BEG_DATE and END_DATE;

    if vBeg=pDtBeg or vEnd=pDtBeg then
        select VALUE into pBR from SUMS
        where SUM_ID =
            (select END_VAL_ID from REMS 
             where CODE=pAccCode and REM_DATE=pDtBeg);   
    else
        --- Определяем куда дальше считать
        select count(*) into vTmp1 from OP_ITEMS OI, OPERS O
        where
            OI.CODE=pAccCode and 
            OI.OP_DATE>= vBeg and OI.OP_DATE< pDtBeg and
            OI.OP_ID = O.OP_ID and 
            OI.OP_SUBID = O.OP_SUBID and
            O.STATUS= 1 and
            O.POST= 0;


        select count(*) into vTmp2
        from OP_ITEMS OI, OPERS O
        where
            OI.CODE=pAccCode and 
            OI.OP_DATE>= pDtBeg and OI.OP_DATE< vEnd and
            OI.OP_ID = O.OP_ID and 
            OI.OP_SUBID = O.OP_SUBID and
            O.STATUS= 1 and
            O.POST= 0;

        -- Лучше посчитать до конца
        if vTmp1> vTmp2 then
            select sum(GetAccCorTotValue ('D', OI.SIDE, O.STORNO, S.VALUE)),
                   sum(GetAccCorTotValue ('K', OI.SIDE, O.STORNO, S.VALUE))
            into vDT, vCT
            from OP_ITEMS OI, OPERS O, SUMS S
            where
                OI.CODE=pAccCode and
                OI.OP_DATE>= pDtBeg and OI.OP_DATE< vEnd and
                OI.OP_ID = O.OP_ID and
                OI.OP_SUBID = O.OP_SUBID and
                O.STATUS= 1 and
                O.POST= 0 and S.SUM_ID=OI.ITEM_SUM_ID;
            vDT:=nvl(vDT,0);
            vCT:=nvl(vCT,0);

            -- Находим остаток на конец
            select VALUE into vTmp1 from SUMS
            where SUM_ID =
                (select END_VAL_ID from REMS
                 where CODE=pAccCode and REM_DATE=vEnd);

            pBR:=vTmp1-vDT+vCT;
        else
        -- Лучше считать до начала
            select sum(GetAccCorTotValue ('D', OI.SIDE, O.STORNO, S.VALUE)),
                   sum(GetAccCorTotValue ('K', OI.SIDE, O.STORNO, S.VALUE))
            into vDT, vCT
            from OP_ITEMS OI, OPERS O, SUMS S
            where
                OI.CODE=pAccCode and
                OI.OP_DATE>= vBeg and OI.OP_DATE< pDtBeg and
                OI.OP_ID = O.OP_ID and
                OI.OP_SUBID = O.OP_SUBID and
                O.STATUS= 1 and
                O.POST= 0 and S.SUM_ID=OI.ITEM_SUM_ID;
            vDT:=nvl(vDT,0);
            vCT:=nvl(vCT,0);

            -- Находим остаток на конец
            select VALUE into vTmp0 from SUMS
            where SUM_ID =
                (select END_VAL_ID from REMS
                 where CODE=pAccCode and REM_DATE=vEnd);
            --находим обороты
            select SD.VALUE, SC.VALUE into vTmp1, vTmp2
            from TURNS T, SUMS SD, SUMS SC
            where
                T.BEG_DATE=vBeg and
                T.END_DATE=vEnd and
                T.CODE = pAccCode and
                T.D_TURN_ID=SD.SUM_ID and
                T.C_TURN_ID=SC.SUM_ID;

            vDT:=nvl(vTmp1,0)-vDT;
            vCT:=nvl(vTmp2,0)-vCT;

            pBR:=vTmp0-vDT+vCT;
        end if; -- Считаем до начала или конца
    end if;  -- Точно попали в начало периода

    --------------------------------------------------
    -- Находим обороты
    --------------------------------------------------
    if vEnd<pDtEnd then
        vTmp0:=0;
        vBeg:=vEnd;
        while (vTmp0=0) loop
            select max(END_DATE) into vEnd from CINTS
            where   BEG_DATE=vBeg and
                    END_DATE<=pDtEnd;
            if vEnd is NULL then
                vTmp0:=1;
            else                
                --находим обороты
                select SD.VALUE, SC.VALUE into vTmp1, vTmp2
                from TURNS T, SUMS SD, SUMS SC
                where
                    T.BEG_DATE=vBeg and
                    T.END_DATE=vEnd and
                    T.CODE = pAccCode and
                    T.D_TURN_ID=SD.SUM_ID and
                    T.C_TURN_ID=SC.SUM_ID;

                vDT:=nvl(vTmp1,0)+vDT;
                vCT:=nvl(vTmp2,0)+vCT;
                vBeg:=vEnd;
            end if;
        end loop;
        
        --- Находим остаток оборотов
        if (vBeg!=pDtEnd) then
            select sum(GetAccCorTotValue ('D', OI.SIDE, O.STORNO, S.VALUE)),
                   sum(GetAccCorTotValue ('K', OI.SIDE, O.STORNO, S.VALUE))
            into vTmp1, vTmp2
            from OP_ITEMS OI, OPERS O, SUMS S
            where
                OI.CODE=pAccCode and
                OI.OP_DATE>= vBeg and OI.OP_DATE< pDtEnd and
                OI.OP_ID = O.OP_ID and
                OI.OP_SUBID = O.OP_SUBID and
                O.STATUS= 1 and
                O.POST= 0 and S.SUM_ID=OI.ITEM_SUM_ID;
            vDT:=vDT+nvl(vTmp1,0);
            vCT:=vCt+nvl(vTmp2,0);
        end if;
    else
        -- Находим обороты между двумя датами
        select sum(GetAccCorTotValue ('D', OI.SIDE, O.STORNO, S.VALUE)),
               sum(GetAccCorTotValue ('K', OI.SIDE, O.STORNO, S.VALUE))
        into vTmp1, vTmp2
        from OP_ITEMS OI, OPERS O, SUMS S
        where
            OI.CODE=pAccCode and
            OI.OP_DATE>= pDtBeg and OI.OP_DATE< pDtEnd and
            OI.OP_ID = O.OP_ID and
            OI.OP_SUBID = O.OP_SUBID and
            O.STATUS= 1 and
            O.POST= 0 and S.SUM_ID=OI.ITEM_SUM_ID;
        vDT:=nvl(vTmp1,0);
        vCT:=nvl(vTmp2,0);
    end if;


    pDT:=vDT;
    pCT:=vCT;
    pER:=pBR+pDT-pCT;
end;
/
create or replace function
GetDbLine (pOP_ID number, pOP_SUBID number) return varchar2
as
    vDbName varchar2(30);
    vArticle varchar2(30);
    vVer number;
    vTiedToDb number;
    vRes varchar2(250);
begin
    GetLinkDb(pOP_ID, pOP_SUBID, vDbName, vArticle, vVer, vTiedToDb);
    if vDbName is not NULL then
        vRes:=GetDbShortInfo( vDbName, vArticle, vVer);
    end if;
    return vRes;    
end;
/
--------------------------------------
-- для функционирования объекта AccMan
--------------------------------------
create or replace function
OpSign ( pSide OP_ITEMS.SIDE%TYPE, pStorno OPERS.STORNO%Type) return NUMBER
as
begin
    if (pSide='D' and pStorno=0) or (pSide!='D' and pStorno=1) then
        return 1;
    else
        return 0;
    end if;
end;
/            
create or replace procedure
GetBegSumAndDir ( pAccCode number, pDate date,
                pBegSum out number,
                pOpCursor out Ams_Types.GenCursor) as
    vBegDate date;
    vEndDate date;
    vTmp1 number(8);
    vTmp2 number(8);
begin      
--------- Возвращает код аналитической суммы и дату от которой нужно дальше считать остаток
    select max(BEG_DATE), max(END_DATE) into vBegDate, vEndDate from CINTS
    where (END_DATE-BEG_DATE<35) and pDate between BEG_DATE and END_DATE;

    pBegSum:=0;

    if vBegDate is NULL then
        return;
    end if;

    if vBegDate=pDate OR vEndDate=pDate then
        select max(END_VAL_ID) into pBegSum from REMS
        where CODE=pAccCode and REM_DATE=pDate;
        pBegSum:=nvl(pBegSum,0);
        return;
    end if;
        
        
    select count(*) into vTmp1 from OP_ITEMS OI, OPERS O
    where   OI.OP_DATE>=vBegDate and OI.OP_DATE<pDate and
            OI.CODE=pAccCode and
            OI.OP_ID=O.OP_ID and
            OI.OP_SUBID=O.OP_SUBID and
            O.STATUS=1 and O.POST=0; 

    select count(*) into vTmp2
    from OP_ITEMS OI, OPERS O
    where   OI.OP_DATE>=pDate and OI.OP_DATE<vEndDate and
            OI.CODE=pAccCode and
            OI.OP_ID=O.OP_ID and
            OI.OP_SUBID=O.OP_SUBID and
            O.STATUS=1 and O.POST=0; 

    if vTmp1 > vTmp2 then
        vBegDate:=vEndDate;
        open pOpCursor for
            select OI.ITEM_SUM_ID, -OpSign(OI.SIDE, O.STORNO) OS  
            from OP_ITEMS OI, OPERS O
            where   OI.OP_DATE>=pDate and OI.OP_DATE<vEndDate and
                    OI.CODE=pAccCode and
                    OI.OP_ID=O.OP_ID and
                    OI.OP_SUBID=O.OP_SUBID and
                    O.STATUS=1 and O.POST=0;
    else
        open pOpCursor for
            select OI.ITEM_SUM_ID, OpSign(OI.SIDE, O.STORNO) OS  
            from OP_ITEMS OI, OPERS O
            where   OI.OP_DATE>=vBegDate and OI.OP_DATE<pDate and
                    OI.CODE=pAccCode and
                    OI.OP_ID=O.OP_ID and
                    OI.OP_SUBID=O.OP_SUBID and
                    O.STATUS=1 and O.POST=0;
    end if;


    select max(END_VAL_ID) into pBegSum from REMS
    where CODE=pAccCode and REM_DATE=vBegDate;
    pBegSum:=nvl(pBegSum,0);
end;
/
create or replace procedure
SetDeadDateToday as
begin
    update USER_OPTIONS set DEAD_DATE= TRUNC(SYSDATE, 'DD')
    where USERNAME=USER;
end;
/
show errors;

exit
