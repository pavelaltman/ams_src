drop table ADD_OPITEMS
/
create table ADD_OPITEMS as
(select O.OP_ID, O.OP_SUBID, OI.SETNAME, OI.ITEM_SUM_ID, O.DOC_NUM,
    O.DOC_NUM_V, O.DOC_TYPE, O.TEXT, O.OP_DATE, O.STORNO, OI.SIDE
from  OPERS O, OP_ITEMS OI where ROWNUM < 1)
/
grant select on ADD_OPITEMS to &1.ROLE;
exit;
