SET DIR_LETTER=C
@echo off
REM ------------------------------------------------------------------
set AMS_ROOT=%DIR_LETTER%:\AMSORA
md %AMS_ROOT%
ECHO  ********************************** >  %AMS_ROOT%\DIRINFO
ECHO  *    Корневая директория для AMS * >> %AMS_ROOT%\DIRINFO
ECHO  *          Клиент-Сервер         * >> %AMS_ROOT%\DIRINFO 
ECHO  ********************************** >> %AMS_ROOT%\DIRINFO
md %AMS_ROOT%\client
ECHO  ********************************** >  %AMS_ROOT%\client\DIRINFO
ECHO  * Все файлы для запуска клиента  * >> %AMS_ROOT%\client\DIRINFO
echo  *        и администратора        * >> %AMS_ROOT%\client\DIRINFO
ECHO  *      *.exe, *.dll, *.ini,      * >> %AMS_ROOT%\client\DIRINFO
echo  *     *.bat для запуска exe      * >> %AMS_ROOT%\client\DIRINFO
ECHO  ********************************** >> %AMS_ROOT%\client\DIRINFO
md %AMS_ROOT%\arch
ECHO  ********************************** >  %AMS_ROOT%\arch\DIRINFO
ECHO  *    Чево скачали или принесли   * >> %AMS_ROOT%\arch\DIRINFO
ECHO  *      *.rar, *.zip              * >> %AMS_ROOT%\arch\DIRINFO
ECHO  ********************************** >> %AMS_ROOT%\arch\DIRINFO
md %AMS_ROOT%\save
ECHO  ********************************** >  %AMS_ROOT%\save\DIRINFO
ECHO  *  Тут будут хранится архивные   * >> %AMS_ROOT%\save\DIRINFO
ECHO  *      копии данных программы    * >> %AMS_ROOT%\save\DIRINFO
ECHO  *           *.dmp, *.rar         * >> %AMS_ROOT%\save\DIRINFO
ECHO  ********************************** >> %AMS_ROOT%\save\DIRINFO
md %AMS_ROOT%\sql
ECHO  *************************************** >  %AMS_ROOT%\sql\DIRINFO
ECHO  *         Тут хранятся SQL скрипты    * >> %AMS_ROOT%\sql\DIRINFO
ECHO  *   *.sql, *.bat для запуска скриптов * >> %AMS_ROOT%\sql\DIRINFO
ECHO  *************************************** >> %AMS_ROOT%\sql\DIRINFO
md %AMS_ROOT%\forms
ECHO  ********************************** >  %AMS_ROOT%\forms\DIRINFO
ECHO  *        Тут хранятся формы      * >> %AMS_ROOT%\forms\DIRINFO
ECHO  *         если это сервер        * >> %AMS_ROOT%\forms\DIRINFO
ECHO  *       *.prg, *.xls, *.lib      * >> %AMS_ROOT%\forms\DIRINFO
ECHO  ********************************** >> %AMS_ROOT%\forms\DIRINFO
md %AMS_ROOT%\DOC
ECHO  ********************************** >  %AMS_ROOT%\DOC\DIRINFO
ECHO  * Тут хранятся файлы документации* >> %AMS_ROOT%\DOC\DIRINFO
ECHO  *           *.doc, *.hlp         * >> %AMS_ROOT%\DOC\DIRINFO
ECHO  ********************************** >> %AMS_ROOT%\DOC\DIRINFO
echo on
copy SQL.rar %AMS_root%\sql 
%dir_letter%:
cd %AMS_ROOT%\sql
rar x %AMS_root%\sql\sql.rar  
