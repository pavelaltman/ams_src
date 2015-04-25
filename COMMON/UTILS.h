//---------------------------------------------------------------------------

#ifndef UTILSH
#define UTILSH
//---------------------------------------------------------------------------
// Для преобразования номера колонки Excel в ее букв представление
extern char* XLSC(int ColNum, // Номер колонки
                  char *Buf); // Куда писать результат работы

// Заменяет комбинацию "*." на "ИМЯ_БАЗОВОГО_ПОЛЬЗОВАТЕЛЯ." в
// тексте SQL запроса
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
// С тремя кнопками
extern  int  AskMessage3  (const AnsiString&, bool DefNo= true);
//---------------------------------------------------------------------------
extern  int AskString  (const AnsiString& Prompt, int MaxLen, AnsiString& ResVal);
extern  int AskInt     (const AnsiString& Prompt, int& DefaultVal);
extern  int AskDate    (const AnsiString& Prompt, TDateTime& ResVal);
extern  int AskSum     (const AnsiString& Prompt, double& ResVal);
//---------------------------------------------------------------------------
// Выполняет преобразование из строки в другие типы с учетом ввода
// пользователем
extern  double     GetSum (const AnsiString& Val, int ShowErr=0);
extern  TDateTime  GetDate(const AnsiString& Val, int ShowErr=0);
// Преобразование І,і украинского языка в английский I, i
extern bool ToEnglI;
//---------------------------------------------------------------------------
#endif
