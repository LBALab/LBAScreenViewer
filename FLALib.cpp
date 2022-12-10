/*
------------------------[ LBA Screen Viewer Source ]-------------------------
Copyright (C) 2004-2007
-------------------------------[ FLALiv.cpp ]--------------------------------

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
#include <sysutils.hpp>
#include <io.h>
#include <fcntl.h>
#pragma hdrstop

#include "FLALib.h"
#include "ImgLib.h"
#include "FullScreen.h"
#include "LBAScreenViewer_main.h"
#include "FLAControler.h"

TFLAheader FLAheader;
TFLAsample FLAsample;
TFrameData FrameData;
unsigned char * flaPtr;
extern unsigned char * palPtr;
int sampleIdx;
int samplesInFLA;
int flaSampleTable[100];
unsigned char flaBuffer[320*200];
unsigned char palBuffer[256*3];
char * workVideoBufferCopy;
int runFLAscriptVar0;
//int lastNumOfColor;
//int lastStartColor;
extern Graphics::TBitmap *pBitmap;
extern bool IsPlaying;
int frameindex=0;
vector<TFLA> FLA;
//---------------------------------------------------------------------------

#pragma package(smart_init)

void OpenFLA(char* fileName, TCanvas *imgCanvas)
{
   int quit = 0; //quitFla
   int currentFrame;
   int handle;
   int filesize;
   int count=1;
   FILE * flaHandle;

   FLA.resize(0);
   //FLA.allocation_size(0);
   FLA.clear();

   handle = open(fileName, O_RDONLY);
   filesize = filelength(handle);
   frm_img->status_img->Panels->Items[0]->Text = "File Size: " + IntToStr(filesize);

   flaHandle = fopen(fileName,"rb");
   flaPtr = (unsigned char * )malloc(filesize);
   fread((char *)flaPtr,filesize,1,flaHandle);

   memcpy(FLAheader.version, flaPtr, 6); flaPtr+=6;
   FLAheader.numOfFrames = *((int *)flaPtr); flaPtr+=4;

 //  FLA.allocation_size(FLAheader.numOfFrames-1);
   frm_controlers->TrackBar1->Max = FLAheader.numOfFrames-1;
   frm_img->pb_loading->Position = 0;

   FLAheader.speed = *(flaPtr++);
   FLAheader.var1 = *(flaPtr++);
   FLAheader.xsize = *((short int *)flaPtr); flaPtr+=2;
   FLAheader.ysize = *((short int *)flaPtr); flaPtr+=2;

   samplesInFLA = *((int *)flaPtr); flaPtr+=4;
   samplesInFLA&=0xFFFF; // TODO: to remove

   for(int i=0; i<samplesInFLA; i++)
   {
       flaSampleTable[i] = *((short int *)flaPtr);
       flaPtr+=4; // <------------
   }

   frm_img->pl_loading->Visible = True;

   if(!strcmp(FLAheader.version,"V1.3"))
   {
       currentFrame=0;

       if(!quit)
       {
           do
           {
               if(currentFrame == FLAheader.numOfFrames-1)
                   quit=1;
               else {
              //     Sleep(60);
                   Application->ProcessMessages();
                   GetFLAData();
                   frm_img->pb_loading->Position++;
               }
               currentFrame++;
           }while(!quit);
       }
   }

   frm_controlers->Show();

   loadPaletteFromPtr(FLA[0].flaPalette);
   DrawPalette(frm_img->PaintBox2->Canvas);
   MakeLBAImage(FLA[0].flaBuffer, 320,200);
   DrawLBAImage(frm_img->LBAimg->Canvas);

   frm_controlers->TrackBar1->Position = 0;

   FLAheader.numOfFrames = FLA.size();
   frm_controlers->TrackBar1->Max = FLA.size()-1;

   frm_img->Timer1->Enabled = False;
   frm_img->pl_loading->Visible = False;
   IsPlaying = true;

   frm_controlers->lb_version->Caption = "Version: ";
   frm_controlers->lb_numframes->Caption = "Number of Frames: ";
   frm_controlers->lb_speed->Caption = "Speed: ";
   frm_controlers->lb_numsamples->Caption = "Number of Samples: ";
   frm_controlers->lb_version->Caption = frm_controlers->lb_version->Caption + FLAheader.version;
   frm_controlers->lb_numframes->Caption = frm_controlers->lb_numframes->Caption + FLAheader.numOfFrames;
   frm_controlers->lb_speed->Caption = frm_controlers->lb_speed->Caption + FLAheader.speed;
   frm_controlers->lb_numsamples->Caption = frm_controlers->lb_numsamples->Caption + samplesInFLA;
}

void OpenFLA1(char* fileName, TCanvas *imgCanvas)
{
   int quit = 0; //quitFla
   int currentFrame;
   int handle;
   int filesize;
   FILE * flaHandle;

   FLA.clear();
   //FLA.allocation_size(0);

   handle = open(fileName, O_RDONLY);
   filesize = filelength(handle);

   flaHandle = fopen(fileName,"rb");
   flaPtr = (unsigned char * )malloc(filesize);
   fread((char *)flaPtr,filesize,1,flaHandle);

   memcpy(FLAheader.version, flaPtr, 6); flaPtr+=6;
   FLAheader.numOfFrames = *((int *)flaPtr); flaPtr+=4;

//   FLA.allocation_size(FLAheader.numOfFrames-1);
   FLA.resize(FLAheader.numOfFrames-1);

   FLAheader.speed = *(flaPtr++);
   FLAheader.var1 = *(flaPtr++);
   FLAheader.xsize = *((short int *)flaPtr); flaPtr+=2;
   FLAheader.ysize = *((short int *)flaPtr); flaPtr+=2;

   samplesInFLA = *((int *)flaPtr); flaPtr+=4;
   samplesInFLA&=0xFFFF; // TODO: to remove

   for(int i=0; i<samplesInFLA; i++)
   {
       flaSampleTable[i] = *((short int *)flaPtr);
       flaPtr+=4; // <------------
   }

   if(!strcmp(FLAheader.version,"V1.3"))
   {
       currentFrame=0;

       if(!quit)
       {
           do
           {
               if(currentFrame == FLAheader.numOfFrames-1)
                   quit=1;
               else {
              //     Sleep(60);
                   Application->ProcessMessages();
                   GetFLAData();
                   frm_img->pb_loading->Position++;
               }
               currentFrame++;
           }while(!quit);
       }
   }

   loadPaletteFromPtr(FLA[0].flaPalette);
   DrawPalette(frm_img->PaintBox2->Canvas);
   MakeLBAImage(FLA[0].flaBuffer, 320,200);
   DrawLBAImage(frm_img->LBAimg->Canvas);

   frm_img->Timer1->Enabled = False;
   IsPlaying = true;
}

void GetFLAData()
{
   char * ptr;
   unsigned int currentOpcodeGlob;
   unsigned char currentOpcode;
   int aux = 0;

   palPtr = (char *)palBuffer;

   FrameData.videoSize = *(flaPtr++);
   FrameData.dummy = *(flaPtr++);
   FrameData.frameVar0 = *((int *)flaPtr); flaPtr+=4;

   runFLAscriptVar0 = FrameData.videoSize;

   workVideoBufferCopy = (char * )malloc(FrameData.frameVar0);

   memcpy(workVideoBufferCopy, flaPtr, FrameData.frameVar0); flaPtr+=FrameData.frameVar0;

   if(aux >= runFLAscriptVar0)
       return;

   ptr = (char * )workVideoBufferCopy;


   do
   {
       currentOpcode = *((unsigned char*)ptr); ptr+=2;
       currentOpcodeGlob = *((unsigned short int*)ptr); ptr+=2;

       //ShowMessage("Opcode: " + IntToStr(currentOpcode-1));

       switch(currentOpcode-1)
       {
           case 0: // load palette
           {
               short int numOfColor = *((short int*)ptr);
               short int startColor = *((short int*)(ptr+2));

             /*  ShowMessage(numOfColor);
               ShowMessage(startColor);      */

               memcpy((palPtr+(startColor*3)),(ptr+4),numOfColor*3);
             /*  loadPaletteFromPtr(palPtr);
               DrawPalette(frm_img->PaintBox2->Canvas);  */

               break;
           }
           case 1:
           {
             //  ShowMessage("Fade/music opcode in fla\n");
               break;
           }
           case 2: // play sample
           {
               TFLAsample FLAsample;
               memcpy(&FLAsample,ptr,sizeof(TFLAsample));
   //            playSampleFla(header.sampleNum,header.freq,header.repeat,header.x,header.y);
   //            ShowMessage("Play Sample");

                sampleIdx = FLAsample.sampleNum;


               break;
           }
           case 4: // stop sample
           {
     //          ShowMessage("Stop Sample");
               break;
           }
           case 5: // draw delta frame
           {
      //         ShowMessage("Update Frame");
               DrawDeltaFrame(ptr,320);
               break;
           }
           case 7: // draw key frame
           {
//               ShowMessage("Draw Frame");
               DrawKeyFrame(ptr,320,200);
               break;
           }
           default:
           {
         //      ShowMessage("Unhandled fla opcode " + currentOpcode-1);
            //   exit(1);
               return;
           //    break;
           }
       }

       aux++;
       ptr += currentOpcodeGlob;

   }while(aux < runFLAscriptVar0);
}

void DrawKeyFrame(char * ptr, int width, int height)
{
   unsigned char * destPtr = (char *)flaBuffer;
   unsigned char * startOfLine = destPtr;
   char flag1;
   char flag2;

   do
   {
       flag1 = *(ptr++);

       for(int a=0; a < flag1; a++)
       {
           flag2 = *(ptr++);

           if(flag2 < 0)
           {
               flag2 =- flag2;
               for(int b=0; b < flag2; b++)
               {
                   *(destPtr++) = *(ptr++);
               }
           }
           else
           {
               char colorFill;

               colorFill = *(ptr++);

               for(int b=0; b < flag2; b++)
               {
                   *(destPtr++) = colorFill;
               }
           }
       }

       startOfLine = destPtr = startOfLine+width;
   }while(--height);


/*   memcpy(FLA[frameindex].flaBuffer,flaBuffer,320*200);
   memcpy(FLA[frameindex].flaPalette,palPtr,768);
   frameindex++;   */

   TFLA tmp;
   memcpy(tmp.flaBuffer,flaBuffer,320*200);
   memcpy(tmp.flaPalette,palPtr,768);
   tmp.sampleIdx = sampleIdx;
   FLA.push_back(tmp);

   /* MakeLBAImage(flaBuffer, 320, 200);
    DrawLBAImage(frm_img->LBAimg->Canvas);       */
//    frm_fullscreen->Image1->Canvas->Draw(0,0,frm_img->LBAimg);
}

void DrawDeltaFrame(char * ptr, int width)
{
   unsigned short int skip;
   unsigned char * destPtr;
   unsigned char * startOfLine;
   int height;

   char flag1;
   char flag2;

   skip = *((unsigned short int*)ptr); ptr+=2;
   skip *= width;
   startOfLine = destPtr = (char *)flaBuffer+skip;
   height = *((signed short int*)ptr); ptr+=2;
   do
   {
       flag1 = *(ptr++);

       for(int a=0; a < flag1; a++)
       {
           destPtr += (unsigned char)*(ptr++);
           flag2 = *(ptr++);

           if(flag2>0)
           {
               for(int b=0; b < flag2; b++)
               {
                   *(destPtr++) = *(ptr++);
               }
           }
           else
           {
               char colorFill;
               flag2 =- flag2;

               colorFill = *(ptr++);

               for(int b=0; b < flag2; b++)
               {
                   *(destPtr++)=colorFill;
               }
           }
       }

       startOfLine=destPtr=startOfLine+width;
   }while(--height);

 /*  memcpy(FLA[frameindex].flaBuffer,flaBuffer,320*200);
   memcpy(FLA[frameindex].flaPalette,palPtr,256*3);
   frameindex++;       */

   TFLA tmp;
   memcpy(tmp.flaBuffer,flaBuffer,320*200);
   memcpy(tmp.flaPalette,palPtr,768);
   tmp.sampleIdx = sampleIdx;
   FLA.push_back(tmp);

  /* MakeLBAImage(flaBuffer, 320,200);
   DrawLBAImage(frm_img->LBAimg->Canvas);  */
//   frm_fullscreen->Image1->Canvas->Draw(0,0,frm_img->LBAimg);

}

void DrawFLAmovie()
{
   for(int a=0; a<FLAheader.numOfFrames; a++)
   {
      Application->ProcessMessages();

      loadPaletteFromPtr(FLA[a].flaPalette);
      DrawPalette(frm_img->PaintBox2->Canvas);

      MakeLBAImage(FLA[a].flaBuffer, 320,200);
      DrawLBAImage(frm_img->LBAimg->Canvas);
   }
}

