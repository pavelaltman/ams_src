SELECT SUM(pins) "Executions", SUM(reloads) "Cache Misses while Executing"
FROM v$librarycache;
