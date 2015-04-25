object FrmExit: TFrmExit
  Left = 397
  Top = 326
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Ams Бухгалтерия'
  ClientHeight = 95
  ClientWidth = 219
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
    Left = 35
    Top = 16
    Width = 154
    Height = 13
    Alignment = taCenter
    BiDiMode = bdRightToLeft
    Caption = 'Изменить имя пользователя?'
    ParentBiDiMode = False
  end
  object mbYes: TBitBtn
    Left = 24
    Top = 55
    Width = 75
    Height = 25
    Caption = '&Да'
    Default = True
    ModalResult = 6
    TabOrder = 0
    OnClick = mbYesClick
    NumGlyphs = 4
  end
  object mbNo: TBitBtn
    Left = 120
    Top = 55
    Width = 75
    Height = 25
    Cancel = True
    Caption = '&Нет'
    ModalResult = 7
    TabOrder = 1
    NumGlyphs = 2
  end
end
