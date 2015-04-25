#include "AMS_HDR.h"
#pragma hdrstop

#include "amsclnt.h"

void AnalyzeRes(int res,char *text)
{
  if (res==R_SUMSDC) AMS_EXC("Суммы не совпадают",0) ;
  else if (res==R_DIVACC) AMS_EXC("Счет разбит на субсчета",0) ;
  else if (res==R_BALPOST) AMS_EXC("Балансовый счет не может использоваться для откладывания",0) ;
  else if (res==R_NORIGHTS) AMS_EXC("Недостаточно прав",0) ;
  else if (res==R_BADDATE) AMS_EXC("Дата операции вне допустимых пределов",0);
  else if (res==R_ANALSYNT) AMS_EXC("Не совпадает аналитика и синтетика по одному из счетов",0) ;
  else if (res==R_ANALITOGO) AMS_EXC("Итого строка в одном из счетов не равна сумме по позициям",0) ;
  else if (res==R_NEXTOPERS) AMS_EXC("Существуют следующие операции",0) ;
  else if (res==R_FIRSTOPER) ;
  else if (res==R_NATIVEDOC) AMS_EXC("Тип документа этой стандартной операции не может быть изменен",0) ;
  else if (res==R_OPERBUSY)
     {
      AnsiString str=AnsiString("Операция редактируется пользователем ")+text ;
      AMS_EXC(str,0) ;
     }
  else if (res==R_NEGREMS || res==R_NEGREMSOK)
  {
   AmsQuery Q ;
   Q.SetSqlText(" SELECT e.aarticle, e.aver, a.name, a.CODE FROM *.anal_errs e, "
                "*.accs a WHERE SID=:SID and e.code=a.code") ;
   Q.DeclVarInt("SID",G.audsid) ;
   Q.Exec("Информация об отриц. ост.") ;

   if (res==R_NEGREMS)
      AMS_EXC(AnsiString("Действие ведет к отрицательным остаткам")+"\n Счет "+
        TAccName::ToUserName(Q.GetFldStr(2))+" Артикул "+Q()->FieldAsString(0)+" верс:"+Q()->FieldAsString(1),0) ;
   else
      OkMessage(AnsiString("Предупреждение: действие привело к отрицательным остаткам")+"\n Счет "+
        TAccName::ToUserName(Q.GetFldStr(2))+" Артикул "+Q()->FieldAsString(0)+" верс:"+Q()->FieldAsString(1)) ;

   Q.SetPLText(" delete *.anal_errs where sid=:sid;") ;
   Q.Exec("Удаляем инф об отриц ост") ;
   DatMod->OSession->Commit() ;
  }
  else
  {
   AMS_EXC("Внутренняя ошибка \n"+AnsiString(text),0) ;
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
 Q.Exec("Удаление операции") ;
 int res=Q.GetVarInt("RESULT") ;
 if (res!=0)
 {
  AnalyzeRes(res,Q.GetVarStr("TEXT").data()) ;

  if (res==R_FIRSTOPER)
  {
   if (AskMessage("Удалить всю группу проводок")==ID_YES)
   {
    AmsQuery Q1 ;
    Q1.DeclVarInt("OP_ID",op_id) ;
    Q1.DeclVarInt("RESULT") ;
    Q1.SetPLText("*.bookkern.DeleteOperGroup(:OP_ID,:RESULT);") ;
    Q1.Exec("Удаление группы операций") ;
   }
   else AMS_EXC("Операция не удалена",-1) ;
  }
  else
   if (res!=R_NEGREMSOK)
     return 0 ;
 }

 // НЕОБХОДИМО ВНЕСТИ УДАЛЕНИЕ СТРОКИ СВЯЗАННОЙ БАЗЫ В PL/SQL

 return 1 ;
}

