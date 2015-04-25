/*  Создание "VIEW", необходимых для работы программы           */
/*  Запускать после cretabs.sql и crefuncs.sql                  */


CREATE OR REPLACE VIEW remainders AS -- Остатки по счетам
  SELECT rem_date,code,sum_id,value FROM rems,sums
  WHERE sums.sum_id=rems.end_val_id ;

CREATE OR REPLACE VIEW rems_a AS -- Остатки по счетам с информацией о счетах
  SELECT rem_date,rems.code,sum_id,value,name,descr FROM rems,sums,accs
  WHERE sums.sum_id=rems.end_val_id AND accs.code=rems.code ;

CREATE OR REPLACE VIEW perturns AS   -- Обороты по счетам
  SELECT a.code,beg_date,end_date,d.sum_id as d_sum_id,d.value as d_turn,
                                c.sum_id as c_sum_id,c.value as c_turn,a.name
  FROM turns,sums d,sums c,accs a
  WHERE d.sum_id=d_turn_id AND c.sum_id=c_turn_id AND a.code=turns.code;

CREATE OR REPLACE VIEW periodturns AS   -- Обороты по счетам в периоде
                                        -- видимости текущего пользователя
  SELECT a.code,d.sum_id as d_sum_id,d.value as d_turn,
         c.sum_id as c_sum_id,c.value as c_turn,a.name,a.bal,a.parent
  FROM turns t,sums d,sums c,accs a,user_options u
  WHERE d.sum_id=d_turn_id AND c.sum_id=c_turn_id AND a.code=t.code AND
        t.beg_date=u.beg_date AND t.end_date=u.end_date AND u.username=USER ;


CREATE OR REPLACE VIEW corturns AS   -- Корреспондирующие обороты
  SELECT beg_date,end_date,d_code,c_code,drecs.name as dname,crecs.name as cname,value as turn
  FROM cor_turns,accs drecs,accs crecs,sums
  WHERE drecs.code=d_code AND crecs.code=c_code AND sum_id=turn_id ;

CREATE OR REPLACE VIEW per_corturns AS -- Корреспондирующие обороты в периоде
                                       -- видимости текущего пользователя
  SELECT d_code,c_code,drecs.name as dname,crecs.name as cname,value as turn,drecs.parent as dparent,crecs.parent as cparent,drecs.node as dnode,crecs.node as cnode
  FROM cor_turns ct,accs drecs,accs crecs,sums,user_options u
  WHERE drecs.code=d_code AND crecs.code=c_code AND sum_id=turn_id AND
        ct.beg_date=u.beg_date AND ct.end_date=u.end_date AND u.username=USER;

CREATE OR REPLACE VIEW rems_turns AS   -- Остатки и обороты по счетам в периоде
                                       -- видимости текущего пользователя
  SELECT a.*,b.sum_id as b_sum_id,b.value as begin,
             t.d_sum_id,t.d_turn as debet,t.c_sum_id,t.c_turn as credit,
             e.sum_id as e_sum_id,e.value as end
  FROM accs a,remainders b,perturns t,remainders e,user_options u
  WHERE b.code=a.code AND b.rem_date=u.beg_date
    AND t.code=a.code AND t.beg_date=u.beg_date AND t.end_date=u.end_date
    AND e.code=a.code AND e.rem_date=u.end_date
    AND u.username=USER ;

CREATE OR REPLACE VIEW opitems AS        -- Элементы операций
 SELECT op_items.ROWID AS ROW_ID,op_items.*,accs.name,accs.bal,accs.anal_id,sums.value
 FROM op_items,accs,sums
 WHERE item_sum_id=sum_id AND op_items.code=accs.code ;

CREATE OR REPLACE VIEW ops_and_items AS  -- Элементы операций вместе с
                                         -- информацией о самих операциях
 SELECT i.op_id,i.op_subid,i.code,i.op_date,i.side,i.item_sum_id,i.setname,s.value,s.value-2*s.value*o.storno as svalue,o.text,o.doc_type,o.doc_num,o.doc_num_v,o.storno,o.cond_id
 FROM op_items i,opers o,sums s
 WHERE o.op_id=i.op_id AND o.op_subid=i.op_subid AND status=1 AND
       post=for_post AND i.item_sum_id=s.sum_id ;

CREATE OR REPLACE VIEW ops_and_items_o AS  -- Элементы операций вместе с
                                           -- информацией о самих операциях
                                           -- Только для отложенных
 SELECT i.op_id,i.op_subid,i.code,i.op_date,i.side,i.item_sum_id,i.setname,s.value,s.value-2*s.value*o.storno as svalue,o.text,o.doc_type,o.doc_num,o.doc_num_v,o.storno,o.cond_id
 FROM op_items i,opers o,sums s
 WHERE o.op_id=i.op_id AND o.op_subid=i.op_subid AND status=1 AND
       post=1 AND for_post=0 AND i.item_sum_id=s.sum_id ;

CREATE OR REPLACE VIEW ops_and_items_a AS  -- Элементы операций вместе с
                                           -- информацией о самих операциях
                                           -- и счетах и с отложенными
 SELECT i.op_id,i.op_subid,i.code,i.op_date,i.side,i.item_sum_id,i.setname,s.value,o.text,o.doc_type,o.doc_num,o.doc_num_v,o.storno,o.cond_id,o.post,a.name
 FROM op_items i,opers o,sums s,accs a
 WHERE o.op_id=i.op_id AND o.op_subid=i.op_subid AND status=1 AND
       i.item_sum_id=s.sum_id AND a.code=i.code ;

CREATE OR REPLACE VIEW ops_and_items_u AS  -- Элементы операций вместе с
                                           -- информацией о самих операциях
                                           -- вместе с отложенными операциями
 SELECT i.op_id,i.op_subid,i.code,i.op_date,i.side,i.item_sum_id,s.value,o.text,o.doc_type,o.doc_num,o.doc_num_v,o.storno,o.cond_id
 FROM op_items i,opers o,sums s
 WHERE o.op_id=i.op_id AND o.op_subid=i.op_subid AND status=1 AND
       i.item_sum_id=s.sum_id ;

CREATE OR REPLACE VIEW coritems AS       -- Корреспондирующие элементы операций
 SELECT op_id,op_subid,d_code,c_code,value as turn,drecs.name as dname,crecs.name as cname,drecs.parent as dparent,crecs.parent as cparent,drecs.node as dnode,crecs.node as cnode
 FROM cor_items,accs drecs,accs crecs,sums
 WHERE item_sum_id=sum_id AND drecs.code=d_code AND crecs.code=c_code ;

CREATE OR REPLACE VIEW unapp_coritems AS       -- Непроведенные Корреспондирующие элементы операций
 SELECT i.*,u.cp_date
 FROM unapp_cor_items u,coritems i
 WHERE i.op_id=u.op_id and i.op_subid=i.op_subid and i.d_code=u.d_code and i.c_code=u.c_code ;


CREATE OR REPLACE VIEW ops_and_coritems AS  -- Корреспондирующие элементы операций
 SELECT o.op_id,o.op_subid,d_code,c_code,value as turn,value-2*value*storno as svalue,drecs.name as dname,crecs.name as cname,drecs.parent as dparent,crecs.parent as cparent,drecs.node as dnode,crecs.node as cnode,o.op_date,o.storno,o.doc_type,o.doc_num,o.doc_num_v
 FROM cor_items i,accs drecs,accs crecs,sums,opers o
 WHERE item_sum_id=sum_id AND drecs.code=d_code AND crecs.code=c_code AND i.op_id=o.op_id AND i.op_subid=o.op_subid AND post=0 ;

CREATE OR REPLACE VIEW del_opitems AS    -- Элементы удаленных операций
 SELECT del_op_items.ROWID AS ROW_ID,del_op_items.*,accs.name,accs.bal,sums.value
 FROM del_op_items,accs,sums
 WHERE item_sum_id=sum_id AND del_op_items.code=accs.code ;

CREATE OR REPLACE VIEW opers_docs AS     -- Список операций с выровненными
                                         -- номерами документов
 SELECT opers.*,NormChar(doc_num,30) as doc_num_align FROM opers ;

CREATE OR REPLACE VIEW history_last AS
 SELECT op_id,op_subid,MAX(rev_time) AS rev_time
 FROM history 
 GROUP BY op_id,op_subid ;

CREATE OR REPLACE VIEW ops_and_items_h AS  -- Элементы операций вместе с
                                         -- информацией о самих операциях
                                         -- с датой последнего изменения
 SELECT i.op_id,i.op_subid,i.code,i.op_date,i.side,i.item_sum_id,i.setname,s.value,s.value-2*s.value*o.storno as svalue,o.text,o.doc_type,o.doc_num,o.doc_num_v,o.storno,o.cond_id,h.rev_time
 FROM op_items i,opers o,sums s,history_last h
 WHERE o.op_id=i.op_id AND o.op_subid=i.op_subid AND status=1 AND
       post=for_post AND i.item_sum_id=s.sum_id AND 
       h.op_id=o.op_id AND h.op_subid=o.op_subid ;

CREATE OR REPLACE VIEW ops_and_items_o_h AS  -- Элементы операций вместе с
                                           -- информацией о самих операциях
                                           -- Только для отложенных
                                           -- с датой последнего изменения
 SELECT i.op_id,i.op_subid,i.code,i.op_date,i.side,i.item_sum_id,i.setname,s.value,s.value-2*s.value*o.storno as svalue,o.text,o.doc_type,o.doc_num,o.doc_num_v,o.storno,o.cond_id,rev_time
 FROM op_items i,opers o,sums s,history_last h
 WHERE o.op_id=i.op_id AND o.op_subid=i.op_subid AND status=1 AND
       post=1 AND for_post=0 AND i.item_sum_id=s.sum_id AND
       h.op_id=o.op_id AND h.op_subid=o.op_subid ;

CREATE OR REPLACE VIEW ops_and_items_u AS  -- Элементы операций вместе с
                                           -- информацией о самих операциях
                                           -- вместе с отложенными операциями
                                           -- с датой последнего изменения
 SELECT i.op_id,i.op_subid,i.code,i.op_date,i.side,i.item_sum_id,s.value,o.text,o.doc_type,o.doc_num,o.doc_num_v,o.storno,o.cond_id,rev_time
 FROM op_items i,opers o,sums s,history_last h
 WHERE o.op_id=i.op_id AND o.op_subid=i.op_subid AND status=1 AND
       i.item_sum_id=s.sum_id AND
       h.op_id=o.op_id AND h.op_subid=o.op_subid ;

exit
