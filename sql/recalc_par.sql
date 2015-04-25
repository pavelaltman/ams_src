alter rollback segment myroll online ;


DECLARE
   CURSOR accounts IS SELECT * FROM accs FOR UPDATE ;
   sub_num NUMBER ;
BEGIN
  FOR A IN accounts LOOP
      SELECT COUNT(*) INTO sub_num FROM accs WHERE parent=A.code ;
      UPDATE accs SET node=sub_num WHERE CURRENT OF accounts ;
  END LOOP ;
END ;
/

COMMIT ;
set transaction use rollback segment myroll ;
DELETE op_items WHERE node>0 ;
COMMIT ;

begin
 set transaction use rollback segment myroll ;
 bookkern.AddAllParentItems ;
 commit ;
end ;
/

exit
