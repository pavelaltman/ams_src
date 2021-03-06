CREATE TABLE dbhistory                 -- ����� ᮧ�����,��������� � 㤠����� ����権 ��
       (
        db_id    NUMBER,               -- �� ��ꥪ�
        article  varchar2(30),         -- ��⨪� ����樨
        ver      number(8),            -- ����� ����樨
        revision NUMBER,               -- ����� ���������
        rev_time DATE,                 -- ��� ���������
        username VARCHAR2(30),         -- ���짮��⥫�
        CONSTRAINT dbhist_prim PRIMARY KEY (db_id,article,ver,revision),
        CONSTRAINT dbhist_user FOREIGN KEY (username) REFERENCES user_options (username),
        CONSTRAINT dbhist_objs FOREIGN KEY (db_id) REFERENCES objs (oid) ON DELETE CASCADE
       ) ;
