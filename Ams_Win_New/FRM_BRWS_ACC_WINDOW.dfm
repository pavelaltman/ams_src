inherited FrmBrwsAccWindow: TFrmBrwsAccWindow
  Left = 91
  Top = 210
  Caption = ''
  ClientHeight = 469
  ClientWidth = 774
  OldCreateOrder = True
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  inherited Label1: TLabel
    Top = 348
    Width = 774
    Constraints.MaxHeight = 13
  end
  inherited Button1: TButton
    TabOrder = 6
    TabStop = False
  end
  inherited Grid2: TRxDBGrid
    Top = 365
    Width = 774
    Height = 85
    Columns = <
      item
        Expanded = False
        FieldName = 'ASIDE'
        PickList.Strings = ()
        Title.Caption = ' '
        Width = 16
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'ANAME'
        PickList.Strings = ()
        Title.Caption = 'Счет'
        Width = 67
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'VALUE'
        PickList.Strings = ()
        Title.Caption = 'Сумма'
        Width = 84
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'ANALINFO'
        PickList.Strings = ()
        Title.Caption = 'Аналитика'
        Width = 367
        Visible = True
      end>
  end
  inherited ToolBar1: TToolBar
    Width = 774
    Height = 25
  end
  inherited RxSplitter1: TRxSplitter
    Top = 361
    Width = 774
    Height = 4
    ControlFirst = Grid2
    Align = alBottom
    OnPosChanged = RxSplitter1PosChanged
  end
  object AFEnd: TAmsFooter [5]
    Left = 0
    Top = 309
    Width = 774
    Height = 39
    Align = alBottom
    Color = clBtnFace
    DefaultRowHeight = 18
    Enabled = False
    FixedCols = 0
    RowCount = 4
    FixedRows = 0
    ScrollBars = ssNone
    TabOrder = 4
  end
  object AFBeg: TAmsFooter [6]
    Left = 0
    Top = 25
    Width = 774
    Height = 20
    Align = alTop
    Color = clBtnFace
    DefaultRowHeight = 18
    Enabled = False
    FixedCols = 0
    RowCount = 1
    FixedRows = 0
    ScrollBars = ssNone
    TabOrder = 5
  end
  object StatusBar: TStatusBar [7]
    Left = 0
    Top = 450
    Width = 774
    Height = 19
    Panels = <
      item
        Width = 200
      end>
    SimplePanel = False
  end
  inherited Grid1: TAmsDBGrid
    Top = 45
    Width = 774
    Height = 264
    OnAmsFooterSyncho = Grid1AmsFooterSyncho
    Columns = <
      item
        Expanded = False
        FieldName = 'OP_ID'
        PickList.Strings = ()
        Visible = False
      end
      item
        Expanded = False
        FieldName = 'OP_SUBID'
        PickList.Strings = ()
        Visible = False
      end
      item
        Expanded = False
        FieldName = 'STORNO'
        PickList.Strings = ()
        Visible = False
      end
      item
        Expanded = False
        FieldName = 'TEXT'
        PickList.Strings = ()
        Title.Caption = 'Текст'
        Width = 228
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'DOC_TYPE'
        PickList.Strings = ()
        Title.Caption = 'Документ'
        Width = 66
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'DOC_NUM'
        PickList.Strings = ()
        Title.Caption = 'Номер'
        Width = 82
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'OP_DATE'
        PickList.Strings = ()
        Title.Caption = 'Дата'
        Width = 62
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'SUMMA'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        PickList.Strings = ()
        Width = 88
        Visible = True
      end>
  end
  inherited DSet: TOracleDataSet
    DetailFields = ''
    OnCalcFields = DSetCalcFields
    Left = 152
    object DSetTEXT: TStringField
      FieldName = 'TEXT'
      Size = 250
    end
    object DSetDOC_NUM_V: TFloatField
      FieldName = 'DOC_NUM_V'
    end
    object DSetSTORNO: TIntegerField
      FieldName = 'STORNO'
    end
    object DSetOP_ID: TFloatField
      FieldName = 'OP_ID'
      Required = True
    end
    object DSetOP_SUBID: TFloatField
      FieldName = 'OP_SUBID'
      Required = True
    end
  end
  inherited DSetItems: TOracleDataSet
    Master = nil
    MasterFields = ''
    Left = 32
    Top = 136
  end
  inherited DSrcItems: TDataSource
    Left = 56
    Top = 136
  end
  inherited ActionList1: TActionList
    Left = 272
    Top = 88
    inherited ActPrint: TAction
      Caption = '&Печать'
    end
    object ActViewOpt: TAction [14]
      Category = 'Accs Window'
      Caption = 'Режимы [Alt+Р]'
      Hint = 'Режимы отображения '
      ImageIndex = 9
      OnExecute = ActViewOptExecute
    end
    object ActFiltr: TAction [15]
      Category = 'Accs Window'
      Caption = 'Фильтр [Alt+Ф]'
      Hint = 'Фильтр по позиции базы данных'
      OnExecute = ActFiltrExecute
    end
    object ActFieldName: TAction [16]
      Category = 'Accs Window'
      Caption = 'Выбрать поле [Alt+В]'
      Hint = 'Выбрать поле аналитики'
      OnExecute = ActFieldNameExecute
    end
    object ActAnaliticShow: TAction [17]
      Category = 'Accs Window'
      Caption = 'Просмотр аналитики [Alt+T]'
      Hint = 'Просмотр аналитики по операции'
      OnExecute = ActAnaliticShowExecute
    end
    object ActAnalChoose: TAction [18]
      Category = 'Accs Window'
      Caption = 'Аналитика... [Alt+А]'
      Hint = 
        'Какую аналитику показывать'#13#10'(на дату, обороты, текущей операции)' +
        ' '
      OnExecute = ActAnalChooseExecute
    end
    object ActAnalEndRem: TAction [19]
      Category = 'Accs Window'
      Caption = 'Остаток на конец [Alt+О]'
      Hint = 'Аналитика остатка на конец'
      OnExecute = ActAnalEndRemExecute
    end
    object ActRunForm: TAction [20]
      Category = 'Accs Window'
      Caption = 'Форма...[Alt+M]'
      OnExecute = ActRunFormExecute
    end
    object ActShowDeffered: TAction [21]
      Category = 'Accs Window'
      Caption = 'ActShowDeffered'
      OnExecute = ActShowDefferedExecute
    end
    object ActRecalc: TAction [22]
      Category = 'Accs Window'
      Caption = 'Пересчитать'
      Hint = 'Пересчитать обороты и остатки по счету'
      ShortCut = 16497
      OnExecute = ActRecalcExecute
    end
    object ActNotPlainPeriod: TAction
      Category = 'New'
      Caption = 'Установить период'
      Hint = 'Установить период видимости на произвольный диапазон дат'
      ShortCut = 16498
      OnExecute = ActNotPlainPeriodExecute
    end
    object ActRecalcBegin: TAction
      Category = 'Accs Window'
      Caption = 'Пересчитать на начало'
      OnExecute = ActRecalcBeginExecute
    end
  end
  inherited PopupMenu1: TPopupMenu
    Left = 248
    Top = 88
    object N16: TMenuItem [13]
      Action = ActNotPlainPeriod
    end
    object N10: TMenuItem [15]
      Action = ActViewOpt
    end
    object N11: TMenuItem [16]
      Action = ActFieldName
    end
    object N12: TMenuItem [17]
      Action = ActFiltr
    end
    object N13: TMenuItem [18]
      Action = ActAnaliticShow
    end
    object AltM1: TMenuItem [19]
      Action = ActRunForm
    end
    object Alt5: TMenuItem [20]
      Action = ActAnalChoose
    end
    object N14: TMenuItem [21]
      Caption = '-'
    end
    object N15: TMenuItem [22]
      Action = ActRecalc
    end
    object N17: TMenuItem
      Action = ActRecalcBegin
      ShortCut = 16503
    end
  end
  object TDelay: TTimer
    OnTimer = TDelayTimer
    Left = 184
    Top = 32
  end
end
