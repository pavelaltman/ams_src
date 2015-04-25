object FrmCondsEdit: TFrmCondsEdit
  Left = 319
  Top = 269
  Width = 612
  Height = 413
  Caption = 'FrmCondsEdit'
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
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 0
    Top = 260
    Width = 604
    Height = 3
    Cursor = crVSplit
    Align = alBottom
  end
  object Splitter2: TSplitter
    Left = 241
    Top = 29
    Width = 3
    Height = 231
    Cursor = crHSplit
  end
  object DBGridObjs: TDBGrid
    Left = 0
    Top = 29
    Width = 241
    Height = 231
    Align = alLeft
    DataSource = DataSource1
    Options = [dgEditing, dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit, dgMultiSelect]
    TabOrder = 0
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
    Columns = <
      item
        Expanded = False
        FieldName = 'OID'
        Title.Caption = 'ID'
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'DESCR'
        Title.Caption = 'Имя стд операции'
        Width = 149
        Visible = True
      end>
  end
  object Panel1: TPanel
    Left = 0
    Top = 263
    Width = 604
    Height = 123
    Align = alBottom
    Caption = 'Panel1'
    TabOrder = 1
    object Splitter3: TSplitter
      Left = 241
      Top = 1
      Width = 3
      Height = 121
      Cursor = crHSplit
    end
    object DBGrid3: TDBGrid
      Left = 244
      Top = 1
      Width = 359
      Height = 121
      Align = alClient
      DataSource = DataSource3
      TabOrder = 0
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clWindowText
      TitleFont.Height = -11
      TitleFont.Name = 'MS Sans Serif'
      TitleFont.Style = []
      Columns = <
        item
          Expanded = False
          FieldName = 'ITEM_NUM'
          Title.Caption = '#'
          Width = 22
          Visible = True
        end
        item
          Expanded = False
          FieldName = 'SIDE'
          Title.Caption = 'Д/К'
          Width = 49
          Visible = True
        end
        item
          Expanded = False
          FieldName = 'NAME1'
          Title.Caption = 'Счет'
          Width = 62
          Visible = True
        end
        item
          Expanded = False
          FieldName = 'SETNAME'
          Title.Caption = 'Набор установок'
          Width = 99
          Visible = True
        end
        item
          Expanded = False
          FieldName = 'ITEM_FORMULA'
          Title.Caption = 'Формула'
          Width = 100
          Visible = True
        end>
    end
    object DBGrid2: TDBGrid
      Left = 1
      Top = 1
      Width = 240
      Height = 121
      Align = alLeft
      DataSource = DataSource2
      Options = [dgEditing, dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit]
      TabOrder = 1
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clWindowText
      TitleFont.Height = -11
      TitleFont.Name = 'MS Sans Serif'
      TitleFont.Style = []
      Columns = <
        item
          Expanded = False
          FieldName = 'COND_SUBID'
          Title.Caption = 'SUBID'
          Width = 26
          Visible = True
        end
        item
          Expanded = False
          FieldName = 'TEXT'
          Title.Caption = 'Название'
          Width = 183
          Visible = True
        end>
    end
  end
  object ToolBar1: TToolBar
    Left = 0
    Top = 0
    Width = 604
    Height = 29
    Caption = 'ToolBar1'
    Images = MainFrm.ImageList1
    TabOrder = 2
    object ToolButton1: TToolButton
      Left = 0
      Top = 2
      Hint = 'Показать категории'
      Caption = 'ToolButton1'
      ImageIndex = 2
      ParentShowHint = False
      ShowHint = True
      OnClick = ToolButton1Click
    end
    object ToolButton2: TToolButton
      Left = 23
      Top = 2
      Hint = 'Применить изменения'
      Caption = 'ToolButton2'
      ImageIndex = 12
      ParentShowHint = False
      ShowHint = True
      OnClick = ToolButton2Click
    end
    object ToolButton3: TToolButton
      Left = 46
      Top = 2
      Hint = 'Копировать проводки в другую бухгалтерию (категорию)'
      Caption = 'ToolButton3'
      ImageIndex = 18
      ParentShowHint = False
      ShowHint = True
      OnClick = ToolButton3Click
    end
  end
  object Panel2: TPanel
    Left = 244
    Top = 29
    Width = 360
    Height = 231
    Align = alClient
    Caption = 'Panel2'
    TabOrder = 3
    object Splitter4: TSplitter
      Left = 1
      Top = 61
      Width = 358
      Height = 3
      Cursor = crVSplit
      Align = alBottom
    end
    object DBMemo1: TDBMemo
      Left = 1
      Top = 64
      Width = 358
      Height = 166
      Align = alBottom
      DataField = 'ITEM_FORMULA'
      DataSource = DataSource3
      TabOrder = 0
    end
    object DBMemo2: TDBMemo
      Left = 1
      Top = 1
      Width = 358
      Height = 60
      Align = alClient
      DataField = 'PRE_FORMULA'
      DataSource = DataSource2
      TabOrder = 1
    end
  end
  object DataSource3: TDataSource
    DataSet = QCondItems
    Left = 48
    Top = 200
  end
  object QCondItems: TOracleDataSet
    SQL.Strings = (
      'select COND_ITEMS.side, COND_ITEMS.setname, '
      
        '       COND_ITEMS.item_formula, COND_ITEMS.Item_NUM, useracc_new' +
        '(A.NAME) as NAME1, COND_ITEMS.COND_ID, COND_ITEMS.COND_SUBID,'
      '       COND_ITEMS.ROWID'
      'from COND_ITEMS , ACCS A'
      'where '
      '        COND_ITEMS.COND_ID =         :COND_ID        AND'
      '        COND_ITEMS.COND_SUBID = :COND_SUBID  AND'
      '        A.CODE               =  COND_ITEMS.CODE')
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Variables.Data = {
      0300000002000000080000003A434F4E445F49440300000004000000F7930400
      000000000B0000003A434F4E445F535542494403000000040000000000000000
      000000}
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
    Master = QConds
    MasterFields = 'COND_ID;COND_SUBID'
    DetailFields = 'COND_ID;COND_SUBID'
    ReadOnly = False
    LockingMode = lmCheckImmediate
    QueryAllRecords = True
    CountAllRecords = False
    RefreshOptions = []
    UpdatingTable = 'COND_ITEMS'
    CommitOnPost = True
    CachedUpdates = False
    QBEMode = False
    Session = MainFrm.OSess
    DesignActivation = False
    Active = False
    Left = 80
    Top = 200
  end
  object DataSource1: TDataSource
    DataSet = QObjs
    Left = 24
    Top = 88
  end
  object QObjs: TOracleDataSet
    SQL.Strings = (
      
        'select ROWID, OID, DESCR from OBJS where CATID= :CATID and OTYPE' +
        ' = 3'
      'order by ORD')
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Variables.Data = {
      0300000001000000060000003A43415449440300000004000000010000000000
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
    QueryAllRecords = True
    CountAllRecords = False
    RefreshOptions = []
    CommitOnPost = True
    CachedUpdates = False
    QBEMode = False
    Session = MainFrm.OSess
    DesignActivation = False
    Active = False
    Left = 56
    Top = 88
  end
  object DataSource2: TDataSource
    DataSet = QConds
    Left = 344
    Top = 112
  end
  object QConds: TOracleDataSet
    SQL.Strings = (
      'select ROWID, TEXT, COND_ID, COND_SUBID, PRE_FORMULA'
      'from CONDS'
      'where COND_ID = :COND_ID'
      'order by COND_ID, COND_SUBID')
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Variables.Data = {
      0300000001000000080000003A434F4E445F49440300000004000000F7930400
      00000000}
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
    Master = QObjs
    MasterFields = 'OID'
    DetailFields = 'COND_ID'
    ReadOnly = False
    LockingMode = lmCheckImmediate
    QueryAllRecords = True
    CountAllRecords = False
    RefreshOptions = []
    CommitOnPost = True
    CachedUpdates = False
    QBEMode = False
    Session = MainFrm.OSess
    DesignActivation = False
    Active = False
    Left = 376
    Top = 112
  end
  object OQ: TOracleQuery
    Session = MainFrm.OSess
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Cursor = crDefault
    StringFieldsOnly = False
    Threaded = False
    ThreadSynchronized = True
    Left = 456
    Top = 48
  end
  object OQ2: TOracleQuery
    Session = MainFrm.OSess
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Cursor = crDefault
    StringFieldsOnly = False
    Threaded = False
    ThreadSynchronized = True
    Left = 488
    Top = 48
  end
end
