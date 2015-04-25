object FrmCustomPeriod: TFrmCustomPeriod
  Left = 244
  Top = 139
  Width = 401
  Height = 185
  BorderIcons = []
  Caption = 'Установки'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 393
    Height = 41
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    object DirectoryEdit1: TDirectoryEdit
      Left = 180
      Top = 8
      Width = 200
      Height = 21
      NumGlyphs = 1
      TabOrder = 0
    end
    object StaticText1: TStaticText
      Left = 19
      Top = 13
      Width = 119
      Height = 17
      Caption = 'Папка для сохранения'
      TabOrder = 1
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 82
    Width = 393
    Height = 41
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 1
    object DirectoryEdit3: TDirectoryEdit
      Left = 180
      Top = 8
      Width = 200
      Height = 21
      NumGlyphs = 1
      TabOrder = 0
    end
    object StaticText3: TStaticText
      Left = 19
      Top = 13
      Width = 118
      Height = 17
      Caption = 'Папка с SQL файлами'
      TabOrder = 1
    end
  end
  object Panel3: TPanel
    Left = 0
    Top = 41
    Width = 393
    Height = 41
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 2
    object DirectoryEdit2: TDirectoryEdit
      Left = 180
      Top = 8
      Width = 200
      Height = 21
      NumGlyphs = 1
      TabOrder = 0
    end
    object StaticText2: TStaticText
      Left = 19
      Top = 13
      Width = 143
      Height = 17
      Caption = 'Папка для восстановления'
      TabOrder = 1
    end
  end
  object Panel4: TPanel
    Left = 0
    Top = 123
    Width = 393
    Height = 35
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 3
    object ButClose: TButton
      Left = 136
      Top = 2
      Width = 83
      Height = 25
      Anchors = [akRight, akBottom]
      Cancel = True
      Caption = 'Закрыть'
      TabOrder = 0
      OnClick = ButCloseClick
    end
  end
  object FormStorage1: TFormStorage
    IniFileName = 'amswin.ini'
    StoredProps.Strings = (
      'DirectoryEdit1.Text'
      'DirectoryEdit2.Text'
      'DirectoryEdit3.Text')
    StoredValues = <>
    Left = 56
    Top = 123
  end
end
