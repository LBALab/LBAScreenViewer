/*
------------------------[ LBA Screen Viewer Source ]-------------------------
Copyright (C) 2004-2007
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
#include <jpeg.hpp>
#pragma hdrstop

#include "ImgLib.h"
#include "HQRLib.h"
#include "RoomLib.h"
#include "resvalues.h"
#include "LBAScreenViewer_main.h"
#include "GIFImage.hpp"
#include "pngimage.hpp"
#include "PCXCtrl.hpp"
//#include "math.h"

extern Graphics::TBitmap *pBitmap;
extern Graphics::TBitmap *palpic;
extern AnsiString palPath;

Pal TPalette[256];
Pal TPalette2[256];

//typedef struct TPalette[256];

int filesize;
unsigned char* imgPtr;
unsigned char* palPtr;
int width;
int height;
bool isLBA2=false;

int nlenght;

//---------------------------------------------------------------------------

#pragma package(smart_init)

// -------  PALETTE  --------------------------------------------------------

void loadPaletteFromResource(int value)
{
   Byte r,g,b;
   TResourceStream *res;
   LOGPALETTE* SysPal = (LOGPALETTE*)new Byte[sizeof(LOGPALETTE) + (256 - 1) * sizeof(PALETTEENTRY)];
    SysPal->palVersion = 0x300;
    SysPal->palNumEntries = 256;

   if(value == 1)
       res = new TResourceStream(0,"LBA_1","LBA_PALETTE");
   else
       res = new TResourceStream(0,"LBA_2","LBA_PALETTE");

   res->Seek(0,soFromBeginning);
   for(int a=0; a < 256; a++){
       res->Read(&r,1);
       TPalette[a].Red = r;
       SysPal->palPalEntry[a].peRed = r;
       res->Read(&g,1);
       TPalette[a].Green = g;
       SysPal->palPalEntry[a].peGreen = g;
       res->Read(&b,1);
       TPalette[a].Blue = b;
       SysPal->palPalEntry[a].peBlue = b;
       TPalette[a].RGB = r+g*256+b*256*256;
       SysPal->palPalEntry[a].peFlags = PC_RESERVED;
   }
   res->Free();

   // transparency for spriteraw
   /*if(value!=1){
       SysPal->palPalEntry[0].peRed = 0xFF;
       SysPal->palPalEntry[0].peGreen = 0xFF;
       SysPal->palPalEntry[0].peBlue = 0xFF;
   }*/

   pBitmap->Palette = CreatePalette(SysPal);
   delete[] SysPal;
}

void loadPaletteFromResource(bool spraw, int value)
{
   Byte r,g,b;
   TResourceStream *res;
   LOGPALETTE* SysPal = (LOGPALETTE*)new Byte[sizeof(LOGPALETTE) + (256 - 1) * sizeof(PALETTEENTRY)];
    SysPal->palVersion = 0x300;
    SysPal->palNumEntries = 256;

   if(value == 1)
       res = new TResourceStream(0,"LBA_1","LBA_PALETTE");
   else
       res = new TResourceStream(0,"LBA_2","LBA_PALETTE");

   res->Seek(0,soFromBeginning);
   for(int a=0; a < 256; a++){
       res->Read(&r,1);
       TPalette[a].Red = r;
       SysPal->palPalEntry[a].peRed = r;
       res->Read(&g,1);
       TPalette[a].Green = g;
       SysPal->palPalEntry[a].peGreen = g;
       res->Read(&b,1);
       TPalette[a].Blue = b;
       SysPal->palPalEntry[a].peBlue = b;
       TPalette[a].RGB = r+g*256+b*256*256;
       SysPal->palPalEntry[a].peFlags = PC_RESERVED;
   }
   res->Free();

   // transparency for spriteraw
   if(spraw)
   {
       if(value==2){
           SysPal->palPalEntry[0].peRed = 0xFF;
           SysPal->palPalEntry[0].peGreen = 0xFF;
           SysPal->palPalEntry[0].peBlue = 0xFF;
       }
   }

   pBitmap->Palette = CreatePalette(SysPal);
   delete[] SysPal;
}

void LoadPaletteFromResourceWithoutUpdate(int value)
{
   Byte r,g,b;
   TResourceStream *res;

   if(value == 1)
       res = new TResourceStream(0,"LBA_1","LBA_PALETTE");
   else
       res = new TResourceStream(0,"LBA_2","LBA_PALETTE");

   res->Seek(0,soFromBeginning);
   for(int a=0; a < 256; a++){
       res->Read(&r,1);
       TPalette2[a].Red = TPalette[a].Red;
       TPalette[a].Red = r;
       res->Read(&g,1);
       TPalette2[a].Green = TPalette[a].Green;
       TPalette[a].Green = g;
       res->Read(&b,1);
       TPalette2[a].Blue = TPalette[a].Blue;
       TPalette[a].Blue = b;
       TPalette2[a].RGB = TPalette[a].RGB;
       TPalette[a].RGB = r+g*256+b*256*256;
   }
   res->Free();
}

void UpdatePalette()
{
   LOGPALETTE* SysPal = (LOGPALETTE*)new Byte[sizeof(LOGPALETTE) + (256 - 1) * sizeof(PALETTEENTRY)];
   SysPal->palVersion = 0x300;
   SysPal->palNumEntries = 256;

   for(int a=0; a < 256; a++){
       SysPal->palPalEntry[a].peRed = TPalette[a].Red;
       SysPal->palPalEntry[a].peGreen = TPalette[a].Green;
       SysPal->palPalEntry[a].peBlue = TPalette[a].Blue;
       SysPal->palPalEntry[a].peFlags = PC_RESERVED;
   }

   pBitmap->Palette = CreatePalette(SysPal);
   delete[] SysPal;
}

void loadPaletteFromPtr(unsigned char* palPtr)
{
   Byte r,g,b;
   LOGPALETTE* SysPal = (LOGPALETTE*)new Byte[sizeof(LOGPALETTE) + (256 - 1) * sizeof(PALETTEENTRY)];
   SysPal->palVersion = 0x300;
   SysPal->palNumEntries = 256;

   for(int a=0; a < 256; a++){
       r = *(palPtr++);
       TPalette[a].Red = r;
       SysPal->palPalEntry[a].peRed = r;
       g = *(palPtr++);
       TPalette[a].Green = g;
       SysPal->palPalEntry[a].peGreen = g;
       b = *(palPtr++);
       TPalette[a].Blue = b;
       SysPal->palPalEntry[a].peBlue = b;
       TPalette[a].RGB = r+g*256+b*256*256;
       SysPal->palPalEntry[a].peFlags = PC_RESERVED;       
   }
   pBitmap->Palette = CreatePalette(SysPal);
   delete[] SysPal;
}

void LoadPaletteFromPtrWithoutUpdate(unsigned char* palPtr)
{
   Byte r,g,b;

   for(int a=0; a < 256; a++){
       r = *(palPtr++);
       TPalette[a].Red = r;
       g = *(palPtr++);
       TPalette[a].Green = g;
       b = *(palPtr++);
       TPalette[a].Blue = b;
       TPalette[a].RGB = r+g*256+b*256*256;
   }
}

void loadPaletteFromBitmap()
{
   Byte r,g,b;
   LOGPALETTE* SysPal = (LOGPALETTE*)new Byte[sizeof(LOGPALETTE) + (256 - 1) * sizeof(PALETTEENTRY)];
   SysPal->palVersion = 0x300;
   SysPal->palNumEntries = 256;

   GetPaletteEntries(pBitmap->Palette,0,256,SysPal->palPalEntry);

   for(int a=0; a < 256; a++){
       r = (Byte)SysPal->palPalEntry[a].peRed;
       TPalette[a].Red = r;
       g = SysPal->palPalEntry[a].peGreen;
       TPalette[a].Green = g;
       b = SysPal->palPalEntry[a].peBlue;
       TPalette[a].Blue = b;
       TPalette[a].RGB = r+g*256+b*256*256;
   }
   delete[] SysPal;
}

Pal* GetPaletteFromBitmap()
{
   Byte r,g,b;
   Pal pal[256];
   LOGPALETTE* SysPal = (LOGPALETTE*)new Byte[sizeof(LOGPALETTE) + (256 - 1) * sizeof(PALETTEENTRY)];
   SysPal->palVersion = 0x300;
   SysPal->palNumEntries = 256;

   GetPaletteEntries(pBitmap->Palette,0,256,SysPal->palPalEntry);

   for(int a=0; a < 256; a++){
       r = (Byte)SysPal->palPalEntry[a].peRed;
       pal[a].Red = r;
       g = SysPal->palPalEntry[a].peGreen;
       pal[a].Green = g;
       b = SysPal->palPalEntry[a].peBlue;
       pal[a].Blue = b;
       pal[a].RGB = r+g*256+b*256*256;
   }
   delete[] SysPal;
   return pal;
}
/*
void DrawPalette(TCanvas *Canvas)
{
   palpic->Width = 256;
   palpic->Height = 20;
   for(int a=0; a < 256; a++)
   {
       palpic->Canvas->MoveTo(a,0);
       palpic->Canvas->Pen->Color = TPalette[a].RGB;
       palpic->Canvas->LineTo(a,20);
   }
   palpic->Canvas->Brush->Color = clBlack;
   palpic->Canvas->FrameRect(Rect(0,0,256,20));
   palpic->Canvas->Brush->Color = clWhite;
   palpic->Canvas->FrameRect(Rect(1,1,255,19));
   palpic->Canvas->FrameRect(Rect(2,2,254,18));
   palpic->FreeImage();
   Canvas->Draw(0,2,palpic);
} */

void DrawPalette(TCanvas *Canvas)
{
   palpic->Width = 128;
   palpic->Height = 128;
   for(int b=0; b < 16; b++)
       for(int a=0; a < 16; a++)
       {
          palpic->Canvas->Brush->Color = TPalette[a+b*16].RGB;
          palpic->Canvas->FillRect(Rect(a*8+1,b*8+1,a*8+8,b*8+8));
       }
   /*palpic->Canvas->Brush->Color = clBlack;
   palpic->Canvas->FrameRect(Rect(0,0,256,20));
   palpic->Canvas->Brush->Color = clWhite;
   palpic->Canvas->FrameRect(Rect(1,1,255,19));
   palpic->Canvas->FrameRect(Rect(2,2,254,18));*/
   palpic->FreeImage();
   Canvas->Draw(0,2,palpic);
}

void resetPal()
{
   palpic->PixelFormat = pf24bit;
   palpic->Transparent = false;
   palpic->TransparentColor = clWhite;

   Byte b = 0xFF;
   for(int a=0; a < 256; a++)
   {
        TPalette[a].Red = b;
        TPalette[a].Green = b;
        TPalette[a].Blue = b;
        TPalette[a].RGB = (TColor)(b+b*256+b*256*256);
   }
}

int colourIndex(int colour)
{
   for(int a=0; a < 256; a++ )
       if(colour == TPalette[a].RGB)
           return a;
   return -1;
}

int colourIndex(TColor colour)
{
   for(int a=0; a < 256; a++ )
       if(colour == TPalette[a].RGB)
           return a;
   return -1;
}

// -------           --------------------------------------------------------

// -------  IMAGE  ----------------------------------------------------------

void MakeLBAImage(bool jump, unsigned char* imgPtr)
{
   pBitmap->Transparent = false;
   if(jump){
       imgPtr+=8;
       width = *(imgPtr++);
       height = *(imgPtr++);
       imgPtr+=2;
       pBitmap->Transparent = true;
   }
   pBitmap->Width = width;
   pBitmap->Height = height;
 //  pBitmap->PixelFormat = pf8bit;

   Byte *ptr;
   for(int a = 0; a < height; a++){
       ptr = (Byte *)pBitmap->ScanLine[a];
       memcpy(ptr, imgPtr+width*a, width);
   }

   frm_img->LBAimg->Width = width;
   frm_img->LBAimg->Height = height;
   AnsiString xis = "Image Size: ";
   xis = xis + width;
   xis = xis + "x" + height;
   frm_img->status_img->Panels->Items[1]->Text = xis;
   pBitmap->FreeImage();
}

void MakeLBAImage(unsigned char* imgPtr, int width, int height)
{
   pBitmap->Width = width;
   pBitmap->Height = height;
   pBitmap->Transparent = false;

   Byte *ptr;
   for(int a = 0; a < height; a++){
       ptr = (Byte *)pBitmap->ScanLine[a];
       memcpy(ptr, imgPtr+width*a, width);
   }

   frm_img->LBAimg->Width = width;
   frm_img->LBAimg->Height = height;
   AnsiString xis = "Image Size: ";
   xis = xis + width;
   xis = xis + "x" + height;
   frm_img->status_img->Panels->Items[1]->Text = xis;
   pBitmap->FreeImage();
}

void DrawLBAImage(TCanvas *Canvas)
{
   Canvas->Draw(0,0,pBitmap);
}

void LoadImageFromResource(int index, AnsiString path, TCanvas *imgCanvas, TCanvas *palCanvas, bool showmsg)
{
    free(imgPtr);
    if(StrPos(LowerCase(path).c_str(),"sprite")){
       pBitmap->TransparentColor = clWhite;
       pBitmap->Transparent = true;
       if(showmsg){
           /*int r = MessageDlg("It's an LBA2 image?",mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0);
           if(r==6)
               isLBA2 = true;
           else
               isLBA2 = false;   */

           int r = getNumEntries(path.c_str());
           if(r==118)
               isLBA2 = false;
           else if(r==425)
               isLBA2 = true;
       }
       if(StrPos(LowerCase(path).c_str(),"sprite")){
           if(isLBA2){
               if(index > 17)
                   index+=82;
           }
           filesize = loadResource(path.c_str(),index,&imgPtr);
  	        if(imgPtr)
           {
               if(isLBA2){
                   loadPaletteFromResource(2);
                   frm_img->edt_index->MaxValue=342;
               }else{
                   loadPaletteFromResource(1);
                   frm_img->edt_index->MaxValue=117;
               }
               DrawBrick(loadBrickPtr(imgPtr, true), Point(0,0));
               DrawLBAImage(imgCanvas);
               DrawPalette(palCanvas);
           }
       }
    }else if(StrPos(LowerCase(path).c_str(),"lba_brk") || StrPos(LowerCase(path).c_str(),"lba_bkg") || StrPos(LowerCase(path).c_str(),"spriraw")){
       pBitmap->Transparent = true;
       pBitmap->TransparentColor = clWhite;
       if(StrPos(LowerCase(path).c_str(),"lba_brk")){
           filesize = loadResource(path.c_str(),index,&imgPtr);
  	        if(imgPtr)
           {
               loadPaletteFromResource(1);
               DrawBrick(loadBrickPtr(imgPtr, false), Point(0,0));
               DrawLBAImage(imgCanvas);
               DrawPalette(palCanvas);
               frm_img->edt_index->MaxValue=8714;
           }
       }
       else if(StrPos(LowerCase(path).c_str(),"lba_bkg"))
       {
           filesize = loadResource(path.c_str(),index+197,&imgPtr);
  	        if(imgPtr)
           {
               loadPaletteFromResource(2);
               frm_img->edt_index->MaxValue=17902;
               DrawBrick(loadBrickPtr(imgPtr, false), Point(0,0));
               DrawLBAImage(imgCanvas);
               DrawPalette(palCanvas);
           }
       }
       else if(StrPos(LowerCase(path).c_str(),"spriraw")){ // LBA2 SpriteRaw
           // BEGIN TODO ---------------------------------
           if(index > 2){
               if(index > 30){
                   if(index > 61)
                       index+=56;
                   else
                       index+=19;
               }
               else
                   index++;

           }
           // END TODO ---------------------------------
           filesize = loadResource(path.c_str(),index,&imgPtr);
           if(imgPtr)
           {
               frm_img->edt_index->MaxValue = 110; //166 max
               loadPaletteFromResource(true,2);
               DrawPalette(palCanvas);
               MakeLBAImage(true, imgPtr);
               DrawLBAImage(imgCanvas);
               //free(imgPtr);
           }
       }
    }  // FLA_GIF
    else if(StrPos(LowerCase(path).c_str(),"fla_gif")){
       filesize = loadResource(path.c_str(),index+1,&imgPtr);
       frm_img->edt_index->MaxValue = 17;
       if(imgPtr)
       {
           LoadFLAGIFFromPtr(imgPtr,filesize);
           DrawPalette(palCanvas);
           DrawLBAImage(imgCanvas);
       }
    }
    else if(StrPos(LowerCase(path).c_str(),"fla_pcx")){
       filesize = loadResource(path.c_str(),index+1,&imgPtr);
       frm_img->edt_index->MaxValue = 17;
       if(imgPtr)
       {
           LoadFLAPCXFromPtr(imgPtr,filesize);
           DrawPalette(palCanvas);
           DrawLBAImage(imgCanvas);
       }
    }
    else{
       pBitmap->Transparent = false;

       if(StrPos(LowerCase(path).c_str(),"ress")){
           if(showmsg){
              /* int r = MessageDlg("It's an LBA2 image?",mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0);
               if(r==6)
                   isLBA2 = true;
               else
                   isLBA2 = false;*/

               int r = getNumEntries(path.c_str());
               if(r==54)
                   isLBA2 = false;
               else if(r==49)
                   isLBA2 = true;

           }
           if(!isLBA2){ // LBA1 ress
               filesize = loadResource(path.c_str(),ress1[index],&imgPtr);
               if(palress1[index] != -1 ){
                   loadResource(path.c_str(),palress1[index],&palPtr);
                   loadPaletteFromPtr(palPtr);
               }
               else{
                   loadPaletteFromResource(1);
               }
               frm_img->edt_index->MaxValue = 9;
           }
           else // LBA2 ress
           {
               filesize = loadResource(path.c_str(),ress2[index],&imgPtr);
               loadPaletteFromResource(2);
               frm_img->edt_index->MaxValue = 25;
           }
       }else if(StrPos(LowerCase(path).c_str(),".ile")){ // LBA2 Islands
           filesize = loadResource(path.c_str(),ile[index],&imgPtr);
           loadPaletteFromResource(2);
           frm_img->edt_index->MaxValue = 1;
       }else if(StrPos(LowerCase(path).c_str(),"holomap")){ // LBA2 Holomap
           filesize = loadResource(path.c_str(),holomap[index],&imgPtr);
           loadPaletteFromResource(2);
           frm_img->edt_index->MaxValue = 17;
       }
       else if(StrPos(LowerCase(path).c_str(),"screen")){ // LBA2 Screen images
           loadResource(path.c_str(),(screen[index]+1),&palPtr);
           loadPaletteFromPtr(palPtr);
           filesize = loadResource(path.c_str(),screen[index],&imgPtr);
           frm_img->edt_index->MaxValue = 38;
       }else if(StrPos(LowerCase(path).c_str(),"scrshot")){ // LBA2 screen shots
           loadResource(path.c_str(),(scrshot[index]+1),&palPtr);
           loadPaletteFromPtr(palPtr);
           filesize = loadResource(path.c_str(),scrshot[index],&imgPtr);
           frm_img->edt_index->MaxValue = 10;
       }

       DrawPalette(palCanvas);
       getImageSize(filesize);
       MakeLBAImage(false, imgPtr);
       DrawLBAImage(imgCanvas);
//       free(palPtr);
   }
   frm_img->status_img->Panels->Items[0]->Text = "File Size: " + IntToStr(filesize);
}

void LoadImageFromFile(AnsiString path, AnsiString pal,TCanvas *imgCanvas, TCanvas *palCanvas)
{
   pBitmap->Transparent = false;

   free(imgPtr);
   free(palPtr);
   filesize = ResourceSize(path.c_str());
   FILE* imgHandle = openResource(path.c_str());
   imgPtr = (unsigned char*)malloc(filesize);
   readResource(imgHandle,(char*)imgPtr,filesize);
   closeResource(imgHandle);

   FILE* palHandle = openResource(pal.c_str());
   palPtr = (unsigned char*)malloc(768);
   readResource(palHandle,(char*)palPtr,768);
   closeResource(palHandle);

   // last palette used
   AnsiString xis = ExtractFileDir(Application->ExeName)+"\\paltemp.dat";
   FILE* paltemp = fopen(xis.c_str(), "w+b");
   saveResource(paltemp,palPtr,768);
   closeResource(paltemp);

   loadPaletteFromPtr(palPtr);
   DrawPalette(palCanvas);

   getImageSize(filesize);
   MakeLBAImage(false, imgPtr);
   DrawLBAImage(imgCanvas);
   frm_img->status_img->Panels->Items[0]->Text = "File Size: " + IntToStr(filesize);
}

void LoadImageFromFile(AnsiString path, TCanvas *imgCanvas, TCanvas *palCanvas, bool file=false)
{
   pBitmap->Transparent = false;

   free(imgPtr);
   filesize = ResourceSize(path.c_str());
   FILE* imgHandle = openResource(path.c_str());
   imgPtr = (unsigned char*)malloc(filesize);
   readResource(imgHandle,(char*)imgPtr,filesize);
   closeResource(imgHandle);

   // load last file used if exist.
   if(!file){
      AnsiString xis = ExtractFileDir(Application->ExeName)+"\\paltemp.dat";
      if(checkResource(xis.c_str())==1)
      {
         FILE* paltemp = fopen(xis.c_str(), "r+b");
         palPtr = (unsigned char*)malloc(768);
         readResource(paltemp,(char*)palPtr,768);
         closeResource(paltemp);

         loadPaletteFromPtr(palPtr);
         DrawPalette(palCanvas);
      }
   }      

   getImageSize(filesize);
   MakeLBAImage(false, imgPtr);
   DrawPalette(palCanvas);
   frm_img->status_img->Panels->Items[0]->Text = "File Size: " + IntToStr(filesize);
}

void LoadBrickFromFile(AnsiString path, TCanvas *imgCanvas, TCanvas *palCanvas)
{
   pBitmap->Transparent = true;
   pBitmap->TransparentColor = pBitmap->Canvas->Pixels[0][0];

   int r = MessageDlg("It's an LBA2 image?",mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0);
   if(r==6)
       isLBA2 = true;
   else
       isLBA2 = false;

   free(imgPtr);
   filesize = ResourceSize(path.c_str());
   FILE* imgHandle = openResource(path.c_str());
   imgPtr = (unsigned char*)malloc(filesize);
   readResource(imgHandle,(char*)imgPtr,filesize);
   closeResource(imgHandle);

   if(isLBA2){
       loadPaletteFromResource(2);
   }else{
       loadPaletteFromResource(1);
   }
   bool isSprite = false;
   if(StrPos(LowerCase(path).c_str(),".lsp"))
       isSprite = true;

   DrawBrick(loadBrickPtr(imgPtr, isSprite), Point(0,0));

   DrawLBAImage(imgCanvas);
   DrawPalette(palCanvas);
   frm_img->status_img->Panels->Items[0]->Text = "File Size: " + IntToStr(filesize);
}

void LoadRawSpriteFromFile(AnsiString path, TCanvas *imgCanvas, TCanvas *palCanvas)
{
   pBitmap->Transparent = true;

   free(imgPtr);
   filesize = ResourceSize(path.c_str());
   FILE* imgHandle = openResource(path.c_str());
   imgPtr = (unsigned char*)malloc(filesize);
   readResource(imgHandle,(char*)imgPtr,filesize);
   closeResource(imgHandle);

   loadPaletteFromResource(true,2);//specially for transparancy
   MakeLBAImage(true, imgPtr);
   DrawLBAImage(imgCanvas);
   DrawPalette(palCanvas);
   frm_img->status_img->Panels->Items[0]->Text = "File Size: " + IntToStr(filesize);
}

void LoadLBASavedGameFromFile(AnsiString path, TCanvas *imgCanvas, TCanvas *palCanvas)
{
   unsigned char * tmp2;
   unsigned char * sgPtr;
   unsigned char * tmp;
   filesize = ResourceSize(path.c_str());
   FILE* imgHandle = openResource(path.c_str());
   tmp2 = (unsigned char*)malloc(filesize);
   readResource(imgHandle,(char*)tmp2,filesize);
   closeResource(imgHandle);

   if(*(tmp2)!= 0x03){    // LBA2
       if( *(tmp2) == 0xA4) // Compressed  164 = 0xA4
       {
         //  String name;
       /*    unsigned char * tmp;
           tmp = (unsigned char*)malloc(filesize);   */
           tmp = tmp2;

           tmp2+=5;

           while(*(tmp2++)!= 0x00)
           {
            //   name += *(tmp2++);
               nlenght++;
           }

           int RlSize = *((unsigned short *)tmp2); tmp2+=4;
           sgPtr = (unsigned char*)malloc(RlSize+6+nlenght);

           CopyFirstBytes(sgPtr, tmp);


           decompResource(RlSize,sgPtr+6+nlenght,tmp2,1);
           FILE* resourceFile = openResourceAs(path.c_str());
           saveResource(resourceFile, sgPtr, RlSize+6+nlenght);
           closeResource(resourceFile);
           free(tmp2);
           free(sgPtr);
       }
       // Already decompressed
//       ShowMessage("Decompressed File");

       // Open

        filesize = ResourceSize(path.c_str());
        FILE* imgHandle = openResource(path.c_str());
        tmp = (unsigned char*)malloc(filesize);
        readResource(imgHandle,(char*)tmp,filesize);
        closeResource(imgHandle);

        tmp+=5;

        while(*(tmp++)!= 0x00)
        {
            //   name += *(tmp2++);
        //    nlenght++;
        }

        imgPtr = (unsigned char*)malloc(19200);

        CopySavedGameImg(imgPtr,tmp);

        loadPaletteFromResource(2);
        DrawPalette(palCanvas);
        MakeLBAImage(imgPtr,160,120);
        DrawLBAImage(imgCanvas);

        frm_img->pl_bkg->Visible = False;
        frm_img->status_img->Panels->Items[2]->Text = "Info: Saved game image succesfully opened!";
        frm_img->status_path->Panels->Items[0]->Text = "Path: " + path;
        frm_img->menu_closeall->Enabled = True;
   }
   else
   {
       ShowMessage("This is a LBA1 saved game file that doesn't have raw images!");
       frm_img->status_img->Panels->Items[2]->Text = "Info: Unable to open the file!";
       frm_img->status_path->Panels->Items[0]->Text = "Path: " + path;
       frm_img->menu_closeall->Enabled = True;
   }
}

void LoadPaletteFromFile(AnsiString pal,TCanvas *imgCanvas, TCanvas *palCanvas)
{
   free(palPtr);
   FILE* palHandle = openResource(pal.c_str());
   palPtr = (unsigned char*)malloc(768);
   readResource(palHandle,(char*)palPtr,768);
   closeResource(palHandle);

   // last palette used
   AnsiString xis = ExtractFileDir(Application->ExeName)+"\\paltemp.dat";
   FILE* paltemp = fopen(xis.c_str(), "w+b");
   saveResource(paltemp,palPtr,768);
   closeResource(paltemp); 

   loadPaletteFromPtr(palPtr);
   DrawPalette(palCanvas);
   DrawLBAImage(imgCanvas);
}

void LoadFLAGIFFromPtr(unsigned char* ptr,int filesize)
{
   TMemoryStream *strgif = new TMemoryStream();
   TGIFImage *gif = new TGIFImage();
   strgif->Write(ptr,filesize);
   strgif->SaveToFile(ExtractFileDir(Application->ExeName)+"\\temp.dat");
   gif->LoadFromFile(ExtractFileDir(Application->ExeName)+"\\temp.dat");
   pBitmap->Assign(gif);
   loadPaletteFromBitmap();
   width = 640;
   height = 480;
   frm_img->LBAimg->Width = width;
   frm_img->LBAimg->Height = height;
   gif->Free();
   strgif->Free();
   pBitmap->FreeImage();
}

void LoadFLAPCXFromPtr(unsigned char* ptr,int filesize)
{
   TMemoryStream *strpcx = new TMemoryStream();
   TPCXBitmap *pcx = new TPCXBitmap();
   strpcx->Write(ptr,filesize);
   strpcx->SaveToFile(ExtractFileDir(Application->ExeName)+"\\temp.dat");
   pcx->LoadFromFile(ExtractFileDir(Application->ExeName)+"\\temp.dat");
   pBitmap->Assign(pcx);
   loadPaletteFromBitmap();
   width = 640;
   height = 480;
   frm_img->LBAimg->Width = width;
   frm_img->LBAimg->Height = height;
   pcx->Free();
   strpcx->Free();
   pBitmap->FreeImage();
}

void getImageSize(int filesize)
{
   switch(filesize)
   {
       case 307200: width = 640; height = 480; break;
       case 131884: width = 512; height = 256; break;
       case 320*200: width = 320; height = 200; break;
       case 65536: width = 256; height = 256; break;
       case 19200: width = 160; height = 120; break;
       default: width = 640; height = 480; break;
   }
}

void getFileSize(int width)
{
   switch(width)
   {
       case 640: filesize = 307200; break;
       case 512: filesize = 131884; break;
       case 320: filesize = 320*200; break;
       case 256: filesize = 65536; break;
       case 160: filesize = 19200; break;
   }
}

void FreeImage()
{
   pBitmap->ReleaseHandle();
   palpic->ReleaseHandle();
}

void SaveLBAImageToPtr(unsigned char* savePtr)
{
   Byte * ptr;
   for(int a = 0; a < height; a++){
       ptr = (Byte *) pBitmap->ScanLine[a];
       for(int b = 0; b < width; b++){
           *(savePtr++) = ptr[b];
       }
   }
}

void SaveLBAImageToBitmap(AnsiString savePath)
{
   if(!StrPos(LowerCase(savePath).c_str(),".bmp"))
       savePath += ".bmp";
   pBitmap->SaveToFile(savePath);
}

void SaveLBAImageToJpeg(AnsiString savePath)
{
   TJPEGImage *jpg = new TJPEGImage();
   if(!StrPos(LowerCase(savePath).c_str(),".jpg"))
       savePath += ".jpg";
   jpg->Assign(pBitmap);
   jpg->SaveToFile(savePath);
}

void SaveLBAImageToGIF(AnsiString savePath)
{
   TGIFImage *gif = new TGIFImage();
   if(!StrPos(LowerCase(savePath).c_str(),".gif"))
       savePath += ".gif";
   gif->Assign(pBitmap);
   gif->SaveToFile(savePath);
   delete gif;
}

void SaveLBAImageToPNG(AnsiString savePath)
{
   TPNGObject *png = new TPNGObject();
   if(!StrPos(LowerCase(savePath).c_str(),".png"))
       savePath += ".png";
   png->Assign(pBitmap);
   png->SaveToFile(savePath);
   delete png;
}

void SaveLBAImageToPCX(AnsiString savePath)
{
   TPCXBitmap *pcx = new TPCXBitmap();
   if(!StrPos(LowerCase(savePath).c_str(),".pcx"))
       savePath += ".pcx";
   pcx->Assign(pBitmap);
   pcx->SaveToFile(savePath);
   delete pcx;
}

void SaveLBAPalette(unsigned char* savepalPtr)
{
   for(int a = 0; a < 256; a++){
       *(savepalPtr++) = TPalette[a].Red;
       *(savepalPtr++) = TPalette[a].Green;
       *(savepalPtr++) = TPalette[a].Blue;
   }
}

void SavePaletteToBitmap(AnsiString savePath)
{
   if(!StrPos(LowerCase(savePath).c_str(),".bmp"))
       savePath += ".bmp";
   palpic->SaveToFile(savePath);
}

void SavePaletteToJpeg(AnsiString savePath)
{
   TJPEGImage *jpg = new TJPEGImage();
   if(!StrPos(LowerCase(savePath).c_str(),".jpg"))
       savePath += ".jpg";
   jpg->Assign(palpic);
   jpg->SaveToFile(savePath);
}

void SavePaletteToGIF(AnsiString savePath)
{
   TGIFImage *gif = new TGIFImage();
   if(!StrPos(LowerCase(savePath).c_str(),".gif"))
       savePath += ".gif";
   gif->Assign(palpic);
   gif->SaveToFile(savePath);
   delete gif;
}

void SavePaletteToPNG(AnsiString savePath)
{
   TPNGObject *png = new TPNGObject();
   if(!StrPos(LowerCase(savePath).c_str(),".png"))
       savePath += ".png";
   png->Assign(palpic);
   png->SaveToFile(savePath);
   delete png;
}

void SavePaletteToPCX(AnsiString savePath)
{
   TPCXBitmap *pcx = new TPCXBitmap();
   if(!StrPos(LowerCase(savePath).c_str(),".pcx"))
       savePath += ".pcx";
   pcx->Assign(palpic);
   pcx->SaveToFile(savePath);
   delete pcx;
}

int getImgWidth()
{
   return pBitmap->Width;
}

int getImgHeight()
{
   return pBitmap->Height;
}

void CopyFirstBytes(unsigned char * ptr, unsigned char * aux)
{
   *(ptr++) = (Byte)36; aux++; // Decompressed Identify flag
   *(ptr++) = *(aux++);
   *(ptr++) = *(aux++);
   *(ptr++) = *(aux++);

   for(int a =0; a <= nlenght; a++){
       *(ptr++) = *(aux++);
   }
}

void CopySavedGameImg(unsigned char * ptr, unsigned char * tmp)
{
    for(int a =0; a < 19200; a++){
        *(ptr++) = *(tmp++);
    }
}

int SubColors(TColor c1, TColor c2)
{
   return (abs((c1&0xFF)-(c2&0xFF)) + abs(((c1>>8)&0xFF)-((c2>>8)&0xFF)) + abs(((c1>>16)&0xFF)-((c2>>16)&0xFF)));
}

int GetIdxOfNearstColor(TColor c)
{
   int res, diff, mindiff;
   res = 0;
   mindiff = 2147483647;
   for(int a=0; a < 256; a++)
   {
       if(TPalette[a].RGB==c)
       {
           res = a;
           break;
       }
       else
       {
           diff = SubColors(c, TPalette[a].RGB);
           if(diff < mindiff)
           {
               mindiff = diff;
               res = a;
           }
       }
   }
   return res;
}

void ConvertImagePalette()
{
   /*frm_img->pl_loading->Visible = True;
   frm_img->pb_loading->Position = 0;
   frm_img->pl_loading->Caption = "Please Wait ...";

   int count=1;

   //TODO: make it faster
   for(int y=0; y < height; y++)
   {
       for(int x=0; x < width; x++)
       {
           pBitmap->Canvas->Pixels[x][y] = TPalette[GetIdxOfNearstColor(pBitmap->Canvas->Pixels[x][y])].RGB;

           if(((width*height)/100)*count == x*y)
           {
              frm_img->pb_loading->Position++;
              count++;
           }
       }
   }

   frm_img->pl_loading->Visible = False;
   */

   frm_img->pl_loading->Visible = True;
   frm_img->pb_loading->Position = 0;
   frm_img->pl_loading->Caption = "Please Wait ...";

   int count=1;

   Byte r,g,b;
   TResourceStream *res;
   LOGPALETTE* SysPal = (LOGPALETTE*)new Byte[sizeof(LOGPALETTE) + (256 - 1) * sizeof(PALETTEENTRY)];
   SysPal->palVersion = 0x300;
   SysPal->palNumEntries = 256;

   for(int a=0; a < 256; a++){
       int idx = GetIdxOfNearstColor(TPalette[a].RGB);

       SysPal->palPalEntry[a].peRed = TPalette[idx].Red;
       SysPal->palPalEntry[a].peGreen = TPalette[idx].Green;
       SysPal->palPalEntry[a].peBlue = TPalette[idx].Blue;
       SysPal->palPalEntry[a].peFlags = PC_RESERVED;

       if(((256)/100)*count == a)
       {
           frm_img->pb_loading->Position++;
           count++;
       }
   }

   pBitmap->Palette = CreatePalette(SysPal);
   delete[] SysPal;

   frm_img->pl_loading->Visible = False;
}

int Smaller(int a, int b)
{
   if(a < b)
       return a;

   return b;
}

int Larger(int a, int b)
{
   if(a > b)
       return a;

   return b;
}

//1-single-col, 2-single-col-change, 3-multi-col, 4-single-transp
unsigned char PixelType(int OffX, int MaxX, int X, int Y)
{
   unsigned char res;
   if((X < MaxX)&&(pBitmap->Canvas->Pixels[X][Y]==pBitmap->Canvas->Pixels[X+1][Y]))
   {
       if((X == OffX)||(pBitmap->Canvas->Pixels[X][Y]==pBitmap->Canvas->Pixels[X-1][Y]))
           res = 1;
       else
           res = 2;
   }
   else if((X > OffX)&&(pBitmap->Canvas->Pixels[X][Y]==pBitmap->Canvas->Pixels[X-1][Y]))
       res = 1;
   else if(pBitmap->Canvas->Pixels[X][Y]==0)
       res = 4;
   else
       res = 3;

   return res;
}

void SaveSpriteImageToPtr(AnsiString path, TCanvas *imgCanvas, TCanvas *palCanvas)
{
   int OffX, OffY, MaxX, MaxY, auxPix, auxBuff, auxPtr=0;
   unsigned char SLMode, NewMode;
   unsigned char PixCount, SLCount;
   unsigned char aux = 8;
   unsigned char *PixBuf;
   unsigned char *buff;
   unsigned char *ptr;

   PixBuf = (unsigned char *)malloc(sizeof(char)*1);
   buff = (unsigned char *)malloc(sizeof(char)*1);
   ptr = (unsigned char *)malloc(sizeof(char)*12);
   //*ptr = 0;

   *(ptr+auxPtr) = aux;
   auxPtr++;
   aux = 0;
   *(ptr+auxPtr) = aux;
   auxPtr++;
   *(ptr+auxPtr) = aux;
   auxPtr++;
   *(ptr+auxPtr) = aux;
   auxPtr++;
   *(ptr+auxPtr) = aux; // probably offset too
   auxPtr++;
   *(ptr+auxPtr) = aux; // probably offset too
   auxPtr++;
   *(ptr+auxPtr) = aux;
   auxPtr++;
   *(ptr+auxPtr) = aux;
   auxPtr++;
   *(ptr+auxPtr) = (unsigned char)pBitmap->Width;
   auxPtr++;
   *(ptr+auxPtr) = (unsigned char)pBitmap->Height;
   auxPtr++;
   *(ptr+auxPtr) = aux; // Offset X
   auxPtr++;
   *(ptr+auxPtr) = aux; // Offset Y
   auxPtr++;


   OffX = pBitmap->Width;
   OffY = pBitmap->Height;
   MaxX = 0;
   MaxY = 0;

  /* for(int b=0; b <= pBitmap->Height; b++)
       for(int a=0; a <= pBitmap->Width; a++)
           if(pBitmap->Canvas->Pixels[a][b]>0)
           {
               OffX = Smaller(OffX,a);
               OffY = Smaller(OffY,b);
               MaxX = Larger(MaxX,a);
               MaxY = Larger(MaxY,b);
           }
    */
    /*
    If MaxX-OffX<0 then Result:=#01#01#23#18#01#00
 else begin
  Result:=Char(MaxX-OffX+1)+Char(MaxY-OffY+1)+Char(OffX)+Char(OffY);
    */

   //for(int b=OffY; b < MaxY; b++)
   for(int b=0; b < pBitmap->Height; b++)
   {
       auxPix = 1;
       auxBuff = 1;
       PixCount = 0;
       SLCount = 0;
       SLMode = 0;
       PixBuf = (unsigned char *)realloc((unsigned char *)PixBuf,1);
       //*PixBuf = 0;
       buff = (unsigned char *)realloc((unsigned char *)buff,1);
       //*buff = 0;

       //SLMode = PixelType(OffX,MaxX,OffX,b);
       SLMode = PixelType(0,pBitmap->Width,0,b);

       //for(int a=OffX; a < MaxX; a++)
       for(int a=0; a < pBitmap->Width; a++)
       {
           if(a < MaxX)
               //NewMode = PixelType(OffX,MaxX,a+1,b);
               NewMode = PixelType(0,pBitmap->Width,a+1,b);
           else
               NewMode = 5;

           if(SLMode == 3)
           {
               PixBuf = (unsigned char *)realloc((unsigned char *)PixBuf,auxPix+1);
               *(PixBuf+(auxPix)) = (unsigned char)colourIndex(pBitmap->Canvas->Pixels[a][b]);
               auxPix++;
           }

           PixCount++;

           if(NewMode != SLMode)
           {
               switch(SLMode)
               {
                   case 1:
                       if(pBitmap->Canvas->Pixels[a][b]==0)
                       {
                           buff = (unsigned char *)realloc((unsigned char *)buff,auxBuff+1);
                           *(buff+(auxBuff-1)) = (unsigned char)((PixCount-1)&0x3F);
                           auxBuff++;
                       }
                       else
                       {
                           buff = (unsigned char *)realloc((unsigned char *)buff,auxBuff+2);
                           *(buff+(auxBuff)) = (unsigned char)(((PixCount-1)&0x3F)||0x80);
                           *(buff+(auxBuff+1)) = (unsigned char)colourIndex(pBitmap->Canvas->Pixels[a][b]);
                           auxBuff+=2;
                       }
                       break;
                   case 3:
                       {
                           buff = (unsigned char *)realloc((unsigned char *)buff,auxBuff+1+auxPix);
                           *(buff+(auxBuff)) = (((PixCount-1)&0x3F)||0x40);
                           for(int i=0; i< auxPix; i++)
                               *(buff+(auxBuff+1+i)) = (unsigned char)*(PixBuf+i);

                           auxBuff+=auxPix+1;
                       }
                       break;
                   case 4:
                       {
                           buff = (unsigned char *)realloc((unsigned char *)buff,auxBuff+1);
                           *(buff+(auxBuff)) = 0x00;
                           auxBuff++;
                       }
                       break;
               }
               SLCount++;
               PixCount = 0;
               auxPix = 0;

               if(NewMode == 2)
                   SLMode = 1;
               else
                   SLMode = NewMode;
           }
       }

       ptr = (unsigned char *)realloc((unsigned char *)ptr,auxPtr+auxBuff+1);

       *(ptr+auxPtr) = SLCount;
       for(int i=0; i<auxBuff; i++)
           *(ptr+auxPtr+i) = *(buff+i);
       //memcpy((ptr+(auxPtr+1)), buff, auxBuff);
       auxPtr += auxBuff+1;
   }

   AnsiString xis = ExtractFileDir(Application->ExeName)+"\\test.lsp";
   FILE* fstream = fopen(xis.c_str(), "w+b");
   saveResource(fstream,ptr,auxPtr);
   closeResource(fstream);

   DrawBrick(loadBrickPtr(ptr, true), Point(0,0));

   DrawLBAImage(imgCanvas);
   DrawPalette(palCanvas);
}
