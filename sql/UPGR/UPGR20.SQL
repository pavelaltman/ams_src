alter table conds add
       (
        native_doc   NUMBER(1)       -- признак "сохранять только с родным типом документа"
       ) ;

alter table conds add
       (
        auto_apply   NUMBER(1)       -- признак "Проводить автоматически"
       ) ;

exit
