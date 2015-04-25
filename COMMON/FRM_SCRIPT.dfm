object FrmScript: TFrmScript
  Left = 202
  Top = 103
  Width = 696
  Height = 480
  Caption = 'Редактирование и выполнение скрипта'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 0
    Top = 225
    Width = 688
    Height = 3
    Cursor = crVSplit
    Align = alTop
  end
  object Mem: TMemo
    Left = 0
    Top = 0
    Width = 688
    Height = 225
    Align = alTop
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Courier New'
    Font.Style = []
    Lines.Strings = (
      'Mem')
    ParentFont = False
    ScrollBars = ssBoth
    TabOrder = 0
    WantTabs = True
    WordWrap = False
    OnChange = MemChange
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 415
    Width = 688
    Height = 19
    Panels = <>
    SimplePanel = False
  end
  object MemRes: TMemo
    Left = 0
    Top = 228
    Width = 688
    Height = 187
    Align = alClient
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Courier New'
    Font.Style = []
    Lines.Strings = (
      'Memo1')
    ParentFont = False
    ScrollBars = ssBoth
    TabOrder = 2
  end
  object OScript: TOracleScript
    Session = OSession
    Debug = False
    Cursor = crDefault
    AutoCommit = False
    ExitOnError = False
    OutputOptions = [ooSQL, ooNonSQL, ooData, ooFeedback, ooError]
    OnOutput = OScriptOutput
    Left = 96
    Top = 32
  end
  object OSession: TOracleSession
    Cursor = crHourGlass
    DesignConnection = False
    LogonDatabase = 'S'
    ConnectAs = caNormal
    ThreadSafe = False
    Preferences.FloatPrecision = 0
    Preferences.IntegerPrecision = 0
    Preferences.SmallIntPrecision = -1
    Preferences.UseOCI7 = False
    Preferences.ConvertCRLF = True
    Preferences.TrimStringFields = True
    Preferences.MaxStringFieldSize = 0
    Preferences.ZeroDateIsNull = True
    Preferences.NullLOBIsEmpty = False
    Pooling = spNone
    MTSOptions = [moImplicit, moUniqueServer]
    Connected = False
    RollbackOnDisconnect = False
    NullValue = nvUnAssigned
    SQLTrace = stUnchanged
    OptimizerGoal = ogUnchanged
    IsolationLevel = ilUnchanged
    BytesPerCharacter = bc1Byte
    Left = 64
    Top = 32
  end
  object ActionList1: TActionList
    Left = 280
    Top = 32
    object ActExec: TAction
      Caption = 'Выполнить'
      Hint = 'Выполнить SQL скрипт'
      ShortCut = 16453
      OnExecute = ActExecExecute
    end
    object ActLogon: TAction
      Caption = 'Приконнектится'
      Hint = 'Приконнектится к серверу'
      OnExecute = ActLogonExecute
    end
    object ActWhoAmI: TAction
      Caption = 'Имя пользователя'
      Hint = 'Имя пользователя который приконнектился к программе'
      ShortCut = 16456
    end
    object ActClose: TAction
      Caption = 'Выход'
      Hint = 'Выход из программы'
    end
    object ActOpen: TAction
      Caption = 'Открыть'
      Hint = 'Открыть файл'
      ShortCut = 16463
      OnExecute = ActOpenExecute
    end
    object ActSave: TAction
      Caption = 'Сохранить'
      Hint = 'Сохранить файл'
      ShortCut = 16467
      OnExecute = ActSaveExecute
    end
    object ActChCoding: TAction
      Caption = 'Изменить кодировку'
      Hint = 'Изменить кодировку'
      ShortCut = 16466
      OnExecute = ActChCodingExecute
    end
    object Action1: TAction
      Caption = 'Action1'
    end
  end
  object MainMenu1: TMainMenu
    Left = 248
    Top = 32
    object N1: TMenuItem
      Caption = 'Файл'
      object N4: TMenuItem
        Action = ActOpen
      end
      object N5: TMenuItem
        Action = ActSave
      end
      object N7: TMenuItem
        Caption = '-'
      end
      object N6: TMenuItem
        Action = ActClose
      end
    end
    object N2: TMenuItem
      Caption = 'Редактирование'
    end
    object N3: TMenuItem
      Caption = 'Установки'
      object N8: TMenuItem
        Action = ActChCoding
      end
      object N9: TMenuItem
        Caption = '-'
      end
      object N10: TMenuItem
        Action = ActWhoAmI
      end
      object N11: TMenuItem
        Action = ActLogon
      end
    end
  end
  object OLogon: TOracleLogon
    Session = OSession
    Retries = 2
    Options = [ldAuto, ldDatabase]
    AliasDropDownCount = 8
    HistorySize = 6
    HistoryWithPassword = False
    Caption = 'Test base'
    Left = 40
    Top = 32
  end
  object PMenu: TPopupMenu
    Left = 216
    Top = 32
  end
  object OD: TOpenDialog
    Filter = 
      'SQL files (*.sql)|*.sql|Text files (*.txt)|*.txt|All Files (*.*)' +
      '|*.*'
    Options = [ofEnableSizing]
    Title = 'Открыть скрипт'
    Left = 56
    Top = 120
  end
  object SD: TSaveDialog
    DefaultExt = 'sql'
    Title = 'Сохранить в файл'
    Left = 88
    Top = 120
  end
end
