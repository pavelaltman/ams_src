#define MAXSTMTLEN 2000
#include "vctr.h"
#ifndef  _PROGPIPE
#define _PROGPIPE

struct ProgPos {
  int line;
  ::string file;
};

class ProgPipe {
public:
 virtual int Open(ProgPos &PP) =0;
 virtual int GetLine(char *line,ProgPos &PP) =0;
 virtual void Close() =0;
 virtual ~ProgPipe() {}
};

class ProgStr:public ProgPipe {
 ::string str;
 const char *c;
 int done;
public:
 int Init(const char *_s) { str=_s;return 0; }
 int Open(ProgPos &PP) { PP.file="."; PP.line=0; c=str.data();return 1; }
 int GetLine(char *line,ProgPos &PP);
 void Close() { }
};

class ProgFile:public ProgPipe {
 FILE *ifile;
 char fname[MAXSTMTLEN];
 char *tail;
public:
 ProgFile() { ifile=0; }
 int Init(char *_fname) { strcpy(fname,_fname); return 0;}
 int Open(ProgPos &PP);
 int GetLine(char *line,ProgPos &PP);
 void Close() { if(ifile) fclose(ifile); ifile=0; }
 ~ProgFile() { Close(); }
};

class ProgMem:public ProgPipe {
 gcl::vector<::string> strs;
 int count;
public:
 void Add(::string const &s) { strs.push_back(s); }
 void Add(char const *s)   { strs.push_back(s); }
 int Open(ProgPos &PP) { PP.file="."; PP.line=count=0;return 0; }
 int GetLine(char *line,ProgPos &PP) ;
 void Close() { }
};

#endif

