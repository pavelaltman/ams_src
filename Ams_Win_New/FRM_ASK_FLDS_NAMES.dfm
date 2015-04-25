object FrmAskFldsNames: TFrmAskFldsNames
  Left = 362
  Top = 200
  Width = 384
  Height = 306
  Caption = 'Поставить соответсвие копируемых полей'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object SG: TAmsStringGrid
    Left = 0
    Top = 56
    Width = 372
    Height = 190
    Anchors = [akLeft, akTop, akRight, akBottom]
    DefaultRowHeight = 18
    FixedCols = 0
    RowCount = 10
    FixedRows = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing]
    TabOrder = 0
    OnMouseDown = SGMouseDown
  end
  object CBColName: TComboBox
    Left = 0
    Top = 32
    Width = 49
    Height = 21
    Hint = 'Название колонки Excel'
    ItemHeight = 13
    ParentShowHint = False
    ShowHint = True
    TabOrder = 1
    Text = 'CBColName'
  end
  object CBFldName: TComboBox
    Left = 56
    Top = 32
    Width = 236
    Height = 21
    Hint = 'Имя поля базы данных'
    Anchors = [akLeft, akTop, akRight]
    ItemHeight = 13
    ParentShowHint = False
    ShowHint = True
    TabOrder = 2
    Text = 'CBFldName'
  end
  object ButSet: TButton
    Left = 296
    Top = 29
    Width = 76
    Height = 25
    Anchors = [akTop, akRight]
    Caption = 'Установить'
    TabOrder = 3
    OnClick = ButSetClick
  end
  object BitBtn1: TBitBtn
    Left = 291
    Top = 253
    Width = 84
    Height = 25
    Anchors = [akRight, akBottom]
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
    Top = 253
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'Ok'
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
  object ToolBar1: TToolBar
    Left = 0
    Top = 0
    Width = 376
    Height = 29
    Caption = 'ToolBar1'
    Images = DatMod.ImageList1
    ParentShowHint = False
    ShowHint = True
    TabOrder = 6
    object ToolButton1: TToolButton
      Left = 0
      Top = 2
      Action = ActReadFlds
    end
    object ToolButton2: TToolButton
      Left = 23
      Top = 2
      Action = ActWriteFlds
    end
  end
  object ActionList1: TActionList
    Images = DatMod.ImageList1
    Left = 168
    Top = 160
    object ActReadFlds: TAction
      Caption = 'Прочитать последовательность полей'
      Hint = 'Прочитать сохраненную последовательность полей'
      ImageIndex = 67
      OnExecute = ActReadFldsExecute
    end
    object ActWriteFlds: TAction
      Caption = 'Записать последовательность полей'
      Hint = 'Записать последовательность полей на сервер'
      ImageIndex = 96
      OnExecute = ActWriteFldsExecute
    end
  end
end
