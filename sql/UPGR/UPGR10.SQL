drop table anal_errs ;
CREATE TABLE anal_errs                        -- отрицательные остатки
       (
        SID      NUMBER,                      -- Ид сессии
        code     NUMBER,                      -- Код счета
        undo     NUMBER(1),
        AARTICLE VARCHAR2(30),                -- Артикул
        AVER     NUMBER(8)                    -- Версия
       ) ;
