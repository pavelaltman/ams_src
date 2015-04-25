object FrmInputHistory: TFrmInputHistory
  Left = 543
  Top = 258
  AutoSize = True
  BorderIcons = []
  BorderStyle = bsNone
  ClientHeight = 21
  ClientWidth = 273
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object CBInpHist: TComboBox
    Left = 0
    Top = 0
    Width = 273
    Height = 21
    Color = clAqua
    DropDownCount = 20
    ItemHeight = 13
    TabOrder = 0
    Text = 'CBInpHist'
    OnChange = CBInpHistChange
    OnKeyPress = CBInpHistKeyPress
  end
end
