/* �।��⠢����� ����室���� �ࠢ ஫� &1.role */

grant select on objs to &1.role;
grant select,insert,update,delete on consts to &1.role;
grant select on objcat to &1.role;
grant select on cattree to &1.role;
grant select on objpriv to &1.role;
grant select on cps to &1.role;
grant select on rems to &1.role;
grant select on conds to &1.role;
grant select on cond_items to &1.role;
grant select on cints to &1.role;
grant select on accs to &1.role;
grant select,update on user_options to &1.role;
grant select,insert,update,delete on opers to &1.role;
grant select,insert,update,delete on op_items to &1.role;
grant select,insert,update,delete on cor_items to &1.role;
grant select on opitems to &1.role;
grant select on opers_docs to &1.role;
grant select on coritems to &1.role;
grant select on perturns to &1.role;
grant select on periodturns to &1.role;
grant select on remainders to &1.role;
grant select on rems_a to &1.role;
grant select on per_corturns to &1.role;
grant select on corturns to &1.role;
grant select on ops_and_items to &1.role;
grant select on ops_and_items_o to &1.role;
grant select on ops_and_items_o_h to &1.role;
grant select on ops_and_coritems to &1.role;
grant select on ops_and_items_u to &1.role;
grant select on ops_and_items_u_h to &1.role;
grant select on ops_and_items_a to &1.role;
grant select on ops_and_items_h to &1.role;
grant select on turns to &1.role;
grant select on rems_turns to &1.role;
grant select,insert,update,delete on sums to &1.role;
grant select on OperSeq to &1.role ;
grant select on SumSeq to &1.role ;
grant select on SumSeq to &1.role ;
grant select on privtest to &1.role ;
grant select on history to &1.role ;
grant select on dbhistory to &1.role ;
grant select on del_opers to &1.role ;
grant select on del_opitems to &1.role ;
grant select on del_op_items to &1.role ;
grant select,update on dbstr to &1.role ;
grant select on dbaddinfo to &1.role ;
grant select,insert,update,delete on dbcat to &1.role ;
grant select,insert,update,delete on dbcattree to &1.role ;
grant select on dbopt to &1.role ;
grant select on dboptov to &1.role ;
grant select,delete on anal_errs to &1.role ;
grant select on UDBOPT to &1.ROLE;
grant execute on containobjs to &1.role;
grant execute on havepriv to &1.role;
grant execute on haverole to &1.role;
grant execute on CreateSum to &1.role;
grant execute on NormChar to &1.role;
grant execute on periods to &1.role;
grant execute on bookkern to &1.role;
grant execute on makedbtree to &1.role;
grant execute on insertdbcat to &1.role;
grant execute on GetConst to &1.role;
grant execute on CopyAnalSum to &1.role;
grant execute on AddAnalSum to &1.role;
grant execute on MoveAnalSum to &1.role;
grant execute on MoveDBCat to &1.role;
grant execute on DeleteAnalSum to &1.role;
grant execute on getshema to &1.role;
grant execute on init_rights to &1.role;
grant execute on expandedsaldo to &1.role;
grant execute on getrangedates to &1.role;
grant execute on rest to &1.role;
grant execute on dcturn to &1.role;
grant execute on dccorturn to &1.role;
grant execute on r_as to &1.role;
grant execute on dt_ct_as to &1.role;
grant execute on dt_ct_as_cor to &1.role;
grant execute on dt_ct_as_text to &1.role;
grant execute on DBRevision to &1.role;
grant alter rollback segment to &1.role ;
grant select on SYS.V_$SESSION to &1.role ;
grant execute on haverole to &1.role;
grant execute on CSM to &1.role;
grant execute on CSIM to &1.role;
grant execute on GETOBJNAME to &1.role;
grant execute on GETOPERSUM to &1.role;
grant execute on GETDELOPERSUM to &1.role;
grant execute on GETOPERACC to &1.role;
grant execute on GETDELOPERACC to &1.role;
grant execute on USER_ACC to &1.role;
grant execute on ANALINFO to &1.role;
grant execute on GETDCLETTER to &1.role;
grant execute on ChangeDbCats   to &1.role;
grant execute on DelDbCat       to &1.role;
grant execute on MoveDbCatTo    to &1.role;
grant execute on MoveAllDbCatTo to &1.role;
grant execute on GetObjCatFullName to &1.ROLE;
grant execute on GetCatFullName to &1.ROLE;
grant execute on GetObjCode to &1.ROLE;
grant execute on GetAccCode to &1.ROLE;
grant execute on R_ASM  to &1.ROLE;
grant execute on RIM    to &1.ROLE;
grant execute on USERACC_NEW to &1.ROLE;
grant execute on DeleteIfTmpSum to &1.Role;
grant execute on DeleteTmpSum   to &1.Role;
grant execute on CreateTmpSum   to &1.Role;
grant execute on PREPARE_EDIT_OPER to &1.Role;
grant execute on CANCEL_EDIT_OPER to &1.Role;
grant execute on NEW_HIST_RECORD to &1.Role;
grant select on message_table to &1.Role;
grant select on tmp_sums to &1.Role;
grant select on AN_FLD_ORDER to &1.ROLE;
grant execute on CreateAndCopyAnSum to &1.ROLE;
grant execute on RI to &1.ROLE;
grant execute on DTI to &1.ROLE;
grant execute on CTI to &1.ROLE;
grant execute on BRI to &1.ROLE;
grant execute on GetInpDate to &1.ROLE;
grant execute on GetLinkDb to &1.ROLE;
grant execute on GetAccSumId to &1.ROLE;
grant execute on GetAnSumByAnId to &1.ROLE;
grant execute on GetParentAcc to &1.ROLE;
grant execute on GetAccCorValue to &1.ROLE;
grant execute on GetAccCorTotValue to &1.ROLE;
grant execute on RefreshAccs to &1.ROLE;
grant execute on GetAccDKValue to &1.ROLE;
grant execute on GetDKValue to &1.ROLE;
grant execute on GetAnalInfo to &1.ROLE;
grant execute on GetExistedAcc to &1.ROLE;
grant select, delete, insert on EXCEL_IMP to &1._EXIM;
grant select, delete, insert on EXCEL_IMP_ITEMS to &1._EXIM;
grant select on UNAPP_OPERS to &1.ROLE;
grant select on UNAPP_ITEMS to &1.ROLE;
grant insert,select,update on CATS_MAP to &1._EXIM;
grant delete on CATS_MAP to &1.SUPER;
grant select on ADD_OPITEMS to &1.ROLE;
grant execute on V_RT to &1.ROLE;
grant execute on GetDbLine to &1.ROLE;
grant execute on GetBegSumAndDir to &1.ROLE;
grant execute on OpSign to &1.ROLE;
grant execute on SetDeadDateToday to &1.ROLE;

grant select,insert,update,delete on region to &1.role;
grant select,insert,update,delete on point to &1.role;
grant select on PointSeq to &1.role ;
grant select on RegionSeq to &1.role ;
exit

