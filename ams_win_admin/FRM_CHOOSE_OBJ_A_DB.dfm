inherited FrmChooseObjADb: TFrmChooseObjADb
  Height = 356
  Caption = 'FrmChooseObjADb'
  OldCreateOrder = True
  PixelsPerInch = 96
  TextHeight = 13
  inherited Splitter1: TSplitter
    Height = 317
  end
  object Splitter2: TSplitter [1]
    Left = 0
    Top = 317
    Width = 518
    Height = 3
    Cursor = crVSplit
    Align = alBottom
  end
  inherited Panel1: TPanel
    Height = 317
    inherited TV: TTreeView
      Height = 287
    end
  end
  inherited SG: TAmsStringGrid
    Height = 317
  end
  object MRes: TMemo [4]
    Left = 0
    Top = 320
    Width = 518
    Height = 9
    Align = alBottom
    Constraints.MinHeight = 8
    ScrollBars = ssVertical
    TabOrder = 2
  end
  inherited ActionList1: TActionList
    inherited ActDelPos: TAction
      Visible = False
    end
    object ActCopyData: TAction
      Caption = 'Копировать данные'
      Hint = 'Копировать данные в другую бухгалтерию'
      OnExecute = ActCopyDataExecute
    end
    object ActUpdStruct: TAction
      Caption = 'Обновление структуры'
      Hint = 'Обновление структуры в другой бухгалтерии'
      OnExecute = ActUpdStructExecute
    end
  end
  inherited PopupMenu1: TPopupMenu
    object N2: TMenuItem
      Action = ActCopyData
    end
    object N3: TMenuItem
      Action = ActUpdStruct
    end
  end
  object OrSess: TOracleSession
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
    Left = 40
    Top = 200
  end
  object OSc: TOracleScript
    Session = OrSess
    Debug = False
    Cursor = crDefault
    AutoCommit = False
    ExitOnError = False
    OutputOptions = [ooSQL, ooNonSQL, ooData, ooFeedback, ooError]
    AfterCommand = OScAfterCommand
    Left = 64
    Top = 200
  end
  object OrLogon: TOracleLogon
    Session = OrSess
    Retries = 2
    Options = [ldAuto]
    AliasDropDownCount = 8
    HistorySize = 6
    HistoryWithPassword = False
    Left = 88
    Top = 200
  end
end
