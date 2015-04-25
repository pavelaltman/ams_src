object DatMod: TDatMod
  OldCreateOrder = False
  OnDestroy = DataModuleDestroy
  Left = 300
  Top = 173
  Height = 480
  Width = 696
  object OQPool1: TOracleQuery
    SQL.Strings = (
      'select * from ams.licance'
      '')
    Session = MainFrm.OSess
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Cursor = crDefault
    StringFieldsOnly = False
    Threaded = False
    ThreadSynchronized = True
    Left = 32
    Top = 24
  end
  object OQPool2: TOracleQuery
    Session = MainFrm.OSess
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Cursor = crDefault
    StringFieldsOnly = False
    Threaded = False
    ThreadSynchronized = True
    Left = 80
    Top = 24
  end
  object OQPool3: TOracleQuery
    Session = MainFrm.OSess
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Cursor = crDefault
    StringFieldsOnly = False
    Threaded = False
    ThreadSynchronized = True
    Left = 128
    Top = 24
  end
  object OQPool4: TOracleQuery
    Session = MainFrm.OSess
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Cursor = crDefault
    StringFieldsOnly = False
    Threaded = False
    ThreadSynchronized = True
    Left = 176
    Top = 24
  end
  object OQPool5: TOracleQuery
    Session = MainFrm.OSess
    ReadBuffer = 25
    Optimize = True
    Debug = False
    Cursor = crDefault
    StringFieldsOnly = False
    Threaded = False
    ThreadSynchronized = True
    Left = 224
    Top = 24
  end
end
