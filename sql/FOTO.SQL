drop table fotos ;
create table fotos
       (
        ART   VARCHAR2(30),
        V     NUMBER,
        FOTO  LONG RAW ,
        constraint FOTOS_PK PRIMARY KEY (ART,V),
        constraint FOTOS_FK FOREIGN KEY (ART,V) REFERENCES ����� (ARTICLE,VER)
       ) ;
insert into fotos (ART,V)
  SELECT ARTICLE,VER FROM ����� ;

drop table infos ;
create table infos
       (
        ART   VARCHAR2(30),
        V     NUMBER,
        INFO  LONG RAW ,
        constraint INFOS_PK PRIMARY KEY (ART,V),
        constraint INFOS_FK FOREIGN KEY (ART,V) REFERENCES ����� (ARTICLE,VER)
       ) ;
insert into INFOS (ART,V)
  SELECT ARTICLE,VER FROM ����� ;
