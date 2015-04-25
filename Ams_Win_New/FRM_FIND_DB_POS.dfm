object FrmFindDbPos: TFrmFindDbPos
  Left = 62
  Top = 129
  Width = 580
  Height = 293
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
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 121
    Top = 0
    Width = 3
    Height = 240
    Cursor = crHSplit
  end
  object Panel1: TPanel
    Left = 0
    Top = 240
    Width = 572
    Height = 26
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 2
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
  object Panel2: TPanel
    Left = 124
    Top = 0
    Width = 193
    Height = 240
    Align = alLeft
    BevelOuter = bvNone
    TabOrder = 1
    object Label2: TLabel
      Left = 8
      Top = 152
      Width = 91
      Height = 13
      Caption = 'С чем сравнивать'
    end
    object SpeedButton1: TSpeedButton
      Left = 8
      Top = 0
      Width = 22
      Height = 22
      Action = ActDelAllCond
      Glyph.Data = {
        F6000000424DF600000000000000760000002800000010000000100000000100
        0400000000008000000000000000000000001000000000000000000000000000
        8000008000000080800080000000800080008080000080808000C0C0C0000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
        8888888888888888888888888888888888888888881788888887888888117888
        8817888888711888871888888881178811788888000711771188888806081111
        1888888808087111888888880877111178888880811118811788880881167888
        1178806FF8886088811880000000008888888888888888888888}
      ParentShowHint = False
      ShowHint = True
    end
    object RGSign: TRadioGroup
      Left = 8
      Top = 24
      Width = 169
      Height = 81
      Anchors = [akLeft, akTop, akRight, akBottom]
      Items.Strings = (
        '= Равно '
        '> Больше'
        '< Меньше'
        '~ Приблизительно равно')
      TabOrder = 4
    end
    object ChBCaseSensive: TCheckBox
      Left = 8
      Top = 112
      Width = 177
      Height = 17
      Anchors = [akLeft, akTop, akRight]
      Caption = 'Учитывать регистр'
      TabOrder = 5
    end
    object AIDate: TAmsInputDate
      Left = 0
      Top = 176
      Width = 193
      Height = 24
      AmsMaxLen = 10
      AmsFldIndex = 0
      AmsEnabled = True
      AmsIsNull = True
      Visible = True
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 2
      AmsPromptLen = 40
    end
    object AISum: TAmsInputSum
      Left = 0
      Top = 176
      Width = 193
      Height = 24
      AmsFldIndex = 0
      AmsEnabled = True
      AmsIsNull = True
      Visible = True
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
      AmsPromptLen = 40
    end
    object AIInt: TAmsInputInt
      Left = 0
      Top = 176
      Width = 193
      Height = 24
      AmsMaxLen = 9
      AmsFldIndex = 0
      AmsEnabled = True
      AmsIsNull = True
      Visible = True
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 1
      AmsPromptLen = 40
    end
    object AIStr: TAmsInputStr
      Left = 0
      Top = 176
      Width = 193
      Height = 24
      AmsFldIndex = 0
      AmsEnabled = True
      AmsIsNull = True
      Visible = True
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 3
      AmsPromptLen = 40
    end
    object BitBtn1: TBitBtn
      Left = 152
      Top = 0
      Width = 22
      Height = 22
      Action = ActAddCond
      ParentShowHint = False
      ShowHint = True
      TabOrder = 6
      Glyph.Data = {
        76010000424D7601000000000000760000002800000020000000100000000100
        04000000000000010000120B0000120B00001000000000000000000000000000
        800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
        33333FF3333333333333447333333333333377FFF33333333333744473333333
        333337773FF3333333333444447333333333373F773FF3333333334444447333
        33333373F3773FF3333333744444447333333337F333773FF333333444444444
        733333373F3333773FF333334444444444733FFF7FFFFFFF77FF999999999999
        999977777777777733773333CCCCCCCCCC3333337333333F7733333CCCCCCCCC
        33333337F3333F773333333CCCCCCC3333333337333F7733333333CCCCCC3333
        333333733F77333333333CCCCC333333333337FF7733333333333CCC33333333
        33333777333333333333CC333333333333337733333333333333}
      NumGlyphs = 2
    end
    object BBDel: TBitBtn
      Left = 128
      Top = 0
      Width = 22
      Height = 22
      Action = ActDelCond
      ParentShowHint = False
      ShowHint = True
      TabOrder = 7
      Glyph.Data = {
        76010000424D7601000000000000760000002800000020000000100000000100
        04000000000000010000130B0000130B00001000000000000000000000000000
        800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
        333333333333333333333333333333333333333FFF33FF333FFF339993370733
        999333777FF37FF377733339993000399933333777F777F77733333399970799
        93333333777F7377733333333999399933333333377737773333333333990993
        3333333333737F73333333333331013333333333333777FF3333333333910193
        333333333337773FF3333333399000993333333337377737FF33333399900099
        93333333773777377FF333399930003999333337773777F777FF339993370733
        9993337773337333777333333333333333333333333333333333333333333333
        3333333333333333333333333333333333333333333333333333}
      NumGlyphs = 2
    end
  end
  object SGrid: TStringGrid
    Left = 317
    Top = 0
    Width = 255
    Height = 240
    Align = alClient
    ColCount = 3
    DefaultRowHeight = 18
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goDrawFocusSelected, goColSizing, goRowSelect]
    TabOrder = 3
    ColWidths = (
      111
      26
      96)
  end
  object LBFieldsNames: TListBox
    Left = 0
    Top = 0
    Width = 121
    Height = 240
    Align = alLeft
    ExtendedSelect = False
    ItemHeight = 13
    TabOrder = 0
    OnClick = LBFieldsNamesClick
    OnKeyDown = LBFieldsNamesKeyUp
    OnKeyUp = LBFieldsNamesKeyUp
  end
  object OQ: TOracleQuery
    Session = DatMod.OSession
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Cursor = crDefault
    StringFieldsOnly = False
    Threaded = False
    ThreadSynchronized = True
    Left = 96
    Top = 88
  end
  object ActionList1: TActionList
    Images = DatMod.ImageList1
    Left = 256
    Top = 168
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
      OnExecute = SbSaveClick
    end
    object ActDelCond: TAction
      Hint = 'Удалить условие'
      ShortCut = 120
      OnExecute = SBDelClick
    end
  end
end
