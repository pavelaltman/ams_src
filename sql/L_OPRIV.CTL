LOAD DATA
INFILE 'opriv.txt'
REPLACE
INTO TABLE objpriv
FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"'
TRAILING NULLCOLS
(OID,RUNAME,RU,PR0,PR1,PR2,PR3,PR4,PR5)
