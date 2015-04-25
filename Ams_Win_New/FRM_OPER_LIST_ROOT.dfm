object FrmOperListRoot: TFrmOperListRoot
  Left = 235
  Top = 141
  HelpContext = 6
  VertScrollBar.Visible = False
  Anchors = []
  AutoScroll = False
  BiDiMode = bdLeftToRight
  Caption = 'Журнал операций'
  ClientHeight = 464
  ClientWidth = 688
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsMDIChild
  KeyPreview = True
  OldCreateOrder = False
  ParentBiDiMode = False
  Position = poDefault
  PrintScale = poNone
  Scaled = False
  Visible = True
  OnActivate = FormActivate
  OnClose = FormClose
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 0
    Top = 371
    Width = 688
    Height = 13
    Align = alBottom
    Caption = 'Информация о текущей проводке'
    Layout = tlBottom
  end
  object Button1: TButton
    Left = 8
    Top = 72
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Button1'
    TabOrder = 4
    TabStop = False
    OnClick = Button1Click
  end
  object Grid2: TRxDBGrid
    Left = 0
    Top = 384
    Width = 688
    Height = 80
    Align = alBottom
    Color = clActiveBorder
    DataSource = DSrcItems
    Options = [dgTitles, dgColumnResize, dgColLines, dgRowLines, dgConfirmDelete, dgCancelOnExit]
    TabOrder = 1
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
    Columns = <
      item
        Expanded = False
        FieldName = 'ASIDE'
        Title.Caption = ' '
        Width = 16
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'ANAME'
        Title.Caption = 'Счет'
        Width = 67
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'VALUE'
        Title.Caption = 'Сумма'
        Width = 84
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'ANALINFO'
        Title.Caption = 'Аналитика'
        Width = 539
        Visible = True
      end>
  end
  object ToolBar1: TToolBar
    Left = 0
    Top = 0
    Width = 688
    Height = 29
    ButtonWidth = 24
    Caption = 'ToolBar1'
    Images = DatMod.ImageList1
    ParentShowHint = False
    ShowHint = True
    TabOrder = 2
    object ToolButton1: TToolButton
      Left = 0
      Top = 2
      Action = ActNew
      ParentShowHint = False
      ShowHint = True
    end
    object ToolButton2: TToolButton
      Left = 24
      Top = 2
      Action = ActEdit
      ParentShowHint = False
      ShowHint = True
    end
    object ToolButton4: TToolButton
      Left = 48
      Top = 2
      Action = ActS
      ParentShowHint = False
      ShowHint = True
    end
    object ToolButton5: TToolButton
      Left = 72
      Top = 2
      Action = ActDel
    end
    object ToolButton6: TToolButton
      Left = 96
      Top = 2
      Action = ActRead
    end
    object ToolButton7: TToolButton
      Left = 120
      Top = 2
      Action = ActHist
    end
    object ToolButton8: TToolButton
      Left = 144
      Top = 2
      Action = ActSel
    end
    object ToolButton9: TToolButton
      Left = 168
      Top = 2
      Action = ActAll
    end
    object ToolButton10: TToolButton
      Left = 192
      Top = 2
      Action = ActFind
    end
    object ToolButton11: TToolButton
      Left = 216
      Top = 2
      Action = ActFindAg
    end
    object ToolButton12: TToolButton
      Left = 240
      Top = 2
      Action = ActSort
    end
    object ToolButton13: TToolButton
      Left = 264
      Top = 2
      Action = ActAnSort
    end
    object ToolButton14: TToolButton
      Left = 288
      Top = 2
      Action = ActPer
    end
    object ToolButton3: TToolButton
      Left = 312
      Top = 2
      Action = ActPrint
    end
  end
  object RxSplitter1: TRxSplitter
    Left = 0
    Top = 29
    Width = 688
    Height = 3
    ControlFirst = ToolBar1
    Align = alTop
    BottomRightLimit = 0
  end
  object Grid1: TAmsDBGrid
    Left = 0
    Top = 32
    Width = 688
    Height = 339
    Align = alClient
    DataSource = DSrc
    Options = [dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgRowSelect, dgConfirmDelete, dgCancelOnExit, dgMultiSelect]
    PopupMenu = PopupMenu1
    ReadOnly = True
    TabOrder = 0
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
    OnDblClick = Grid1DblClick
    MultiSelect = True
    OnGetCellParams = Grid1GetCellParams
    DefaultRowHeight = 17
    Columns = <
      item
        Expanded = False
        FieldName = 'TEXT'
        Title.Caption = 'Текст'
        Width = 189
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'DOC_TYPE'
        Title.Caption = 'Документ'
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'DOC_NUM'
        Title.Caption = 'Номер'
        Width = 85
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'OP_DATE'
        Title.Caption = 'Дата'
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'SUMMA'
        Title.Caption = 'Сумма'
        Width = 80
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'DACCS'
        Title.Caption = 'Дебет'
        Width = 69
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'CACCS'
        Title.Caption = 'Кредит'
        Width = 95
        Visible = True
      end>
  end
  object DSrc: TDataSource
    AutoEdit = False
    DataSet = DSet
    Left = 128
    Top = 32
  end
  object DSet: TOracleDataSet
    SQL.Strings = (
      
        'select opers.rowid,opers.* , INIT.GetOperSum(op_id,op_subid) as ' +
        'SUMMA ,'
      '                INIT.GetOperAcc('#39'D'#39',op_id,op_subid) as DACCS,'
      '                INIT.GetOperAcc('#39'K'#39',op_id,op_subid) as CACCS'
      '                from INIT.opers'
      '                where status=1 AND post=:post'
      
        '                 AND op_date>=:beg_date AND op_date<:end_date AN' +
        'D OP_ID=:OP_ID'
      '                ORDER by OP_DATE '
      ' '
      ' '
      ' '
      ' ')
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Variables.Data = {
      0300000003000000090000003A4245475F444154450C00000000000000000000
      00090000003A454E445F444154450C0000000000000000000000050000003A50
      4F5354030000000000000000000000}
    StringFieldsOnly = False
    SequenceField.ApplyMoment = amOnPost
    OracleDictionary.EnforceConstraints = False
    OracleDictionary.UseMessageTable = False
    OracleDictionary.DefaultValues = False
    OracleDictionary.DynamicDefaults = False
    OracleDictionary.FieldKinds = False
    OracleDictionary.DisplayFormats = False
    OracleDictionary.RangeValues = False
    OracleDictionary.RequiredFields = True
    QBEDefinition.SaveQBEValues = True
    QBEDefinition.AllowFileWildCards = True
    QBEDefinition.QBEFontColor = clNone
    QBEDefinition.QBEBackgroundColor = clNone
    QBEDefinition.QBEFieldDefs = {
      030000000F000000050000004F505F49440000000000080000004F505F535542
      4944000000000006000000535441545553000000000004000000504F53540000
      0000000600000053544F524E4F01000000000400000054455854010101000008
      000000444F435F54595045010000000007000000444F435F4E554D0100000000
      09000000444F435F4E554D5F560100000000070000004F505F44415445010000
      000007000000434F4E445F494401000000000600000045444954454401000000
      000500000053554D4D4101000000000500000044414343530100000000050000
      0043414343530100000000}
    Cursor = crDefault
    AutoCalcFields = False
    DetailFields = 'OP_ID;OP_SUBID'
    ReadOnly = False
    LockingMode = lmCheckImmediate
    QueryAllRecords = False
    CountAllRecords = True
    RefreshOptions = []
    OnApplyRecord = DSetApplyRecord
    UpdatingTable = 'OPERS'
    CommitOnPost = False
    CachedUpdates = False
    QBEMode = False
    Session = DatMod.OSession
    DesignActivation = False
    Active = False
    AfterScroll = DSetAfterScroll
    Left = 160
    Top = 32
  end
  object DSetItems: TOracleDataSet
    SQL.Strings = (
      'select INIT.GetDCLetter(side) AS ASIDE,'
      '         INIT.User_Acc(a.name) AS ANAME,'
      '         INIT.AnalInfo(op_id,op_subid,i.code) AS ANALINFO, VALUE'
      '         from INIT.op_items i,INIT.accs a, INIT.sums'
      
        '         where op_id = :op_id and op_subid = :op_subid AND i.cod' +
        'e=a.code'
      '         AND a.node=0 AND sum_id=item_sum_id'
      '         ORDER BY ASIDE,ANAME ;'
      ' ')
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Variables.Data = {
      0300000002000000060000003A4F505F49440400000008000000000000000000
      C84000000000090000003A4F505F535542494404000000080000000000000000
      00000000000000}
    StringFieldsOnly = False
    SequenceField.ApplyMoment = amOnPost
    OracleDictionary.EnforceConstraints = False
    OracleDictionary.UseMessageTable = False
    OracleDictionary.DefaultValues = False
    OracleDictionary.DynamicDefaults = False
    OracleDictionary.FieldKinds = False
    OracleDictionary.DisplayFormats = False
    OracleDictionary.RangeValues = False
    OracleDictionary.RequiredFields = True
    QBEDefinition.SaveQBEValues = True
    QBEDefinition.AllowFileWildCards = True
    QBEDefinition.QBEFontColor = clNone
    QBEDefinition.QBEBackgroundColor = clNone
    QBEDefinition.QBEFieldDefs = {
      0300000004000000050000004153494445010000000005000000414E414D4501
      000000000500000056414C5545010000000008000000414E414C494E464F0100
      000000}
    Cursor = crDefault
    Master = DSet
    MasterFields = 'OP_ID;OP_SUBID'
    DetailFields = 'OP_ID;OP_SUBID'
    ReadOnly = False
    LockingMode = lmCheckImmediate
    QueryAllRecords = False
    CountAllRecords = False
    RefreshOptions = []
    CommitOnPost = True
    CachedUpdates = False
    QBEMode = False
    Session = DatMod.OSession
    DesignActivation = False
    Active = False
    Left = 16
    Top = 432
    object DSetItemsASIDE: TStringField
      FieldName = 'ASIDE'
      Size = 4000
    end
    object DSetItemsANAME: TStringField
      FieldName = 'ANAME'
      Size = 4000
    end
    object DSetItemsANALINFO: TStringField
      FieldName = 'ANALINFO'
      Size = 4000
    end
    object DSetItemsVALUE: TFloatField
      FieldName = 'VALUE'
      DisplayFormat = '0.00'
    end
  end
  object DSrcItems: TDataSource
    AutoEdit = False
    DataSet = DSetItems
    Left = 48
    Top = 432
  end
  object ActionList1: TActionList
    Images = DatMod.ImageList1
    Left = 592
    object ActEdit: TAction
      Caption = 'Редактировать [Enter]'
      Hint = 'Редактировать операцию'
      ImageIndex = 41
      OnExecute = ActEditExecute
    end
    object ActS: TAction
      Caption = 'Связанные операции [Alt+С]'
      Hint = 'Показать связанные операции'
      ImageIndex = 51
      OnExecute = ActSExecute
    end
    object ActDel: TAction
      Caption = 'Удалить'
      Hint = 'Удалить операцию'
      ImageIndex = 14
      ShortCut = 46
      OnExecute = ActDelExecute
    end
    object ActRead: TAction
      Caption = 'Просмотр [Пробел]'
      Hint = 'Просмотр операции'
      ImageIndex = 78
      OnExecute = ActReadExecute
    end
    object ActSel: TAction
      Caption = 'Выборка'
      Hint = 'Выборка операций'
      ImageIndex = 71
      ShortCut = 116
      OnExecute = ActSelExecute
    end
    object ActNew: TAction
      Caption = 'Новая'
      Hint = 'Ввод новой операции'
      ImageIndex = 40
      ShortCut = 45
      OnExecute = ActNewExecute
    end
    object ActAll: TAction
      Caption = 'Убрать выборку'
      Hint = 'Убрать выборку операций'
      ImageIndex = 72
      ShortCut = 16500
      OnExecute = ActAllExecute
    end
    object ActFind: TAction
      Caption = 'Найти операцию'
      Hint = 'Найти операцию'
      ImageIndex = 39
      ShortCut = 118
      OnExecute = ActFindExecute
    end
    object ActFindAg: TAction
      Caption = 'Найти далее'
      Hint = 'Найти далее'
      ImageIndex = 76
      ShortCut = 8310
      OnExecute = ActFindAgExecute
    end
    object ActSort: TAction
      Caption = 'Сортировка'
      Hint = 'Сортировка операций'
      ImageIndex = 69
      ShortCut = 120
      OnExecute = ActSortExecute
    end
    object ActAnSort: TAction
      Caption = 'Убрать сортировку'
      Hint = 'Убрать сортировку'
      ImageIndex = 70
      ShortCut = 16504
      OnExecute = ActAnSortExecute
    end
    object ActPer: TAction
      Caption = 'Период видимости'
      Hint = 'Выбрать период видимости'
      ImageIndex = 80
      ShortCut = 114
      OnExecute = ActPerExecute
    end
    object ActPrint: TAction
      Caption = 'Печать [Alt+П]'
      Hint = 'Вывести на печать'
      ImageIndex = 37
      OnExecute = ActPrintExecute
    end
    object ActHist: TAction
      Caption = 'История [Alt+И]'
      Hint = 'История Создания/Изменения/Просмотра/Удаления операций'
      ImageIndex = 82
      OnExecute = ActHistExecute
    end
    object ActShowTiedBase: TAction
      Category = 'New'
      Caption = 'Показывать связанную базу [Alt+Б]'
      Hint = 'Показывать поля связанной '#13#10'базы данных'
      OnExecute = ActShowTiedBaseExecute
    end
  end
  object PopupMenu1: TPopupMenu
    Images = DatMod.ImageList1
    Left = 624
    object N1: TMenuItem
      Action = ActNew
    end
    object Enter1: TMenuItem
      Action = ActEdit
    end
    object Alt1: TMenuItem
      Action = ActS
    end
    object N2: TMenuItem
      Action = ActDel
    end
    object N3: TMenuItem
      Action = ActRead
    end
    object Alt4: TMenuItem
      Action = ActHist
    end
    object N4: TMenuItem
      Action = ActSel
    end
    object N5: TMenuItem
      Action = ActAll
    end
    object N6: TMenuItem
      Action = ActFind
    end
    object N7: TMenuItem
      Action = ActFindAg
    end
    object N8: TMenuItem
      Action = ActSort
    end
    object N9: TMenuItem
      Action = ActAnSort
    end
    object Alt2: TMenuItem
      Action = ActPer
    end
    object Alt3: TMenuItem
      Action = ActPrint
    end
    object MenShowDB: TMenuItem
      Action = ActShowTiedBase
    end
  end
  object FindQ: TOracleQuery
    Session = DatMod.OSession
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Cursor = crDefault
    StringFieldsOnly = False
    Threaded = False
    ThreadSynchronized = True
    Left = 56
    Top = 96
  end
  object TMAScroll: TTimer
    Enabled = False
    OnTimer = TMAScrollTimer
    Left = 128
    Top = 64
  end
end
