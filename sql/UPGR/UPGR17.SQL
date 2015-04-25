DROP TABLE unapp_items ;
DROP TABLE unapp_cor_items ;
DROP TABLE unapp_opers ;

CREATE TABLE unapp_opers               -- Операции, подлежащие проведению
       (
        op_id    NUMBER,               -- Код операции
        op_subid NUMBER,               -- Суб-код операции
        undo     NUMBER(1) NOT NULL,   -- 0-прибавить, 1-отнять
        SID      NUMBER NOT NULL,      -- Уникальный идентификатор сессии
        CONSTRAINT unapp_opers_prim PRIMARY KEY (op_id,op_subid),
        CONSTRAINT unapp_opers_opers FOREIGN KEY (op_id,op_subid) REFERENCES opers (op_id,op_subid)
       ) ;


CREATE TABLE unapp_items               -- Элементы операций, подлежащие проведению
       (
        op_id    NUMBER,               -- Код операции
        op_subid NUMBER,               -- Суб-код операции
        code     NUMBER(*,0),          -- Код счета
        cp_date  DATE,                 -- Дата КТ, к которой нужно прибавить
        CONSTRAINT unapp_items_prim PRIMARY KEY (op_id,op_subid,code,cp_date),
        CONSTRAINT unapp_items_opers FOREIGN KEY (op_id,op_subid) REFERENCES unapp_opers (op_id,op_subid),
        CONSTRAINT unapp_items_items FOREIGN KEY (op_id,op_subid,code) REFERENCES op_items (op_id,op_subid,code),
        CONSTRAINT unapp_cpdate FOREIGN KEY (cp_date) REFERENCES cps (point_date)
       ) ;

CREATE TABLE unapp_cor_items           -- Корреспондирующие элементы операций,
                                       -- подлежащие проведению
       (
        op_id    NUMBER,               -- Код операции
        op_subid NUMBER,               -- Суб-код операции
        d_code   NUMBER(*,0),          -- Код счета по дебету
        c_code   NUMBER(*,0),          -- Код счета по кредиту
        cp_date  DATE,                 -- Дата КТ, к которой нужно прибавить
        CONSTRAINT unapp_cor_prim PRIMARY KEY (op_id,op_subid,d_code,c_code,cp_date),
        CONSTRAINT unapp_cor_opers FOREIGN KEY (op_id,op_subid) REFERENCES unapp_opers (op_id,op_subid),
        CONSTRAINT unapp_cor_items FOREIGN KEY (op_id,op_subid,d_code,c_code) REFERENCES cor_items (op_id,op_subid,d_code,c_code),
        CONSTRAINT unapp_cor_cpdate FOREIGN KEY (cp_date) REFERENCES cps (point_date)
       ) ;
exit
