object frm_BMP2FLA: Tfrm_BMP2FLA
  Left = 361
  Top = 252
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Convert BMPs to FLA Movies by MGLC'
  ClientHeight = 304
  ClientWidth = 478
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 48
    Width = 102
    Height = 13
    Caption = '* BMP Source Folder:'
  end
  object Label2: TLabel
    Left = 32
    Top = 8
    Width = 406
    Height = 13
    Caption = 
      'All BMP files must be in 256 colors and should have all the same' +
      ' prefix!'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label3: TLabel
    Left = 8
    Top = 96
    Width = 291
    Height = 13
    Caption = '* BMP Prefix: (ex: Twinsen0.bmp, where Twinsen is the prefix)'
  end
  object Label4: TLabel
    Left = 8
    Top = 144
    Width = 158
    Height = 13
    Caption = '* Number of frames (bmp images):'
  end
  object pgr_fla: TCGauge
    Left = 8
    Top = 240
    Width = 465
    Height = 22
    ForeColor = clBlue
  end
  object Label5: TLabel
    Left = 8
    Top = 192
    Width = 247
    Height = 13
    Caption = 'Additional KeyFrames (Frame Number. ex: 5 24 100):'
  end
  object Label6: TLabel
    Left = 56
    Top = 24
    Width = 353
    Height = 13
    Caption = 'All BMP files after a Keyframe must have the same 256 colors!'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label7: TLabel
    Left = 256
    Top = 48
    Width = 125
    Height = 13
    Caption = '* -> means obligatory fields'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object edt_folder: TEdit
    Left = 8
    Top = 64
    Width = 377
    Height = 21
    TabOrder = 0
  end
  object bt_browse: TButton
    Left = 392
    Top = 64
    Width = 75
    Height = 22
    Caption = 'Browse'
    TabOrder = 1
    OnClick = bt_browseClick
  end
  object edt_prefix: TEdit
    Left = 8
    Top = 112
    Width = 153
    Height = 21
    TabOrder = 2
  end
  object edt_frames: TEdit
    Left = 8
    Top = 160
    Width = 153
    Height = 21
    TabOrder = 3
  end
  object GroupBox1: TGroupBox
    Left = 312
    Top = 104
    Width = 161
    Height = 129
    Caption = 'Samples: '
    TabOrder = 5
    object lb_sampleidx: TLabel
      Left = 8
      Top = 40
      Width = 67
      Height = 13
      Caption = 'Sample Index:'
      Enabled = False
    end
    object lb_freq: TLabel
      Left = 8
      Top = 72
      Width = 92
      Height = 13
      Caption = 'Sample Frequence:'
      Enabled = False
    end
    object lb_repeat: TLabel
      Left = 8
      Top = 104
      Width = 76
      Height = 13
      Caption = 'Sample Repeat:'
      Enabled = False
    end
    object edt_sampleidx: TEdit
      Left = 112
      Top = 36
      Width = 41
      Height = 21
      Enabled = False
      TabOrder = 1
      Text = '0'
    end
    object cb_usesample: TCheckBox
      Left = 8
      Top = 16
      Width = 97
      Height = 17
      Caption = 'Use samples'
      TabOrder = 0
      OnClick = cb_usesampleClick
    end
    object edt_freq: TEdit
      Left = 112
      Top = 68
      Width = 41
      Height = 21
      Enabled = False
      TabOrder = 2
      Text = '0'
    end
    object edt_repeat: TEdit
      Left = 112
      Top = 100
      Width = 41
      Height = 21
      Enabled = False
      TabOrder = 3
      Text = '0'
    end
  end
  object bt_createfla: TButton
    Left = 312
    Top = 272
    Width = 161
    Height = 25
    Caption = 'Create FLA Movie'
    TabOrder = 6
    OnClick = bt_createflaClick
  end
  object bt_playfla: TButton
    Left = 200
    Top = 272
    Width = 99
    Height = 25
    Caption = 'Play FLA Movie'
    Enabled = False
    TabOrder = 7
    OnClick = bt_playflaClick
  end
  object bt_clear: TButton
    Left = 8
    Top = 272
    Width = 65
    Height = 25
    Caption = 'Clear'
    TabOrder = 8
    OnClick = bt_clearClick
  end
  object edt_keyframes: TEdit
    Left = 8
    Top = 208
    Width = 249
    Height = 21
    TabOrder = 4
  end
  object FolderDialog1: TFolderDialog
    Title = 'Select BMP Source Folder'
    Directory = 'D:\XesF\LBA\LBA Screen Viewer'
    DialogX = 0
    DialogY = 0
    Version = '1.0.0.0'
    Left = 440
    Top = 16
  end
end
