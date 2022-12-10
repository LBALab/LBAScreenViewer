/*
------------------------[ LBA Screen Viewer Source ]-------------------------
Copyright (C) 2004
--------------------------------[ ImgLib.h ]---------------------------------

Author: Alexandre Fontoura [xesf]
Begin : Sun Sep 12 2004

-------------------------------[ GNU License ]-------------------------------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

-----------------------------------------------------------------------------
*/

#ifndef LBAScreenViewer_mainH
#define LBAScreenViewer_mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "CSPIN.h"
#include <Dialogs.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <jpeg.hpp>
#include "FLAControler.h"
#include <Buttons.hpp>

//---------------------------------------------------------------------------

class Tfrm_img : public TForm
{
__published:	// IDE-managed Components
   TOpenDialog *OpenDialog1;
   TSaveDialog *SaveDialog1;
   TMainMenu *MainMenu1;
   TMenuItem *File1;
   TMenuItem *LoadResource1;
   TMenuItem *ExporttoBitmap1;
   TMenuItem *N1;
   TMenuItem *Quit1;
   TMenuItem *Help1;
   TMenuItem *About1;
   TStatusBar *status_img;
   TStatusBar *status_path;
   TStatusBar *status_palpath;
   TMenuItem *OpenLIMfile1;
   TMenuItem *N2;
   TOpenDialog *OpenDialog2;
   TScrollBox *ScrollBox1;
   TMenuItem *Import1;
   TMenuItem *Open1;
   TMenuItem *LoadLBARawSpritelsr1;
   TMenuItem *menu_closeall;
   TPanel *pl_bkg;
   TLabel *Label2;
   TLabel *lb_version;
   TLabel *Label4;
   TLabel *Label5;
   TPaintBox *LBAimg;
   TPopupMenu *img_popupmenu;
   TMenuItem *ExporttoLBAImagelim1;
   TMenuItem *N5;
   TMenuItem *CloseImage1;
   TMenuItem *Edit1;
   TMenuItem *CopyImageToClipboard1;
   TMenuItem *PasteImageFromClipboard1;
    TMenuItem *LoadLBA2SavedGameImagelba1;
    TImageList *ImageList1;
   TMenuItem *N4;
   TMenuItem *SelectNewPalette1;
   TMenuItem *SelectLBA1Palette1;
   TMenuItem *SelectLBA2Palette1;
   TMenuItem *SelectPaletteFromPath1;
   TToolBar *ToolBar1;
   TToolButton *ToolButton1;
   TToolButton *ToolButton2;
   TToolButton *ToolButton3;
   TToolButton *ToolButton4;
   TToolButton *ToolButton5;
   TToolButton *closeall;
   TMenuItem *Settings1;
   TImage *Image1;
   TMenuItem *ExportPaletteImagebmpjpggifpngpcx1;
   TMenuItem *View1;
   TMenuItem *FullScreen1;
   TMenuItem *SelectLastPaletteLoaded1;
   TTimer *Timer1;
   TPanel *pl_loading;
   TProgressBar *pb_loading;
   TLabel *lb_loading;
   TPopupMenu *fla_popupmenu;
   TMenuItem *MenuItem1;
   TMenuItem *MenuItem2;
   TMenuItem *MenuItem9;
   TMenuItem *MenuItem10;
   TMenuItem *LoadLBAFLAMoviesfla1;
   TToolButton *ToolButton6;
   TMenuItem *Exportlimpalbmpjpg1;
    TMenuItem *N3;
    TMenuItem *Checknewversion1;
    TMenuItem *BugTrackingSystemBugTrack1;
    TMenuItem *TheSacredCarrotHomePage1;
   TMenuItem *N6;
   TMenuItem *ConverttoLBA1Palette1;
   TMenuItem *ConverttoLBA2Palette1;
   TMenuItem *ConverttoOutputPalette1;
   TMenuItem *TESTE1;
   TMenuItem *N7;
   TMenuItem *ConvertBMPtoFLAMovies1;
   TPanel *Panel1;
   TLabel *Label1;
   TCSpinEdit *edt_index;
   TCheckBox *cb_autodraw;
   TBitBtn *bt_draw;
   TPanel *Panel2;
   TPaintBox *PaintBox2;
   TPaintBox *pb_color;
   TPaintBox *pb_red;
   TPaintBox *pb_green;
   TLabel *Label3;
   TLabel *Label6;
   TLabel *Label7;
   TPaintBox *pb_blue;
   TLabel *lb_red;
   TLabel *lb_green;
   TLabel *lb_blue;
   void __fastcall PaintBox2Paint(TObject *Sender);
   void __fastcall LoadResource1Click(TObject *Sender);
   void __fastcall OpenLIMfile1Click(TObject *Sender);
   void __fastcall Open1Click(TObject *Sender);
   void __fastcall LoadLBARawSpritelsr1Click(TObject *Sender);
   void __fastcall menu_closeallClick(TObject *Sender);
   void __fastcall Quit1Click(TObject *Sender);
   void __fastcall edt_indexChange(TObject *Sender);
   void __fastcall About1Click(TObject *Sender);
   void __fastcall cb_autodrawClick(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall FormDestroy(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall LBAimgPaint(TObject *Sender);
   void __fastcall Import1Click(TObject *Sender);
   void __fastcall LBAimgContextPopup(TObject *Sender, TPoint &MousePos,
          bool &Handled);
   void __fastcall CopyImageToClipboard1Click(TObject *Sender);
   void __fastcall PasteImageFromClipboard1Click(TObject *Sender);
    void __fastcall LoadLBA2SavedGameImagelba1Click(TObject *Sender);
    void __fastcall ExporttoBitmap1Click(TObject *Sender);
   void __fastcall SelectPaletteFromParh1Click(TObject *Sender);
   void __fastcall SelectLBA2Palette1Click(TObject *Sender);
   void __fastcall SelectNewPalette1Click(TObject *Sender);
   void __fastcall SelectLBA1Palette1Click(TObject *Sender);
   void __fastcall ToolButton15Click(TObject *Sender);
   void __fastcall Settings1Click(TObject *Sender);
   void __fastcall LBAimgMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
   void __fastcall LBAimgMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall LBAimgMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall ExportPaletteImagebmpjpggifpngpcx1Click(
          TObject *Sender);
   void __fastcall FullScreen1Click(TObject *Sender);
   void __fastcall SelectLastPaletteLoaded1Click(TObject *Sender);
   void __fastcall Timer1Timer(TObject *Sender);
   void __fastcall LoadLBAFLAMoviesfla1Click(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall MenuItem2Click(TObject *Sender);
   void __fastcall Exportlimpalbmpjpg1Click(TObject *Sender);
   void __fastcall MenuItem1Click(TObject *Sender);
    void __fastcall Checknewversion1Click(TObject *Sender);
    void __fastcall BugTrackingSystemBugTrack1Click(TObject *Sender);
    void __fastcall TheSacredCarrotHomePage1Click(TObject *Sender);
   void __fastcall ConverttoLBA1Palette1Click(TObject *Sender);
   void __fastcall ConverttoLBA2Palette1Click(TObject *Sender);
   void __fastcall ConverttoOutputPalette1Click(TObject *Sender);
   void __fastcall TESTE1Click(TObject *Sender);
   void __fastcall ConvertBMPtoFLAMovies1Click(TObject *Sender);
   void __fastcall bt_drawClick(TObject *Sender);
   void __fastcall PaintBox2MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
private:	// User declarations
   void saveSettings();
   void loadSettings();
   MESSAGE void DropFile(TMessage &Message);
public:		// User declarations
   __fastcall Tfrm_img(TComponent* Owner);

BEGIN_MESSAGE_MAP
MESSAGE_HANDLER(WM_DROPFILES, TMessage, DropFile);
END_MESSAGE_MAP(TForm);

};
//---------------------------------------------------------------------------
extern PACKAGE Tfrm_img *frm_img;
//---------------------------------------------------------------------------
#endif
