DROP TABLE unapp_items ;
DROP TABLE unapp_cor_items ;
DROP TABLE unapp_opers ;

CREATE TABLE unapp_opers               -- ����樨, �������騥 �஢������
       (
        op_id    NUMBER,               -- ��� ����樨
        op_subid NUMBER,               -- ��-��� ����樨
        undo     NUMBER(1) NOT NULL,   -- 0-�ਡ�����, 1-�����
        SID      NUMBER NOT NULL,      -- �������� �����䨪��� ��ᨨ
        CONSTRAINT unapp_opers_prim PRIMARY KEY (op_id,op_subid),
        CONSTRAINT unapp_opers_opers FOREIGN KEY (op_id,op_subid) REFERENCES opers (op_id,op_subid)
       ) ;


CREATE TABLE unapp_items               -- �������� ����権, �������騥 �஢������
       (
        op_id    NUMBER,               -- ��� ����樨
        op_subid NUMBER,               -- ��-��� ����樨
        code     NUMBER(*,0),          -- ��� ���
        cp_date  DATE,                 -- ��� ��, � ���ன �㦭� �ਡ�����
        CONSTRAINT unapp_items_prim PRIMARY KEY (op_id,op_subid,code,cp_date),
        CONSTRAINT unapp_items_opers FOREIGN KEY (op_id,op_subid) REFERENCES unapp_opers (op_id,op_subid),
        CONSTRAINT unapp_items_items FOREIGN KEY (op_id,op_subid,code) REFERENCES op_items (op_id,op_subid,code),
        CONSTRAINT unapp_cpdate FOREIGN KEY (cp_date) REFERENCES cps (point_date)
       ) ;

CREATE TABLE unapp_cor_items           -- ����ᯮ������騥 �������� ����権,
                                       -- �������騥 �஢������
       (
        op_id    NUMBER,               -- ��� ����樨
        op_subid NUMBER,               -- ��-��� ����樨
        d_code   NUMBER(*,0),          -- ��� ��� �� ������
        c_code   NUMBER(*,0),          -- ��� ��� �� �।���
        cp_date  DATE,                 -- ��� ��, � ���ன �㦭� �ਡ�����
        CONSTRAINT unapp_cor_prim PRIMARY KEY (op_id,op_subid,d_code,c_code,cp_date),
        CONSTRAINT unapp_cor_opers FOREIGN KEY (op_id,op_subid) REFERENCES unapp_opers (op_id,op_subid),
        CONSTRAINT unapp_cor_items FOREIGN KEY (op_id,op_subid,d_code,c_code) REFERENCES cor_items (op_id,op_subid,d_code,c_code),
        CONSTRAINT unapp_cor_cpdate FOREIGN KEY (cp_date) REFERENCES cps (point_date)
       ) ;
exit
