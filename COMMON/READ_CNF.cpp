//---------------------------------------------------------------------------

#ifdef AMS_PROJ
#include "AMS_HDR.h"
#pragma hdrstop
#else
#include <vcl.h>
#pragma hdrstop
#endif

#include "READ_CNF.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
char* CConfig::Next()
{
    if (!ReadFile)
    {
        if (LastArg>=argc)
        {
            return NULL;
        }
        else
        {
            return argv[LastArg++];
        }
   }
   else
   {
        while (!feof(F))
        {
            memset(Buf,0, 200);
            char* L = fgets(Buf, 199, F);
            if (L && (L[0]!='#')&&(L[0]!='\n'))
            {
                int Len = strlen(L);
                char* Back = &L[Len-1];
                // Trunc left
                while ((
                        (*L==' ')||(*L=='\t')
                        ) && (*L!=0)
                      ) L++ ;
                // Trunc Right
                while ( (
                        (*Back=='\n')||(*Back==' ')||(*Back=='\t')
                        ) && (*Back!=0)
                    ) Back--;
                Back[1]=0;
                if (strlen(L)>0) return L;
            }
    } // Пока не конец файла
    fclose(F);
    ReadFile= false;
    return Next();
   } // Если конфигурация считывается из файла
}
//----------------------------------------------------------------
