-- Таблица для копирования проводок
-- содержит соответствие счетов в разных бухгалтериях
-- Создаем в бухгалтерии ОТКУДА копируем
---------------------------------------------------------
drop table ACCS_TIES;
commit;

CREATE TABLE ACCS_TIES 
 (
  BASEUSERNAME VARCHAR2(30) NOT NULL,      -- имя базового пользователя куда копируем проводки
  ACC_NAME VARCHAR2(30) NOT NULL,          -- ИМЯ счета в текущей бухгалтерии
  ACC_NAME2 VARCHAR2(30) NOT NULL,         -- ИМЯ счета в бухгалтерии куда копируем
  CONSTRAINT PK_ACCS_TIES PRIMARY KEY (BASEUSERNAME, ACC_NAME),  
  CONSTRAINT FK_ACCS_TIES_TO_ACCS FOREIGN KEY (ACC_NAME) REFERENCES ACCS(NAME))  
  PCTFREE 20 PCTUSED 60 INITRANS 2 MAXTRANS 255 STORAGE ( INITIAL 48K NEXT 500K MINEXTENTS 1 MAXEXTENTS 121 PCTINCREASE 0);
exit;