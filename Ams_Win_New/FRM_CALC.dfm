object FrmCalc: TFrmCalc
  Left = 255
  Top = 193
  Width = 335
  Height = 134
  Caption = 'Калькулятор'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 11
    Top = 36
    Width = 52
    Height = 13
    Caption = 'Результат'
  end
  object ChB: TComboBox
    Left = 7
    Top = 8
    Width = 312
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    ItemHeight = 13
    TabOrder = 0
  end
  object EdRes: TEdit
    Left = 72
    Top = 32
    Width = 249
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 1
  end
  object ButCalc: TButton
    Left = 8
    Top = 81
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'Вычислить'
    Default = True
    TabOrder = 2
    OnClick = ButCalcClick
  end
  object ButClose: TButton
    Left = 244
    Top = 81
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = 'Закрыть'
    TabOrder = 3
    OnClick = ButCloseClick
  end
  object ChBRoundUp: TCheckBox
    Left = 11
    Top = 58
    Width = 126
    Height = 17
    Caption = 'Десятичные цифры:'
    Checked = True
    State = cbChecked
    TabOrder = 4
    OnClick = ChBRoundUpClick
  end
  object SEKolZnak: TRxSpinEdit
    Left = 140
    Top = 56
    Width = 41
    Height = 21
    Hint = 'Округлять до ... знаков после запятой'
    MaxValue = 6
    Value = 2
    ParentShowHint = False
    ShowHint = True
    TabOrder = 5
    OnChange = SEKolZnakChange
  end
  object ActionList1: TActionList
    Left = 88
    Top = 80
    object ActOk: TAction
      Caption = 'ActOk'
      ShortCut = 113
      OnExecute = ActOkExecute
    end
  end
  object FormStorage1: TFormStorage
    IniFileName = 'amswin.ini'
    StoredProps.Strings = (
      'SEKolZnak.Value')
    StoredValues = <>
    Left = 128
    Top = 80
  end
end
