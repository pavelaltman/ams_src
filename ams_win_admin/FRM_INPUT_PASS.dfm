object FrmInputPass: TFrmInputPass
  Left = 337
  Top = 236
  Width = 271
  Height = 81
  BorderIcons = []
  Caption = 'FrmInputPass'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object EdPass: TEdit
    Left = 3
    Top = 0
    Width = 257
    Height = 21
    PasswordChar = '*'
    TabOrder = 0
  end
  object Button1: TButton
    Left = 94
    Top = 25
    Width = 75
    Height = 25
    Caption = 'Ok'
    Default = True
    TabOrder = 1
    OnClick = Button1Click
  end
end
