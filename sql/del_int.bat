REM ��� 㤠����� ���ࢠ��� 
REM
REM %1 ��� ���짮��⥫�
REM %2 ��஫�
REM %3 ��� ��砫� ����饣��� �஬���⪠
REM %4 ��� ����  ����饣��� �஬���⪠

if A%1== A goto END_VAL 
if A%2== A goto END_VAL 
if A%3== A goto END_VAL 
if A%4== A goto END_VAL 

call sql %1 %2 del_int %3 %4  
call sql %1 %2 del_int %3 %4  
call sql %1 %2 del_int %3 %4  
call sql %1 %2 del_int %3 %4  

call sql %1 %2 MAKE_QUICK_TURNS
:END_VAL
