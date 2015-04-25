//---------------------------------------------------------------------------

#ifndef AmsInputPointH
#define AmsInputPointH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "AmsInputAnal.h"
#include "AmsInputDbPos.h"
#include "AmsInputStr.h"
#include "AmsInput.h"
//---------------------------------------------------------------------------
class PACKAGE TAmsInputPoint : public TAmsInputAnal
{
private:
protected:
public:
    virtual void Clear(){AmsAnalInfo="Не определено";AmsSumId=0;};
    __fastcall TAmsInputPoint(TComponent* Owner);
__published:
};
//---------------------------------------------------------------------------
#endif
