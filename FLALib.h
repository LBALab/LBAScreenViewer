//---------------------------------------------------------------------------

#ifndef FLALibH
#define FLALibH
#include <vector>
#include <stdio.h>
#include <fstream.h>
#include <assert.h>
   
//---------------------------------------------------------------------------

struct TFLAsample
{
   short int sampleNum;
   short int freq;
   short int repeat;
   char dummy;
   unsigned char x;
   unsigned char y;
};

struct TFLAheader
{
   char version[6];
   int numOfFrames;
   Byte speed;
   char var1;
   short int xsize; //var2
   short int ysize; //var3
};

struct TFrameData
{
   char videoSize;
   char dummy;   
   int frameVar0;
};


struct TFLA
{
   unsigned char flaBuffer[320*200];
   unsigned char flaPalette[768];
   int sampleIdx;
};

void OpenFLA(char* fileName, TCanvas *imgCanvas);
void OpenFLA1(char* fileName, TCanvas *imgCanvas);
void GetFLAData();
void DrawKeyFrame(char * ptr, int width, int height);
void DrawDeltaFrame(char * ptr, int width);
void DrawFLAmovie();


/*
int useFlaPCX;
int flaTime;
short int flaVar2;
int numOfFrameInFLA;
char flaPalette[256*3];
char flaPaletteRGBA[256*4];
FLAheaderStruct flaHeaderData;
byte* workVideoBufferCopy;
FILE* dataFileHandle;
int flahVar2;
int flahVar3;
int flaSpeed;
int samplesInFla;
frameDataStruct frameData;
int runFLAscriptVar0;
int lastNumOfColor;
int lastStartColor;
char flaBuffer[320*200];


struct flaSampleStruct
{
  short int sampleNum;
  short int freq;
  short int repeat;
  char dummy;
  unsigned char x;
  unsigned char y;
};


struct flagDataStruct
{
  short int x;
  short int z;
  short int y;
};

typedef struct flagDataStruct flagDataStruct;


struct FLAheaderStruct
{
  char version[6];
  int numOfFrames;
  char speed;
  char var1;
  short int xsize;  //var2
  short int ysize; //var3
};

typedef struct FLAheaderStruct FLAheaderStruct;
//////////////////////////////////////
type
 tflaheader = record
  id       : array[0..5] of byte;
  nframes  : longint;
  framed   : word;
{  framed2   : byte;   }

  xsize    : word;
  ysize    : word;
 end;
/////////////////////////////
struct frameDataStruct
{
  char videoSize;
  char dummy;
  int frameVar0;
};

typedef struct frameDataStruct frameDataStruct;


*/

#endif
