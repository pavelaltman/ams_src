object FrmInputOper: TFrmInputOper
  Left = 211
  Top = 111
  Width = 675
  Height = 327
  BorderWidth = 2
  Caption = 'Ввод операции'
  Color = clInactiveCaptionText
  Constraints.MinHeight = 320
  Constraints.MinWidth = 400
  ParentFont = True
  FormStyle = fsMDIChild
  OldCreateOrder = False
  PopupMenu = PopupMenu1
  Position = poDefault
  Visible = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnMouseMove = FormMouseMove
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 0
    Top = 32
    Width = 30
    Height = 13
    Caption = 'Текст'
  end
  object Label2: TLabel
    Left = 0
    Top = 72
    Width = 26
    Height = 13
    Caption = 'Дата'
  end
  object Label3: TLabel
    Left = 112
    Top = 72
    Width = 51
    Height = 13
    Caption = 'Документ'
  end
  object Label4: TLabel
    Left = 224
    Top = 72
    Width = 34
    Height = 13
    Caption = 'Номер'
  end
  object DBText1: TDBText
    Left = 362
    Top = 94
    Width = 47
    Height = 17
    DataField = 'DOC_NUM_V'
    DataSource = DSrc
  end
  object LabDbInfo: TLabel
    Left = 0
    Top = 112
    Width = 663
    Height = 17
    Anchors = [akLeft, akTop, akRight]
    AutoSize = False
    WordWrap = True
  end
  object ButEnter: TButton
    Left = 264
    Top = 8
    Width = 41
    Height = 17
    Caption = 'ButEnter'
    Default = True
    TabOrder = 12
    TabStop = False
    OnClick = ButEnterClick
  end
  object text_edit: TDBEdit
    Left = 0
    Top = 48
    Width = 661
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    DataField = 'TEXT'
    DataSource = DSrc
    TabOrder = 0
    OnKeyDown = FormKeyDown
  end
  object DBDateEdit1: TDBDateEdit
    Left = 0
    Top = 91
    Width = 89
    Height = 18
    DataField = 'OP_DATE'
    DataSource = DSrc
    DialogTitle = 'Выберите дату'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    NumGlyphs = 2
    ParentFont = False
    TabOrder = 1
    Weekends = [Sun, Sat]
    OnExit = DBDateEdit1Exit
    OnKeyDown = FormKeyDown
  end
  object DBEdit1: TDBEdit
    Left = 112
    Top = 90
    Width = 97
    Height = 21
    DataField = 'DOC_TYPE'
    DataSource = DSrc
    TabOrder = 2
    OnKeyDown = FormKeyDown
  end
  object DBEdit2: TDBEdit
    Left = 224
    Top = 90
    Width = 97
    Height = 21
    DataField = 'DOC_NUM'
    DataSource = DSrc
    TabOrder = 3
    OnEnter = DBEdit2Enter
    OnExit = DBEdit2Exit
    OnKeyDown = FormKeyDown
  end
  object items_grid: TRxDBGrid
    Left = 0
    Top = 136
    Width = 661
    Height = 124
    Anchors = [akLeft, akTop, akRight, akBottom]
    Color = clCaptionText
    DataSource = DSrcItems
    Options = [dgEditing, dgAlwaysShowEditor, dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgConfirmDelete]
    PopupMenu = PMenuGrid
    TabOrder = 5
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
    OnDblClick = items_gridDblClick
    OnKeyDown = items_gridKeyDown
    Columns = <
      item
        DropDownRows = 2
        Expanded = False
        FieldName = 'ASIDE'
        PickList.Strings = (
          'Д'
          'К')
        Width = 31
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
        Color = clBtnFace
        Expanded = False
        FieldName = 'ANALINFO'
        Title.Caption = 'Аналитика'
        Width = 437
        Visible = True
      end>
  end
  object storno_check: TDBCheckBox
    Left = 604
    Top = 88
    Width = 57
    Height = 25
    Anchors = [akTop, akRight]
    Caption = 'Сторно'
    DataField = 'STORNO'
    DataSource = DSrc
    TabOrder = 8
    ValueChecked = '1'
    ValueUnchecked = '0'
    OnKeyDown = FormKeyDown
  end
  object BBOk: TBitBtn
    Left = 8
    Top = 268
    Width = 89
    Height = 24
    Anchors = [akLeft, akBottom]
    Caption = 'Ok'
    TabOrder = 6
    OnClick = BBOkClick
    OnKeyDown = FormKeyDown
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
    Left = 562
    Top = 268
    Width = 97
    Height = 24
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = 'Выход'
    TabOrder = 10
    OnClick = BBCancelClick
    OnKeyDown = FormKeyDown
    OnMouseDown = BBCancelMouseDown
    OnMouseMove = BBCancelMouseMove
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
  object BBPost: TBitBtn
    Left = 104
    Top = 268
    Width = 105
    Height = 24
    Anchors = [akLeft, akBottom]
    Caption = 'Отложить'
    TabOrder = 7
    OnClick = BBPostClick
    OnKeyDown = FormKeyDown
    Glyph.Data = {
      76010000424D7601000000000000760000002800000020000000100000000100
      0400000000000001000000000000000000001000000000000000000000000000
      8000008000000080800080000000800080008080000080808000C0C0C0000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00FFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFF0000000000FFFFFFFF7777777777FFFF0FFFFFFFF0FF
      FFFFFF7FFFFFFFF7FFFF0F000000F0FFFFFFFF7F777777F7FFFF0FFFFFFFF0FF
      FFFFFF7FFFFFFFF7FFFF0F000000F0FFFFFFFF7F777777F7FFFF0FFFFFFFF0FF
      FFFFFF7FFFFFFFF7FFFF0F000000F0FFFFFFFF7F777777F7FFFF0FFFFFFFF0FF
      FFFFFF7FFFFFFFF7FFFF0F000F0000FFFFFFFF7F777F7777FFFF0FFFFF0F0FFF
      FFFFFF7FFFFF7F7FFFFF0FFFFF00FF9FFFFFFF7FFFFF77FFF0FF0000009FFF99
      FFFFFF7777770FFFF00FFFFFFF9999999FFFFFFFFFFF00000000FFFFFFFFFF99
      FFFFFFFFFFFFFFFFF00FFFFFFFFFFF9FFFFFFFFFFFFFFFFFF0FF}
    NumGlyphs = 2
  end
  object BitBtn1: TBitBtn
    Left = 462
    Top = 268
    Width = 95
    Height = 24
    Action = ActDocGen
    Anchors = [akRight, akBottom]
    Caption = '&Документ'
    TabOrder = 9
    OnKeyDown = FormKeyDown
    Glyph.Data = {
      36030000424D3603000000000000360000002800000010000000100000000100
      1800000000000003000000000000000000000000000000000000FF00FFFF00FF
      FF00FFFF00FF0000000000000000000000000000000000000000000000000000
      00000000FF00FFFF00FFFF00FFFF00FFFF00FFFF00FF000000FF00FFFF00FFFF
      00FFFF00FFFF00FFFF00FFFF00FFFF00FF000000FF00FFFF00FFFF00FFFF00FF
      FF00FFFF00FF000000FF00FF000000000000000000000000000000000000FF00
      FF000000FF00FFFF00FFFF00FFFF00FFFF00FFFF00FF000000FF00FFFF00FFFF
      00FFFF00FFFF00FFFF00FFFF00FFFF00FF000000FF00FFFF00FFFF00FFFF00FF
      FF00FFFF00FF000000FF00FF000000000000000000000000000000000000FF00
      FF000000FF00FFFF00FFFF00FFFF00FFFF00FFFF00FF000000FF00FFFF00FFFF
      00FFFF00FFFF00FFFF00FFFF00FFFF00FF000000FF00FFFF00FFFF00FFFF00FF
      FF00FFFF00FF000000FF00FF000000000000000000000000000000000000FF00
      FF000000FF00FFFF00FFFF00FFFF00FFFF00FFFF00FF000000FF00FFFF00FFFF
      00FFFF00FFFF00FFFF00FFFF00FFFF00FF000000FF00FFFF00FFFF00FFFF00FF
      FF00FFFF00FF000000FF00FF000000000000000000FF00FF0000000000000000
      00000000FF00FFFF00FFFF00FFFF00FFFF00FFFF00FF000000FF00FFFF00FFFF
      00FFFF00FFFF00FF000000FF00FF000000FF00FFFF00FFFF00FFFF00FFFF00FF
      FF00FFFF00FF000000FF00FFFF00FFFF00FFFF00FFFF00FF000000000000FF00
      FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF00000000000000000000
      0000000000000000000000FF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF
      FF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00
      FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF
      00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF
      FF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00
      FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF
      00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF}
  end
  object ToolBar1: TToolBar
    Left = 0
    Top = 0
    Width = 663
    Height = 29
    Caption = 'ToolBar1'
    Images = DatMod.ImageList1
    ParentShowHint = False
    ShowHint = True
    TabOrder = 11
    object ToolButton2: TToolButton
      Left = 0
      Top = 2
      Action = ActEdit
    end
    object ToolButton3: TToolButton
      Left = 23
      Top = 2
      Action = ActRead
    end
    object ToolButton8: TToolButton
      Left = 46
      Top = 2
      Action = ActDocType
    end
    object ToolButton9: TToolButton
      Left = 69
      Top = 2
      Action = ActStand
    end
    object ToolButton10: TToolButton
      Left = 92
      Top = 2
      Action = ActStandIz
    end
    object ToolButton11: TToolButton
      Left = 115
      Top = 2
      Action = ActDelNeS
    end
  end
  object ButBegin: TButton
    Left = 328
    Top = 88
    Width = 29
    Height = 25
    Hint = 'Начало ввода операции на основе стандартной операции'
    Caption = 'Нач'
    TabOrder = 4
    OnClick = ButBeginClick
  end
  object DSet: TOracleDataSet
    SQL.Strings = (
      
        'select * from INIT.opers o where op_id=:op_id AND op_subid=:op_s' +
        'ubid ')
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Variables.Data = {
      0300000002000000060000003A4F505F49440300000000000000000000000900
      00003A4F505F5355424944030000000000000000000000}
    StringFieldsOnly = False
    SequenceField.ApplyMoment = amOnPost
    OracleDictionary.EnforceConstraints = True
    OracleDictionary.UseMessageTable = True
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
      030000000C000000050000004F505F49440100000000080000004F505F535542
      4944010000000006000000535441545553010000000004000000504F53540100
      0000000600000053544F524E4F01000000000400000054455854010000000008
      000000444F435F54595045010000000007000000444F435F4E554D0100000000
      09000000444F435F4E554D5F560100000000070000004F505F44415445010000
      000007000000434F4E445F494401000000000600000045444954454401000000
      00}
    Cursor = crDefault
    ReadOnly = False
    LockingMode = lmCheckImmediate
    QueryAllRecords = True
    CountAllRecords = True
    RefreshOptions = []
    OnApplyRecord = DSetApplyRecord
    CommitOnPost = False
    CachedUpdates = False
    QBEMode = False
    Session = DatMod.OSession
    DesignActivation = True
    Active = False
    Left = 264
    Top = 40
    object DSetOP_ID: TFloatField
      FieldName = 'OP_ID'
      Required = True
    end
    object DSetOP_SUBID: TFloatField
      FieldName = 'OP_SUBID'
      Required = True
    end
    object DSetSTATUS: TIntegerField
      FieldName = 'STATUS'
    end
    object DSetPOST: TIntegerField
      FieldName = 'POST'
    end
    object DSetSTORNO: TIntegerField
      FieldName = 'STORNO'
    end
    object DSetTEXT: TStringField
      FieldName = 'TEXT'
      Size = 250
    end
    object DSetDOC_TYPE: TStringField
      FieldName = 'DOC_TYPE'
    end
    object DSetDOC_NUM: TStringField
      FieldName = 'DOC_NUM'
      Size = 30
    end
    object DSetDOC_NUM_V: TFloatField
      FieldName = 'DOC_NUM_V'
    end
    object DSetOP_DATE: TDateTimeField
      DisplayWidth = 10
      FieldName = 'OP_DATE'
    end
    object DSetCOND_ID: TFloatField
      FieldName = 'COND_ID'
    end
    object DSetEDITED: TFloatField
      FieldName = 'EDITED'
    end
  end
  object DSrc: TDataSource
    DataSet = DSet
    Left = 288
    Top = 40
  end
  object DSetItems: TOracleDataSet
    SQL.Strings = (
      'select i.rowid, i.*, INIT.GetDCLetter(side) AS ASIDE,'
      '         INIT.User_Acc(a.name) AS ANAME,'
      
        '         INIT.AnalInfo(op_id,op_subid,i.code) AS ANALINFO, VALUE' +
        ', anal_id'
      '         from INIT.op_items i,INIT.accs a, INIT.sums'
      
        '         where op_id = :op_id and op_subid = :op_subid AND i.cod' +
        'e=a.code'
      '         AND a.node=0 AND sum_id=item_sum_id'
      '         ORDER BY ASIDE,ANAME'
      ' ')
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Variables.Data = {
      0300000002000000060000003A4F505F49440300000000000000000000000900
      00003A4F505F5355424944030000000000000000000000}
    StringFieldsOnly = False
    SequenceField.ApplyMoment = amOnPost
    OracleDictionary.EnforceConstraints = True
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
      030000000E000000050000004153494445010000000005000000414E414D4501
      000000000500000056414C5545010000000008000000414E414C494E464F0100
      000000050000004F505F49440100000000080000004F505F5355424944010000
      0000070000004F505F44415445010000000004000000434F4445010000000004
      0000005349444501000000000B0000004954454D5F53554D5F49440100000000
      040000004E4F4445010000000008000000464F525F504F535401000000000700
      00005345544E414D45010000000007000000414E414C5F49440100000000}
    Cursor = crDefault
    DetailFields = 'OP_ID;OP_SUBID'
    ReadOnly = False
    LockingMode = lmCheckImmediate
    QueryAllRecords = False
    CountAllRecords = True
    RefreshOptions = [roAllFields]
    OnApplyRecord = DSetItemsApplyRecord
    CommitOnPost = True
    CachedUpdates = False
    QBEMode = False
    Session = DatMod.OSession
    DesignActivation = True
    Active = False
    OnNewRecord = DSetItemsNewRecord
    Left = 24
    Top = 152
    object DSetItemsASIDE: TStringField
      DisplayLabel = ' '
      FieldKind = fkInternalCalc
      FieldName = 'ASIDE'
      OnChange = DSetItemsASIDEChange
      Size = 4000
    end
    object DSetItemsANAME: TStringField
      FieldKind = fkInternalCalc
      FieldName = 'ANAME'
      OnChange = DSetItemsANAMEChange
      Size = 4000
    end
    object DSetItemsANALINFO: TStringField
      FieldKind = fkInternalCalc
      FieldName = 'ANALINFO'
      ReadOnly = True
      Size = 4000
    end
    object DSetItemsVALUE: TFloatField
      FieldKind = fkInternalCalc
      FieldName = 'VALUE'
      OnChange = DSetItemsVALUEChange
      DisplayFormat = ',0.00###'
    end
    object DSetItemsSIDE: TStringField
      FieldName = 'SIDE'
      Size = 1
    end
    object DSetItemsCODE: TFloatField
      FieldName = 'CODE'
      Required = True
    end
    object DSetItemsANAL_ID: TIntegerField
      FieldName = 'ANAL_ID'
    end
    object DSetItemsITEM_SUM_ID: TFloatField
      FieldName = 'ITEM_SUM_ID'
    end
    object DSetItemsOP_ID: TFloatField
      FieldName = 'OP_ID'
      Required = True
    end
    object DSetItemsOP_SUBID: TFloatField
      FieldName = 'OP_SUBID'
      Required = True
    end
    object DSetItemsOP_DATE: TDateTimeField
      FieldName = 'OP_DATE'
    end
    object DSetItemsNODE: TIntegerField
      FieldName = 'NODE'
    end
    object DSetItemsFOR_POST: TIntegerField
      FieldName = 'FOR_POST'
    end
    object DSetItemsSETNAME: TStringField
      FieldName = 'SETNAME'
    end
  end
  object DSrcItems: TDataSource
    DataSet = DSetItems
    Left = 56
    Top = 152
  end
  object ActionList1: TActionList
    Left = 64
    Top = 32
    object ActEdit: TAction
      Caption = 'Редактировать [Enter]'
      Hint = 'Редактировать операцию'
      ImageIndex = 41
      OnExecute = ActEditExecute
    end
    object ActRead: TAction
      Caption = 'Просмотр [Пробел]'
      Hint = 'Просмотр операции'
      ImageIndex = 78
      OnExecute = ActReadExecute
    end
    object ActAcc: TAction
      Caption = 'Счет [Alt+С]'
      Hint = 'Редактирование номера счета операции'
      ImageIndex = 0
    end
    object ActDeb: TAction
      Caption = 'Дебет/кредит [Alt+Е]'
      Hint = 'Редактирование счета операции дебет/кредит'
      ImageIndex = 92
    end
    object ActOtl: TAction
      Caption = 'Откладывание [Alt+О]'
      Hint = 'Отложить операцию'
      ImageIndex = 46
    end
    object ActDocGen: TAction
      Caption = '&Документ'
      Hint = 'Генерировать документ'
      ImageIndex = 26
      OnExecute = ActDocGenExecute
    end
    object ActDocType: TAction
      Caption = 'Виды документов [Alt+В]'
      Hint = 'Выбрать документ'
      ImageIndex = 94
      OnExecute = ActDocTypeExecute
    end
    object ActStand: TAction
      Caption = 'Стандартная оперция [Alt+С]'
      Hint = 'Выбрать стандартную операцию'
      ImageIndex = 98
      OnExecute = ActStandExecute
    end
    object ActStandIz: TAction
      Caption = 'Стандартная(изменить) (Alt+М)'
      Hint = 'Изменить стандартную операцию'
      ImageIndex = 100
      OnExecute = ActStandIzExecute
    end
    object ActAuto: TAction
      Caption = 'Автоматический ввод'
      Hint = 'Автоматический ввод операции'
      ImageIndex = 97
    end
    object ActDelNeS: TAction
      Caption = 'Удалить не стандартные'
      Hint = 'Удалить не стандартные операции'
      ImageIndex = 84
    end
    object ActOk: TAction
      Caption = 'ActOk'
      ShortCut = 113
      OnExecute = ActOkExecute
    end
    object ActShowTiedDB: TAction
      Caption = 'Показать связанную БД'
      Hint = 'Показать связанную БД'#13#10'в отдельном окне'
      ShortCut = 45
      OnExecute = ActShowTiedDBExecute
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 32
    Top = 32
    object Enter1: TMenuItem
      Action = ActEdit
    end
    object N3: TMenuItem
      Action = ActRead
    end
    object Alt1: TMenuItem
      Action = ActAcc
    end
    object Alt2: TMenuItem
      Action = ActDeb
    end
    object Alt3: TMenuItem
      Action = ActOtl
    end
    object Alt4: TMenuItem
      Action = ActDocType
    end
    object Alt5: TMenuItem
      Action = ActStand
    end
    object Alt6: TMenuItem
      Action = ActStandIz
    end
    object N4: TMenuItem
      Action = ActDelNeS
    end
    object N5: TMenuItem
      Action = ActAuto
    end
    object Alt7: TMenuItem
      Caption = 'Период видимости '
      Hint = 'Выбрать период видимости'
      ImageIndex = 80
      ShortCut = 114
    end
  end
  object CondOQ: TOracleQuery
    Session = DatMod.OSession
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Cursor = crDefault
    StringFieldsOnly = False
    Threaded = False
    ThreadSynchronized = True
    Left = 432
    Top = 80
  end
  object QCondItems: TOracleQuery
    Session = DatMod.OSession
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Cursor = crDefault
    StringFieldsOnly = False
    Threaded = False
    ThreadSynchronized = True
    Left = 472
    Top = 80
  end
  object PMenuGrid: TPopupMenu
    Left = 248
    Top = 176
    object NewAcc1: TMenuItem
      Caption = 'Новый счет'
      ShortCut = 45
      OnClick = NewAcc1Click
    end
    object DelAcc2: TMenuItem
      Caption = 'Удалить счет'
      ShortCut = 46
      OnClick = DelAcc2Click
    end
  end
end
