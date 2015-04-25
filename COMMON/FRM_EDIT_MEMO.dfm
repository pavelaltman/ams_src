object FrmEditMemo: TFrmEditMemo
  Left = 260
  Top = 140
  Width = 627
  Height = 449
  Caption = 'FrmEditMemo'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnClose = FormClose
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 367
    Width = 619
    Height = 36
    Align = alBottom
    TabOrder = 0
    object BBCancel: TBitBtn
      Left = 86
      Top = 8
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
      Left = 8
      Top = 8
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
  object Mem: TRichEdit
    Left = 0
    Top = 0
    Width = 619
    Height = 367
    Align = alClient
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    PlainText = True
    ScrollBars = ssBoth
    TabOrder = 1
    WantTabs = True
    WordWrap = False
  end
  object MainMenu1: TMainMenu
    Left = 416
    Top = 16
    object N1: TMenuItem
      Caption = 'Файл'
      object N2: TMenuItem
        Caption = 'Считать'
        ShortCut = 16466
        OnClick = N2Click
      end
      object N3: TMenuItem
        Caption = 'Записать'
        ShortCut = 16467
        OnClick = N3Click
      end
      object N4: TMenuItem
        Caption = '-'
      end
      object N6: TMenuItem
        Caption = 'Печать'
        OnClick = N6Click
      end
      object N5: TMenuItem
        Caption = 'Сохранить и выйти'
        ShortCut = 113
        OnClick = N5Click
      end
    end
  end
  object OD: TOpenDialog
    Filter = 
      'AMS файлы (*.ams)|*.ams|SQL файлы(*.sql)|*.sql|Текстовые файлы (' +
      '*.txt)|*.txt|Все файлы (*.*)|*.*'
    Options = [ofHideReadOnly, ofNoNetworkButton, ofEnableSizing]
    Title = 'Какой файл загрузить?'
    Left = 40
    Top = 16
  end
  object SD: TSaveDialog
    Filter = 
      'AMS файлы (*.ams)|*.ams|SQL файлы(*.sql)|*.sql|Текстовые файлы (' +
      '*.txt)|*.txt|Все файлы (*.*)|*.*'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofPathMustExist, ofNoNetworkButton, ofEnableSizing]
    Title = 'Под каким именем сохранить?'
    Left = 72
    Top = 16
  end
  object PrintDialog1: TPrintDialog
    Left = 240
    Top = 32
  end
end
