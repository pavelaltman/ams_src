#ifndef CURR_PROG_HDR_H
#define CURR_PROG_HDR_h

#include "vector.h"

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AMS_TREE_VIEW.h"
#include "AmsObjCatView.h"
#include "Oracle.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <ToolWin.hpp>
#include "AmsStringGrid.h"
#include <ActnList.hpp>
#include <Menus.hpp>

//#define This *this
//#define Delete delete

typedef char* text;
#include "enums.h"
#include <string>
using std::string;
#include <stdio.h>
#include "vector.h"
#include "datetime.h"
#include "DAT_MOD.H"
#include "AMS_QUERY.h"
#include "orcpp.h"
#include "SUM.h"
#include "prog.H"
#include "accs.h"
#include "strlist.h"
#include "rctype.h"

inline AnsiString ToAnsiStr (const ::string& S) { return S.data();};
inline string ToString (const AnsiString S) { return S.c_str();};

#define GOOD 0
#define BAD  1
//#define DeleteA(a) if(a) delete [](a); a=0


inline void message(const char* Msg, int MsgType)
{
    if (MsgType== GOOD)
        Message (Msg);
    else
        ErrorMessage(Msg);
}

#include "FLDS_DESCR.h"

#include "FRM_EDIT_DB_POS.h"
#include "FRM_FIND_DB_POS2.h"
#include "FRM_SORT_ORDER.h"
#include "FRM_QUICK_SEARCH.h"
#include "FRM_SET_FLD.h"
#include "FRM_HIST_VIEW.h"
#include "FRM_ASK_ACC_CODE.h"

#include "FRM_BROWS_DB.h"
#include "FRM_HIST_BRWS.h"
#include "FRM_BROWS_AN.h"
#include "FRM_GET_SET_NAME.h"
#include "LOCAL_VARS.h"

#endif
