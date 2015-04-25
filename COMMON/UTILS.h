//---------------------------------------------------------------------------

#ifndef UTILSH
#define UTILSH
//---------------------------------------------------------------------------
// ��� �������������� ������ ������� Excel � �� ���� �������������
extern char* XLSC(int ColNum, // ����� �������
                  char *Buf); // ���� ������ ��������� ������

// �������� ���������� "*." �� "���_��������_������������." �
// ������ SQL �������
extern AnsiString ReplaceBaseUser (const AnsiString& Sql);

class Sum ;
int CreateSum(Sum &value) ;
int NewOpId() ;
//---------------------------------------------------------------------------
extern  void InitErrors  (TApplication* Appl_);
extern  void ErrorMessage(const AnsiString& );
extern  void OkMessage   (const AnsiString& );
extern  void Message     (const AnsiString& );
extern  int  AskMessage  (const AnsiString&, bool DefNo= true);
extern  int  AskQuestion (const AnsiString&);
// � ����� ��������
extern  int  AskMessage3  (const AnsiString&, bool DefNo= true);
//---------------------------------------------------------------------------
extern  int AskString  (const AnsiString& Prompt, int MaxLen, AnsiString& ResVal);
extern  int AskInt     (const AnsiString& Prompt, int& DefaultVal);
extern  int AskDate    (const AnsiString& Prompt, TDateTime& ResVal);
extern  int AskSum     (const AnsiString& Prompt, double& ResVal);
//---------------------------------------------------------------------------
// ��������� �������������� �� ������ � ������ ���� � ������ �����
// �������������
extern  double     GetSum (const AnsiString& Val, int ShowErr=0);
extern  TDateTime  GetDate(const AnsiString& Val, int ShowErr=0);
// �������������� �,� ����������� ����� � ���������� I, i
extern bool ToEnglI;
//---------------------------------------------------------------------------
#endif
