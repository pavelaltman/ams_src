SELECT name, value
FROM v$sysstat
WHERE name IN ('sorts (memory)', 'sorts (disk)') ;