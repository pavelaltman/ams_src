object FrmWindowsList: TFrmWindowsList
  Left = 150
  Top = 163
  Width = 274
  Height = 220
  BorderIcons = [biSystemMenu, biHelp]
  Caption = 'Список окон'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object LBWin: TListBox
    Left = 0
    Top = 0
    Width = 266
    Height = 193
    Align = alClient
    ItemHeight = 13
    TabOrder = 0
    OnDblClick = LBWinClick
    OnKeyDown = LBWinKeyDown
  end
end
