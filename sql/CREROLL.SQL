/* �������� ROLLBACK ᥣ���� � ����࠭�祭�� ������⢮� EXTENT'��        */
/* ��� �ᯮ�짮����� � ������� �࠭������ (��砫� ������ �����, ������) */

CREATE PUBLIC ROLLBACK SEGMENT myroll TABLESPACE ROLLBACK_DATA STORAGE (MAXEXTENTS UNLIMITED) ;
ALTER ROLLBACK SEGMENT myroll ONLINE ;
