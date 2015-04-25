ALTER SESSION SET nls_date_language=RUSSIAN ;
ALTER SESSION SET NLS_DATE_FORMAT = 'DD-MM-YY' ;
set serveroutput on

declare
 res NUMBER ;
 nl  NUMBER ;
 root NUMBER ;
begin

 -- Начальный узел иерархии
 root:=NULL ;
 InsertCat(NULL,'/',root,res) ;
 dbms_output.put_line(res);
 dbms_output.put_line(root);

 -- добавление документа '-'
 nl:=NULL ;
 InsertObj(root,1,'-','',nl,res) ;
 dbms_output.put_line(res);
 dbms_output.put_line(nl);

 periods.CreateFirstDate('1-1-2001') ;

end ;
/
exit
