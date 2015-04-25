/* основных таблиц (кроме баз данных и аналитик) */

insert into objcat  select * from &1..objcat;
insert into objs    select * from &1..objs;
insert into cattree select * from &1..cattree;
insert into consts  select * from &1..consts;
insert into dbstr   select * from &1..dbstr;
insert into accs    select * from &1..accs;
insert into ex_accs select * from &1..ex_accs;
insert into cps     select * from &1..cps;
insert into cints   select * from &1..cints;
insert into user_option  select * from &1..user_options;
insert into sums    select * from &1..sums;
insert into rems    select * from &1..rems;
insert into turns   select * from &1..turns;
insert into cor_turns    select * from &1..cor_turns;
insert into conds        select * from &1..conds;
insert into cond_items   select * from &1..cond_items;
insert into opers        select * from &1..opers;
insert into op_items     select * from &1..op_items;
insert into del_opers    select * from &1..del_opers;
insert into del_op_items select * from &1..del_op_items;
insert into cor_items    select * from &1..cor_items;
insert into unapp_items  select * from &1..unapp_items;
insert into unapp_cor_items     select * from &1..unapp_cor_items;
insert into history      select * from &1..history;
insert into dbopt        select * from &1..dbopt;
insert into dboptov      select * from &1..dboptov;
insert into dbcat        select * from &1..dbcat;
insert into dbcattree    select * from &1..dbcattree;
insert into objpriv      select * from &1..objpriv;
insert into privtest     select * from &1..privtest;
exit
