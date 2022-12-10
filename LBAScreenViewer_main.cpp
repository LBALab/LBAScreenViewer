/*
------------------------[ LBA Screen Viewer Source ]-------------------------
Copyright (C) 2004
-------------------------------[ ImgLib.cpp ]--------------------------------

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

#include <vcl.h>
#include <Clipbrd.hpp>
#pragma hdrstop

#include "LBAScreenViewer_main.h"
#include "About.h"
#include "ImgLib.h"
#include "HQRLib.h"
#include "Settings.h"
#include "FullScreen.h"
#include "FLALib.h"
#include "FLAControler.h"
#include "GIFImage.hpp"
#include "pngimage.hpp"
#include "PCXCtrl.hpp"
#include "frmBMP2FLA.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma link "FLAControler"
#pragma resource "*.dfm"
Tfrm_img *frm_img;
AnsiString Path;
AnsiString palPath;
String vnum = "1.7";

extern unsigned char* imgPtr;
extern unsigned char* palPtr;
extern int filesize;

extern vector<TFLA> FLA;
extern TFLAheader FLAheader;
bool IsPlaying = false;
extern int samplesInFLA;

int w;
int h;

Graphics::TBitmap *pBitmap = new Graphics::TBitmap();
Graphics::TBitmap *palpic = new Graphics::TBitmap();

extern Pal TPalette[256];

TPoint StartPoint;                  
bool Panning = false;
const crOpenHand = 5;
const crCloseHand = 6;

int canOpen=0;

//---------------------------------------------------------------------------
__fastcall Tfrm_img::Tfrm_img(TComponent* Owner)
   : TForm(Owner)
{
   frm_img->Caption = frm_img->Caption + " v" + vnum + " by xesf";
   lb_version->Caption = lb_version->Caption + " " + vnum;

   pBitmap->PixelFormat = pf8bit;
   pBitmap->Transparent = true;
   pBitmap->TransparentColor = clWhite;
   resetPal();

   /*frm_img->MediaPlayer1->FileName = "flasample\\57.wav";
                frm_img->MediaPlayer1->Open();
                frm_img->MediaPlayer1->Play(); */

   // ############################################
   // Open File -----------------------------------------------
   if(ParamCount()>0){
       canOpen = 1;

       if(StrPos(LowerCase(ParamStr(2)).c_str(),".pal"))
          palPath = ParamStr(2);
       else
          Path = ParamStr(2);

       if(StrPos(LowerCase(ParamStr(1)).c_str(),".pal"))
           palPath = ParamStr(1);
       else
           Path = ParamStr(1);
   }

   // ############################################
   //---------------------------------------------------------------

}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::FormCreate(TObject *Sender)
{
   DragAcceptFiles(Handle,true);
   AnsiString tmp = ExtractFileDir(Application->ExeName)+"\\config.ini";
   if(!frm_settings->file_exists(tmp.c_str()))
       ShowMessage("You can set LBA association files in the [View]->[Settings] menu! Take a look...");

  Screen->Cursors[crOpenHand] = LoadCursor(HInstance, "OPEN");
  Screen->Cursors[crCloseHand] = LoadCursor(HInstance, "CLOSE");
  LBAimg->Cursor = crOpenHand;
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::FormDestroy(TObject *Sender)
{
   DragAcceptFiles(Handle,false);
}
//---------------------------------------------------------------------------

MESSAGE void Tfrm_img::DropFile(TMessage &Message)
{
   //menu_closeallClick(this);
   
 /*  LBAimg->Canvas->FillRect(Rect(0,0,640,480));
   pBitmap->Canvas->FillRect(Rect(0,0,640,480));    */

   int nFiles;
   char buffer[256];
   nFiles = DragQueryFile((HDROP) Message.WParam,0xFFFFFFFF,NULL,0);

   if(frm_controlers->Showing){
          frm_controlers->Close();
          IsPlaying = false;
   }

   if(nFiles==1)
   {
       DragQueryFile((HDROP) Message.WParam,0,buffer,256); // 0 means one file dragged
       DragFinish((HDROP) Message.WParam);

       AnsiString *xis = new AnsiString(buffer);

       if(StrPos(LowerCase(*xis).c_str(),".pal"))
           palPath = *xis;
       else
           Path = *xis;

       delete xis;

       // IF RESOURCE FILE
       if(StrPos(LowerCase(Path).c_str(),".hqr") || StrPos(LowerCase(Path).c_str(),".ile")){
           edt_index->Value = 0;
           cb_autodraw->Enabled = True;
           status_img->Panels->Items[2]->Text = "Info: Loading High Quality Resource ...";
           status_img->Update();
           status_path->Panels->Items[0]->Text = "Path: " + Path;
           palPath = "";
           status_palpath->Panels->Items[0]->Text = "Palette Path: --[ not specified ]--";           
           LBAimg->Canvas->FillRect(Rect(0,0,640,480));
           LoadImageFromResource(0, Path, LBAimg->Canvas, PaintBox2->Canvas, true);
           pl_bkg->Visible = False;
           edt_index->Enabled = True;
           status_img->Panels->Items[2]->Text = "Info: High Quality Resource succesfully opened!";
           menu_closeall->Enabled = True;
           closeall->Enabled = True;
           CopyImageToClipboard1->Enabled = True;
           PasteImageFromClipboard1->Enabled = True;
           SelectNewPalette1->Enabled = False;
           SelectLBA1Palette1->Enabled = False;
           SelectLBA2Palette1->Enabled = False;
           SelectPaletteFromPath1->Enabled = False;
           SelectLastPaletteLoaded1->Enabled = False;
       }
       // IF BRICK/SPRITE
       else if(StrPos(LowerCase(Path).c_str(),".brk") || StrPos(LowerCase(Path).c_str(),".lsp")){
           edt_index->Value = 0;
           cb_autodraw->Enabled = False;
           status_img->Panels->Items[2]->Text = "Info: Opening ...";
           status_img->Update();
           LoadBrickFromFile(Path, LBAimg->Canvas, PaintBox2->Canvas);
           pl_bkg->Visible = False;
           status_img->Panels->Items[2]->Text = "Info: Image succesfully opened!";
           status_path->Panels->Items[0]->Text = "Path: " + Path;
           palPath = "";
           status_palpath->Panels->Items[0]->Text = "Palette Path: --[ not specified ]--";
           menu_closeall->Enabled = True;
           closeall->Enabled = True;           
           CopyImageToClipboard1->Enabled = True;
           PasteImageFromClipboard1->Enabled = True;
           SelectNewPalette1->Enabled = False;
           SelectLBA1Palette1->Enabled = False;
           SelectLBA2Palette1->Enabled = False;
           SelectPaletteFromPath1->Enabled = False;
           SelectLastPaletteLoaded1->Enabled = False;
       }
       // IF RAW SPRITE
       else if(StrPos(LowerCase(Path).c_str(),".lsr")){
           edt_index->Value = 0;
           cb_autodraw->Enabled = False;
           status_img->Panels->Items[2]->Text = "Info: Opening ...";
           status_img->Update();
           LoadRawSpriteFromFile(Path, LBAimg->Canvas, PaintBox2->Canvas);
           pl_bkg->Visible = False;
           status_img->Panels->Items[2]->Text = "Info: Image succesfully opened!";
           status_path->Panels->Items[0]->Text = "Path: " + Path;
           palPath = "";
           status_palpath->Panels->Items[0]->Text = "Palette Path: --[ not specified ]--";
           menu_closeall->Enabled = True;
           closeall->Enabled = True;
           CopyImageToClipboard1->Enabled = True;
           PasteImageFromClipboard1->Enabled = True;
           SelectNewPalette1->Enabled = False;
           SelectLBA1Palette1->Enabled = False;
           SelectLBA2Palette1->Enabled = False;
           SelectPaletteFromPath1->Enabled = False;
           SelectLastPaletteLoaded1->Enabled = False;
       }
       else if(StrPos(LowerCase(Path).c_str(),".lba")){
           edt_index->Value = 0;
           cb_autodraw->Enabled = False;
           status_img->Panels->Items[2]->Text = "Info: Opening ...";
           status_img->Update();
           LoadLBASavedGameFromFile(Path, LBAimg->Canvas, PaintBox2->Canvas);
           pl_bkg->Visible = False;
           status_img->Panels->Items[2]->Text = "Info: Image succesfully opened!";
           status_path->Panels->Items[0]->Text = "Path: " + Path;
           palPath = "";
           status_palpath->Panels->Items[0]->Text = "Palette Path: --[ not specified ]--";
           menu_closeall->Enabled = True;
           closeall->Enabled = True;
           CopyImageToClipboard1->Enabled = True;
           PasteImageFromClipboard1->Enabled = True;
           SelectNewPalette1->Enabled = False;
           SelectLBA1Palette1->Enabled = False;
           SelectLBA2Palette1->Enabled = False;
           SelectPaletteFromPath1->Enabled = False;
           SelectLastPaletteLoaded1->Enabled = False;
       }
       // IF BMP
       else if(StrPos(LowerCase(Path).c_str(),".bmp")){
           edt_index->Value = 0;
           cb_autodraw->Enabled = False;
           Graphics::TBitmap *pBitmap2 = new Graphics::TBitmap();
           pBitmap2->LoadFromFile(Path);
           if(pBitmap2->PixelFormat == pf8bit){
               pBitmap->Assign(pBitmap2);
               pBitmap->FreeImage();
               DrawLBAImage(LBAimg->Canvas);
               loadPaletteFromBitmap();
               DrawPalette(PaintBox2->Canvas);
               LBAimg->Width = pBitmap->Width;
               LBAimg->Height = pBitmap->Height;
               AnsiString xis = "Image Size: ";
               xis = xis + LBAimg->Width;
               xis = xis + "x" + LBAimg->Height;
               frm_img->status_img->Panels->Items[1]->Text = xis;
               filesize = ResourceSize(Path.c_str());
               status_img->Panels->Items[0]->Text = "File Size: " + IntToStr(filesize);
               status_path->Panels->Items[0]->Text = "Path: " + Path;
               palPath = "";
               status_palpath->Panels->Items[0]->Text = "Palette Path: --[ not specified ]--";
               status_img->Panels->Items[2]->Text = "Info: Bitmap succesfully imported!";
               pl_bkg->Visible = False;
               menu_closeall->Enabled = True;
               closeall->Enabled = True;
               cb_autodraw->Enabled = False;
               CopyImageToClipboard1->Enabled = True;
               PasteImageFromClipboard1->Enabled = True;
               SelectNewPalette1->Enabled = False;
               SelectLBA1Palette1->Enabled = False;
               SelectLBA2Palette1->Enabled = False;
               SelectPaletteFromPath1->Enabled = False;
               SelectLastPaletteLoaded1->Enabled = False;
               delete pBitmap2;
           }
           else
               ShowMessage("In this version is just possible import 8bit Bitmaps!");
       }
       else if(StrPos(LowerCase(Path).c_str(),".jpg") || StrPos(LowerCase(Path).c_str(),".jpeg")){
           edt_index->Value = 0;
           cb_autodraw->Enabled = False;
           TJPEGImage *jpg = new TJPEGImage();
           jpg->PixelFormat = jf8Bit;
           jpg->LoadFromFile(Path);
           if(jpg->PixelFormat == jf8Bit){
               pBitmap->Assign(jpg);
               loadPaletteFromBitmap();
               DrawPalette(PaintBox2->Canvas);
               pBitmap->FreeImage();
               DrawLBAImage(LBAimg->Canvas);
               LBAimg->Width = pBitmap->Width;
               LBAimg->Height = pBitmap->Height;
               AnsiString xis = "Image Size: ";
               xis = xis + LBAimg->Width;
               xis = xis + "x" + LBAimg->Height;
               frm_img->status_img->Panels->Items[1]->Text = xis;
               filesize = ResourceSize(Path.c_str());
               status_img->Panels->Items[0]->Text = "File Size: " + IntToStr(filesize);
               status_path->Panels->Items[0]->Text = "Path: " + Path;
               palPath = "";
               status_palpath->Panels->Items[0]->Text = "Palette Path: --[ not specified ]--";               
               status_img->Panels->Items[2]->Text = "Info: JPEG succesfully opened!";
               pl_bkg->Visible = False;
               menu_closeall->Enabled = True;
               closeall->Enabled = True;
               cb_autodraw->Enabled = False;
               CopyImageToClipboard1->Enabled = True;
               PasteImageFromClipboard1->Enabled = True;
               ExporttoLBAImagelim1->Enabled = True;
               SelectNewPalette1->Enabled = False;
               SelectLBA1Palette1->Enabled = False;
               SelectLBA2Palette1->Enabled = False;
               SelectLastPaletteLoaded1->Enabled = False;
               delete jpg;
           }
           else
               ShowMessage("In this version is just possible import 8bit JPEG!");  
       }
       // GIF Image
       else if(StrPos(LowerCase(Path).c_str(),".gif")){
           edt_index->Value = 0;
           cb_autodraw->Enabled = False;
           TGIFImage *gif = new TGIFImage();
           gif->LoadFromFile(Path);
           if(gif->Version == gv87a){
               pBitmap->Assign(gif);
               pBitmap->PixelFormat = pf8bit;               
               pBitmap->FreeImage();
               DrawLBAImage(LBAimg->Canvas);
               loadPaletteFromBitmap();
               DrawPalette(PaintBox2->Canvas);
               LBAimg->Width = pBitmap->Width;
               LBAimg->Height = pBitmap->Height;
               AnsiString xis = "Image Size: ";
               xis = xis + LBAimg->Width;
               xis = xis + "x" + LBAimg->Height;
               frm_img->status_img->Panels->Items[1]->Text = xis;
               filesize = ResourceSize(Path.c_str());
               status_img->Panels->Items[0]->Text = "File Size: " + IntToStr(filesize);
               status_path->Panels->Items[0]->Text = "Path: " + Path;
               palPath = "";
               status_palpath->Panels->Items[0]->Text = "Palette Path: --[ not specified ]--";
               status_img->Panels->Items[2]->Text = "Info: GIF succesfully opened!";
               pl_bkg->Visible = False;
               menu_closeall->Enabled = True;
               closeall->Enabled = True;               
               cb_autodraw->Enabled = False;
               CopyImageToClipboard1->Enabled = True;
               PasteImageFromClipboard1->Enabled = True;
           }
           delete gif;
       }
       //PNG Image
       else if(StrPos(LowerCase(Path).c_str(),".png")){
           edt_index->Value = 0;
           cb_autodraw->Enabled = False;
           TPNGObject *png = new TPNGObject();
           png->LoadFromFile(Path);
//           if(png->Version == gv87a){
               pBitmap->Assign(png);
               pBitmap->PixelFormat = pf8bit;               
               pBitmap->FreeImage();
               DrawLBAImage(LBAimg->Canvas);
               loadPaletteFromBitmap();
               DrawPalette(PaintBox2->Canvas);
               LBAimg->Width = pBitmap->Width;
               LBAimg->Height = pBitmap->Height;
               AnsiString xis = "Image Size: ";
               xis = xis + LBAimg->Width;
               xis = xis + "x" + LBAimg->Height;
               frm_img->status_img->Panels->Items[1]->Text = xis;
               filesize = ResourceSize(Path.c_str());
               status_img->Panels->Items[0]->Text = "File Size: " + IntToStr(filesize);
               status_path->Panels->Items[0]->Text = "Path: " + Path;
               palPath = "";
               status_palpath->Panels->Items[0]->Text = "Palette Path: --[ not specified ]--";
               status_img->Panels->Items[2]->Text = "Info: PNG succesfully opened!";
               pl_bkg->Visible = False;
               menu_closeall->Enabled = True;
               closeall->Enabled = True;               
               cb_autodraw->Enabled = False;
               CopyImageToClipboard1->Enabled = True;
               PasteImageFromClipboard1->Enabled = True;
  //         }
           delete png;
       }
       else if(StrPos(LowerCase(Path).c_str(),".pcx"))
       {
           TPCXBitmap *pcx = new TPCXBitmap();
           pcx->LoadFromFile(Path);
               pBitmap->Assign(pcx);
               pBitmap->FreeImage();
               DrawLBAImage(LBAimg->Canvas);
               loadPaletteFromBitmap();
               DrawPalette(PaintBox2->Canvas);
               LBAimg->Width = pBitmap->Width;
               LBAimg->Height = pBitmap->Height;
               palPath = "";
               AnsiString xis = "Image Size: ";
               xis = xis + LBAimg->Width;
               xis = xis + "x" + LBAimg->Height;
               frm_img->status_img->Panels->Items[1]->Text = xis;
               filesize = ResourceSize(Path.c_str());
               status_img->Panels->Items[0]->Text = "File Size: " + IntToStr(filesize);
               status_path->Panels->Items[0]->Text = "Path: " + Path;
               status_img->Panels->Items[2]->Text = "Info: PCX succesfully opened!";
               pl_bkg->Visible = False;
               menu_closeall->Enabled = True;
               closeall->Enabled = True;
               cb_autodraw->Enabled = False;
               CopyImageToClipboard1->Enabled = True;
               PasteImageFromClipboard1->Enabled = True;
               delete pcx;
       }
       else if(StrPos(LowerCase(Path).c_str(),".fla")){
           status_img->Panels->Items[2]->Text = "Info: FLA succesfully opened!";
           status_path->Panels->Items[0]->Text = "Path: " + Path;
           edt_index->Value = 0;
           cb_autodraw->Enabled = False;
           DragFinish((HDROP) Message.WParam);           
           pl_bkg->Visible = False;
           palPath = "";
           OpenFLA(Path.c_str(), LBAimg->Canvas);
           menu_closeall->Enabled = True;
           closeall->Enabled = True;
           cb_autodraw->Enabled = False;
           CopyImageToClipboard1->Enabled = True;
           PasteImageFromClipboard1->Enabled = True;
       /*    SelectNewPalette1->Enabled = True;
           SelectLBA1Palette1->Enabled = True;
           SelectLBA2Palette1->Enabled = True;
           SelectPaletteFromPath1->Enabled = True;
           SelectLastPaletteLoaded1->Enabled = True;     */
       }
       else if(StrPos(LowerCase(palPath).c_str(),".pal")){
           edt_index->Value = 0;
           cb_autodraw->Enabled = False;
           DragFinish((HDROP) Message.WParam);
           if(StrPos(LowerCase(Path).c_str(),".lim")){
               LoadImageFromFile(Path, palPath, LBAimg->Canvas, PaintBox2->Canvas);
               status_path->Panels->Items[0]->Text = "Path: " + Path;
               menu_closeall->Enabled = True;
               pl_bkg->Visible = False;
               closeall->Enabled = True;
               cb_autodraw->Enabled = False;
               CopyImageToClipboard1->Enabled = True;
               PasteImageFromClipboard1->Enabled = True;
               SelectNewPalette1->Enabled = True;
               SelectLBA1Palette1->Enabled = True;
               SelectLBA2Palette1->Enabled = True;
               SelectPaletteFromPath1->Enabled = True;
               SelectLastPaletteLoaded1->Enabled = True;
           }
           else
               LoadPaletteFromFile(palPath, LBAimg->Canvas, PaintBox2->Canvas);

           status_palpath->Panels->Items[0]->Text = "Palette Path: " + palPath;
           status_img->Panels->Items[2]->Text = "Info: Palette succesfully opened!";
       }
       else if(StrPos(LowerCase(Path).c_str(),".lim")){
           edt_index->Value = 0;
           cb_autodraw->Enabled = False;
           DragFinish((HDROP) Message.WParam);
           LoadImageFromFile(Path, LBAimg->Canvas, PaintBox2->Canvas, false);
           pl_bkg->Visible = False;
           status_img->Panels->Items[2]->Text = "Info: Image succesfully opened!";
           status_path->Panels->Items[0]->Text = "Path: " + Path;
           menu_closeall->Enabled = True;
           closeall->Enabled = True;
           cb_autodraw->Enabled = False;
           CopyImageToClipboard1->Enabled = True;
           PasteImageFromClipboard1->Enabled = True;
           SelectNewPalette1->Enabled = True;
           SelectLBA1Palette1->Enabled = True;
           SelectLBA2Palette1->Enabled = True;
           SelectPaletteFromPath1->Enabled = True;
           SelectLastPaletteLoaded1->Enabled = True;
       }
       else
           ShowMessage("Invalid file type!");
   }
   else if(nFiles==2)
   {
       edt_index->Value = 0;
       cb_autodraw->Enabled = False;
       AnsiString spal;
       AnsiString spath;
       for(int a=0; a<2; a++){
           DragQueryFile((HDROP) Message.WParam,a,buffer,256);
           AnsiString *xis = new AnsiString(buffer);
           if(StrPos(LowerCase(*xis).c_str(),".pal"))
               palPath = *xis;
           else if(StrPos(LowerCase(*xis).c_str(),".lim"))
               Path = *xis;
           else
               ShowMessage("Invalid file type!");
           delete xis;
       }
       DragFinish((HDROP) Message.WParam);
       pl_bkg->Visible = False;
       LoadImageFromFile(Path, palPath, LBAimg->Canvas, PaintBox2->Canvas);
       status_img->Panels->Items[2]->Text = "Info: Image succesfully opened!";
       status_path->Panels->Items[0]->Text = "Path: " + Path;
       status_palpath->Panels->Items[0]->Text = "Palette Path: " + palPath;
       menu_closeall->Enabled = True;
       closeall->Enabled = True;       
       cb_autodraw->Enabled = False;
       CopyImageToClipboard1->Enabled = True;
       PasteImageFromClipboard1->Enabled = True;
       SelectNewPalette1->Enabled = True;
       SelectLBA1Palette1->Enabled = True;
       SelectLBA2Palette1->Enabled = True;
       SelectPaletteFromPath1->Enabled = True;
       SelectLastPaletteLoaded1->Enabled = True;
   }
   else
       ShowMessage("You cannot drag more than 2 files. See Readme.txt for details.");
   DrawLBAImage(LBAimg->Canvas);
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::PaintBox2Paint(TObject *Sender)
{
   DrawPalette(PaintBox2->Canvas);
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::LoadResource1Click(TObject *Sender)
{
   if(OpenDialog1->Execute())
   {
       menu_closeallClick(this);
       if(frm_controlers->Showing){
          frm_controlers->Close();
          IsPlaying = false;
       }
       status_img->Panels->Items[2]->Text = "Info: Loading High Quality Resource ...";
       status_img->Update();
       Path = OpenDialog1->FileName;
       status_path->Panels->Items[0]->Text = "Path: " + Path;

       LBAimg->Canvas->FillRect(Rect(0,0,640,480));

       LoadImageFromResource(0, Path, LBAimg->Canvas, PaintBox2->Canvas, true);

       edt_index->Value = 0;
       pl_bkg->Visible = False;
       cb_autodraw->Enabled = True;
       edt_index->Enabled = True;
       status_img->Panels->Items[2]->Text = "Info: High Quality Resource succesfully opened!";
       menu_closeall->Enabled = True;
       closeall->Enabled = True;
       CopyImageToClipboard1->Enabled = True;
       PasteImageFromClipboard1->Enabled = True;
   }
   else
       status_img->Panels->Items[2]->Text = "Info: Load High Quality Resource aborted by user ...";
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::OpenLIMfile1Click(TObject *Sender)
{
   status_img->Panels->Items[2]->Text = "Info: Opening ...";
   status_img->Update();
   OpenDialog2->Title = "Open LBA Image (You can multiselect the image and the palette)";
   OpenDialog2->Filter = "LBA Image/Palette (*.lim, *.pal)|*.lim;*.pal";
   OpenDialog2->Options << ofAllowMultiSelect;
   if(OpenDialog2->Execute())
   {
        menu_closeallClick(this);
       if(frm_controlers->Showing){
          frm_controlers->Close();
          IsPlaying = false;
       }

       if(OpenDialog2->Files->Count > 2) return;
       if(OpenDialog2->Files->Count == 2){
           if(StrPos(LowerCase(OpenDialog2->Files->Strings[1]).c_str(),".pal"))
               palPath = OpenDialog2->Files->Strings[1];
           else
               Path = OpenDialog2->Files->Strings[1];
       }

       if(StrPos(LowerCase(OpenDialog2->Files->Strings[0]).c_str(),".pal"))
           palPath = OpenDialog2->Files->Strings[0];
       else
           Path = OpenDialog2->Files->Strings[0];

       edt_index->Value = 0;
       cb_autodraw->Enabled = False;

       if(OpenDialog2->Files->Count==1 && StrPos(LowerCase(Path).c_str(),".lim")){
          pl_bkg->Visible = False;
          LoadImageFromFile(Path, LBAimg->Canvas, PaintBox2->Canvas, false);
          status_path->Panels->Items[0]->Text = "Path: " + Path;
       }
       else if(OpenDialog2->Files->Count==1 && StrPos(LowerCase(palPath).c_str(),".pal")){
          pl_bkg->Visible = True;
          LoadPaletteFromFile(palPath, LBAimg->Canvas, PaintBox2->Canvas);
          status_palpath->Panels->Items[0]->Text = "Palette Path: " + palPath;
       }
       else if(OpenDialog2->Files->Count==2 && StrPos(LowerCase(Path).c_str(),".lim") && StrPos(LowerCase(palPath).c_str(),".pal")){
          pl_bkg->Visible = False;
          LoadImageFromFile(Path, palPath, LBAimg->Canvas, PaintBox2->Canvas);
          status_path->Panels->Items[0]->Text = "Path: " + Path;
          status_palpath->Panels->Items[0]->Text = "Palette Path: " + palPath;
       }

       status_img->Panels->Items[2]->Text = "Info: Image succesfully opened!";
       menu_closeall->Enabled = True;
       closeall->Enabled = True;
       cb_autodraw->Enabled = False;
       CopyImageToClipboard1->Enabled = True;
       PasteImageFromClipboard1->Enabled = True;
       SelectNewPalette1->Enabled = True;
       SelectLBA1Palette1->Enabled = True;
       SelectLBA2Palette1->Enabled = True;
       SelectPaletteFromPath1->Enabled = True;
       SelectLastPaletteLoaded1->Enabled = True;
   }
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::Open1Click(TObject *Sender)
{
   status_img->Panels->Items[2]->Text = "Info: Opening ...";
   status_img->Update();
   OpenDialog2->Title = "Open LBA Brick/Sprite";
   OpenDialog2->Filter = "LBA Brick/Sprite (*.brk, *.lsp)|*.brk;*.lsp";
   OpenDialog2->Options >> ofAllowMultiSelect;
   if(OpenDialog2->Execute())
   {
       menu_closeallClick(this);
       if(frm_controlers->Showing){
          frm_controlers->Close();
          IsPlaying = false;
       }

       edt_index->Value = 0;
       ExporttoLBAImagelim1->Enabled = False;
       CopyImageToClipboard1->Enabled = True;
       PasteImageFromClipboard1->Enabled = True;
       Path = OpenDialog2->FileName;
       LoadBrickFromFile(Path, LBAimg->Canvas, PaintBox2->Canvas);
       pl_bkg->Visible = False;
       status_img->Panels->Items[2]->Text = "Info: Image succesfully opened!";
       status_path->Panels->Items[0]->Text = "Path: " + Path;
       menu_closeall->Enabled = True;
       closeall->Enabled = True;
       SelectNewPalette1->Enabled = False;
       SelectLBA1Palette1->Enabled = False;
       SelectLBA2Palette1->Enabled = False;
       SelectPaletteFromPath1->Enabled = False;
       SelectLastPaletteLoaded1->Enabled = False;
   }
   else
      status_img->Panels->Items[2]->Text = "Info: Opened image aborted by user ...";
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::LoadLBARawSpritelsr1Click(TObject *Sender)
{
   menu_closeallClick(this);
   status_img->Panels->Items[2]->Text = "Info: Opening ...";
   status_img->Update();
   OpenDialog2->Title = "Open LBA2 Raw Sprite";
   OpenDialog2->Filter = "LBA2 Raw Sprite (*.lsr)|*.lsr";
   OpenDialog2->Options >> ofAllowMultiSelect;
   if(OpenDialog2->Execute())
   {
       menu_closeallClick(this);
       if(frm_controlers->Showing){
          frm_controlers->Close();
          IsPlaying = false;
       }

       edt_index->Value = 0;
       ExporttoLBAImagelim1->Enabled = False;
       CopyImageToClipboard1->Enabled = True;
       PasteImageFromClipboard1->Enabled = True;
       Path = OpenDialog2->FileName;
       LoadRawSpriteFromFile(Path, LBAimg->Canvas, PaintBox2->Canvas);
       pl_bkg->Visible = False;
       status_img->Panels->Items[2]->Text = "Info: Image succesfully opened!";
       status_path->Panels->Items[0]->Text = "Path: " + Path;
       menu_closeall->Enabled = True;
       closeall->Enabled = True;
       SelectNewPalette1->Enabled = False;
       SelectLBA1Palette1->Enabled = False;
       SelectLBA2Palette1->Enabled = False;
       SelectPaletteFromPath1->Enabled = False;
       SelectLastPaletteLoaded1->Enabled = False;
   }
   else
      status_img->Panels->Items[2]->Text = "Info: Opened image aborted by user ...";
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::menu_closeallClick(TObject *Sender)
{
   menu_closeall->Enabled = False;
   closeall->Enabled = False;
   edt_index->Enabled = False;
   edt_index->Value = 0;
   LBAimg->Canvas->FillRect(Rect(0,0,640,480));
   resetPal();
   DrawPalette(PaintBox2->Canvas);
   LBAimg->Width = 105;
   LBAimg->Height = 105;
   pl_bkg->Visible = True;
   bt_draw->Enabled = False;
   cb_autodraw->Enabled = False;
   CopyImageToClipboard1->Enabled = False;
   PasteImageFromClipboard1->Enabled = False;
   SelectNewPalette1->Enabled = False;
   SelectLBA1Palette1->Enabled = False;
   SelectLBA2Palette1->Enabled = False;
   SelectPaletteFromPath1->Enabled = False;
   SelectLastPaletteLoaded1->Enabled = False;
   status_img->Panels->Items[0]->Text = "File Size:";
   status_img->Panels->Items[1]->Text = "Image Size:";
   status_img->Panels->Items[2]->Text = "Info: Image succesfully closed!";
   status_path->Panels->Items[0]->Text = "Path: --[ not specified ]--";
   status_palpath->Panels->Items[0]->Text = "Palette Path: --[ not specified ]--";
   Path="";
   palPath="";
   if(frm_fullscreen->Showing)
   {
      frm_fullscreen->Close();
      frm_fullscreen->Image1->Canvas->FillRect(Rect(0,0,Image1->Width,Image1->Height));
   }
   if(frm_controlers->Showing){
      frm_controlers->Close();
      IsPlaying = false;
   }
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::Quit1Click(TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::edt_indexChange(TObject *Sender)
{
   if(cb_autodraw->Checked == True){
       LBAimg->Canvas->FillRect(Rect(0,0,640,480));
       status_img->Panels->Items[2]->Text = "Info: Loading ...";
       status_img->Update();
       LoadImageFromResource(edt_index->Value, Path, LBAimg->Canvas, PaintBox2->Canvas, false);
       status_img->Panels->Items[2]->Text = "Info: Image sucesfully opened!";
   }
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::About1Click(TObject *Sender)
{
   frm_about->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::cb_autodrawClick(TObject *Sender)
{
   if(cb_autodraw->Checked == True)
   {
       bt_draw->Enabled = False;
   }
   else
       bt_draw->Enabled = True;
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::FormClose(TObject *Sender, TCloseAction &Action)
{
//   saveSettings();
   AnsiString asdf = ExtractFileDir(Application->ExeName)+"\\temp.dat";
   remove(asdf.c_str());
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::FormResize(TObject *Sender)
{
   Label2->Left = (Image1->Width-177)/2;
   Label2->Top = (Image1->Height*64)/360;

   lb_version->Left = (Image1->Width-305)/2;
   lb_version->Top = (Image1->Height*88)/360;

   Label4->Left = (Image1->Width-279)/2;
   Label4->Top = (Image1->Height*160)/360;

   Label5->Left = (Image1->Width-537)/2;
   Label5->Top = (Image1->Height*240)/360;

   // Loading Panel
   pl_loading->Left = (Image1->Width-233)/2;
   pl_loading->Top = (Image1->Height*160)/360;
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::LBAimgPaint(TObject *Sender)
{
   DrawLBAImage(LBAimg->Canvas);
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::Import1Click(TObject *Sender)
{
   OpenDialog2->Title = "Import Images";
   OpenDialog2->Filter = "All possible files(*.bmp, *.jpg, *.jpeg,*.gif,*.png,*.pcx)|*.bmp;*.jpg;*.jpeg;*.gif;*.png;*.pcx|Bitmap(*.bmp)|*.bmp|JPEG Images(*.jpg, *.jpeg)|*.jpg;*.jpeg|GIF Images (*.gif)|*.gif|PNG Images (*.png)|*.png|PCX Images (*.pcx)|*.pcx";
   OpenDialog2->Options >> ofAllowMultiSelect;
   if(OpenDialog2->Execute()){
      if(frm_controlers->Showing){
         frm_controlers->Close();
         IsPlaying = false;
      }
       edt_index->Value = 0;
       cb_autodraw->Enabled = False;
       Path = OpenDialog2->FileName;
       if(StrPos(LowerCase(OpenDialog2->FileName).c_str(),".bmp")){
           Graphics::TBitmap *pBitmap2 = new Graphics::TBitmap();
           pBitmap2->LoadFromFile(OpenDialog2->FileName);
           if(pBitmap2->PixelFormat == pf8bit){
               pBitmap->Assign(pBitmap2);
               pBitmap->FreeImage();
               loadPaletteFromBitmap();
               DrawPalette(PaintBox2->Canvas);
               DrawLBAImage(LBAimg->Canvas);
               LBAimg->Width = pBitmap->Width;
               LBAimg->Height = pBitmap->Height;
               AnsiString xis = "Image Size: ";
               xis = xis + LBAimg->Width;
               xis = xis + "x" + LBAimg->Height;
               frm_img->status_img->Panels->Items[1]->Text = xis;
               filesize = ResourceSize(SaveDialog1->FileName.c_str());
               status_img->Panels->Items[0]->Text = "File Size: " + IntToStr(filesize);
               status_path->Panels->Items[0]->Text = "Path: " + Path;
               status_img->Panels->Items[2]->Text = "Info: Bitmap succesfully imported!";
               pl_bkg->Visible = False;
               menu_closeall->Enabled = True;
               closeall->Enabled = True;
               delete pBitmap2;
           }
           else
               ShowMessage("In this version is just possible import 8bit Bitmaps!");
       }
       else if(StrPos(LowerCase(OpenDialog2->FileName).c_str(),".jpg") || StrPos(LowerCase(OpenDialog2->FileName).c_str(),".jpeg"))
       {
           TJPEGImage *jpg = new TJPEGImage();
           jpg->PixelFormat = jf8Bit;
           jpg->LoadFromFile(Path);
           if(jpg->PixelFormat == jf8Bit){
               pBitmap->Assign(jpg);
               pBitmap->FreeImage();
               DrawLBAImage(LBAimg->Canvas);
               loadPaletteFromBitmap();
               DrawPalette(PaintBox2->Canvas);
               LBAimg->Width = pBitmap->Width;
               LBAimg->Height = pBitmap->Height;
               AnsiString xis = "Image Size: ";
               xis = xis + LBAimg->Width;
               xis = xis + "x" + LBAimg->Height;
               frm_img->status_img->Panels->Items[1]->Text = xis;
               filesize = ResourceSize(Path.c_str());
               status_img->Panels->Items[0]->Text = "File Size: " + IntToStr(filesize);
               status_path->Panels->Items[0]->Text = "Path: " + Path;
               status_img->Panels->Items[2]->Text = "Info: JPEG succesfully imported!";
               pl_bkg->Visible = False;
               menu_closeall->Enabled = True;
               closeall->Enabled = True;
               delete jpg;
           }
           else
               ShowMessage("In this version is just possible import 8bit JPEG!");
       }
       else if(StrPos(LowerCase(OpenDialog2->FileName).c_str(),".gif"))
       {
           TGIFImage *gif = new TGIFImage();
           gif->LoadFromFile(Path);
           if(gif->Version == gv87a){
               pBitmap->Assign(gif);
               pBitmap->FreeImage();
               DrawLBAImage(LBAimg->Canvas);
               loadPaletteFromBitmap();
               DrawPalette(PaintBox2->Canvas);
               LBAimg->Width = pBitmap->Width;
               LBAimg->Height = pBitmap->Height;
               AnsiString xis = "Image Size: ";
               xis = xis + LBAimg->Width;
               xis = xis + "x" + LBAimg->Height;
               frm_img->status_img->Panels->Items[1]->Text = xis;
               filesize = ResourceSize(Path.c_str());
               status_img->Panels->Items[0]->Text = "File Size: " + IntToStr(filesize);
               status_path->Panels->Items[0]->Text = "Path: " + Path;
               status_img->Panels->Items[2]->Text = "Info: JPEG succesfully imported!";
               pl_bkg->Visible = False;
               menu_closeall->Enabled = True;
               closeall->Enabled = True;
               delete gif;
           }
           else
               ShowMessage("In this version is just possible import 8bit GIF!");
       }
       else if(StrPos(LowerCase(OpenDialog2->FileName).c_str(),".png"))
       {
           TPNGObject *png = new TPNGObject();
           png->LoadFromFile(Path);
//           if(jpg->PixelFormat == jf8Bit){
               pBitmap->Assign(png);
               pBitmap->FreeImage();
               DrawLBAImage(LBAimg->Canvas);
               loadPaletteFromBitmap();
               DrawPalette(PaintBox2->Canvas);
               LBAimg->Width = pBitmap->Width;
               LBAimg->Height = pBitmap->Height;
               AnsiString xis = "Image Size: ";
               xis = xis + LBAimg->Width;
               xis = xis + "x" + LBAimg->Height;
               frm_img->status_img->Panels->Items[1]->Text = xis;
               filesize = ResourceSize(Path.c_str());
               status_img->Panels->Items[0]->Text = "File Size: " + IntToStr(filesize);
               status_path->Panels->Items[0]->Text = "Path: " + Path;
               status_img->Panels->Items[2]->Text = "Info: PNG succesfully imported!";
               pl_bkg->Visible = False;
               menu_closeall->Enabled = True;
               closeall->Enabled = True;
               delete png;
         /*  }
           else
               ShowMessage("In this version is just possible import 8bit JPEG!"); */
       }
       else if(StrPos(LowerCase(OpenDialog2->FileName).c_str(),".pcx"))
       {
           TPCXBitmap *pcx = new TPCXBitmap();
           pcx->LoadFromFile(Path);
               pBitmap->Assign(pcx);
               pBitmap->FreeImage();
               DrawLBAImage(LBAimg->Canvas);
               loadPaletteFromBitmap();
               DrawPalette(PaintBox2->Canvas);
               LBAimg->Width = pBitmap->Width;
               LBAimg->Height = pBitmap->Height;
               AnsiString xis = "Image Size: ";
               xis = xis + LBAimg->Width;
               xis = xis + "x" + LBAimg->Height;
               frm_img->status_img->Panels->Items[1]->Text = xis;
               filesize = ResourceSize(Path.c_str());
               status_img->Panels->Items[0]->Text = "File Size: " + IntToStr(filesize);
               status_path->Panels->Items[0]->Text = "Path: " + Path;
               status_img->Panels->Items[2]->Text = "Info: PCX succesfully imported!";
               pl_bkg->Visible = False;
               menu_closeall->Enabled = True;
               closeall->Enabled = True;
               delete pcx;
       }
   }else
      status_img->Panels->Items[2]->Text = "Info: Imported bitmap aborted by user ...";
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::LBAimgContextPopup(TObject *Sender,
      TPoint &MousePos, bool &Handled)
{
    TMouse *xis = new TMouse();
    if(LBAimg->Width!=105 && LBAimg->Height!=105 && !StrPos(LowerCase(Path).c_str(),".fla")){
        img_popupmenu->Popup(xis->CursorPos.x,xis->CursorPos.y);
    }
    else if(StrPos(LowerCase(Path).c_str(),".fla"))
    {
        fla_popupmenu->Popup(xis->CursorPos.x,xis->CursorPos.y);
    }
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::CopyImageToClipboard1Click(TObject *Sender)
{
   Clipboard()->Assign(pBitmap);
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::PasteImageFromClipboard1Click(TObject *Sender)
{
   pBitmap->Assign(Clipboard());
   LBAimg->Width = pBitmap->Width;
   LBAimg->Height = pBitmap->Height;
   AnsiString xis = "Image Size: ";
   xis = xis + LBAimg->Width;
   xis = xis + "x" + LBAimg->Height;
   frm_img->status_img->Panels->Items[1]->Text = xis;
   loadPaletteFromBitmap();
   DrawPalette(PaintBox2->Canvas);
   DrawLBAImage(LBAimg->Canvas);
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::LoadLBA2SavedGameImagelba1Click(TObject *Sender)
{
   status_img->Panels->Items[2]->Text = "Info: Opening ...";
   status_img->Update();
   OpenDialog2->Title = "Open LBA2 Saved Game Raw Image";
   OpenDialog2->Filter = "LBA2 Saved Game (*.lba)|*.lba";
   OpenDialog2->Options >> ofAllowMultiSelect;
   if(OpenDialog2->Execute())
   {
        menu_closeallClick(this);
       if(frm_controlers->Showing){
          frm_controlers->Close();
          IsPlaying = false;
       }

       edt_index->Value = 0;
       ExporttoLBAImagelim1->Enabled = True;
       CopyImageToClipboard1->Enabled = True;
       PasteImageFromClipboard1->Enabled = True;
       SelectNewPalette1->Enabled = False;
       SelectLBA1Palette1->Enabled = False;
       SelectLBA2Palette1->Enabled = False;
       SelectPaletteFromPath1->Enabled = False;
       SelectLastPaletteLoaded1->Enabled = False;
       Path = OpenDialog2->FileName;
       LoadLBASavedGameFromFile(Path, LBAimg->Canvas, PaintBox2->Canvas);
   }
   else
      status_img->Panels->Items[2]->Text = "Info: Opened image aborted by user ...";
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::ExporttoBitmap1Click(TObject *Sender)
{
   SaveDialog1->Title = "Export ...";
   SaveDialog1->Filter = "LBA Images (*.lim)|*.lim|LBA Palette (*.pal)|*.pal|Bitmap (*.bmp)|*.bmp|JPEG Images (*.jpg, *.jpeg)|*.jpg;*.jpeg|GIF Image (*.gif)|*.gif|PNG Image (*.png)|*.png|PCX Image (*.pcx)|*.pcx|Raw Palette (*.raw)|*.raw";
   SaveDialog1->InitialDir = ExtractFileDir(Path);
   if(SaveDialog1->Execute()){
       // LIM
       if(SaveDialog1->FilterIndex == 1){
           if(!StrPos(LowerCase(SaveDialog1->FileName).c_str(),".lim"))
               SaveDialog1->FileName = SaveDialog1->FileName + ".lim";
           getFileSize(LBAimg->Width);
           unsigned char * savePtr = (unsigned char *)malloc(filesize);
           SaveLBAImageToPtr(savePtr);
           FILE* resourceFile = openResourceAs(SaveDialog1->FileName.c_str());
           saveResource(resourceFile, savePtr,filesize);
           closeResource(resourceFile);
           status_img->Panels->Items[2]->Text = "Info: Image succesfully exported!";
       }
       // PAL
       else if(SaveDialog1->FilterIndex == 2){
           if(!StrPos(LowerCase(SaveDialog1->FileName).c_str(),".pal"))
               SaveDialog1->FileName = SaveDialog1->FileName + ".pal";
           unsigned char * ptr = (unsigned char *)malloc(768);
           SaveLBAPalette(ptr);
           FILE* resourceFile = openResourceAs(SaveDialog1->FileName.c_str());
           saveResource(resourceFile, ptr,768);
           closeResource(resourceFile);
           status_img->Panels->Items[2]->Text = "Info: Palette succesfully saved!";
       }
       // BMP
       else if(SaveDialog1->FilterIndex == 3){
           SaveLBAImageToBitmap(SaveDialog1->FileName);
           status_img->Panels->Items[2]->Text = "Info: Image succesfully saved!";
       }
       // Jpeg
       else if(SaveDialog1->FilterIndex == 4){
           SaveLBAImageToJpeg(SaveDialog1->FileName);
           status_img->Panels->Items[2]->Text = "Info: Image succesfully saved!";
       }
       // GIF
       else if(SaveDialog1->FilterIndex == 5){
           SaveLBAImageToGIF(SaveDialog1->FileName);
           status_img->Panels->Items[2]->Text = "Info: Image succesfully saved!";
       }
       // PNG
       else if(SaveDialog1->FilterIndex == 6){
           SaveLBAImageToPNG(SaveDialog1->FileName);
           status_img->Panels->Items[2]->Text = "Info: Image succesfully saved!";
       }
       // PCX
       else if(SaveDialog1->FilterIndex == 7){
           SaveLBAImageToPCX(SaveDialog1->FileName);
           status_img->Panels->Items[2]->Text = "Info: Image succesfully saved!";
       }
       // Raw PAL
       else if(SaveDialog1->FilterIndex == 8){
           if(!StrPos(LowerCase(SaveDialog1->FileName).c_str(),".raw"))
               SaveDialog1->FileName = SaveDialog1->FileName + ".raw";
           unsigned char * ptr = (unsigned char *)malloc(768);
           SaveLBAPalette(ptr);
           FILE* resourceFile = openResourceAs(SaveDialog1->FileName.c_str());
           saveResource(resourceFile, ptr,768);
           closeResource(resourceFile);
           status_img->Panels->Items[2]->Text = "Info: Palette succesfully saved!";
       }
   }else
       status_img->Panels->Items[2]->Text = "Info: Export aborted by user ...";

}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::SelectPaletteFromParh1Click(TObject *Sender)
{
   if(palPath != ""){
       LBAimg->Canvas->FillRect(Rect(0,0,640,480));
       LoadPaletteFromFile(palPath, LBAimg->Canvas, PaintBox2->Canvas);
          DrawPalette(PaintBox2->Canvas);
       if(StrPos(LowerCase(Path).c_str(),".lim"))
           LoadImageFromFile(Path, LBAimg->Canvas, PaintBox2->Canvas, true);
       else if(StrPos(LowerCase(Path).c_str(),".hqr"))
           ShowMessage("Isn't Possible for this type of file!");
       else if(StrPos(LowerCase(Path).c_str(),".brk") || StrPos(LowerCase(Path).c_str(),".lsp"))
           LoadBrickFromFile(Path, LBAimg->Canvas, PaintBox2->Canvas);
       else if(StrPos(LowerCase(Path).c_str(),".lsr"))
           LoadRawSpriteFromFile(Path, LBAimg->Canvas, PaintBox2->Canvas);
       else if(StrPos(LowerCase(Path).c_str(),".bmp") || StrPos(LowerCase(Path).c_str(),".jpg") || StrPos(LowerCase(Path).c_str(),".jpeg") || StrPos(LowerCase(Path).c_str(),".gif"))
           ShowMessage("Isn't Possible for this type of file!");

       DrawLBAImage(LBAimg->Canvas);
       if(frm_fullscreen->Showing)
           DrawLBAImage(frm_fullscreen->Image1->Canvas);
   }
   else
       ShowMessage("You don't have the path to load the palette.\n Please choose the Select New Palette option!");
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::SelectLBA2Palette1Click(TObject *Sender)
{
   loadPaletteFromResource(2);
   DrawPalette(PaintBox2->Canvas);
   if(StrPos(LowerCase(Path).c_str(),".lim"))
      LoadImageFromFile(Path, LBAimg->Canvas, PaintBox2->Canvas, true);
   else if(StrPos(LowerCase(Path).c_str(),".hqr"))
      ShowMessage("Isn't Possible for this type of file!");
   else if(StrPos(LowerCase(Path).c_str(),".brk") || StrPos(LowerCase(Path).c_str(),".lsp"))
      LoadBrickFromFile(Path, LBAimg->Canvas, PaintBox2->Canvas);
   else if(StrPos(LowerCase(Path).c_str(),".lsr"))
      LoadRawSpriteFromFile(Path, LBAimg->Canvas, PaintBox2->Canvas);
   else if(StrPos(LowerCase(Path).c_str(),".bmp") || StrPos(LowerCase(Path).c_str(),".jpg") || StrPos(LowerCase(Path).c_str(),".jpeg") || StrPos(LowerCase(Path).c_str(),".gif"))
      ShowMessage("Isn't Possible for this type of file!");

   DrawLBAImage(LBAimg->Canvas);
   if(frm_fullscreen->Showing)
      DrawLBAImage(frm_fullscreen->Image1->Canvas);
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::SelectNewPalette1Click(TObject *Sender)
{
   status_img->Panels->Items[2]->Text = "Info: Opening ...";
   status_img->Update();
   OpenDialog2->Title = "Select new palette for the current image";
   OpenDialog2->Filter = "LBA Palette (*.pal)|*.pal";
   OpenDialog2->InitialDir = ExtractFileDir(Path);
   if(OpenDialog2->Execute())
   {
       LBAimg->Canvas->FillRect(Rect(0,0,640,480));
       palPath = OpenDialog2->FileName;

       FILE* palHandle = openResource(palPath.c_str());
       palPtr = (unsigned char*)malloc(768);
       readResource(palHandle,(char*)palPtr,768);
       closeResource(palHandle);

       loadPaletteFromPtr(palPtr);
       DrawPalette(PaintBox2->Canvas);

       // IF RESOURCE FILE
       if(StrPos(LowerCase(Path).c_str(),".hqr")){
           edt_index->Value = 0;
           cb_autodraw->Enabled = True;
           status_img->Panels->Items[2]->Text = "Info: Loading High Quality Resource ...";
           status_img->Update();
           status_path->Panels->Items[0]->Text = "Path: " + Path;
           LBAimg->Canvas->FillRect(Rect(0,0,640,480));
           LoadImageFromResource(0, Path, LBAimg->Canvas, PaintBox2->Canvas, true);
           pl_bkg->Visible = False;
           edt_index->Enabled = True;
           status_img->Panels->Items[2]->Text = "Info: High Quality Resource succesfully opened!";
           menu_closeall->Enabled = True;
           closeall->Enabled = True;
           CopyImageToClipboard1->Enabled = True;
           PasteImageFromClipboard1->Enabled = True;
           SelectNewPalette1->Enabled = False;
           SelectLBA1Palette1->Enabled = False;
           SelectLBA2Palette1->Enabled = False;
           SelectPaletteFromPath1->Enabled = False;
           SelectLastPaletteLoaded1->Enabled = False;
       }
       // IF BRICK/SPRITE
       else if(StrPos(LowerCase(Path).c_str(),".brk") || StrPos(LowerCase(Path).c_str(),".lsp")){
           edt_index->Value = 0;
           cb_autodraw->Enabled = False;
           status_img->Panels->Items[2]->Text = "Info: Opening ...";
           status_img->Update();
           LoadBrickFromFile(Path, LBAimg->Canvas, PaintBox2->Canvas);
           pl_bkg->Visible = False;
           status_img->Panels->Items[2]->Text = "Info: Image succesfully opened!";
           status_path->Panels->Items[0]->Text = "Path: " + Path;
           menu_closeall->Enabled = True;
           closeall->Enabled = True;
           CopyImageToClipboard1->Enabled = True;
           PasteImageFromClipboard1->Enabled = True;
           SelectNewPalette1->Enabled = False;
           SelectLBA1Palette1->Enabled = False;
           SelectLBA2Palette1->Enabled = False;
           SelectPaletteFromPath1->Enabled = False;
           SelectLastPaletteLoaded1->Enabled = False;
       }
       // IF RAW SPRITE
       else if(StrPos(LowerCase(Path).c_str(),".lsr")){
           edt_index->Value = 0;
           cb_autodraw->Enabled = False;
           status_img->Panels->Items[2]->Text = "Info: Opening ...";
           status_img->Update();
           LoadRawSpriteFromFile(Path, LBAimg->Canvas, PaintBox2->Canvas);
           pl_bkg->Visible = False;
           status_img->Panels->Items[2]->Text = "Info: Image succesfully opened!";
           status_path->Panels->Items[0]->Text = "Path: " + Path;
           menu_closeall->Enabled = True;
           closeall->Enabled = True;
           CopyImageToClipboard1->Enabled = True;
           PasteImageFromClipboard1->Enabled = True;
           SelectNewPalette1->Enabled = False;
           SelectLBA1Palette1->Enabled = False;
           SelectLBA2Palette1->Enabled = False;
           SelectPaletteFromPath1->Enabled = False;
           SelectLastPaletteLoaded1->Enabled = False;
       }
       // IF BMP
       else if(StrPos(LowerCase(Path).c_str(),".bmp")){
           Graphics::TBitmap *pBitmap2 = new Graphics::TBitmap();
           pBitmap2->LoadFromFile(Path);
           if(pBitmap2->PixelFormat == pf8bit){
               pBitmap->Assign(pBitmap2);
               pBitmap->FreeImage();
               DrawLBAImage(LBAimg->Canvas);
               loadPaletteFromBitmap();
               DrawPalette(PaintBox2->Canvas);
               LBAimg->Width = pBitmap->Width;
               LBAimg->Height = pBitmap->Height;
               AnsiString xis = "Image Size: ";
               xis = xis + LBAimg->Width;
               xis = xis + "x" + LBAimg->Height;
               frm_img->status_img->Panels->Items[1]->Text = xis;
               filesize = ResourceSize(Path.c_str());
               status_img->Panels->Items[0]->Text = "File Size: " + IntToStr(filesize);
               status_path->Panels->Items[0]->Text = "Path: " + Path;
               status_img->Panels->Items[2]->Text = "Info: Bitmap succesfully imported!";
               pl_bkg->Visible = False;
               menu_closeall->Enabled = True;
               closeall->Enabled = True;
               cb_autodraw->Enabled = False;
               CopyImageToClipboard1->Enabled = True;
               PasteImageFromClipboard1->Enabled = True;
               SelectNewPalette1->Enabled = False;
               SelectLBA1Palette1->Enabled = False;
               SelectLBA2Palette1->Enabled = False;
               SelectPaletteFromPath1->Enabled = False;
               SelectLastPaletteLoaded1->Enabled = False;
               delete pBitmap2;
           }
           else
               ShowMessage("In this version is just possible import 8bit Bitmaps!");
       }
       else if(StrPos(LowerCase(Path).c_str(),".jpg") || StrPos(LowerCase(Path).c_str(),".jpeg")){
           TJPEGImage *jpg = new TJPEGImage();
           jpg->PixelFormat = jf8Bit;
           jpg->LoadFromFile(Path);
           if(jpg->PixelFormat == jf8Bit){
               pBitmap->Assign(jpg);
               pBitmap->FreeImage();
               DrawLBAImage(LBAimg->Canvas);
               loadPaletteFromBitmap();
               DrawPalette(PaintBox2->Canvas);
               LBAimg->Width = pBitmap->Width;
               LBAimg->Height = pBitmap->Height;
               AnsiString xis = "Image Size: ";
               xis = xis + LBAimg->Width;
               xis = xis + "x" + LBAimg->Height;
               frm_img->status_img->Panels->Items[1]->Text = xis;
               filesize = ResourceSize(Path.c_str());
               status_img->Panels->Items[0]->Text = "File Size: " + IntToStr(filesize);
               status_path->Panels->Items[0]->Text = "Path: " + Path;
               status_img->Panels->Items[2]->Text = "Info: JPEG succesfully imported!";
               pl_bkg->Visible = False;
               menu_closeall->Enabled = True;
               closeall->Enabled = True;
               ExporttoLBAImagelim1->Enabled = False;
               cb_autodraw->Enabled = False;
               CopyImageToClipboard1->Enabled = True;
               PasteImageFromClipboard1->Enabled = True;
               SelectNewPalette1->Enabled = False;
               SelectLBA1Palette1->Enabled = False;
               SelectLBA2Palette1->Enabled = False;
               SelectPaletteFromPath1->Enabled = False;
               SelectLastPaletteLoaded1->Enabled = False;
               delete jpg;
           }
           else
               ShowMessage("In this version is just possible import 8bit JPEG!");
       }
       else if(StrPos(LowerCase(Path).c_str(),".lim")){
           pl_bkg->Visible = False;
           LoadImageFromFile(Path, palPath, LBAimg->Canvas, PaintBox2->Canvas);
           status_img->Panels->Items[2]->Text = "Info: Image succesfully opened!";
           status_path->Panels->Items[0]->Text = "Path: " + Path;
           status_palpath->Panels->Items[0]->Text = "Palette Path: " + palPath;
           menu_closeall->Enabled = True;
           closeall->Enabled = True;
           cb_autodraw->Enabled = False;
           CopyImageToClipboard1->Enabled = True;
           PasteImageFromClipboard1->Enabled = True;
           SelectNewPalette1->Enabled = True;
           SelectLBA1Palette1->Enabled = True;
           SelectLBA2Palette1->Enabled = True;
           SelectPaletteFromPath1->Enabled = True;
           SelectLastPaletteLoaded1->Enabled = True;
       }

       status_img->Panels->Items[2]->Text = "Info: Image succesfully selected!";
       status_palpath->Panels->Items[0]->Text = "Palette Path: " + palPath;
       DrawLBAImage(LBAimg->Canvas);
   }
   else
      status_img->Panels->Items[2]->Text = "Info: Selected paletted aborted by user ...";
}
//---------------------------------------------------------------------------


void __fastcall Tfrm_img::SelectLBA1Palette1Click(TObject *Sender)
{
   loadPaletteFromResource(1);
   DrawPalette(PaintBox2->Canvas);
   if(StrPos(LowerCase(Path).c_str(),".lim"))
      LoadImageFromFile(Path, LBAimg->Canvas, PaintBox2->Canvas, true);
   else if(StrPos(LowerCase(Path).c_str(),".hqr"))
      ShowMessage("Isn't Possible for this type of file!");
   else if(StrPos(LowerCase(Path).c_str(),".brk") || StrPos(LowerCase(Path).c_str(),".lsp"))
      LoadBrickFromFile(Path, LBAimg->Canvas, PaintBox2->Canvas);
   else if(StrPos(LowerCase(Path).c_str(),".lsr"))
      LoadRawSpriteFromFile(Path, LBAimg->Canvas, PaintBox2->Canvas);
   else if(StrPos(LowerCase(Path).c_str(),".bmp") || StrPos(LowerCase(Path).c_str(),".jpg") || StrPos(LowerCase(Path).c_str(),".jpeg") || StrPos(LowerCase(Path).c_str(),".gif"))
      ShowMessage("Isn't Possible for this type of file!");

   DrawLBAImage(LBAimg->Canvas);
   if(frm_fullscreen->Showing)
      DrawLBAImage(frm_fullscreen->Image1->Canvas);
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::ToolButton15Click(TObject *Sender)
{
   status_img->Panels->Items[2]->Text = "Info: Loading ...";
   status_img->Update();
   LoadImageFromResource(edt_index->Value, Path, LBAimg->Canvas, PaintBox2->Canvas, false);
   status_img->Panels->Items[2]->Text = "Info: Image sucesfully opened!";
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::Settings1Click(TObject *Sender)
{
   frm_settings->ShowSettings();
}
//---------------------------------------------------------------------------


void __fastcall Tfrm_img::LBAimgMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
   if(ScrollBox1->HorzScrollBar->IsScrollBarVisible() || ScrollBox1->VertScrollBar->IsScrollBarVisible())
       LBAimg->Cursor = crOpenHand;
   else
       LBAimg->Cursor = crDefault;

   if(Panning){
//       LBAimg->Cursor = crCloseHand;
       ScrollBox1->HorzScrollBar->Position = ScrollBox1->HorzScrollBar->Position - ( X - StartPoint.x);
       ScrollBox1->VertScrollBar->Position = ScrollBox1->VertScrollBar->Position - ( Y - StartPoint.y);
   }
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::LBAimgMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
//   LBAimg->Cursor = crCloseHand;
   Panning = true;
   StartPoint = Point(X,Y);
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::LBAimgMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   Panning = false;
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::ExportPaletteImagebmpjpggifpngpcx1Click(
      TObject *Sender)
{
   SaveDialog1->Title = "Export ...";
   SaveDialog1->Filter = "Bitmap (*.bmp)|*.bmp|JPEG Images (*.jpg, *.jpeg)|*.jpg;*.jpeg|GIF Image (*.gif)|*.gif|PNG Image (*.png)|*.png|PCX Image (*.pcx)|*.pcx";
   SaveDialog1->InitialDir = ExtractFileDir(Path);
   if(SaveDialog1->Execute()){
       // BMP
       if(SaveDialog1->FilterIndex == 1){
           SavePaletteToBitmap(SaveDialog1->FileName);
           status_img->Panels->Items[2]->Text = "Info: Image succesfully saved!";
       }
       // Jpeg
       else if(SaveDialog1->FilterIndex == 2){
           SavePaletteToJpeg(SaveDialog1->FileName);
           status_img->Panels->Items[2]->Text = "Info: Image succesfully saved!";
       }
       // GIF
       else if(SaveDialog1->FilterIndex == 3){
           SavePaletteToGIF(SaveDialog1->FileName);
           status_img->Panels->Items[2]->Text = "Info: Image succesfully saved!";
       }
       // PNG
       else if(SaveDialog1->FilterIndex == 4){
           SavePaletteToPNG(SaveDialog1->FileName);
           status_img->Panels->Items[2]->Text = "Info: Image succesfully saved!";
       }
       // PCX
       else if(SaveDialog1->FilterIndex == 5){
           SavePaletteToPCX(SaveDialog1->FileName);
           status_img->Panels->Items[2]->Text = "Info: Image succesfully saved!";
       }
   }else
       status_img->Panels->Items[2]->Text = "Info: Export aborted by user ...";
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::FullScreen1Click(TObject *Sender)
{
   if(LBAimg->Width!=105 && LBAimg->Height!=105 && !StrPos(LowerCase(Path).c_str(),".fla")){
      frm_fullscreen->Show();
   }
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::SelectLastPaletteLoaded1Click(TObject *Sender)
{
   AnsiString xis = ExtractFileDir(Application->ExeName)+"\\paltemp.dat";
   if(checkResource(xis.c_str())==1){
       LBAimg->Canvas->FillRect(Rect(0,0,640,480));
       LoadPaletteFromFile(xis, LBAimg->Canvas, PaintBox2->Canvas);
          DrawPalette(PaintBox2->Canvas);
       if(StrPos(LowerCase(Path).c_str(),".lim"))
           LoadImageFromFile(Path, LBAimg->Canvas, PaintBox2->Canvas, true);
       else if(StrPos(LowerCase(Path).c_str(),".hqr"))
           ShowMessage("Isn't Possible for this type of file!");
       else if(StrPos(LowerCase(Path).c_str(),".brk") || StrPos(LowerCase(Path).c_str(),".lsp"))
           LoadBrickFromFile(Path, LBAimg->Canvas, PaintBox2->Canvas);
       else if(StrPos(LowerCase(Path).c_str(),".lsr"))
           LoadRawSpriteFromFile(Path, LBAimg->Canvas, PaintBox2->Canvas);
       else if(StrPos(LowerCase(Path).c_str(),".bmp") || StrPos(LowerCase(Path).c_str(),".jpg") || StrPos(LowerCase(Path).c_str(),".jpeg") || StrPos(LowerCase(Path).c_str(),".gif"))
           ShowMessage("Isn't Possible for this type of file!");

       DrawLBAImage(LBAimg->Canvas);
       if(frm_fullscreen->Showing)
           DrawLBAImage(frm_fullscreen->Image1->Canvas);
   }
   else
       ShowMessage("The Last Palette used wasn't set yet!");
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::Timer1Timer(TObject *Sender)
{
   if(IsPlaying){
      Application->ProcessMessages();

    /*  loadPaletteFromPtr(FLA[frm_controlers->TrackBar1->Position].flaPalette);
      DrawPalette(frm_img->PaintBox2->Canvas);

      MakeLBAImage(FLA[frm_controlers->TrackBar1->Position].flaBuffer, 320,200);
      DrawLBAImage(frm_img->LBAimg->Canvas);  */

      if(frm_controlers->TrackBar1->Position == FLAheader.numOfFrames-1)
         Timer1->Enabled = False;
      else
         frm_controlers->TrackBar1->Position++;
   }
   else
      Timer1->Enabled = False;
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::LoadLBAFLAMoviesfla1Click(TObject *Sender)
{
   status_img->Panels->Items[2]->Text = "Info: Opening ...";
   status_img->Update();
   OpenDialog2->Title = "Open LBA FLA Movie";
   OpenDialog2->Filter = "LBA FLA Movie (*.fla)|*.fla";
   OpenDialog2->Options >> ofAllowMultiSelect;
   if(OpenDialog2->Execute())
   {
      menu_closeallClick(this);
      Path = OpenDialog2->FileName;
      status_img->Panels->Items[2]->Text = "Info: FLA succesfully opened!";
      status_path->Panels->Items[0]->Text = "Path: " + Path;
      edt_index->Value = 0;
      cb_autodraw->Enabled = False;
      pl_bkg->Visible = False;
      OpenFLA(Path.c_str(), LBAimg->Canvas);
      menu_closeall->Enabled = True;
      closeall->Enabled = True;
      cb_autodraw->Enabled = False;
      CopyImageToClipboard1->Enabled = True;
      PasteImageFromClipboard1->Enabled = True;
/*      SelectNewPalette1->Enabled = True;
      SelectLBA1Palette1->Enabled = True;
      SelectLBA2Palette1->Enabled = True;
      SelectPaletteFromPath1->Enabled = True;
      SelectLastPaletteLoaded1->Enabled = True;*/
   }
   else
      status_img->Panels->Items[2]->Text = "Info: Opened movie aborted by user ...";
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::FormShow(TObject *Sender)
{
   if(canOpen!=0)
   {
       canOpen = 0;    
       // IF RESOURCE FILE
       if(StrPos(LowerCase(Path).c_str(),".hqr") || StrPos(LowerCase(Path).c_str(),".ile")){
           edt_index->Value = 0;
           cb_autodraw->Enabled = True;
           status_img->Panels->Items[2]->Text = "Info: Loading High Quality Resource ...";
           status_img->Update();
           status_path->Panels->Items[0]->Text = "Path: " + Path;
           palPath = "";
           status_palpath->Panels->Items[0]->Text = "Palette Path: --[ not specified ]--";
           LBAimg->Canvas->FillRect(Rect(0,0,640,480));
           LoadImageFromResource(0, Path, LBAimg->Canvas, PaintBox2->Canvas, true);
           pl_bkg->Visible = False;
           edt_index->Enabled = True;
           status_img->Panels->Items[2]->Text = "Info: High Quality Resource succesfully opened!";
           menu_closeall->Enabled = True;
           closeall->Enabled = True;
           CopyImageToClipboard1->Enabled = True;
           PasteImageFromClipboard1->Enabled = True;
           SelectNewPalette1->Enabled = False;
           SelectLBA1Palette1->Enabled = False;
           SelectLBA1Palette1->Enabled = False;
           SelectPaletteFromPath1->Enabled = False;
       }
       // IF BRICK/SPRITE
       else if(StrPos(LowerCase(Path).c_str(),".brk") || StrPos(LowerCase(Path).c_str(),".lsp")){
           edt_index->Value = 0;
           cb_autodraw->Enabled = False;
           status_img->Panels->Items[2]->Text = "Info: Opening ...";
           status_img->Update();
           LoadBrickFromFile(Path, LBAimg->Canvas, PaintBox2->Canvas);
           pl_bkg->Visible = False;
           status_img->Panels->Items[2]->Text = "Info: Image succesfully opened!";
           status_path->Panels->Items[0]->Text = "Path: " + Path;
           palPath = "";
           status_palpath->Panels->Items[0]->Text = "Palette Path: --[ not specified ]--";
           menu_closeall->Enabled = True;
           closeall->Enabled = True;           
           CopyImageToClipboard1->Enabled = True;
           PasteImageFromClipboard1->Enabled = True;
           SelectNewPalette1->Enabled = False;
           SelectLBA1Palette1->Enabled = False;
           SelectLBA2Palette1->Enabled = False;
           SelectPaletteFromPath1->Enabled = False;
           SelectLastPaletteLoaded1->Enabled = False;
       }
       // IF RAW SPRITE
       else if(StrPos(LowerCase(Path).c_str(),".lsr")){
           edt_index->Value = 0;
           cb_autodraw->Enabled = False;
           status_img->Panels->Items[2]->Text = "Info: Opening ...";
           status_img->Update();
           LoadRawSpriteFromFile(Path, LBAimg->Canvas, PaintBox2->Canvas);
           pl_bkg->Visible = False;
           status_img->Panels->Items[2]->Text = "Info: Image succesfully opened!";
           status_path->Panels->Items[0]->Text = "Path: " + Path;
           palPath = "";
           status_palpath->Panels->Items[0]->Text = "Palette Path: --[ not specified ]--";
           menu_closeall->Enabled = True;
           closeall->Enabled = True;
           CopyImageToClipboard1->Enabled = True;
           PasteImageFromClipboard1->Enabled = True;
           SelectNewPalette1->Enabled = False;
           SelectLBA1Palette1->Enabled = False;
           SelectLBA2Palette1->Enabled = False;
           SelectPaletteFromPath1->Enabled = False;
           SelectLastPaletteLoaded1->Enabled = False;
       }
       else if(StrPos(LowerCase(Path).c_str(),".lba")){
           edt_index->Value = 0;
           cb_autodraw->Enabled = False;
           status_img->Panels->Items[2]->Text = "Info: Opening ...";
           status_img->Update();
           LoadLBASavedGameFromFile(Path, LBAimg->Canvas, PaintBox2->Canvas);
           pl_bkg->Visible = False;
           status_img->Panels->Items[2]->Text = "Info: Image succesfully opened!";
           status_path->Panels->Items[0]->Text = "Path: " + Path;
           menu_closeall->Enabled = True;
           closeall->Enabled = True;
           CopyImageToClipboard1->Enabled = True;
           PasteImageFromClipboard1->Enabled = True;
           SelectNewPalette1->Enabled = False;
           SelectLBA1Palette1->Enabled = False;
           SelectLBA2Palette1->Enabled = False;
           SelectPaletteFromPath1->Enabled = False;
           SelectLastPaletteLoaded1->Enabled = False;
       }
       // IF BMP
       else if(StrPos(LowerCase(Path).c_str(),".bmp")){
           edt_index->Value = 0;
           cb_autodraw->Enabled = False;
           Graphics::TBitmap *pBitmap2 = new Graphics::TBitmap();
           pBitmap2->LoadFromFile(Path);
           if(pBitmap2->PixelFormat == pf8bit){
               pBitmap->Assign(pBitmap2);
               pBitmap->FreeImage();
               DrawLBAImage(LBAimg->Canvas);
               loadPaletteFromBitmap();
               DrawPalette(PaintBox2->Canvas);
               LBAimg->Width = pBitmap->Width;
               LBAimg->Height = pBitmap->Height;
               AnsiString xis = "Image Size: ";
               xis = xis + LBAimg->Width;
               xis = xis + "x" + LBAimg->Height;
               frm_img->status_img->Panels->Items[1]->Text = xis;
               filesize = ResourceSize(Path.c_str());
               status_img->Panels->Items[0]->Text = "File Size: " + IntToStr(filesize);
               status_path->Panels->Items[0]->Text = "Path: " + Path;
               palPath = "";
               status_palpath->Panels->Items[0]->Text = "Palette Path: --[ not specified ]--";
               status_img->Panels->Items[2]->Text = "Info: Bitmap succesfully opened!";
               pl_bkg->Visible = False;
               menu_closeall->Enabled = True;
               closeall->Enabled = True;
               cb_autodraw->Enabled = False;
               CopyImageToClipboard1->Enabled = True;
               PasteImageFromClipboard1->Enabled = True;
               SelectNewPalette1->Enabled = False;
               SelectLBA1Palette1->Enabled = False;
               SelectLBA2Palette1->Enabled = False;
               SelectPaletteFromPath1->Enabled = False;
               SelectLastPaletteLoaded1->Enabled = False;               
               delete pBitmap2;
           }
           else
               ShowMessage("In this version is just possible import 8bit Bitmaps!");
       }
       else if(StrPos(LowerCase(Path).c_str(),".jpg") || StrPos(LowerCase(Path).c_str(),".jpeg")){
           edt_index->Value = 0;
           cb_autodraw->Enabled = False;
           TJPEGImage *jpg = new TJPEGImage();
           jpg->PixelFormat = jf8Bit;
           jpg->LoadFromFile(Path);
           if(jpg->PixelFormat == jf8Bit){
               pBitmap->Assign(jpg);
               loadPaletteFromBitmap();
               DrawPalette(PaintBox2->Canvas);
               pBitmap->FreeImage();
               DrawLBAImage(LBAimg->Canvas);
               LBAimg->Width = pBitmap->Width;
               LBAimg->Height = pBitmap->Height;
               AnsiString xis = "Image Size: ";
               xis = xis + LBAimg->Width;
               xis = xis + "x" + LBAimg->Height;
               frm_img->status_img->Panels->Items[1]->Text = xis;
               filesize = ResourceSize(Path.c_str());
               status_img->Panels->Items[0]->Text = "File Size: " + IntToStr(filesize);
               status_path->Panels->Items[0]->Text = "Path: " + Path;
               palPath = "";
               status_palpath->Panels->Items[0]->Text = "Palette Path: --[ not specified ]--";               
               status_img->Panels->Items[2]->Text = "Info: JPEG succesfully opened!";
               pl_bkg->Visible = False;
               menu_closeall->Enabled = True;
               closeall->Enabled = True;
               ExporttoLBAImagelim1->Enabled = False;
               cb_autodraw->Enabled = False;
               CopyImageToClipboard1->Enabled = True;
               PasteImageFromClipboard1->Enabled = True;
               SelectNewPalette1->Enabled = False;
               SelectLBA1Palette1->Enabled = False;
               SelectLBA2Palette1->Enabled = False;
               SelectPaletteFromPath1->Enabled = False;
               SelectLastPaletteLoaded1->Enabled = False;
               delete jpg;
           }
           else
               ShowMessage("In this version is just possible import 8bit JPEG!");  
       }
       // GIF Image
       else if(StrPos(LowerCase(Path).c_str(),".gif")){
           edt_index->Value = 0;
           cb_autodraw->Enabled = False;
           TGIFImage *gif = new TGIFImage();
           gif->LoadFromFile(Path);
           if(gif->Version == gv87a){
               pBitmap->Assign(gif);
               pBitmap->PixelFormat = pf8bit;               
               pBitmap->FreeImage();
               DrawLBAImage(LBAimg->Canvas);
               loadPaletteFromBitmap();
               DrawPalette(PaintBox2->Canvas);
               LBAimg->Width = pBitmap->Width;
               LBAimg->Height = pBitmap->Height;
               AnsiString xis = "Image Size: ";
               xis = xis + LBAimg->Width;
               xis = xis + "x" + LBAimg->Height;
               frm_img->status_img->Panels->Items[1]->Text = xis;
               filesize = ResourceSize(Path.c_str());
               status_img->Panels->Items[0]->Text = "File Size: " + IntToStr(filesize);
               status_path->Panels->Items[0]->Text = "Path: " + Path;
               palPath = "";
               status_palpath->Panels->Items[0]->Text = "Palette Path: --[ not specified ]--";
               status_img->Panels->Items[2]->Text = "Info: GIF succesfully opened!";
               pl_bkg->Visible = False;
               menu_closeall->Enabled = True;
               closeall->Enabled = True;               
               cb_autodraw->Enabled = False;
               CopyImageToClipboard1->Enabled = True;
               PasteImageFromClipboard1->Enabled = True;
           }
           delete gif;
       }
       //PNG Image
       else if(StrPos(LowerCase(Path).c_str(),".png")){
           edt_index->Value = 0;
           cb_autodraw->Enabled = False;
           TPNGObject *png = new TPNGObject();
           png->LoadFromFile(Path);
//           if(png->Version == gv87a){
               pBitmap->Assign(png);
               pBitmap->PixelFormat = pf8bit;               
               pBitmap->FreeImage();
               DrawLBAImage(LBAimg->Canvas);
               loadPaletteFromBitmap();
               DrawPalette(PaintBox2->Canvas);
               LBAimg->Width = pBitmap->Width;
               LBAimg->Height = pBitmap->Height;
               AnsiString xis = "Image Size: ";
               xis = xis + LBAimg->Width;
               xis = xis + "x" + LBAimg->Height;
               frm_img->status_img->Panels->Items[1]->Text = xis;
               filesize = ResourceSize(Path.c_str());
               status_img->Panels->Items[0]->Text = "File Size: " + IntToStr(filesize);
               status_path->Panels->Items[0]->Text = "Path: " + Path;
               palPath = "";
               status_palpath->Panels->Items[0]->Text = "Palette Path: --[ not specified ]--";
               status_img->Panels->Items[2]->Text = "Info: PNG succesfully opened!";
               pl_bkg->Visible = False;
               menu_closeall->Enabled = True;
               closeall->Enabled = True;               
               cb_autodraw->Enabled = False;
               CopyImageToClipboard1->Enabled = True;
               PasteImageFromClipboard1->Enabled = True;
  //         }
           delete png;
       }
       else if(StrPos(LowerCase(Path).c_str(),".pcx"))
       {
           TPCXBitmap *pcx = new TPCXBitmap();
           pcx->LoadFromFile(Path);
               pBitmap->Assign(pcx);
               pBitmap->FreeImage();
               DrawLBAImage(LBAimg->Canvas);
               loadPaletteFromBitmap();
               DrawPalette(PaintBox2->Canvas);
               LBAimg->Width = pBitmap->Width;
               LBAimg->Height = pBitmap->Height;
               AnsiString xis = "Image Size: ";
               xis = xis + LBAimg->Width;
               xis = xis + "x" + LBAimg->Height;
               frm_img->status_img->Panels->Items[1]->Text = xis;
               filesize = ResourceSize(Path.c_str());
               status_img->Panels->Items[0]->Text = "File Size: " + IntToStr(filesize);
               status_path->Panels->Items[0]->Text = "Path: " + Path;
               status_img->Panels->Items[2]->Text = "Info: PCX succesfully opened!";
               pl_bkg->Visible = False;
               menu_closeall->Enabled = True;
               closeall->Enabled = True;
               cb_autodraw->Enabled = False;
               CopyImageToClipboard1->Enabled = True;
               PasteImageFromClipboard1->Enabled = True;
               delete pcx;
       }
       else if(StrPos(LowerCase(Path).c_str(),".lim")){
           edt_index->Value = 0;
           cb_autodraw->Enabled = False;

           if(ParamCount()==1)
           {
               LoadImageFromFile(Path, LBAimg->Canvas, PaintBox2->Canvas, false);
           }
           else if(ParamCount()==2)
           {
               LoadImageFromFile(Path, palPath, LBAimg->Canvas, PaintBox2->Canvas);
           }

           status_img->Panels->Items[2]->Text = "Info: Image succesfully opened!";
           status_path->Panels->Items[0]->Text = "Path: " + Path;
           pl_bkg->Visible = False;
           menu_closeall->Enabled = True;
           closeall->Enabled = True;
           cb_autodraw->Enabled = False;
           CopyImageToClipboard1->Enabled = True;
           PasteImageFromClipboard1->Enabled = True;
           SelectNewPalette1->Enabled = True;
           SelectLBA1Palette1->Enabled = True;
           SelectLBA2Palette1->Enabled = True;
           SelectPaletteFromPath1->Enabled = True;
           SelectLastPaletteLoaded1->Enabled = True;
       }
       else if(StrPos(LowerCase(palPath).c_str(),".pal")){
           edt_index->Value = 0;
           cb_autodraw->Enabled = False;
           LoadPaletteFromFile(palPath, LBAimg->Canvas, PaintBox2->Canvas);
           status_palpath->Panels->Items[0]->Text = "Palette Path: " + palPath;
           status_img->Panels->Items[2]->Text = "Info: Palette succesfully opened!";
       }
       else if(StrPos(LowerCase(Path).c_str(),".fla")){
           status_img->Panels->Items[2]->Text = "Info: FLA succesfully opened!";
           status_path->Panels->Items[0]->Text = "Path: " + Path;
           edt_index->Value = 0;
           cb_autodraw->Enabled = False;
           OpenFLA(Path.c_str(), LBAimg->Canvas);
           pl_bkg->Visible = False;
           menu_closeall->Enabled = True;
           closeall->Enabled = True;
           cb_autodraw->Enabled = False;
           CopyImageToClipboard1->Enabled = True;
           PasteImageFromClipboard1->Enabled = True;
       }
       else
           ShowMessage("Invalid file type!");
   }

  // OLD CODE 
  /*if(StrPos(LowerCase(Path).c_str(),".fla")){
      FLAheader.numOfFrames = FLA.size();
      frm_controlers->TrackBar1->Max = FLA.size()-1;
      frm_img->pb_loading->Position = 0;
      frm_controlers->TrackBar1->Position = 0;
      frm_img->pb_loading->Max = FLAheader.numOfFrames-1;
      frm_controlers->Show();
      frm_controlers->lb_version->Caption = "Version: ";
      frm_controlers->lb_numframes->Caption = "Number of Frames: ";
      frm_controlers->lb_speed->Caption = "Speed: ";
      frm_controlers->lb_numsamples->Caption = "Number of Samples: ";
      frm_controlers->lb_version->Caption = frm_controlers->lb_version->Caption + FLAheader.version;
      frm_controlers->lb_numframes->Caption = frm_controlers->lb_numframes->Caption + FLAheader.numOfFrames;
      frm_controlers->lb_speed->Caption = frm_controlers->lb_speed->Caption + FLAheader.speed;
      frm_controlers->lb_numsamples->Caption = frm_controlers->lb_numsamples->Caption + samplesInFLA;
  } */
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::MenuItem2Click(TObject *Sender)
{
   SaveDialog1->Title = "Export Frames ...";
   SaveDialog1->Filter = "LBA Images (*.lim)|*.lim|Bitmap (*.bmp)|*.bmp|JPEG Images (*.jpg, *.jpeg)|*.jpg;*.jpeg|GIF Image (*.gif)|*.gif|PNG Image (*.png)|*.png|PCX Image (*.pcx)|*.pcx";
   SaveDialog1->InitialDir = ExtractFileDir(Path);
   if(SaveDialog1->Execute()){
       // LIM
      AnsiString xis = ExtractFileDir(SaveDialog1->FileName) + "\\" + ExtractFileName(SaveDialog1->FileName);
      pl_loading->Visible = True;
      lb_loading->Caption = "Exporting Frames...";
      lb_loading->Caption = "Exporting Frames...";
      pb_loading->Position = 0;
      for(int a=0; a < FLAheader.numOfFrames-1; a++)
      {
         pb_loading->Position++;
         MakeLBAImage(FLA[a].flaBuffer, 320,200);
         if(SaveDialog1->FilterIndex == 1){
//            savePtr = (unsigned char *)malloc(320*200);
//            SaveLBAImageToPtr(FLA[a].flaBuffer);
            FILE* resourceFile = openResourceAs((xis + "_" + a + ".lim").c_str());
            saveResource(resourceFile, FLA[a].flaBuffer,320*200);
            closeResource(resourceFile);

  //          savePtr2 = (unsigned char *)malloc(256*3);
  //          SaveLBAPalette(FLA[a].flaBuffer);
            FILE* resourceFile2 = openResourceAs((xis + "_" + a + ".pal").c_str());
            saveResource(resourceFile2, FLA[a].flaPalette,256*3);
            closeResource(resourceFile2);
         }
         // BMP
         else if(SaveDialog1->FilterIndex == 2){
            SaveLBAImageToBitmap(xis + "_" + a + ".bmp");
            status_img->Panels->Items[2]->Text = "Info: Image succesfully saved!";
         }
         // Jpeg
         else if(SaveDialog1->FilterIndex == 3){
            SaveLBAImageToJpeg(xis + "_" + a + ".jpg");
            status_img->Panels->Items[2]->Text = "Info: Image succesfully saved!";
         }
         // GIF
         else if(SaveDialog1->FilterIndex == 4){
            SaveLBAImageToGIF(xis + "_" + a + ".gif");
            status_img->Panels->Items[2]->Text = "Info: Image succesfully saved!";
         }
         // PNG
         else if(SaveDialog1->FilterIndex == 5){
            SaveLBAImageToPNG(xis + "_" + a + ".png");
            status_img->Panels->Items[2]->Text = "Info: Image succesfully saved!";
         }
         // PCX
         else if(SaveDialog1->FilterIndex == 6){
            SaveLBAImageToPCX(xis + "_" + a + ".pcx");
            status_img->Panels->Items[2]->Text = "Info: Image succesfully saved!";
         }
      }
      pl_loading->Visible = False;
      lb_loading->Caption = "Loading FLA Movie ...";
      lb_loading->Caption = "Loading FLA Movie ...";      
      pb_loading->Position = 0;
   }
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::Exportlimpalbmpjpg1Click(TObject *Sender)
{
   SaveDialog1->Title = "Export Current Frame...";
   SaveDialog1->Filter = "LBA Images (*.lim)|*.lim|LBA Palette (*.pal)|*.pal|Bitmap (*.bmp)|*.bmp|JPEG Images (*.jpg, *.jpeg)|*.jpg;*.jpeg|GIF Image (*.gif)|*.gif|PNG Image (*.png)|*.png|PCX Image (*.pcx)|*.pcx|Raw Palette (*.raw)|*.raw";
   SaveDialog1->InitialDir = ExtractFileDir(Path);
   if(SaveDialog1->Execute()){
       // LIM
       if(SaveDialog1->FilterIndex == 1){
           if(!StrPos(LowerCase(SaveDialog1->FileName).c_str(),".lim"))
               SaveDialog1->FileName = SaveDialog1->FileName + ".lim";
           getFileSize(LBAimg->Width);
           FILE* resourceFile = openResourceAs(SaveDialog1->FileName.c_str());
           saveResource(resourceFile, FLA[frm_controlers->TrackBar1->Position].flaBuffer,filesize);
           closeResource(resourceFile);
           status_img->Panels->Items[2]->Text = "Info: Image succesfully exported!";
       }
       // PAL
       else if(SaveDialog1->FilterIndex == 2){
           if(!StrPos(LowerCase(SaveDialog1->FileName).c_str(),".pal"))
               SaveDialog1->FileName = SaveDialog1->FileName + ".pal";
           FILE* resourceFile = openResourceAs(SaveDialog1->FileName.c_str());
           saveResource(resourceFile, FLA[frm_controlers->TrackBar1->Position].flaPalette,768);
           closeResource(resourceFile);
           status_img->Panels->Items[2]->Text = "Info: Palette succesfully saved!";
       }
       // BMP
       else if(SaveDialog1->FilterIndex == 3){
           SaveLBAImageToBitmap(SaveDialog1->FileName);
           status_img->Panels->Items[2]->Text = "Info: Image succesfully saved!";
       }
       // Jpeg
       else if(SaveDialog1->FilterIndex == 4){
           SaveLBAImageToJpeg(SaveDialog1->FileName);
           status_img->Panels->Items[2]->Text = "Info: Image succesfully saved!";
       }
       // GIF
       else if(SaveDialog1->FilterIndex == 5){
           SaveLBAImageToGIF(SaveDialog1->FileName);
           status_img->Panels->Items[2]->Text = "Info: Image succesfully saved!";
       }
       // PNG
       else if(SaveDialog1->FilterIndex == 6){
           SaveLBAImageToPNG(SaveDialog1->FileName);
           status_img->Panels->Items[2]->Text = "Info: Image succesfully saved!";
       }
       // PCX
       else if(SaveDialog1->FilterIndex == 7){
           SaveLBAImageToPCX(SaveDialog1->FileName);
           status_img->Panels->Items[2]->Text = "Info: Image succesfully saved!";
       }
       // Raw PAL
       else if(SaveDialog1->FilterIndex == 8){
           if(!StrPos(LowerCase(SaveDialog1->FileName).c_str(),".raw"))
               SaveDialog1->FileName = SaveDialog1->FileName + ".raw";
           FILE* resourceFile = openResourceAs(SaveDialog1->FileName.c_str());
           saveResource(resourceFile, FLA[frm_controlers->TrackBar1->Position].flaPalette,768);
           closeResource(resourceFile);
           status_img->Panels->Items[2]->Text = "Info: Palette succesfully saved!";
       }
   }else
       status_img->Panels->Items[2]->Text = "Info: Export aborted by user ...";
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_img::MenuItem1Click(TObject *Sender)
{
   SaveDialog1->Title = "Export Frames as Animated GIF ...";
   SaveDialog1->Filter = "Animated GIF (*.gif)|*.gif";
   SaveDialog1->InitialDir = ExtractFileDir(Path);

   if(SaveDialog1->Execute()){
      TGIFImage *gif = new TGIFImage();
      if(!StrPos(LowerCase(SaveDialog1->FileName).c_str(),".gif"))
       SaveDialog1->FileName += ".gif";
      pl_loading->Visible = True;
      lb_loading->Caption = "Exporting Frames...";
      lb_loading->Caption = "Exporting Frames...";
      pb_loading->Position = 0;
      gif->AnimationSpeed = FLAheader.speed;
      for(int a=0; a < FLAheader.numOfFrames-1; a++)
      {
         pb_loading->Position++;
         loadPaletteFromPtr(FLA[a].flaPalette);
         MakeLBAImage(FLA[a].flaBuffer, 320,200);
         gif->Add(pBitmap);
      }
      gif->Pack();
      gif->SaveToFile(SaveDialog1->FileName);
      delete gif;
      pl_loading->Visible = False;
      lb_loading->Caption = "Loading FLA Movie ...";
      lb_loading->Caption = "Loading FLA Movie ...";      
      pb_loading->Position = 0;
   }
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::Checknewversion1Click(TObject *Sender)
{
    AnsiString tmp = "http://sacredcarrot.fishos.cc/screenviewer.php?vnum=";
    tmp+=vnum;
    ShellExecute(Handle,"open",tmp.c_str(),NULL,NULL,SW_SHOWMAXIMIZED);
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::BugTrackingSystemBugTrack1Click(TObject *Sender)
{
    ShellExecute(Handle,"open","http://sacredcarrot.fishos.cc/mantis/project_issues.php?project_id=15;2",NULL,NULL,SW_SHOWMAXIMIZED);
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::TheSacredCarrotHomePage1Click(TObject *Sender)
{
    ShellExecute(Handle,"open","http://sacredcarrot.fishos.cc",NULL,NULL,SW_SHOWMAXIMIZED);    
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::ConverttoLBA1Palette1Click(TObject *Sender)
{
   LoadPaletteFromResourceWithoutUpdate(1);
   ConvertImagePalette();
   UpdatePalette();

   DrawPalette(PaintBox2->Canvas);

   DrawLBAImage(LBAimg->Canvas);
   if(frm_fullscreen->Showing)
      DrawLBAImage(frm_fullscreen->Image1->Canvas);
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::ConverttoLBA2Palette1Click(TObject *Sender)
{
   LoadPaletteFromResourceWithoutUpdate(2);
   ConvertImagePalette();
   UpdatePalette();

   DrawPalette(PaintBox2->Canvas);

   DrawLBAImage(LBAimg->Canvas);
   if(frm_fullscreen->Showing)
      DrawLBAImage(frm_fullscreen->Image1->Canvas);
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::ConverttoOutputPalette1Click(TObject *Sender)
{
   status_img->Panels->Items[2]->Text = "Info: Opening ...";
   status_img->Update();
   OpenDialog2->Title = "Convert image palette to output palette";
   OpenDialog2->Filter = "LBA Palette (*.pal)|*.pal";
   OpenDialog2->InitialDir = ExtractFileDir(Path);
   if(OpenDialog2->Execute())
   {
       //LBAimg->Canvas->FillRect(Rect(0,0,640,480));
       palPath = OpenDialog2->FileName;

       FILE* palHandle = openResource(palPath.c_str());
       palPtr = (unsigned char*)malloc(768);
       readResource(palHandle,(char*)palPtr,768);
       closeResource(palHandle);

       LoadPaletteFromPtrWithoutUpdate(palPtr);
       ConvertImagePalette();
       UpdatePalette();

       DrawPalette(PaintBox2->Canvas);

       DrawLBAImage(LBAimg->Canvas);
       if(frm_fullscreen->Showing)
           DrawLBAImage(frm_fullscreen->Image1->Canvas);
   }
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::TESTE1Click(TObject *Sender)
{
   SaveSpriteImageToPtr("xis",LBAimg->Canvas,PaintBox2->Canvas);
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::ConvertBMPtoFLAMovies1Click(TObject *Sender)
{
   frm_BMP2FLA->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::bt_drawClick(TObject *Sender)
{
   if(cb_autodraw->Checked != True){
       LBAimg->Canvas->FillRect(Rect(0,0,640,480));
       status_img->Panels->Items[2]->Text = "Info: Loading ...";
       status_img->Update();
       LoadImageFromResource(edt_index->Value, Path, LBAimg->Canvas, PaintBox2->Canvas, false);
       status_img->Panels->Items[2]->Text = "Info: Image sucesfully opened!";
   }   
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_img::PaintBox2MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
   int idx = GetIdxOfNearstColor(PaintBox2->Canvas->Pixels[X][Y]);

   pb_color->Canvas->Brush->Color = PaintBox2->Canvas->Pixels[X][Y];
   pb_color->Canvas->FillRect(TRect(0,0,116,22));
   pb_color->Canvas->Brush->Color = clBlack;
   pb_color->Canvas->FrameRect(TRect(0,0,116,22));

   lb_red->Caption = TPalette[idx].Red;
   pb_red->Canvas->Brush->Color = TPalette[idx].Red;
   pb_red->Canvas->FillRect(TRect(0,0,22,22));
   pb_red->Canvas->Brush->Color = clBlack;
   pb_red->Canvas->FrameRect(TRect(0,0,22,22));

   lb_green->Caption = TPalette[idx].Green;
   pb_green->Canvas->Brush->Color = TPalette[idx].Green*256;
   pb_green->Canvas->FillRect(TRect(0,0,22,22));
   pb_green->Canvas->Brush->Color = clBlack;
   pb_green->Canvas->FrameRect(TRect(0,0,22,22));

   lb_blue->Caption = TPalette[idx].Blue;
   pb_blue->Canvas->Brush->Color = TPalette[idx].Blue*256*256;
   pb_blue->Canvas->FillRect(TRect(0,0,22,22));
   pb_blue->Canvas->Brush->Color = clBlack;
   pb_blue->Canvas->FrameRect(TRect(0,0,22,22));
}
//---------------------------------------------------------------------------

