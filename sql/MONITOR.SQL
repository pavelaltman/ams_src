SELECT MAX(USERNAME),o.SID,COUNT(*) FROM UNAPP_OPERS o,V$SESSION s
  WHERE o.SID=s.AUDSID
  GROUP BY o.SID ;
