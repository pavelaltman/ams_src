alter table objs add
      (
       CATS NUMBER(8)
      ) ;
alter table objs add
      (
        CONSTRAINT FK_CATSOBJ FOREIGN KEY (CATS) REFERENCES OBJS(OID)
      ) ;
exit
