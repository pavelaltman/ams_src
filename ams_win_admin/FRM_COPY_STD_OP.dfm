object FrmCopyStdOp: TFrmCopyStdOp
  Left = 387
  Top = 304
  Width = 236
  Height = 209
  Caption = '����������� ������'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object LabQuestion: TLabel
    Left = 0
    Top = 0
    Width = 228
    Height = 57
    Align = alTop
    Alignment = taCenter
    AutoSize = False
    Caption = 'LabQuestion'
    Layout = tlCenter
    WordWrap = True
  end
  object Label1: TLabel
    Left = 8
    Top = 72
    Width = 189
    Height = 13
    Caption = '��� ����������� ������ ����������:'
  end
  object Label2: TLabel
    Left = 8
    Top = 120
    Width = 73
    Height = 13
    Caption = '��� ��������:'
  end
  object CBSchema: TComboBox
    Left = 8
    Top = 88
    Width = 193
    Height = 21
    ItemHeight = 13
    TabOrder = 0
    Text = 'CBSchema'
  end
  object EdOpId: TEdit
    Left = 88
    Top = 120
    Width = 113
    Height = 21
    TabOrder = 1
  end
  object BBCancel: TBitBtn
    Left = 70
    Top = 152
    Width = 97
    Height = 24
    Cancel = True
    Caption = '��������'
    TabOrder = 2
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
  object BBOk: TBitBtn
    Left = 0
    Top = 152
    Width = 65
    Height = 24
    Caption = 'Ok'
    Default = True
    TabOrder = 3
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
  object OQuery: TOracleQuery
    Session = MainFrm.OSess
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Cursor = crDefault
    StringFieldsOnly = False
    Threaded = False
    ThreadSynchronized = True
    Left = 184
    Top = 40
  end
end