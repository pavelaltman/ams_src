--------------------------------------
-- для функционирования объекта AccMan
--------------------------------------
create or replace function
OpSign ( pSide OP_ITEMS.SIDE%TYPE, pStorno OPERS.STORNO%Type) return NUMBER
as
begin
    if (pSide='D' and pStorno=0) or (pSide!='D' and pStorno=1) then
        return 1;
    else
        return 0;
    end if;
end;
/            
create or replace procedure
GetBegSumAndDir ( pAccCode number, pDate date,
                pBegSum out number,
                pOpCursor out Ams_Types.GenCursor) as
    vBegDate date;
    vEndDate date;
    vTmp1 number(8);
    vTmp2 number(8);
begin      
--------- Возвращает код аналитической суммы и дату от которой нужно дальше считать остаток
    select max(BEG_DATE), max(END_DATE) into vBegDate, vEndDate from CINTS
    where (END_DATE-BEG_DATE<35) and pDate between BEG_DATE and END_DATE;

    pBegSum:=0;

    if vBegDate is NULL then
        return;
    end if;

    if vBegDate=pDate OR vEndDate=pDate then
        select max(END_VAL_ID) into pBegSum from REMS
        where CODE=pAccCode and REM_DATE=pDate;
        pBegSum:=nvl(pBegSum,0);
        return;
    end if;
        
        
    select count(*) into vTmp1 from OP_ITEMS OI, OPERS O
    where   OI.OP_DATE>=vBegDate and OI.OP_DATE<pDate and
            OI.CODE=pAccCode and
            OI.OP_ID=O.OP_ID and
            OI.OP_SUBID=O.OP_SUBID and
            O.STATUS=1 and O.POST=0; 

    select count(*) into vTmp2
    from OP_ITEMS OI, OPERS O
    where   OI.OP_DATE>=pDate and OI.OP_DATE<vEndDate and
            OI.CODE=pAccCode and
            OI.OP_ID=O.OP_ID and
            OI.OP_SUBID=O.OP_SUBID and
            O.STATUS=1 and O.POST=0; 

    if vTmp1 > vTmp2 then
        vBegDate:=vEndDate;
        open pOpCursor for
            select OI.ITEM_SUM_ID, -OpSign(OI.SIDE, O.STORNO) OS  
            from OP_ITEMS OI, OPERS O
            where   OI.OP_DATE>=pDate and OI.OP_DATE<vEndDate and
                    OI.CODE=pAccCode and
                    OI.OP_ID=O.OP_ID and
                    OI.OP_SUBID=O.OP_SUBID and
                    O.STATUS=1 and O.POST=0;
    else
        open pOpCursor for
            select OI.ITEM_SUM_ID, OpSign(OI.SIDE, O.STORNO) OS  
            from OP_ITEMS OI, OPERS O
            where   OI.OP_DATE>=vBegDate and OI.OP_DATE<pDate and
                    OI.CODE=pAccCode and
                    OI.OP_ID=O.OP_ID and
                    OI.OP_SUBID=O.OP_SUBID and
                    O.STATUS=1 and O.POST=0;
    end if;


    select max(END_VAL_ID) into pBegSum from REMS
    where CODE=pAccCode and REM_DATE=vBegDate;
    pBegSum:=nvl(pBegSum,0);
end;
/
grant execute on GetBegSumAndDir to &1.ROLE;
grant execute on OpSign to &1.ROLE;
show errors;
exit;