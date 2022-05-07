//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FLAControler.h"
#include "FLALib.h"
#include "ImgLib.h"
#include "LBAScreenViewer_main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrame1 *Frame1;
extern int frameindex;
extern TFLAheader FLAheader;
extern vector<TFLA> FLA;
//---------------------------------------------------------------------------
__fastcall TFrame1::TFrame1(TComponent* Owner)
   : TFrame(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrame1::BitBtn2Click(TObject *Sender)
{
   if(frameindex>=FLAheader.numOfFrames)
      frameindex=0;
   frm_img->Timer1->Enabled = True;
}
//---------------------------------------------------------------------------
void __fastcall TFrame1::BitBtn3Click(TObject *Sender)
{
   frm_img->Timer1->Enabled = False;   
}
//---------------------------------------------------------------------------
void __fastcall TFrame1::BitBtn4Click(TObject *Sender)
{
   frm_img->Timer1->Enabled = False;
   frameindex = 0;
   loadPaletteFromPtr(FLA[0].flaPalette);
   DrawPalette(frm_img->PaintBox2->Canvas);
   MakeLBAImage(FLA[0].flaBuffer, 320,200);
   DrawLBAImage(frm_img->LBAimg->Canvas);
}
//---------------------------------------------------------------------------
void __fastcall TFrame1::TrackBar2Change(TObject *Sender)
{
   frm_img->Timer1->Interval = TrackBar2->Position;
}
//---------------------------------------------------------------------------
