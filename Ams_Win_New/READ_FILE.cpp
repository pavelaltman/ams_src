//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "READ_FILE.h"
extern const char* unknownfuncerr;

//---------------------------------------------------------------------------

#pragma package(smart_init)
VReadFile::VReadFile(const string& FileName_):FileName(FileName_),
    File(FileName_.data())
{
};
//---------------------------------------------------------------------------
StackItem VReadFile::Method(Node &node,::string const &method,int pc,StackItem *stk)
{
    if(method=="GOOD" && !pc)
    {
        int Res = File.good();
        return StackItem(Res);
    }
    if(method=="NOTEOF" && !pc)
    {
        int Res = File.good() && !File.bad() && !File.eof();
        return StackItem(Res);
    }
    if(method=="GETSTR" && !pc)
    {
        string S="";
        if (File.good() && !File.bad() && !File.eof())
        File>>S;
        return StackItem(S);
    }
    if(method=="GETSTRWHILENOT" && pc==1)
    {
        string TermStr=stk[1-pc].String();
        string Res="";
        bool More= true;
        while (File.good() && !File.bad() && !File.eof() && More)
        {
            char S[2];
            char& C= S[0];
            S[0]=0;
            S[1]=0;
            File.get(C);
            if (strchr(TermStr.data(), C))
            {
                More=false;
            }
            else
            {
                Res=Res+S;
            }
        }
        return StackItem(Res);
    }
    if(method=="GETLINE" && !pc)
    {
        string Res;
        if (File.good() && !File.bad() && !File.eof())
        {
            char S[10000];
            char* P=S;
            File.getline(P, 9999);
            int Len=strlen(S);
            if (S[Len-1]=='\n')
            {
                S[Len-1]=0;
            }
            Res=S;
        }
        return StackItem(Res);
    }
    prog_ptr->RTError(101,::string(unknownfuncerr)+"  "+method);
    return StackItem();
}
//---------------------------------------------------------------------------
VReadFile::~VReadFile()
{
    File.close();
};
//---------------------------------------------------------------------------

