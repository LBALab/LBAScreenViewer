/*
------------------------[ LBA Screen Viewer Source ]-------------------------
Copyright (C) 2004
--------------------------------[ ImgLib.h ]---------------------------------

Author: Alexandre Fontoura [alexfont]
Begin : Sun Sep 12 2004
Email : alexandrefontoura@oninetspeed.pt

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

#ifndef ImgLibH
#define ImgLibH
//---------------------------------------------------------------------------

// Palette -----------------------------------------

struct Pal{
    Byte Red;
    Byte Green;
    Byte Blue;
    TColor RGB;
};

void loadPaletteFromResource(int value);
void loadPaletteFromResource(bool spraw, int value);
void LoadPaletteFromResourceWithoutUpdate(int value);
void UpdatePalette();
void loadPaletteFromPtr(unsigned char* palPtr);
void LoadPaletteFromPtrWithoutUpdate(unsigned char* palPtr);
void loadPaletteFromBitmap();
Pal* GetPaletteFromBitmap();
void DrawPalette(TCanvas *Canvas);
void resetPal();
int colourIndex(int colour);
int colourIndex(TColor colour);

// Image -----------------------------------------
void MakeLBAImage(bool jump, unsigned char* imgPtr);
void MakeLBAImage(unsigned char* imgPtr, int width, int height);
void DrawLBAImage(TCanvas *Canvas);
void LoadImageFromFile(AnsiString path, AnsiString pal, TCanvas *imgCanvas, TCanvas *palCanvas);
void LoadImageFromFile(AnsiString path, TCanvas *imgCanvas, TCanvas *palCanvas, bool file);
void LoadImageFromResource(int index, AnsiString path, TCanvas *imgCanvas, TCanvas *palCanvas, bool showmsg);

void LoadBrickFromFile(AnsiString path, TCanvas *imgCanvas, TCanvas *palCanvas);
void LoadRawSpriteFromFile(AnsiString path, TCanvas *imgCanvas, TCanvas *palCanvas);
void LoadLBASavedGameFromFile(AnsiString path, TCanvas *imgCanvas, TCanvas *palCanvas);

void LoadPaletteFromFile(AnsiString pal,TCanvas *imgCanvas, TCanvas *palCanvas);
void LoadFLAGIFFromPtr(unsigned char* ptr,int filesize);
void LoadFLAPCXFromPtr(unsigned char* ptr,int filesize);

void getImageSize(int filesize);
void getFileSize(int width);

void FreeImage();
void SaveLBAImageToPtr(unsigned char* savePtr);
void SaveLBAImageToBitmap(AnsiString savePath);
void SaveLBAImageToJpeg(AnsiString savePath);
void SaveLBAImageToGIF(AnsiString savePath);
void SaveLBAImageToPNG(AnsiString savePath);
void SaveLBAImageToPCX(AnsiString savePath);
void SaveLBAPalette(unsigned char* savepalPtr);
void SavePaletteToBitmap(AnsiString savePath);
void SavePaletteToJpeg(AnsiString savePath);
void SavePaletteToGIF(AnsiString savePath);
void SavePaletteToPNG(AnsiString savePath);
void SavePaletteToPCX(AnsiString savePath);

int getImgWidth();
int getImgHeight();
void CopyFirstBytes(unsigned char * ptr, unsigned char * aux);
void CopySavedGameImg(unsigned char * ptr, unsigned char * tmp);

int SubColors(TColor c1, TColor c2);
int GetIdxOfNearstColor(TColor c);
void ConvertImagePalette();

int Smaller(int a, int b);
int Larger(int a, int b);
unsigned char PixelType(int OffX, int MaxX, int X, int Y);
void SaveSpriteImageToPtr(AnsiString path, TCanvas *imgCanvas, TCanvas *palCanvas);

#endif
