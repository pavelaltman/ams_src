object FrmEditBuchSetup: TFrmEditBuchSetup
  Left = 225
  Top = 178
  Width = 591
  Height = 394
  Caption = 'Установки бухгалтерии'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object OracleNavigator1: TOracleNavigator
    Left = 0
    Top = 0
    Width = 583
    Height = 25
    DataSource = DataSource1
    VisibleButtons = [nbFirst, nbPrior, nbNext, nbLast, nbInsert, nbDelete, nbEdit, nbPost, nbCancel, nbEnterQBE, nbExecuteQBE, nbRefresh]
    Align = alTop
    TabOrder = 0
  end
  object DBGrid1: TDBGrid
    Left = 0
    Top = 25
    Width = 583
    Height = 80
    Align = alTop
    DataSource = DataSource1
    TabOrder = 1
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
    Columns = <
      item
        Expanded = False
        FieldName = 'ID'
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'FIRM'
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'DOC_PATH'
        Width = 329
        Visible = True
      end>
  end
  object OracleNavigator2: TOracleNavigator
    Left = 0
    Top = 105
    Width = 583
    Height = 25
    DataSource = DataSource2
    VisibleButtons = [nbFirst, nbPrior, nbNext, nbLast, nbInsert, nbDelete, nbEdit, nbPost, nbCancel, nbEnterQBE, nbExecuteQBE, nbRefresh]
    Align = alTop
    TabOrder = 2
  end
  object AmsDBGrid1: TAmsDBGrid
    Left = 0
    Top = 130
    Width = 583
    Height = 237
    Align = alClient
    DataSource = DataSource2
    TabOrder = 3
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
    DefaultRowHeight = 17
    Columns = <
      item
        Expanded = False
        FieldName = 'ID'
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'SUB_ID'
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'BASE_USER'
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'DESCR'
        Width = 191
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'STATUS'
        PickList.Strings = (
          'АКТИВНА'
          'ВРЕМЕННАЯ'
          'АРХИВ')
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'LAST_UPD'
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'SYS_BASE_USER'
        Visible = True
      end>
  end
  object DataSource1: TDataSource
    DataSet = ODSLic
    Left = 8
  end
  object ODSLic: TOracleDataSet
    SQL.Strings = (
      'select L.ROWID, L.* from AMS.LICENSES L')
    ReadBuffer = 25
    Optimize = True
    Debug = False
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
    QBEDefinition.QBEFieldDefs = {
      03000000030000000200000049440100000000040000004649524D0100000000
      08000000444F435F504154480100000000}
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
    Left = 32
    object ODSLicID: TIntegerField
      DisplayLabel = 'Код'
      DisplayWidth = 5
      FieldName = 'ID'
      Required = True
    end
    object ODSLicFIRM: TStringField
      DisplayLabel = 'Название фирмы'
      DisplayWidth = 30
      FieldName = 'FIRM'
      Required = True
      Size = 250
    end
    object ODSLicDOC_PATH: TStringField
      DisplayLabel = 'Путь к документам (*.prg)'
      DisplayWidth = 50
      FieldName = 'DOC_PATH'
      Size = 200
    end
  end
  object DataSource2: TDataSource
    DataSet = ODSBooks
    Left = 24
    Top = 120
  end
  object ODSBooks: TOracleDataSet
    SQL.Strings = (
      'select B.ROWID, B.* from AMS.BOOKS B where id = :id')
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Variables.Data = {0300000001000000030000003A494403000000040000000100000000000000}
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
    QBEDefinition.QBEFieldDefs = {
      03000000070000000200000049440100000000060000005355425F4944010000
      000009000000424153455F5553455201000000000D0000005359535F42415345
      5F555345520100000000060000005354415455530100000000080000004C4153
      545F55504401000000000500000044455343520100000000}
    Cursor = crDefault
    Master = ODSLic
    MasterFields = 'ID'
    DetailFields = 'ID'
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
    Top = 120
    object ODSBooksID: TIntegerField
      DisplayLabel = 'Код'
      DisplayWidth = 5
      FieldName = 'ID'
      Required = True
    end
    object ODSBooksSUB_ID: TIntegerField
      DisplayLabel = 'Субкод'
      DisplayWidth = 5
      FieldName = 'SUB_ID'
      Required = True
    end
    object ODSBooksBASE_USER: TStringField
      DisplayLabel = 'Имя базового пользователя'
      DisplayWidth = 10
      FieldName = 'BASE_USER'
      Required = True
      Size = 30
    end
    object ODSBooksSYS_BASE_USER: TStringField
      DisplayWidth = 10
      FieldName = 'SYS_BASE_USER'
      Required = True
      Size = 30
    end
    object ODSBooksSTATUS: TStringField
      DisplayWidth = 10
      FieldName = 'STATUS'
      Required = True
    end
    object ODSBooksLAST_UPD: TStringField
      DisplayWidth = 10
      FieldName = 'LAST_UPD'
    end
    object ODSBooksDESCR: TStringField
      DisplayWidth = 50
      FieldName = 'DESCR'
      Size = 255
    end
  end
end
