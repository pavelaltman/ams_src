SELECT name, value
FROM v$sysstat
WHERE name IN ('db block gets', 'consistent gets','physical reads');