SELECT SUM(gets) "Data Dictionary Gets",
SUM(getmisses) "Data Dictionary Misses"
FROM v$rowcache;