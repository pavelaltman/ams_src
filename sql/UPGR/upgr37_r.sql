-- Для запоминания порядка полей при копировании из Excel
-- Права выдаются пользователю _EXIM

drop table EXCEL_IMP_ITEMS;
drop table EXCEL_IMP;
/
/*==============================================================*/
/* Table : EXCEL_IMP                                            */
/*==============================================================*/
create table EXCEL_IMP (
   DB_ID                NUMBER(8)                       not null,
   SHORT_NAME           VARCHAR2(20)                    not null,
   DESCR                VARCHAR2(255),
   constraint PK_EXCEL_IMP primary key (DB_ID, SHORT_NAME),
   constraint FK_EXCEL_IM_EXCEL_IMP_OBJS foreign key (DB_ID)
         references OBJS (OID)
)
/
comment on table EXCEL_IMP is 'Хранит последовательность полей для импорта в БД из Excel'
/
comment on column EXCEL_IMP.DB_ID is 'Код БД в которую будем импортировать данные'
/
comment on column EXCEL_IMP.SHORT_NAME is 'Короткое название импорта'
/
comment on column EXCEL_IMP.DESCR is 'Подробное описание '
/
/*==============================================================*/
/* Table : EXCEL_IMP_ITEMS                                      */
/*==============================================================*/
create table EXCEL_IMP_ITEMS (
   DB_ID                NUMBER(8)                       not null,
   SHORT_NAME           VARCHAR2(20)                    not null,
   ORD                  NUMBER(4)                       not null,
   REALNAME             VARCHAR2(30)                    not null,
   constraint PK_EXCEL_IMP_ITEMS primary key (DB_ID, SHORT_NAME, ORD),
   constraint FK_EXCEL_IM_REFERENCE_EXCEL_IM foreign key (DB_ID, SHORT_NAME)
         references EXCEL_IMP (DB_ID, SHORT_NAME)
)
/
comment on table EXCEL_IMP_ITEMS is 'Хранит порядок полей'
/
comment on column EXCEL_IMP_ITEMS.ORD is 'Порядок поля'
/
comment on column EXCEL_IMP_ITEMS.REALNAME is 'Название поля'
/
grant select, delete, insert on EXCEL_IMP to &1._EXIM;
grant select, delete, insert on EXCEL_IMP_ITEMS to &1._EXIM;
exit;