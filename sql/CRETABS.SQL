/* Создание основных таблиц (кроме баз данных и аналитик) */

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

CREATE SEQUENCE SumSeq        -- Последовательность ключей для таблицы sums
INCREMENT BY 1
START WITH 1
CACHE 100 ;

CREATE SEQUENCE OperSeq   -- Последовательность ключей для таблицы операций
INCREMENT BY 1
START WITH 1
CACHE 20 ;

CREATE SEQUENCE ObjSeq   -- Последовательность ключей для таблиц счетов
INCREMENT BY 1           -- и объектов
START WITH 1
NOCACHE ;

CREATE SEQUENCE TreeSeq  -- Последовательность ключей для дерева объектов
INCREMENT BY 1
START WITH 1
CACHE 100 ;

CREATE SEQUENCE DBCatSeq -- Последовательность ключей для категорий БД
INCREMENT BY 1
START WITH 1
CACHE 10 ;

CREATE TABLE OBJCAT                                 -- Иерархия объектов
       (OWNCAT NUMBER(8) ,                          -- Код категории
        CATID  NUMBER(8) REFERENCES OBJCAT(OWNCAT), -- Код родительской категории
        ORD    NUMBER(8) ,                          -- Порядковый номер
        DESCR  VARCHAR2(200) ,                      -- Описание
        CONSTRAINT PK_OCAT PRIMARY KEY (OWNCAT)
        );

CREATE TABLE OBJS                          -- Объекты
       (OID    NUMBER(8) ,                 -- Код объекта
        OTYPE  NUMBER(4) ,                 -- Тип обекта
        CATID  NUMBER(8) NOT NULL,         -- Код категории
        ORD    NUMBER(8) ,                 -- порядковый номер
        NAME   VARCHAR2(20) ,              -- Короткое имя
        DESCR  VARCHAR2(200) ,             -- Описание
        DUP    NUMBER(1,0),                -- Установки дублирования для документов
        AUTON  NUMBER(1,0),                -- Автонумерация документов
        POPTS  NUMBER(*,0),                -- Установки печати документов и форм
        RANGE  NUMBER(1,0),                -- Период нумерации для документа. 0 - бесконечный, 1 - год , 2-месяц
        PARENT NUMBER(8) ,                 -- Код "Родительского" объекта
        HIST   NUMBER(1),                  -- Хранить историю (DB)
        CATS   NUMBER(8),                  -- where to get cat id's
        CONSTRAINT PK_OBJS PRIMARY KEY (OID),
        CONSTRAINT FK_OCAT FOREIGN KEY (CATID) REFERENCES OBJCAT(OWNCAT),
        CONSTRAINT FK_PAROBJ FOREIGN KEY (PARENT) REFERENCES OBJS(OID),
        CONSTRAINT FK_CATSOBJ FOREIGN KEY (CATS) REFERENCES OBJS(OID),
        CONSTRAINT UN_OBJS UNIQUE (NAME),
        CONSTRAINT UN_ORD  UNIQUE (CATID,ORD)
        ) ;

create table CATTREE  -- Таблица для представления дерева категорий обьектов
       (
        LEV    NUMBER(8) ,                 -- Номер уровня
        TORD   NUMBER(8) ,                 -- Порядковый номер (сквозной)
        LAST   NUMBER(2) ,                 -- \  служебные поля для рисования
        LEVM   NUMBER(8) ,                 -- /  рамок при показе дерева
        OWNCAT NUMBER(8) ,                 -- Ид категории
        CATID  NUMBER(8) ,                 -- Ид родительской категории
        ORD    NUMBER(8) ,                 -- Порядковый номер (внутри своего родителя)
        DESCR  VARCHAR2(200)               -- Описание
       ) ;

CREATE TABLE consts       -- Таблица констант
       (
        oid          NUMBER(*,0),      -- Ид объекта
        const_type   NUMBER(*,0),      -- Тип константы
        value        VARCHAR2(250),    -- Значение
        CONSTRAINT   const_oid FOREIGN KEY (oid) REFERENCES OBJS (oid) ON DELETE CASCADE
       ) ;

CREATE TABLE DBSTR                        -- Структуры баз данных и аналитик
       (
        DBID             NUMBER(8),       -- Ид объекта базы данных
        RealName         VARCHAR2(30),    -- Название поля
        vtype            NUMBER(3,0),     -- Тип поля
        rdbid            NUMBER(8),       -- Ид базы для поля типа база
        maxlen           NUMBER(3,0),     -- Максимальная длина поля
        deleted          NUMBER(3,0) DEFAULT 0,  -- Признак удаленности
        Alias            VARCHAR2(40),    -- Имя для пользователя
        disp_opt         NUMBER(8),       -- Пользовательские установки
        width            NUMBER(3,0),     -- Ширина
        dec              NUMBER(3,0),     -- Показываемые дес. цифры
        ord              NUMBER(4),       -- порядковый номер при показе
        def_opt          NUMBER(8),       -- Админ. установки
        DB_SetName       VARCHAR2(20),    -- Набop установок для связанной базы
        lines            NUMBER(8),       
        CONSTRAINT PK_DBSTR PRIMARY KEY (DBID,RealName),
        CONSTRAINT DBSTR_id FOREIGN KEY (DBID) REFERENCES objs (oid) ON DELETE CASCADE
       );

CREATE TABLE accs                             -- Рабочий план счетов
       (
        code       NUMBER(*,0),               -- Код счета
        name       VARCHAR(20) NOT NULL,      -- Название 'A/01/001'
        parent     NUMBER(*,0),               -- Код родителя
        descr      VARCHAR2(250),             -- Описание 'Основные ср-ва'
        bal        NUMBER(*,0),               -- Признак балансовости
        node       NUMBER(*,0),               -- Количество сыновей (0 - лист)
        rev_code   NUMBER(*,0),               -- Код счета для переоценок
        val_code   VARCHAR2(30),              -- Код валюты
        opts       NUMBER(*,0),               -- Установки
        expanded   NUMBER(1),                 -- Признак развернутости
        anal_id    NUMBER(8),                 -- Ид аналитики
        bal_fld    VARCHAR2(30),              -- балансовое поле
        setname    VARCHAR2(20),              -- имя набора установок
        CONSTRAINT accs_pk PRIMARY KEY (code),
        CONSTRAINT accs_name UNIQUE (name),
        CONSTRAINT accs_parent FOREIGN KEY (parent) REFERENCES accs (code),
        CONSTRAINT accs_rev FOREIGN KEY (rev_code) REFERENCES accs (code),
        CONSTRAINT anal_id_oid FOREIGN KEY (anal_id) REFERENCES objs (oid),
        CONSTRAINT bal_fld_dbstr FOREIGN KEY (anal_id,bal_fld) REFERENCES dbstr (dbid,RealName)
       ) ;

-- Полный план счетов
CREATE TABLE ex_accs AS SELECT * FROM accs ;

CREATE TABLE cps                         -- Даты контрольных точек
       (
        point_date    DATE,              -- Дата
        is_start NUMBER(*,0) NOT NULL,   -- На сколько месяцев вперед считать обороты
        CONSTRAINT cps_pk PRIMARY KEY (point_date)
       ) ;

CREATE TABLE cints                       -- Контрольные интервалы
       (
        beg_date   DATE,                 -- Дата начала
        end_date   DATE,                 -- Дата конца
        CONSTRAINT cints_prim PRIMARY KEY (beg_date,end_date),
        CONSTRAINT cints_beg FOREIGN KEY (beg_date) REFERENCES cps (point_date),
        CONSTRAINT cints_end FOREIGN KEY (end_date) REFERENCES cps (point_date)
       ) ;

CREATE TABLE user_options                -- Установки пользователей
       (
        username   VARCHAR2(30),         -- Пользователь
        full_name  VARCHAR(80),          -- Полное имя
        beg_date   DATE,                 -- Начало периода видимости
        end_date   DATE,                 -- Конец периода видимости
        sum_len    NUMBER(*,0),          -- Длина суммы
        sum_dig    NUMBER(*,0),          -- Цифры после десятичной точки
        apps       NUMBER(1,0),          -- Апострофы
        inp_date   DATE,                 -- Дата ввода
        cur_dates  NUMBER(1,0),          -- Текущие даты
        dig_input  NUMBER(1,0),          -- Ввод по цифрам
        sort_num   NUMBER(1,0),          -- Сортировка по номерам док-в в счетах
        dead_date  DATE,                 -- Дата оперативных изменений
        readonly   NUMBER(1,0),          -- Только просмотр
        overwrite  NUMBER(1,0),          -- Режим "наложения"
        imp_opt    NUMBER(8),            -- Установки импорта
        set_to_end NUMBER(1,0),          -- Становиться в конец списка
        CONSTRAINT per_prim PRIMARY KEY (username),
        CONSTRAINT per_cints FOREIGN KEY (beg_date,end_date) REFERENCES cints (beg_date,end_date)
       ) ;

CREATE TABLE sums                        -- Синтетические суммы
       (
        sum_id NUMBER(*,0),              -- Код суммы
        value  NUMBER(24,6),             -- Значение
        CONSTRAINT sums_pk PRIMARY KEY (sum_id)
       ) ;

CREATE TABLE rems                        -- Остатки по счетам
       (
        code       NUMBER(*,0),          -- Код счета
        rem_date   DATE,                 -- Дата остатка
        end_val_id NUMBER(*,0),          -- Код суммы остатка
        CONSTRAINT rems_pk PRIMARY KEY (code,rem_date),
        CONSTRAINT rems_cpid FOREIGN KEY (rem_date) REFERENCES cps (point_date),
        CONSTRAINT rems_code FOREIGN KEY (code) REFERENCES accs(code)
       ) ;
CREATE OR REPLACE TRIGGER Rem_Trg  -- Триггер удаления суммы при удалении
AFTER DELETE ON rems               -- записи из базы остатков
FOR EACH ROW
BEGIN
 DELETE from sums where sum_id=:old.end_val_id ;
END;
/


CREATE TABLE turns                 -- Обороты по счетам
       (
        beg_date   DATE,           -- Начало контрольного интервала
        end_date   DATE,           -- Конец контрольного интервала
        code       NUMBER(*,0),    -- Код счета
        d_turn_id   NUMBER(*,0),   -- Код суммы оборота по дебету
        c_turn_id   NUMBER(*,0),   -- Код суммы оборота по кредиту
        CONSTRAINT turns_prim PRIMARY KEY (beg_date,end_date,code),
        CONSTRAINT turns_dates FOREIGN KEY (beg_date,end_date) REFERENCES cints (beg_date,end_date),
        CONSTRAINT turns_code FOREIGN KEY (code) REFERENCES accs(code)
       ) ;
CREATE OR REPLACE TRIGGER Turn_Trg  -- Триггер удаления суммы
AFTER DELETE ON turns
FOR EACH ROW
BEGIN
 DELETE from sums where sum_id=:old.d_turn_id OR sum_id=:old.c_turn_id;
END;
/


CREATE TABLE cor_turns              -- Корреспондирующие обороты
       (
        beg_date   DATE,            -- Начало контрольного интервала
        end_date   DATE,            -- Конец контрольного интервала
        d_code     NUMBER(*,0),     -- счет по дебету
        c_code     NUMBER(*,0),     -- счет по кредиту
        turn_id    NUMBER(*,0),     -- код суммы оборота
        CONSTRAINT cor_turns_prim PRIMARY KEY (beg_date,end_date,d_code,c_code),
        CONSTRAINT cor_turns_dates FOREIGN KEY (beg_date,end_date) REFERENCES cints (beg_date,end_date),
        CONSTRAINT cor_turns_d_code FOREIGN KEY (d_code) REFERENCES accs (code),
        CONSTRAINT cor_turns_c_code FOREIGN KEY (c_code) REFERENCES accs (code)
       ) ;
CREATE OR REPLACE TRIGGER CorTurn_Trg   -- Триггер удаления суммы
AFTER DELETE ON cor_turns
FOR EACH ROW
BEGIN
 DELETE from sums where sum_id=:old.turn_id ;
END;
/


CREATE TABLE conds                    -- Проводки стандартных операций
       (
        cond_id      NUMBER(*,0),     -- Код проводки
        cond_subid   NUMBER(*,0),     -- Суб-код
        ord          NUMBER(8),       -- порядковый номер при вводе
        text         VARCHAR2(250),   -- Текст проводки
        doc_type     VARCHAR2(20),    -- Тип документа
        storno       NUMBER(1),       -- признак "Сторно"
        native_doc   NUMBER(1),       -- признак "сохранять только с родным типом документа"
        auto_input   NUMBER(1),       -- признак "Авто ввод"
        auto_gen     NUMBER(1),       -- признак "Авто документ"
        auto_apply   NUMBER(1),       -- признак "Проводить автоматически"
        for_post     NUMBER(1),       -- Предлагать откладывать
        pre_formula  VARCHAR2(250),   -- фоpмула, исполняемая пеpед вводом пpоводки
        CONSTRAINT conds_prim PRIMARY KEY (cond_id,cond_subid),
        CONSTRAINT conds_ord UNIQUE (cond_id,ord),
        CONSTRAINT conds_id FOREIGN KEY (cond_id) REFERENCES objs (oid) ON DELETE CASCADE,
        CONSTRAINT conds_doc FOREIGN KEY (doc_type) REFERENCES objs (name)
       ) ;

CREATE TABLE cond_items           -- Элементы стандартных проводок
       (
        cond_id      NUMBER(*,0),   -- Код проводки
        cond_subid   NUMBER(*,0),   -- Суб-код
        code         NUMBER(*,0),   -- Код счета
        setname      VARCHAR2(20),  -- имя набора установок
        side         VARCHAR2(1),   -- Сторона ('D' или 'K')
        for_post     NUMBER(1,0),   -- Признак откладывания
        item_num     NUMBER(*,0),   -- Порядковый номер в проводке
        item_formula VARCHAR2(250), -- Формула
        CONSTRAINT cond_items_prim PRIMARY KEY (cond_id,cond_subid,code),
        CONSTRAINT cond_items_uni_num UNIQUE (cond_id,cond_subid,item_num),
        CONSTRAINT cond_items_cond_id FOREIGN KEY (cond_id,cond_subid) REFERENCES conds (cond_id,cond_subid) ON DELETE CASCADE,
        CONSTRAINT cond_items_code FOREIGN KEY (code) REFERENCES accs (code)
       ) ;

CREATE TABLE opers   -- Операции
       (
        op_id      NUMBER(*,0),   -- Код операции
        op_subid   NUMBER(*,0),   -- Суб-код
        status     NUMBER(1,0),   -- 0-вводимая  1-проведенная, 2-удаляемая ,3-изменяемая
        post       NUMBER(1,0),   -- Признак отложенности
        storno     NUMBER(1,0),   -- Признак сторнирования
        text       VARCHAR2(250), -- Текст операции
        doc_type   VARCHAR2(20),  -- Тип документа
        doc_num    VARCHAR2(30),  -- Номер документа
        doc_num_v  NUMBER(*,0),   -- Версия номера документа
        op_date    DATE,          -- Дата операции
        cond_id    NUMBER(*,0),   -- Код стандартной операции
        edited     NUMBER,        -- Признак редактируемости в данный момент
        CONSTRAINT opers_prim PRIMARY KEY (op_id,op_subid),
        CONSTRAINT opers_doc FOREIGN KEY (doc_type) REFERENCES objs (name),
        CONSTRAINT opers_doc_num UNIQUE (status,doc_type,doc_num,doc_num_v),
        CONSTRAINT opers_cond FOREIGN KEY (cond_id,op_subid) REFERENCES conds (cond_id,cond_subid)
       ) ;
CREATE UNIQUE INDEX date_ind ON opers(op_date,op_id,op_subid) ;

CREATE TABLE op_items                         -- Элементы операции
       (
        op_id         NUMBER(*,0),            -- Код операции
        op_subid      NUMBER(*,0),            -- Суб-код операции
        op_date       DATE,                   -- Дата
        code          NUMBER(*,0),            -- Код счета
        side          VARCHAR2(1),            -- Сторона (D или K)
        item_sum_id   NUMBER(*,0),            -- Код суммы
        node          NUMBER(1,0) DEFAULT 0,  -- 0 - лист , 1 - узел , 2 - узел с собственной суммой
        for_post      NUMBER(1,0),            -- для откладывания
        setname       VARCHAR2(20),           -- имя набора установок
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

CREATE TABLE del_opers   -- Удаленные Операции
       (
        op_id      NUMBER(*,0),   -- Код операции
        op_subid   NUMBER(*,0),   -- Суб-код
        status     NUMBER(1,0),   -- Не используется
        post       NUMBER(1,0),   -- Признак откложенности
        storno     NUMBER(1,0),   -- Признак сторнирования
        text       VARCHAR2(250), -- Текст операции
        doc_type   VARCHAR2(20),  -- Тип документа
        doc_num    VARCHAR2(50),  -- Номер документа
        doc_num_v  NUMBER(*,0),  --  Версия номера документа
        op_date    DATE,          -- Дата операции
        cond_id    NUMBER(*,0),   -- Код стандартной операции
        edited     NUMBER,
        CONSTRAINT del_opers_prim PRIMARY KEY (op_id,op_subid)
       ) ;

CREATE TABLE del_op_items                     -- Элементы удаленных операций
       (
        op_id         NUMBER(*,0),            -- Код операции
        op_subid      NUMBER(*,0),            -- Суб-код операции
        op_date       DATE,                   -- Дата
        code          NUMBER(*,0),            -- Код счета
        side          VARCHAR2(1),            -- Сторона (D или K)
        item_sum_id   NUMBER(*,0),            -- Код суммы
        node          NUMBER(1,0) DEFAULT 0,  -- 0 - лист , 1 - узел , 2 - узел с собственной суммой
        for_post      NUMBER(1,0),            -- для откладывания
        CONSTRAINT del_op_items_prim PRIMARY KEY (op_id,op_subid,code),
        CONSTRAINT del_op_items_op_id FOREIGN KEY (op_id,op_subid) REFERENCES del_opers (op_id,op_subid)
       ) ;

CREATE TABLE cor_items                 -- Корреспондирующие элементы операций
       (
        op_id          NUMBER(*,0),    -- Код операции
        op_subid       NUMBER(*,0),    -- Суб-код операции
        d_code         NUMBER(*,0),    -- Код счета по дебету
        c_code         NUMBER(*,0),    -- Код счета по кредиту
        item_sum_id    NUMBER(*,0),    -- Код суммы
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


CREATE TABLE history                   -- История создания,изменений и удаления оперций
       (
        op_id    NUMBER,               -- Код операции
        op_subid NUMBER,               -- Суб-код операции
        revision NUMBER,               -- номер изменения
        rev_time DATE,                 -- дата изменения
        username VARCHAR2(30),         -- пользователь
        CONSTRAINT hist_prim PRIMARY KEY (op_id,op_subid,revision),
        CONSTRAINT hist_user FOREIGN KEY (username) REFERENCES user_options (username)
       ) ;

CREATE TABLE dbhistory                 -- История создания,изменений и удаления позиций БД
       (
        db_id    NUMBER,               -- ид объекта
        article  varchar2(30),         -- Артикул позиции
        ver      number(8),            -- Версия позиции
        revision NUMBER,               -- номер изменения
        rev_time DATE,                 -- дата изменения
        username VARCHAR2(30),         -- пользователь
        CONSTRAINT dbhist_prim PRIMARY KEY (db_id,article,ver,revision),
        CONSTRAINT dbhist_user FOREIGN KEY (username) REFERENCES user_options (username),
        CONSTRAINT dbhist_objs FOREIGN KEY (db_id) REFERENCES objs (oid) ON DELETE CASCADE
       ) ;


CREATE TABLE DBOPT (                   -- Наборы установок
       DBID             NUMBER(8),     -- Ид объекта
       SETNAME          VARCHAR2(20),  -- Имя набора
       CONSTRAINT PK_DBOPT PRIMARY KEY (DBID,SETNAME),
       CONSTRAINT DBOPT_id FOREIGN KEY (DBID) REFERENCES objs (oid) ON DELETE CASCADE
);

CREATE TABLE DBOPTOV (                 -- Установки для разных наборов
       DBID             NUMBER(8),     -- Ид объекта
       SETNAME          VARCHAR2(20),  -- Имя набора
       RealName         VARCHAR2(30),  -- Имя поля
       OPT              NUMBER(8),     -- Установки (Административные)
       DB_setname       VARCHAR2(20),
       CONSTRAINT PK_DBOPTOV PRIMARY KEY (DBID,SETNAME,RealName),
       CONSTRAINT DBOPTS_FK FOREIGN KEY (DBID,SETNAME) REFERENCES DBOPT (DBID,SETNAME) ON DELETE CASCADE
);


CREATE TABLE dbaddinfo
       (
        dbid        NUMBER(8),     -- Ид объекта
        setname     VARCHAR2(20),  -- Имя набора
        RealName    VARCHAR2(30),  -- Имя поля
        name        VARCHAR2(30),  -- Системное имя
        code        NUMBER(*,0),   -- Код счета
        turntype    VARCHAR2(3) NOT NULL,   -- Тип суммы (R,BR,DT,CT)
        alias       VARCHAR2(30),  -- Пользовательское имя
        CONSTRAINT  PK_DBADD PRIMARY KEY (DBID,setname,RealName,name),
        CONSTRAINT  DBADD_id_SET FOREIGN KEY (DBID,SetName) REFERENCES dbopt (dbid,SetName) ON DELETE CASCADE,
        CONSTRAINT  DBADD_id_NAME FOREIGN KEY (DBID,RealName) REFERENCES dbstr (dbid,RealName) ON DELETE CASCADE,
        CONSTRAINT  dbadd_code FOREIGN KEY (code) REFERENCES accs (code),
        CONSTRAINT  check_type CHECK (turntype IN ('BR','DT','CT','R'))
       );

CREATE TABLE DBCAT                  -- Категории для баз данных
       (DBID   NUMBER(8) ,             -- Ид объекта
        OWNCAT NUMBER(8) ,             -- Код категории
        CATID  NUMBER(8) ,             -- Код родительской категории
        ORD    NUMBER(8) ,             -- Прорядковый номер при показе
        DESCR  VARCHAR2(200) ,         -- Описание
        CONSTRAINT PK_DBCAT PRIMARY KEY (DBID,OWNCAT),
        CONSTRAINT PK_DBOWNCAT UNIQUE (OWNCAT),
        CONSTRAINT DBCAT_FK FOREIGN KEY (DBID,CATID) REFERENCES DBCAT(DBID,OWNCAT),
        CONSTRAINT DBCAT_id FOREIGN KEY (DBID) REFERENCES objs (oid) ON DELETE CASCADE
        );

create table DBCATTREE     -- таблица для показа дерева категорий баз данных
       (
        LEV    NUMBER(8) ,                 -- Номер уровня
        TORD   NUMBER(8) ,                 -- Порядковый номер (сквозной)
        LAST   NUMBER(2) ,                 -- \  служебные поля для рисования
        LEVM   NUMBER(8) ,                 -- /  рамок при показе дерева
        DBID   NUMBER(8) ,                 -- Ид базы данных
        OWNCAT NUMBER(8) ,                 -- Ид категории
        CATID  NUMBER(8) ,                 -- Ид родительской категории
        ORD    NUMBER(8) ,                 -- Порядковый номер (внутри своего родителя)
        DESCR  VARCHAR2(200),              -- Описание
        CONSTRAINT U_DBCATTREE UNIQUE (OWNCAT)
       );

CREATE TABLE OBJPRIV                 -- Права доступа к объектам
       (OID       NUMBER(8) NOT NULL,  -- Ид объекта
        RUNAME    VARCHAR2(30) ,       -- Имя роли или пользователя
        RU        number(1) ,          -- Признак пользователя
        PR0       NUMBER(1) ,          -- Права на просмотр
        PR1       NUMBER(1) ,          -- Права на изменение
        PR2       NUMBER(1) ,          -- Права на создание
        PR3       NUMBER(1) ,          -- Права на удаление
        PR4       NUMBER(1) ,          -- Зарезервировано
        PR5       NUMBER(1) ,          -- Зарезервировано
        CONSTRAINT PK_OPRIV PRIMARY KEY (OID,RU,RUNAME)
        );

CREATE TABLE PRIVTEST                  -- Права пользователей
       (
        POID      NUMBER(8) NOT NULL,         -- Ид объекта
        UNAME     VARCHAR2(30) ,              -- Имя пользователя
        PR0       NUMBER(1) ,                 -- права на просмотр
        PR1       NUMBER(1) ,                 -- права на изменение
        PR2       NUMBER(1) ,                 -- права на создание
        PR3       NUMBER(1) ,                 -- права на удаление
        PR4       NUMBER(1) ,                 -- Зарезервировано
        PR5       NUMBER(1) ,                 -- Зарезервировано
        CONSTRAINT PK_PRIVTEST PRIMARY KEY (POID,UNAME)
       );

CREATE TABLE anal_errs                        -- отрицательные остатки
       (
        SID      NUMBER,                      -- Ид сессии
        code     NUMBER,                      -- Код счета
        AARTICLE VARCHAR2(30),                -- Артикул
        AVER     NUMBER(8),                   -- Версия
        undo     NUMBER(1)                    -- Проводилась или откатывалась
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
COMMENT ON TABLE UDBOPT IS 'ПОЛЬЗОВАТЕЛЬСКИЕ УСТАНОВКИ ДЛЯ БАЗЫ ДАННЫХ ДЛЯ КАЖДОГО ПОЛЬЗОВАТЕЛЯ И ДЛЯ КАЖДОГО НАБОРА УСТАНОВОК'
/
COMMENT ON COLUMN UDBOPT.SETNAME IS 'НАБОР УСТАНОВОК ДЛЯ ПУСТОГО НАБОРА = " " (ПРОБЕЛ) Т.К. ВХОДИТ В ПЕРИЧНЫЙ КЛЮЧ И НЕ МОЖЕТ БЫТЬ NULL, ПОЭТОМУ НЕ НАДО ССЫЛКИ НА DBOPT'
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

comment on table TMP_SUMS is 'Таблица хранит коды временных сумм' ;
comment on column TMP_SUMS.SUM_ID is 'Код временной суммы';
comment on column TMP_SUMS.CREATE_DATE is 'Дата создания временной суммы';
comment on column TMP_SUMS.CREATOR is 'Пользователь, создавший временную сумму';
comment on column TMP_SUMS.DESCR is 'Описание временной суммы (может быть NULL)';

create table message_table
(
 Constraint_Name varchar2(30) not null,
 Actions         varchar2(3)  not null,
 Parent_Child    varchar2(1)  not null,
 Error_Message   varchar2(2000)
);
insert into message_table values('OPERS_DOC','IU','C','Нет такого документа') ;


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


comment on table AN_FLD_ORDER is 'Для определения порядка полей при
редактировании аналитической суммы'
/

comment on column AN_FLD_ORDER.ANID is 'Код аналитики которую редактируем'
/

comment on column AN_FLD_ORDER.SETNAME is 'Имя набора установок'
/

comment on column AN_FLD_ORDER.RealName is 'Как называется поле в аналитике или в базе данных'
/

comment on column AN_FLD_ORDER.name is 'Алиас поля в SQL запросе ( для полей БД и АН пустой?)'
/

comment on column AN_FLD_ORDER.DB_ID is 'для поля базы данных показывает из какой оно БД, для поля аналитики =ANID, для доп полей NULL'
/

comment on column AN_FLD_ORDER.FLD_TYPE is 'Тип поля
             DB - база данных
             AN - аналитика
            ANR - аналитика остатка
            ADD - дополнительное поле'
/

comment on column AN_FLD_ORDER.FLD_ORD is 'Порядок поля при просмотре аналитики'
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
comment on table EXCEL_IMP is 'Хранит последовательность полей для импорта в БД из Excel'
/
comment on column EXCEL_IMP.DB_ID is 'Код БД в которую будем импортировать данные'
/
comment on column EXCEL_IMP.SHORT_NAME is 'Короткое название импорта'
/
comment on column EXCEL_IMP.DESCR is 'Подробное описание '
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
comment on table EXCEL_IMP_ITEMS is 'Хранит порядок полей'
/
comment on column EXCEL_IMP_ITEMS.ORD is 'Порядок поля'
/
comment on column EXCEL_IMP_ITEMS.REALNAME is 'Название поля'
/
create table ADD_OPITEMS as
(select O.OP_ID, O.OP_SUBID, OI.SETNAME, OI.ITEM_SUM_ID, O.DOC_NUM,
    O.DOC_NUM_V, O.DOC_TYPE, O.TEXT, O.OP_DATE, O.STORNO, OI.SIDE
from  OPERS O, OP_ITEMS OI where ROWNUM < 1);


exit
