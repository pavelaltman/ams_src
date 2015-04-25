inherited FrmHistBrws: TFrmHistBrws
  Caption = 'FrmHistBrws'
  OldCreateOrder = True
  PixelsPerInch = 96
  TextHeight = 13
  inherited Lab1: TLabel
    Visible = False
  end
  inherited CV: TAmsDbCatView
    Visible = False
  end
  inherited ToolBar3: TToolBar
    Visible = False
  end
  inherited Panel1: TPanel
    inherited DBG: TAmsDBGrid
      Options = [dgEditing, dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgConfirmDelete, dgCancelOnExit]
      MultiSelect = False
    end
  end
end
