object MainFrm: TMainFrm
  Left = 345
  Top = 243
  Width = 357
  Height = 203
  Caption = 'MainFrm'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnKeyDown = FormKeyDown
  PixelsPerInch = 96
  TextHeight = 13
  object Button1: TButton
    Left = 8
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Button1'
    TabOrder = 0
    OnClick = Button1Click
  end
  object SB: TStatusBar
    Left = 0
    Top = 157
    Width = 349
    Height = 19
    Panels = <>
    SimplePanel = False
  end
end
