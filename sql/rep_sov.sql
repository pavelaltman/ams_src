/*
create table kadry1 as select * from kadry ;
drop table kadry ;

CREATE TABLE KADRY
(
CatId NUMBER(*,0),
Article              VARCHAR2(30),
Ver                  NUMBER(8),
���                  VARCHAR2(50),
���������            VARCHAR2(50),
�������              VARCHAR2(5),
�����                VARCHAR2(6),
N����                VARCHAR2(10),
����_�_�����         VARCHAR2(50),
�����                VARCHAR2(50),
�������              VARCHAR2(12),
����_������          DATE,
����_����������      DATE,
�����                NUMBER(24,6),
�                    NUMBER(8),
����                 NUMBER(8),
������               NUMBER(8),
�3                   NUMBER(8),
����                 NUMBER(8),
��                   NUMBER(8),
���                  NUMBER(8),
���                  NUMBER(8),
����                 NUMBER(8),
����_�               NUMBER(8),
�����                NUMBER(24,6),
�_������             NUMBER(24,6),
���                  VARCHAR2(30),
���_V                NUMBER(8),
CONSTRAINT PK_KADRY PRIMARY KEY (Article,Ver),
CONSTRAINT FK_CAT_KADRY FOREIGN KEY (CatId) REFERENCES DBCAT (OWNCAT)
) ;
CREATE INDEX CAT_KADRY ON KADRY (CatId,Article,Ver) ;
GRANT SELECT,UPDATE,INSERT,DELETE ON KADRY TO HANDELSSUPER ;
*/

insert into kadry select * from kadry1 ;

update dbstr set realname='�' where realname='���' ;

update kadry set article=' ' where article='!' ;
