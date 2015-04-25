/* Начало новой бухгалтерии */

alter rollback segment myroll online ;

ALTER SESSION SET nls_date_language=RUSSIAN ;
ALTER SESSION SET NLS_DATE_FORMAT = 'DD-MM-YY' ;
set serveroutput on

declare
 res  NUMBER ;
 nl   NUMBER ;
 root NUMBER ;
 nr   NUMBER ;
begin

 begin

   select count(*) into nr from objcat where descr='/' ;
   IF nr=0 THEN
      -- Начальный узел иерархии
      root:=NULL ;
      InsertCat(NULL,'/',root,res) ;
      dbms_output.put_line(res);
      dbms_output.put_line(root);

   END IF ;

   select owncat into root from objcat where descr='/' ;

   INIT_RIGHTS(root) ;
   commit ;

   select count(*) into nr from objs where name='-' ;
   IF nr=0 THEN
      -- добавление документа '-'
      nl:=NULL ;
      InsertObj(root,1,'-','',nl,res) ;
      dbms_output.put_line(res);
      dbms_output.put_line(nl);
   END IF ;
 exception
   when others then commit ;
 end ;


 periods.AddAccount('A','Балансовые счета',NULL,1,res) ;
 dbms_output.put_line(res);

 periods.AddAccount('B','Нацiональнi стандарти',NULL,1,res) ;
 dbms_output.put_line(res);

 periods.AddAccount('Z','Внебалансовые счета',NULL,0,res) ;
 dbms_output.put_line(res);


 commit ;

 BEGIN
   INSERT INTO user_options(username,full_name) VALUES (GETSHEMA,GETSHEMA) ;
 EXCEPTION
   WHEN OTHERS THEN
   dbms_output.put_line('already inserted');
 END ;

 DECLARE
  cpnum NUMBER ;
 BEGIN
  select count(*) into cpnum from cps ;
  if cpnum=0 then periods.CreateFirstMonth('1-01-2002') ; end if ;
  if cpnum<2 then periods.BeginNextMonth ; end if ;
 END

 commit ;
end;
/

rem VT_DATE=1,VT_NUM=2,VT_INT=3,VT_ROWID=4,VT_STRING=5
rem Addable=0x1,Totalable=0x2,DebetCredit=0x8,Hidden=0x10,SystemFld=0x100
rem Frozen=0x200,Editable=0x400,AlwaysFree=0x800
rem ZestFld=0x8000,RemFld=0x10000,ShowFld=0x20000
declare
 key number;
 res number;
 root number ;
begin
 key:=NULL;
 select owncat into root from objcat where descr='/' ;
 InsertObj(root,7,'OPERVIEW','Журнал операций',key,res);
 dbms_output.put('Use OID:');
 dbms_output.put_line(key);
 dbms_output.put('Result of insert:');
 dbms_output.put_line(res);
 if res=0 then
    insert into DBSTR
     (DBID,RealName,vtype,maxlen,Alias         ,width,dec,ord,def_opt) values
     (key ,'OP_ID' ,3    ,8     ,'Код операции',8    ,0  ,1  ,256+16 );
    insert into DBSTR
     (DBID,RealName,vtype,maxlen,Alias         ,width,dec,ord,def_opt) values
     (key ,'OP_SUBID',3  ,8     ,'Суб-код операции',4,0  ,2  ,256+16 );
    insert into DBSTR
     (DBID,RealName,vtype,maxlen,Alias         ,width,dec,ord,def_opt) values
     (key ,'STATUS',3    ,1     ,'Статус'      ,1    ,0  ,3  ,256+16  );
    insert into DBSTR
     (DBID,RealName,vtype,maxlen,Alias         ,width,dec,ord,def_opt) values
     (key ,'POST'  ,3    ,1     ,'Отложена'    ,1    ,0  ,4  ,256+16  );
    insert into DBSTR
     (DBID,RealName,vtype,maxlen,Alias         ,width,dec,ord,def_opt) values
     (key ,'STORNO',3    ,1     ,'Сторно'      ,1    ,0  ,5  ,16    );
    insert into DBSTR
     (DBID,RealName,vtype,maxlen,Alias         ,width,dec,ord,def_opt) values
     (key ,'TEXT'  ,5    ,250    ,'Текст'      ,25   ,0  ,6  ,3072  );
    insert into DBSTR
     (DBID,RealName,vtype,maxlen,Alias         ,width,dec,ord,def_opt) values
     (key ,'DOC_TYPE',5  ,20    ,'Тип документа',7   ,0  ,7  ,0     );
    insert into DBSTR
     (DBID,RealName,vtype,maxlen,Alias         ,width,dec,ord,def_opt) values
     (key ,'DOC_NUM',5   ,30    ,'Номер документа',4 ,0  ,8  ,0     );
    insert into DBSTR
     (DBID,RealName,vtype,maxlen,Alias         ,width,dec,ord,def_opt) values
     (key ,'DOC_NUM_V',3 ,8     ,'Версия номера',2   ,0  ,9  ,0     );
    insert into DBSTR
     (DBID,RealName,vtype,maxlen,Alias         ,width,dec,ord,def_opt) values
     (key ,'OP_DATE',1   ,8     ,'Дата операции',8   ,0  ,10 ,0     );
    insert into DBSTR
     (DBID,RealName,vtype,maxlen,Alias         ,width,dec,ord,def_opt) values
     (key ,'COND_ID' ,3    ,8     ,'Код стд. оп.',8  ,0  ,11  ,16   );
     commit;
 end if ;

 key:=NULL;
 InsertObj(root,7,'ACCVIEW','Просмотр счета',key,res);
 dbms_output.put('Use OID:');
 dbms_output.put_line(key);
 dbms_output.put('Result of insert:');
 dbms_output.put_line(res);
 if res!=0 then return; end if;
 insert into DBSTR
  (DBID,RealName,vtype,maxlen,Alias         ,width,dec,ord,def_opt) values
  (key ,'OP_ID' ,3    ,8     ,'Код операции',8    ,0  ,1  ,256+16 );
 insert into DBSTR
  (DBID,RealName,vtype,maxlen,Alias         ,width,dec,ord,def_opt) values
  (key ,'OP_SUBID',3  ,8     ,'Суб-код операции',4,0  ,2  ,256+16 );
 insert into DBSTR
  (DBID,RealName,vtype,maxlen,Alias         ,width,dec,ord,def_opt) values
  (key ,'STATUS',3    ,1     ,'Статус'      ,1    ,0  ,3  ,256+16  );
 insert into DBSTR
  (DBID,RealName,vtype,maxlen,Alias         ,width,dec,ord,def_opt) values
  (key ,'POST'  ,3    ,1     ,'Отложена'    ,1    ,0  ,4  ,256+16  );
 insert into DBSTR
  (DBID,RealName,vtype,maxlen,Alias         ,width,dec,ord,def_opt) values
  (key ,'STORNO',3    ,1     ,'Сторно'      ,1    ,0  ,5  ,16    );
 insert into DBSTR
  (DBID,RealName,vtype,maxlen,Alias         ,width,dec,ord,def_opt) values
  (key ,'TEXT'  ,5    ,250    ,'Текст'      ,25   ,0  ,6  ,3072  );
 insert into DBSTR
  (DBID,RealName,vtype,maxlen,Alias         ,width,dec,ord,def_opt) values
  (key ,'DOC_TYPE',5  ,20    ,'Тип документа',7   ,0  ,7  ,0     );
 insert into DBSTR
  (DBID,RealName,vtype,maxlen,Alias         ,width,dec,ord,def_opt) values
  (key ,'DOC_NUM',5   ,30    ,'Номер документа',4 ,0  ,8  ,0     );
 insert into DBSTR
  (DBID,RealName,vtype,maxlen,Alias         ,width,dec,ord,def_opt) values
  (key ,'DOC_NUM_V',3 ,8     ,'Версия номера',2   ,0  ,9  ,0     );
 insert into DBSTR
  (DBID,RealName,vtype,maxlen,Alias         ,width,dec,ord,def_opt) values
  (key ,'OP_DATE',1   ,8     ,'Дата операции',8   ,0  ,10 ,0     );
 insert into DBSTR
  (DBID,RealName,vtype,maxlen,Alias         ,width,dec,ord,def_opt) values
  (key ,'SIDE'  ,5   ,1     ,'Дебет/Кредит' ,4    ,0  ,11  ,0     );
 insert into DBSTR
  (DBID,RealName,vtype,maxlen,Alias         ,width,dec,ord,def_opt) values
  (key ,'VALUE' ,2   ,25    ,'Сумма'        ,25   ,6  ,12  ,0     );
 insert into DBSTR
  (DBID,RealName,vtype,maxlen,Alias         ,width,dec,ord,def_opt) values
  (key ,'COND_ID' ,3    ,8   ,'Код стд. оп.',8  ,0  ,13  ,16   );
  commit;

end;
/

exit
