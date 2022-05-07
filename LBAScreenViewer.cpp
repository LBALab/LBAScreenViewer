//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USEFORM("LBAScreenViewer_main.cpp", frm_img);
USEFORM("About.cpp", frm_about);
USEFORM("Settings.cpp", frm_settings);
USEFORM("FullScreen.cpp", frm_fullscreen);
USEFORM("FLAControler.cpp", frm_controlers);
USEFORM("frmBMP2FLA.cpp", frm_BMP2FLA);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
   try
   {
       Application->Initialize();
       Application->Title = "LBA Screen Viewer";
       Application->CreateForm(__classid(Tfrm_img), &frm_img);
       Application->CreateForm(__classid(Tfrm_about), &frm_about);
       Application->CreateForm(__classid(Tfrm_settings), &frm_settings);
       Application->CreateForm(__classid(Tfrm_fullscreen), &frm_fullscreen);
       Application->CreateForm(__classid(Tfrm_controlers), &frm_controlers);
       Application->CreateForm(__classid(Tfrm_BMP2FLA), &frm_BMP2FLA);
       Application->Run();
   }
   catch (Exception &exception)
   {
       Application->ShowException(&exception);
   }
   return 0;
}
//---------------------------------------------------------------------------
