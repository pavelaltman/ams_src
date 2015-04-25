object FrmEditGlobalSetup: TFrmEditGlobalSetup
  Left = 220
  Top = 153
  Width = 653
  Height = 397
  Caption = 'Глобальные установки'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object OracleNavigator1: TOracleNavigator
    Left = 0
    Top = 0
    Width = 645
    Height = 25
    DataSource = DataSource1
    VisibleButtons = [nbFirst, nbPrior, nbNext, nbLast, nbInsert, nbDelete, nbEdit, nbPost, nbCancel, nbEnterQBE, nbExecuteQBE, nbRefresh]
    Align = alTop
    Flat = True
    TabOrder = 0
  end
  object DBGrid1: TDBGrid
    Left = 0
    Top = 25
    Width = 645
    Height = 345
    Align = alClient
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
        FieldName = 'VAL_TYPE'
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'VALUE'
        Width = 178
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'VAL_DESCR'
        Visible = True
      end>
  end
  object DataSource1: TDataSource
    DataSet = ODS
    Left = 16
  end
  object ODS: TOracleDataSet
    SQL.Strings = (
      'select S.ROWID, S.* from SYS_INFO S'
      ' order by S.VALUE ')
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
      03000000030000000800000056414C5F5459504501000000000500000056414C
      554501000000000900000056414C5F44455343520100000000}
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
    Left = 48
    object ODSVAL_TYPE: TStringField
      DisplayLabel = 'Переменная'
      FieldName = 'VAL_TYPE'
      Required = True
    end
    object ODSVALUE: TStringField
      DisplayLabel = 'Значение'
      DisplayWidth = 50
      FieldName = 'VALUE'
      Size = 250
    end
    object ODSVAL_DESCR: TStringField
      DisplayLabel = 'Описание переменной'
      DisplayWidth = 50
      FieldName = 'VAL_DESCR'
      Size = 255
    end
  end
end
