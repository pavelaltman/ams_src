//---------------------------------------------------------------------------
#include "AMS_HDR.H"
#pragma hdrstop
USERES("AMS_COMP2.res");
USERES("TREEVIEW.dcr");
USERES("INPUT_FLDS.dcr");
USEPACKAGE("vcl50.bpi");
USEPACKAGE("Vcldb50.bpi");
USEPACKAGE("doa.bpi");
USEUNIT("AMS_TREE_VIEW.cpp");
USEUNIT("AmsAccsView.cpp");
USEUNIT("..\COMMON\AMS_CONST.cpp");
USEUNIT("AmsObjCatView.cpp");
USEUNIT("AmsDbCatView.cpp");
USEUNIT("..\COMMON\AMS_EXC.cpp");
USEUNIT("AmsInputStr.cpp");
USEUNIT("AmsInputInt.cpp");
USEUNIT("AmsInputSum.cpp");
USEUNIT("AmsInputHistory.cpp");
USEFORM("FRM_INPUT_HISTORY.cpp", TFrmInputHistory);
USEUNIT("AmsInputDate.cpp");
USEUNIT("AmsInputDbPos.cpp");
USEUNIT("AmsInput.cpp");
USEUNIT("AmsInputBool.cpp");
USEUNIT("AmsStringGrid.cpp");
USEUNIT("AmsInputStrML.cpp");
USEUNIT("..\COMMON\AMS_UTILS.cpp");
USEUNIT("AmsDBGrid.cpp");
USEPACKAGE("RXDB5.bpi");
USEPACKAGE("Vclx50.bpi");
USEUNIT("AmsLogon.cpp");
USEUNIT("AmsQuery.cpp");
USEPACKAGE("doa34c5.bpi");
USEUNIT("AmsDataSet.cpp");
USEUNIT("..\COMMON\utils.cpp");
USEUNIT("AmsInpCond.cpp");
USEUNIT("AmsFooter.cpp");
USEUNIT("AmsInputAnal.cpp");
USEFORM("..\COMMON\FRM_INPUT_VALUE.cpp", FrmInputValue);
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//   Package source.
//---------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
  return 1;
}
//---------------------------------------------------------------------------
