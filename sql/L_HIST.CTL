LOAD DATA
INFILE 'hist.txt'
REPLACE
INTO TABLE history
FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"'
(OP_ID,OP_SUBID,REVISION,REV_TIME DATE "DD-MM-YY HH24:MI",USERNAME)
