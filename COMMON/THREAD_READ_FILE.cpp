//---------------------------------------------------------------------------
#include "AMS_HDR.H"
#pragma hdrstop

#include "THREAD_READ_FILE.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TThreadReadFile::TThreadReadFile(const AnsiString& FileName_,
        TStrings* StrList_,
        bool CreateSuspended)
    : TThread(CreateSuspended), FileName(FileName_), StrList (StrList_)
{
    FreeOnTerminate = true;
}
//---------------------------------------------------------------------------
void __fastcall TThreadReadFile::Execute()
{
    extern char DirBuff [200];
    AnsiString Res= FileName;
    ifstream FS (Res.c_str());
    int i = 0;
    NextLines = "";
    while (!FS.bad() && !FS.eof() && FS.good() && !Terminated)
    {
        FS.getline(Buf, ReadBufLen);
        NextLines+=Buf;
        if (++i==50)
        {
            if (!Terminated)
                Synchronize(UpdMemo);
            i=0;
            NextLines = "";
        }
        else
            NextLines+="\r\n";
    }
    if (!Terminated)
        Synchronize(UpdMemo);
    FS.close();

    while (!Terminated)
    {
        Sleep(1000);
    }
    
}
//---------------------------------------------------------------------------
void __fastcall TThreadReadFile::UpdMemo()
{
    StrList->Add(NextLines);
}