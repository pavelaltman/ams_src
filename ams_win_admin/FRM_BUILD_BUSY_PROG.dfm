object FrmBuildBusyProg: TFrmBuildBusyProg
  Left = 322
  Top = 198
  Width = 624
  Height = 379
  Caption = 'Строим функцию свободного члена для всех баз данных бухгалтерии'
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
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object MemSql: TMemo
    Left = 0
    Top = 0
    Width = 616
    Height = 323
    Align = alClient
    Lines.Strings = (
      'MemSql')
    ScrollBars = ssBoth
    TabOrder = 0
    WordWrap = False
  end
  object Panel1: TPanel
    Left = 0
    Top = 323
    Width = 616
    Height = 29
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    object BBCancel: TBitBtn
      Left = 176
      Top = 3
      Width = 97
      Height = 24
      Cancel = True
      Caption = 'Отменить'
      TabOrder = 0
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
    object BBApply: TBitBtn
      Left = 12
      Top = 3
      Width = 97
      Height = 24
      Hint = 'Применить'
      Caption = 'Применить'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      OnClick = BBApplyClick
      Glyph.Data = {
        F6000000424DF600000000000000760000002800000010000000100000000100
        0400000000008000000000000000000000001000000000000000000000000000
        8000008000000080800080000000800080008080000080808000C0C0C0000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00FFF07FFFFFFF
        FFFFFF007777777777FFF0000000000007FFF0000000000007FFFF00FFF00000
        077FFFF0FFFFFF00007FFFFFFFFFFF00007FFFFFFFFFFF00007FFFFFFFFFFF00
        007FFFFFFFFFFF00007FFFFFFFFFFF000077FFFFFFFFF0000007FFFFFFFFF000
        0007FFFFFFFFF000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
    end
  end
  object DBQ: TOracleQuery
    SQL.Strings = (
      'select NAME, OID from OBJS '
      'where OTYPE = 4'
      'order by ORD')
    Session = MainFrm.OSess
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Cursor = crDefault
    StringFieldsOnly = False
    Threaded = False
    ThreadSynchronized = True
    Left = 64
    Top = 48
  end
  object AnQ: TOracleQuery
    SQL.Strings = (
      'select NAME, OID from OBJS where OTYPE = 5 and PARENT = :POID ')
    Session = MainFrm.OSess
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Variables.Data = {
      0300000001000000050000003A504F49440300000004000000B9000000000000
      00}
    Cursor = crDefault
    StringFieldsOnly = False
    Threaded = False
    ThreadSynchronized = True
    Left = 24
    Top = 48
  end
  object ODbLinks: TOracleQuery
    SQL.Strings = (
      'select D.REALNAME, O.NAME '
      'from DBSTR D, OBJS O  '
      'where '
      #9'D.RDBID = :DB_ID and '
      #9'O.OID = D.DBID')
    Session = MainFrm.OSess
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Variables.Data = {0300000001000000060000003A44425F4944030000000000000000000000}
    Cursor = crDefault
    StringFieldsOnly = False
    Threaded = False
    ThreadSynchronized = True
    Left = 64
    Top = 88
  end
end
