/* �������� �᭮���� ⠡��� (�஬� ��� ������ � �����⨪) */

drop table EXCEL_IMP_ITEMS;
drop table EXCEL_IMP;

drop table AN_FLD_ORDER ;
drop table message_table ;
drop table tmp_sums ;
drop TABLE UDBOPT ;
drop table anal_errs ;
drop table privtest;
drop table objpriv;
DROP table DBCATTREE;
DROP TABLE DBCAT;
DROP TABLE DBADDINFO;
DROP TABLE DBOPTOV;
DROP TABLE DBOPT;
DROP TABLE dbhistory ;
DROP TABLE history ;
DROP TABLE unapp_items ;
DROP TABLE unapp_cor_items ;
DROP TABLE unapp_opers ;
DROP TABLE cor_items ;
DROP TABLE op_items ;
DROP TABLE del_op_items ;
DROP TABLE del_opers ;
DROP TABLE opers ;
DROP TABLE cond_items ;
DROP TABLE conds ;
DROP TABLE consts ;
DROP TABLE cattree ;
DROP TABLE cor_turns ;
DROP TABLE turns ;
DROP TABLE rems ;
DROP TABLE sums ;
DROP TABLE user_options ;
DROP TABLE cints ;
DROP TABLE cps ;
DROP TABLE ex_accs ;
DROP TABLE accs ;
DROP TABLE DBSTR;
DROP TABLE objs;
DROP TABLE objcat;
DROP SEQUENCE SumSeq ;
DROP SEQUENCE OperSeq ;
DROP SEQUENCE ObjSeq ;
DROP SEQUENCE TreeSeq ;
DROP SEQUENCE DBCatSeq ;

CREATE SEQUENCE SumSeq        -- ��᫥����⥫쭮��� ���祩 ��� ⠡���� sums
INCREMENT BY 1
START WITH 1
CACHE 100 ;

CREATE SEQUENCE OperSeq   -- ��᫥����⥫쭮��� ���祩 ��� ⠡���� ����権
INCREMENT BY 1
START WITH 1
CACHE 20 ;

CREATE SEQUENCE ObjSeq   -- ��᫥����⥫쭮��� ���祩 ��� ⠡��� ��⮢
INCREMENT BY 1           -- � ��ꥪ⮢
START WITH 1
NOCACHE ;

CREATE SEQUENCE TreeSeq  -- ��᫥����⥫쭮��� ���祩 ��� ��ॢ� ��ꥪ⮢
INCREMENT BY 1
START WITH 1
CACHE 100 ;

CREATE SEQUENCE DBCatSeq -- ��᫥����⥫쭮��� ���祩 ��� ��⥣�਩ ��
INCREMENT BY 1
START WITH 1
CACHE 10 ;

CREATE TABLE OBJCAT                                 -- ������ ��ꥪ⮢
       (OWNCAT NUMBER(8) ,                          -- ��� ��⥣�ਨ
        CATID  NUMBER(8) REFERENCES OBJCAT(OWNCAT), -- ��� த�⥫�᪮� ��⥣�ਨ
        ORD    NUMBER(8) ,                          -- ���浪��� �����
        DESCR  VARCHAR2(200) ,                      -- ���ᠭ��
        CONSTRAINT PK_OCAT PRIMARY KEY (OWNCAT)
        );

CREATE TABLE OBJS                          -- ��ꥪ��
       (OID    NUMBER(8) ,                 -- ��� ��ꥪ�
        OTYPE  NUMBER(4) ,                 -- ��� �����
        CATID  NUMBER(8) NOT NULL,         -- ��� ��⥣�ਨ
        ORD    NUMBER(8) ,                 -- ���浪��� �����
        NAME   VARCHAR2(20) ,              -- ���⪮� ���
        DESCR  VARCHAR2(200) ,             -- ���ᠭ��
        DUP    NUMBER(1,0),                -- ��⠭���� �㡫�஢���� ��� ���㬥�⮢
        AUTON  NUMBER(1,0),                -- ��⮭㬥��� ���㬥�⮢
        POPTS  NUMBER(*,0),                -- ��⠭���� ���� ���㬥�⮢ � ��
        RANGE  NUMBER(1,0),                -- ��ਮ� �㬥�樨 ��� ���㬥��. 0 - ��᪮����, 1 - ��� , 2-�����
        PARENT NUMBER(8) ,                 -- ��� "����⥫�᪮��" ��ꥪ�
        HIST   NUMBER(1),                  -- �࠭��� ����� (DB)
        CATS   NUMBER(8),                  -- where to get cat id's
        CONSTRAINT PK_OBJS PRIMARY KEY (OID),
        CONSTRAINT FK_OCAT FOREIGN KEY (CATID) REFERENCES OBJCAT(OWNCAT),
        CONSTRAINT FK_PAROBJ FOREIGN KEY (PARENT) REFERENCES OBJS(OID),
        CONSTRAINT FK_CATSOBJ FOREIGN KEY (CATS) REFERENCES OBJS(OID),
        CONSTRAINT UN_OBJS UNIQUE (NAME),
        CONSTRAINT UN_ORD  UNIQUE (CATID,ORD)
        ) ;

create table CATTREE  -- ������ ��� �।�⠢����� ��ॢ� ��⥣�਩ ��쥪⮢
       (
        LEV    NUMBER(8) ,                 -- ����� �஢��
        TORD   NUMBER(8) ,                 -- ���浪��� ����� (᪢�����)
        LAST   NUMBER(2) ,                 -- \  �㦥��� ���� ��� �ᮢ����
        LEVM   NUMBER(8) ,                 -- /  ࠬ�� �� ������ ��ॢ�
        OWNCAT NUMBER(8) ,                 -- �� ��⥣�ਨ
        CATID  NUMBER(8) ,                 -- �� த�⥫�᪮� ��⥣�ਨ
        ORD    NUMBER(8) ,                 -- ���浪��� ����� (����� ᢮��� த�⥫�)
        DESCR  VARCHAR2(200)               -- ���ᠭ��
       ) ;

CREATE TABLE consts       -- ������ ����⠭�
       (
        oid          NUMBER(*,0),      -- �� ��ꥪ�
        const_type   NUMBER(*,0),      -- ��� ����⠭��
        value        VARCHAR2(250),    -- ���祭��
        CONSTRAINT   const_oid FOREIGN KEY (oid) REFERENCES OBJS (oid) ON DELETE CASCADE
       ) ;

CREATE TABLE DBSTR                        -- �������� ��� ������ � �����⨪
       (
        DBID             NUMBER(8),       -- �� ��ꥪ� ���� ������
        RealName         VARCHAR2(30),    -- �������� ����
        vtype            NUMBER(3,0),     -- ��� ����
        rdbid            NUMBER(8),       -- �� ���� ��� ���� ⨯� ����
        maxlen           NUMBER(3,0),     -- ���ᨬ��쭠� ����� ����
        deleted          NUMBER(3,0) DEFAULT 0,  -- �ਧ��� 㤠�������
        Alias            VARCHAR2(40),    -- ��� ��� ���짮��⥫�
        disp_opt         NUMBER(8),       -- ���짮��⥫�᪨� ��⠭����
        width            NUMBER(3,0),     -- ��ਭ�
        dec              NUMBER(3,0),     -- �����뢠��� ���. ����
        ord              NUMBER(4),       -- ���浪��� ����� �� ������
        def_opt          NUMBER(8),       -- �����. ��⠭����
        DB_SetName       VARCHAR2(20),    -- ���op ��⠭���� ��� �易���� ����
        lines            NUMBER(8),       
        CONSTRAINT PK_DBSTR PRIMARY KEY (DBID,RealName),
        CONSTRAINT DBSTR_id FOREIGN KEY (DBID) REFERENCES objs (oid) ON DELETE CASCADE
       );

CREATE TABLE accs                             -- ����稩 ���� ��⮢
       (
        code       NUMBER(*,0),               -- ��� ���
        name       VARCHAR(20) NOT NULL,      -- �������� 'A/01/001'
        parent     NUMBER(*,0),               -- ��� த�⥫�
        descr      VARCHAR2(250),             -- ���ᠭ�� '�᭮��� ��-��'
        bal        NUMBER(*,0),               -- �ਧ��� �����ᮢ���
        node       NUMBER(*,0),               -- ������⢮ �뭮��� (0 - ����)
        rev_code   NUMBER(*,0),               -- ��� ��� ��� ��८業��
        val_code   VARCHAR2(30),              -- ��� ������
        opts       NUMBER(*,0),               -- ��⠭����
        expanded   NUMBER(1),                 -- �ਧ��� ࠧ�������
        anal_id    NUMBER(8),                 -- �� �����⨪�
        bal_fld    VARCHAR2(30),              -- �����ᮢ�� ����
        setname    VARCHAR2(20),              -- ��� ����� ��⠭����
        CONSTRAINT accs_pk PRIMARY KEY (code),
        CONSTRAINT accs_name UNIQUE (name),
        CONSTRAINT accs_parent FOREIGN KEY (parent) REFERENCES accs (code),
        CONSTRAINT accs_rev FOREIGN KEY (rev_code) REFERENCES accs (code),
        CONSTRAINT anal_id_oid FOREIGN KEY (anal_id) REFERENCES objs (oid),
        CONSTRAINT bal_fld_dbstr FOREIGN KEY (anal_id,bal_fld) REFERENCES dbstr (dbid,RealName)
       ) ;

-- ����� ���� ��⮢
CREATE TABLE ex_accs AS SELECT * FROM accs ;

CREATE TABLE cps                         -- ���� ����஫��� �祪
       (
        point_date    DATE,              -- ���
        is_start NUMBER(*,0) NOT NULL,   -- �� ᪮�쪮 ����楢 ���। ����� ������
        CONSTRAINT cps_pk PRIMARY KEY (point_date)
       ) ;

CREATE TABLE cints                       -- ����஫�� ���ࢠ��
       (
        beg_date   DATE,                 -- ��� ��砫�
        end_date   DATE,                 -- ��� ����
        CONSTRAINT cints_prim PRIMARY KEY (beg_date,end_date),
        CONSTRAINT cints_beg FOREIGN KEY (beg_date) REFERENCES cps (point_date),
        CONSTRAINT cints_end FOREIGN KEY (end_date) REFERENCES cps (point_date)
       ) ;

CREATE TABLE user_options                -- ��⠭���� ���짮��⥫��
       (
        username   VARCHAR2(30),         -- ���짮��⥫�
        full_name  VARCHAR(80),          -- ������ ���
        beg_date   DATE,                 -- ��砫� ��ਮ�� ��������
        end_date   DATE,                 -- ����� ��ਮ�� ��������
        sum_len    NUMBER(*,0),          -- ����� �㬬�
        sum_dig    NUMBER(*,0),          -- ����� ��᫥ �����筮� �窨
        apps       NUMBER(1,0),          -- ��������
        inp_date   DATE,                 -- ��� �����
        cur_dates  NUMBER(1,0),          -- ����騥 ����
        dig_input  NUMBER(1,0),          -- ���� �� ��ࠬ
        sort_num   NUMBER(1,0),          -- ����஢�� �� ����ࠬ ���-� � ����
        dead_date  DATE,                 -- ��� ����⨢��� ���������
        readonly   NUMBER(1,0),          -- ���쪮 ��ᬮ��
        overwrite  NUMBER(1,0),          -- ����� "���������"
        imp_opt    NUMBER(8),            -- ��⠭���� ������
        set_to_end NUMBER(1,0),          -- �⠭������� � ����� ᯨ᪠
        CONSTRAINT per_prim PRIMARY KEY (username),
        CONSTRAINT per_cints FOREIGN KEY (beg_date,end_date) REFERENCES cints (beg_date,end_date)
       ) ;

CREATE TABLE sums                        -- ������᪨� �㬬�
       (
        sum_id NUMBER(*,0),              -- ��� �㬬�
        value  NUMBER(24,6),             -- ���祭��
        CONSTRAINT sums_pk PRIMARY KEY (sum_id)
       ) ;

CREATE TABLE rems                        -- ���⪨ �� ��⠬
       (
        code       NUMBER(*,0),          -- ��� ���
        rem_date   DATE,                 -- ��� ���⪠
        end_val_id NUMBER(*,0),          -- ��� �㬬� ���⪠
        CONSTRAINT rems_pk PRIMARY KEY (code,rem_date),
        CONSTRAINT rems_cpid FOREIGN KEY (rem_date) REFERENCES cps (point_date),
        CONSTRAINT rems_code FOREIGN KEY (code) REFERENCES accs(code)
       ) ;
CREATE OR REPLACE TRIGGER Rem_Trg  -- �ਣ��� 㤠����� �㬬� �� 㤠�����
AFTER DELETE ON rems               -- ����� �� ���� ���⪮�
FOR EACH ROW
BEGIN
 DELETE from sums where sum_id=:old.end_val_id ;
END;
/


CREATE TABLE turns                 -- ������ �� ��⠬
       (
        beg_date   DATE,           -- ��砫� ����஫쭮�� ���ࢠ��
        end_date   DATE,           -- ����� ����஫쭮�� ���ࢠ��
        code       NUMBER(*,0),    -- ��� ���
        d_turn_id   NUMBER(*,0),   -- ��� �㬬� ����� �� ������
        c_turn_id   NUMBER(*,0),   -- ��� �㬬� ����� �� �।���
        CONSTRAINT turns_prim PRIMARY KEY (beg_date,end_date,code),
        CONSTRAINT turns_dates FOREIGN KEY (beg_date,end_date) REFERENCES cints (beg_date,end_date),
        CONSTRAINT turns_code FOREIGN KEY (code) REFERENCES accs(code)
       ) ;
CREATE OR REPLACE TRIGGER Turn_Trg  -- �ਣ��� 㤠����� �㬬�
AFTER DELETE ON turns
FOR EACH ROW
BEGIN
 DELETE from sums where sum_id=:old.d_turn_id OR sum_id=:old.c_turn_id;
END;
/


CREATE TABLE cor_turns              -- ����ᯮ������騥 ������
       (
        beg_date   DATE,            -- ��砫� ����஫쭮�� ���ࢠ��
        end_date   DATE,            -- ����� ����஫쭮�� ���ࢠ��
        d_code     NUMBER(*,0),     -- ��� �� ������
        c_code     NUMBER(*,0),     -- ��� �� �।���
        turn_id    NUMBER(*,0),     -- ��� �㬬� �����
        CONSTRAINT cor_turns_prim PRIMARY KEY (beg_date,end_date,d_code,c_code),
        CONSTRAINT cor_turns_dates FOREIGN KEY (beg_date,end_date) REFERENCES cints (beg_date,end_date),
        CONSTRAINT cor_turns_d_code FOREIGN KEY (d_code) REFERENCES accs (code),
        CONSTRAINT cor_turns_c_code FOREIGN KEY (c_code) REFERENCES accs (code)
       ) ;
CREATE OR REPLACE TRIGGER CorTurn_Trg   -- �ਣ��� 㤠����� �㬬�
AFTER DELETE ON cor_turns
FOR EACH ROW
BEGIN
 DELETE from sums where sum_id=:old.turn_id ;
END;
/


CREATE TABLE conds                    -- �஢���� �⠭������ ����権
       (
        cond_id      NUMBER(*,0),     -- ��� �஢����
        cond_subid   NUMBER(*,0),     -- ��-���
        ord          NUMBER(8),       -- ���浪��� ����� �� �����
        text         VARCHAR2(250),   -- ����� �஢����
        doc_type     VARCHAR2(20),    -- ��� ���㬥��
        storno       NUMBER(1),       -- �ਧ��� "��୮"
        native_doc   NUMBER(1),       -- �ਧ��� "��࠭��� ⮫쪮 � த�� ⨯�� ���㬥��"
        auto_input   NUMBER(1),       -- �ਧ��� "��� ����"
        auto_gen     NUMBER(1),       -- �ਧ��� "��� ���㬥��"
        auto_apply   NUMBER(1),       -- �ਧ��� "�஢����� ��⮬���᪨"
        for_post     NUMBER(1),       -- �।������ �⪫��뢠��
        pre_formula  VARCHAR2(250),   -- �p�㫠, �ᯮ��塞�� ��p�� ������ �p������
        CONSTRAINT conds_prim PRIMARY KEY (cond_id,cond_subid),
        CONSTRAINT conds_ord UNIQUE (cond_id,ord),
        CONSTRAINT conds_id FOREIGN KEY (cond_id) REFERENCES objs (oid) ON DELETE CASCADE,
        CONSTRAINT conds_doc FOREIGN KEY (doc_type) REFERENCES objs (name)
       ) ;

CREATE TABLE cond_items           -- �������� �⠭������ �஢����
       (
        cond_id      NUMBER(*,0),   -- ��� �஢����
        cond_subid   NUMBER(*,0),   -- ��-���
        code         NUMBER(*,0),   -- ��� ���
        setname      VARCHAR2(20),  -- ��� ����� ��⠭����
        side         VARCHAR2(1),   -- ��஭� ('D' ��� 'K')
        for_post     NUMBER(1,0),   -- �ਧ��� �⪫��뢠���
        item_num     NUMBER(*,0),   -- ���浪��� ����� � �஢����
        item_formula VARCHAR2(250), -- ���㫠
        CONSTRAINT cond_items_prim PRIMARY KEY (cond_id,cond_subid,code),
        CONSTRAINT cond_items_uni_num UNIQUE (cond_id,cond_subid,item_num),
        CONSTRAINT cond_items_cond_id FOREIGN KEY (cond_id,cond_subid) REFERENCES conds (cond_id,cond_subid) ON DELETE CASCADE,
        CONSTRAINT cond_items_code FOREIGN KEY (code) REFERENCES accs (code)
       ) ;

CREATE TABLE opers   -- ����樨
       (
        op_id      NUMBER(*,0),   -- ��� ����樨
        op_subid   NUMBER(*,0),   -- ��-���
        status     NUMBER(1,0),   -- 0-��������  1-�஢�������, 2-㤠�塞�� ,3-�����塞��
        post       NUMBER(1,0),   -- �ਧ��� �⫮�������
        storno     NUMBER(1,0),   -- �ਧ��� ��୨஢����
        text       VARCHAR2(250), -- ����� ����樨
        doc_type   VARCHAR2(20),  -- ��� ���㬥��
        doc_num    VARCHAR2(30),  -- ����� ���㬥��
        doc_num_v  NUMBER(*,0),   -- ����� ����� ���㬥��
        op_date    DATE,          -- ��� ����樨
        cond_id    NUMBER(*,0),   -- ��� �⠭���⭮� ����樨
        edited     NUMBER,        -- �ਧ��� ।����㥬��� � ����� ������
        CONSTRAINT opers_prim PRIMARY KEY (op_id,op_subid),
        CONSTRAINT opers_doc FOREIGN KEY (doc_type) REFERENCES objs (name),
        CONSTRAINT opers_doc_num UNIQUE (status,doc_type,doc_num,doc_num_v),
        CONSTRAINT opers_cond FOREIGN KEY (cond_id,op_subid) REFERENCES conds (cond_id,cond_subid)
       ) ;
CREATE UNIQUE INDEX date_ind ON opers(op_date,op_id,op_subid) ;

CREATE TABLE op_items                         -- �������� ����樨
       (
        op_id         NUMBER(*,0),            -- ��� ����樨
        op_subid      NUMBER(*,0),            -- ��-��� ����樨
        op_date       DATE,                   -- ���
        code          NUMBER(*,0),            -- ��� ���
        side          VARCHAR2(1),            -- ��஭� (D ��� K)
        item_sum_id   NUMBER(*,0),            -- ��� �㬬�
        node          NUMBER(1,0) DEFAULT 0,  -- 0 - ���� , 1 - 㧥� , 2 - 㧥� � ᮡ�⢥���� �㬬��
        for_post      NUMBER(1,0),            -- ��� �⪫��뢠���
        setname       VARCHAR2(20),           -- ��� ����� ��⠭����
        CONSTRAINT op_items_prim PRIMARY KEY (op_id,op_subid,code),
        CONSTRAINT op_items_op_id FOREIGN KEY (op_id,op_subid) REFERENCES opers (op_id,op_subid),
        CONSTRAINT op_items_code FOREIGN KEY (code) REFERENCES accs (code)
       ) ;
CREATE UNIQUE INDEX items_code_ind ON op_items(code,op_date,op_id,op_subid) ;
create index items_sumid on op_items(item_sum_id) ;

CREATE OR REPLACE TRIGGER OpItems_Trg
AFTER DELETE ON op_items
FOR EACH ROW
BEGIN
 DELETE from sums where sum_id=:old.item_sum_id AND :old.node!=1 ;
END;
/

CREATE TABLE del_opers   -- �������� ����樨
       (
        op_id      NUMBER(*,0),   -- ��� ����樨
        op_subid   NUMBER(*,0),   -- ��-���
        status     NUMBER(1,0),   -- �� �ᯮ������
        post       NUMBER(1,0),   -- �ਧ��� �⪫��������
        storno     NUMBER(1,0),   -- �ਧ��� ��୨஢����
        text       VARCHAR2(250), -- ����� ����樨
        doc_type   VARCHAR2(20),  -- ��� ���㬥��
        doc_num    VARCHAR2(50),  -- ����� ���㬥��
        doc_num_v  NUMBER(*,0),  --  ����� ����� ���㬥��
        op_date    DATE,          -- ��� ����樨
        cond_id    NUMBER(*,0),   -- ��� �⠭���⭮� ����樨
        edited     NUMBER,
        CONSTRAINT del_opers_prim PRIMARY KEY (op_id,op_subid)
       ) ;

CREATE TABLE del_op_items                     -- �������� 㤠������ ����権
       (
        op_id         NUMBER(*,0),            -- ��� ����樨
        op_subid      NUMBER(*,0),            -- ��-��� ����樨
        op_date       DATE,                   -- ���
        code          NUMBER(*,0),            -- ��� ���
        side          VARCHAR2(1),            -- ��஭� (D ��� K)
        item_sum_id   NUMBER(*,0),            -- ��� �㬬�
        node          NUMBER(1,0) DEFAULT 0,  -- 0 - ���� , 1 - 㧥� , 2 - 㧥� � ᮡ�⢥���� �㬬��
        for_post      NUMBER(1,0),            -- ��� �⪫��뢠���
        CONSTRAINT del_op_items_prim PRIMARY KEY (op_id,op_subid,code),
        CONSTRAINT del_op_items_op_id FOREIGN KEY (op_id,op_subid) REFERENCES del_opers (op_id,op_subid)
       ) ;

CREATE TABLE cor_items                 -- ����ᯮ������騥 �������� ����権
       (
        op_id          NUMBER(*,0),    -- ��� ����樨
        op_subid       NUMBER(*,0),    -- ��-��� ����樨
        d_code         NUMBER(*,0),    -- ��� ��� �� ������
        c_code         NUMBER(*,0),    -- ��� ��� �� �।���
        item_sum_id    NUMBER(*,0),    -- ��� �㬬�
        CONSTRAINT cor_items_prim PRIMARY KEY (op_id,op_subid,d_code,c_code),
        CONSTRAINT cor_items_op_id FOREIGN KEY (op_id,op_subid) REFERENCES opers (op_id,op_subid),
        CONSTRAINT cor_items_d_code FOREIGN KEY (d_code) REFERENCES accs (code),
        CONSTRAINT cor_items_c_code FOREIGN KEY (c_code) REFERENCES accs (code)
       ) ;
CREATE OR REPLACE TRIGGER CorItems_Trg
AFTER DELETE ON cor_items
FOR EACH ROW
BEGIN
 DELETE from sums where sum_id=:old.item_sum_id ;
END;
/

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


CREATE TABLE history                   -- ����� ᮧ�����,��������� � 㤠����� ����権
       (
        op_id    NUMBER,               -- ��� ����樨
        op_subid NUMBER,               -- ��-��� ����樨
        revision NUMBER,               -- ����� ���������
        rev_time DATE,                 -- ��� ���������
        username VARCHAR2(30),         -- ���짮��⥫�
        CONSTRAINT hist_prim PRIMARY KEY (op_id,op_subid,revision),
        CONSTRAINT hist_user FOREIGN KEY (username) REFERENCES user_options (username)
       ) ;

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


CREATE TABLE DBOPT (                   -- ������ ��⠭����
       DBID             NUMBER(8),     -- �� ��ꥪ�
       SETNAME          VARCHAR2(20),  -- ��� �����
       CONSTRAINT PK_DBOPT PRIMARY KEY (DBID,SETNAME),
       CONSTRAINT DBOPT_id FOREIGN KEY (DBID) REFERENCES objs (oid) ON DELETE CASCADE
);

CREATE TABLE DBOPTOV (                 -- ��⠭���� ��� ࠧ��� ����஢
       DBID             NUMBER(8),     -- �� ��ꥪ�
       SETNAME          VARCHAR2(20),  -- ��� �����
       RealName         VARCHAR2(30),  -- ��� ����
       OPT              NUMBER(8),     -- ��⠭���� (���������⨢��)
       DB_setname       VARCHAR2(20),
       CONSTRAINT PK_DBOPTOV PRIMARY KEY (DBID,SETNAME,RealName),
       CONSTRAINT DBOPTS_FK FOREIGN KEY (DBID,SETNAME) REFERENCES DBOPT (DBID,SETNAME) ON DELETE CASCADE
);


CREATE TABLE dbaddinfo
       (
        dbid        NUMBER(8),     -- �� ��ꥪ�
        setname     VARCHAR2(20),  -- ��� �����
        RealName    VARCHAR2(30),  -- ��� ����
        name        VARCHAR2(30),  -- ���⥬��� ���
        code        NUMBER(*,0),   -- ��� ���
        turntype    VARCHAR2(3) NOT NULL,   -- ��� �㬬� (R,BR,DT,CT)
        alias       VARCHAR2(30),  -- ���짮��⥫�᪮� ���
        CONSTRAINT  PK_DBADD PRIMARY KEY (DBID,setname,RealName,name),
        CONSTRAINT  DBADD_id_SET FOREIGN KEY (DBID,SetName) REFERENCES dbopt (dbid,SetName) ON DELETE CASCADE,
        CONSTRAINT  DBADD_id_NAME FOREIGN KEY (DBID,RealName) REFERENCES dbstr (dbid,RealName) ON DELETE CASCADE,
        CONSTRAINT  dbadd_code FOREIGN KEY (code) REFERENCES accs (code),
        CONSTRAINT  check_type CHECK (turntype IN ('BR','DT','CT','R'))
       );

CREATE TABLE DBCAT                  -- ��⥣�ਨ ��� ��� ������
       (DBID   NUMBER(8) ,             -- �� ��ꥪ�
        OWNCAT NUMBER(8) ,             -- ��� ��⥣�ਨ
        CATID  NUMBER(8) ,             -- ��� த�⥫�᪮� ��⥣�ਨ
        ORD    NUMBER(8) ,             -- ���浪��� ����� �� ������
        DESCR  VARCHAR2(200) ,         -- ���ᠭ��
        CONSTRAINT PK_DBCAT PRIMARY KEY (DBID,OWNCAT),
        CONSTRAINT PK_DBOWNCAT UNIQUE (OWNCAT),
        CONSTRAINT DBCAT_FK FOREIGN KEY (DBID,CATID) REFERENCES DBCAT(DBID,OWNCAT),
        CONSTRAINT DBCAT_id FOREIGN KEY (DBID) REFERENCES objs (oid) ON DELETE CASCADE
        );

create table DBCATTREE     -- ⠡��� ��� ������ ��ॢ� ��⥣�਩ ��� ������
       (
        LEV    NUMBER(8) ,                 -- ����� �஢��
        TORD   NUMBER(8) ,                 -- ���浪��� ����� (᪢�����)
        LAST   NUMBER(2) ,                 -- \  �㦥��� ���� ��� �ᮢ����
        LEVM   NUMBER(8) ,                 -- /  ࠬ�� �� ������ ��ॢ�
        DBID   NUMBER(8) ,                 -- �� ���� ������
        OWNCAT NUMBER(8) ,                 -- �� ��⥣�ਨ
        CATID  NUMBER(8) ,                 -- �� த�⥫�᪮� ��⥣�ਨ
        ORD    NUMBER(8) ,                 -- ���浪��� ����� (����� ᢮��� த�⥫�)
        DESCR  VARCHAR2(200),              -- ���ᠭ��
        CONSTRAINT U_DBCATTREE UNIQUE (OWNCAT)
       );

CREATE TABLE OBJPRIV                 -- �ࠢ� ����㯠 � ��ꥪ⠬
       (OID       NUMBER(8) NOT NULL,  -- �� ��ꥪ�
        RUNAME    VARCHAR2(30) ,       -- ��� ஫� ��� ���짮��⥫�
        RU        number(1) ,          -- �ਧ��� ���짮��⥫�
        PR0       NUMBER(1) ,          -- �ࠢ� �� ��ᬮ��
        PR1       NUMBER(1) ,          -- �ࠢ� �� ���������
        PR2       NUMBER(1) ,          -- �ࠢ� �� ᮧ�����
        PR3       NUMBER(1) ,          -- �ࠢ� �� 㤠�����
        PR4       NUMBER(1) ,          -- ��१�ࢨ஢���
        PR5       NUMBER(1) ,          -- ��१�ࢨ஢���
        CONSTRAINT PK_OPRIV PRIMARY KEY (OID,RU,RUNAME)
        );

CREATE TABLE PRIVTEST                  -- �ࠢ� ���짮��⥫��
       (
        POID      NUMBER(8) NOT NULL,         -- �� ��ꥪ�
        UNAME     VARCHAR2(30) ,              -- ��� ���짮��⥫�
        PR0       NUMBER(1) ,                 -- �ࠢ� �� ��ᬮ��
        PR1       NUMBER(1) ,                 -- �ࠢ� �� ���������
        PR2       NUMBER(1) ,                 -- �ࠢ� �� ᮧ�����
        PR3       NUMBER(1) ,                 -- �ࠢ� �� 㤠�����
        PR4       NUMBER(1) ,                 -- ��१�ࢨ஢���
        PR5       NUMBER(1) ,                 -- ��१�ࢨ஢���
        CONSTRAINT PK_PRIVTEST PRIMARY KEY (POID,UNAME)
       );

CREATE TABLE anal_errs                        -- ����⥫�� ���⪨
       (
        SID      NUMBER,                      -- �� ��ᨨ
        code     NUMBER,                      -- ��� ���
        AARTICLE VARCHAR2(30),                -- ��⨪�
        AVER     NUMBER(8),                   -- �����
        undo     NUMBER(1)                    -- �஢������� ��� �⪠�뢠����
       ) ;

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
COMMENT ON TABLE UDBOPT IS '���������������� ��������� ��� ���� ������ ��� ������� ������������ � ��� ������� ������ ���������'
/
COMMENT ON COLUMN UDBOPT.SETNAME IS '����� ��������� ��� ������� ������ = " " (������) �.�. ������ � �������� ���� � �� ����� ���� NULL, ������� �� ���� ������ �� DBOPT'
/


/*==============================================================*/
/* Table : TMP_SUMS                                             */
/*==============================================================*/
create table TMP_SUMS (
   SUM_ID               NUMBER                          not null,
   CREATE_DATE          DATE                            not null,
   CREATOR              VARCHAR2(30)                    not null,
   DESCR                VARCHAR2(255),
   constraint PK_TMP_SUMS primary key (SUM_ID),
   constraint FK_TMP_SUMS_2_SUMS foreign key (SUM_ID)
         references SUMS (SUM_ID)
         on delete cascade
)
/

comment on table TMP_SUMS is '������ �࠭�� ���� �६����� �㬬' ;
comment on column TMP_SUMS.SUM_ID is '��� �६����� �㬬�';
comment on column TMP_SUMS.CREATE_DATE is '��� ᮧ����� �६����� �㬬�';
comment on column TMP_SUMS.CREATOR is '���짮��⥫�, ᮧ���訩 �६����� �㬬�';
comment on column TMP_SUMS.DESCR is '���ᠭ�� �६����� �㬬� (����� ���� NULL)';

create table message_table
(
 Constraint_Name varchar2(30) not null,
 Actions         varchar2(3)  not null,
 Parent_Child    varchar2(1)  not null,
 Error_Message   varchar2(2000)
);
insert into message_table values('OPERS_DOC','IU','C','��� ⠪��� ���㬥��') ;


create table AN_FLD_ORDER (
   ANID                 NUMBER(8)                       not null,
   SETNAME              VARCHAR2(20)                    not null,
   RealName             VARCHAR2(30)                    not null,
   name                 VARCHAR2(30)                    not null,
   DB_ID                NUMBER (8),
   FLD_TYPE             VARCHAR2(3)                     not null,
   FLD_ORD              NUMBER(4)                       not null,
   constraint PK_AN_FLD_ORDER primary key (ANID, SETNAME, RealName, name),
   constraint FK_AN_FLD_ORDER_2_OBJS foreign key (ANID)
         references OBJS (OID)
)
/


comment on table AN_FLD_ORDER is '��� ��।������ ���浪� ����� ��
।���஢���� �������᪮� �㬬�'
/

comment on column AN_FLD_ORDER.ANID is '��� �����⨪� ������ ।����㥬'
/

comment on column AN_FLD_ORDER.SETNAME is '��� ����� ��⠭����'
/

comment on column AN_FLD_ORDER.RealName is '��� ���뢠���� ���� � �����⨪� ��� � ���� ������'
/

comment on column AN_FLD_ORDER.name is '����� ���� � SQL ����� ( ��� ����� �� � �� ���⮩?)'
/

comment on column AN_FLD_ORDER.DB_ID is '��� ���� ���� ������ �����뢠�� �� ����� ��� ��, ��� ���� �����⨪� =ANID, ��� ��� ����� NULL'
/

comment on column AN_FLD_ORDER.FLD_TYPE is '��� ����
             DB - ���� ������
             AN - �����⨪�
            ANR - �����⨪� ���⪠
            ADD - �������⥫쭮� ����'
/

comment on column AN_FLD_ORDER.FLD_ORD is '���冷� ���� �� ��ᬮ�� �����⨪�'
/

/*==============================================================*/
/* Index: IND_AN_FLD_ORD                                        */
/*==============================================================*/
create  index IND_AN_FLD_ORD on AN_FLD_ORDER (
   ANID ASC,
   SETNAME ASC,
   FLD_ORD ASC
)
/

/*==============================================================*/
/* Index: IND_AN_FLD_TYPE                                       */
/*==============================================================*/
create  index IND_AN_FLD_TYPE on AN_FLD_ORDER (
   ANID ASC,
   SETNAME ASC,
   FLD_TYPE ASC
)
/
create table EXCEL_IMP (
   DB_ID                NUMBER(8)                       not null,
   SHORT_NAME           VARCHAR2(20)                    not null,
   DESCR                VARCHAR2(255),
   constraint PK_EXCEL_IMP primary key (DB_ID, SHORT_NAME),
   constraint FK_EXCEL_IM_EXCEL_IMP_OBJS foreign key (DB_ID)
         references OBJS (OID)
)
/
comment on table EXCEL_IMP is '�࠭�� ��᫥����⥫쭮��� ����� ��� ������ � �� �� Excel'
/
comment on column EXCEL_IMP.DB_ID is '��� �� � ������ �㤥� ������஢��� �����'
/
comment on column EXCEL_IMP.SHORT_NAME is '���⪮� �������� ������'
/
comment on column EXCEL_IMP.DESCR is '���஡��� ���ᠭ�� '
/
/*==============================================================*/
/* Table : EXCEL_IMP_ITEMS                                      */
/*==============================================================*/
create table EXCEL_IMP_ITEMS (
   DB_ID                NUMBER(8)                       not null,
   SHORT_NAME           VARCHAR2(20)                    not null,
   ORD                  NUMBER(4)                       not null,
   REALNAME             VARCHAR2(30)                    not null,
   constraint PK_EXCEL_IMP_ITEMS primary key (DB_ID, SHORT_NAME, ORD),
   constraint FK_EXCEL_IM_REFERENCE_EXCEL_IM foreign key (DB_ID, SHORT_NAME)
         references EXCEL_IMP (DB_ID, SHORT_NAME)
)
/
comment on table EXCEL_IMP_ITEMS is '�࠭�� ���冷� �����'
/
comment on column EXCEL_IMP_ITEMS.ORD is '���冷� ����'
/
comment on column EXCEL_IMP_ITEMS.REALNAME is '�������� ����'
/
create table ADD_OPITEMS as
(select O.OP_ID, O.OP_SUBID, OI.SETNAME, OI.ITEM_SUM_ID, O.DOC_NUM,
    O.DOC_NUM_V, O.DOC_TYPE, O.TEXT, O.OP_DATE, O.STORNO, OI.SIDE
from  OPERS O, OP_ITEMS OI where ROWNUM < 1);


exit
