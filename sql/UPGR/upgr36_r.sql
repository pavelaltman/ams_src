/*==============================================================*/
/* Table : CATS_MAP                                             */
/*==============================================================*/
create table CATS_MAP (
   DB_ID                NUMBER(8)                       not null,
   OCAT_THERE           NUMBER(8)                       not null,
   OCAT_HERE            NUMBER(8)                       not null,
   constraint PK_CATS_MAP primary key (DB_ID, OCAT_THERE),
   constraint FK_CATS_MAP_TO_OBJS foreign key (DB_ID)
         references OBJS (OID)
)
/
comment on table CATS_MAP is 'Таблица для передачи категорий через импорт'
/
comment on column CATS_MAP.DB_ID is 'Код базы данных'
/
comment on column CATS_MAP.OCAT_THERE is 'Код категории в другой бухгалтерии'
/
comment on column CATS_MAP.OCAT_HERE is 'Код категории в текущей бухгалтерии'
/
grant insert,select,update on CATS_MAP to &1._EXIM;
exit
