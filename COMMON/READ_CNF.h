//---------------------------------------------------------------------------

#ifndef READ_CNFH
#define READ_CNFH
//---------------------------------------------------------------------------
//----------------------------------------------------------------
// ��� ���� ����� ��������� ������������ �������� ������������
//----------------------------------------------------------------
class CConfig
{
   int argc;
   char ** argv;
   int LastArg;
   bool ReadFile;
   char Buf [250];
   FILE* F;
public:
   CConfig(int argc_, char* argv_[])
    {argc= argc_; argv= argv_; LastArg = 1; ReadFile=false;};
   char* Next();
   void BeginFile(const char* FileName)
   {
        if (ReadFile) return; // �� ����� �� ����� ������������ ��� ������
        F=fopen(FileName,"rt");
        if (!F) return; // �� ���� ������� ���� ������������
        ReadFile=true;
   };
};
//----------------------------------------------------------------
#endif
