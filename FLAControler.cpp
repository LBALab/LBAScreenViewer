//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FLAControler.h"
#include "FLALib.h"
#include "ImgLib.h"
#include "LBAScreenViewer_main.h"
#include "FullScreen.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tfrm_controlers *frm_controlers;
extern TFLAheader FLAheader;
extern vector<TFLA> FLA;
extern Graphics::TBitmap *pBitmap;
int tmp = -1;
//---------------------------------------------------------------------------
__fastcall Tfrm_controlers::Tfrm_controlers(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_controlers::BitBtn2Click(TObject *Sender)
{
   if(TrackBar1->Position==FLAheader.numOfFrames-1)
      TrackBar1->Position=0;
   frm_img->Timer1->Enabled = True;   
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_controlers::BitBtn3Click(TObject *Sender)
{
   frm_img->Timer1->Enabled = False;   
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_controlers::BitBtn4Click(TObject *Sender)
{
   frm_img->Timer1->Enabled = False;
   loadPaletteFromPtr(FLA[0].flaPalette);
   frm_controlers->TrackBar1->Position = 0;
   DrawPalette(frm_img->PaintBox2->Canvas);
   MakeLBAImage(FLA[0].flaBuffer, 320,200);
   DrawLBAImage(frm_img->LBAimg->Canvas);
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_controlers::TrackBar2Change(TObject *Sender)
{
   frm_img->Timer1->Interval = TrackBar2->Position;   
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_controlers::TrackBar1Change(TObject *Sender)
{
   loadPaletteFromPtr(FLA[frm_controlers->TrackBar1->Position].flaPalette);
   MakeLBAImage(FLA[frm_controlers->TrackBar1->Position].flaBuffer, 320,200);

 /*      if(tmp != FLA[frm_controlers->TrackBar1->Position].sampleIdx){
        frm_img->MediaPlayer1->Stop();
        frm_img->MediaPlayer1->Close();
    //    frm_img->MediaPlayer1->FileName = ExtractFileDir(Application->ExeName);
        frm_img->MediaPlayer1->FileName = "\\flasample\\"+ FLA[frm_controlers->TrackBar1->Position].sampleIdx+1;
        frm_img->MediaPlayer1->FileName += ".wav";
        ShowMessage(frm_img->MediaPlayer1->FileName);
        frm_img->MediaPlayer1->Open(); //play it
        frm_img->MediaPlayer1->Play();
        tmp = FLA[frm_controlers->TrackBar1->Position].sampleIdx;
      }else  */
  //      tmp = FLA[a].sampleIdx;   */

   if(frm_fullscreen->Showing)
      frm_fullscreen->Image1->Canvas->Draw(0,0,pBitmap);
   else{
      DrawPalette(frm_img->PaintBox2->Canvas);
      DrawLBAImage(frm_img->LBAimg->Canvas);
   }
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_controlers::BitBtn1Click(TObject *Sender)
{
   TrackBar1->Position--;
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_controlers::BitBtn6Click(TObject *Sender)
{
   TrackBar1->Position=0;
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_controlers::BitBtn5Click(TObject *Sender)
{
   TrackBar1->Position++;
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_controlers::BitBtn7Click(TObject *Sender)
{
   TrackBar1->Position = FLAheader.numOfFrames-1;   
}
//---------------------------------------------------------------------------
