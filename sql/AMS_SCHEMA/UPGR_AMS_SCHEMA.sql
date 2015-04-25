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
comment on table SYS_INFO is '�࠭�� ��⥬��� ���ଠ�� -
1) ����, ��� �࠭���� SQL 䠩��, SqlPath
2) ����, �㤠 ��࠭塞                 SavePath
3)           ��㤠 ����⠭�������� RestorePath
3) �������� �६������ ᥣ���� TempSegName
4)                   ���짮��⥫�᪮��      UserSegName
 

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
comment on table LICENSES is '�뤠��� ��業��� �।�����'
/
comment on column LICENSES.ID is '����� ��業���'
/
comment on column LICENSES.FIRM is '��ଠ ���� �뤠�� ��業���'
/


comment on column LICENSES.DOC_PATH is '���� �� ���஬� �ᯮ������ ���㬥��� ��� ������ ��壠��ਨ'
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
         constraint CKC_STATUS_BOOKS check (STATUS in ('�������','���������','�����')),
   LAST_UPD             VARCHAR2(20),
   DESCR                VARCHAR2(255),
   constraint PK_BOOKS primary key (SUB_ID, ID),
   constraint FK_BOOKES_2_LICENSES foreign key (ID)
         references LICENSES (ID)
         on delete cascade
)
/
comment on table BOOKS is '���᮪ ��� ����㯭�� ��壠��਩'
/
comment on column BOOKS.ID is '�����䨪��� ��壠��ਨ (㭨����� � ���)'
/
comment on column BOOKS.SUB_ID is '�������� ��� ��ਮ��� - 0 - ��砫쭠� ��⠭���� � � �� ���浪�.
��⨢�� ��ਮ� �� �।����� = ���ᨬ��쭮� ���祭�� �� SUB_ID '
/
comment on column BOOKS.BASE_USER is '��� �������� ���짮��⥫�'
/
comment on column BOOKS.SYS_BASE_USER is '��� �������� ���짮��⥫� � ��⥬� � ������ �� ��設�'
/
comment on column BOOKS.STATUS is '����ﭨ� ��⥬� �������� ��ਠ���
�������
���������
�����'
/
comment on column BOOKS.LAST_UPD is '����� ��᫥����� �ந���������� update'
/
comment on column BOOKS.DESCR is '���ᠭ�� ��⥬�(�� �� ������ � ������ ���  �������� ���짮��⥫�)'
/
insert into SYS_INFO(VAL_TYPE,VAL_DESCR)values('SavePath','�㤠 ��࠭塞 䠩��-��娢�')            ;
insert into SYS_INFO(VAL_TYPE,VAL_DESCR)values('RestorePath','��㤠 �㤥� �ந�������� ����⠭�������')                                                                                                                                                                                                    ;
insert into SYS_INFO(VAL_TYPE,VAL_DESCR)values('SqlPath','��� �࠭���� �⠭����� SQL 䠩��  ');

CREATE role AMS_BUCH_ROLE; 
GRANT select, insert,update, delete on AMS.SYS_INFO to AMS_BUCH_ROLE;
GRANT select, insert,update, delete on AMS.LICENSES to AMS_BUCH_ROLE;
GRANT select, insert,update, delete on AMS.BOOKS to AMS_BUCH_ROLE;

CREATE role AMS_USER; 
GRANT select on AMS.SYS_INFO to AMS_USER;
GRANT select on AMS.LICENSES to AMS_USER;
GRANT select on AMS.BOOKS to AMS_USER;

exit;


