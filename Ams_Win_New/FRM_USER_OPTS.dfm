object FrmUserOpts: TFrmUserOpts
  Left = 255
  Top = 193
  BorderIcons = [biMinimize, biMaximize]
  BorderStyle = bsDialog
  Caption = '���������������� ���������'
  ClientHeight = 188
  ClientWidth = 331
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Shape1: TShape
    Left = 2
    Top = 4
    Width = 325
    Height = 49
    Brush.Color = clScrollBar
    Shape = stRoundRect
  end
  object Label1: TLabel
    Left = 10
    Top = 8
    Width = 108
    Height = 13
    Caption = '����������� �����'
  end
  object Label2: TLabel
    Left = 9
    Top = 28
    Width = 71
    Height = 13
    Caption = '����� �����'
  end
  object Label3: TLabel
    Left = 135
    Top = 28
    Width = 43
    Height = 13
    Caption = '�������'
  end
  object Shape2: TShape
    Left = 2
    Top = 56
    Width = 325
    Height = 49
    Brush.Color = clScrollBar
    Shape = stRoundRect
  end
  object Label4: TLabel
    Left = 8
    Top = 64
    Width = 76
    Height = 13
    Caption = '���� ��������'
  end
  object Label5: TLabel
    Left = 12
    Top = 84
    Width = 59
    Height = 13
    Caption = '���� �����'
  end
  object SESum: TRxSpinEdit
    Left = 85
    Top = 24
    Width = 43
    Height = 21
    MaxValue = 15
    TabOrder = 0
  end
  object SEKop: TRxSpinEdit
    Left = 183
    Top = 24
    Width = 43
    Height = 21
    MaxValue = 15
    TabOrder = 1
  end
  object ChBApps: TCheckBox
    Left = 233
    Top = 26
    Width = 88
    Height = 17
    Caption = '���������'
    TabOrder = 2
  end
  object DatInp: TDateEdit
    Left = 76
    Top = 80
    Width = 73
    Height = 21
    NumGlyphs = 2
    TabOrder = 3
  end
  object ChBCurrDate: TCheckBox
    Left = 164
    Top = 82
    Width = 97
    Height = 17
    Caption = '������� ����'
    TabOrder = 4
  end
  object ChBSortByDocNum: TCheckBox
    Left = 8
    Top = 112
    Width = 319
    Height = 17
    Anchors = [akLeft, akTop, akRight]
    Caption = '����������� �� ������ ���������'
    TabOrder = 5
  end
  object ChBGotoEnd: TCheckBox
    Left = 8
    Top = 136
    Width = 319
    Height = 17
    Anchors = [akLeft, akTop, akRight]
    Caption = '����������� � ����� ������'
    TabOrder = 6
  end
  object BBOk: TBitBtn
    Left = 2
    Top = 163
    Width = 75
    Height = 23
    Caption = 'Ok'
    Default = True
    TabOrder = 7
    OnClick = BBOkClick
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
  object BBCancel: TBitBtn
    Left = 232
    Top = 163
    Width = 97
    Height = 23
    Anchors = [akTop, akRight]
    Cancel = True
    Caption = '��������'
    TabOrder = 8
    OnClick = BBCancelClick
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
end
