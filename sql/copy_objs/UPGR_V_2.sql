-- ������ ��� ����஢���� �஢����
-- ᮤ�ন� ᮮ⢥��⢨� ��⮢ � ࠧ��� ��壠�����
-- ������� � ��壠��ਨ ������ �����㥬
---------------------------------------------------------
drop table ACCS_TIES;
commit;

CREATE TABLE ACCS_TIES 
 (
  BASEUSERNAME VARCHAR2(30) NOT NULL,      -- ��� �������� ���짮��⥫� �㤠 �����㥬 �஢����
  ACC_NAME VARCHAR2(30) NOT NULL,          -- ��� ��� � ⥪�饩 ��壠��ਨ
  ACC_NAME2 VARCHAR2(30) NOT NULL,         -- ��� ��� � ��壠��ਨ �㤠 �����㥬
  CONSTRAINT PK_ACCS_TIES PRIMARY KEY (BASEUSERNAME, ACC_NAME),  
  CONSTRAINT FK_ACCS_TIES_TO_ACCS FOREIGN KEY (ACC_NAME) REFERENCES ACCS(NAME))  
  PCTFREE 20 PCTUSED 60 INITRANS 2 MAXTRANS 255 STORAGE ( INITIAL 48K NEXT 500K MINEXTENTS 1 MAXEXTENTS 121 PCTINCREASE 0);
exit;