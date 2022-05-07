//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FullScreen.h"
#include "LBAScreenViewer_main.h"
#include "ImgLib.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tfrm_fullscreen *frm_fullscreen;
extern AnsiString Path;
extern Graphics::TBitmap *pBitmap;
//---------------------------------------------------------------------------
__fastcall Tfrm_fullscreen::Tfrm_fullscreen(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_fullscreen::PaintBox1Paint(TObject *Sender)
{
   Image1->Canvas->Draw(0,0,pBitmap);
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_fullscreen::FormShow(TObject *Sender)
{
   Image1->Width = pBitmap->Width;
   Image1->Height = pBitmap->Height;   
   Image1->Canvas->FillRect(Rect(0,0,Image1->Width,Image1->Height));
   Image1->Canvas->Draw(0,0,pBitmap);
   frm_fullscreen->WindowState = wsMaximized;
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_fullscreen::FormKeyPress(TObject *Sender, char &Key)
{
   if(StrPos(LowerCase(Path).c_str(),".hqr") || StrPos(LowerCase(Path).c_str(),".ile")){
      if(Key == '+' || Key == '-'){
         if(Key == '+') frm_img->edt_index->Value++;
         else if (Key == '-') frm_img->edt_index->Value--;

         Image1->Canvas->FillRect(Rect(0,0,Image1->Width,Image1->Height));
         LoadImageFromResource(frm_img->edt_index->Value, Path, Image1->Canvas, frm_img->PaintBox2->Canvas, false);
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_fullscreen::Image1ContextPopup(TObject *Sender,
      TPoint &MousePos, bool &Handled)
{
   TMouse *xis = new TMouse();
   frm_img->img_popupmenu->Popup(xis->CursorPos.x,xis->CursorPos.y);
}
//---------------------------------------------------------------------------

