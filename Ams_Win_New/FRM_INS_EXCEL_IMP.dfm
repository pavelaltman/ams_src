object FrmInsExcelImp: TFrmInsExcelImp
  Left = 289
  Top = 153
  Width = 342
  Height = 342
  Caption = 'Сохраняем установки импорта'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 0
    Width = 99
    Height = 13
    Hint = 'Краткое название (должно быть уникальным)'
    Caption = 'Название импорта:'
  end
  object Label2: TLabel
    Left = 8
    Top = 40
    Width = 153
    Height = 13
    Caption = 'Подробное описание импорта'
  end
  object EdName: TEdit
    Left = 0
    Top = 14
    Width = 329
    Height = 21
    Hint = 'Краткое название (должно быть уникальным)'
    MaxLength = 30
    ParentShowHint = False
    ShowHint = True
    TabOrder = 0
  end
  object MemDescr: TMemo
    Left = 0
    Top = 56
    Width = 329
    Height = 105
    Hint = 'Подробное описание импорта'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 1
  end
  object DBGrid1: TDBGrid
    Left = 0
    Top = 199
    Width = 169
    Height = 113
    Anchors = [akLeft, akBottom]
    DataSource = DataSource1
    Options = [dgTitles, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgConfirmDelete, dgCancelOnExit]
    TabOrder = 2
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
    Columns = <
      item
        Expanded = False
        FieldName = 'SHORT_NAME'
        Width = 139
        Visible = True
      end>
  end
  object DBMemo1: TDBMemo
    Left = 176
    Top = 199
    Width = 150
    Height = 113
    Anchors = [akLeft, akRight, akBottom]
    DataField = 'DESCR'
    DataSource = DataSource1
    ReadOnly = True
    TabOrder = 3
  end
  object BitBtn1: TBitBtn
    Left = 246
    Top = 168
    Width = 84
    Height = 25
    Anchors = [akTop, akRight]
    Cancel = True
    Caption = 'Отменить'
    TabOrder = 4
    OnClick = BitBtn1Click
    Glyph.Data = {
      76010000424D7601000000000000760000002800000020000000100000000100
      04000000000000010000130B0000130B00001000000000000000000000000000
      800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
      3333333333FFFFF3333333333999993333333333F77777FFF333333999999999
      3333333777333777FF3333993333339993333377FF3333377FF3399993333339
      993337777FF3333377F3393999333333993337F777FF333337FF993399933333
      399377F3777FF333377F993339993333399377F33777FF33377F993333999333
      399377F333777FF3377F993333399933399377F3333777FF377F993333339993
      399377FF3333777FF7733993333339993933373FF3333777F7F3399933333399
      99333773FF3333777733339993333339933333773FFFFFF77333333999999999
      3333333777333777333333333999993333333333377777333333}
    NumGlyphs = 2
  end
  object BBOk: TBitBtn
    Left = 0
    Top = 168
    Width = 75
    Height = 25
    Caption = 'Ok'
    Default = True
    ModalResult = 1
    TabOrder = 5
    OnClick = BBOkClick
    Glyph.Data = {
      76010000424D7601000000000000760000002800000020000000100000000100
      04000000000000010000120B0000120B00001000000000000000000000000000
      8000008000000080800080000000800080008080000080808000C0C0C0000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00FFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFAAFFFFFFFFFFFFFF66FFFFFFFFFFFFFFAAFF
      FFFFFFFFFFFF66FFFFFFFFFFFFFAAAAFFFFFFFFFFFF6666FFFFFFFFFFFFAAAAA
      FFFFFFFFFFF66666FFFFFFFFFFAA22AAFFFFFFFFFF662266FFFFFFFFFFAA2FAA
      AFFFFFFFFF662F666FFFFFFFFAAA2FFAA2FFFFFFF6662FF662FFFFFFAAA2FFFF
      A2FFFFFF6662FFFF62FFFFFFAAA2FFFFAA2FFFFF6662FFFF662FFFFFF222FFFF
      FA2FFFFFF222FFFFF62FFFFFFFFFFFFFFFA2FFFFFFFFFFFFFF62FFFFFFFFFFFF
      FFA2FFFFFFFFFFFFFF62FFFFFFFFFFFFFFF2FFFFFFFFFFFFFFF2FFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
    NumGlyphs = 2
  end
  object ODS: TOracleDataSet
    SQL.Strings = (
      'select * from EXCEL_IMP')
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
      03000000030000000500000044425F494401000000000A00000053484F52545F
      4E414D4501000000000500000044455343520100000000}
    Cursor = crDefault
    ReadOnly = True
    LockingMode = lmCheckImmediate
    QueryAllRecords = True
    CountAllRecords = False
    RefreshOptions = []
    CommitOnPost = True
    CachedUpdates = False
    QBEMode = False
    Session = DatMod.OSession
    DesignActivation = False
    Active = False
    Left = 40
    Top = 200
    object ODSDB_ID: TIntegerField
      FieldName = 'DB_ID'
      Required = True
    end
    object ODSSHORT_NAME: TStringField
      DisplayLabel = 'Краткое имя'
      FieldName = 'SHORT_NAME'
      Required = True
    end
    object ODSDESCR: TStringField
      DisplayLabel = 'Описание'
      FieldName = 'DESCR'
      Size = 255
    end
  end
  object DataSource1: TDataSource
    DataSet = ODS
    Left = 80
    Top = 200
  end
end
