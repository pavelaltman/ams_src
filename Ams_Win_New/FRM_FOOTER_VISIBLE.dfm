object FrmFooterVisible: TFrmFooterVisible
  Left = 311
  Top = 147
  Width = 260
  Height = 337
  AutoSize = True
  BorderIcons = []
  Caption = '��� ���������� �����-������'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object FVAll: TRadioGroup
    Left = 0
    Top = 0
    Width = 252
    Height = 56
    Align = alTop
    Caption = '������ �����'
    ItemIndex = 0
    Items.Strings = (
      '���������� ������'
      '���������� ������'
      '������� �� ����������')
    TabOrder = 0
  end
  object FVFiltered: TRadioGroup
    Left = 0
    Top = 56
    Width = 252
    Height = 56
    Align = alTop
    Caption = '��������������� �������'
    ItemIndex = 0
    Items.Strings = (
      '���������� ������'
      '���������� ������'
      '������� �� ����������')
    TabOrder = 1
  end
  object FVCurrCat: TRadioGroup
    Left = 0
    Top = 112
    Width = 252
    Height = 56
    Align = alTop
    Caption = '������� ���������'
    ItemIndex = 0
    Items.Strings = (
      '���������� ������'
      '���������� ������'
      '������� �� ����������')
    TabOrder = 2
  end
  object FVSubTree: TRadioGroup
    Left = 0
    Top = 168
    Width = 252
    Height = 56
    Align = alTop
    Caption = '�� ���������, ������� � ������� ���������'
    ItemIndex = 0
    Items.Strings = (
      '���������� ������'
      '���������� ������'
      '������� �� ����������')
    TabOrder = 3
  end
  object FVSelected: TRadioGroup
    Left = 0
    Top = 224
    Width = 252
    Height = 56
    Align = alTop
    Caption = '���������� ������'
    ItemIndex = 0
    Items.Strings = (
      '���������� ������'
      '���������� ������'
      '������� �� ����������')
    TabOrder = 4
  end
  object BitBtn1: TBitBtn
    Left = 166
    Top = 285
    Width = 84
    Height = 25
    Anchors = [akTop, akRight]
    Caption = '��������'
    TabOrder = 5
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
  object BitBtn2: TBitBtn
    Left = 3
    Top = 285
    Width = 75
    Height = 25
    Caption = 'Ok'
    ModalResult = 1
    TabOrder = 6
    OnClick = BitBtn2Click
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
end
