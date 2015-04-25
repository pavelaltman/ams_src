object FrmShowTiedDb: TFrmShowTiedDb
  Left = 571
  Top = 157
  Width = 347
  Height = 256
  Caption = 'FrmShowTiedDb'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Button1: TButton
    Left = 16
    Top = 40
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Button1'
    TabOrder = 1
    OnClick = Button1Click
  end
  object SG: TAmsStringGrid
    Left = 0
    Top = 0
    Width = 337
    Height = 225
    ColCount = 2
    DefaultRowHeight = 18
    FixedCols = 0
    TabOrder = 0
    ColWidths = (
      126
      190)
  end
end
