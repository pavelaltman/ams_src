CREATE OR REPLACE FUNCTION 
    RI (pName varchar2, pDate Date) return NUMBER AS
-- ���������� ��� �� �㬬� ���⪠ �� ���� (�� � RI, BRI)
 vSumId NUMBER;
begin
 select END_VAL_ID into vSumId  from REMS R, ACCS A 
 where 
  A.NAME = pNAME and A.CODE = R.CODE and REM_DATE = pDate ; 
 return vSumId;
 exception
  when NO_DATA_FOUND then 
    raise_application_error (-20002, 'Date '||pDate||' not is plain period for account '|| pName);  
end;
/
CREATE OR REPLACE FUNCTION 
    RI (pAccCode number, pDate Date default NULL) return NUMBER AS
-- ���������� ��� �� �㬬� ���⪠ �� ���� (�� � RI, BRI)
 vSumId NUMBER;
 pDate1 date;
begin
 pDate1:=pDate;   
 if pDate1 is NULL then 
    select max(END_DATE) into pDATE1
    from USER_OPTIONS
    where
        USERNAME=USER;
 end if;
 select END_VAL_ID into vSumId  from REMS R 
 where 
  R.CODE = pAccCode and REM_DATE = pDate1 ; 
 return vSumId;
 exception
  when NO_DATA_FOUND then 
    raise_application_error (-20002, 'Date '||pDate||' not is plain period for account '|| pAccCode);  
end;
/
CREATE OR REPLACE FUNCTION 
   GetInpDate return DATE AS
-- ���������� ���� ��� ����� �� 㬮�砭��
    vDate date;
    vCurrDates number;
begin
    select INP_DATE, CUR_DATES into vDate, vCurrDates
    from USER_OPTIONS
    where USERNAME=USER;

    if vCurrDates=1 or vDate is NULL then
        select SYSDATE into vDate from DUAL;
    end if;
    return vDate;      
end;
/
CREATE OR REPLACE PROCEDURE
    GetLinkDb(
    pOP_ID number, pOP_SUBID number,
    pDbName   out varchar2 , ---- ��� �易���� ��
    pArticle  out varchar2,  ---- ���+��� ����樨 �� 
    pVer       out number,   ----     �易���� � ������ ����樥�
    pTiedToDoc out number    ---- 1 - �᫨ ���� ������ �易�� � ���㬥�⮬
                             ----     0 - �易�� � ����樥�
    ) AS
    ---- ��� �易���� ���� ������ � ����樥� ��� ���㬥�⮬
    ---- �����頥� �������� �易���� ��, ��⨪� � ����� � ����묨
    ---- ���樨����� ������ ��  
  doc VARCHAR2(30) ;
  cond NUMBER ;
  par NUMBER ;
BEGIN
  pTiedToDoc:=0;  
  pDbName:='';
  SELECT max(parent) INTO par FROM objs WHERE name=(SELECT doc_type FROM opers
                                  WHERE OP_ID=pOP_ID AND OP_SUBID=pOP_SUBID) ;
  IF par IS Not NULL THEN

    pTiedToDoc:=1;
    select NAME into pDbName from OBJS where OID=par;
    select DOC_NUM, DOC_NUM_V into pArticle, pVer
    from OPERS
    where OP_ID=pOP_ID AND OP_SUBID=pOP_SUBID;        

  else
    SELECT max(parent) INTO par FROM objs WHERE oid=(SELECT cond_id FROM opers
                                  WHERE OP_ID=pOP_ID AND OP_SUBID=pOP_SUBID);
    if par is not NULL then
        select NAME into pDbName from OBJS where OID=par;
        select DOC_NUM, DOC_NUM_V into pArticle, pVer
        from OPERS
        where OP_ID=pOP_ID AND OP_SUBID=0;        
    end if;
  END IF ;
END ;
/
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

        select SIDE into vSide
        from COND_ITEMS
        where
            COND_ID=(select COND_ID from OPERS
                     where OP_ID=pOpId and OP_SUBID=pOpSubId) and
            COND_SUBID = pOpSubId and
            ITEM_NUM=pItemNum;
                       
        CreateSum(vSumId,0);

        select OP_DATE, POST into vOpDate, vPost
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
/
CREATE OR REPLACE function
    GetAnSumByAnId(
        pAnalId     number,     ---- ��� �����⨪�
        pOpId       number,     ---- ��� ����樨
        pOpSubId    number)     ---- SubCod
        return number AS
    ---- �����頥� ��� �������᪮� �㬬�  
    cursor S is select OI.ITEM_SUM_ID
                from OP_ITEMS OI, ACCS A
                where
                    OI.OP_ID    = pOpId     and
                    OI.OP_SUBID = pOpSubId  and
                    OI.CODE     = A.CODE    and
                    A.ANAL_ID   = pAnalId
                order by A.NAME;
    vSumId number;
begin
    for X in S loop
        vSumId:= X.ITEM_SUM_ID;
        return vSumId;    
    end loop;
    return 0;
end;
/
show errors
/
grant execute on RI to &1.ROLE;
grant execute on GetInpDate to &1.ROLE;
grant execute on GetLinkDb to &1.ROLE;
grant execute on GetAccSumId to &1.ROLE;
grant execute on GetAnSumByAnId to &1.ROLE;

exit;
