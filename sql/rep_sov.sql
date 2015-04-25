/*
create table kadry1 as select * from kadry ;
drop table kadry ;

CREATE TABLE KADRY
(
CatId NUMBER(*,0),
Article              VARCHAR2(30),
Ver                  NUMBER(8),
”ˆŽ                  VARCHAR2(50),
„Ž‹†Ž‘’œ            VARCHAR2(50),
ŠŽ„Ž„              VARCHAR2(5),
‘…ˆŸ                VARCHAR2(6),
N€‘                VARCHAR2(10),
„€’€_ˆ_Œ…‘’Ž         VARCHAR2(50),
€„…‘                VARCHAR2(50),
€‹Žƒˆ              VARCHAR2(12),
„€’€_ˆ…Œ€          DATE,
„€’€_“‚Ž‹œ…ˆŸ      DATE,
ŽŠ‹€„                NUMBER(24,6),
‘                    NUMBER(8),
„…’ˆ                 NUMBER(8),
‹œƒŽ’€               NUMBER(8),
„3                   NUMBER(8),
—…                 NUMBER(8),
€                   NUMBER(8),
€‘                  NUMBER(8),
”‘                  NUMBER(8),
“‚Ž‹                 NUMBER(8),
…‘_               NUMBER(8),
€‚€‘                NUMBER(24,6),
_ŠŽŒŒ“             NUMBER(24,6),
ƒˆ                  VARCHAR2(30),
ƒˆ_V                NUMBER(8),
CONSTRAINT PK_KADRY PRIMARY KEY (Article,Ver),
CONSTRAINT FK_CAT_KADRY FOREIGN KEY (CatId) REFERENCES DBCAT (OWNCAT)
) ;
CREATE INDEX CAT_KADRY ON KADRY (CatId,Article,Ver) ;
GRANT SELECT,UPDATE,INSERT,DELETE ON KADRY TO HANDELSSUPER ;
*/

insert into kadry select * from kadry1 ;

update dbstr set realname='‘' where realname='‘Ž‚' ;

update kadry set article=' ' where article='!' ;
