/*==============================================================*/
/* Table : UDBOPT                                               */
/*==============================================================*/

CREATE TABLE UDBOPT (
   DBID                 NUMBER(8)                       NOT NULL,
   REALNAME             VARCHAR2(30)                    NOT NULL,
   SETNAME              VARCHAR2(20)                    NOT NULL,
   USERNAME             VARCHAR2(30)                    NOT NULL,
   DISP_OPT             NUMBER(8)                       NOT NULL,
   CONSTRAINT PK_UDBOPT PRIMARY KEY ( DBID, REALNAME, SETNAME, USERNAME),
   CONSTRAINT FK_UDBOPT_2_DBSTR FOREIGN KEY (DBID, REALNAME)
         REFERENCES DBSTR (DBID, REALNAME)
         ON DELETE CASCADE,
   CONSTRAINT FK_UDBOPT_2_USER_OPT FOREIGN KEY (USERNAME)
         REFERENCES USER_OPTIONS (USERNAME)
         ON DELETE CASCADE
)
/
COMMENT ON TABLE UDBOPT IS 'Ž‹œ‡Ž‚€’…‹œ‘Šˆ… “‘’€Ž‚Šˆ „‹Ÿ €‡› „€›• „‹Ÿ Š€†„ŽƒŽ Ž‹œ‡Ž‚€’…‹Ÿ ˆ „‹Ÿ Š€†„ŽƒŽ €Ž€ “‘’€Ž‚ŽŠ'
/
COMMENT ON COLUMN UDBOPT.SETNAME IS '€Ž “‘’€Ž‚ŽŠ „‹Ÿ “‘’ŽƒŽ €Ž€ = " " (Ž…‹) ’.Š. ‚•Ž„ˆ’ ‚ …ˆ—›‰ Š‹ž— ˆ … ŒŽ†…’ ›’œ NULL, Ž’ŽŒ“ … €„Ž ‘‘›‹Šˆ € DBOPT'
/
grant select on UDBOPT to &1.ROLE;
exit;