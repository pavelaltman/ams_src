create or replace procedure
    V_RT ( pBR out number, pDT out number, pCT out number, pER out number,
           pAccCode number, pDtBeg date, pDtEnd date) as

    vBeg date;
    vEnd date;
    vTmp1 number;
    vTmp2 number;
    vTmp0 number;

    vDT number:=0;
    vCT number:=0;

    vBeg1 date;
    vEnd1 date;
begin
    -- Находим остаток на начало
    select max(BEG_DATE), max(END_DATE) into vBeg, vEnd
    from CINTS
    where
        (END_DATE-BEG_DATE)<35 and
        pDtBeg between BEG_DATE and END_DATE;

    if vBeg=pDtBeg or vEnd=pDtBeg then
        select VALUE into pBR from SUMS
        where SUM_ID =
            (select END_VAL_ID from REMS 
             where CODE=pAccCode and REM_DATE=pDtBeg);   
    else
        --- Определяем куда дальше считать
        select count(*) into vTmp1 from OP_ITEMS OI, OPERS O
        where
            OI.CODE=pAccCode and 
            OI.OP_DATE>= vBeg and OI.OP_DATE< pDtBeg and
            OI.OP_ID = O.OP_ID and 
            OI.OP_SUBID = O.OP_SUBID and
            O.STATUS= 1 and
            O.POST= 0;


        select count(*) into vTmp2
        from OP_ITEMS OI, OPERS O
        where
            OI.CODE=pAccCode and 
            OI.OP_DATE>= pDtBeg and OI.OP_DATE< vEnd and
            OI.OP_ID = O.OP_ID and 
            OI.OP_SUBID = O.OP_SUBID and
            O.STATUS= 1 and
            O.POST= 0;

        -- Лучше посчитать до конца
        if vTmp1> vTmp2 then
            select sum(GetAccCorTotValue ('D', OI.SIDE, O.STORNO, S.VALUE)),
                   sum(GetAccCorTotValue ('K', OI.SIDE, O.STORNO, S.VALUE))
            into vDT, vCT
            from OP_ITEMS OI, OPERS O, SUMS S
            where
                OI.CODE=pAccCode and
                OI.OP_DATE>= pDtBeg and OI.OP_DATE< vEnd and
                OI.OP_ID = O.OP_ID and
                OI.OP_SUBID = O.OP_SUBID and
                O.STATUS= 1 and
                O.POST= 0 and S.SUM_ID=OI.ITEM_SUM_ID;
            vDT:=nvl(vDT,0);
            vCT:=nvl(vCT,0);

            -- Находим остаток на конец
            select VALUE into vTmp1 from SUMS
            where SUM_ID =
                (select END_VAL_ID from REMS
                 where CODE=pAccCode and REM_DATE=vEnd);

            pBR:=vTmp1-vDT+vCT;
        else
        -- Лучше считать до начала
            select sum(GetAccCorTotValue ('D', OI.SIDE, O.STORNO, S.VALUE)),
                   sum(GetAccCorTotValue ('K', OI.SIDE, O.STORNO, S.VALUE))
            into vDT, vCT
            from OP_ITEMS OI, OPERS O, SUMS S
            where
                OI.CODE=pAccCode and
                OI.OP_DATE>= vBeg and OI.OP_DATE< pDtBeg and
                OI.OP_ID = O.OP_ID and
                OI.OP_SUBID = O.OP_SUBID and
                O.STATUS= 1 and
                O.POST= 0 and S.SUM_ID=OI.ITEM_SUM_ID;
            vDT:=nvl(vDT,0);
            vCT:=nvl(vCT,0);

            -- Находим остаток на конец
            select VALUE into vTmp0 from SUMS
            where SUM_ID =
                (select END_VAL_ID from REMS
                 where CODE=pAccCode and REM_DATE=vEnd);
            --находим обороты
            select SD.VALUE, SC.VALUE into vTmp1, vTmp2
            from TURNS T, SUMS SD, SUMS SC
            where
                T.BEG_DATE=vBeg and
                T.END_DATE=vEnd and
                T.CODE = pAccCode and
                T.D_TURN_ID=SD.SUM_ID and
                T.C_TURN_ID=SC.SUM_ID;

            vDT:=nvl(vTmp1,0)-vDT;
            vCT:=nvl(vTmp2,0)-vCT;

            pBR:=vTmp0-vDT+vCT;
        end if; -- Считаем до начала или конца
    end if;  -- Точно попали в начало периода

    --------------------------------------------------
    -- Находим обороты
    --------------------------------------------------
    if vEnd<pDtEnd then
        vTmp0:=0;
        vBeg:=vEnd;
        while (vTmp0=0) loop
            select max(END_DATE) into vEnd from CINTS
            where   BEG_DATE=vBeg and
                    END_DATE<=pDtEnd;
            if vEnd is NULL then
                vTmp0:=1;
            else                
                --находим обороты
                select SD.VALUE, SC.VALUE into vTmp1, vTmp2
                from TURNS T, SUMS SD, SUMS SC
                where
                    T.BEG_DATE=vBeg and
                    T.END_DATE=vEnd and
                    T.CODE = pAccCode and
                    T.D_TURN_ID=SD.SUM_ID and
                    T.C_TURN_ID=SC.SUM_ID;

                vDT:=nvl(vTmp1,0)+vDT;
                vCT:=nvl(vTmp2,0)+vCT;
                vBeg:=vEnd;
            end if;
        end loop;
        
        --- Находим остаток оборотов
        if (vBeg!=pDtEnd) then
            select sum(GetAccCorTotValue ('D', OI.SIDE, O.STORNO, S.VALUE)),
                   sum(GetAccCorTotValue ('K', OI.SIDE, O.STORNO, S.VALUE))
            into vTmp1, vTmp2
            from OP_ITEMS OI, OPERS O, SUMS S
            where
                OI.CODE=pAccCode and
                OI.OP_DATE>= vBeg and OI.OP_DATE< pDtEnd and
                OI.OP_ID = O.OP_ID and
                OI.OP_SUBID = O.OP_SUBID and
                O.STATUS= 1 and
                O.POST= 0 and S.SUM_ID=OI.ITEM_SUM_ID;
            vDT:=vDT+nvl(vTmp1,0);
            vCT:=vCt+nvl(vTmp2,0);
        end if;
    else
        -- Находим обороты между двумя датами
        select sum(GetAccCorTotValue ('D', OI.SIDE, O.STORNO, S.VALUE)),
               sum(GetAccCorTotValue ('K', OI.SIDE, O.STORNO, S.VALUE))
        into vTmp1, vTmp2
        from OP_ITEMS OI, OPERS O, SUMS S
        where
            OI.CODE=pAccCode and
            OI.OP_DATE>= pDtBeg and OI.OP_DATE< pDtEnd and
            OI.OP_ID = O.OP_ID and
            OI.OP_SUBID = O.OP_SUBID and
            O.STATUS= 1 and
            O.POST= 0 and S.SUM_ID=OI.ITEM_SUM_ID;
        vDT:=nvl(vTmp1,0);
        vCT:=nvl(vTmp2,0);
    end if;


    pDT:=vDT;
    pCT:=vCT;
    pER:=pBR+pDT-pCT;
end;
/
create or replace function
GetDbLine (pOP_ID number, pOP_SUBID number) return varchar2
as
    vDbName varchar2(30);
    vArticle varchar2(30);
    vVer number;
    vTiedToDb number;
    vRes varchar2(250);
begin
    GetLinkDb(pOP_ID, pOP_SUBID, vDbName, vArticle, vVer, vTiedToDb);
    if vDbName is not NULL then
        vRes:=GetDbShortInfo( vDbName, vArticle, vVer);
    end if;
    return vRes;    
end;
/
show errors;
grant execute on V_RT to &1.ROLE;
grant execute on GetDbLine to &1.ROLE;
exit;


        
    