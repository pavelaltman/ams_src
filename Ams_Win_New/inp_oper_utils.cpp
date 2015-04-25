#include "AMS_HDR.h"
#pragma hdrstop

#include "amsclnt.h"

void AnalyzeRes(int res,char *text)
{
  if (res==R_SUMSDC) AMS_EXC("����� �� ���������",0) ;
  else if (res==R_DIVACC) AMS_EXC("���� ������ �� ��������",0) ;
  else if (res==R_BALPOST) AMS_EXC("���������� ���� �� ����� �������������� ��� ������������",0) ;
  else if (res==R_NORIGHTS) AMS_EXC("������������ ����",0) ;
  else if (res==R_BADDATE) AMS_EXC("���� �������� ��� ���������� ��������",0);
  else if (res==R_ANALSYNT) AMS_EXC("�� ��������� ��������� � ��������� �� ������ �� ������",0) ;
  else if (res==R_ANALITOGO) AMS_EXC("����� ������ � ����� �� ������ �� ����� ����� �� ��������",0) ;
  else if (res==R_NEXTOPERS) AMS_EXC("���������� ��������� ��������",0) ;
  else if (res==R_FIRSTOPER) ;
  else if (res==R_NATIVEDOC) AMS_EXC("��� ��������� ���� ����������� �������� �� ����� ���� �������",0) ;
  else if (res==R_OPERBUSY)
     {
      AnsiString str=AnsiString("�������� ������������� ������������� ")+text ;
      AMS_EXC(str,0) ;
     }
  else if (res==R_NEGREMS || res==R_NEGREMSOK)
  {
   AmsQuery Q ;
   Q.SetSqlText(" SELECT e.aarticle, e.aver, a.name, a.CODE FROM *.anal_errs e, "
                "*.accs a WHERE SID=:SID and e.code=a.code") ;
   Q.DeclVarInt("SID",G.audsid) ;
   Q.Exec("���������� �� �����. ���.") ;

   if (res==R_NEGREMS)
      AMS_EXC(AnsiString("�������� ����� � ������������� ��������")+"\n ���� "+
        TAccName::ToUserName(Q.GetFldStr(2))+" ������� "+Q()->FieldAsString(0)+" ����:"+Q()->FieldAsString(1),0) ;
   else
      OkMessage(AnsiString("��������������: �������� ������� � ������������� ��������")+"\n ���� "+
        TAccName::ToUserName(Q.GetFldStr(2))+" ������� "+Q()->FieldAsString(0)+" ����:"+Q()->FieldAsString(1)) ;

   Q.SetPLText(" delete *.anal_errs where sid=:sid;") ;
   Q.Exec("������� ��� �� ����� ���") ;
   DatMod->OSession->Commit() ;
  }
  else
  {
   AMS_EXC("���������� ������ \n"+AnsiString(text),0) ;
  }
}

int DelOper(int op_id,int op_subid)
{
 AmsQuery Q ;
 Q.DeclVarInt("OP_ID",op_id) ;
 Q.DeclVarInt("OP_SUBID",op_subid) ;
 Q.DeclVarInt("EDITED",0) ;
 Q.DeclVarInt("RESULT") ;
 Q.DeclVarStr("TEXT") ;
 Q.SetPLText("*.bookkern.DeleteOper(:OP_ID,:OP_SUBID,:EDITED,:RESULT,:TEXT);") ;
 Q.Exec("�������� ��������") ;
 int res=Q.GetVarInt("RESULT") ;
 if (res!=0)
 {
  AnalyzeRes(res,Q.GetVarStr("TEXT").data()) ;

  if (res==R_FIRSTOPER)
  {
   if (AskMessage("������� ��� ������ ��������")==ID_YES)
   {
    AmsQuery Q1 ;
    Q1.DeclVarInt("OP_ID",op_id) ;
    Q1.DeclVarInt("RESULT") ;
    Q1.SetPLText("*.bookkern.DeleteOperGroup(:OP_ID,:RESULT);") ;
    Q1.Exec("�������� ������ ��������") ;
   }
   else AMS_EXC("�������� �� �������",-1) ;
  }
  else
   if (res!=R_NEGREMSOK)
     return 0 ;
 }

 // ���������� ������ �������� ������ ��������� ���� � PL/SQL

 return 1 ;
}

