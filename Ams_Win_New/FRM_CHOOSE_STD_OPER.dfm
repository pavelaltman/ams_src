inherited FrmChooseStdOper: TFrmChooseStdOper
  Left = 245
  Width = 532
  Caption = 'FrmChooseStdOper'
  OldCreateOrder = True
  PixelsPerInch = 96
  TextHeight = 13
  object Button2: TButton [1]
    Left = 16
    Top = 48
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Button2'
    TabOrder = 2
    OnClick = Button2Click
  end
  inherited Panel1: TPanel
    inherited ToolBar1: TToolBar
      object ToolButton3: TToolButton
        Left = 23
        Top = 2
        Width = 8
        Caption = 'ToolButton3'
        ImageIndex = 23
        Style = tbsSeparator
      end
      object ToolButton2: TToolButton
        Left = 31
        Top = 2
        Hint = 'Ввод НЕстандартной операции'
        Caption = 'ToolButton2'
        ImageIndex = 114
        ParentShowHint = False
        ShowHint = True
        OnClick = ToolButton2Click
      end
    end
  end
  inherited SG: TAmsStringGrid
    Width = 360
  end
  inherited ActionList1: TActionList
    object ActShowInfo: TAction
      Caption = 'Показать информацию'
      Hint = 'Показать информацию'
      ShortCut = 116
      OnExecute = ActShowInfoExecute
    end
    object ActNonStdOper: TAction
      ShortCut = 115
      OnExecute = ActNonStdOperExecute
    end
    object ActRefresh: TAction
      Caption = 'ActRefresh'
      ShortCut = 16466
      OnExecute = ActRefreshExecute
    end
  end
  inherited PopupMenu1: TPopupMenu
    object N2: TMenuItem
      Action = ActShowInfo
    end
  end
end
