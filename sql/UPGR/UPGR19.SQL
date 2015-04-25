DROP TABLE dbaddinfo ;
CREATE TABLE dbaddinfo
       (
        dbid        NUMBER(8),     -- Ид объекта
        setname     VARCHAR2(20),  -- Имя набора
        RealName    VARCHAR2(30),  -- Имя поля
        name        VARCHAR2(30),  -- Системное имя
        code        NUMBER(*,0),   -- Код счета
        turntype    VARCHAR2(3),   -- Тип суммы (R,BR,DT,CT)
        alias       VARCHAR2(30),  -- Пользовательское имя
        CONSTRAINT  PK_DBADD PRIMARY KEY (DBID,setname,RealName,name),
        CONSTRAINT  DBADD_Set FOREIGN KEY (DBID,SetName) REFERENCES dbopt (dbid,SetName) ON DELETE CASCADE,
        CONSTRAINT  DBADD_id FOREIGN KEY (DBID,RealName) REFERENCES dbstr (dbid,RealName) ON DELETE CASCADE,
        CONSTRAINT  dbadd_code FOREIGN KEY (code) REFERENCES accs (code)
       );
exit
