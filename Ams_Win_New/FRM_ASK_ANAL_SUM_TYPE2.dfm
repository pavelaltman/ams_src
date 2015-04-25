inherited FrmAskAnalSumType2: TFrmAskAnalSumType2
  Width = 249
  Height = 160
  Caption = 'Выбор типа аналитики'
  PixelsPerInch = 96
  TextHeight = 13
  inherited Panel1: TPanel
    Top = 104
    Width = 241
    Height = 29
    inherited BitBtn1: TBitBtn
      Left = 153
      Top = 3
    end
    inherited BBOk: TBitBtn
      Left = 1
      Top = 3
    end
  end
  inherited RGSel: TRadioGroup
    Width = 241
    Height = 104
    Items.Strings = (
      'Аналитика остатка на начало'
      'Аналитика оборота по дебету'
      'Аналитика оборота по кредиту'
      'Аналитика операции [Alt+T]'
      'Аналитика остатка на конец [Alt+O]')
  end
end
