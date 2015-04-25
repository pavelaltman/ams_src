//---------------------------------------------------------------------------
#include "AMS_HDR.h"
#pragma hdrstop
USERES("AmsWinAdmin.res");
USEFORM("MAIN_FRM.cpp", MainFrm);
USEFORM("Frm_Cat_Tree.cpp", FrmCatTree);
USEFORM("FRM_ACCS.cpp", FrmAccs);
USEFORM("FRM_ACCS_STR.cpp", FrmAccsStr);
USEUNIT("..\COMMON\AMS_UTILS.cpp");
USEFORM("FRM_COPY_STD_OP.cpp", FrmCopyStdOp);
USEUNIT("..\COMMON\AMS_EXC.cpp");
USEUNIT("copy_cond_iTEMS.cpp");
USEFORM("FRM_CONDS_EDIT.cpp", FrmCondsEdit);
USEUNIT("..\COMMON\AMS_CONST.cpp");
USEFORM("..\COMMON\FRM_INPUT_VALUE.cpp", FrmInputValue);
USEFORM("FRM_ACCS_TIES.cpp", FrmAccsTies);
USEFORM("FRM_BUILD_BUSY_PROG.cpp", FrmBuildBusyProg);
USEFORM("FRM_BUILD_AN_FUNC.cpp", FrmBuildAnFunc);
USEFORM("FRM_GIVE_RIGHTS.cpp", FrmGiveRights);
USEFORM("FRM_GEN_SETUP.cpp", FrmGenSetup);
USEUNIT("..\COMMON\AMS_READ_CNF_UTIL.cpp");
USEUNIT("..\COMMON\AMS_READ_CNF.cpp");
USEFORM("FRM_SET_AN_FLD_ORDER.cpp", FrmSetAnFldOrder);
USEUNIT("..\COMMON\AMS_QUERY.cpp");
USEFORM("ADMIN_DAT_MOD.cpp", DatMod); /* TDataModule: File Type */
USEFORM("..\COMMON\FRM_GET_SET_NAME.cpp", FrmGetSetName);
USEUNIT("..\COMMON\FLDS_DESCR.cpp");
USEFORM("FRM_CHOOSE_OBJ_ADM.cpp", FrmChooseObj);
USEUNIT("..\COMMON\utils.cpp");
USEFORM("FRM_COPY_TREE.cpp", FrmCopyTree);
USEFORM("FRM_CHOOSE_OBJ_A_DB.cpp", FrmChooseObjADb);
USEUNIT("AMS_ADM_UTILS.cpp");
USEFORM("FRM_EDIT_GLOBAL_SETUP.cpp", FrmEditGlobalSetup);
USEFORM("FRM_EDIT_BUCH_SETUP.cpp", FrmEditBuchSetup);
USEFORM("FRM_INPUT_PASS.cpp", FrmInputPass);
USEFORM("FRM_SEL_CPS_DATE.cpp", FrmSelCpsDate);
//---------------------------------------------------------------------------
#include "AMS_READ_CNF_UTIL.h"
//---------------------------------------------------------------------------

WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR CmdLine_, int)
{
  try
  {
     putenv("NLS_LANG=.CL8MSWIN1251");
     Application->Initialize();
     InitCnf (CmdLine_);
     Application->Title = "Â ïîìîùü AMS Adminó";
     Application->CreateForm(__classid(TMainFrm), &MainFrm);
         Application->CreateForm(__classid(TDatMod), &DatMod);
         Application->CreateForm(__classid(TFrmInputPass), &FrmInputPass);
         Application->Run();
  }
  catch (Exception &exception)
  {
     Application->ShowException(&exception);
  }
  return 0;
}
//---------------------------------------------------------------------------
