object AmsWinC: TAmsWinC
  Left = 96
  Top = 127
  Width = 892
  Height = 530
  Hint = 'Главное меню'
  HelpContext = 1
  Caption = 'Главное меню'
  Color = clAppWorkSpace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsMDIForm
  Menu = MainMenu1
  OldCreateOrder = False
  PopupMenu = FrmChooseObj.PopupMenu1
  Visible = True
  WindowState = wsMaximized
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object ToolBar1: TToolBar
    Left = 0
    Top = 0
    Width = 884
    Height = 33
    HelpContext = 1
    Caption = 'ToolBar1'
    Color = clActiveBorder
    DisabledImages = DatMod.ImageList1
    Images = DatMod.ImageList1
    ParentColor = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 0
    object ToolButton1: TToolButton
      Left = 0
      Top = 2
      Action = ActInpOper
      ImageIndex = 40
      PopupMenu = FrmAccList.PopupMenu2
    end
    object ToolButton2: TToolButton
      Left = 23
      Top = 2
      Action = ActJurOper
      ImageIndex = 27
    end
    object ToolButton3: TToolButton
      Left = 46
      Top = 2
      Action = ActRasrOper
      ImageIndex = 69
    end
    object ToolButton4: TToolButton
      Left = 69
      Top = 2
      Action = ActOtlOper
      ImageIndex = 46
    end
    object ToolButton5: TToolButton
      Left = 92
      Top = 2
      Action = ActOtForm
      ImageIndex = 26
    end
    object ToolButton6: TToolButton
      Left = 115
      Top = 2
      Action = ActBD
      ImageIndex = 5
    end
    object ToolButton7: TToolButton
      Left = 138
      Top = 2
      Action = ActConst
      ImageIndex = 32
    end
    object ToolButton8: TToolButton
      Left = 161
      Top = 2
      Action = ActChangeName
      ImageIndex = 115
    end
    object ToolButton9: TToolButton
      Left = 184
      Top = 2
      Action = ActExit
      ImageIndex = 117
    end
    object ToolButton10: TToolButton
      Left = 207
      Top = 2
      Action = ActPerVis
      ImageIndex = 80
    end
    object ToolButton11: TToolButton
      Left = 230
      Top = 2
      Action = ActNewM
      ImageIndex = 36
    end
    object ToolButton12: TToolButton
      Left = 253
      Top = 2
      Action = ActDelsOper
      ImageIndex = 14
    end
    object ToolButton13: TToolButton
      Left = 276
      Top = 2
      Action = ActDelsOtlOper
      ImageIndex = 25
    end
    object ToolButton14: TToolButton
      Left = 299
      Top = 2
      Hint = 'Установить SQL отладку'
      Action = ActSetSqlDebug
      ImageIndex = 104
    end
    object ToolButton15: TToolButton
      Left = 322
      Top = 2
      Action = ActListWind
      ImageIndex = 100
    end
    object ToolButton16: TToolButton
      Left = 345
      Top = 2
      Action = ActCascade
      ImageIndex = 8
    end
    object ToolButton17: TToolButton
      Left = 368
      Top = 2
      Action = ActTile
      ImageIndex = 9
    end
    object ToolButton18: TToolButton
      Left = 391
      Top = 2
      Action = ActHelp
    end
    object ToolButton20: TToolButton
      Left = 414
      Top = 2
      Width = 8
      Caption = 'ToolButton20'
      ImageIndex = 93
      Style = tbsSeparator
    end
    object ToolButton19: TToolButton
      Left = 422
      Top = 2
      Action = ActCalc
    end
    object ToolButton21: TToolButton
      Left = 445
      Top = 2
      Caption = 'ToolButton21'
      ImageIndex = 93
      OnClick = ToolButton21Click
    end
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 465
    Width = 884
    Height = 19
    Panels = <
      item
        Text = '1'
        Width = 100
      end
      item
        Text = '2'
        Width = 100
      end
      item
        Text = '3'
        Width = 150
      end
      item
        Width = 50
      end>
    SimplePanel = False
  end
  object lv: TListView
    Left = 0
    Top = 33
    Width = 884
    Height = 432
    Align = alClient
    Color = clAppWorkSpace
    Columns = <>
    ColumnClick = False
    IconOptions.AutoArrange = True
    OwnerDraw = True
    ShowColumnHeaders = False
    TabOrder = 2
    Visible = False
    OnClick = lvClick
    OnMouseMove = lvMouseMove
  end
  object edShtrih: TEdit
    Left = 648
    Top = 192
    Width = 0
    Height = 21
    Color = clAppWorkSpace
    TabOrder = 3
    Text = 'edShtrih'
    OnKeyDown = edShtrihKeyDown
    OnKeyPress = edShtrihKeyPress
  end
  object MainMenu1: TMainMenu
    Left = 8
    Top = 40
    object N1: TMenuItem
      Caption = 'Бухгалтерия'
      object N5: TMenuItem
        Action = ActInpOper
      end
      object N6: TMenuItem
        Action = ActJurOper
      end
      object N7: TMenuItem
        Action = ActRasrOper
      end
      object N8: TMenuItem
        Action = ActOtlOper
      end
      object N9: TMenuItem
        Caption = '-'
      end
      object N10: TMenuItem
        Action = ActBD
      end
      object N11: TMenuItem
        Action = ActOtForm
        Caption = 'Отчетные формы'
      end
      object N12: TMenuItem
        Action = ActConst
      end
      object N13: TMenuItem
        Caption = '-'
      end
      object N15: TMenuItem
        Action = ActChangeName
      end
      object N14: TMenuItem
        Action = ActExit
      end
    end
    object N2: TMenuItem
      Caption = 'Периоды'
      object N20: TMenuItem
        Action = ActPerVis
      end
      object N23: TMenuItem
        Action = ActNewM
      end
      object N33: TMenuItem
        Caption = '-'
      end
      object N34: TMenuItem
        Caption = 'Закрыть период'
      end
      object N35: TMenuItem
        Caption = 'Установки '
        OnClick = N35Click
      end
    end
    object N3: TMenuItem
      Caption = 'Разное'
      object N27: TMenuItem
        Action = ActCalc
      end
      object N21: TMenuItem
        Action = ActDelsOper
      end
      object N22: TMenuItem
        Action = ActDelsOtlOper
      end
      object N29: TMenuItem
        Caption = '-'
      end
      object N28: TMenuItem
        Action = ActImport
      end
      object N30: TMenuItem
        Action = ActImportSettings
        Caption = 'Установки импорта'
      end
    end
    object N4: TMenuItem
      Caption = 'Установки'
      object N25: TMenuItem
        Caption = 'Полное имя'
        OnClick = N25Click
      end
      object N26: TMenuItem
        Caption = 'Смена пароля'
        OnClick = N26Click
      end
      object N24: TMenuItem
        Caption = 'Установки пользователя'
        OnClick = N24Click
      end
      object Debug1: TMenuItem
        Caption = 'Debug'
        object TestAnSum1: TMenuItem
          Caption = 'TestAnSum'
          OnClick = TestAnSum1Click
        end
        object SQL2: TMenuItem
          Action = ActSetSqlDebug
        end
        object MNViewDebug: TMenuItem
          Caption = 'Установить VIEWDEBUG'
          OnClick = MNViewDebugClick
        end
      end
    end
    object N16: TMenuItem
      Caption = 'Окна'
      object N17: TMenuItem
        Action = ActListWind
      end
      object N18: TMenuItem
        Action = ActCascade
      end
      object N19: TMenuItem
        Action = ActTile
      end
    end
    object N31: TMenuItem
      Caption = '?'
      object N32: TMenuItem
        Caption = 'О программе'
        OnClick = N32Click
      end
    end
  end
  object ActionList1: TActionList
    Images = DatMod.ImageList1
    Left = 40
    Top = 40
    object ActHelp: TAction
      Category = 'Справка'
      Caption = 'Справка'
      ImageIndex = 13
      ShortCut = 112
      OnExecute = ActHelpExecute
    end
    object ActSetSqlDebug: TAction
      Category = 'Справка'
      Caption = 'Установить SQL отладку'
      OnExecute = ActSetSqlDebugExecute
    end
    object ActConst: TAction
      Caption = 'Константы'
      Hint = 'Константы'
      OnExecute = ActConstExecute
    end
    object ActExit: TAction
      Caption = 'Выход'
      Hint = 'Выход'
      OnExecute = ActExitExecute
    end
    object ActPerVis: TAction
      Caption = 'Период видимости'
      Hint = 'Период видимости'
      ShortCut = 114
      OnExecute = ActPerVisExecute
    end
    object ActListWind: TAction
      Caption = 'Список окон'
      Hint = 'Список окон'
      OnExecute = ActListWindExecute
    end
    object ActCascade: TAction
      Caption = 'Расположить каскадно'
      Hint = 'Расположить каскадно'
      OnExecute = ActCascadeExecute
    end
    object ActTile: TAction
      Caption = 'Расположить черепицей'
      Hint = 'Расположить черепицей'
      OnExecute = ActTileExecute
    end
    object ActInpOper: TAction
      Caption = 'Ввести операцию'
      Hint = 'Ввести новую операцию'
      ShortCut = 45
      OnExecute = ActInpOperExecute
    end
    object ActJurOper: TAction
      Caption = 'Журнал операций'
      Hint = 'Журнал операций'
      OnExecute = ActJurOperExecute
    end
    object ActRasrOper: TAction
      Caption = 'Разрешенные операции'
      Hint = 'Разрешенные операции'
      OnExecute = ActRasrOperExecute
    end
    object ActOtlOper: TAction
      Caption = 'Отложенные операции'
      Hint = 'Отложенные операции'
      OnExecute = ActOtlOperExecute
    end
    object ActOtForm: TAction
      Caption = 'тчетные формы'
      Hint = 'Отчетные формы'
      OnExecute = ActOtFormExecute
    end
    object ActBD: TAction
      Caption = 'Базы  данных'
      Hint = 'Базы данных'
      OnExecute = ActBDExecute
    end
    object ActChangeName: TAction
      Caption = 'Изменить имя пользователя'
      Hint = 'Изменить имя пользователя'
      OnExecute = ActChangeNameExecute
    end
    object ActNewM: TAction
      Caption = 'Начать новый месяц'
      Hint = 'Начать новый месяц'
      OnExecute = ActNewMExecute
    end
    object ActDelsOtlOper: TAction
      Caption = 'Удаленные отложенные операции'
      Hint = 'Удаленные отложенные операции'
      OnExecute = ActDelsOtlOperExecute
    end
    object ActDelsOper: TAction
      Caption = 'Удаленные операции'
      Hint = 'Удаленные операции'
      OnExecute = ActDelsOperExecute
    end
    object ActCalc: TAction
      Caption = 'Калькулятор'
      Hint = 'Вызов встроенного калькулятора'
      ImageIndex = 92
      ShortCut = 16507
      OnExecute = ActCalcExecute
    end
    object ActImport: TAction
      Category = 'Импорт'
      Caption = 'Импорт из текстового файла'
      Hint = 'Импорт из текстового файла специального формата'
      OnExecute = ActImportExecute
    end
    object ActImportSettings: TAction
      Category = 'Импорт'
      Caption = 'Истановки импорта'
      OnExecute = ActImportSettingsExecute
    end
    object ActShowMap: TAction
      Caption = 'Показать карту'
      OnExecute = ActShowMapExecute
    end
    object ActShowPlanAcc: TAction
      Caption = 'ActShowPlanAcc'
      OnExecute = ActShowPlanAccExecute
    end
    object ActPeriod: TAction
      Caption = 'ActPeriod'
    end
  end
  object MainMenu2: TMainMenu
    Left = 72
    Top = 40
  end
  object ImageList1: TImageList
    ImageType = itMask
    Left = 344
    Top = 152
    Bitmap = {
      494C010101000400040010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      000000000000360000002800000040000000100000000100180000000000000C
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000004D4D4D5F5F5F0808081616163366
      996699FF00000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000099CCFF99CCFF99CCFF4D4D4D6666
      9933336600000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000099CCFF3366CC3333666699CC9999
      CC6699CC00000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000C6D6EF333333666666B2B2B26699
      CC9999CC00000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000B2B2B2292929A4A0A0B2B2B26699
      FFC6D6EF00000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000090A9AD33669999CCFFCBCBCB6699
      CCC6D6EF00000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000C0C0C05555557777778686866699
      CCCBCBCB00000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000100000000100010000000000800000000000000000000000
      000000000000000000000000FFFFFF00FFFF000000000000FFFF000000000000
      FFFF000000000000FFFF000000000000FFFF000000000000FFFF000000000000
      FFFF000000000000FFFF000000000000FFFF00000000000003FF000000000000
      03FF00000000000003FF00000000000003FF00000000000003FF000000000000
      03FF00000000000003FF00000000000000000000000000000000000000000000
      000000000000}
  end
end
