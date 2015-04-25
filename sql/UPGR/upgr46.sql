--------------------------------------
-- 23.11.01 ��ࠢ����� �訡�� FOR_POST
--  ��� OP_ITEMS � GetAccSumId --
--------------------------------------
CREATE OR REPLACE PROCEDURE
    GetAccSumId(
        pAccCode    number,     ---- ��� ���
        pOpId       number,     ---- ��� ����樨
        pOpSubId    number,     ---- SubCod
        pAnalSumId  in out number,--- �� �室� -����� �㬬� ����஢���
                                  --- �� ��室� � ����� �㬬� ᪮��஢���
        pCopy   number,         ---- ����஢��� ���������� �㬬� ���� 
                                ----  �᫨ ���� �-� � ������ �㬬�
        pItemNum number,         ---- ����� ��� � �⠭� ����
        pSetName varchar2 default NULL

        ) AS 
                                                                   
    ---- ������� ���������� �㬬� �� ���� � ��砥 A=... 
    ---- �᫨ 㪠���� pAnalSumId ==0 � �� �㤥� ������� ����஢���
    vSumId number;
    vSide varchar2(1);
    vPost number(1);
    vOpDate date;
    vNode number(8);
    vAnalId number(8);
    vNew number(1) :=0;

BEGIN
    select max(ITEM_SUM_ID) into vSumId
    from OP_ITEMS
    where OP_ID=pOpId and OP_SUBID=pOpSubId and CODE=pAccCode;

    select ANAL_ID, NODE into vAnalId, vNode
    from ACCS
    where CODE=pAccCode;

    if vNode != 0 then
        raise_application_error(-20012, '����஢���� ⮫쪮 ��� ��⮢ ��� ����⮢');
    end if;

 

    ---- �᫨ �� �� �뫮 ��⠢���� ������ � OP_ITEMS 
    if vSumId is NULL then
        vNew:=1;

        select SIDE, FOR_POST into vSide, vPost
        from COND_ITEMS
        where
            COND_ID=(select COND_ID from OPERS
                     where OP_ID=pOpId and OP_SUBID=pOpSubId) and
            COND_SUBID = pOpSubId and
            ITEM_NUM=pItemNum;
                       
        CreateSum(vSumId,0);

        select OP_DATE into vOpDate
        from OPERS
        where OP_ID=pOpId and OP_SUBID=pOpSubId;

        insert into OP_ITEMS
            (OP_ID, OP_SUBID, OP_DATE, CODE,      SIDE, ITEM_SUM_ID, NODE, FOR_POST, SETNAME)
        values
            (pOpId, pOpSubId, vOpDate, pAccCode,  vSide,   vSumId,     0,   vPost,  pSetName);
    end if;

    if pAnalSumId!=0 then
        if vNew=1 or pCopy=1 or GetKolPos(vSumId, vAnalId)=0 then
            CopyAnalSum(vAnalId, vSumId, pAnalSumId);
        end if;
    end if;
    pAnalSumId:=vSumId;
    return;
END ;
/
show errors;
