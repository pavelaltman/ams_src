DROP TABLE dbaddinfo ;
CREATE TABLE dbaddinfo
       (
        dbid        NUMBER(8),     -- �� ��ꥪ�
        setname     VARCHAR2(20),  -- ��� �����
        RealName    VARCHAR2(30),  -- ��� ����
        name        VARCHAR2(30),  -- ���⥬��� ���
        code        NUMBER(*,0),   -- ��� ���
        turntype    VARCHAR2(3),   -- ��� �㬬� (R,BR,DT,CT)
        alias       VARCHAR2(30),  -- ���짮��⥫�᪮� ���
        CONSTRAINT  PK_DBADD PRIMARY KEY (DBID,setname,RealName,name),
        CONSTRAINT  DBADD_Set FOREIGN KEY (DBID,SetName) REFERENCES dbopt (dbid,SetName) ON DELETE CASCADE,
        CONSTRAINT  DBADD_id FOREIGN KEY (DBID,RealName) REFERENCES dbstr (dbid,RealName) ON DELETE CASCADE,
        CONSTRAINT  dbadd_code FOREIGN KEY (code) REFERENCES accs (code)
       );
exit
