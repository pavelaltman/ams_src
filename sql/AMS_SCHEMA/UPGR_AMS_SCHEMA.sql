/*==============================================================*/
/* Database name:  AMS_SCHEMA                                   */
/* DBMS name:      ORACLE Version 8i                            */
/* Created on:     25.10.01 13:20:21                            */
/*==============================================================*/

/*==============================================================*/
/* Table : SYS_INFO                                             */
/*==============================================================*/
create table SYS_INFO (
   VAL_TYPE             VARCHAR2(20)                    not null,
   VALUE                VARCHAR2(250),
   VAL_DESCR            VARCHAR2(255),
   constraint PK_SYS_INFO primary key (VAL_TYPE)
)
/
comment on table SYS_INFO is 'Хранит системную информацию -
1) путь, где хранятся SQL файлы, SqlPath
2) путь, куда сохраняем                 SavePath
3)           откуда восстанавливаем RestorePath
3) Название временного сегмента TempSegName
4)                   пользовательского      UserSegName
 

/
/*==============================================================*/
/* Table : LICENSES                                             */
/*==============================================================*/
create table LICENSES (
   ID                   NUMBER(8)                       not null,
   FIRM                 VARCHAR2(250)                   not null,
   DOC_PATH             VARCHAR2(200),
   constraint PK_LICENSES primary key (ID),
   constraint AK_FIRM_LICENSES unique (FIRM)
)
/
comment on table LICENSES is 'Выданные лицензии предприятиям'
/
comment on column LICENSES.ID is 'Номер лицензии'
/
comment on column LICENSES.FIRM is 'Фирма кому выдана лицензия'
/


comment on column LICENSES.DOC_PATH is 'Путь по которому расположены документы для данной бухгалтерии'
/
/*==============================================================*/
/* Table : BOOKS                                                */
/*==============================================================*/
create table BOOKS (
   ID                   NUMBER(8)                       not null,
   SUB_ID               NUMBER(8)                       not null,
   BASE_USER            VARCHAR2(30)                    not null,
   SYS_BASE_USER        VARCHAR2(30)                    not null,
   STATUS               VARCHAR2(20)                    not null
         constraint CKC_STATUS_BOOKS check (STATUS in ('АКТИВНА','ВРЕМЕННАЯ','АРХИВ')),
   LAST_UPD             VARCHAR2(20),
   DESCR                VARCHAR2(255),
   constraint PK_BOOKS primary key (SUB_ID, ID),
   constraint FK_BOOKES_2_LICENSES foreign key (ID)
         references LICENSES (ID)
         on delete cascade
)
/
comment on table BOOKS is 'Список всех доступных бухгалтерий'
/
comment on column BOOKS.ID is 'Идентификатор бухгалтерии (уникальный в мире)'
/
comment on column BOOKS.SUB_ID is 'Вводится для периодов - 0 - начальная установка и тд по порядку.
активный период по предприятию = максимальное значение из SUB_ID '
/
comment on column BOOKS.BASE_USER is 'Имя базового пользователя'
/
comment on column BOOKS.SYS_BASE_USER is 'Имя базового пользователя в системе у клиента на машине'
/
comment on column BOOKS.STATUS is 'Состояние системы возможные варианты
АКТИВНА
ВРЕМЕННАЯ
АРХИВ'
/
comment on column BOOKS.LAST_UPD is 'Номер последнего произведенного update'
/
comment on column BOOKS.DESCR is 'Описание системы(То что пишется в ПОЛНОЕ имя  базового пользователя)'
/
insert into SYS_INFO(VAL_TYPE,VAL_DESCR)values('SavePath','Куда сохраняем файлы-архивы')            ;
insert into SYS_INFO(VAL_TYPE,VAL_DESCR)values('RestorePath','Откуда будет производится восстановление')                                                                                                                                                                                                    ;
insert into SYS_INFO(VAL_TYPE,VAL_DESCR)values('SqlPath','Где хранятся стандартные SQL файлы  ');

CREATE role AMS_BUCH_ROLE; 
GRANT select, insert,update, delete on AMS.SYS_INFO to AMS_BUCH_ROLE;
GRANT select, insert,update, delete on AMS.LICENSES to AMS_BUCH_ROLE;
GRANT select, insert,update, delete on AMS.BOOKS to AMS_BUCH_ROLE;

CREATE role AMS_USER; 
GRANT select on AMS.SYS_INFO to AMS_USER;
GRANT select on AMS.LICENSES to AMS_USER;
GRANT select on AMS.BOOKS to AMS_USER;

exit;


