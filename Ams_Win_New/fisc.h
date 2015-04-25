#ifndef FISC_H__
#define FISC_H__

extern "C" int __declspec(dllexport) SetComPort(int Numb, int Speed=4800);
extern "C" int __declspec(dllexport) CloseComPort(int Numb);
extern "C" int __declspec(dllexport) ResetCashes(void);
extern "C" int __declspec(dllexport) IsCash(int NumCash);
extern "C" int __declspec(dllexport) StrPrntCash(int NumCash,char* str1,
                                       char * str2=NULL,
                                       char * str3=NULL,
                                       char * str4=NULL,
                                       char * str5=NULL);
extern "C" int __declspec(dllexport) SellArtCash(int NumCash,char * Art);
extern "C" int __declspec(dllexport) GetMode(int NumCash);
extern "C" int __declspec(dllexport) SetMode(int NumCash, int Mode);
extern "C" int __declspec(dllexport) CloseReceipt(int Numb);
extern "C" char*  __declspec(dllexport) GetVerDLL();
extern "C" int __declspec(dllexport) GetLicence();
//extern "C" int __declspec(dllexport) Payment(int NumCash,int TypeOfMoney,double Summa);

#endif

