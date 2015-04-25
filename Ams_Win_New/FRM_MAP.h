//---------------------------------------------------------------------------

#ifndef FRM_MAPH
#define FRM_MAPH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <Menus.hpp>
#include <ActnList.hpp>
#include "Oracle.hpp"
#include <vector.h>
#include "AmsInputPoint.h"
#include <ImgList.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------
typedef struct {
 int X,Y, Id;
 } TPnt;

typedef struct {
 int PX;
 int PY;
 } TMapQuery;

typedef struct {
 int id,color,AllSum;
 AnsiString path, name;
 } TTreeData;

class TFrmMap : public TForm
{
__published:	// IDE-managed Components
    TActionList *ActionList1;
    TAction *ActGoTop;
    TAction *ActChange;
    TAction *ActStandart;
    TPopupMenu *PopupMenu1;
    TMenuItem *N10;
    TMenuItem *N11;
    TAction *ActAddPoint;
    TStatusBar *sb;
    TMenuItem *N2;
    TSplitter *Splitter1;
    TPanel *pan;
    TTreeView *tv;
    TPanel *pim;
    TAction *ActRefresh;
    TMenuItem *N1;
    TImageList *ImageList1;
    TPanel *ptb;
    TToolBar *ToolBar1;
    TToolButton *tbGoHigh;
    TImage *im;
    TToolButton *tbQuery;
    TToolButton *tbCreateQry;
    TAction *ActShowStat;
    TToolButton *tbBack;
    TAction *ActGoBack;
    TToolButton *ToolButton1;
    TAction *ActGoStraight;
    
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall imMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall ActGoTopExecute(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall tvChange(TObject *Sender, TTreeNode *Node);
    void __fastcall Splitter1Moved(TObject *Sender);
    void __fastcall imMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
    void __fastcall ActShowStatExecute(TObject *Sender);
    void __fastcall ActGoBackExecute(TObject *Sender);
    void __fastcall ActGoStraightExecute(TObject *Sender);

private:	// User declarations

    int type;                 // 0 - новая запись, idPoint -  переопределить
    int AddPoint;             // Добавлять ли точку в таблицу
    int IdRegion;             // Идентиф региона
    bool ShowStatistic;       // Поkазывать ли статистику при нажатии на Объект
    int  PntSeq;              // Идентиф точки при вызове из ввода в БД
    int  CurMapHistPos;       // Текущая позиции истории просмотров
    int  CurColor;            // Цвет текущей области(по которой бегает мышь)
    bool InsIntoMainMap;
    bool AddToHist;           // Добавлять ли в историю просмотра карт
    string CurHintTempl;      // Шаблон вывода тултипа
    AmsQuery ShowQry;

    void __fastcall LoadMap(AnsiString FileName, AnsiString Name, int id);
    void __fastcall InputTree(TTreeNode* tnd, int id);
    void __fastcall DrawPoint(int id, int Radius=3);
    void __fastcall FillPointsList();
    int __fastcall IsNearPoint(int X, int Y);
    void __fastcall FillHint(int Id_Point, string);
    void __fastcall Add(int X, int Y);
    void __fastcall HintShow(int X, int Y);
    int __fastcall tvGetTreeNodeIndex(TTreeNode* td);
    int __fastcall FindMainPntCoord(TColor cl, AnsiString s);
        int __fastcall CalcRegionData(int id);
        int __fastcall FindPoint(int Id_);

public:		// User declarations

    TAmsInputPoint* AmsIn;
    std::vector< TPnt > Points;             // Набор точек на текущей карте
    std::map<int, TMapQuery*> MapQry;       // Все точки 
    std::map<int, TTreeNode*> MapHist;      // Последовательность просмотров карт
    __fastcall TFrmMap(TComponent* Owner);
    void __fastcall InitInsertPoint(TAmsInputPoint*, int);
    void __fastcall FreeTreeData();
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMap *FrmMap;
//---------------------------------------------------------------------------
#endif
