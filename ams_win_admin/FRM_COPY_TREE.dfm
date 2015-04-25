object FrmCopyTree: TFrmCopyTree
  Left = 328
  Top = 217
  Width = 438
  Height = 278
  Caption = 'Подготовка к копированию'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object Progress: TMemo
    Left = 214
    Top = 8
    Width = 209
    Height = 209
    Anchors = [akTop, akRight, akBottom]
    TabOrder = 0
  end
  object BBApply: TBitBtn
    Left = 200
    Top = 222
    Width = 100
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'Применить'
    Default = True
    Enabled = False
    TabOrder = 1
    Visible = False
    OnClick = BBApplyClick
    Glyph.Data = {
      F6000000424DF600000000000000760000002800000010000000100000000100
      0400000000008000000000000000000000001000000000000000000000000000
      8000008000000080800080000000800080008080000080808000C0C0C0000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00FFF07FFFFFFF
      FFFFFF007777777777FFF0000000000007FFF0000000000007FFFF00FFF00000
      077FFFF0FFFFFF00007FFFFFFFFFFF00007FFFFFFFFFFF00007FFFFFFFFFFF00
      007FFFFFFFFFFF00007FFFFFFFFFFF000077FFFFFFFFF0000007FFFFFFFFF000
      0007FFFFFFFFF000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
  end
  object PC: TPageControl
    Left = 8
    Top = 8
    Width = 199
    Height = 209
    ActivePage = TS3
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabOrder = 2
    OnChange = PCChange
    object TS1: TTabSheet
      Caption = 'Из'
      TabVisible = False
      object Label1: TLabel
        Left = 8
        Top = 0
        Width = 164
        Height = 13
        Caption = 'Из какой категории копировать'
      end
      object OCFrom: TAmsObjCatView
        Left = 0
        Top = 16
        Width = 191
        Height = 187
        Anchors = [akLeft, akTop, akRight, akBottom]
        HideSelection = False
        Indent = 19
        TabOrder = 0
        AmsOraQuery = OQ
        AmsSecureShow = False
      end
    end
    object TS2: TTabSheet
      Caption = 'Бухгалтерия'
      ImageIndex = 1
      TabVisible = False
      object Label2: TLabel
        Left = 8
        Top = 48
        Width = 170
        Height = 13
        Caption = 'В какую бухгалтерию копировать'
      end
      object EdWhereCopy: TEdit
        Left = 8
        Top = 64
        Width = 305
        Height = 21
        Anchors = [akLeft, akTop, akRight]
        TabOrder = 0
      end
    end
    object TS3: TTabSheet
      Caption = 'Куда'
      ImageIndex = 2
      TabVisible = False
      object Label3: TLabel
        Left = 0
        Top = 0
        Width = 160
        Height = 13
        Caption = 'В какую категорию копировать'
      end
      object OCTo: TAmsObjCatView
        Left = 0
        Top = 12
        Width = 191
        Height = 187
        Anchors = [akLeft, akTop, akRight, akBottom]
        HideSelection = False
        Indent = 19
        TabOrder = 0
        AmsOraQuery = OQ
        AmsSecureShow = False
      end
    end
    object TS4: TTabSheet
      Caption = 'TS4'
      ImageIndex = 3
      TabVisible = False
      object Label4: TLabel
        Left = 8
        Top = 8
        Width = 164
        Height = 13
        Caption = 'Из какой категории копировать'
      end
      object LabFromCat: TLabel
        Left = 8
        Top = 24
        Width = 57
        Height = 13
        Caption = 'LabFromCat'
        Color = clHighlightText
        ParentColor = False
      end
      object LabBuchName: TLabel
        Left = 8
        Top = 60
        Width = 71
        Height = 13
        Caption = 'LabBuchName'
        Color = clHighlightText
        ParentColor = False
      end
      object LabToCat: TLabel
        Left = 8
        Top = 96
        Width = 47
        Height = 13
        Caption = 'LabToCat'
        Color = clHighlightText
        ParentColor = False
      end
      object Label9: TLabel
        Left = 8
        Top = 80
        Width = 64
        Height = 13
        Caption = 'В категорию'
      end
      object Label10: TLabel
        Left = 8
        Top = 44
        Width = 74
        Height = 13
        Caption = 'В бухгалтерию'
      end
    end
  end
  object ButPrev: TButton
    Left = 8
    Top = 222
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Cancel = True
    Caption = '<<'
    TabOrder = 3
    OnClick = ButPrevClick
  end
  object ButNext: TButton
    Left = 88
    Top = 222
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = '>>'
    Default = True
    TabOrder = 4
    OnClick = ButNextClick
  end
  object ButClose: TButton
    Left = 353
    Top = 222
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'Закрыть'
    ModalResult = 1
    TabOrder = 5
  end
  object OQ: TOracleQuery
    Session = MainFrm.OSess
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Cursor = crDefault
    StringFieldsOnly = False
    Threaded = False
    ThreadSynchronized = True
    Left = 144
    Top = 24
  end
  object OSess: TOracleSession
    Cursor = crHourGlass
    DesignConnection = False
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
    Left = 204
    Top = 230
  end
  object OterBaseQ: TOracleQuery
    Session = OSess
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Cursor = crDefault
    StringFieldsOnly = False
    Threaded = False
    ThreadSynchronized = True
    Left = 236
    Top = 230
  end
  object OLogon: TOracleLogon
    Session = OSess
    Retries = 2
    Options = []
    AliasDropDownCount = 8
    HistorySize = 6
    HistoryWithPassword = False
    Left = 204
    Top = 198
  end
end
