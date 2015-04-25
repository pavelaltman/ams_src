object FrmAccsTies: TFrmAccsTies
  Left = 326
  Top = 207
  Width = 557
  Height = 319
  Caption = 'Соответствие счетов в бухгалтериях'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDefault
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 333
    Top = 0
    Width = 3
    Height = 265
    Cursor = crHSplit
    Align = alRight
    OnCanResize = Splitter1CanResize
  end
  object StrGrid: TStringGrid
    Left = 0
    Top = 0
    Width = 333
    Height = 265
    Hint = 'Счета из проводки'
    Align = alClient
    ColCount = 3
    FixedCols = 2
    RowCount = 1
    FixedRows = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goColSizing, goEditing]
    ParentShowHint = False
    ShowHint = True
    TabOrder = 0
    OnDblClick = StrGridDblClick
    ColWidths = (
      64
      64
      138)
  end
  object Panel1: TPanel
    Left = 0
    Top = 265
    Width = 549
    Height = 27
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    object BBCancel: TBitBtn
      Left = 78
      Top = 0
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
    object BBOk: TBitBtn
      Left = 0
      Top = 0
      Width = 75
      Height = 24
      Caption = 'Ok'
      Default = True
      TabOrder = 1
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
  end
  object AccView: TAmsAccsView
    Left = 336
    Top = 0
    Width = 213
    Height = 265
    Hint = 'Счета из бухгалтерии куда копируем проводку'
    Align = alRight
    Indent = 19
    ParentShowHint = False
    ShowHint = True
    TabOrder = 2
    OnChange = AccViewChange
    AmsOraQuery = OQ
  end
  object ActionList1: TActionList
    Images = MainFrm.ImageList1
    Left = 224
    Top = 56
    object ActSelAcc: TAction
      Caption = 'ActSelAcc'
      Hint = 'Выбрать счет из списка'
      ShortCut = 49219
    end
  end
  object OQ: TOracleQuery
    SQL.Strings = (
      'insert into ACCS_TIES'
      'values (:WHERE_COPY, :MY_ACC, :WHERE_ACC); '
      'commit;')
    Session = MainFrm.OSess
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Variables.Data = {
      03000000030000000B0000003A57484552455F434F505905000000070000004E
      4557434F4D0000000000070000003A4D595F4143430500000006000000422F32
      2F3800000000000A0000003A57484552455F4143430500000006000000422F32
      2F380000000000}
    Cursor = crDefault
    StringFieldsOnly = False
    Threaded = False
    ThreadSynchronized = True
    Left = 232
    Top = 8
  end
end
