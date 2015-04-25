alter rollback segment myroll online ;
commit ;
set transaction use rollback segment myroll ;

begin
    Periods.AddNotExistedCInts;--  Добавляет периоды, необходимые по IS_START
                               --  и быстро считает обороты и корр обороты       
    commit;
end;
/

exit
