//---------------------------------------------------------------------------

#ifndef FRM_BF_CH_POSH
#define FRM_BF_CH_POSH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "FRM_BUILD_AN_FUNC.h"
#include "Oracle.hpp"
#include <ExtCtrls.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TFrmBFChPos : public TFrmBuildAnFunc
{
__published:	// IDE-managed Components
protected:	// User declarations
    virtual int BuildThis(int BuildWhat);
public:		// User declarations
    __fastcall TFrmBFChPos(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmBFChPos *FrmBFChPos;
//---------------------------------------------------------------------------
#endif
