-- ��� ���������� �� ������� �஡���� � ��⨪㫠�
-- ��ࠬ��� ����᪠ - ��� ���� ������
-- � ���� �뢮����� ������⢮ �����饭�� ��⨪㫮�
--------------------------------------------------------
begin
declare 
    cursor S is select ARTICLE, VER from &1. 
        where ARTICLE LIKE '% ' and ARTICLE!=' ';
    ART_NEW varchar2 (40);
begin
    for X in S LOOP
        begin
            ART_NEW:=RTRIM(X.ARTICLE);
            &1._RENAME (X.ARTICLE, X.VER, ART_NEW, X.VER);
            commit;
        EXCEPTION
            when others then
                NULL;
        end;
    end LOOP;
end;
end;
/
select count(*) from &1. 
where ARTICLE LIKE '% ' and ARTICLE!=' ';
