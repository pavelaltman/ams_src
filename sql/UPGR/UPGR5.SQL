--alter table an127 drop constraint fk_dban127 ;
--delete an127 where aarticle!=' ' ;
--insert into an127
--   select sumid,'���',0,�����_���,����,�����
--   from an127
--   where aarticle=' ' ;
--alter table an127 add constraint fk_dban127 foreign key (aarticle,aver) references currency (article,ver) ;
--commit ;
alter table accs add
      (
       val_code   VARCHAR2(30)
      ) ;


