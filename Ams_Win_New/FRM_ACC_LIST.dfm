object FrmAccList: TFrmAccList
  Left = 344
  Top = 210
  Width = 464
  Height = 455
  ActiveControl = EdAccCode
  Caption = 'План счетов'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsMDIChild
  KeyPreview = True
  OldCreateOrder = False
  Position = poDefault
  Visible = True
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Button2: TButton
    Left = 8
    Top = 72
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Button2'
    TabOrder = 4
    TabStop = False
    OnClick = Button2Click
  end
  object Button1: TButton
    Left = 8
    Top = 32
    Width = 75
    Height = 25
    Caption = 'Button1'
    Default = True
    TabOrder = 3
    TabStop = False
    OnClick = Button1Click
  end
  object Panel1: TPanel
    Left = 0
    Top = 409
    Width = 456
    Height = 19
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    object Label1: TLabel
      Left = 9
      Top = 4
      Width = 68
      Height = 13
      Caption = 'Номер счета:'
    end
    object EdAccCode: TEdit
      Left = 80
      Top = 0
      Width = 375
      Height = 21
      Hint = 'Быстрый поиск счета'
      Anchors = [akLeft, akTop, akRight]
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
      OnChange = EdAccCodeChange
    end
  end
  object AccsView: TAmsAccsView
    Left = 0
    Top = 25
    Width = 456
    Height = 384
    Hint = 'План счетов'
    HelpContext = 12
    Align = alClient
    HideSelection = False
    Indent = 19
    PopupMenu = PopupMenu2
    RightClickSelect = True
    TabOrder = 1
    OnDblClick = AccsViewDblClick
    OnKeyPress = AccsViewKeyPress
    AmsOraQuery = OracleQuery1
  end
  object ToolBar1: TToolBar
    Left = 0
    Top = 0
    Width = 456
    Height = 25
    Caption = 'ToolBar1'
    Images = DatMod.ImageList1
    ParentShowHint = False
    ShowHint = True
    TabOrder = 2
    object ToolButton1: TToolButton
      Left = 0
      Top = 2
      Action = ActBegRems
      PopupMenu = PopupMenu2
    end
    object ToolButton2: TToolButton
      Left = 23
      Top = 2
      Action = ActUstAnal
    end
    object ToolButton3: TToolButton
      Left = 46
      Top = 2
      Action = ActOtlOper
    end
    object ToolButton4: TToolButton
      Left = 69
      Top = 2
      Width = 8
      Caption = 'ToolButton4'
      ImageIndex = 47
      Style = tbsSeparator
    end
    object ToolButton5: TToolButton
      Left = 77
      Top = 2
      Action = ActExecForm
    end
    object ToolButton6: TToolButton
      Left = 100
      Top = 2
      Action = ActShowRems
    end
    object ToolButton7: TToolButton
      Left = 123
      Top = 2
      Action = ActRefresh
    end
    object ToolButton8: TToolButton
      Left = 146
      Top = 2
      Width = 8
      Caption = 'ToolButton8'
      ImageIndex = 22
      Style = tbsSeparator
    end
    object ToolButton9: TToolButton
      Left = 154
      Top = 2
      Action = ActExpandAll
    end
    object ToolButton10: TToolButton
      Left = 177
      Top = 2
      Action = ActCollapseAll
    end
  end
  object OracleQuery1: TOracleQuery
    Session = DatMod.OSession
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Cursor = crDefault
    StringFieldsOnly = False
    Threaded = False
    ThreadSynchronized = True
    Left = 224
    Top = 48
  end
  object ActionList1: TActionList
    Images = DatMod.ImageList1
    Left = 256
    Top = 32
    object ActBegRems: TAction
      Caption = 'Остаток на начало'
      Hint = 'Ввод остатка на начало'
      ImageIndex = 55
      OnExecute = ActBegRemsExecute
    end
    object ActUstAnal: TAction
      Caption = 'Установки аналитики'
      Hint = 'Установки аналитического учета'
      ImageIndex = 63
      OnExecute = ActUstAnalExecute
    end
    object ActOtlOper: TAction
      Caption = 'Отложенные операции'
      Hint = 'Отложенные операции'
      ImageIndex = 46
      OnExecute = ActOtlOperExecute
    end
    object ActExecForm: TAction
      Caption = 'Выполнение отчетной формы'
      Hint = 'Выполнение отчетной формы'
      ImageIndex = 15
      Visible = False
      OnExecute = ActExecFormExecute
    end
    object ActShowRems: TAction
      Caption = 'Остаток по счету'
      Hint = 'Остаток по счету'
      ImageIndex = 0
      Visible = False
      OnExecute = ActShowRemsExecute
    end
    object ActAccView: TAction
      Caption = 'Просмотр счета'
      ShortCut = 113
      OnExecute = ActAccViewExecute
    end
    object ActRecalcLev: TAction
      Caption = 'Пересчитать уровни счетов'
      OnExecute = ActRecalcLevExecute
    end
    object ActRefresh: TAction
      Caption = 'Обновить'
      Hint = 'Обновить план счетов'
      ImageIndex = 21
      OnExecute = ActRefreshExecute
    end
    object ActExpandAll: TAction
      Caption = 'ActExpandAll'
      Hint = 'Развернуть все'
      ImageIndex = 59
      ShortCut = 16423
      OnExecute = ActExpandAllExecute
    end
    object ActCollapseAll: TAction
      Caption = 'ActCollapseAll'
      Hint = 'Свернуть все'
      ImageIndex = 49
      ShortCut = 16421
      OnExecute = ActCollapseAllExecute
    end
  end
  object PopupMenu2: TPopupMenu
    Images = DatMod.ImageList1
    Left = 296
    Top = 40
    object N2: TMenuItem
      Action = ActBegRems
    end
    object ActUstAnal1: TMenuItem
      Action = ActUstAnal
    end
    object N3: TMenuItem
      Action = ActOtlOper
    end
    object N1: TMenuItem
      Action = ActAccView
    end
    object N4: TMenuItem
      Caption = '-'
    end
    object N5: TMenuItem
      Action = ActRecalcLev
    end
  end
end
