/*
------------------------[ LBA Screen Viewer Source ]-------------------------
Copyright (C) 2004
-------------------------------[ RoomLib.cpp ]-------------------------------

Author: Alexandre Fontoura [xesf]
Begin : Thu Aug 26 2004

Original code in Delphi: Kazimierz Kr�l [Zink]

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
#pragma hdrstop

#include "RoomLib.h"
#include "HQRLib.h"
#include "ImgLib.h"
#include "LBAScreenViewer_main.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

extern Graphics::TBitmap *pBitmap;
extern struct Pal TPalette[256];

//---------------------------------------------------------------------------

int Smaller(int v1, int v2)
{
   return (v1<v2) ? v1 : v2;
}
//---------------------------------------------------------------------------

int Larger(int v1, int v2)
{
   return (v1>v2) ? v1 : v2;
}
//---------------------------------------------------------------------------

TBrick loadBrickPtr(unsigned char * ptr, bool isSprite)
{
   TBrick brick;
   int PixCount;
   if(isSprite)
      ptr+=8;

   brick.Width = *(ptr++);
   brick.Height = *(ptr++);
   brick.OffsetX = *(ptr++);
   brick.OffsetY = *(ptr++);

//   if(brick.OffsetY<0 || brick.OffsetX<0){
       brick.OffsetY = 0;
       brick.OffsetX = 0;
//   }

   brick.Lines.resize(brick.Height);
   for(int a=0; a < brick.Height; a++)
   {
       brick.Lines[a].SubCount = *(ptr++);
       brick.Lines[a].SubLines.resize(brick.Lines[a].SubCount);
       for(int b=0; b < brick.Lines[a].SubCount; b++)
       {
           brick.Lines[a].SubLines[b].Flags = *(ptr++);
           PixCount = (brick.Lines[a].SubLines[b].Flags & 0x3F) + 1;
           switch(brick.Lines[a].SubLines[b].Flags & 0xC0)
           {
               case 0x40:
               case 0xC0:
                   brick.Lines[a].SubLines[b].Colours.resize(PixCount);
                   for(int c=0; c < PixCount; c++)
                   {
                       brick.Lines[a].SubLines[b].Colours[c] = *(ptr++);
                   }
                   break;
               case 0x80:
                   brick.Lines[a].SubLines[b].Colours.resize(1);
                   brick.Lines[a].SubLines[b].Colours[0] = *(ptr++);
                   break;
           }
       }
   }
   return brick;
}
//---------------------------------------------------------------------------

void PaintBrickFromPrt(unsigned char * ptr, bool isSprite, TPoint pos)
{
   TBrick Brick;
   int PixCount, Xpos, Ypos;
   if(isSprite)
      ptr+=8;

   Brick.Width = *(ptr++);
   Brick.Height = *(ptr++);
   Brick.OffsetX = *(ptr++);
   Brick.OffsetY = *(ptr++);

   if(Brick.OffsetY<0 || Brick.OffsetX<0){
       Brick.OffsetY = 0;
       Brick.OffsetX = 0;
   }

   pBitmap->Canvas->FillRect(Rect(0,0,255,255));
   pBitmap->Width = Brick.Width;
   pBitmap->Height = Brick.Height;
   pBitmap->PixelFormat = pf8bit;

   pBitmap->Transparent = true;
   pBitmap->TransparentColor = clWhite;
   pBitmap->TransparentMode = tmFixed;

   Brick.Lines.resize(Brick.Height);
   Ypos = pos.y+Brick.OffsetY;
   for(int a=0; a < Brick.Height; a++)
   {
       Xpos = pos.x+Brick.OffsetX;
       Brick.Lines[a].SubCount = *(ptr++);
       Brick.Lines[a].SubLines.resize(Brick.Lines[a].SubCount);
       for(int b=0; b < Brick.Lines[a].SubCount; b++)
       {
           Brick.Lines[a].SubLines[b].Flags = *(ptr++);
           PixCount = (Brick.Lines[a].SubLines[b].Flags & 0x3F) + 1;

           if(Brick.Lines[a].SubLines[b].Flags & 0x40){
               Brick.Lines[a].SubLines[b].Colours.resize(PixCount);
               for(int c=0; c < PixCount; c++)
               {
                   Brick.Lines[a].SubLines[b].Colours[c] = *(ptr++);
                   pBitmap->Canvas->Pixels[Xpos+c][Ypos+a] = TPalette[Brick.Lines[a].SubLines[b].Colours[c]].RGB;
               }
               Xpos++;
           }
           else if(Brick.Lines[a].SubLines[b].Flags & 0x80){
               Brick.Lines[a].SubLines[b].Colours.resize(1);
               Brick.Lines[a].SubLines[b].Colours[0] = *(ptr++);
               for(int c=0; c < PixCount; c++)
               {
                   pBitmap->Canvas->Pixels[Xpos+c][Ypos+a] = TPalette[Brick.Lines[a].SubLines[b].Colours[0]].RGB;
               }
               Xpos += PixCount;
           }
           else
               Xpos += PixCount;
       }
       Ypos++;
   }
   frm_img->LBAimg->Width = Brick.Width;
   frm_img->LBAimg->Height = Brick.Height;
   AnsiString xis = "Image Size: ";
   xis = xis + Brick.Width;
   xis = xis + "x" + Brick.Height;
   frm_img->status_img->Panels->Items[1]->Text = xis;
   pBitmap->FreeImage();
}

//---------------------------------------------------------------------------

void DrawBrick(TBrick Brick, TPoint pos)
{
   pBitmap->Canvas->FillRect(Rect(0,0,255,255));
   pBitmap->Width = Brick.Width;
   pBitmap->Height = Brick.Height;
   pBitmap->PixelFormat = pf8bit;
   pBitmap->Transparent = true;
   pBitmap->TransparentColor = clWhite;
   pBitmap->TransparentMode = tmFixed;

   int PixCount, Xpos, Ypos;
   Ypos = pos.y+Brick.OffsetY;
   for(int a=0; a < Brick.Height; a++)
   {
       Xpos = pos.x+Brick.OffsetX;
       for(int b=0; b < Brick.Lines[a].SubCount; b++)
       {
           PixCount = (Brick.Lines[a].SubLines[b].Flags & 0x3F) + 1;
           switch(Brick.Lines[a].SubLines[b].Flags & 0xC0)
           {
               case 0x40:
               case 0xC0:
                           for(int c=0; c < PixCount; c++)
                           {
                               pBitmap->Canvas->Pixels[Xpos+c][Ypos+a] = TPalette[Brick.Lines[a].SubLines[b].Colours[c]].RGB;
                           }
                           break;
               case 0x80:  for(int c=0; c < PixCount; c++)
                           {
                               pBitmap->Canvas->Pixels[Xpos+c][Ypos+a] = TPalette[Brick.Lines[a].SubLines[b].Colours[0]].RGB;
                           }
                           break;
           }
           Xpos += PixCount;
       }
   }
   frm_img->LBAimg->Width = Brick.Width;
   frm_img->LBAimg->Height = Brick.Height;
   AnsiString xis = "Image Size: ";
   xis = xis + Brick.Width;
   xis = xis + "x" + Brick.Height;
   frm_img->status_img->Panels->Items[1]->Text = xis;
}
//---------------------------------------------------------------------------

Byte PixelOfBrick(TBrick Brick, int X, int Y)
{
   int a, b, c;
   Byte ret;
   X -= Brick.OffsetX;
   Y -= Brick.OffsetY;
   b = 0;
   for(int a=0; a < (signed)Brick.Lines[a].SubLines.size(); a++ )
   {
       c = (Brick.Lines[Y].SubLines[a].Flags & 0x1F)+1;
       if((b+c)>=X){
           if((Brick.Lines[Y].SubLines[a].Flags & 0x40) > 0)
               ret = Brick.Lines[Y].SubLines[a].Colours[X-b];
           else if((Brick.Lines[Y].SubLines[a].Flags & 0x80) > 0)
               ret = Brick.Lines[Y].SubLines[a].Colours[0];
       }
       break;
   }
   return ret;
}
//---------------------------------------------------------------------------

vector<TLayout> LoadLibraryPtr(unsigned char * ptr)
{
   vector<TLayout>Library;
   Library.resize(2);
   Library[0].XCount = 0;
   Library[0].YCount = 0;
   Library[0].ZCount = 0;
   Library[0].Map.resize(0);
   Library[1].Offset = *((long *)ptr); ptr+=4;
   Library.resize((Library[1].Offset / 4)+1);
   for(int a=2; a < (signed)Library.size(); a++)
       Library[a].Offset = *((long *)ptr); ptr+=4;

   for(int a=1; a < (signed)Library.size(); a++)
   {
       Library[a].XCount = *(ptr++);
       Library[a].YCount = *(ptr++);
       Library[a].ZCount = *(ptr++);
       Library[a].Map.resize(Library[a].XCount * Library[a].YCount * Library[a].ZCount);
       for(int b=0; b < (Library[a].XCount * Library[a].YCount * Library[a].ZCount); b++)
       {
           Library[a].Map[b].Barrier = *(ptr++);
           Library[a].Map[b].Sound = *(ptr++);
           Library[a].Map[b].Index = *((short *)ptr); ptr+=2;
       }
   }
   return Library;
}
//---------------------------------------------------------------------------

void PaintLibrary(vector<TLayout> TLibrary, int index, TCanvas *Canvas, unsigned char * brickPtr, char * path)
{
   int x=0,y=100;
   for(int b=0; b < (TLibrary[index].XCount * TLibrary[index].YCount * TLibrary[index].ZCount); b++)
   {
       loadResource(path,TLibrary[index].Map[b].Index,&brickPtr);
       DrawBrick(loadBrickPtr(brickPtr, false), Point(x,y));
       free(brickPtr);
       x += 48;
       y += 26;
   }
}
//---------------------------------------------------------------------------

TGrid LoadGridPtr(unsigned char * ptr)
{
   unsigned char * aux1 = ptr;
   unsigned char * aux2 = ptr;
   unsigned char * aux3 = ptr;
   int BlockCount;
   Byte d;
   TGrid Grid;
   Grid.Cells.resize(64*64);
   d = *(++aux1);
   Grid.LBA2 = d < 0x20;
   if(Grid.LBA2){
       Grid.LibIndex = *(aux2++);
       for(int a=0; a <= 32; a++)
           Grid.UnknownData[a] = *(aux2++);
       d = 34;
   }
   else
       d = 0;
   delete aux1,aux2;

   ptr += d;

   for(int a=0; a < 64*64; a++)
       Grid.Cells[a].Offset = *((short*)aux3); aux3+=2;

   delete aux3;

   for(int a=0; a < 64*64; a++){
       ptr += Grid.Cells[a].Offset+d;
       Grid.Cells[a].ObjCount = *(ptr++);
       Grid.Cells[a].Objects.resize(Grid.Cells[a].ObjCount);
       for(int b=0; b < Grid.Cells[a].Objects.size(); b++){
           Grid.Cells[a].Objects[b].Flags = *(ptr++);
           BlockCount = (Grid.Cells[a].Objects[b].Flags & 0x1F)+1;
           switch(Grid.Cells[a].Objects[b].Flags & 0xE0)
           {
               case 0x00: Grid.Cells[a].Objects[b].Blocks.resize(0); break;
               case 0x40:
                   {
                       Grid.Cells[a].Objects[b].Blocks.resize(BlockCount);
                       for(int c=0; c < BlockCount; c++)
                       {
                           Grid.Cells[a].Objects[b].Blocks[c].LayIdx = *(ptr++);
                           Grid.Cells[a].Objects[b].Blocks[c].BrkIdx = *(ptr++);
                       }
                       break;
                   }
               case 0x80:
                   {
                       Grid.Cells[a].Objects[b].Blocks.resize(1);
                       Grid.Cells[a].Objects[b].Blocks[0].LayIdx = *(ptr++);
                       Grid.Cells[a].Objects[b].Blocks[0].BrkIdx = *(ptr++);
                       break;
                   }
           }
       }
   }
   return Grid;
}
//---------------------------------------------------------------------------

TPoint GridToPos(int X, int Y, int Z, TPoint Start)
{
   return Point(X*24-Z*24+Start.x,Start.y+X*12+Z*12-Y*15);
}
//---------------------------------------------------------------------------

TPoint BrickAtPos(int X, int Y, int Layer)
{
   return Point((X / 48)+(Y / 24),((Y+Layer*15) / 24)-(X / 48));
}
//---------------------------------------------------------------------------
