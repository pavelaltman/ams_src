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
comment on table CATS_MAP is '������ ��� ��।�� ��⥣�਩ �१ ������'
/
comment on column CATS_MAP.DB_ID is '��� ���� ������'
/
comment on column CATS_MAP.OCAT_THERE is '��� ��⥣�ਨ � ��㣮� ��壠��ਨ'
/
comment on column CATS_MAP.OCAT_HERE is '��� ��⥣�ਨ � ⥪�饩 ��壠��ਨ'
/
grant insert,select,update on CATS_MAP to &1._EXIM;
exit
