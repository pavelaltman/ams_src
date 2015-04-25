object FrmGenSetup: TFrmGenSetup
  Left = 488
  Top = 197
  Width = 188
  Height = 118
  Caption = 'FrmGenSetup'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object ChBAlwaysYes: TCheckBox
    Left = 8
    Top = 32
    Width = 164
    Height = 17
    Hint = 'Не спрашивать подтверждения каждый раз'
    Anchors = [akLeft, akTop, akRight]
    Caption = 'ChBAlwaysYes'
    Checked = True
    State = cbChecked
    TabOrder = 0
  end
  object ChBOnlyBad: TCheckBox
    Left = 8
    Top = 8
    Width = 164
    Height = 17
    Hint = 'Выполнять только для изменившихся функций'
    Anchors = [akLeft, akTop, akRight]
    Caption = 'ChBOnlyBad'
    Checked = True
    ParentShowHint = False
    ShowHint = True
    State = cbChecked
    TabOrder = 1
  end
  object Button1: TButton
    Left = 8
    Top = 56
    Width = 164
    Height = 25
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Button1'
    TabOrder = 2
    OnClick = Button1Click
  end
end
