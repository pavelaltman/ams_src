//---------------------------------------------------------------------------

#ifndef GET_DB_POSH
#define GET_DB_POSH
//---------------------------------------------------------------------------
int GetDbPos (int DbId,  AnsiString& Article, int& Ver, const AnsiString& SetName);
int GetDbPos (int DbId,  int& WithOutVer_, AnsiString& Article, int& Ver, const AnsiString& SetName);
//---------------------------------------------------------------------------
#endif
