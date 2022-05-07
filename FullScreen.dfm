object frm_fullscreen: Tfrm_fullscreen
  Left = 266
  Top = 137
  Width = 648
  Height = 509
  Caption = 'LBA Screen Viewer - Full Screen'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnKeyPress = FormKeyPress
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Image1: TImage
    Left = 0
    Top = 0
    Width = 640
    Height = 480
    Align = alClient
    Stretch = True
    OnContextPopup = Image1ContextPopup
  end
end
