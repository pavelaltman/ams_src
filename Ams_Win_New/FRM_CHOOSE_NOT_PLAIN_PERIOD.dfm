object FrmChooseNotPlainPeriod: TFrmChooseNotPlainPeriod
  Left = 380
  Top = 230
  Width = 217
  Height = 162
  Caption = 'Установка периода'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 9
    Width = 37
    Height = 13
    Caption = 'Начало'
  end
  object Label2: TLabel
    Left = 8
    Top = 36
    Width = 31
    Height = 13
    Caption = 'Конец'
  end
  object DEBeg: TDateEdit
    Left = 72
    Top = 3
    Width = 121
    Height = 21
    Hint = 
      'Начало промежутка видимости'#13#10'(операции за этот день будут показы' +
      'ваться)'
    DialogTitle = 'Выберите дату'
    NumGlyphs = 2
    ParentShowHint = False
    ShowHint = True
    Weekends = [Sun, Sat]
    TabOrder = 0
    OnChange = DEBegChange
  end
  object DEEnd: TDateEdit
    Left = 73
    Top = 29
    Width = 121
    Height = 21
    Hint = 
      'Конец промежутка видимости'#13#10'(операции за этот день уже не будут ' +
      'показываться)'
    NumGlyphs = 2
    ParentShowHint = False
    ShowHint = True
    Weekends = [Sun, Sat]
    TabOrder = 1
  end
  object ChBSyntOnly: TCheckBox
    Left = 8
    Top = 77
    Width = 200
    Height = 17
    Hint = 
      'Выполнять показ только для балансового поля '#13#10'(значительно быстр' +
      'ее отрабатывает, когда включена эта опция)'
    Caption = 'Только балансовое поле'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 2
  end
  object BitBtn1: TBitBtn
    Left = 124
    Top = 108
    Width = 84
    Height = 24
    Anchors = [akLeft, akBottom]
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
    Left = 2
    Top = 108
    Width = 75
    Height = 24
    Anchors = [akLeft, akBottom]
    Caption = 'Ok'
    Default = True
    ModalResult = 1
    TabOrder = 3
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
  object ChBJustAdd: TCheckBox
    Left = 8
    Top = 56
    Width = 121
    Height = 17
    Caption = 'Прибавить дней'
    TabOrder = 5
    OnClick = ChBJustAddClick
  end
  object SpEdit: TRxSpinEdit
    Left = 139
    Top = 53
    Width = 41
    Height = 21
    Hint = 'Сколько дней прибавлять'
    Value = 1
    Enabled = False
    TabOrder = 6
    OnChange = SpEditChange
  end
  object ActionList1: TActionList
    Left = 48
    Top = 24
    object Action1: TAction
      Caption = 'Action1'
      ShortCut = 113
      OnExecute = Action1Execute
    end
  end
end
