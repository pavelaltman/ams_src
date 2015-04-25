CREATE role AMS_BUCH_ROLE; 
GRANT select, insert,update, delete on AMS.SYS_INFO to AMS_BUCH_ROLE;
GRANT select, insert,update, delete on AMS.LICENSES to AMS_BUCH_ROLE;
GRANT select, insert,update, delete on AMS.BOOKS to AMS_BUCH_ROLE;

CREATE role AMS_USER; 
GRANT select on AMS.SYS_INFO to AMS_USER;
GRANT select on AMS.LICENSES to AMS_USER;
GRANT select on AMS.BOOKS to AMS_USER;
exit;
