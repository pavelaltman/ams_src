object FrmQuickSearch: TFrmQuickSearch
  Left = 358
  Top = 314
  HelpContext = 50
  AutoSize = True
  BorderStyle = bsNone
  Caption = 'FrmQuickSearch'
  ClientHeight = 21
  ClientWidth = 193
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Visible = True
  PixelsPerInch = 96
  TextHeight = 13
  object EdInp: TEdit
    Left = 0
    Top = 0
    Width = 193
    Height = 21
    Color = clAqua
    TabOrder = 0
    Text = 'EdInp'
    OnChange = EdInpChange
    OnKeyPress = EdInpKeyPress
  end
end
