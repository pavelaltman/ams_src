object FrmGiveRights: TFrmGiveRights
  Left = 143
  Top = 142
  Width = 611
  Height = 437
  Caption = 'Выдача прав'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsMDIChild
  OldCreateOrder = False
  Position = poDefault
  Visible = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 603
    Height = 49
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    object CBUserName: TComboBox
      Left = 8
      Top = 8
      Width = 233
      Height = 21
      DropDownCount = 20
      ItemHeight = 13
      Sorted = True
      TabOrder = 0
      OnChange = CBUserNameChange
    end
    object ChBUsers: TCheckBox
      Left = 256
      Top = 8
      Width = 97
      Height = 17
      Caption = 'Пользователи'
      TabOrder = 1
      OnClick = ChBUsersClick
    end
    object ChBRoles: TCheckBox
      Left = 256
      Top = 24
      Width = 97
      Height = 17
      Caption = 'Роли'
      TabOrder = 2
      OnClick = ChBUsersClick
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 378
    Width = 603
    Height = 32
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    object BBCancel: TBitBtn
      Left = 80
      Top = 4
      Width = 97
      Height = 24
      Cancel = True
      Caption = 'Отменить'
      TabOrder = 0
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
    object BBOk: TBitBtn
      Left = 2
      Top = 4
      Width = 75
      Height = 24
      Caption = 'Ok'
      Default = True
      TabOrder = 1
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
  end
  object PC: TPageControl
    Left = 0
    Top = 49
    Width = 603
    Height = 329
    ActivePage = TSCat
    Align = alClient
    TabOrder = 2
    OnChange = PCChange
    object TSCat: TTabSheet
      Caption = 'Категории'
      object SGCat: TStringGrid
        Left = 0
        Top = 0
        Width = 595
        Height = 301
        Align = alClient
        DefaultRowHeight = 18
        FixedCols = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goDrawFocusSelected, goColSizing]
        TabOrder = 0
        OnDblClick = SGCatDblClick
        OnDrawCell = SGCatDrawCell
        ColWidths = (
          36
          102
          217
          33
          28)
      end
    end
    object TSAcc: TTabSheet
      Caption = 'Счета'
      ImageIndex = 1
      object SGAcc: TStringGrid
        Left = 0
        Top = 0
        Width = 595
        Height = 301
        Align = alClient
        DefaultRowHeight = 18
        FixedCols = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goColSizing]
        TabOrder = 0
        OnDblClick = SGCatDblClick
        OnDrawCell = SGCatDrawCell
        ColWidths = (
          41
          103
          224
          25
          25)
      end
    end
    object TSDoc: TTabSheet
      Caption = 'Документы'
      ImageIndex = 2
      object SGDoc: TStringGrid
        Left = 0
        Top = 0
        Width = 595
        Height = 301
        Align = alClient
        DefaultRowHeight = 18
        FixedCols = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goColSizing]
        TabOrder = 0
        OnDblClick = SGCatDblClick
        OnDrawCell = SGCatDrawCell
        ColWidths = (
          82
          129
          150
          26
          28)
      end
    end
    object TSFrm: TTabSheet
      Caption = 'Формы'
      ImageIndex = 3
      object SGFrm: TStringGrid
        Left = 0
        Top = 0
        Width = 595
        Height = 301
        Align = alClient
        DefaultRowHeight = 18
        FixedCols = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goColSizing]
        TabOrder = 0
        OnDblClick = SGCatDblClick
        OnDrawCell = SGCatDrawCell
        ColWidths = (
          64
          136
          130
          37
          37)
      end
    end
    object TSBd: TTabSheet
      Caption = 'Базы данных'
      ImageIndex = 4
      object SGBd: TStringGrid
        Left = 0
        Top = 0
        Width = 595
        Height = 301
        Align = alClient
        ColCount = 7
        DefaultRowHeight = 18
        FixedCols = 0
        TabOrder = 0
        OnDblClick = SGCatDblClick
        OnDrawCell = SGCatDrawCell
        ColWidths = (
          77
          123
          123
          28
          26
          31
          21)
      end
    end
    object TSAn: TTabSheet
      Caption = 'Аналитика'
      ImageIndex = 5
      object SGAn: TStringGrid
        Left = 0
        Top = 0
        Width = 595
        Height = 301
        Align = alClient
        ColCount = 7
        DefaultRowHeight = 18
        FixedCols = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goColSizing]
        TabOrder = 0
        OnDblClick = SGCatDblClick
        OnDrawCell = SGCatDrawCell
        ColWidths = (
          74
          98
          140
          26
          30
          32
          29)
      end
    end
    object TSOper: TTabSheet
      Caption = 'Операции'
      ImageIndex = 6
      object SGOpers: TStringGrid
        Left = 0
        Top = 0
        Width = 595
        Height = 301
        Align = alClient
        ColCount = 7
        DefaultRowHeight = 18
        FixedCols = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goDrawFocusSelected, goColSizing]
        TabOrder = 0
        OnDblClick = SGCatDblClick
        OnDrawCell = SGCatDrawCell
        ColWidths = (
          32
          123
          157
          26
          26
          26
          26)
      end
    end
    object TSFuncs: TTabSheet
      Caption = 'Функции'
      ImageIndex = 7
      object SGFuncs: TStringGrid
        Left = 0
        Top = 0
        Width = 595
        Height = 301
        Align = alClient
        ColCount = 4
        DefaultRowHeight = 18
        FixedCols = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goColSizing]
        TabOrder = 0
        OnDblClick = SGFuncsDblClick
        OnDrawCell = SGFuncsDrawCell
        ColWidths = (
          36
          279
          167
          19)
      end
    end
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
    Left = 376
    Top = 8
  end
end
