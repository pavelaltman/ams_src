REM Для удаления интервалов 
REM
REM %1 Имя пользователя
REM %2 Пароль
REM %3 Дата начала остающегося промежутка
REM %4 Дата конца  остающегося промежутка

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
