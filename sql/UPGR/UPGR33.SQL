/*==============================================================*/
/* Table : TMP_SUMS                                             */
/*==============================================================*/
create table TMP_SUMS (
   SUM_ID               NUMBER                          not null,
   CREATE_DATE          DATE                            not null,
   CREATOR              VARCHAR2(30)                    not null,
   DESCR                VARCHAR2(255),
   constraint PK_TMP_SUMS primary key (SUM_ID),
   constraint FK_TMP_SUMS_2_SUMS foreign key (SUM_ID)
         references SUMS (SUM_ID)
         on delete cascade
)
/

comment on table TMP_SUMS is 'Таблица хранит коды временных сумм' ;
comment on column TMP_SUMS.SUM_ID is 'Код временной суммы';
comment on column TMP_SUMS.CREATE_DATE is 'Дата создания временной суммы';
comment on column TMP_SUMS.CREATOR is 'Пользователь, создавший временную сумму';
comment on column TMP_SUMS.DESCR is 'Описание временной суммы (может быть NULL)';

exit

