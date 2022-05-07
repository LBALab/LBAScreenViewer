/*
------------------------[ LBA Screen Viewer Source ]-------------------------
Copyright (C) 2004
-------------------------------[ RoomLib.h ]---------------------------------

Author: Alexandre Fontoura [alexfont]
Begin : Thu Aug 26 2004
Email : alexandrefontoura@oninetspeed.pt

Original code in Delphi: Kazimierz Król [Zink]

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

#ifndef RoomLibH
#define RoomLibH

#include <stdio.h>
#include <vector>
#include <fstream.h>
//---------------------------------------------------------------------------

// ---- [ Brick ] ----

struct TSubLine{
   Byte Flags;
   vector<Byte> Colours;
};

struct TLine{
   Byte SubCount;
   vector <TSubLine>SubLines;
};

struct TBrick{
   Byte Width, Height;
   signed char OffsetX, OffsetY;
   vector <TLine>Lines;
};

//vector <TBrick>TBricks;

// ---- [ Layout ] ----

struct TElement{
   Byte Barrier, Sound;
   short Index;
};

struct TLayout{
   long Offset;
   Byte XCount, YCount, ZCount;
   vector <TElement>Map;
};

//vector <TLayout>TLibrary;

// ---- [ Grid ] ----

struct TBlockIndex{
   Byte LayIdx, BrkIdx;
};

struct TGridObj{
   Byte Flags;
   vector <TBlockIndex>Blocks;
};

struct TCell{
   long Offset;
   Byte ObjCount;
   vector <TGridObj>Objects;
};

struct TGrid{
   bool LBA2;
   long LibIndex;
   Byte UnknownData[32];
   vector <TCell>Cells;
};

int Smaller(int v1, int v2);
int Larger(int v1, int v2);

TBrick loadBrickPtr(unsigned char * ptr, bool isSprite);
void DrawBrick(TBrick brick, TPoint pos);
void PaintBrickFromPrt(unsigned char * ptr, bool isSprite, TPoint pos);
vector<TLayout> LoadLibraryPtr(unsigned char * ptr);
void PaintLibrary(vector<TLayout> TLibrary, int index, TCanvas *Canvas, unsigned char * brickPtr, char * path);

TPoint GridToPos(int X, int Y, int Z, TPoint Start);
TPoint BrickAtPos(int X, int Y, int Layer);

#endif
