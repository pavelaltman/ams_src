object FrmFindDbPos: TFrmFindDbPos
  Left = 281
  Top = 187
  Width = 284
  Height = 318
  HelpContext = 40
  Caption = 'Поиск позиции в БД'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = FormActivate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 265
    Width = 276
    Height = 26
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    object BBCancel: TBitBtn
      Left = 79
      Top = 2
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
      Left = 2
      Top = 2
      Width = 75
      Height = 24
      Caption = 'Ok'
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
  object ToolBar1: TToolBar
    Left = 0
    Top = 0
    Width = 276
    Height = 25
    Caption = 'ToolBar1'
    Images = DatMod.ImageList1
    ParentShowHint = False
    ShowHint = True
    TabOrder = 1
    object ToolButton1: TToolButton
      Left = 0
      Top = 2
      Action = ActDelAllCond
    end
  end
  object SBox: TScrollBox
    Left = 0
    Top = 25
    Width = 276
    Height = 240
    Align = alClient
    TabOrder = 2
  end
  object ActionList1: TActionList
    Images = DatMod.ImageList1
    Left = 232
    Top = 248
    object ActDelAllCond: TAction
      Hint = 'Очистить все'
      ImageIndex = 112
      ShortCut = 119
      OnExecute = ActDelAllCondExecute
    end
    object ActInsAndExec: TAction
      Caption = 'ActInsAndExec'
      ShortCut = 113
      OnExecute = ActInsAndExecExecute
    end
    object ActAddCond: TAction
      Hint = 'Добавить условие'
      ImageIndex = 3
      ShortCut = 115
    end
    object ActDelCond: TAction
      Hint = 'Удалить условие'
      ShortCut = 120
    end
  end
end
