LOAD DATA
INFILE 'accs.txt'
REPLACE
INTO TABLE accs
FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"'
TRAILING NULLCOLS
(CODE,NAME,PARENT,DESCR,BAL,ANAL_ID,BAL_FLD)
