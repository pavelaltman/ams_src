/* Создание ROLLBACK сегмента с неограниченным количеством EXTENT'ов        */
/* для использования в длинных транзакциях (начало нового месяца, пересчет) */

CREATE PUBLIC ROLLBACK SEGMENT myroll TABLESPACE ROLLBACK_DATA STORAGE (MAXEXTENTS UNLIMITED) ;
ALTER ROLLBACK SEGMENT myroll ONLINE ;
