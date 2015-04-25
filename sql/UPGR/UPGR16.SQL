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
