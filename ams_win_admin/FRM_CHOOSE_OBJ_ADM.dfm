object FrmChooseObj: TFrmChooseObj
  Left = 221
  Top = 138
  Width = 526
  Height = 350
  HelpContext = 7
  Caption = 'FrmChooseObj'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsMDIChild
  OldCreateOrder = False
  Position = poDefault
  Visible = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 161
    Top = 0
    Width = 3
    Height = 323
    Cursor = crHSplit
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 161
    Height = 323
    Align = alLeft
    Caption = 'Panel1'
    TabOrder = 0
    object ToolBar1: TToolBar
      Left = 1
      Top = 1
      Width = 159
      Height = 28
      Caption = 'ToolBar1'
      TabOrder = 0
      object ToolButton1: TToolButton
        Left = 0
        Top = 2
        Hint = 'Обновить список категорий'
        Caption = 'ToolButton1'
        ImageIndex = 21
        ParentShowHint = False
        ShowHint = True
        OnClick = ToolButton1Click
      end
    end
    object TV: TTreeView
      Left = 1
      Top = 29
      Width = 159
      Height = 293
      Align = alClient
      HideSelection = False
      Indent = 19
      TabOrder = 1
      OnChange = TVChange
      OnEditing = TVEditing
      OnKeyUp = TVKeyUp
    end
  end
  object SG: TAmsStringGrid
    Left = 164
    Top = 0
    Width = 354
    Height = 323
    Align = alClient
    ColCount = 4
    DefaultRowHeight = 18
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goRowSelect, goThumbTracking]
    PopupMenu = PopupMenu1
    ScrollBars = ssVertical
    TabOrder = 1
    OnDblClick = SGDblClick
    OnKeyPress = SGKeyPress
    OnResize = SGResize
    ColWidths = (
      48
      115
      82
      24)
  end
  object OQ: TOracleQuery
    SQL.Strings = (
      'select '
      #9'OC.OWNCAT, '
      #9'OC.CATID, '
      #9'OC.ORD, '
      #9'OC.DESCR '
      'from OBJCAT OC, PRIVTEST PT'
      'where '
      #9'OC.OWNCAT = PT.POID and'
      #9'UNAME = USER and'
      #9'PR0 = 1 and'
      #9'OC.OWNCAT in '
      #9#9'('
      #9#9#9'select CATID '
      #9#9#9'from OBJS O '
      #9#9#9'where '
      #9#9#9#9'OTYPE = :OTYPE'
      #9#9')'
      'union'
      'select '
      #9'OC.OWNCAT, '
      #9'OC.CATID, '
      #9'OC.ORD, '
      #9'OC.DESCR '
      'from OBJCAT OC'
      'where'
      #9'OC.CATID is null and'
      #9'OC.OWNCAT in '
      #9#9'('
      #9#9#9'select CATID '
      #9#9#9'from OBJS O '
      #9#9#9'where '
      #9#9#9#9'OTYPE = :OTYPE'
      #9#9')'
      ' ')
    Session = MainFrm.OSess
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Cursor = crDefault
    StringFieldsOnly = False
    Threaded = False
    ThreadSynchronized = True
    Left = 80
    Top = 96
  end
  object ActionList1: TActionList
    Left = 320
    Top = 160
    object ActDelPos: TAction
      Caption = 'Удаленные позиции'
      Hint = 'Удаленные позиции'
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 240
    Top = 96
    object N1: TMenuItem
      Action = ActDelPos
    end
  end
end
