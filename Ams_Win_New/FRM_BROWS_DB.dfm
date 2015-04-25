object FrmBrowsDb: TFrmBrowsDb
  Left = 249
  Top = 114
  Width = 514
  Height = 425
  HelpContext = 10
  Caption = 'FrmBrowsDb'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsMDIChild
  OldCreateOrder = False
  Position = poOwnerFormCenter
  Visible = True
  OnActivate = FormActivate
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 177
    Top = 40
    Width = 3
    Height = 339
    Cursor = crHSplit
  end
  object Lab1: TLabel
    Left = 0
    Top = 25
    Width = 506
    Height = 15
    Hint = '������ �������� ���������'
    Align = alTop
    AutoSize = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
  end
  object ButEnter: TButton
    Left = 8
    Top = 48
    Width = 75
    Height = 25
    Caption = 'ButEnter'
    Default = True
    TabOrder = 4
    TabStop = False
    OnClick = ButEnterClick
  end
  object CV: TAmsDbCatView
    Left = 0
    Top = 40
    Width = 177
    Height = 339
    Align = alLeft
    ChangeDelay = 1
    HideSelection = False
    Images = DatMod.TreeImageList
    Indent = 19
    PopupMenu = PopupMenu2
    TabOrder = 0
    OnChange = CVChange
    OnChanging = CVChanging
    OnEnter = CVEnter
    OnKeyDown = CVKeyDown
    OnKeyPress = CVKeyPress
    AmsOraQuery = OQ
    AmsBaseSchema = 'OKOP'
  end
  object SBar: TStatusBar
    Left = 0
    Top = 379
    Width = 506
    Height = 19
    Panels = <
      item
        Width = 100
      end
      item
        Width = 100
      end
      item
        Width = 100
      end
      item
        Width = 100
      end
      item
        Width = 50
      end>
    SimplePanel = False
  end
  object ToolBar3: TToolBar
    Left = 0
    Top = 0
    Width = 506
    Height = 25
    ButtonWidth = 24
    Caption = 'ToolBar1'
    Images = DatMod.ImageList1
    ParentShowHint = False
    PopupMenu = PopupMenu3
    ShowHint = True
    TabOrder = 1
    object ToolButton21: TToolButton
      Left = 0
      Top = 2
      Action = ActNew
    end
    object ToolButton22: TToolButton
      Left = 24
      Top = 2
      Action = ActEdit
    end
    object ToolButton23: TToolButton
      Left = 48
      Top = 2
      Action = ActDel
    end
    object ToolButton24: TToolButton
      Left = 72
      Top = 2
      Action = ActMove
    end
    object ToolButton25: TToolButton
      Left = 96
      Top = 2
      Action = ActMoveAll
    end
    object ToolButton26: TToolButton
      Left = 120
      Top = 2
      Action = ActCopy
    end
    object ToolButton28: TToolButton
      Left = 144
      Top = 2
      Action = ActWCat
    end
    object ToolButton29: TToolButton
      Left = 168
      Top = 2
      Action = ActWVer
    end
    object ToolButton30: TToolButton
      Left = 192
      Top = 2
      Action = ActSel
    end
    object ToolButton31: TToolButton
      Left = 216
      Top = 2
      Action = ActAll
    end
    object ToolButton32: TToolButton
      Left = 240
      Top = 2
      Action = ActSort
    end
    object ToolButton33: TToolButton
      Left = 264
      Top = 2
      Action = ActAnSort
    end
    object ToolButton35: TToolButton
      Left = 288
      Top = 2
      Action = ActFind
    end
    object ToolButton36: TToolButton
      Left = 312
      Top = 2
      Action = ActFindCat
    end
    object ToolButton37: TToolButton
      Left = 336
      Top = 2
      Action = ActFindAg
    end
    object ToolButton3: TToolButton
      Left = 360
      Top = 2
      Action = ActRefreshCat
    end
    object ToolButton1: TToolButton
      Left = 384
      Top = 2
      Action = ActBuildCatTree
    end
    object ToolButton2: TToolButton
      Left = 408
      Top = 2
      Action = ActHideCatWindow
    end
    object ToolButton4: TToolButton
      Left = 432
      Top = 2
      Action = ActCopyFromExcel
    end
  end
  object Panel1: TPanel
    Left = 180
    Top = 40
    Width = 326
    Height = 339
    Align = alClient
    BevelOuter = bvNone
    Caption = 'Panel1'
    TabOrder = 2
    object DBG: TAmsDBGrid
      Left = 0
      Top = 0
      Width = 326
      Height = 339
      Align = alClient
      DataSource = DataSource1
      Options = [dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgConfirmDelete, dgCancelOnExit, dgMultiSelect]
      PopupMenu = PopupMenu3
      TabOrder = 0
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clWindowText
      TitleFont.Height = -11
      TitleFont.Name = 'MS Sans Serif'
      TitleFont.Style = []
      OnCellClick = DBGCellClick
      OnDblClick = DBGFrosenDblClick
      OnEnter = DBGFrosenEnter
      OnKeyDown = DBGKeyDown
      OnKeyPress = DBGKeyPress
      OnMouseMove = DBGMouseMove
      MultiSelect = True
      DefaultRowHeight = 17
    end
  end
  object OQ: TOracleQuery
    Session = DatMod.OSession
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Cursor = crDefault
    StringFieldsOnly = False
    Threaded = False
    ThreadSynchronized = True
    Left = 48
    Top = 96
  end
  object ODS: TOracleDataSet
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Variables.Data = {
      0300000001000000060000003A43415449440300000004000000FFFFFFFF0000
      0000}
    StringFieldsOnly = False
    SequenceField.ApplyMoment = amOnPost
    OracleDictionary.EnforceConstraints = False
    OracleDictionary.UseMessageTable = False
    OracleDictionary.DefaultValues = False
    OracleDictionary.DynamicDefaults = False
    OracleDictionary.FieldKinds = False
    OracleDictionary.DisplayFormats = False
    OracleDictionary.RangeValues = False
    OracleDictionary.RequiredFields = True
    QBEDefinition.SaveQBEValues = True
    QBEDefinition.AllowFileWildCards = True
    QBEDefinition.QBEFontColor = clNone
    QBEDefinition.QBEBackgroundColor = clNone
    Cursor = crDefault
    ReadOnly = False
    LockingMode = lmCheckImmediate
    QueryAllRecords = False
    CountAllRecords = False
    RefreshOptions = []
    OnApplyRecord = ODSApplyRecord
    CommitOnPost = True
    CachedUpdates = False
    QBEMode = False
    Session = DatMod.OSession
    DesignActivation = False
    Active = False
    AfterScroll = ODSAfterScroll
    Left = 96
    Top = 184
  end
  object DataSource1: TDataSource
    DataSet = ODS
    Left = 64
    Top = 186
  end
  object PopupMenu2: TPopupMenu
    Left = 72
    Top = 96
    object N2: TMenuItem
      Action = ActNew
    end
    object CtrlIns1: TMenuItem
      Action = ActNewSubCat
    end
    object N5: TMenuItem
      Action = ActEditDbCatName
    end
    object N3: TMenuItem
      Action = ActDel
    end
    object ActMoveAllDbCat1: TMenuItem
      Action = ActMove
    end
    object N1: TMenuItem
      Action = ActMoveAll
    end
    object N4: TMenuItem
      Action = ActFindCat
    end
    object ShiftF72: TMenuItem
      Action = ActFindAg
    end
    object N9: TMenuItem
      Action = ActHideCatWindow
    end
    object N6: TMenuItem
      Caption = '-'
    end
    object N7: TMenuItem
      Action = ActBuildCatTree
    end
  end
  object ActionList2: TActionList
    Images = DatMod.ImageList1
    Left = 128
    Top = 136
    object ActEdit: TAction
      Caption = '������������� [Enter]'
      Hint = '������������� ������� ��'
      ImageIndex = 41
      OnExecute = ActEdDbPosExecute
    end
    object ActDel: TAction
      Caption = '�������'
      Hint = '������� �������'
      ImageIndex = 14
      ShortCut = 46
      OnExecute = ActDelExecute
    end
    object ActSel: TAction
      Caption = '�������'
      Hint = '�������  �������'
      ImageIndex = 111
      ShortCut = 116
      OnExecute = ActSelExecute
    end
    object ActNew: TAction
      Caption = '�����'
      Hint = '���� ����� �������'
      ImageIndex = 40
      ShortCut = 45
      OnExecute = ActInsExecute
    end
    object ActAll: TAction
      Caption = '������ �������'
      Hint = '������ ������� �������'
      ImageIndex = 112
      ShortCut = 16500
      OnExecute = ActAllExecute
    end
    object ActSort: TAction
      Caption = '����������'
      Hint = '���������� �������'
      ImageIndex = 110
      ShortCut = 120
      OnExecute = ActSortOrderExecute
    end
    object ActAnSort: TAction
      Caption = '������ ����������'
      Hint = '������ ����������'
      ImageIndex = 113
      ShortCut = 16504
      OnExecute = ActSortBackExecute
    end
    object ActPrint: TAction
      Caption = '������ [Alt+�]'
      Hint = '������� �� ������'
      ImageIndex = 37
    end
    object ActMove: TAction
      Caption = '����������� (Alt+�)'
      Hint = '����������� ������� � ���������'
      ImageIndex = 38
      OnExecute = ActMoveToCatExecute
    end
    object ActMoveAll: TAction
      Caption = '����������� ���'
      Hint = '����������� ��� ������� � ���������'
      ImageIndex = 67
      OnExecute = ActMoveAllExecute
    end
    object ActCopy: TAction
      Caption = '����������� (Alt+�)'
      Hint = '����������� ������� ��'
      ImageIndex = 31
      OnExecute = ActCopyExecute
    end
    object ActPerehod: TAction
      Caption = '������� '
      Hint = '������� � ��������� ��'
      ImageIndex = 28
      ShortCut = 115
    end
    object ActWCat: TAction
      Caption = '�������� ��������� (*)'
      Hint = '����� ��������� �� ��� ���������'
      ImageIndex = 45
      OnExecute = ActWCatExecute
    end
    object ActWVer: TAction
      Caption = '�������� ������'
      Hint = '����� ��������� �� ��� ������'
      ImageIndex = 73
      ShortCut = 119
      OnExecute = ActSVerExecute
    end
    object ActCalc: TAction
      Caption = '�����������'
      Hint = '����������� ��'
      ImageIndex = 101
      ShortCut = 16497
      OnExecute = ActCalcExecute
    end
    object ActFind: TAction
      Caption = '����� �������'
      Hint = '����� ������� ��'
      ImageIndex = 39
      ShortCut = 118
      OnExecute = ActFindExecute
    end
    object ActFindCat: TAction
      Caption = '����� ���������'
      Hint = '����� ��������� ��'
      ImageIndex = 33
      ShortCut = 16502
      OnExecute = ActFindCatExecute
    end
    object ActFindAg: TAction
      Caption = '����� ����� (Shift+F7)'
      Hint = '���������� �����'
      ImageIndex = 76
      ShortCut = 8310
      OnExecute = ActFindAgainExecute
    end
    object ActOpSel: TAction
      Caption = '�������� ��� ��������'
      Hint = '�������� ��� ��������'
      ImageIndex = 65
    end
    object ActOpTree: TAction
      Caption = '�������� ��� ����������'
      Hint = '�������� ��� ����������'
      ImageIndex = 59
    end
    object ActDelAll: TAction
      Caption = '������� ���'
      Hint = '������� ���'
      ImageIndex = 4
      OnExecute = MenSelDelAllClick
    end
    object ActCatFld: TAction
      Caption = '���������� ����'
      Hint = '���������� ���� ��� ���� ������� ��������� (���������)'
      ImageIndex = 44
      OnExecute = MenCatSetFldClick
    end
    object ActNull: TAction
      Caption = '���������� ������ ����'
      Hint = '���������� ������ ���� ��� ������ ������� �������'
      ImageIndex = 75
      OnExecute = MenSelSetFreeFldClick
    end
    object ActRefreshCat: TAction
      Caption = '���������� ��������� ��'
      Hint = '���������� ��������� ��'
      ImageIndex = 21
      OnExecute = ActRefreshCatExecute
    end
    object ActNewSubCat: TAction
      Caption = '����� ������������ [Ctrl+Ins]'
      Hint = '����� ������������ [Ctrl+Ins]'
      ImageIndex = 11
      OnExecute = ActNewSubCatExecute
    end
    object ActSelFld: TAction
      Caption = '���������� ����'
      Hint = '���������� ���� ��� ���� ������� �������'
      ImageIndex = 44
      OnExecute = MenSelSetFldClick
    end
    object ActTest: TAction
      Caption = 'ActTest'
    end
    object ActHist: TAction
      Caption = '������� ��������� '
      Hint = '������� ��������� (Alt+�)'
      OnExecute = ActHistExecute
    end
    object ActEditDbCatName: TAction
      Caption = '�������������� �������� ���������'
      Hint = '�������������� �������� ���������'
      ShortCut = 8237
      OnExecute = ActEditDbCatNameExecute
    end
    object ActBuildCatTree: TAction
      Caption = '����������� ������ ���������'
      Hint = 
        '����������� ������ ���������'#13#10'(������������ ����� ��������� ����' +
        '����� ���������)'
      ImageIndex = 114
      OnExecute = ActBuildCatTreeExecute
    end
    object ActHideCatWindow: TAction
      Caption = '�������� ���� ���������'
      Hint = '�������� ���� ���������'
      ImageIndex = 115
      ShortCut = 123
      OnExecute = ActHideCatWindowExecute
    end
    object ActEditAn: TAction
      Category = 'Analictica'
      Caption = 'ActEditAn'
      ImageIndex = 0
      OnExecute = ActEditAnExecute
    end
    object ActVisibilityMode: TAction
      Category = 'Analictica'
      Caption = '����� ���������'
      Hint = 
        '�������� ����� ��������� - ��� ����, '#13#10'��������� �������'#13#10'������' +
        '�'
      OnExecute = ActVisibilityModeExecute
    end
    object ActSumInfo: TAction
      Category = 'Analictica'
      Caption = '��� ������������� �����'
      Hint = '��� ������������� �����'
      OnExecute = ActSumInfoExecute
    end
    object ActGoto: TAction
      Caption = '�������'
      Hint = '������� �� ����������'
      ImageIndex = 59
      ShortCut = 115
      OnExecute = ActGotoExecute
    end
    object ActFindInCurrCat: TAction
      Category = 'NEW'
      Caption = '����� � ��� ��������� (Alt+F7)'
      Hint = '����� ������� � ������� ���������'
      OnExecute = ActFindInCurrCatExecute
    end
    object ActCopyFromExcel: TAction
      Category = 'NEW'
      Caption = '���������� �� Excel'
      Hint = 
        '���������� �� Excel. '#13#10'(������� ���������� �������� ����� � Exce' +
        'l � ����������� ���)'
      ImageIndex = 119
      OnExecute = ActCopyFromExcelExecute
    end
    object ActEscExit: TAction
      Caption = 'ActEscExit'
      ShortCut = 27
      OnExecute = ActEscExitExecute
    end
  end
  object PopupMenu3: TPopupMenu
    OnPopup = PopupMenu3Popup
    Left = 264
    Top = 120
    object MenuItem1: TMenuItem
      Action = ActNew
    end
    object Enter1: TMenuItem
      Action = ActEdit
    end
    object MenuItem2: TMenuItem
      Action = ActDel
    end
    object Alt1: TMenuItem
      Action = ActMove
    end
    object MenuItem3: TMenuItem
      Action = ActMoveAll
    end
    object Alt4: TMenuItem
      Action = ActCopy
    end
    object MenuItem5: TMenuItem
      Action = ActWCat
    end
    object MenuItem6: TMenuItem
      Action = ActWVer
    end
    object MenuItem7: TMenuItem
      Action = ActSel
    end
    object MenuItem8: TMenuItem
      Action = ActAll
    end
    object MenuItem9: TMenuItem
      Action = ActSort
    end
    object MenuItem10: TMenuItem
      Action = ActAnSort
    end
    object MenuItem11: TMenuItem
      Action = ActCalc
    end
    object MenuItem12: TMenuItem
      Action = ActFind
    end
    object AltF71: TMenuItem
      Action = ActFindInCurrCat
    end
    object MenuItem13: TMenuItem
      Action = ActFindCat
    end
    object ShiftF71: TMenuItem
      Action = ActFindAg
    end
    object N8: TMenuItem
      Action = ActHideCatWindow
    end
    object MenuItem14: TMenuItem
      Caption = '�������� ��� ��������'
      Hint = '�������� ��� ��������'
      ImageIndex = 65
      object MenSelDelAll: TMenuItem
        Action = ActDelAll
      end
      object MenSelSetFld: TMenuItem
        Action = ActSelFld
      end
      object MenSelSetFreeFld: TMenuItem
        Action = ActNull
      end
    end
    object MenuItem17: TMenuItem
      Caption = '�������� ��� ����������'
      Hint = '�������� ��� ����������'
      ImageIndex = 59
      object MenCatSetFld: TMenuItem
        Action = ActCatFld
      end
    end
    object MenGoto: TMenuItem
      Action = ActGoto
    end
    object Excel1: TMenuItem
      Action = ActCopyFromExcel
    end
  end
  object OQFind: TOracleQuery
    Session = DatMod.OSession
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Cursor = crDefault
    StringFieldsOnly = False
    Threaded = False
    ThreadSynchronized = True
    Left = 192
    Top = 176
  end
end
