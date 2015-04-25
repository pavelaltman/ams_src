inherited FrmBrowsAn: TFrmBrowsAn
  Left = 148
  Top = 159
  Width = 781
  Height = 458
  Caption = 'FrmBrowsAn'
  OldCreateOrder = True
  PixelsPerInch = 96
  TextHeight = 13
  inherited Splitter1: TSplitter
    Top = 53
    Height = 334
  end
  inherited Lab1: TLabel
    Top = 38
    Width = 773
  end
  object LabTotal: TLabel [2]
    Left = 0
    Top = 25
    Width = 773
    Height = 13
    Align = alTop
    Alignment = taRightJustify
    Caption = 'LabTotal'
  end
  inherited ButEnter: TButton
    Top = 72
    TabOrder = 5
  end
  inherited CV: TAmsDbCatView
    Top = 53
    Height = 334
  end
  inherited SBar: TStatusBar
    Top = 412
    Width = 773
    Panels = <
      item
        Width = 100
      end
      item
        Width = 100
      end
      item
        Width = 100
      end
      item
        Width = 50
      end
      item
        Width = 100
      end
      item
        Width = 50
      end>
  end
  inherited ToolBar3: TToolBar
    Width = 773
    object ToolButton5: TToolButton
      Left = 456
      Top = 2
      Width = 8
      Caption = 'ToolButton5'
      ImageIndex = 120
      Style = tbsSeparator
    end
    object EdShtrixCode: TEdit
      Left = 464
      Top = 2
      Width = 153
      Height = 22
      TabOrder = 0
    end
  end
  object Panel21: TPanel [7]
    Left = 0
    Top = 387
    Width = 773
    Height = 25
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 4
    object BBOk: TBitBtn
      Left = 4
      Top = 2
      Width = 75
      Height = 23
      Caption = 'Ok'
      TabOrder = 0
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
      Left = 673
      Top = 2
      Width = 97
      Height = 23
      Anchors = [akTop, akRight]
      Cancel = True
      Caption = 'Отменить'
      TabOrder = 1
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
  inherited Panel1: TPanel
    Top = 53
    Width = 593
    Height = 334
    inherited DBG: TAmsDBGrid
      Width = 593
      Height = 277
      OnAmsFooterSyncho = DBGAmsFooterSyncho
    end
    object Footer: TAmsFooter
      Left = 0
      Top = 277
      Width = 593
      Height = 57
      Align = alBottom
      Color = clBtnFace
      DefaultRowHeight = 18
      Enabled = False
      FixedCols = 0
      RowCount = 3
      FixedRows = 0
      ScrollBars = ssNone
      TabOrder = 1
    end
  end
  inherited ODS: TOracleDataSet
    RefreshOptions = [roAfterInsert, roAfterUpdate, roAllFields]
  end
  inherited ActionList2: TActionList
    object ActFooter: TAction [35]
      Category = 'Analictica'
      Caption = 'Всего строка'
      Hint = 'Выбор показываемого во всего строке'
      OnExecute = ActFooterExecute
    end
    object ActDelPos: TAction [36]
      Category = 'Analictica'
      Caption = 'Удалить позицию'
      Hint = 'Удалить позицию'#13#10'в аналитике и базе данных'
      ImageIndex = 84
      ShortCut = 16430
      OnExecute = ActDelPosExecute
    end
    object ActClearAn: TAction [37]
      Category = 'Analictica'
      Caption = 'Очистить'
      Hint = 'Очистиить аналитическую сумму'
      ShortCut = 8238
      OnExecute = ActClearAnExecute
    end
    object ActTakeAnSum: TAction [38]
      Category = 'Analictica'
      Caption = 'Взять (Alt+В)'
      Hint = 'Взять аналитическую сумму'
      OnExecute = ActTakeAnSumExecute
    end
    object ActDestr: TAction [39]
      Category = 'Analictica'
      Caption = 'Распределять'
      Hint = 'Распределить по остатку'
      ShortCut = 8309
      OnExecute = ActDestrExecute
    end
    object ActCalcWithClear: TAction [40]
      Category = 'Analictica'
      Caption = 'Пересчитать ред'
      Hint = 'Пересчитать с обнулением полей'
      ShortCut = 16498
      OnExecute = ActCalcWithClearExecute
    end
    object ActCalcVisible: TAction [41]
      Category = 'Analictica'
      Caption = 'Пересчитать видимые'
      Hint = 'Пересчитать только видимые позиции'
      ShortCut = 8305
      OnExecute = ActCalcVisibleExecute
    end
    object ActClaclVisibleWithClear: TAction [42]
      Category = 'Analictica'
      Caption = 'Пересчитать видимые с очисткой'
      Hint = 'Пересчитать только видимые поля с очисткой '
      ShortCut = 8306
      OnExecute = ActClaclVisibleWithClearExecute
    end
    object ActOk_: TAction [44]
      Category = 'NEW'
      Caption = 'ActOk_'
      ShortCut = 113
      OnExecute = ActOk_Execute
    end
    object ActDocGen: TAction [45]
      Category = 'Analictica'
      Caption = 'Генерировать документ (Alt+О)'
      Hint = 'Генерировать документ'
      OnExecute = ActDocGenExecute
    end
    object ActFormGen: TAction [46]
      Category = 'Analictica'
      Caption = 'Генерировать фор&му (Alt+M)'
      Hint = 'Генерировать форму'
      OnExecute = ActFormGenExecute
    end
    object ActOperVars: TAction [47]
      Category = 'Analictica'
      Caption = 'Переменные операции (Alt+П)'
      Hint = 'Переменные операции (из связанной БД)'
      OnExecute = ActOperVarsExecute
    end
    object ActFilter: TAction [48]
      Category = 'Analictica'
      Caption = 'Фильтр [Alt+Ф]'
      Hint = 'Фильтр по позиции БД'
      OnExecute = ActFilterExecute
    end
    object ActShtrixModeOnOff: TAction
      Category = 'NEW'
      Caption = 'Штрих-код режим включить'
      Hint = 'Штрих-код режим включить (выключить)'
      ShortCut = 122
      OnExecute = ActShtrixModeOnOffExecute
    end
  end
  inherited PopupMenu3: TPopupMenu
    object N22: TMenuItem [1]
      Action = ActVisibilityMode
      Caption = 'Режим видимости [Alt+Р]'
    end
    object N13: TMenuItem [4]
      Action = ActDelPos
    end
    inherited MenuItem12: TMenuItem [14]
    end
    inherited MenuItem13: TMenuItem [15]
    end
    inherited ShiftF71: TMenuItem [16]
    end
    inherited N8: TMenuItem [17]
    end
    object N11: TMenuItem [18]
      Caption = '-'
    end
    inherited MenuItem14: TMenuItem [19]
      object N23: TMenuItem
        Caption = '-'
      end
      object N24: TMenuItem
        Action = ActClearAn
      end
    end
    inherited MenuItem17: TMenuItem [20]
    end
    object N10: TMenuItem [21]
      Caption = '-'
    end
    object N12: TMenuItem [22]
      Action = ActFooter
    end
    object Alt3: TMenuItem [23]
      Action = ActFilter
    end
    object N20: TMenuItem [24]
      Action = ActTakeAnSum
    end
    object N14: TMenuItem [25]
      Caption = 'Пересчитать'
      object N19: TMenuItem
        Action = ActCalc
      end
      object N15: TMenuItem
        Action = ActCalcWithClear
      end
      object N16: TMenuItem
        Caption = '-'
      end
      object N17: TMenuItem
        Action = ActCalcVisible
      end
      object N18: TMenuItem
        Action = ActClaclVisibleWithClear
      end
      object N25: TMenuItem
        Caption = '-'
      end
      object N26: TMenuItem
        Action = ActDestr
      end
    end
    inherited AltF71: TMenuItem [26]
    end
    inherited MenuItem11: TMenuItem [27]
      Visible = False
    end
    object N21: TMenuItem
      Caption = '-'
    end
    object AltM1: TMenuItem
      Action = ActFormGen
    end
    object AltM2: TMenuItem
      Action = ActDocGen
    end
    object Alt2: TMenuItem
      Action = ActOperVars
    end
  end
  object TimSelected: TTimer
    Enabled = False
    OnTimer = TimSelectedTimer
    Left = 452
    Top = 32
  end
end
