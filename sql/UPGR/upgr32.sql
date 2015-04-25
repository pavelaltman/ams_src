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


comment on table AN_FLD_ORDER is 'Для определения порядка полей при
редактировании аналитической суммы'
/


comment on column AN_FLD_ORDER.ANID is 'Код аналитики которую редактируем'
/


comment on column AN_FLD_ORDER.SETNAME is 'Имя набора установок'
/


comment on column AN_FLD_ORDER.RealName is 'Как называется поле в аналитике или в базе данных'
/


comment on column AN_FLD_ORDER.name is 'Алиас поля в SQL запросе ( для полей БД и АН пустой?)'
/


comment on column AN_FLD_ORDER.DB_ID is 'для поля базы данных показывает из какой оно БД, для поля аналитики =ANID, для доп полей NULL'
/


comment on column AN_FLD_ORDER.FLD_TYPE is 'Тип поля
             DB - база данных
             AN - аналитика
            ANR - аналитика остатка
            ADD - дополнительное поле'
/


comment on column AN_FLD_ORDER.FLD_ORD is 'Порядок поля при просмотре аналитики'
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
