/*==============================================================*/
/* Table : AN_FLD_ORDER                                         */
/*==============================================================*/
create table AN_FLD_ORDER (
   ANID                 NUMBER(8)                       not null,
   SETNAME              VARCHAR2(20)                    not null,
   RealName             VARCHAR2(30)                    not null,
   name                 VARCHAR2(30)                    not null,
   DB_ID                NUMBER (8),
   FLD_TYPE             VARCHAR2(3)                     not null,
   FLD_ORD              NUMBER(4)                       not null,
   constraint PK_AN_FLD_ORDER primary key (ANID, SETNAME, RealName, name),
   constraint FK_AN_FLD_ORDER_2_OBJS foreign key (ANID)
         references OBJS (OID)
)
/


comment on table AN_FLD_ORDER is '��� ��।������ ���浪� ����� ��
।���஢���� �������᪮� �㬬�'
/


comment on column AN_FLD_ORDER.ANID is '��� �����⨪� ������ ।����㥬'
/


comment on column AN_FLD_ORDER.SETNAME is '��� ����� ��⠭����'
/


comment on column AN_FLD_ORDER.RealName is '��� ���뢠���� ���� � �����⨪� ��� � ���� ������'
/


comment on column AN_FLD_ORDER.name is '����� ���� � SQL ����� ( ��� ����� �� � �� ���⮩?)'
/


comment on column AN_FLD_ORDER.DB_ID is '��� ���� ���� ������ �����뢠�� �� ����� ��� ��, ��� ���� �����⨪� =ANID, ��� ��� ����� NULL'
/


comment on column AN_FLD_ORDER.FLD_TYPE is '��� ����
             DB - ���� ������
             AN - �����⨪�
            ANR - �����⨪� ���⪠
            ADD - �������⥫쭮� ����'
/


comment on column AN_FLD_ORDER.FLD_ORD is '���冷� ���� �� ��ᬮ�� �����⨪�'
/


/*==============================================================*/
/* Index: IND_AN_FLD_ORD                                        */
/*==============================================================*/
create  index IND_AN_FLD_ORD on AN_FLD_ORDER (
   ANID ASC,
   SETNAME ASC,
   FLD_ORD ASC
)
/


/*==============================================================*/
/* Index: IND_AN_FLD_TYPE                                       */
/*==============================================================*/
create  index IND_AN_FLD_TYPE on AN_FLD_ORDER (
   ANID ASC,
   SETNAME ASC,
   FLD_TYPE ASC
)
/
exit
