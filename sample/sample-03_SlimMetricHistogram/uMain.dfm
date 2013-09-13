object fMain: TfMain
  Left = 192
  Top = 107
  Width = 574
  Height = 238
  Caption = 'fMain'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object MM: TMemo
    Left = 0
    Top = 8
    Width = 561
    Height = 169
    ScrollBars = ssVertical
    TabOrder = 0
  end
  object Run: TButton
    Left = 0
    Top = 184
    Width = 75
    Height = 25
    Caption = 'Run'
    TabOrder = 1
    OnClick = RunClick
  end
end
