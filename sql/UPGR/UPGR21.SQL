alter table dbaddinfo add
      (
       CONSTRAINT check_type CHECK (turntype IN ('BR','DT','CT','R'))
      ) ;
exit
