alter rollback segment myroll online ;
commit ;
set transaction use rollback segment myroll ;

begin
    Periods.AddNotExistedCInts;--  �������� ��ਮ��, ����室��� �� IS_START
                               --  � ����� ��⠥� ������ � ���� ������       
    commit;
end;
/

exit
