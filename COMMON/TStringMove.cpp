//---------------------------------------------------------------------------

#include "AMS_HDR.h"
#pragma hdrstop

#include "TStringMove.h"
#include "FRM_EDIT_GROUPING.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//  Перемещает из одного ListBox в Другой LB
void MoveLR (TListBox* LBFrom, TListBox* LBWhere, bool TakeAll)
{
  int i = 0;
  while (i< LBFrom->Items->Count)
  {
    if (TakeAll | LBFrom->Selected[i])
    {
      LBWhere->Items->Add(LBFrom->Items->Strings[i]);
      LBFrom->Items->Delete(i);
    }
    else i++;
  };
};
//--------------------------------------------------------------------------
bool MoveAllUp   (TListBox* LB)
{
    bool Moved = false;
    int i = LB->ItemIndex; 
    if ( i!= -1)
    {
        if (i)
        {
            Moved=true;
            TStrings* S= LB->Items;
            AnsiString AS= S->Strings[i];
            for (int j = i; j >0 ; j--)
                S->Strings[j]= S->Strings[j-1];
            S->Strings[0]=AS;
            LB->ItemIndex = 0;
        } 
    }
    return Moved;    
}
//--------------------------------------------------------------------------
bool MoveAllDown (TListBox* LB)
{
    bool Moved = false;
    int i = LB->ItemIndex;
    int Count  = LB->Items->Count;
    if ( i!= -1)
    {
        if (i!= (Count-1) )
        {
            Moved= true;
            TStrings* S= LB->Items;
            AnsiString AS= S->Strings[i];
            for (int j = i ; j< Count-1; j++)
                S->Strings[j]=S->Strings[j+1]; 
            S->Strings[Count-1]=AS;
            LB->ItemIndex = Count-1;
        } 
    }  
    return Moved;              
};
//--------------------------------------------------------------------------
bool MoveOneUp   (TListBox* LB)
{
    bool Moved = false;
    int i = LB->ItemIndex; 
    if ( i!= -1)
    {
        if (i)
        {
            Moved=true;
            TStrings* S= LB->Items;
            AnsiString AS= S->Strings[i];
            S->Strings[i]=S->Strings[i-1];
            S->Strings[i-1]=AS;
            LB->ItemIndex = i-1;
        } 
    }       
    return Moved;
};
//--------------------------------------------------------------------------
bool MoveOneDown (TListBox* LB)
{
    bool Moved = false;
    int i = LB->ItemIndex;
    int Count  = LB->Items->Count;
    if ( i!= -1)
    {
        if (i!= (Count-1) )
        {
            Moved = true;
            TStrings* S= LB->Items;
            AnsiString AS= S->Strings[i];
            S->Strings[i]=S->Strings[i+1];
            S->Strings[i+1]=AS;
            LB->ItemIndex = i+1;
        } 
    }      
    return Moved;     
};
//--------------------------------------------------------------------------

 
