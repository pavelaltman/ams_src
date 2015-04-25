//---------------------------------------------------------------------------

#ifndef THREAD_READ_FILEH
#define THREAD_READ_FILEH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
const int ReadBufLen = 8196;
class TThreadReadFile : public TThread
{
private:
protected:
    void __fastcall Execute();
    AnsiString FileName;
    TStrings* StrList;
    AnsiString NextLines;
    char Buf [ReadBufLen+1];
    void __fastcall UpdMemo();
public:
    __fastcall TThreadReadFile(
        const AnsiString& FileName_,
        TStrings* StrList_,
        bool CreateSuspended=false);
};
//---------------------------------------------------------------------------
#endif
