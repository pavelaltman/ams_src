//---------------------------------------------------------------------------

#ifndef TStringMoveH
#define TStringMoveH
//---------------------------------------------------------------------------
extern void MoveLR (TListBox* LBFrom, TListBox* LBWhere, bool TakeAll = true);
// Возвращает true если подвинуло 
extern bool MoveAllUp   (TListBox* LB);
extern bool MoveAllDown (TListBox* LB);
extern bool MoveOneUp   (TListBox* LB);
extern bool MoveOneDown (TListBox* LB);
//---------------------------------------------------------------------------
#endif
