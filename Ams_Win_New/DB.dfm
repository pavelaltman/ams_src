object DataModule1: TDataModule1
  OldCreateOrder = False
  Left = 204
  Top = 109
  Height = 480
  Width = 696
  object DB1: TDatabase
    DatabaseName = 'DDD'
    DriverName = 'Microsoft Access Driver (*.mdb)'
    Params.Strings = (
      'ODBC DSN=MSDB1')
    SessionName = 'Default'
    Left = 32
    Top = 16
  end
  object Query1: TQuery
    DatabaseName = 'DDD'
    SQL.Strings = (
      'select * from Заказы')
    Left = 80
    Top = 16
  end
end
