
#ifndef DATE_TIME_H__
#define DATE_TIME_H__

#include <time.h>
#include "usedef.h"
#include "mem.h"

#define DATE_LEN 8

class DateTime  {
    TDateTime DTVal;
public:
      void Zero()
      {DTVal;};
      DateTime() { Zero(); }
      DateTime(int d,int m,int y) { Set(d,m,y); }
      DateTime ( const TDateTime& DT) { DTVal = DT;};
      //DateTime(time_t const &tt) {tm* TM=localtime(&tt);Set(TM->tm_mday,TM->tm_mon,TM->tm_year); }
      //operator time_t() const { return mktime((tm *)this); }
      //time_t   sec() const    { return time_t(this); }
      //DateTime &operator=(time_t const &tt) { /*tm* TM=localtime(&tt);Set(TM->tm_mday,TM->tm_mon,TM->tm_year);*/return *this;};
      DateTime ChMonth(int pmn) ;
      DateTime NextMonth() { return ChMonth(1);  }
      DateTime PrevMonth() { return ChMonth(-1); }
      DateTime ChDay(int pmn) const {DateTime D(DTVal); D.DTVal+=pmn; return D;}
      DateTime NextDay() const { return ChDay(1);  }
      DateTime PrevDay() const { return ChDay(-1); }
      int      Cmp(DateTime const &dt) const { return DTVal-dt.DTVal ; }
      int      operator==(DateTime const &dt) { return Cmp(dt)==0; }
      int      operator!=(DateTime const &dt) { return Cmp(dt)!=0; }
      int      operator<=(DateTime const &dt) { return Cmp(dt)<=0; }
      int      operator>=(DateTime const &dt) { return Cmp(dt)>=0; }
      int      operator<(DateTime const &dt) { return Cmp(dt)<0; }
      int      operator>(DateTime const &dt) { return Cmp(dt)>0; }
      char     *Out(char *buf,int bs,char *fmt=NULL) const;

      TDateTime T() { return DTVal;};
      DateTime &operator=(const TDateTime& T)
      { DTVal=T;return *this;};
      int SetStr(const char *str);
      int SetStr(const char *str,int y);
      int SetStr(const char *str,int m,int y);
      void Set(int d,int m,int y);
      int Month() const
      {unsigned short y, m, d; DTVal.DecodeDate(&y, &m, &d); return m;};
      int Year() const
      {unsigned short y, m, d; DTVal.DecodeDate(&y, &m, &d); return y % 100;};
      int FYear() const
      {unsigned short y, m, d; DTVal.DecodeDate(&y, &m, &d); return y;};
      int Day() const
      {unsigned short y, m, d; DTVal.DecodeDate(&y, &m, &d); return d;};
      char *GetMonStr(char *buf,int no_year,int lang);
      char *GetTextStr(char *buf,int lang,int date_quote);
      char *IntervalStr(char *buf,DateTime const &to,int lang,int no_year);
};

inline int mon_bet(DateTime sd,DateTime ed)
{ return (ed.FYear()-sd.FYear())*12+(ed.Month()-sd.Month()) ; }

#endif
