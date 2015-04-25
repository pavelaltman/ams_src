object FrmViewOptionsAW: TFrmViewOptionsAW
  Left = 333
  Top = 207
  Width = 269
  Height = 308
  Caption = 'Режимы отображения'
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
  object Label1: TLabel
    Left = 8
    Top = 96
    Width = 194
    Height = 26
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Какие счета отображать в '#13#10'развернутой корреспонденции счетов'
  end
  object RGOpt: TRadioGroup
    Left = 8
    Top = 0
    Width = 247
    Height = 65
    Anchors = [akLeft, akTop, akRight]
    ItemIndex = 2
    Items.Strings = (
      'Дебет'
      'Кредит'
      'Дебет и Кредит')
    TabOrder = 0
  end
  object ChBCorresp: TCheckBox
    Left = 8
    Top = 72
    Width = 247
    Height = 17
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Развернутая корреспонденция счетов'
    TabOrder = 1
    OnClick = ChBCorrespClick
  end
  object CBDepth: TComboBox
    Left = 8
    Top = 128
    Width = 247
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    ItemHeight = 13
    TabOrder = 2
    Text = 'Показывать листья'
    Items.Strings = (
      'Показывать листья'
      'Классы 2'
      'Подклассы 28'
      'Подклассы 281'
      'Субсчета подклассов 281/1'
      'Субсчета субсчетов 281/1/3')
  end
  object ChBGroupByDate: TCheckBox
    Left = 8
    Top = 160
    Width = 247
    Height = 17
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Группировать по датам'
    TabOrder = 3
    OnClick = ChBCorrespClick
  end
  object ChBShowAnal: TCheckBox
    Left = 8
    Top = 184
    Width = 247
    Height = 17
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Показывать аналитику'
    TabOrder = 4
  end
  object ChBSortByLastModif: TCheckBox
    Left = 8
    Top = 208
    Width = 247
    Height = 17
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Сортировать по дате последнего изменения'
    TabOrder = 5
  end
  object BBOk: TBitBtn
    Left = 7
    Top = 256
    Width = 89
    Height = 23
    Anchors = [akLeft, akBottom]
    Caption = 'Ok'
    Default = True
    TabOrder = 6
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
    Left = 158
    Top = 256
    Width = 97
    Height = 23
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = 'Выход'
    TabOrder = 7
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
  object ChBCheckOps: TCheckBox
    Left = 8
    Top = 232
    Width = 247
    Height = 17
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Проверять операции'
    TabOrder = 8
  end
  object ActionList1: TActionList
    Left = 96
    Top = 8
    object ActOk: TAction
      Caption = 'ActOk'
      ShortCut = 113
      OnExecute = ActOkExecute
    end
  end
end
