object FrmOperHist: TFrmOperHist
  Left = 264
  Top = 220
  Width = 480
  Height = 231
  HelpContext = 8
  Caption = 'История операции'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 177
    Width = 472
    Height = 27
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    object BBOk: TBitBtn
      Left = 2
      Top = 2
      Width = 75
      Height = 24
      Cancel = True
      Caption = 'Ok'
      ModalResult = 1
      TabOrder = 0
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
  end
  object RxDBGrid1: TRxDBGrid
    Left = 0
    Top = 0
    Width = 472
    Height = 177
    Align = alClient
    DataSource = DataSource1
    Options = [dgColumnResize, dgColLines, dgRowLines, dgRowSelect, dgConfirmDelete, dgCancelOnExit]
    ReadOnly = True
    TabOrder = 1
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
    Columns = <
      item
        Expanded = False
        FieldName = 'REV_TIME'
        Width = 108
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'USERNAME'
        Width = 114
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'FULL_NAME'
        Width = 283
        Visible = True
      end>
  end
  object ODS: TOracleDataSet
    SQL.Strings = (
      'select H.REVISION, H.REV_TIME, H.USERNAME, U.FULL_NAME'
      'from history H, USER_OPTIONS U'
      'where '
      #9'H.OP_ID = :OP_ID and'
      #9'H.OP_SUBID = :OP_SUBID and'
      #9'H.USERNAME = U.USERNAME')
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Variables.Data = {
      0300000002000000060000003A4F505F49440300000000000000000000000900
      00003A4F505F5355424944030000000000000000000000}
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
    Session = DatMod.OSession
    DesignActivation = True
    Active = False
    Left = 8
    Top = 8
  end
  object DataSource1: TDataSource
    DataSet = ODS
    Left = 32
    Top = 8
  end
end
