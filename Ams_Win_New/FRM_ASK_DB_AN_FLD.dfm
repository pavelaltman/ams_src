object FrmAskDbAnFld: TFrmAskDbAnFld
  Left = 267
  Top = 148
  Width = 309
  Height = 255
  Caption = 'Выбор поля для копирования'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  PixelsPerInch = 96
  TextHeight = 13
  object BBOk: TBitBtn
    Left = 0
    Top = 204
    Width = 75
    Height = 24
    Anchors = [akLeft, akBottom]
    Caption = 'Ok'
    Default = True
    TabOrder = 0
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
  object BBCancel: TBitBtn
    Left = 201
    Top = 204
    Width = 97
    Height = 24
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = 'Отменить'
    TabOrder = 1
    OnClick = BBCancelClick
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
  object AmsDBGrid1: TAmsDBGrid
    Left = 0
    Top = 0
    Width = 298
    Height = 197
    Anchors = [akLeft, akTop, akRight, akBottom]
    DataSource = DataSource1
    Options = [dgTitles, dgColumnResize, dgColLines, dgRowLines, dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit]
    TabOrder = 2
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
    DefaultRowHeight = 17
    Columns = <
      item
        Expanded = False
        FieldName = 'DESCR'
        Width = 163
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'ALIAS'
        Width = 110
        Visible = True
      end>
  end
  object ODS: TOracleDataSet
    SQL.Strings = (
      
        'select O.NAME,O.DESCR, D.REALNAME, D.ALIAS, D.DBID from DBSTR D,' +
        ' OBJS O '
      'where '
      #9'VTYPE=3 and RDBID=:DBID and '
      #9'HavePriv(DBID)=1 and O.OID = D.DBID')
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Variables.Data = {
      0300000001000000050000003A444249440300000004000000CE010000000000
      00}
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
      0300000005000000040000004E414D4501000000000500000044455343520100
      000000080000005245414C4E414D45010000000005000000414C494153010000
      000004000000444249440100000000}
    Cursor = crDefault
    ReadOnly = False
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
    Left = 264
    Top = 8
    object ODSNAME: TStringField
      FieldName = 'NAME'
    end
    object ODSDESCR: TStringField
      DisplayLabel = 'База данных'
      FieldName = 'DESCR'
      Size = 200
    end
    object ODSREALNAME: TStringField
      FieldName = 'REALNAME'
      Required = True
      Size = 30
    end
    object ODSALIAS: TStringField
      DisplayLabel = 'Поле'
      FieldName = 'ALIAS'
      Size = 40
    end
    object ODSDBID: TIntegerField
      FieldName = 'DBID'
      Required = True
    end
  end
  object DataSource1: TDataSource
    DataSet = ODS
    Left = 264
    Top = 32
  end
end
