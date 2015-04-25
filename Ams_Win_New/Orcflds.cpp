#include "AMS_HDR.h"
#pragma hdrstop

#include "orcpp.h"
#include <stdlib.h>

extern r_char toolongintmes , toomuchsummes,badsummes,baddatemes ;

// ::string
template <>
VTYPE Field<::string>::vtype() { return gcl::VT_STRING; }

template <>
void Field<::string>::SetVal (TAmsInput* AI)
{
    if (isNull()) AI->Clear();
    else
    AI->SetDefaultVal(ToAnsiStr(val));
}

template <>
void Field<::string>::SetVal (TField* Fld)
{
    if (isNull()) Fld->Clear();
    else
        Fld->AsString = ToAnsiStr(val);
}
template <>
void Field<::string>::GetVal(TField* Fld)
{
    if (Fld->IsNull)
    {
        SetNull();
    }
    else
    {
        SetNotNull();
        val=Fld->AsString.c_str();
    }
}



template <>
void Field<::string>::SetFld (AmsQuery& AQ, int FldNo)
{
    if (AQ.IsFldNull(FldNo)) SetNull();
    else
        (*this)=ToString(AQ.GetFldStr(FldNo));
}

template <>
AnsiString Field<::string>::AsString (const AnsiString& Format)
{
    return ToAnsiStr(val);
}

template <>
void Field<::string>::GetResult (TAmsInput* AI)
{
    TAmsInputStr * AID = dynamic_cast<TAmsInputStr*> (AI);
    if (AID && !AI->AmsIsNull)
        *this=ToString(AID->GetResultStr());
    else
        SetNull();
};



// Устанавливает OraField из Датасета, Флд Ном- номер поля внутри датасета
template <>
void Field<::string>::SetField(TOracleDataSet* ODS, int FldNo)
{
    null = 0;
    TField* Fld = ODS->Fields->Fields[FldNo];
    if (Fld->IsNull)
    {
        val="";
        SetNull();
    }
    else
        val = ToString(Fld->AsString);
};
// Уст поля датасета из OraFielda
template <>
void Field<::string>::SetDS(TOracleDataSet* ODS, int FldNo)
{
    TField* Fld = ODS->Fields->Fields[FldNo];
    if (isNull())
    {
        Fld->Clear();
    }
    else
    {
       Fld->AsString=ToAnsiStr(val);
    }
};


template <>
void Field<::string>::Buf2Field(ub1 *buf)
{ val=(char *)buf; null=0; }

/*template <>
void Field<::string>::Field2Buf(ub1 *buf) const
{ strcpy((char *)buf,val.data()); }
*/

template <>
void Field<::string>::_SetNull() { val=""; }

#pragma argsused
template <>
char *Field<::string>::Output(char *buf,int flags,int num_dig,int pchar)
{
 return strcpy(buf,val.data()) ;
}

#pragma argsused
template <>
char *Field<::string>::OutputPval(char *buf,int side,int flags,int num_dig,int pchar)
{
 if (side==DEBET) strcpy(buf,val.data()) ;
 else *buf=0 ;
 return buf ;
}

#pragma argsused
template <>
int Field<::string>::Input(const char *buf,int num_dig,char *errmes)
{
 val=buf ;
 null=0;
 return 1 ;
}

template <>
int Field<::string>::DefAlign() { return SA_LEFT ; }
template <>
int Field<::string>::TSize(int len) { return len ; }
template <>
Sum Field<::string>::Number() { return Sum(0) ; }
//--------------------------------------------------------------
/*
template <>
void Field<::string>::Field2Buf (cursor& Curs, FieldLink& FL )
{
    (*this)= ToString(Curs()->FieldAsString(FL.fldNum));
};
*/

template <>
void Field<::string>::Buf2Field (AmsQuery& Curs, FieldLink& FL )
{
    if (FL.FldNum==-1)
    {
        FL.FldNum = Curs()->FieldIndex((char*)FL.name);
    };
    if (Curs()->FieldIsNull(FL.FldNum))
    {
        null = 1;
        val = "";
    }
    else
    {
        (*this)= ToString(Curs()->FieldAsString(FL.FldNum));
        null=0;
    };
};
//--------------------------------------------------------------
template <>
void Field<::string>::DeclVar (AmsQuery& Curs, const AnsiString& Val)
{
    Curs.DeclVarStr(Val);
}
//--------------------------------------------------------------
template <>
void Field<::string>::GetVar (AmsQuery& Curs, const AnsiString& Val)
{
    (*this)= ToString(Curs.GetVarStr(Val));
};
//--------------------------------------------------------------
template <>
void Field<::string>::SetVar (AmsQuery& Curs, const AnsiString& Val)
{
    Curs.SetVar(Val, ToAnsiStr(val));
};
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------

// int
template <>
VTYPE Field<int>::vtype() { return gcl::VT_INT; }

template <>
void Field<int>::Buf2Field(ub1 *buf)
{ val=*(int *)buf; null=0; }
/*
template <>
void Field<int>::Field2Buf(ub1 *buf) const
{ *(int *)buf=val; }
*/
template <>
void Field<int>::_SetNull() { val=0; }

#pragma argsused
template <>
char *Field<int>::Output(char *buf,int flags,int num_dig,int pchar)
{
 if (val)
   sprintf(buf,"%-d",(int)val) ;
 else
 {
  if (flags&GS_NOZERO)
     *buf=0 ;
  else strcpy(buf,"0") ;
 }
 return buf ;
}

#pragma argsused
template <>
char *Field<int>::OutputPval(char *buf,int side,int flags,int num_dig,int pchar)
{
 int i=(int)val ;
 if (side==DEBET) if (i>0) sprintf(buf,"%-d",i) ; else *buf=0 ;
 else if (i<0) sprintf(buf,"%-d",-i) ; else *buf=0 ;
 return buf ;
}

#pragma argsused
template <>
int Field<int>::Input(const char *buf,int num_dig,char *errmes)
{
 if (strlen(buf)>8)
 {
  if (errmes) strcpy(errmes,toolongintmes) ;
  return 0 ;
 }
 val=atoi(buf) ;
 if (val>99990000)
 {
  if (errmes) strcpy(errmes,toolongintmes) ;
  return 0 ;
 }
 null=0;
 return 1 ;
}

template <>
void Field<int>::SetVal (TAmsInput* AI)
{
    if (isNull()) AI->Clear();
    else
    AI->SetDefaultVal(Val());
}

template <>
void Field<int>::SetVal (TField* Fld)
{
    if (isNull()) Fld->Clear();
    else
        Fld->AsInteger = val;
}
template <>
void Field<int>::GetVal(TField* Fld)
{
    if (Fld->IsNull)
    {
        SetNull();
    }
    else
    {
        if (Fld->DataType==ftBoolean)
            val=Fld->AsBoolean;
        else
            val=Fld->AsInteger;
        SetNotNull();
    }
}


template <>
void Field<int>::SetFld (AmsQuery& AQ, int FldNo)
{
    if (AQ.IsFldNull(FldNo)) SetNull();
    else
        (*this)=AQ.GetFldInt(FldNo);
}

template <>
AnsiString Field<int>::AsString (const AnsiString& Format)
{
    if (val)
        return IntToStr(val);
    else
        return "";
}


template <>
void Field<int>::AddVal (TField* Fld, bool OldVal)
{
    if (isNull())
    {
        if ( !(!OldVal && Fld->IsNull) && !(OldVal && Fld->OldValue.IsNull()) )
        {
            (*this)=OldVal?(int)Fld->OldValue:Fld->AsInteger;
        }
    }
    else
    {
        if ( !(!OldVal && Fld->IsNull) && !(OldVal && Fld->OldValue.IsNull()) )
            val = val + (OldVal?(int)Fld->OldValue:Fld->AsInteger);
    }
}

template <>
void Field<int>::SubVal (TField* Fld, bool OldVal)
{
    if (isNull())
    {
        if ( !(!OldVal && Fld->IsNull) && !(OldVal && Fld->OldValue.IsNull()) )
        {
            (*this)=-(OldVal?(int)Fld->OldValue:Fld->AsInteger);
        }
    }
    else
    {
        if ( !(!OldVal && Fld->IsNull) && !(OldVal && Fld->OldValue.IsNull()) )
            val = val - (OldVal?(int)Fld->OldValue:Fld->AsInteger);
    }
}


template <>
void Field<int>::GetResult (TAmsInput* AI)
{
    TAmsInputInt * AID = dynamic_cast<TAmsInputInt*> (AI);
    if (!AID)
    {
        TAmsInputAnal* IA= dynamic_cast<TAmsInputAnal*> (AI);
        {
            if (IA && !AI->AmsIsNull)
                *this=IA->AmsSumId;
            else
                SetNull();
        }
    }
    else
    {
        if (AID && !AI->AmsIsNull)
            *this=AID->GetResultInt();
        else
            SetNull();
    }
};


// Устанавливает OraField из Датасета, Флд Ном- номер поля внутри датасета
template <>
void Field<int>::SetField(TOracleDataSet* ODS, int FldNo)
{
    null = 0;
    TField* Fld = ODS->Fields->Fields[FldNo];
    if (Fld->IsNull)
    {
        val=0;
        SetNull();
    }
    else
        val = Fld->AsInteger;

};
// Уст поля датасета из OraFielda
template <>
void Field<int>::SetDS(TOracleDataSet* ODS, int FldNo)
{
    TField* Fld = ODS->Fields->Fields[FldNo];
    if (isNull())
    {
        Fld->Clear();
    }
    else
    {
       Fld->AsInteger=val;
    }
};


template <>
int Field<int>::DefAlign() { return SA_RIGHT ; }
#pragma argsused
template <>
int Field<int>::TSize(int len) { return 9 ; }
template <>
Sum Field<int>::Number() { return Sum(val) ; }
//--------------------------------------------------------------
template <>
void Field<int>::Buf2Field (AmsQuery& Curs, FieldLink& FL )
{
    if (FL.FldNum==-1)
    {
        FL.FldNum = Curs()->FieldIndex((char*)FL.name);
    };
    if (Curs()->FieldIsNull(FL.FldNum))
    {
        null = 1;
        val = 0;
    }
    else
    {
        null=0;
        val= Curs()->FieldAsInteger(FL.FldNum);
    }    
};
//--------------------------------------------------------------
template <>
void Field<int>::DeclVar (AmsQuery& Curs, const AnsiString& Val)
{
    Curs.DeclVarInt(Val);
}
//--------------------------------------------------------------
template <>
void Field<int>::GetVar (AmsQuery& Curs, const AnsiString& Val)
{
    (*this)= Curs.GetVarInt(Val);
};
//--------------------------------------------------------------
template <>
void Field<int>::SetVar (AmsQuery& Curs, const AnsiString& Val)
{
    if (!isNull())
        Curs.SetVar(Val, val);
};
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------

// rowid
template <>
VTYPE Field<orarowid>::vtype() { return gcl::VT_ROWID; }

template <>
void Field<orarowid>::Buf2Field(ub1 *buf)
{ val=*(orarowid *)buf; null=0; }
/*
template <>
void Field<orarowid>::Field2Buf(ub1 *buf) const
{ *(orarowid *)buf=val; }
*/
template <>
void Field<orarowid>::_SetNull() { val.strrep=""; }

template <>
void Field<orarowid>::SetVal (TAmsInput* AI)
{
    if (isNull()) AI->Clear();
    else
    AI->SetDefaultVal(val.strrep);
}
template <>
void Field<orarowid>::SetVal (TField* Fld)
{
    if (isNull()) Fld->Clear();
    else
        Fld->AsString=val.strrep;
}
template <>
void Field<orarowid>::GetVal(TField* Fld)
{
    if (Fld->IsNull)
    {
        SetNull();
    }
    else
    {
        SetNotNull();
        val.strrep=Fld->AsString;
    }
}

template <>
void Field<orarowid>::SetFld (AmsQuery& AQ, int FldNo)
{
    if (AQ.IsFldNull(FldNo)) SetNull();
    else;
        //(*this)=AQ.GetFldInt(FldNo);
}

template <>
AnsiString Field<orarowid>::AsString (const AnsiString& Format)
{
    return val.strrep;
}

template <>
void Field<orarowid>::GetResult (TAmsInput* AI)
{
    TAmsInputStr * AID = dynamic_cast<TAmsInputStr*> (AI);
    if (AID && !AI->AmsIsNull)
    {
        null = 0;
        val.strrep=AID->GetResultStr();
    }
    else
        SetNull();
};


// Устанавливает OraField из Датасета, Флд Ном- номер поля внутри датасета
template <>
void Field<orarowid>::SetField(TOracleDataSet* ODS, int FldNo)
{
    null = 0;
    TField* Fld = ODS->Fields->Fields[FldNo];
    if (Fld->IsNull)
    {
        val.strrep="";
        SetNull();
    }
    else
        val.strrep = Fld->AsString;
};
// Уст поля датасета из OraFielda
template <>
void Field<orarowid>::SetDS(TOracleDataSet* ODS, int FldNo)
{
    TField* Fld = ODS->Fields->Fields[FldNo];
    if (isNull())
    {
        Fld->Clear();
    }
    else
    {
       Fld->AsString=val.strrep;
    }
};


#pragma argsused
template <>
char *Field<orarowid>::Output(char *buf,int flags,int num_dig,int pchar)
{
 *buf=0 ;
 return buf ;
}

#pragma argsused
template <>
char *Field<orarowid>::OutputPval(char *buf,int side,int flags,int num_dig,int pchar)
{
 *buf=0 ;
 return buf ;
}

#pragma argsused
template <>
int Field<orarowid>::Input(const char *buf,int num_dig,char *errmes)
{
 return 0 ;
}

template <>
int Field<orarowid>::DefAlign() { return SA_LEFT ; }
template <>
int Field<orarowid>::TSize(int len) { return len ; }
template <>
Sum Field<orarowid>::Number() { return Sum(0) ; }
//--------------------------------------------------------------
template <>
void Field<orarowid>::Buf2Field (AmsQuery& Curs, FieldLink& FL )
{
    if (FL.FldNum==-1)
    {
        FL.FldNum = Curs()->FieldIndex((char*)FL.name);
    };
    if (Curs()->FieldIsNull(FL.FldNum))
    {
        null = 1;
        val.strrep = "";
    }
    else
    {
        null=0;
        val.strrep= Curs()->FieldAsString(FL.FldNum);
    }
};
//--------------------------------------------------------------
template <>
void Field<orarowid>::DeclVar (AmsQuery& Curs, const AnsiString& Val)
{
    Curs.DeclVarStr(Val);
}
//--------------------------------------------------------------
template <>
void Field<orarowid>::GetVar (AmsQuery& Curs, const AnsiString& Val)
{
    val.strrep= Curs.GetVarStr(Val);
};
//--------------------------------------------------------------
template <>
void Field<orarowid>::SetVar (AmsQuery& Curs, const AnsiString& Val)
{
    Curs.SetVar(Val, val.strrep);
};
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------


// Sum
template <>
VTYPE Field<Sum>::vtype() { return gcl::VT_NUM; }

template <>
void Field<Sum>::Buf2Field(ub1 *buf)
{
 val=_atold((char*)buf);
 null=0;
}
/*
template <>
void Field<Sum>::Field2Buf(ub1 *buf) const
{
 sprintf((char*)buf,"%.6Lf",val.dlv);
}
*/
template <>
void Field<Sum>::_SetNull() { val=0; }

#pragma argsused
template <>
char *Field<Sum>::Output(char *buf,int flags,int num_dig,int pchar)
{
 return val.GetStr(buf,flags,num_dig,pchar) ;
}

template <>
void Field<Sum>::SetVal(TAmsInput* AI)
{
    if (isNull()) AI->Clear();
    else
    AI->SetDefaultVal((long double)val);
}

template <>
void Field<Sum>::SetVal(TField* Fld)
{
    if (isNull()) Fld->Clear();
    else
        Fld->AsFloat=(long double)val;
}
template <>
void Field<Sum>::GetVal(TField* Fld)
{
    if (Fld->IsNull)
    {
        SetNull();
    }
    else
    {
        SetNotNull();
        val=Fld->AsFloat;
    }
}


template <>
void Field<Sum>::SetFld (AmsQuery& AQ, int FldNo)
{
    if (AQ.IsFldNull(FldNo)) SetNull();
    else
        (*this)=AQ.GetFldSum(FldNo);
}

template <>
AnsiString Field<Sum>::AsString (const AnsiString& Format)
{
    if (val)
        return FormatFloat(Format, val);
    else
        return "";
}


template <>
void Field<Sum>::AddVal (TField* Fld, bool OldVal)
{
    if (isNull())
    {
        if ( !(!OldVal && Fld->IsNull) && !(OldVal && Fld->OldValue.IsNull()) )
        {
            (*this)=OldVal?(double)Fld->OldValue:Fld->AsFloat;
        }
    }
    else
    {
        if ( !(!OldVal && Fld->IsNull) && !(OldVal && Fld->OldValue.IsNull()) )
            val = val + (OldVal?(double)Fld->OldValue:Fld->AsFloat);
    }
}

template <>
void Field<Sum>::SubVal (TField* Fld, bool OldVal)
{
    if (isNull())
    {
        if ( !(!OldVal && Fld->IsNull) && !(OldVal && Fld->OldValue.IsNull()) )
        {
            (*this)=-(OldVal?(double)Fld->OldValue:Fld->AsFloat);
        }
    }
    else
    {
        if ( !(!OldVal && Fld->IsNull) && !(OldVal && Fld->OldValue.IsNull()) )
            val = val -((OldVal?(double)Fld->OldValue:Fld->AsFloat));
    }
}


template <>
void Field<Sum>::GetResult (TAmsInput* AI)
{
    TAmsInputSum * AID = dynamic_cast<TAmsInputSum*> (AI);
    if (AID && !AI->AmsIsNull)
    {
        null = 0;
        val=AID->GetResultSum();
    }
    else
        SetNull();
};


// Устанавливает OraField из Датасета, Флд Ном- номер поля внутри датасета
template <>
void Field<Sum>::SetField(TOracleDataSet* ODS, int FldNo)
{
    null = 0;
    TField* Fld = ODS->Fields->Fields[FldNo];
    if (Fld->IsNull)
    {
        val=0;
        SetNull();
    }
    else
        val = Fld->AsFloat;
};
// Уст поля датасета из OraFielda
template <>
void Field<Sum>::SetDS(TOracleDataSet* ODS, int FldNo)
{
    TField* Fld = ODS->Fields->Fields[FldNo];
    if (isNull())
    {
        Fld->Clear();
    }
    else
    {
       Fld->AsFloat=val;
    }
};


#pragma argsused
template <>
char *Field<Sum>::OutputPval(char *buf,int side,int flags,int num_dig,int pchar)
{
 return val.Pval(side).GetStr(buf,flags,num_dig,pchar) ;
}

template <>
int Field<Sum>::Input(const char *buf,int num_dig,char *errmes)
{
 int r=val.SetStr((char *)buf,DEBET,NULL,0,num_dig,1) ;
 if (r==SS_OK) null=0 ;
 else if (errmes)
 {
  if (r==SS_BAD) strcpy(errmes,badsummes) ;
  if (r==SS_TOOBIG) strcpy(errmes,toomuchsummes) ;
 }

 return (r==SS_OK) ;
}

template <>
int Field<Sum>::DefAlign() { return SA_RIGHT ; }
#pragma argsused
template <>
int Field<Sum>::TSize(int len) { return SUM_LEN ; }
template <>
Sum Field<Sum>::Number() { return val ; }
//--------------------------------------------------------------
template <>
void Field<Sum>::Buf2Field (AmsQuery& Curs, FieldLink& FL )
{
    if (FL.FldNum==-1)
    {
        FL.FldNum = Curs()->FieldIndex((char*)FL.name);
    };
    if (Curs()->FieldIsNull(FL.FldNum))
    {
        null = 1;
        val = 0;
    }
    else
    {
        null=0;
        (*this)= Curs()->FieldAsFloat(FL.FldNum);
    }
};
//--------------------------------------------------------------
template <>
void Field<Sum>::DeclVar (AmsQuery& Curs, const AnsiString& Val)
{
    Curs.DeclVarSum(Val);
}
//--------------------------------------------------------------
template <>
void Field<Sum>::GetVar (AmsQuery& Curs, const AnsiString& Val)
{
    (*this)= Curs.GetVarSum(Val);
};
//--------------------------------------------------------------
template <>
void Field<Sum>::SetVar (AmsQuery& Curs, const AnsiString& Val)
{
    Curs.SetVar(Val, (double)val);
};
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------


// DateTime
template <>
VTYPE Field<DateTime>::vtype() { return gcl::VT_DATE; }

template <>
void Field<DateTime>::Buf2Field(ub1 *buf)
{
 int year=(buf[0]-100)*100+buf[1]-100-1900;
 int mon=buf[2];
 int mday=buf[3];
 val.Set(mday, mon, year);
 null=0;
}
/*
template <>
void Field<DateTime>::Field2Buf(ub1 *buf) const
{
 buf[0]=(val.tm_year+1900)/100+100;
 buf[1]=val.tm_year%100+100;
 buf[2]=val.tm_mon+1;
 buf[3]=val.tm_mday;
 buf[4]=val.tm_hour+1;
 buf[5]=val.tm_min+1;
 buf[6]=val.tm_sec+1;
}
*/
template <>
void Field<DateTime>::_SetNull() { val.Zero(); }

template <>
void Field<DateTime>::SetVal (TAmsInput* AI)
{
    if (isNull()) AI->Clear();
    else
    AI->SetDefaultVal(val.T());
}

template <>
void Field<DateTime>::SetVal(TField* Fld)
{
    if (isNull()) Fld->Clear();
    else
        Fld->AsDateTime=val.T();
}
template <>
void Field<DateTime>::GetVal(TField* Fld)
{
    if (Fld->IsNull)
    {
        SetNull();
    }
    else
    {
        SetNotNull();
        val=Fld->AsDateTime;
    }
}

template <>
void Field<DateTime>::SetFld (AmsQuery& AQ, int FldNo)
{
    if (AQ.IsFldNull(FldNo)) SetNull();
    else
        (*this)=AQ.GetFldDate(FldNo);
}

template <>
AnsiString Field<DateTime>::AsString (const AnsiString& Format)
{
    if ((int) (val.T()))
        return DateToStr(val.T());
    else
        return "";
}


template <>
void Field<DateTime>::GetResult (TAmsInput* AI)
{
    TAmsInputDate * AID = dynamic_cast<TAmsInputDate*> (AI);
    if (AID && !AI->AmsIsNull)
    {
        null = 0;
        val=AID->GetResultDat();
    }
    else
        SetNull();
};

// Устанавливает OraField из Датасета, Флд Ном- номер поля внутри датасета
template <>
void Field<DateTime>::SetField(TOracleDataSet* ODS, int FldNo)
{
    null = 0;
    TField* Fld = ODS->Fields->Fields[FldNo];
    if (Fld->IsNull)
    {
        val=0;
        SetNull();
    }
    else
        val = Fld->AsDateTime;

};
// Уст поля датасета из OraFielda
template <>
void Field<DateTime>::SetDS(TOracleDataSet* ODS, int FldNo)
{
    TField* Fld = ODS->Fields->Fields[FldNo];
    if (isNull())
    {
        Fld->Clear();
    }
    else
    {
       Fld->AsDateTime=val.T();
    }
};



#pragma argsused
template <>
char *Field<DateTime>::Output(char *buf,int flags,int num_dig,int pchar)
{
 if (num_dig==4)
   return val.Out(buf,11,"%d-%m-%Y") ;
 else
   return val.Out(buf,9) ;
}

#pragma argsused
template <>
char *Field<DateTime>::OutputPval(char *buf,int side,int flags,int num_dig,int pchar)
{
 if (side==DEBET) val.Out(buf,9) ;
 else *buf=0 ;
 return buf ;
}

#pragma argsused
template <>
int Field<DateTime>::Input(const char *buf,int num_dig,char *errmes)
{
 int r=val.SetStr(buf) ;
 if (r) null=0 ;
 else
   if (errmes) strcpy(errmes,baddatemes) ;
 return r ;
}

template <>
int Field<DateTime>::DefAlign() { return SA_RIGHT ; }
#pragma argsused
template <>
int Field<DateTime>::TSize(int len) { return 8 ; }
template <>
Sum Field<DateTime>::Number() { return Sum(0) ; }
//--------------------------------------------------------------
template <>
void Field<DateTime>::Buf2Field (AmsQuery& Curs, FieldLink& FL )
{
    if (FL.FldNum==-1)
    {
        FL.FldNum = Curs()->FieldIndex((char*)FL.name);
    };
    if (Curs()->FieldIsNull(FL.FldNum))
    {
        null = 1;
        val = 0;
    }
    else
    {
        null=0;
        val=Curs()->FieldAsDate(FL.FldNum);
    }
};
//--------------------------------------------------------------
template <>
void Field<DateTime>::DeclVar (AmsQuery& Curs, const AnsiString& Val)
{
    Curs.DeclVarDate(Val);
}
//--------------------------------------------------------------
template <>
void Field<DateTime>::GetVar (AmsQuery& Curs, const AnsiString& Val)
{
    val= Curs.GetVarDate(Val);
};
//--------------------------------------------------------------
template <>
void Field<DateTime>::SetVar (AmsQuery& Curs, const AnsiString& Val)
{
    Curs.SetVar(Val, val.T());
};
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------


int FieldIntDef::Set(FieldLink &lnk)
{
 if(stricmp(name,(char*)lnk.name))
  if(!altname || stricmp(altname,(char*)lnk.name))
   if(!altname1 || stricmp(altname1,(char*)lnk.name))
    return 0;
 lnk.int_type=type;

 switch(type) {
  case INT_TYPE: size=sizeof(int); break;
  case DATE_TYPE: size=7; break;
 }
 lnk.int_size=size?size:lnk.dbsize+1;
 lnk.fld=fld;
 lnk.fldNum=-1;
 return 1;
}

int FieldSet::_MakeLink(FieldLink &lnk,FieldIntDef *flds,int fldnum, int inp,int sn)
{
 lnk.fld=0;
 lnk.fldNum=-1;
 if(sn!=lnk.setN)
 {
  if(merged) merged->MakeLink(lnk,inp,sn+1);
  return 1;
 }
 for(int i=0;i<fldnum;i++) if(flds[i].Set(lnk)) return 1;
 return 0;
}

void FieldDynaSet::MakeLink(FieldLink &lnk, int inp, int sn)
{
 if(inp || sn!=lnk.setN)
 {
  if(merged) merged->MakeLink(lnk,inp,sn+1);
  return;
 }
 // ???????
 if (lnk.fldNum !=-1)
 {
    return;
 }
 lnk.fld=NULL;
 OraField *fld;
 if(lnk.dbtype==NUMBER_TYPE)
 {
   /* Handle NUMBER with scale as float. */
   if (lnk.scale != 0 || lnk.precision==0 )
   {
      lnk.int_type = STRING_TYPE;
      lnk.int_size = 34;
      fld=new Field<Sum>;
   }
   else
   {
      lnk.int_type = INT_TYPE;
      lnk.int_size = sizeof(int);
      fld=new Field<int>;
   }
 }
 else if (lnk.dbtype == ROWID_TYPE)
 {
      lnk.int_type = STRING_TYPE;
      lnk.int_size = 19;
      fld=new Field<orarowid>;
 }
 else if (lnk.dbtype == DATE_TYPE)
 {
      lnk.int_type = DATE_TYPE;
      lnk.int_size = 7 ;
      fld=new Field<DateTime>;
 }
 else
 {
    lnk.int_size=lnk.dbsize+1;
    lnk.int_type=STRING_TYPE;
    fld=new Field<::string>;
 }
 dynaflds.push_back(fld);
 // ?????????
 lnk.fldNum=dynaflds.size()-1;
 //lnk.fld=dynaflds[lnk.fldNum];
}

OraField *FieldDynaSet::Fld(FieldLink const &lnk,int sn)
{
 if(lnk.setN!=sn) return merged ? merged->Fld(lnk,sn+1) : NULL;
 if(lnk.fld) return &(this->*(lnk.fld));
 else if(lnk.fldNum>=0) return dynaflds[lnk.fldNum];
 else return NULL;
}

void FieldDynaSet::SetNull()
{
 for(int i=0;i<dynaflds.size();i++) dynaflds[i]->SetNull();
 if(merged) merged->SetNull();
}

FieldDynaSet::~FieldDynaSet()
{
  int i;
  for(i=0;i<dynaflds.size();i++) delete(dynaflds[i]);
}

FieldDynaSet &FieldDynaSet::operator =(FieldDynaSet const &c)
{
  for(int i=0;i<c.dynaflds.size();i++)
     dynaflds.push_back(c.dynaflds[i]->clone());
  return This;
}

FieldLink* FieldSet::Link(char const *name)
{
 if(tied) return tied->Link(name);
 else return NULL;
}

FieldLink* FieldLinks::Link(char const *name)
{
 for(int i=0;i<Num();i++)
  if(!stricmp((char*)flds[i].name,name)) return &flds[i];
 return NULL;
}

int FieldLinks::BufSize() const
{
 int tsize,i,def_flds;
 for(tsize=0,i=0,def_flds=0;i<Num();i++)
 {
   tsize+=flds[i].int_size;
   if(flds[i].int_type!=NULL_TYPE) def_flds++;
 }
 if(def_flds) return tsize;
 return 0;
}

template <class T>
void Field<T>::AddVal (TField* Fld, bool OldVal)
{
}

template <class T>
void Field<T>::SubVal (TField* Fld, bool OldVal)
{
}

template <class T>
void Field<T>::SetNumber(double Val)
{
}


template <>
void Field<int>::SetNumber(double Val)
{
    *this = ((int)Val);
}

template <>
void Field<Sum>::SetNumber(double Val)
{
    *this = Val;
}

