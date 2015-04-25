object FrmBuildAnFunc: TFrmBuildAnFunc
  Left = 254
  Top = 181
  Width = 546
  Height = 368
  Caption = 'FrmBuildAnFunc'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsMDIChild
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poDefault
  Visible = True
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 0
    Top = 230
    Width = 538
    Height = 3
    Cursor = crVSplit
    Align = alBottom
  end
  object MemSQL: TMemo
    Left = 0
    Top = 0
    Width = 538
    Height = 230
    Align = alClient
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Courier New'
    Font.Style = []
    Lines.Strings = (
      'MemSQL')
    ParentFont = False
    ScrollBars = ssBoth
    TabOrder = 0
  end
  object MemResult: TMemo
    Left = 0
    Top = 233
    Width = 538
    Height = 89
    Align = alBottom
    Lines.Strings = (
      '')
    TabOrder = 1
  end
  object OQ1: TOracleQuery
    SQL.Strings = (
      'select * from OBJS where OTYPE=5')
    Session = MainFrm.OSess
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Cursor = crDefault
    StringFieldsOnly = False
    Threaded = False
    ThreadSynchronized = True
    Left = 64
    Top = 16
  end
  object OQ2: TOracleQuery
    SQL.Strings = (
      
        'select * from DBSTR where DBID = :ANID and REALNAME not in ('#39'AAR' +
        'TICLE'#39', '#39'AVER'#39') '
      'order by vtype , ord')
    Session = MainFrm.OSess
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Variables.Data = {
      0300000001000000050000003A414E49440300000004000000C0000000000000
      00}
    Cursor = crDefault
    StringFieldsOnly = False
    Threaded = False
    ThreadSynchronized = True
    Left = 104
    Top = 16
  end
  object OQ3: TOracleQuery
    Session = MainFrm.OSess
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Cursor = crDefault
    StringFieldsOnly = False
    Threaded = False
    ThreadSynchronized = True
    Left = 64
    Top = 48
  end
  object MainMenu1: TMainMenu
    Left = 288
    Top = 80
    object N1: TMenuItem
      Caption = 'Файл'
      object N2: TMenuItem
        Caption = 'Сохранить'
        ShortCut = 16467
        OnClick = N2Click
      end
      object N3: TMenuItem
        Caption = 'Загрузить'
      end
    end
    object N4: TMenuItem
      Caption = 'Установки генерации'
    end
  end
  object OQ4: TOracleQuery
    Session = MainFrm.OSess
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Cursor = crDefault
    StringFieldsOnly = False
    Threaded = False
    ThreadSynchronized = True
    Left = 104
    Top = 48
  end
  object SD: TSaveDialog
    Left = 296
    Top = 16
  end
  object OD: TOpenDialog
    Left = 328
    Top = 16
  end
end
